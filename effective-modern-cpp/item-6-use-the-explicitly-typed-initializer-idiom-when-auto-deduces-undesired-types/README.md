# Item 6. Use the explicity typed initializer idiom when auto deduces undesired types

## Test Codes

```bash
$ # TODO: 
```

## Notes 

### "Proxy" design pattern / proxy class

- proxy class: a class that exists for the purpose of emulating and augmenting the behavior of some other type. 
  - `std::vector<bool>::reference` is an example of a proxy class.
  - The utility of proxy classes is well-established. In fact, the design pattern "Proxy" is one of the most longstanding members of the software design patterns Pantheon.
- Some proxy classes are designed to be apparent to clients. 
  - That's the case for `std::shared_ptr` and `std::unique_ptr`, for example.    
- Other proxy classes are designed to act more or less invisibly. 
  - `std::vector<bool>::reference` is an example of such "invisible" proxies, as is its `std::bitset` compatriot, `std::bitset::reference`.
    - Be aware that when use `C++11 range-based for` on the proxy classes like `std::vector<bool>`, make sure you're working with correct way. See details in [What is the correct way of using C++11's range-based for?](https://stackoverflow.com/questions/15927033/what-is-the-correct-way-of-using-c11s-range-based-for). 

### General Rule: "invisible" proxy classes don't play well with auto

- avoid code of this form
  - "invisible" proxy types can cause `auto` to deduce the "wrong" type for an initializing expression. 

```c++
  // avoid code of this form
  auto someVar = expression of "invisible" proxy class type

  /********* example **********/

  std::vector<bool> features(const Widget& w);  // implemented in somewhere

  Widget w;
  //...
  bool highPriority1 = features(w)[5]; // correct
  auto highPriority2 = features(w)[5]; // wrong! highPriority2 's type won't be bool! 

```

- how can you recognize when proxy objects are in use? 
  - by document
    - Although "invisible" proxy classes are designed to fly beneath programmer radar in day-to-day use, libraries using them often document that they do so. 
  - by header files
    - Where documentation comes up short, header files fill the gap.
    - It's rarely possible for source code to fully cloak proxy objects.
  - misc.
    - Paying careful attention to the interfaces you're using can often reveal the existence of proxy classes. 
    - In practice, many developers discover the use of proxy classes only when they try to track down mystifying compilation problems or debug incorrect unit test results.
  
- solution: the explicity typed initializer idiom
  - `auto` itself isn't the problem. The problem is that `auto` isn't deducing the type you want it to deduce.

```c++
  // solution: the explicity typed initializer idiom, 
  //    i.e. force a different type deduction
  auto highPriority = static_case<bool>(features(w)[5]);  // correct. highPriority 's type forced to bool!
```



## References
- [What is the correct way of using C++11's range-based for?](https://stackoverflow.com/questions/15927033/what-is-the-correct-way-of-using-c11s-range-based-for)
- [std::vector<bool>::reference](https://en.cppreference.com/w/cpp/container/vector_bool/reference)

