# coroutines     

## Build & Run

```bash
g++ -std=c++20 -fcoroutines ./coroutines.cc && ./a.out
g++ -std=c++20 -fcoroutines ./fibonacci.cc && ./a.out

# or via clang (add '-MJ compile_commands.json' to generate JSON Compilation Database)
clang++ -std=c++20 -fcoroutines-ts ./coroutines.cc && ./a.out
clang++ -std=c++20 -fcoroutines-ts ./fibonacci.cc && ./a.out
```

## References
- [Coroutines (C++20)](https://en.cppreference.com/w/cpp/language/coroutines)    
- [Deciphering C++ Coroutines - A Diagrammatic Coroutine Cheat Sheet - Andreas Weis - CppCon 2022](https://www.youtube.com/watch?v=J7fYddslH0Q&ab_channel=CppCon)     
- [C++20 Coroutine Cheat Sheet](https://github.com/ComicSansMS/cpp20_coroutine_cheat_sheet)
- [Asymmetric Transfer - Coroutine Theory](https://lewissbaker.github.io/2017/09/25/coroutine-theory)
- [My tutorial and take on C++20 coroutines](https://www.scs.stanford.edu/~dm/blog/c++-coroutines.html)
