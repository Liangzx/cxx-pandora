// https://www.caichinger.com/boost-range/boost-any_range.html

#include <iostream>
#include <list>
#include <vector>

#include <boost/range/any_range.hpp>


// any_range() is a range object that can represent vectors, lists, or other
// range objects. It can be used in interfaces to hide the type of the
// underlying range, which decouples the interface from the implementation.
// The different internal storage container types can be used without affecting
// the public interface.
//
// Note that any_range() does not copy the input range. The range remains valid
// only as long as the underlying containers exist. In other words, you can't
// return an any_range() of a temporary variable!
// Hence the static list/vector in the example below.

typedef boost::any_range<int,
                         boost::bidirectional_traversal_tag,
                         int,
                         std::ptrdiff_t
                        > int_any_range;

int_any_range make_range(bool use_list) {
    static std::vector<int> vec = {1, 2, 3};
    static std::list<int> lst = {1, 2, 3};

    if (use_list) {
        return lst;
    } else {
        return vec;
    }
}

void func2() {
    // 创建一个 vector 范围
    std::vector<int> vec = {1, 2, 3, 4, 5};

    // 创建一个 string 范围
    std::string str = "Hello World";

    // 创建一个 any_range 对象，它可以容纳 vector 或 string 范围
    boost::any_range<int, boost::forward_traversal_tag> any_vec(vec.begin(), vec.end());
    boost::any_range<char, boost::forward_traversal_tag> any_str(str.begin(), str.end());

    // 使用 any_range
    for (int i : any_vec) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    for (char c : any_str) {
        std::cout << c;
    }
    std::cout << std::endl;

    // 处理 any_range
    if (any_vec.empty()) {
        std::cout << "Vector is empty." << std::endl;
    } else {
        std::cout << "First element: " << *any_vec.begin() << std::endl;
    }

    if (any_str.empty()) {
        std::cout << "String is empty." << std::endl;
    } else {
        std::cout << "First character: " << *any_str.begin() << std::endl;
    }
}

int main() {
    for (const auto & i : make_range(false)) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    return 0;
}
