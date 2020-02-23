# Item 14. Declare functions noexcept if they won't emit exceptions

## Test Codes

## Notes

During work on `C++11`, a consensus emerged that **the truly meaningful information about a function's exception-emitting behavior was whether it had any**.     
- I.e. **black or white, either a function might emit an exception or it guaranteed that it wouldn't**.    
- This maybe-or-never dichotomy forms the basis of `C++11`'s exception specifications, which essentially replace `C++98`'s. (`C++98`-style exception specifications, `throw()`, remain valid, but they're deprecated.)    
- In `C++11`, unconditional `noexcept` is for functions that guarantee they won't emit exceptions.         

### Advantages 
- Whether a function should be so declared with `noexcept` is a matter of interface design.    
  - Whether a function is `noexcept` is as important a piece of imformation as whether a member function is `const`.    
  - Failure to declare a function `noexcept` when you know that it won't emit an exception is simply poor interface specification.    

- Applying `noexcept` to functions **permits compilers to generate better object code**.       
  - If, at runtime, an exception leaves `f`, `f`'s exception specification is violated.     
    - With the `C++98` exception specification, the call stack is unwound to `f`'s caller, and, after some actions not relevant here, program execution is terminated.     
    - With the `C++11` exception specification, runtime behavior is slightly different: the stack is only possibly unwound before program execution is terminated.    
  - The difference between unwinding the call stack and possibly unwinding it has a surprisingly large impact on code generation.    
    - In a `noexception` function, optimizers need not keep the runtime stack in an unwindable state if an exception would propagate out of the function, nor must they ensure that objects in a `noexcept` function are destoryed in the inverse order of construction should an exception leave the function.    
    - Functions with `throw()` exception specifications lack such optimization flexibility, as do functions with no exception specification at all.    
```c++
    int f(int x);           // no guarantee for exceptions. less optimizable

    int f(int x) throw();   // no exceptions from f: C++98 style. less optimizable

    int f(int x) noexcept;  // no exceptions from f: C++11 style. most optimizable
```

- Declare `noexcept` possible to improve performance     
  - In `C++11`, the copy operations inside `std::vector::push_back` can be silently replaced with moves unless it's known that the move operations won't emit exceptions.     
    - The copy operations inside `std::vector::push_back` occurs when capacity is insufficient for a new element.     
    - In that case, having moves replace copies would be safe, and the only side affect would be improved performance.     
    - `std::vector::push_back` takes advantage of this "move if you can, but copy if you must" strategy, and it's not the only function in the Standard Library that does(e.g., `std::vector::reserve`, `std::deque::insert`, etc.)      
    - They check to see if the operation is declared `noexcept`.     
  - `swap` functions comprise another case where `noexcept` is particularly desirable.     
    - Whether `swap`s in the Standard Library are `noexcept` is sometimes dependent on whether user-defined `swap`s are `noexcept`.     
    - The fact that swapping higher-level data structures can generally be `noexcept` only if swapping their lower-level constituents is `noexcept` should motivate you to offer `noexcept` `swap` functions whenever you can.     

```c++
    // These functions are conditionally noexcept: 
    //    whether they're noexcept depends on whether the expressions inside the noexcept clauses are noexcept. 

    template<class T, size_t N>
    void swap(T (&a)[N], T (&b)[N]) noexcept(noexcept(swap(*a, *b)));

    template<class T1, class T2>
    struct pair {
        void swap(pair& p) noexcept(noexcept(swap(first, p.first)) && 
                                    noexcept(swap(second, p.second)));
        //...
    }
```

### Make sure correctness
- Declare a function `noexcept` only if you're willing to commit to a `noexcept` implementation over the long term.     
- The fact of the matter is that most functions are **exception-neutral**.     
  - Such functions throw no excetpions themselves, but functions they call might emit one.    
  - **Exception-netural** functions are never `noexcept`, because they may emit such "just passing through" exceptions.     
- Some functions, however, have natural implementations that emit no exceptions, and for a few more - notably the move operations and `swap` - being `noexcept` can have such a significant payoff, it's worth implementing them in a `noexcept` manner if at all possible.    
  - But if a straightforward function implementation might yield exceptions(e.g., by invoking a function that might throw), the hoops you'll jump through to hide that from callers(e.g., catching all exceptions and replacing them with status codes or speical return values) will not only complicate your function's implementation, it will typically complicate code at call sites, too. That'd be poor software engineering.       

- For some functions, being `noexcept` is so important, they're that way by default.     
  - In `C++11`, by default, **all memory deallocation functions and all destructors - both user-defined and compiler-generated - are implicitly `noexcept`**.    
    - (There's thus no need to declare them `noexcept`. Declaring them `noexcept` explicity doesn't hurt anything, it's just unconventional.)     
  - The only time a destructor is not implicity `noexcept` is when a data member of the class(including inherited members and those contained inside other data members) is of a type that expressly states that its destructor may emit exceptions(e.g., declares it `noexcept(false)`).    
    - Such destructors are uncommon.     
    - There're none in the Standard Library, and if the destructor for an object being used by the Standard Library(e.g., because it's in a container or was passed to an algorithm) emits an exception, the behavior of the program is undefined.     

- **wide contract** vs. **narrow contract**    
  - It's worth noting that some library interface designers distinguish functions with **wide contract**s from those with **narrow contract**s.    
    - A function with a **wide contract** has no preconditions(e.g., it imposes no constraints on the arguments that callers pass it.).     
      - It never exhibit undefined behavior.         
      - Easy to follow above advices and declaring it `noexcept`.    
    - Functions without **wide contract** have **narrow contract**s.    
      - If a precondition is violated, results are undefined.       
      - Sometimes may need to throw exception if precondition violated. If so, it can't be declared with `noexcept`.      


```c++
    // E.g., the f is wide contract and we know thiat t will never emit exception, so declare it `noexcept` will good to go.    
    void f(const std::string& s) noexcept;  

    // But if f requires precondition: s.length() <= 32, then it's narrow contract. 
    // Possibly a precondition checking will at the beginning of f, and hope to throw a "precondition was violated" exception if it happens.     
    void f(const std::string& s) ;  
```

## References
- [(Chinese) Understanding C++11 - noexcept_test](https://github.com/wangyoucao577/modern-cpp/tree/master/understanding-cpp11#noexcept_test)
