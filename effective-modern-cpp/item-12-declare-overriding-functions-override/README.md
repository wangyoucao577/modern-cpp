# Item 12. Declare overriding functions override     

## Test Codes

- [member-function-reference-qualifiers.cc](./member-function-reference-qualifiers.cc)

```bash
$ g++ -std=c++14 member-function-reference-qualifiers.cc && ./a.out
Widget::DataType &Widget::values() & invoked.
Widget::DataType &&Widget::values() && invoked.
```

## Notes

### MUST requirements for overriding to occur

- The base class function must be `virtual`.    
- The base and derived function names must be identical(except in the case of destructors).    
- The parameter types of the base and derived functions must be identical.     
- The `const`ness of the base and derived functions must be identical.     
- The return types and exception specifications of the base and derived functions must be compatible.     
- The functions' **reference qualifiers**(`C++11` new feature: member function reference qualifiers) must be identical. 


All these requirements for overriding mean that small mistakes can make a big difference. Code containing overriding errors is typically valid, but its meaning isn't what you intended. You therefore can't rely on compilers notifying you if you do something wrong.       

### Declare a derived class function `override`     
Because declaring derived class overrides is important to get right, but easy to get wrong, `C++11` gives you a way to make explicit that a derived class function is supposed to override a base class version: declare it `override`.         

- It enables compilers to tell you when would-be overrides aren't overriding anything.    
- It can also help you gauge the ramifications if you're contemplating changing the signature of a virtual function in a base class.    
  - I.e., if derived classes use `override` everywhere, you can just change the signature, recompile your system, see how much damage you've caused(i.e., how many derived classes fail to compile). Without `override`, you'd have to hope you have comprehensive unit tests in place.       

So, **whenever you declare a function in a derived class that's meant to override a virtual function in a base class, be sure to declare that function `override`**.    

### keywords `override` and `final` are contextual    
These keywords have the characteristic that they are reserved, but only in certain context.     
- In the case of `override`, it has a reserved meaning only when it occurs at the end of a member function declaration. That means that if you have legacy code that already uses the name `override`, you don't need to change it for `C++11`.    

```c++
    class Warning {         // potential legacy class from C++98
    public: 
        void override();    // legal in both C++98 and C++11 (with the same meaning)

        //...
    };
```

### Member function reference qualifiers     
`C++11` introduces new feature **member function reference qualifiers** that makes member function possible to be invoked only by `lvalue` object or `rvalue` object(i.e., `*this`).     
- It's precisely analogous to the `const` at the end of a member function declaration, which indicates that the object on which the member function is invoked(i.e., this) is `const`.     

Refer to sample [member-function-reference-qualifiers.cc](./member-function-reference-qualifiers.cc).     

By the way, if a member function is reference qualified, all overloads of that function must be reference qualified. That's because a non-reference-qualified overload may be invoked on both `lvalue` and `rvalue` objects.       

## References
