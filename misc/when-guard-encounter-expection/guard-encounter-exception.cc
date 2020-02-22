
#include <iostream>

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

auto handle(const std::string& s) {
    std::cout << "start handle: " << s << std::endl;
    ScopeGuard<void()> local_guard([&](){
            std::cout << "finished handle: " << s << " (by scope guard)." << std::endl;
    });

    //NOTE: assume there's heavy handling here, e.g. many functions call, complex calculation that costs much time, etc.
    auto ul = std::stoul(s);    // will throw exception if not convertible

    std::cout << "finished handle: " << s << std::endl;
    return ul;
}

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