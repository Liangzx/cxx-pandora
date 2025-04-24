#include <iostream>
#include <type_traits>
#include <tuple>
#include <string>

template <typename... Args>
class ArgumentHolder {
    using TupleType = std::tuple<std::decay_t<Args>...>;
    TupleType args;

public:
    ArgumentHolder(Args&&... args)
        : args(std::forward<Args>(args)...) {}

    template <typename Callable>
    auto apply(Callable&& func) {
        return std::apply(std::forward<Callable>(func), args);
    }
};

int main() {
    // // 方案1：匹配存储类型
    // ArgumentHolder holder1(42, std::string("hello"), 3.14);
    // holder1.apply([](int x, const std::string& s, double d) {
    //     std::cout << x << ", " << s << ", " << d << "\n";
    // });

    // 方案2：使用字符串字面量
    ArgumentHolder holder2(42, static_cast<const char*>("hello"), 3.14);
    holder2.apply([](int x, const char* s, double d) {
        std::cout << x << ", " << s << ", " << d << "\n";
    });

    // // 方案3：通用lambda
    // ArgumentHolder holder3(42, "hello", 3.14);
    // holder3.apply([](auto x, auto s, auto d) {
    //     std::cout << x << ", " << s << ", " << d << "\n";
    // });
}
