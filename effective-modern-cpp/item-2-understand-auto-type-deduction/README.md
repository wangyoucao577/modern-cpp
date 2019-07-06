# Item 2. Understand auto type deductions    

## Test Codes

```bash
$ # TODO: 
```

## Notes

```c++
// When a variable is declared using auto, auto plays the role of T in the template, 
//   and the type specifier for the variable acts as ParamType.

// In this example, 
//   auto plays the role of T, 
//   const auto& acts as ParamType.              
const auto& x = 100;    
```

With only one curious exception, `auto` type deduction is `template` type deduction.    

### Same part with Item 1 (template type deduction)

- Case 1: The type specifier is a pointer or reference, but not a universal reference.    
- Case 2: The type specifier is a universal reference.    
- Case 3: The type specifier is neither a pointer nor a reference.    
- Array Arguments
- Function Arguments    

### Different part

The only real difference between `auto` and `template` type deduction is that `auto` assumes that a braced initializer(i.e. `{ }`) represents a `std::initializer_list`, but `template` type deduction doesn't.    

| Code | `auto`'s Type | Comment |
|------|---------------|---------|
|`auto x = 100;` |`int`||
|`auto x(100);`  |`int`||
|`auto x = {100};`| `std::intializer_list<int>` |See **comment-1**|
|`auto x {100};`  | `std::intializer_list<int>` |See **comment-1**|
|`auto x = {1, 2, 3.0};` | ERROR! | can't deduce `T` for `std::initializer_list<T>`|

**comment-1**: be noted that some compilers may result the type by `int` instead of `std::initializer_list<int>` under [N3922(only proposal, not part of `C++11` or `C++14` standard)](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3922.html).    

|   | `ParamType` | Template Function | `expr` | Deduced `T` | Deduced `ParamType` | Comment |
|---|-------------|-------------------|--------|-------------|---------------------|---------|
|`T`|`T` |`template<typename T>`<br>`void f(T)` |`f({1, 2});`|ERROR!|ERROR!||
|`T`|`std::intializer_list<T>`|`template<typename T>`<br>`void f(std::intializer_list<T>)` |`f({1, 2});`|`int`|`std::intializer_list<int>`|The `std::initializer_list<T>` should be `ParamType` if want to deduce template function by braced initializer(i.e. `{ }`)|


### When use auto as return type or lambda parameter type

- `C++14` permits `auto` to indicate that a function's return type should be deduced, and `C++14` lambdas may use `auto` in parameter declarations.    
- However, these uses of `auto` employ `template` type deduction, not `auto` type deduction.    

## References
- [(Chinese) auto vs. decltype](https://github.com/wangyoucao577/modern-cpp/tree/master/understanding-cpp11#auto_decltype)    
