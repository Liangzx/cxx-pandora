#include <boost/concept_check.hpp>

// 定义一个概念：要求类型 T 必须具有 `doSomething` 成员函数
template <typename T>
struct HasDoSomething {
    // 用 BOOST_CONCEPT_USAGE 定义检查逻辑
    BOOST_CONCEPT_USAGE(HasDoSomething) {
        // 检查 T 是否有 doSomething 成员函数
        T::doSomething(); // 若 T 没有该函数，编译时会报错
    }
};

// 使用概念断言检查类型
template <typename T>
void foo(T t) {
    // BOOST_CONCEPT_ASSERT((HasDoSomething<T>)); // 若 T 不满足 HasDoSomething，编译失败
    typedef ::boost::concepts::detail::instantiate<
        &::boost::concepts::requirement_<void (*)(HasDoSomething<T>)>::failed>
        boost_concept_check16 __attribute__((__unused__));
}

// 假设有一个类 MyClass 满足 HasDoSomething 概念
class MyClass {
public:
    // static void doSomething() {}
};


int main() {
    MyClass myObj;
    foo(myObj); // 这将通过编译，因为 MyClass 满足 HasDoSomething 概念

    return 0;
}
