# std::error_code
Understanding standard library error code and try its usage, i.e. [std::error_code](http://naipc.uchicago.edu/2014/ref/cppreference/en/cpp/error/error_code.html), [std::error_category](http://naipc.uchicago.edu/2014/ref/cppreference/en/cpp/error/error_category.html), [std::error_condition](http://naipc.uchicago.edu/2014/ref/cppreference/en/cpp/error/error_condition.html).    

## Sample code
It's a flight service comes from [Your own error code](https://akrzemi1.wordpress.com/2017/07/12/your-own-error-code/), see my implementation in [flightservice](./flightservice/).    

- requirements for the flight service    
  - It's used for looking for flight connections. You tell me where from and where to you want to go, and I will offer you concrete flights, and a price. 
  - In order to be able to do this, the flight service calls other services in turn:
    - one for finding the (short) sequence of flights that will take you to your destination,
    - one for checking if there is still seats available on these flights in the requested class of service (economy class, business class),
  - Each of these services can fail for a number of reasons.     
  - See details in [Your own error code](https://akrzemi1.wordpress.com/2017/07/12/your-own-error-code/).    



- coding style
```bash
$ cd flightservice 
$ clang-format -style=webkit -i *
```

- run test
```bash
$ cd flightservice 
$ g++ -std=c++14 main.cpp flights_err.cpp seats_err.cpp failure_source.cpp && ./a.out
FlightsErr:10
  value: 10 msg: nonexistent airport name in request
  FailureSource message: invalid user request
SeatsErr:20
  value: 20 msg: all seats booked
  FailureSource message: no solution found for specified request

```

## Note

### Convention
- always use `0` to indicate `OK/Success`

### Understand types

- [std::error_code](http://naipc.uchicago.edu/2014/ref/cppreference/en/cpp/error/error_code.html)
  - used to store and transport error codes
  - it has a `int` to store error code it self, and `const std::error_category*` to store the `error_category` of this error code
  - a `error_code` will be either `print` to log or compare with `error_condition`
- [std::error_category](http://naipc.uchicago.edu/2014/ref/cppreference/en/cpp/error/error_category.html)
  - used to store which subsystem the error comes from, and give concrete message of each code value of the subsystem 
  - when call `error_code.message()`, the concrete message will be returned due to its polymorphic properties
- [std::error_condition](http://naipc.uchicago.edu/2014/ref/cppreference/en/cpp/error/error_condition.html)
  - used to match errors
  - callers will only handle errors by check whether `error_code` belongs to `error_condition`(the meaning is 'belongs to' though using `==`)

### How to use `std::error_code/error_condition` in your own application
See example in [flightservice](./flightservice/).    

- define your own error code/condition enum, e.g. `enum class xxx`
- specialize the `std::is_error_code_enum/is_error_condition_enum` template for your error code/condition enum 
- overload the `make_error_code()/make_error_condition()` function for your error code/condition enum
- implement your error category(inherit from `std::error_category`) for your error code/condition enum(so that it's possible to print subsystem name and concrete message into log) 

### Pros and Cons

- Pros
  - able to store error code itself in a `int`
  - able to distinguish which subsystem the error comes from
  - able to print concrete code value and message to log
  - callers only need to handle errors they care (`std::error_condition`)
    - in the [flightservice](./flightservice/) example, callers only want to distinguish whether error caused by `BadUserInput/InternalError/NoSolution` instead of handle many `FlightsErr/SeatsErr`.    

- Cons 
  - difficult to understand
    - i.e. difficult to understand the whole story that why design it like this(maybe due to too complex requirements of standard library)
  - difficult to use
    - e.g. library/application developers have to do a lot of work to plugin new defined enum to `std::error_code`
    - e.g. users have to understand the `std::error_code` design before use it
  - values of `std::error_code` and `std::error_condition` are not the same
    - generally when we do `a == b`, in intuitive `a` and `b` have same type and same value, but they're not. 
    - it'll very confuse if we print both `std::error_code` and `std::error_condition` into log.

### Ideas for design good error code
`std::error_code` is designed for `C++` standard library, which may have a lot of complex requirments to make it so complex like this.     
When we need a error code design for our own library/application, there could be some ideas can help us to get a simpler but sufficient design.     

- follow the convention that `0` always means `OK/Success`    
- use `int` to store error code
- each error code has a corrsponding string message for better logging
- make error code consistent(a `int` code only has one meaning)
- distinguish subsystem by use different code segment since we possible to pre-define the segments(e.g. `1000~1999` indicate A subsystem, `2000~2999` indicate B subsystem, etc.)
- simplify error handling for caller
  - provide methods for caller maybe a good way, e.g. [rocksdb/status.h](https://github.com/facebook/rocksdb/blob/master/include/rocksdb/status.h), [leveldb/status.h](https://github.com/google/leveldb/blob/master/include/leveldb/status.h)



### Remain questions

- you can teach a new error condition to recognize existing error codes, but also you can teach a new error code to be recognized by existing error conditions    
It's metioned in [Your own error condition](https://akrzemi1.wordpress.com/2017/08/12/your-own-error-condition/). Not quite understand it.    

## References
- [Your own error code](https://akrzemi1.wordpress.com/2017/07/12/your-own-error-code/)
- [Your own error condition](https://akrzemi1.wordpress.com/2017/08/12/your-own-error-condition/)
- [Use cases for std::error_code](https://stackoverflow.com/questions/32232295/use-cases-for-stderror-code)
- [std::error_code](http://naipc.uchicago.edu/2014/ref/cppreference/en/cpp/error/error_code.html)
- [std::error_category](http://naipc.uchicago.edu/2014/ref/cppreference/en/cpp/error/error_category.html)
- [std::error_condition](http://naipc.uchicago.edu/2014/ref/cppreference/en/cpp/error/error_condition.html)
- [System error support in C++0x - part 1](http://blog.think-async.com/2010/04/system-error-support-in-c0x-part-1.html)
- [System error support in C++0x - part 2](http://blog.think-async.com/2010/04/system-error-support-in-c0x-part-2.html)
- [System error support in C++0x - part 3](http://blog.think-async.com/2010/04/system-error-support-in-c0x-part-3.html)
- [System error support in C++0x - part 4](http://blog.think-async.com/2010/04/system-error-support-in-c0x-part-4.html)
- [System error support in C++0x - part 5](http://blog.think-async.com/2010/04/system-error-support-in-c0x-part-5.html)
- [rocksdb/status.h](https://github.com/facebook/rocksdb/blob/master/include/rocksdb/status.h)
- [leveldb/status.h](https://github.com/google/leveldb/blob/master/include/leveldb/status.h)
