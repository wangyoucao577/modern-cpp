# Item 10. Prefer scoped enums to unscoped enums

## Test Codes

## Notes
- scoped enum: aka **strong typed enum** or **enum class**, `C++11` style `enum class` or `enum struct`.     
- unscoped enum: `C++98` style `enum`.    

### Scoped enum vs. Unscoped enum

| | Scoped enum | Unscoped enum |
|-|-------------|---------------|
|**namespace pullution**<br>I.e. enumerator names leak into the scope containing their enm definition.| No | Yes |
|**strongly typed** | Yes<br>No implicit conversions from enumerators in a scoped enum to any other type.<br>Use `static_cast<>` if really need to convert.| No<br>Enumerators implicity convert to integral types.|
|**specify underlying type**| Yes | Yes |
|**default underlying type**| `int` | Not have, decide by compiler |
|**forward-declaration**| Yes | Yes, requires specify underlying type|

About the **forward-declaration**,     
- To make efficient use of memory, compilers often want to choose the smallest underlying type for an `enum` that's sufficient to represent its range of enumerator values.    
  - To make that possible, `C++98` supports only `enum` definition(where all enumerators are listed); `enum` declarations are not allowed. That's why unscoped enum seems can not forward declared.        
- But how can `C++11`'s `enum class` get away with forward declarations when `C++98`'s `enum` can't?     
  - The underlying type for `C++11`'s `enum class` is always known: `int` by default.    
  - For `C++98`'s unscoped enum, specify its underlying type then it may be forward-declared.     
    - E.g. `enum Color: std::uint8_t;`    

### One situation where unscoped enums may be useful    
That's when referring to fields within `C++11`'s `std::tuple`.     
Refer to below cases, unscope enum looks more intuitive, in contrast scoped enum requires explicitly conversion but avoids namespace pullution. In which case you may want to decide which one to use in practice.        

```c++
    using UserInfo = std::tuple<std::string, // name
                                std::string, // email
                                std::size_t>;// reputation

    UserInfo uInfo; // object of tuple type
    //...

    auto val = std::get<1>(uInfo);  // get value of field 1, not intuitive
```
vs.     
```c++
    //// Use unscoped enum 

    enum UserInfoFields { UserInfoFieldsName, UserInfoFieldsEmail, UserInfoFieldsReputation };
    using UserInfo = std::tuple<std::string, // name
                                std::string, // email
                                std::size_t>;// reputation

    UserInfo uInfo; // object of tuple type
    //...

    auto val = std::get<UserInfoFieldsEmail>(uInfo);    // get value of field email, looks good, 
                                                        // works due to unscoped enum's implicitly conversion
```
vs. 
```c++
    //// Use scoped enum 

    enum class UserInfoFields { Name, Email, Reputation };
    using UserInfo = std::tuple<std::string, // name
                                std::string, // email
                                std::size_t>;// reputation

    UserInfo uInfo; // object of tuple type
    //...

    auto val = std::get<static_cast<std::size_t>(UserInfoFields.Email)>(uInfo); // get value of field email, 
                                                                                // too long, 
                                                                                // must explicitly convert
```
vs. 
```c++
    //// Use scoped enum and helper template function

    template<type E>
    constexpr auto toUType(E enumerator) noexcept {
        return static_cast<std::underlying_type_t<E>>(enumerator);
    }

    enum class UserInfoFields { Name, Email, Reputation };
    using UserInfo = std::tuple<std::string, // name
                                std::string, // email
                                std::size_t>;// reputation

    UserInfo uInfo; // object of tuple type
    //...

    auto val = std::get<toUType(UserInfoFields.Email)>(uInfo);  // get value of field email, 
                                                                // still too long but a little better 
```


## References
- [(Chinese) Understanding C++11 - Strong Typed Enum](https://github.com/wangyoucao577/modern-cpp/tree/master/understanding-cpp11#strong_typed_enum)
- [std::tuple](https://zh.cppreference.com/w/cpp/utility/tuple)