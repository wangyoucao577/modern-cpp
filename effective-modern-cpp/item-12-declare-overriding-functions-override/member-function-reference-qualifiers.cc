#include <iostream>
#include <vector>

class Widget {
public:
    using DataType = std::vector<int>;
    DataType& values() &   { 
        std::cout << __PRETTY_FUNCTION__ << " invoked." << std::endl;
        return values_; 
    }
    DataType&& values() && { 
        std::cout << __PRETTY_FUNCTION__ << " invoked." << std::endl;
        return std::move(values_); 
    }

private: 
    DataType values_ {1, 2, 3};
};

int main() {

    Widget w; 
    auto v1 = w.values();

    auto v2 = Widget().values();
    return 0;
}