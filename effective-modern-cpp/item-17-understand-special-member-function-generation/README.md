# Item 17. Understand special member function generation

## Test Codes

## Notes
In official `C++` parlance, the **special member functions** are the ones that `C++` is willing to generate on its own.    

### `C++98`'s rule     
- `C++98` has four such functions:     
  - the default constructor
  - the destructor
  - the copy constructor
  - the copy assignment operator           
- These functions are generated only if they're needed, i.e., if some code uses them without their being expressly declared in the class.    
  - A default constructor is generated only if the class declares no constructor at all.    
- Generated **special member functions** are implicitly `public` and `inline`, and they're non`virtual` unless the function in question is a destructor in a derived class inheriting from a base class with a `virtual` destructor.    
- **Rule of Three**
  - if you declare any of a copy constructor, copy assignment operator, or destructor, you should declare all three.    

### In `C++11`
- As of `C++11`, the **special member functions** club has two more inductees:     
  - the move constructor    
  - the move assignment operator    
- As is the case with the copy operations, the move operations aren't generated if you declare them yourself. However, the precise conditions under which they're generated differ a bit from those for copy operations.    
  - The two copy operations are independent: declaring one doesn't prevent compilers from generating the other.    
  - The two move operations are NOT independent: if you declare either, that prevents compilers from generating the other.    
  - Futhermore, move operations won't be generated for any class that explicitly declares a copy operation.    
  - Declaring a move operation(construction or assignment) in a class causes compilers to disable the copy operations.      
  - The **Rule of Three** motivates the fact that `C++11` does not generate move operations for a class with a user-declared destructor.     
- So move operations are generated for classes(when needed) only if these things are true:    
  - No copy operations are declared in the class.    
  - No move operations are declared in the class.    
  - No destructor is declared in the class.    
- At some point, analogous rules may be extended to the copy operations, because `C++11` deprecates the automatic generation of copy operations for classes declaring copy operations or a destructor.    
- `C++11` allows `=default` to explicitly have default behavior. This approach is often useful in polymorphic base classes.     

```c++
class Base {
public:
    virtual ~Base() = default;  // let compiler generate it but virtual

    Base(Base&&) = default;
    Base& operator=(Base&&) = default;

    Base(const Base&) = default;
    Base& operator=(const Base&) = default;
}
```

- `C++11` rules summary    
  - Default constructor     
    - same rules as `C++98`.     
    - Generate only if the class contains no user-declared constructors.    
  - Destructor     
    - Essentially same rules as `C++98`;     
    - sole difference is that destructors are `noexcept` by default(see [Item 14](../item-14-declare-functions-noexcept-if-they-wont-emit-exceptions/)).     
    - As in `C++98`, `virtual` only if a base class destructor is `virtual`.    
  - Copy constructor 
    - Same runtime behavior as `C++98`: memberwise copy construction of non-`static` data members.    
    - Generated only if the class lacks a user-declared copy construtor.    
    - Deleted if the class declares a move operation.    
    - Generation of this function in a class with a user-declared copy assignment operator or destructor is deprecated.    
  - Copy assignment operator    
    - Same runtime behavior as `C++98`: memberwise copy assignment of non-`static` data members.     
    - Generated only if the class lacks a user-declared copy assignment operator.    
    - Deleted if the class declares a move operation.    
    - Generation of this function in a class with a user-declared copy constructor or destructor is deprecated.    
  - Move constructor and move assignment operator    
    - Each performs memberwise moving of non-`static` data members.    
    - Generated only if the class contains no user-declared copy operations, move operations, or destructor.    
- Note that there's nothing in the rules about the existence of a member function `template` preventing compilers from generating the **special member functions**s.    
  - For example, compilers will still generate copy and move operations for `Widget`, even though these `template`s could be instantiated to produce the signatures for the copy constructor and copy assignment operator.(That would be the case when `T` is `Widget`.)      
```c++
class Widget {
public:
    // compilers will still generate copy and move operations in this case, 
    // even though these templates could be instantiated to produce 
    //    the signatures for the copy constructor and copy assignment operators. 
    // (that would be the case when T is Widget.)

    template<typename T>
    Widget(const T&);

    template<typename T>
    Widget& operator=(const T&);
}
```

## References
- [(Chinese) Understanding C++11 - default_delete_control](https://github.com/wangyoucao577/modern-cpp/tree/master/understanding-cpp11#default_delete_control)
- [Rule of three (C++ programming)](https://en.wikipedia.org/wiki/Rule_of_three_(C%2B%2B_programming))
- [Special member functions](https://en.wikipedia.org/wiki/Special_member_functions)