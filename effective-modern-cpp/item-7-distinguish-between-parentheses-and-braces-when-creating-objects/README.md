# Item 7. Distinguish between () and {} when creating objects

## Test Codes

- most_vexing_parse.cc
```bash
clang++ -std=c++14 most_vexing_parse.cc
most_vexing_parse.cc:22:25: warning: parentheses were disambiguated as a function declaration [-Wvexing-parse]
  TimeKeeper time_keeper(Timer());
                        ^~~~~~~~~
most_vexing_parse.cc:22:26: note: add a pair of parentheses to declare a variable
  TimeKeeper time_keeper(Timer());
                         ^
                         (      )
most_vexing_parse.cc:23:21: error: member reference base type 'TimeKeeper (Timer (*)())' is not a structure or union
  return time_keeper.get_time();
         ~~~~~~~~~~~^~~~~~~~~
1 warning and 1 error generated.
```

- prohibit_narrowing_conversion.cc
```bash
clang++ -std=c++14 prohibit_narrowing_conversion.cc
prohibit_narrowing_conversion.cc:13:14: error: type 'double' cannot be narrowed to 'int' in initializer list [-Wc++11-narrowing]
    int sum3{x + y + z};  // compile error
             ^~~~~~~~~
prohibit_narrowing_conversion.cc:13:14: note: insert an explicit cast to silence this issue
    int sum3{x + y + z};  // compile error
             ^~~~~~~~~
             static_cast<int>( )
1 error generated.
```

## Notes 

### 4 syntaxs to initialize values/objects

| Index | Syntax | Comment | 
|-------|-------|---------|
|1|`int x(0);`|initializer is in parentheses|
|2|`int x = 0;`|initializer follows `=`|
|3|`int x{ 0 };`|initializer is in braces <br>i.e. **Uniform Initialization**|
|4|`int x = {0};`|equal to `int x{ 0 };`|

- For built-in types like `int`, they're no difference in practice.    
- For user-defined types, different function calls may be involved.    

### Benefits of **Uniform Initialization**

| Item | Sample | Able to do by `()` | Able to do by `=` | 
|------|--------|----------------|---------------|
|Initialize container with contents | `// initialize a vector with contents 1, 3 and 5.`<br>`std::vector<int> v{1, 3, 5}; ` | No | No |
|Initialize non-static member | `private:`<br>`  int x{ 0 };` | No | Yes |
|Initialize uncopyable object | `std::atomic<int> a{ 0 };` | Yes | No |
|Prohibit implicit narrowing conversions among built-in types | `double x, y, z;` <br>`int sum1{ x + y + z}; //compile error` | No | No |
|Avoid `C++`'s [Most vexing parse](https://en.wikipedia.org/wiki/Most_vexing_parse)|`Widget w1; // OK, default constructor`<br>`Widget w2(); // ERROR, most vexing parse!`<br>`Widget w3{}; // OK, default constructor` |No| No|

### Drawback of **Uniform Initialization**

- Unusual behavior when `{ xxx }` + `std::initializer_list` + `constructor overload`
  - In constructor calls, `(xxx)` and `{ xxx }` have the same meaning as long as `std::initializer_list` parameters are not involved.    
  - If there's one or more constructors declare a parameter of type `std::intializer_list`, calls using the `{ xxx }` **stronglly prefer** the overloads taking `std::initializer_list`, even sometimes it will lead to compilation error. (go through the amazing samples from the book) 
  - Empty `{}` always means no arguments, not an empty `std::intializer_list`.     

- Significant differences when initialize `std::vector<T>` by `{xxx}` and `(xxx)`

```c++
//use non-std::intializer_list constructor
//i.e. create 10 element std::vector, all elements have value 20
std::vector<int>(10, 20);   

//use std::initializer_list constructor
//i.e. create 2 element std::vector, element values are 10 and 20
std::vector<int>{10, 20};
```

### Conclusion
- As a class author, it's best to design your constructors so that the overload called isn't affected by whether clients use `()` or `{}`. In another word, design your classes to avoid the error in the design of `std::vector`;
- As a class client, choose carefully between `()` and `{}` when creating objects.   
    - Most developers end up choosing one kind of delimiter as a default, using the other only when they have to. E.g. Braces-by-default or go-parentheses-go.     
- If you're a template author, in general it's not possible to know `()` or `{}` should be used.
    - This is precisely the problem faced by the Standard Library function `std::make_unique` and `std::make_shared`. They resolve the problem by internally using `()` and by documenting this decision as part of their interfaces.    

## References
- [Most vexing parse](https://en.wikipedia.org/wiki/Most_vexing_parse)
