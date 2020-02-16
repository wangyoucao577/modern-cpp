# Item 8. Prefer `nullptr` to `0` and `NULL`

## Test Codes


## Notes

### Type of `nullptr`

- It doesn't have a pointer type, but you can think of it as **a pointer of all types**.    
- `nullptr`'s actual type is `std::nullptr_t`, which defined as `typedef decltype(nullptr) nullptr_t;`(see [std::nullptr_t](https://en.cppreference.com/w/cpp/types/nullptr_t)).    
- The type `std::nullptr_t` **implicitly converts to all raw pointer types**, and that's what makes `nullptr` act as if it were a pointer of all types.    

### Advantages

- Using `nullptr` instead of `0` or `NULL` thus avoids overload resolution surprises. 
   
```c++
    // three overloads of `f`
    void f(int);
    void f(bool);
    void f(void*);

    f(0);    // calls f(int)
    f(NULL); // might not compile, but typically calls f(int). Never calls f(void*)

    f(nullptr); // calls f(void*), i.e. only nullptr works
```

- Improves code clarity, espically when `auto` variables are involved.     

```c++
    auto result = findRecord( /* arguments */ );
    
    // type of result may not be clear
    if (result == 0) {      // can not know result is integer or pointer
        // ... 
    }

    // no ambiguity
    if (result == nullptr){ // result must be a pointer
        // ...
    }

```

- `nullptr` shines espically brightly when templates enter the picture.   
Refer to [Item 1. Understand template type deductions](../item-1-understand-template-type-deduction/) for more about template type deduction.     

```c++

    void f(void*){
        std::cout << "function f" << std::endl;
    }

    template<typename FuncType, typename PtrType>
    decltype(auto) call(FuncType func, PtrType ptr) {
        return func(ptr);
    }

    // both compile error! 
    // because both 0 and NULL will be deduced to integer(`int` or `long int`) 
    call(f, 0); 
    call(f, NULL);

    // works! 
    call(f, nullptr); // output "function f"
```

## References
- [std::nullptr_t](https://en.cppreference.com/w/cpp/types/nullptr_t)