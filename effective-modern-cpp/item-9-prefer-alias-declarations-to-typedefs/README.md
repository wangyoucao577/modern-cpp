# Item 9. Prefer alias declarations to typedefs

## Test Codes


## Notes

### Normally `typedef` and alias declaration do exactly same thing

```c++
    //for types
    typedef std::unique_ptr<std::unordered_map<std::string, std::string>> UPtrMapSS;
    using UPtrMapSS = std::unique_ptr<std::unordered_map<std::string, std::string>>; // same meaning as above

    //function pointers
    typedef void(*FP)(int, const std::string&);
    using FP = void(*)(int, const std::string&); // same meaning as above 
```

### A compelling reason to prefer alias declarations to typedefs: templates.     
In particular, alias declarations maybe templatized (in which case they're called **alias templates**), while typedefs cannot.      

- sample by `typedef`

```c++
// typedef
template<typename T>
struct MyAllocList {    // must define a struct template
    typedef std::list<T, MyAlloc<T>> type;
};

MyAllocList<Widget>::type lw;   // client code 1, plus `::type` is mandatory

template<typename T>
class Widget{
public:
    // Here, `MyAllocList<T>::type` refers to a type that's dependent on a template type parameter `T`.     
    // `MyAllocList<T>::type` is thus a dependent type, 
    //    and one of `C++`'s many endearing rules is that the names of dependent types must be preceded by `typename`. 
    typename MyAllocList<T>::type list;   // client code 2, both `typename` and `::type` are mandatory.
}
```

- sample by **alias declarations/templates**    
```c++
// alias declaration, aka alias template
template<typename T>
using MyAllocList = std::list<T, MyAlloc<T>>;

MyAllocList<Widget> lw;     // client code 1

template<typename T>
class Widget{
public:
    MyAllocList<T> list;      // client code 2, not extra `typename` and `::type`
}

```

### `C++14` offers alias templates for all `C++11` type traits transformations 

- `C++11` type traits are implemented as nested `typedef`s inside templatized structs, so `::type` have to at the end of each use.     
- Because the Standardization Committee belatedly recognized that alias templates are the better way to go, they included such templates in `C++14` for all the `C++11` type transformations. These aliases have a common form:          
  - for each `C++11` transformation `std::transformation<T>::type`, there's a corresponding `C++14` alias template named `std::transformation_t`.    

```c++
    //examples 

    std::remove_const<T>::type      //C++11: const T -> T
    std::remove_const_t<T>          //C++14: equivalent

    std::remove_reference<T>::type  //C++11: T&/T&& -> T
    std::remove_reference_t<T>      //C++14: equivalent

    std::add_lvalue_reference<T>::type  //C++11: T -> T&
    std::add_lvalue_reference_t<T>      //C++14: equivalent

```

- Even if you don't have access to `C++14`, writing the alias templates yourself instead of use `C++11` type traits directly:    

```c++
    template<class T>
    using remove_const_t = typename std::remove_const<T>::type;

    template<class T> 
    using remove_reference_t = typename std::remove_reference<T>::type;

    template<class T>
    using add_lvalue_reference_t = typename std::add_lvalue_reference<T>::type;
```


## References
- [Standard library header <type_traits>](https://en.cppreference.com/w/cpp/header/type_traits)
- [(Chinese) Understanding C++11 - using_alias](https://github.com/wangyoucao577/modern-cpp/tree/master/understanding-cpp11#using_alias)