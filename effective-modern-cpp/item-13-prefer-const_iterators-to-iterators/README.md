# Item 13. Prefer const_iterators to iterators     

## Test Codes

- [const_iterator_test.cc](const_iterator_test.cc)

```bash
$ g++ -std=c++14 const_iterator_test.cc&& ./a.out
const iterator type: NSt3__111__wrap_iterIPKiEE, is const: 0 0
      iterator type: NSt3__111__wrap_iterIPiEE, is const: 0 0
values: 1 2 3 100 4 200 5
$ 
$ # type of std::vector<int>::const_iterator
$ c++filt -t NSt3__111__wrap_iterIPKiEE
std::__1::__wrap_iter<int const*>
$ # type of std::vector<int>::iterator
$ c++filt -t NSt3__111__wrap_iterIPiEE
std::__1::__wrap_iter<int*>
```

## Notes

- The standard practice of using `const` whenever possible dictates that you should use `const_iterator`s anytime you need an `iterator`, yet have no need to modify what the `iterator` points to.     
  - That's as true for `C++98` as for `C++11`.    
  - But it's rarely worth in `c++98` since `const_iterator`s were so much trouble in `C++98`.    
  - Now `const_iterator`s are both easy to get and easy to use in `C++11`.    
    - E.g. easy to get `const_iterator`s by `cbegin()/cend()`, `insert()/erase()` uses `const_iterator`s to identify positions, etc.    
  
- There's no portable conversion from a `const_iterator` to an `iterator`.     
  - Not even with a `static_cast`.     
  - Even the semantic sledgehammer known as `reinterpret_cast` can't do the job.     
  - It's true in both `C++98` and `C++11`.    
  - `const_iterator`s simply don't convert to `iterator`s, no matter how much it might seem like they should.    
  - From the sample in [Test Codes](#test-codes),    
    - type of `std::vector<int>::const_iterator`: `std::__1::__wrap_iter<int const*>`,     
    - type of `std::vector<int>::iterator`: `std::__1::__wrap_iter<int*>`.    
    - The differences between them are not whether themselves are `const`, but whether their base type are `const`.    

- An oversight during standardization:     
  - `C++11` added the non-member functions `begin()` and `end()`, but it failed to add `cbegin()/cend()/rbegin()/rend()/crbegin()/crend()`.     
  - `C++14` rectifies the oversight.        
  - If you're using `C++11`, you want to write maximally generic code, and none of the libraries you're using provides the missing templates for non-member `cbegin()` and friends, you can throw your own implementations together with ease.     

```c++
    //E.g. an implementation of non-member `cbegin()`
    template<class C>
    auto cbegin(const C& container) -> decltype(std::begin(container)){
        return std::begin(container);
    }
```

## References
