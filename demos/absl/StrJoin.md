
### 函数签名

```c++
template <typename Iterator, typename Formatter>
std::string StrJoin(Iterator start, Iterator end, absl::string_view sep,
                    Formatter&& fmt) {/*...*/}
    
template <typename Range, typename Formatter>
std::string StrJoin(const Range& range, absl::string_view separator,
                    Formatter&& fmt) {/*...*/}

template <typename... T>
std::string StrJoin(const std::tuple<T...>& value,
                    absl::string_view separator) {/* ...*/}                    
                    
    
// ...
```

StrJoin 为函数模板，其参数可以是 Iterator, std::tuple, std::initializer_list 等。
以tuple为例其调用过程：
```c++
std::string s = absl::StrJoin(std::make_tuple(123, "abc", 0.456), "-")
template <typename... T>

std::string StrJoin(const std::tuple<T...>& value,
                    absl::string_view separator) {
  return strings_internal::JoinAlgorithm(value, separator, AlphaNumFormatter());
}

inline strings_internal::AlphaNumFormatterImpl AlphaNumFormatter() {
  return strings_internal::AlphaNumFormatterImpl();
}

template <typename... T, typename Formatter>
std::string JoinAlgorithm(const std::tuple<T...>& tup, absl::string_view sep,
                          Formatter&& fmt) {
  std::string result;
  JoinTupleLoop<0, sizeof...(T)>()(&result, tup, sep, fmt);
  return result;
}
// 当 I = N 时，直接放回
// 当 I != N 时，递归调用
// fmt 起到格式转换的作用，比如将 int 转成 string
//
template <size_t I, size_t N>
struct JoinTupleLoop {
  template <typename Tup, typename Formatter>
  void operator()(std::string* out, const Tup& tup, absl::string_view sep,
                  Formatter&& fmt) {
    if (I > 0) out->append(sep.data(), sep.size());
    fmt(out, std::get<I>(tup));
    JoinTupleLoop<I + 1, N>()(out, tup, sep, fmt);
  }
};

template <size_t N>
struct JoinTupleLoop<N, N> {
  template <typename Tup, typename Formatter>
  void operator()(std::string*, const Tup&, absl::string_view, Formatter&&) {}
};

// AlphaNumFormatterImpl 实现了operator()，既可以像函数调用一样使用对象
// AlphaNum 单参构造实现类型转换
struct AlphaNumFormatterImpl {
  // This template is needed in order to support passing in a dereferenced
  // vector<bool>::iterator
  template <typename T>
  void operator()(std::string* out, const T& t) const {
    StrAppend(out, AlphaNum(t));
  }

  void operator()(std::string* out, const AlphaNum& t) const {
    StrAppend(out, t);
  }
};

class AlphaNum {
    // ...
      AlphaNum(unsigned int x)  // NOLINT(runtime/explicit)
      : piece_(digits_, static_cast<size_t>(
                            numbers_internal::FastIntToBuffer(x, digits_) -
                            &digits_[0])) {}
    // ...
}

// str_cat.cc
void StrAppend(std::string* dest, const AlphaNum& a) {
  ASSERT_NO_OVERLAP(*dest, a);
  dest->append(a.data(), a.size());
}
```