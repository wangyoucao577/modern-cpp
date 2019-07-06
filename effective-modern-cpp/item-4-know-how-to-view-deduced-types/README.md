# Item 4. Know how to view deduced types

## Test Codes

- Compiler Diagnostics     

```bash
# simple example
# my compiler g++(clang++) reports `int` and `const int*` which are deduced types of `x` and `y`
$ g++ -std=c++14 compiler_diagnostics.cc
compiler_diagnostics.cc:17:21: error: implicit instantiation of undefined template 'TD<int>'
    TD<decltype(x)> x1;
                    ^
compiler_diagnostics.cc:7:7: note: template is declared here
class TD;
      ^
compiler_diagnostics.cc:18:21: error: implicit instantiation of undefined template 'TD<const int *>'
    TD<decltype(y)> y1;
                    ^
compiler_diagnostics.cc:7:7: note: template is declared here
class TD;
      ^
2 errors generated.
```

```bash
# complex example
# my compile g++(clang++) reports `const Widget* const&` for `ParamType` and `const Widget*` for `T`
$ g++ -std=c++14 -DCOMPILER_DIAGNOSTICS  complex_sample.cc
In file included from complex_sample.cc:2:
./complex_sample.hpp:12:25: error: implicit instantiation of undefined template 'TD<const Widget *const &>'
    TD<decltype(param)> t;
                        ^
complex_sample.cc:20:9: note: in instantiation of function template specialization 'f<const Widget *>' requested here
        f(&vw[0]);
        ^
./compiler_diagnostics.hpp:6:7: note: template is declared here
class TD;
      ^
1 error generated.
```

- Runtime output

```bash
# complex example
$ g++ -std=c++14 -DBOOST_SUPPORT complex_sample.cc && ./a.out 
-- reports by std:
typeid(T).name(): PK6Widget
typeid(decltype(param)).name(): PK6Widget
std::is_const<T>(): 0, std::is_reference<T>(): 0
std::is_const<decltype(param)>(): 0, std::is_reference<decltype(param)>(): 1

-- reports by boost::type_index:
boost::typeindex::type_id_with_cvr<T>().pretty_name(): Widget const*
boost::typeindex::type_id_with_cvr<decltype(param)>().pretty_name(): Widget const* const&

$ c++filt -t PK6Widget
Widget const*
```

## Notes 

- Three possibilities:    
  - getting type deduction information as you edit your code     
    - (i.e. by IDE, e.g. `Visual Studio`, `XCode`, etc.)    
  - getting it during compilation    
    - A very useful way!     
  - getting it at runtime    
    - `typeid()` will lose `const/reference/volatile` since it's pass-by-value        
      - `std::is_const<>` and `std::is_reference<>` will help    
    - `boost::typeindex::type_id_with_cvr<>` works    

## References
- [(Chinese) auto vs. decltype](https://github.com/wangyoucao577/modern-cpp/tree/master/understanding-cpp11#auto_decltype)    

