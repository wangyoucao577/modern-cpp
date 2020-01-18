
#include <iostream>

class Widget {
public:
    Widget(){
        std::cout << "default constructor" << std::endl;
    }
    Widget(std::initializer_list<int>){
        std::cout << "constructor takes initilaizer_list" << std::endl;
    }
};

int main(){

    Widget w0;      // calls default constructor
    Widget w1();    // most vexing parse! w1 will be interpreted as a function declare
    Widget w2{};    // calls default constructor
    Widget w3({});  // calls constructor takes initilaizer_list
    Widget w4{{}};  // calls constructor takes initilaizer_list

    return 0;
}