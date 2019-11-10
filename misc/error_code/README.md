# std::error_code

## Test

- coding style
```bash
$ cd flightservice 
$ clang-format -style=webkit -i *
```

- run test
```bash
$ cd flightservice 
$ g++ -std=c++14 main.cpp flights_err.cpp seats_err.cpp && ./a.out
FlightsErr:20, value: 20 msg: no flight combination found
SeatsErr:6, value: 6 msg: all seats booked
```

## Note

## References
- [Your own error code](https://akrzemi1.wordpress.com/2017/07/12/your-own-error-code/)
- [Your own error condition](https://akrzemi1.wordpress.com/2017/08/12/your-own-error-condition/)