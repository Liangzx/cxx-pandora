#include <array>
#include <iostream>

// 13.1 Using auto for Template Parameters
template <auto N> class S {};

// 13.1.1 Parameterizing Templates for Characters and Strings
template <auto Sep = ' ', typename First, typename... Args>
void print(const First &first, const Args &... args) {
  std::cout << first;
  auto outWithSep = [](const auto &arg) { std::cout << Sep << arg; };
  (..., outWithSep(args));
  std::cout << '\n';
}

void func1() { print<'-'>("a", "b", "c"); }

// 13.2 Using auto as Variable Template Parameter
template <typename T, auto N> std::array<T, N> arr{};

void printArr() {
  std::cout << "arr<int,5>: ";
  for (const auto &elem : arr<int, 5>) {
    std::cout << elem << ' ';
  }
  std::cout << "\narr<int,5u>: ";
  for (const auto &elem : arr<int, 5u>) {
    std::cout << elem << ' ';
  }
  std::cout << '\n';
}

// 13.3 Using decltype(auto) as Template Parameter

#include <iostream>
#include <map>
#include <optional>
#include <string>

int main() {
  func1();
  printArr();

  return 0;
}
