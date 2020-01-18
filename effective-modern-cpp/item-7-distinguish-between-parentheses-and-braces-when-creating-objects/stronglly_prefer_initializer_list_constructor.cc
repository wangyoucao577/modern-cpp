
#include <iostream>

class Widget {
public:
  Widget(int i, bool b) { std::cout << "constructor 1" << std::endl; }
  Widget(int i, double d) { std::cout << "constructor 2" << std::endl; }
#if defined(STD_INITIALIZER_LIST_CONSTRUCTOR)
  Widget(std::initializer_list<double>) {
    std::cout << "constructor 3" << std::endl;
  }
#endif
};

int main() {

  Widget w1(10, true); // call constructor 1
  Widget w2{
      10,
      true}; // call constructor 3 if it's exist, otherwise call constructor 1
  Widget w3(10, 5.0); // call constructor 2
  Widget w4{
      10,
      5.0}; // call constructor 3 if it's exist, otherwise call constructor 2

  return 0;
}