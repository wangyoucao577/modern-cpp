#include <iostream>
#include <array>

constexpr 
int pow(int base, int exp) noexcept {
#if __cplusplus >= 201402L
    auto result = 1;
    for (int i = 0; i < exp; ++i){
        result *= base;
    }
    return result;
#else
    // C++11 requires one single return
    return exp <= 0 ? 1 : base * pow(base, exp - 1);
#endif
}


int main() {
    std::cout << "C++ " << __cplusplus << std::endl;

    // compile-time using pow()
    constexpr auto num = 3;
    std::array<int, pow(2, num)> data;
    
    // runtime using pow()
    const auto base = 3;
    const auto num2 = 10;
    std::cout << "pow(" << base << "," << num2 << ")=" << pow(base, num2) << std::endl;
    return 0;
}