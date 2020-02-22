
#include <iostream>

class Guard{
public:
    Guard() {
        std::cout << "Guard start" << std::endl;
    }
    ~Guard() {
        std::cout << "Guard end" << std::endl;
    }
};

auto str_to_integer(const std::string& s) {
    std::cout << "enter \"" << __PRETTY_FUNCTION__ << "\"" << std::endl;
    Guard g;

    auto ul = std::stoul(s);    // will throw exception if not convertible

    std::cout << "exit  \"" << __PRETTY_FUNCTION__ << "\""  << std::endl;
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