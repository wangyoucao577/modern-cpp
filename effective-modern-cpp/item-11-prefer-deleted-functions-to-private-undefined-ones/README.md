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

- [delete_template_instantiations.cc](./delete_template_instantiations.cc)

```bash
$ g++ -std=c++14 delete_template_instantiations.cc && ./a.out
void processPointer(T *) [T = int]
void processPointer(T *) [T = void]
void processPointer(T *) [T = char]
void processPointer(T *) [T = const void]
void processPointer(T *) [T = const char]
$ 
$ 
$ g++ -std=c++14 -DDELETE_OVERLOADS delete_template_instantiations.cc && ./a.out
delete_template_instantiations.cc:29:5: error: call to deleted function 'processPointer'
    processPointer(v);
    ^~~~~~~~~~~~~~
delete_template_instantiations.cc:12:6: note: candidate function [with T = void] has been implicitly deleted
void processPointer<void>(void*) = delete;
     ^
delete_template_instantiations.cc:30:5: error: call to deleted function 'processPointer'
    processPointer(c);
    ^~~~~~~~~~~~~~
delete_template_instantiations.cc:14:6: note: candidate function [with T = char] has been implicitly deleted
void processPointer<char>(char*) = delete;
     ^
delete_template_instantiations.cc:31:5: error: call to deleted function 'processPointer'
    processPointer(cv);
    ^~~~~~~~~~~~~~
delete_template_instantiations.cc:16:6: note: candidate function [with T = const void] has been implicitly deleted
void processPointer<const void>(const void*) = delete;
     ^
delete_template_instantiations.cc:32:5: error: call to deleted function 'processPointer'
    processPointer(cc);
    ^~~~~~~~~~~~~~
delete_template_instantiations.cc:18:6: note: candidate function [with T = const char] has been implicitly deleted
void processPointer<const char>(const char*) = delete;
     ^
4 errors generated.
```

## Notes

### Preventing clients calling some `C++` automatically generated functions
The problem arises when you want to prevent clients calling some `C++` automatically generated functions(i.e. default constructor, copy constructor, etc.).      

- `C++98` approach: declare them `private` and not define them
  - declaring these functions `private` prevents clients from calling them
  - not define them so that if code that still has access to them(i.e. member functions or `friend`s of class) uses them, linking will fail due to missing function definitions.   

```c++
    template<class charT, class traits = char_traits<charT> >
    class basic_ios : public ios_base { 
    public:
        // ... 
    
    private:
        basic_ios(const basic_ios&);            // not defined
        basic_ios& operator=(const basic_ios&); // not defined
    }
```

- `C++11` approach: use `= delete`
  - deleted functions may not be used in any way, so **even code that's in member and `friend` functions will fail to compile** if it tries to copy `basic_ios` objects. 
    - that's an improvement over the `C++98` behavior, where such improper usage wouldn't be diagnosed until link-time.    
  - by convention, deleted functions are declared `public`, not `private`.    
    - because making the new functions `public` will generally result in better error messages.    
      - when client codes tries to use a member function, `C++` checks accessibility before deleted status. When client codes tries to use a deleted `private` function, some compilers complain only about the function being `private`, even though the function's accessibility doesn't really affect whether it can be used.      

```c++
    template<class charT, class traits = char_traits<charT> >
    class basic_ios : public ios_base { 
    public:
        basic_ios(const basic_ios&) = delete;            
        basic_ios& operator=(const basic_ios&) = delete; 
        // ... 
    }
```

### More advantages of `C++11` deleted functions
`C++98` approach doesn't support these advantages.      

- any function may be deleted, while only member functions maybe `private`    
refer to [delete_normal_functions.cc](./delete_normal_functions.cc) in [Test Codes](#test-codes).    

- deleted functions can prevent(`private` member functions can't) use of template instantiations that should be disabled     
refer to [delete_template_instantiations.cc](./delete_template_instantiations.cc) in [Test Codes](#test-codes).    

- possible to delete function template inside a class
  - `C++98`'s approach can not do it because template specializations much be written at namespace scope, not class scope.    

```c++
    class Widget {
    public: 
    // ...

    template<typename T>
    void processPointer(T* ptr){ 
        // ... 
    }

    private:
    template<>
    void processPointer<void>(void*);    // C++98 approach, error! 

    };

    template<>
    void Widget::processPointer<void>(void*) = delete;  // C++11 approach, OK!
```

### Conclusion
The truth is that the `C++98` practice of declaring functions `private` and not defining them was really an attempt to achieve what `C++11`'s deleted functions actually accomplish. As an emulation, the `C++98` approach is not as good as the real thing. It doesn't work outside classes, it doesn't always work inside classes, and when it does work, it may not work until link-time. So stick to deleted functions.     

## References
- [(Chinese) Understanding C++11 - default_delete_control](https://github.com/wangyoucao577/modern-cpp/tree/master/understanding-cpp11#default_delete_control)
