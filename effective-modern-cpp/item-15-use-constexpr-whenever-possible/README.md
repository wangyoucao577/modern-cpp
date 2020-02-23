# Item 15. User constexpr whenever possible

## Test Codes

- [pow.cc](./pow.cc)

```bash
$ g++ -std=c++11 pow.cc&&./a.out
C++ 201103
pow(3,10)=59049
$ 
$ g++ -std=c++14 pow.cc&&./a.out
C++ 201402
pow(3,10)=59049
```

- [point.cc](./point.cc)

```bash
$ g++ -std=c++11 point.cc && ./a.out
p1(9.4,27.7)
p2(28.8,5.3)
mid point(19.1,16.5)
$ 
$ g++ -std=c++14 point.cc && ./a.out
p1(9.4,27.7)
p2(28.8,5.3)
mid point(19.1,16.5)
rp1(-9.4,-27.7)
```

## Notes
Conceptually, `constexpr` indicates a value that's not only constant, it's known during compliation.    

### `constexpr` objects 
`constexpr` objects are `const`, which have values that are known at compile time(technically compiling or linking).     
- Values known during compilation are privileged.     
  - E.g. they may be placed in read-only memory.     
- Of broader applicability is that integral values that are constant and known during compilation can be used in contexts where `C++` requires an **integral constant expression**.     
  - Such contexts include specification of array sizes, integral template arguments(including lengths of `std::array` objects), enumerator values, alighment specifiers, and more.     

```c++
    const int sz = 10; 
    std::array<int, sz> data1;  // error! sz's value not known at compilation.

    constexpr auto sz2 = 10;    
    std::array<int, sz2> data2; // fine! 
```

- All `constexpr` objects are `const`, but not all `const` objects are `constexpr`.     

### `constexpr` functions
`constexpr` functions **produce compile-time constants when they're called with compile-time constants. If they're called with values not known until runtime, they produce runtime values**.     
- The right way to view it:     
  - `constexpr` functions can be used in context that demand compile-time constants.     
    - If the values of the arguments you pass to a `constexpr` function in such a context are known during compilation, the result will be computed during compilation.      
    - If any of the arguments' values is not known during compilation, your code will be rejected.     
  - When a `constexpr` function is called with one or more values that are not known during compilation, it acts like a normal function, computing its result at runtime.     
    - This means you don't need two functions to perform the same operation, one for compile-time constants and one for all other values. The `constexpr` function does it all.    
- `constexpr` functions implementation restructions
  - Because `constexpr` functions must be able to return compile-time results when called with compile-time values, restrictions are imposed on their implementations.     
  - In `C++11`, `constexpr` functions may contain no more than a single executable statement: a `return`.(`C++14` doesn't have this limitation.) Refer to [pow.cc](./pow.cc) for full code.          

  ```c++
  constexpr 
  int pow(int base, int exp) noexcept {
  #if __cplusplus >= 201402L
      auto result = 1;
      for (int i = 0; i < exp; ++i){
          result *= base;
      }
      return result;
  #else
      // C++11 requires one single return
      return exp <= 0 ? 1 : base * pow(base, exp - 1);
  #endif
  }
  ```

  - `constexpr` functions are limited to taking and returning literal types, which essentially means types that can have values determined during compilation.     
    - In `C++11`, all built-in types except `void` qualify, but user-defined types may be literal, too, because constructors and other member functions may be `constexpr`. `C++14` also allows returning `void`.      
    - In `C++11`, `constexpr` member functions are implicitly `const`. `C++14` doesn't have this convention.     
    - Refer to [point.cc](./point.cc) for full code.       
    ```c++
    class Point{
    public:
        constexpr Point(double x = 0, double y = 0) noexcept
        : x_(x), y_(y) {}
        
        constexpr double x() const noexcept { return x_; }
        constexpr double y() const noexcept { return y_; }

    #if __cplusplus >= 201402L
        constexpr
    #endif
        void setX(double x) noexcept { x_ = x; }
    #if __cplusplus >= 201402L
        constexpr
    #endif
        void setY(double y) noexcept { y_ = y; }

    private:
        double x_, y_;
    };
    ```

### Conclution

- Both `constexpr` objects and `constexpr` functions can be employed in a wider range of contexts than non-`constexpr` objects and functions.    
- By using `constexpr` whenever possible, you maximize the range of situations in which your objects and functions may be used.    
- By using `constexpr`, the traditionally fairly strict line between work done during compilation and work done at runtime begins to blur:      
  - Some computations traditionally done at runtime can migrate to compile time.    
  - The more code taking part in the migration, the faster your software will run.(Compilation may take longer, however.)    
- It's important to note that `constexpr` is part of an object's or function's interface.     
  - `constexpr` proclaims "I can be used in a context where `C++` requires a constant expression."      
  - Part of "whenever possible" in "Use `constexpr` whenever possible" is your willingness to make a long-term commitment to the constraints it imposes on the objects and functions you apply it to.     

## References
- [(Chinese) Understanding C++11 - constexpr](https://github.com/wangyoucao577/modern-cpp/tree/master/understanding-cpp11#constexpr)
