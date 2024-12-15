
#include <array>
#include <chrono>
#include <iostream>
#include <vector>
#include <array>

void func1() {
  const auto start_time = std::chrono::high_resolution_clock::now();
  constexpr auto cnt = 1000000;
  std::vector<int> vec(cnt);
  vec.reserve(cnt);
  // for (int i = 0; i < cnt; i++) {
  //   // vec.push_back(i);
  //   vec[i] = i;
  // }

  std::cout << vec[100060] << std::endl;
  const auto end_time = std::chrono::high_resolution_clock::now();
  const auto dur = std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - start_time);
  std::cout << dur.count() << std::endl;
}

void func2() {
  const auto start_time = std::chrono::high_resolution_clock::now();
  constexpr auto cnt = 1000000;
  std::array<int, cnt> vec{};
  vec.fill(0);
  // for (int i = 0; i < cnt; i++) {
  //   // vec.push_back(i);
  //   vec[i] = i;
  // }

  const auto end_time = std::chrono::high_resolution_clock::now();
  const auto dur = std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - start_time);
  std::cout << dur.count() << std::endl;
}

int main() {
  func1();
  // func2();
  return 0;
}
