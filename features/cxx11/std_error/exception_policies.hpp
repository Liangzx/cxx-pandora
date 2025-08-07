#include "exception.hpp"

namespace lzx {

template <typename AE, typename IDB, typename UB, typename UV>
struct exception_policy {
  static constexpr void on_arithmetic_error(const safe_numerics_error &e,
                                            const char *msg) {
    AE(e, msg);
  }

  static constexpr void
  on_implementation_defined_behavior(const safe_numerics_error &e,
                                     const char *msg) {
    IDB(e, msg);
  }

  static constexpr void on_undefined_behavior(const safe_numerics_error &e,
                                              const char *msg) {
    UB(e, msg);
  }

  static constexpr void on_uninitialized_value(const safe_numerics_error &e,
                                               const char *msg) {
    UV(e, msg);
  }
};

// ignore any error and just return.
struct ignore_exception {
  constexpr ignore_exception(const safe_numerics_error &, const char *) {}
};

// If an exceptional condition is detected at runtime throw the exception.
struct throw_exception {
  throw_exception(const safe_numerics_error &e, const char *message) {
    throw std::system_error(std::error_code(e), message);
  }
};

constexpr safe_numerics_actions
make_safe_numerics_action(const safe_numerics_error & e){
    // we can't use standard algorithms since we want this to be constexpr
    // this brute force solution is simple and pretty fast anyway
    switch(e){
    case safe_numerics_error::negative_overflow_error:
    case safe_numerics_error::underflow_error:
    case safe_numerics_error::range_error:
    case safe_numerics_error::domain_error:
    case safe_numerics_error::positive_overflow_error:
    case safe_numerics_error::precision_overflow_error:
        return safe_numerics_actions::arithmetic_error;

    case safe_numerics_error::negative_value_shift:
    case safe_numerics_error::negative_shift:
    case safe_numerics_error::shift_too_large:
        return safe_numerics_actions::implementation_defined_behavior;

    case safe_numerics_error::uninitialized_value:
        return safe_numerics_actions::uninitialized_value;

    case safe_numerics_error::success:
        return safe_numerics_actions::no_action;
    default:
        assert(false);
    }
    // should never arrive here
    //include to suppress bogus warning
    return safe_numerics_actions::no_action;
}


} // namespace lzx
