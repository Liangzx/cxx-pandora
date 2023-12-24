// https://stackoverflow.com/questions/9860503/templated-recursive-data-types/9860911#9860911

#include <memory>
#include <string>
#include <unordered_map>
#include <iostream>

template <typename T, template <typename U, typename V, typename... Args>
                      class Container = std::unordered_map>
struct SomeType {
  SomeType(T &&a1) : a(std::move(a1)) {}
  Container<T, std::unique_ptr<SomeType>> mapping;
  T a = {};
};

int main() {
  SomeType<std::string> aa("hello");
  aa.mapping["hello1"] = std::make_unique<SomeType<std::string>>("world");
  std::cout << aa.mapping["hello1"]->a << std::endl;
  return 0;
}