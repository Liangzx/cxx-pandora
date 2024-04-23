#include <iostream>
#include <vector>
#include <list>
#include <array>

// 使用模板模板参数
template <typename T, template <typename ...> class Container>
void printContainer(const Container<T>& container) {
    for (const auto& element : container) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}

template<typename T, size_t N>
void printContainer(const std::array<T, N>& container) {
    for (const auto& element : container) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::list<int> lvec = {1, 2, 3, 4, 5};
    std::array<int, 3> a2 = {1, 2, 3};
    printContainer(vec); // 使用 vector 作为容器类型
    printContainer(lvec);
    printContainer(a2);

    return 0;
}
