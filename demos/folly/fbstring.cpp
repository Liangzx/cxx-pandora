#include <folly/FBstring.h>
#include <string>

  template <
      typename StringViewLike,
      std::enable_if_t<
          std::is_convertible_v<const StringViewLike&, string_view_type> &&
              !std::is_convertible_v<const StringViewLike&, const value_type*>,
          int> = 0>
  basic_fbstring(
      const StringViewLike& view, size_type pos, size_type n, const A& a = A())
      : basic_fbstring(
            string_view_type(view).substr(pos, n), a, string_view_ctor{}) {}

int main() {
  folly::fbstring s;
  return 0;
}
