
#include <iostream>
#include <vector>

int main() {

  auto print = [](const std::string &name, const std::vector<int> &vec) {
    std::cout << name;
    for (const auto &v : vec) {
      std::cout << " " << v;
    }
    std::cout << std::endl;
  };

  std::vector<int> v1(10, 20);
  std::vector<int> v2{10, 20};
  print("v1", v1);
  print("v2", v2);

  return 0;
}