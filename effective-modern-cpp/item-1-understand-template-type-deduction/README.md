# Item 1. Understand template type deductions    

## Test Codes

```bash
# compile and run test codes
$ cd item-1-understand-template-type-deduction
$ g++ -std=c++14 main.cc && ./a.out
```

## Notes

```c++
template<typename T>
void f(ParamType param);     // ParamType may different from `T`, e.g. `const T&`

// `T` and `ParamType` will be deduced by `expr`.
// `expr` is Argument. 
f(expr);         
```

The type deduced for `T` is dependent not just on the type of `expr`, but also on the form of `ParamType`.    

### Case 1: `ParamType` is a Reference or Pointer, but not a Universal Reference      
- If `expr`'s type is a reference(or pointer), ignore the reference part(or pointer part).    
- Then pattern-match `expr`'s type against `ParamType` to determin `T`.    

|   | `ParamType` | Template Function | `expr` | Deduced `T` | Deduced `ParamType` | Comment |
|---|-------------|-------------------|--------|-------------|---------------------|---------|
|`T`|`T&`  |`template<typename T>`<br>`void f(T&)`|`int x = 100;`<br>`f(x);`  |`int` |`int&`||
||||`const int x = 100;`<br>`f(x);` |`const int` |`const int&`|`const` will be part of `T`.|
||||`int& x = 100;`<br>`f(x);`      |`int`       |`int&`||
||||`const int& x = 100;`<br>`f(x);`|`const int` |`const int&`|`const` will be part of `T`.|
|`T`|`const T&`  |`template<typename T>`<br>`void f(const T&)`|`int x = 100;`<br>`f(x);`  |`int` |`const int&`||
||||`const int x = 100;`<br>`f(x);` |`int`       |`const int&`||
||||`int& x = 100;`<br>`f(x);`      |`int`       |`const int&`||
||||`const int& x = 100;`<br>`f(x);`|`int`       |`const int&`||
|`T`|`T*`  |`template<typename T>`<br>`void f(T*)`|`int x = 100;`<br>`f(&x);` |`int`|`int*`||
||||`const int x = 100;`<br>`f(&x);`|`const int` |`const int*`|`const` will be part of `T`.|
||||`const int* const y = &x;`<br>`f(y);`|`const int` |`const int*`|`const` will be part of `T`.|
|`T`|`const T*`  |`template<typename T>`<br>`void f(const T*)`|`int x = 100;`<br>`f(&x);` |`int`|`const int*`||
||||`const int x = 100;`<br>`f(&x);`|`int` |`const int*`||
||||`const int* const y = &x;`<br>`f(y);`|`int` |`const int*`||

### Case 2: `ParamType` is a Universal Reference    
- If `expr` is an `lvalue`, both `T` and `ParamType` are deduced to be `lvalue references`.    
  - First, it's the only situation in template type deduction where `T` is deduced to be a reference.   
  - Second, although `ParamType` is declared using the syntax for an `rvalue reference`, its deduced type is an `lvalue reference`.    
- If `expr` is an rvalue, the normal(i.e. Case 1) rules apply.    

|   | `ParamType` | Template Function | `expr` | Deduced `T` | Deduced `ParamType` | Comment |
|---|-------------|-------------------|--------|-------------|---------------------|---------|
|`T`|`T&&`  |`template<typename T>`<br>`void f(T&&)`|`int x = 100;`<br>`f(x);`  |`int&` |`int&`||
||||`const int x = 100;`<br>`f(x);` |`const int&` |`const int&`|`const` will be part of `T`.|
||||`int& x = 100;`<br>`f(x);`      |`int&`       |`int&`||
||||`const int& x = 100;`<br>`f(x);`|`const int&` |`const int&`|`const` will be part of `T`.|
||||`int x = 100;`<br>`f(std::move(x));`|`int` |`int&&`||
||||`const int x = 100;`<br>`f(std::move(x));`|`const int` |`const int&&`|`const` will be part of `T`.|
||||`f(100);`|`int` |`int&&`||


### Case 3: `ParamType` is neither a Pointer nor a Reference    
-  I.e. pass by value
- As before, it `expr`'s type is a reference, ignore the reference part.    
- If, after ignoring `expr`'s reference-ness, `expr` is `const`, ignore that, too. If it's `volatile`, also ignore that.     

|   | `ParamType` | Template Function | `expr` | Deduced `T` | Deduced `ParamType` | Comment |
|---|-------------|-------------------|--------|-------------|---------------------|---------|
|`T`|`T`  |`template<typename T>`<br>`void f(T)`|`int x = 100;`<br>`f(x);`  |`int` |`int`||
||||`const int x = 100;`<br>`f(x);` |`int`|`int`|ignore `const`|
||||`const int& x = 100;`<br>`f(x);` |`int`|`int`|ignore both `reference` and `const`|
||||`const int&& x = 100;`<br>`f(x);` |`int`|`int`|ignore both `reference` and `const`|
||||`int* y = &x;`<br>`f(y);`|`int*`|`int*`||
||||`const int* y = &x;`<br>`f(y);` |`const int*`|`const int*`|`const` will NOT be ignored for pointer|
||||`const int* const y = &x;`<br>`f(y);` |`const int*`|`const int*`|`const` will NOT be ignored for pointer|

### Array Arugments    

|   | `ParamType` | Template Function | `expr` | Deduced `T` | Deduced `ParamType` | Comment |
|---|-------------|-------------------|--------|-------------|---------------------|---------|
|`T`|`T` |`template<typename T>`<br>`void f(T)` |`const char name[] = "J. P. Briggs";`<br>`f(name);`|`const char*` |`const char*`|same as `const char* name="J. P. Briggs";`|
|`T`|`T` |`template<typename T>`<br>`void f(T*)` |`const char name[] = "J. P. Briggs";`<br>`f(name);`|`const char` |`const char*`|same as `const char* name="J. P. Briggs";`|
|`T`|`T` |`template<typename T>`<br>`void f(T&)` |`const char name[] = "J. P. Briggs";`<br>`f(name);`|`const char[13]` |`const char(&)[13]`||

### Function Arguments    

|   | `ParamType` | Template Function | `expr` | Deduced `T` | Deduced `ParamType` | Comment |
|---|-------------|-------------------|--------|-------------|---------------------|---------|
|`T`|`T` |`template<typename T>`<br>`void f(T)` |`void some_func(int, double);`<br>`f(some_func);`|`void(*)(int,double)` |`void(*)(int,double)`|will be function pointer|
|`T`|`T*` |`template<typename T>`<br>`void f(T*)` |`void some_func(int, double);`<br>`f(some_func);`|`void(int,double)` |`void(*)(int,double)`||
|`T`|`T&` |`template<typename T>`<br>`void f(T&)` |`void some_func(int, double);`<br>`f(some_func);`|`void(int,double)` |`void(&)(int,double)`|will be function reference.|


