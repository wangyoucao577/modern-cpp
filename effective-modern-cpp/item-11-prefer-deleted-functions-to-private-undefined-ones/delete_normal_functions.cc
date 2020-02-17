
#include <iostream>

bool is_lucky(int a) {
    return a > 0;
}

#if defined(DELETE_OVERLOADS)
bool is_lucky(char) = delete;   // reject call for char
bool is_lucky(bool) = delete;   // reject call for bool

// Note that this delelted function can reject both double and float, 
// because given a choice between converting a float to a int or to a double, C++ prefers 
// that conversion to double. So calling is_lucky with a float will therefore call the double
// overload, not the int one.  
bool is_lucky(double) = delete; // reject call for double and float
#endif

int main() {

    std::cout << "is_lucky(1): " << is_lucky(1) << std::endl;

    // can not compile after deleted is_lucky() overloads
    std::cout << "is_lucky('a'): " << is_lucky('a') << std::endl;
    std::cout << "is_lucky(true): " << is_lucky(true) << std::endl;
    std::cout << "is_lucky(3.5): " << is_lucky(3.5) << std::endl;
    std::cout << "is_lucky(3.5f): " << is_lucky(3.5f) << std::endl;
    return 0;
}