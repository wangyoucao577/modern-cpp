
#include <iostream>

int main() {
  double x{0}, y{1.5}, z{2.0};

  int sum1(x + y + z);
  std::cout << "sum1: " << sum1 << std::endl;

  int sum2 = x + y + z;
  std::cout << "sum2: " << sum2 << std::endl;

  int sum3{x + y + z}; // compile error
  std::cout << "sum3: " << sum3 << std::endl;

  return 0;
}