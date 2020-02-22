
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

auto str_to_integer(const std::string& s) {
    std::cout << "begin convert \"" << s << "\" to integer." << std::endl;
    ScopeGuard<void()> local_guard([&](){
        std::cout << "end   convert \"" << s << "\" to integer (by scope guard)." << std::endl;
    });

    auto ul = std::stoul(s);    // will throw exception if not convertible

    std::cout << "end   convert \"" << s << "\" to integer." << std::endl;
    return ul;
}

int main(){

    while (true){
        std::cout << "waiting for input: ";
        
        std::string s;
        std::cin >> s;

        try{
            auto v = str_to_integer(s);
            std::cout << "got integer: " << v << std::endl;
        }catch (std::exception& e){
            std::cout << e.what() << std::endl;
        }
        std::cout << std::endl;
    }
    
    return 0;
}