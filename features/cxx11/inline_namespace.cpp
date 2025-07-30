#include <iostream>

namespace lzx {

namespace _V1 {
[[deprecated("改用 V2")]] void foo() { std::cout << "V1\n"; }
} // namespace _V1

inline namespace _V2 {
void foo() { std::cout << "V2\n"; }
} // namespace _V2

} // namespace lzx

int main() {
  lzx::foo();
  return 0;
}
