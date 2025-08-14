#include <forward_list>
#include <iostream>
#include <list>
#include <string>
#include <type_traits>
#include <vector>

#include <array>
#include <iterator>
#include <string>
#include <type_traits>

// 基础模板（默认非容器）
template <typename T, typename = std::void_t<>> struct is_container : std::false_type {};

// 使用 void_t 的特化版本（检测容器特征）
template <typename T>
struct is_container<
    T, std::void_t<typename T::iterator, // 必须定义迭代器类型
                   decltype(std::declval<T>().begin()), // 必须支持 begin()
                   decltype(std::declval<T>().end()),   // 必须支持 end()
                   typename T::value_type               // 必须定义值类型
                   >> : std::true_type {};

// 显式排除 std::string（虽然是容器但不希望被识别）
template <> struct is_container<std::string, void> : std::false_type {};

// 显式排除原生数组（需单独处理）
template <typename T, std::size_t N>
struct is_container<T[N], void> : std::true_type {};

// 容器处理（通过标签分发实现）
template <typename JSON, typename T>
void process_impl(const JSON &json, T &value, std::true_type) {
  std::cout << "处理容器类型: [";
  for (auto it = std::begin(container); it != std::end(container); ++it) {
    std::cout << *it;
    if (std::next(it) != std::end(container)) {
      std::cout << " ";
    }
  }
  std::cout << "]";

  std::cout << std::endl;
}

// 非容器处理
template <typename JSON, typename T>
void process_impl(const JSON &json, T &value, std::false_type) {
  std::cout << "处理非容器类型: " << value << std::endl;
}

// 统一接口
template <typename JSON, typename T> void process(const JSON &json, T &input) {
  process_impl(input, typename is_container<T>::type{});
}

int main() {
  int num = 42;
  std::vector<int> vec = {1, 2, 3};
  std::list<std::string> lst = {"A", "B", "C"};
  std::forward_list<float> flist = {1.1f, 2.2f, 3.3f};
  std::string str = "hello";
  int arr[] = {4, 5, 6};

  std::string json_obj;

  process(json_obj, num);   // 非容器
  process(json_obj, vec);   // 容器
  process(json_obj, lst);   // 容器
  process(json_obj, flist); // 容器（forward_list）
  process(json_obj, str);   // 非容器（字符串特例）
  process(json_obj, arr);   // 容器（原生数组）

  return 0;
}
