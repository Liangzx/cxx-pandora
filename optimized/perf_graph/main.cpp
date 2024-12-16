#include <chrono>
#include <iostream>
#include <vector>


class ComplexObject {
 public:
  ComplexObject(int value) : value_(value) {
    // std::cout << "ComplexObject(" << value_ << ") constructed\n";
  }
  ~ComplexObject() {
    // std::cout << "ComplexObject(" << value_ << ") destroyed\n";
  }

  int value_;
};

void func1() {
  std::vector<ComplexObject> vec;

  // 使用 push_back 添加元素
  vec.reserve(10000000);
  auto start_push_back = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < 10000000; ++i) {
    vec.push_back(ComplexObject(i));
  }
  auto end_push_back = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> push_back_time =
      end_push_back - start_push_back;
  std::cout << "Time taken by push_back: " << push_back_time.count() << " ms\n";
}

void func2() {
  std::vector<ComplexObject> vec;

  // 使用 push_back 添加元素
  vec.reserve(10000000);
  auto start_push_back = std::chrono::high_resolution_clock::now();
  const auto func = [&vec](int i)->void{
    vec.emplace_back(i);
  };

  for (int i = 0; i < 10000000; ++i) {
    // vec.emplace_back(i);
    func(i);
  }
  auto end_push_back = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> push_back_time =
      end_push_back - start_push_back;
  std::cout << "Time taken by emplace_back: " << push_back_time.count()
            << " ms\n";
}

int main() {
  func1();
  func2();
  return 0;
}

/**
 * [Linux 性能调优必备：perf
 * 使用指南](https://mp.weixin.qq.com/s/iJCL2EaKrbg6lInYasyiAw) g++ -std=c++11
 * -o vector_test vector_test.cpp
 * ./vector_test
 * perf record -g -o vector_test.data ./vector_test
 * perf script -i vector_test.data > vector_test.perf
 * stackcollapse-perf.pl vector_test.perf > vector_test.folded
 * flamegraph.pl vector_test.folded > vector_test.svg
 */
