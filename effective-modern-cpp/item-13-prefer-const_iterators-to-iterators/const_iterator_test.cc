#include <iostream>
#include <vector>
#include <type_traits>

int main() {

    std::vector<int> values{1, 2, 3, 4, 5};

    auto cit = std::find(values.cbegin(), values.cend(), 4);
    std::cout << "const iterator type: " << typeid(cit).name() 
        << ", is const: " << std::is_const<decltype(cit)>::value << " " << std::is_const<std::vector<int>::const_iterator>::value << std::endl;
    values.insert(cit, 100);

    auto it = std::find(values.begin(), values.end(), 5);
    std::cout << "      iterator type: " << typeid(it).name() 
        << ", is const: " << std::is_const<decltype(it)>::value << " " << std::is_const<std::vector<int>::iterator>::value << std::endl;
    values.insert(it, 200);


    std::cout << "values: ";
    for (auto v : values) {
        std::cout << v << " ";
    }
    std::cout << std::endl;

    return 0;
}