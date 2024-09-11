#include <vector>
#include <iostream>

int main() {
  std::vector<int> a{1, 3, 5};
  std::vector<int> b{1, 3, 5};
  if (a == b) {
    std::cout << "a == b" << std::endl;
  }
  return 0;
}
