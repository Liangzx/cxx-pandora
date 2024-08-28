#include <iostream>
#include <vector>
#include <boost/range/iterator_range.hpp>
#include <boost/iterator/filter_iterator.hpp>

// 谓词：检查一个数是否为偶数
bool is_even(int n) {
    return n % 2 == 0;
}

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // 创建过滤迭代器
    auto even_numbers_begin = boost::make_filter_iterator(is_even, numbers.begin(), numbers.end());
    auto even_numbers_end = boost::make_filter_iterator(is_even, numbers.end(), numbers.end());

    // 使用 range-based for 循环遍历偶数
    for (auto it = even_numbers_begin; it != even_numbers_end; ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    return 0;
}
