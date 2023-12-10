#include <iostream>
#include <functional>

int show(int a) { return 2 * a; }

int main() {
  std::function<int(int)> sf{show};
  sf(12);

  return 0;
}


