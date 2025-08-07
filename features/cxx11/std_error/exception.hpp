// https://juejin.cn/post/6844903492897751054
// boost_safe_numerics

#include <algorithm>
#include <cassert>
#include <cstdint> // std::uint8_t
#include <string>
#include <system_error> // error_code, system_error

namespace lzx {

enum class safe_numerics_error : std::uint8_t {
  success = 0,
  positive_overflow_error,  // result is above representational maximum
  negative_overflow_error,  // result is below representational minimum
  domain_error,             // one operand is out of valid range
  range_error,              // result cannot be produced for this operation
  precision_overflow_error, // result lost precision
  underflow_error,          // result is too small to be represented
  negative_value_shift,     // negative value in shift operator
  negative_shift,           // shift a negative value
  shift_too_large,          // l/r shift exceeds variable size
  uninitialized_value       // creating of uninitialized value
};

const class : public std::error_category {
public:
  virtual const char *name() const noexcept { return "safe numerics error"; }
  virtual std::string message(int ev) const {
    switch (static_cast<safe_numerics_error>(ev)) {
    case safe_numerics_error::success:
      return "success";
    case safe_numerics_error::positive_overflow_error:
      return "positive overflow error";
    case safe_numerics_error::negative_overflow_error:
      return "negative overflow error";
    case safe_numerics_error::underflow_error:
      return "underflow error";
    case safe_numerics_error::range_error:
      return "range error";
    case safe_numerics_error::domain_error:
      return "domain error";
    case safe_numerics_error::negative_shift:
      return "negative shift";
    case safe_numerics_error::negative_value_shift:
      return "negative value shift";
    case safe_numerics_error::shift_too_large:
      return "shift too large";
    case safe_numerics_error::uninitialized_value:
      return "uninitialized value";
    default:
      assert(false);
    }
    return ""; // suppress bogus warning
  }
} safe_numerics_error_category{};

// constexpr - damn, can't use constexpr due to std::error_code
inline std::error_code make_error_code(const safe_numerics_error &e) {
  return std::error_code(static_cast<int>(e), safe_numerics_error_category);
}

// actions for error_codes for safe numerics.  I've leveraged on
// error_condition in order to do this.  I'm not sure this is a good
// idea or not.

enum class safe_numerics_actions {
  no_action = 0,
  uninitialized_value,
  arithmetic_error,
  implementation_defined_behavior,
  undefined_behavior
};

} // namespace lzx

namespace std {
template <>
struct is_error_code_enum<lzx::safe_numerics_error> : public true_type {};

} // namespace std


// ---------------------------------------------------------------------------------------
/**
 * make_error_code 不必写在 std 中,为什么？
 * 核心原因：ADL（Argument-Dependent Lookup）机制
 * 1.ADL的工作原理：
 * 当调用一个函数时，C++不仅会在当前作用域查找，还会在参数所属的命名空间中查找
 * 对于 make_error_code(my_error) 这样的调用，编译器会在 my_error 的类型所属命名空间中查找 make_error_code。
 *
 * 2. 标准库的设计意图：
 * // 标准库只声明接口，不强制实现位置
    namespace std {
    template <class T>
    struct is_error_code_enum : public false_type {};

    // 声明但不定义make_error_code
    error_code make_error_code(...);
    }

    3. 用户扩展的实现方式
    namespace mylib {
    enum class my_error { ... };

    // 3.1. 特化类型特征
    template <>
    struct std::is_error_code_enum<mylib::my_error> : std::true_type {};

    // 3.2. 在自己的命名空间实现
    std::error_code make_error_code(my_error e) {
        return {static_cast<int>(e), my_category()};
        }
    }

 * 4. 为什么这样设计更好？
  4.1 避免污染std命名空间：
    C++标准禁止用户向std添加完全新的模板/类
    但允许特化标准库已有的模板（如 is_error_code_enum）

  4.2 更好的封装性：
    错误类型相关的代码（包括工厂函数）应与其类型放在同一命名空间
    符合"相关代码放在一起"的软件工程原则

  4.3 扩展性优势：
    // 使用示例
    void foo() {
        auto ec = make_error_code(mylib::my_error::invalid); // 正确调用mylib中的版本
        std::cout << ec.message();
    }
    编译器通过ADL自动找到 mylib::make_error_code

 *
 */
