# Item 3. Understand decltype

## Test Codes

```bash
$ # TODO: 
```

## Notes

- In contrast to what happens during type deduction for templates and `auto`, `decltype` typically parrots back the exact type of the name or expression you give it.     
  - Normally no surprises.    
  - Won't discard `const` or `reference`.     

```c++
// some examples

const int i = 0;   // decltype(i) is const int
const int& j = i;  // decltype(j) is const int&

vector<int> v;     // decltype(v[0]) is int&
```

- In `C++11/14`, perhaps the primary use for `decltype` is declaring function templates where the function's return type depends on its parameter types.    

```c++
// E.g. in C++11, trailing return type syntax 
//      (works, but requires refinement)
template<typename Container, typename Index>
auto func(Container& c, Index i) -> decltype(c[i]){
    return c[i];
}

// E.g. in C++14 
//      (works, but requires refinement)
//    Note that if use `auto` instead of `decltype(auto)`, it also works but they're different:   
//      return `decltype(auto)`: return reference of `c[i]`, which can be revised.
//      return `auto`: return value of `c[i]`, i.e. return by value 
template<typename Container, typename Index>
decltype(auto) func(Container& c, Index i) { 
    return c[i];
}


// Final C++14 version after refinement, FYI.
template<typename Container, typename Index>
decltype(auto) func(Container&& c, Index i) {
    return std::forward<Container>(c)[i];
}

```

- `decltype(auto)` in `C++14`:     
  - `auto` specifies that the type is to be deduced    
  - `decltype` says that `decltype` rules should be used during the deduction.     

- Special case of `decltype`:    
  - When applying `decltype` to `lvalue expressions`, the type reported is an `lvalue reference`.    
    - That is, if an `lvalue expression` has type `T`, `decltype` reports that type as `T&`.    
  - The primary lesson is to pay very close attention when using `decltype(auto)`.    

```c++
// E.g. explaining
int x = 0;  //     decltype(x) is int
            // but decltype((x)) is int& ! (wrapping the name `x` in paranetheses - `(x)` )
            // Because `x` is an lvalue, 
            //   and `C++` defines the expression `(x)` to be an lvalue too, 
            //   so that `(x)` is a lvalue expression here.    


decltype(auto) func1(){
    int x = 0;
    //...
    return x;   // decltype(x) is int, so func1 returns int
}

decltype(auto) func2(){
    int x = 0;
    //...
    return (x);   // decltype((x)) is int&, so func1 returns int& !
                  // ERROR: one more error is funcs returns a reference of local variable!!
}


```

## References
- [(Chinese) auto vs. decltype](https://github.com/wangyoucao577/modern-cpp/tree/master/understanding-cpp11#auto_decltype)    
