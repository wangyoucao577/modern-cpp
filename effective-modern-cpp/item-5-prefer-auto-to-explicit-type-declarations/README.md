# Item 5. Prefer auto to explicit type declarations

## Test Codes

```bash
$ # TODO: 
```

## Notes 

- What's a `std::function` object?    
  - `std::function` is a template in the `C++11` standard library that generalizes the idea of a function pointer. 
  - Whereas function pointers can point only to functions, however, `std::function` objects can refer to any callable object, i.e., to anything that can be invoked like a function.    

- `std::function` vs `auto`
  - using `std::function` is not the same as using `auto`.    
    - An `auto`-declared variable holding a closure has the same type as the closure, and as such it uses only as much memory as the closure requires.  
    - The type of a `std::function`-declared variable holding a closure is an instantiation of the `std::function` template, and that has a fixed size for any given signature. 
      - The size may not be adequate for the closure it's asked to store, and when that's the case, the `std::function` constructor will allocate heap memory to store the closure. 
      - The result is that the `std::function` object typically uses more memory than the `auto`-declared object.   
    - Thanks to implementation details that restrict inlining and yield indirect function calls, invoking a closure via a `std::function` object is almost certain to be slower than calling it via an `auto`-declared object. 
  - In another words, the `std::function` approach is generally **bigger and slower** than the `auto` approach, and it may yield out of memory exceptions, too.  

- The advantages of `auto` 
  - avoidance of uninitialized variables
  - avoidance of verbose variable declarations
  - ability to directly hold closures
  - ability to avoid the problems related to "type shortcuts"
    - see example in below value type of `std::unordered_map` 
  - `auto` types automatically change if the type of their initializing expression changes, and that means that some refactorings are facilitated by the use of `auto`. 
    - The fact of the matter is that writing types explicitly often does little more than introduce opportunities for subtle errors, either in correctness or efficiency or both. 

- value type of `std::unodered_map`

```c++
  std::unordered_map<std::string, int> m;

  // "type shortcut" problem
  // the value_type of std::unordered_map<std::string, int> is 
  //   std::pair<const std::string, int>, so the p in below codes is 
  //   a reference of temp object, not the element of m directly. 
  // use `const auto& p` instead.
  for (const std::pair<std::string, int>& p : m){
      // do something
  }
```

## References


