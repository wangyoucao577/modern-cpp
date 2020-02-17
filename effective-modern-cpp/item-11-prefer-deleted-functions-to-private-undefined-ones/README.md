# Item 11. Prefer deleted functions to private undefined ones

## Test Codes

- [delete_normal_functions.cc](./delete_normal_functions.cc)     
  - From below logs, error will occurred as `call to delete funciton xxx` during compile once the `char/bool/double` overloads are deleted.    
  - Note that the `bool is_lucky(double) = delete;` can reject call for both `double` and `float`, because if given a choice between converting a `float` to a `int` or to a `double`, `C++` prefers that conversion to `double`. So calling `is_lucky()` with a `float` will therefore call the `double` overload, not the `int` one.     

```bash
$ g++ -std=c++14 -Wall delete_normal_functions.cc && ./a.out
delete_normal_functions.cc:26:48: warning: implicit conversion from 'double' to 'int' changes value from 3.5 to 3 [-Wliteral-conversion]
    std::cout << "is_lucky(3.5): " << is_lucky(3.5) << std::endl;
                                      ~~~~~~~~ ^~~
delete_normal_functions.cc:27:49: warning: implicit conversion from 'float' to 'int' changes value from 3.5 to 3 [-Wliteral-conversion]
    std::cout << "is_lucky(3.5f): " << is_lucky(3.5f) << std::endl;
                                       ~~~~~~~~ ^~~~
2 warnings generated.
is_lucky(1): 1
is_lucky('a'): 1
is_lucky(true): 1
is_lucky(3.5): 1
is_lucky(3.5f): 1
$ 
$ 
$ g++ -std=c++14 -DDELETE_OVERLOADS delete_normal_functions.cc && ./a.out
delete_normal_functions.cc:24:39: error: call to deleted function 'is_lucky'
    std::cout << "is_lucky('a'): " << is_lucky('a') << std::endl;
                                      ^~~~~~~~
delete_normal_functions.cc:9:6: note: candidate function has been explicitly deleted
bool is_lucky(char) = delete;   // reject call for char
     ^
delete_normal_functions.cc:4:6: note: candidate function
bool is_lucky(int a) {
     ^
delete_normal_functions.cc:10:6: note: candidate function has been explicitly deleted
bool is_lucky(bool) = delete;   // reject call for bool
     ^
delete_normal_functions.cc:16:6: note: candidate function has been explicitly deleted
bool is_lucky(double) = delete; // reject call for double and float
     ^
delete_normal_functions.cc:25:40: error: call to deleted function 'is_lucky'
    std::cout << "is_lucky(true): " << is_lucky(true) << std::endl;
                                       ^~~~~~~~
delete_normal_functions.cc:10:6: note: candidate function has been explicitly deleted
bool is_lucky(bool) = delete;   // reject call for bool
     ^
delete_normal_functions.cc:4:6: note: candidate function
bool is_lucky(int a) {
     ^
delete_normal_functions.cc:9:6: note: candidate function has been explicitly deleted
bool is_lucky(char) = delete;   // reject call for char
     ^
delete_normal_functions.cc:16:6: note: candidate function has been explicitly deleted
bool is_lucky(double) = delete; // reject call for double and float
     ^
delete_normal_functions.cc:26:39: error: call to deleted function 'is_lucky'
    std::cout << "is_lucky(3.5): " << is_lucky(3.5) << std::endl;
                                      ^~~~~~~~
delete_normal_functions.cc:16:6: note: candidate function has been explicitly deleted
bool is_lucky(double) = delete; // reject call for double and float
     ^
delete_normal_functions.cc:4:6: note: candidate function
bool is_lucky(int a) {
     ^
delete_normal_functions.cc:9:6: note: candidate function has been explicitly deleted
bool is_lucky(char) = delete;   // reject call for char
     ^
delete_normal_functions.cc:10:6: note: candidate function has been explicitly deleted
bool is_lucky(bool) = delete;   // reject call for bool
     ^
delete_normal_functions.cc:27:40: error: call to deleted function 'is_lucky'
    std::cout << "is_lucky(3.5f): " << is_lucky(3.5f) << std::endl;
                                       ^~~~~~~~
delete_normal_functions.cc:16:6: note: candidate function has been explicitly deleted
bool is_lucky(double) = delete; // reject call for double and float
     ^
delete_normal_functions.cc:4:6: note: candidate function
bool is_lucky(int a) {
     ^
delete_normal_functions.cc:9:6: note: candidate function has been explicitly deleted
bool is_lucky(char) = delete;   // reject call for char
     ^
delete_normal_functions.cc:10:6: note: candidate function has been explicitly deleted
bool is_lucky(bool) = delete;   // reject call for bool
     ^
4 errors generated.
```

## Notes

## References
- [(Chinese) Understanding C++11 - default_delete_control](https://github.com/wangyoucao577/modern-cpp/tree/master/understanding-cpp11#default_delete_control)
