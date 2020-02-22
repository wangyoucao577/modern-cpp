# When ScopeGuard encounter exception
As shown in [More C++ Idioms/Scope Guard](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Scope_Guard), people usually uses scope guard to ensure that resources are always released once exit scope, whatever returning normally or exception occurrd.     
In my practice, sometimes I'll also use this technology to monitor when start/finished some function by printing logs, so that we can easier to analyze which part affects more performance(i.e. the bottleneck) by log's timestamp. Maybe sometimes functions hang in processing and we'll easy to know that too. However, it may lead to misunderstanding when exception occurs.       

## Test Codes

- [guard-encounter-exception.cc](./guard-encounter-exception.cc)

```bash
$ g++ -std=c++14 guard-encounter-exception.cc && ./a.out
waiting for input: 123
start handle: 123
finished handle: 123
finished handle: 123 (by scope guard).
result: 123

waiting for input: abc
start handle: abc
finished handle: abc (by scope guard).
stoul: no conversion
```

## Story

Firstly let's implement a simple scope guard as example. It takes a callable object(e.g. function, lambda, etc.) stores in `std::function<T>` as input when create the guard. The callable object will be called during its destructor.         
```c++
template<typename T>
class ScopeGuard{
public:
    ScopeGuard(std::function<T> f) : f_(std::move(f)) {}
    ~ScopeGuard() {
        if (f_){
            f_();
        }
    }
private:
    std::function<T> f_;
};
```

Then assume we have a `handle()` function that may do some heavy calculation. The `handle()` function may call many subroutines, sometimes costs much time, possible to throw exceptions, etc.     
In this sample, we call `std::stoul()` inside to simulate throwing exceptions.    
```c++
auto handle(const std::string& s) {
    std::cout << "start handle: " << s << std::endl;    // log when start the handing
    ScopeGuard<void()> local_guard([&](){
        // log when finished the handling, by scope guard automatically
        std::cout << "finished handle: " << s << " (by scope guard)." << std::endl;
    });

    //NOTE: assume there's heavy handling here, e.g. many functions call, complex calculation that costs much time, etc.
    auto ul = std::stoul(s);    // will throw exception if not convertible

    std::cout << "finished handle: " << s << std::endl; // explicitly log when finished the handling 
    return ul;
}
```
The `main()` function simply takes input from `std::cin` in the example, and pass it to the `handle()` function.        
```c++
int main(){

    while (true){
        std::cout << "waiting for input: ";
        
        std::string s;
        std::cin >> s;

        try{
            auto v = handle(s);
            std::cout << "result: " << v << std::endl;
        }catch (std::exception& e){
            std::cout << e.what() << std::endl;
        }
        std::cout << std::endl;
    }
    
    return 0;
}
```
In normal case, i.e. the input can be handled, both from logs explicitly print and scope guard work well.     
```bash
$ g++ -std=c++14 guard-encounter-exception.cc && ./a.out
waiting for input: 123
start handle: 123
finished handle: 123                    # explicity prints
finished handle: 123 (by scope guard).  # scope guard prints
result: 123
```
But in exception case, e.g. input `abc` that can not be handled, only scope guard can print the log out. That's because when exception occurs, the `handle()` function will return directly from where the exception throws, so the explicitly printing will not be called. But the scope guard object is a stack one, which always will be destructed while exiting scope. In which case its destructor was invoked.         
```bash
$ g++ -std=c++14 guard-encounter-exception.cc && ./a.out
waiting for input: abc
start handle: abc
finished handle: abc (by scope guard).  # only scope guard prints
stoul: no conversion
```
This behavior is very valuable for releasing resources. But if we look at the logs, we'll find that it shows **finished handle** in log, normally means handled successfully but actually failed. The problem is the **finished** is not that accurate here, so developers may misunderstand it if they only read logs.          

### Solution
- Use more accurate words in English instead of **finished**, e.g. **ended**, **exitted**, etc.    
- Always explicitly print log when handled successfully.     

The coming results could look like:     
```bash
$ g++ -std=c++14 guard-encounter-exception.cc && ./a.out
waiting for input: 123
start handle: 123
handled successfully: 123               # explicity prints
ended handle: 123                       # scope guard prints
result: 123

waiting for input: abc
start handle: abc
ended handle: abc                       # only scope guard prints
stoul: no conversion
```


## References
- [RAII and ScopeGuard](https://www.slideshare.net/adankevich/raii-and-scopeguard)
- [More C++ Idioms/Scope Guard](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Scope_Guard)
- [More C++ Idioms/Resource Acquisition Is Initialization](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Resource_Acquisition_Is_Initialization)
- [Wiki - Resource acquisition is initialization](https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization)