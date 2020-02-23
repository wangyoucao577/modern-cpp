# Item 16. Make const member functions thread safe

## Test Codes

## Notes

- classic use case for `mutable`    
The `roots()` function doesn't change the `Polynomial` object on which it operates, but, as part of its caching activity, it may need to modify `rootVals` and `rootsAreValid`.     

```c++
    class Polynomial {
    public:
        using RootsType = std::vector<double>;

        RootsType roots() const {
            std::lock_guard<std::mutex> l(m);   // lock mutex to make sure thread safe

            if (!rootsAreValid) {
                // ... calculation

                rootsAreValid = true;
            }
            return rootVals;
        }

    private:
        mutable std::mutex m;
        mutable bool rootsAreValid {false}; 
        mutable RootsType rootVals{};
    }
```

- It's worth noting that because `std::mutex` can be neither copied nor moved, a side effect of adding `m` to `Polynomial` loses the ability to be copied and moved.     

- A lesson: for a single variable or memory location requiring synchronization, use of a `std::atomic` is adequate, but once you get to two or more variables or memory locations that require manipulation as a unit, you should reach for a `mutex`.     
  - Use of `std::atomic` variables may offer better performance than a mutex, but they're suited for manipulation of only a single variable or memory location.     

- Make `const` member functions thread safe unless you're certain they'll never be used in a concurrent context.     

## References
- [Race condition](https://en.wikipedia.org/wiki/Race_condition)
