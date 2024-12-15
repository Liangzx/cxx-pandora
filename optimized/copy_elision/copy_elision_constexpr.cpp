#include <iostream>
class Verbose {
  int i;

public:
  Verbose() { std::cout << "Constructed\n"; }
  Verbose(const Verbose &) { std::cout << "Copied\n"; }
  Verbose(Verbose &&) { std::cout << "Moved\n"; }
};

Verbose create() {
  Verbose tv;
  return tv;
}

int main() {
  Verbose v = create();
  return 0;
}

// g++ -std=c++11 -fno-elide-constructors copy_elision_constexpr.cpp
// g++ -std=c++17  -fno-elide-constructors copy_elision_constexpr.cpp
