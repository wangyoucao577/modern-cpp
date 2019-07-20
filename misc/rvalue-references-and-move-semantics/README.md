# Rvalue references and move semantics

## Lifetime of Temporary objects
According to [Move semantics and rvalue references in C++11](https://www.cprogramming.com/c++11/rvalue-references-and-move-semantics-in-c++11.html), [Alex Allain](https://www.cprogramming.com/about.html) says that     
**Notice, by the way, that holding on to a const reference to a temporary object ensures that the temporary object isn't immediately destructed. This is a nice guarantee of C++.**    

Also, [The C++ Programming Language - Fourth Edition](https://anekihou.se/programming/2.%20intermediete.pdf) chapter 6.4.2 Lifetimes of Objects describe the lifetime of **Temporary objects** that:    
**Temporary objects (e.g., intermediate results in a computation or an object used to hold a
value for a reference to const argument): their lifetime is determined by their use. If they
are bound to a reference, their lifetime is that of the reference; otherwise, they ‘‘live’’ until
the end of the full expression of which they are part. A full expression is an expression that
is not part of another expression. Typically, temporary objects are automatic.**    

It means that below codes will work well:    
```c++
std::string getName () {
    return "Alex";
}

{
    // use lvalue-reference to remember the temporary object
    const std::string&  name = getName();         // OK

    // do something with `name`
    // ...
}

{
    // use lvalue-reference to remember the temporary object
    const std::string&& name = getName();         // also OK

    // do something with `name`
    // ...
}

```


## Best practice for pass-by-value or pass-by-const-reference
- Old style     
It used to be generally recommended best practice(i.e. in Scott Meyers, Effective C++) to **use pass by const ref for all types, except for builtin types (`char`, `int`, `double`, etc.), for iterators and for function objects (lambdas, classes deriving from `std::*_function`).**     
- New recommendation with move semantics     
**use pass by value if the function takes ownership of the argument, and if the object type supports efficient moving.**    

See discussion in [Is it better in C++ to pass by value or pass by constant reference?](https://stackoverflow.com/questions/270408/is-it-better-in-c-to-pass-by-value-or-pass-by-constant-reference).    


## Pass an std::function by const-reference or by value
Put simply,    
- pass `std::function` by value if want to take ownership of it and it's movable. Move it again with `std::move()` if necessary.       
- pass `std::function` by const reference if only want to simply call it once.    

See discussion in [Is it better in C++ to pass by value or pass by constant reference?](https://stackoverflow.com/questions/270408/is-it-better-in-c-to-pass-by-value-or-pass-by-constant-reference) and [Should I pass an std::function by const-reference?](https://stackoverflow.com/questions/18365532/should-i-pass-an-stdfunction-by-const-reference).    



## References
- https://github.com/wangyoucao577/modern-cpp/tree/master/understanding-cpp11#rvalue_reference
- [The C++ Programming Language - Fourth Edition](https://anekihou.se/programming/2.%20intermediete.pdf)
- [Move semantics and rvalue references in C++11](https://www.cprogramming.com/c++11/rvalue-references-and-move-semantics-in-c++11.html)
- [Const References to Temporary Objects](https://blog.galowicz.de/2016/03/23/const_reference_to_temporary_object/)
- [Is returning by rvalue reference more efficient?](https://stackoverflow.com/questions/1116641/is-returning-by-rvalue-reference-more-efficient)
- [What's the difference between std::move and std::forward](https://stackoverflow.com/questions/9671749/whats-the-difference-between-stdmove-and-stdforward)
- [谈谈 C++ 中的右值引用](https://liam.page/2016/12/11/rvalue-reference-in-Cpp/)
- [Correct usage of rvalue references as parameters](https://stackoverflow.com/questions/15673889/correct-usage-of-rvalue-references-as-parameters)
- [What is move semantics?](https://stackoverflow.com/questions/3106110/what-is-move-semantics)
- [Is it better in C++ to pass by value or pass by constant reference?](https://stackoverflow.com/questions/270408/is-it-better-in-c-to-pass-by-value-or-pass-by-constant-reference)
- [Can std::function be move-constructed from rvalue reference to a temporary functor object?](https://stackoverflow.com/questions/16639131/can-stdfunction-be-move-constructed-from-rvalue-reference-to-a-temporary-funct)
- [How true is “Want Speed? Pass by value”](https://stackoverflow.com/questions/21605579/how-true-is-want-speed-pass-by-value)
- [Should I pass an std::function by const-reference?](https://stackoverflow.com/questions/18365532/should-i-pass-an-stdfunction-by-const-reference)
- [What is the correct way of using C++11's range-based for?](https://stackoverflow.com/questions/15927033/what-is-the-correct-way-of-using-c11s-range-based-for)

