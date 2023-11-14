## src

```c++
template <typename _Res, typename... _ArgTypes>
class function<_Res(_ArgTypes...)>
    : public _Maybe_unary_or_binary_function<_Res, _ArgTypes...>,
      private _Function_base {
private:
  using _Invoker_type = _Res (*)(const _Any_data &, _ArgTypes &&...);
  _Invoker_type _M_invoker;
}

// 
class _Function_base {
public:
  static const size_t _M_max_size = sizeof(_Nocopy_types);
  static const size_t _M_max_align = __alignof__(_Nocopy_types);
  template <typename _Functor> class _Base_manager {
  protected:
    static const bool __stored_locally =
        (__is_location_invariant<_Functor>::value &&
         sizeof(_Functor) <= _M_max_size &&
         __alignof__(_Functor) <= _M_max_align &&
         (_M_max_align % __alignof__(_Functor) == 0));

    typedef integral_constant<bool, __stored_locally> _Local_storage;

    // Retrieve a pointer to the function object
    static _Functor *_M_get_pointer(const _Any_data &__source) {
      if _GLIBCXX17_CONSTEXPR (__stored_locally) {
        const _Functor &__f = __source._M_access<_Functor>();
        return const_cast<_Functor *>(std::__addressof(__f));
      } else // have stored a pointer
        return __source._M_access<_Functor *>();
    }
  };

private:
  typedef bool (*_Manager_type)(_Any_data &, const _Any_data &,
                                _Manager_operation);
  // 在 function 构造函数中进行赋值
  _Any_data _M_functor;
  _Manager_type _M_manager;
};

template <typename _Functor,
          typename = _Requires<__not_<is_same<_Functor, function>>, void>,
          typename = _Requires<_Callable<_Functor>, void>>
function(_Functor);
// 类模板函数
template <typename _Res, typename... _ArgTypes>
template <typename _Functor, typename, typename>
function<_Res(_ArgTypes...)>::function(_Functor __f) : _Function_base() {
  typedef _Function_handler<_Res(_ArgTypes...), _Functor> _My_handler;
  if (_My_handler::_M_not_empty_function(__f)) {
    _My_handler::_M_init_functor(_M_functor, std::move(__f));
    _M_invoker = &_My_handler::_M_invoke;
    _M_manager = &_My_handler::_M_manager;
  }
}
// typedef _Function_handler<_Res(_ArgTypes...), _Functor> _My_handler;
template <typename _Functor, typename... _ArgTypes>
class _Function_handler<void(_ArgTypes...), _Functor>
    : public _Function_base::_Base_manager<_Functor> {
  typedef _Function_base::_Base_manager<_Functor> _Base;

public:
  static void _M_invoke(const _Any_data &__functor, _ArgTypes &&...__args) {
    (*_Base::_M_get_pointer(__functor))(std::forward<_ArgTypes>(__args)...);
  }
};
  // call  sf(12);
template<typename _Res, typename... _ArgTypes>
  _Res
  function<_Res(_ArgTypes...)>::
  operator()(_ArgTypes... __args) const
  {
    if (_M_empty())
      __throw_bad_function_call();
    return _M_invoker(_M_functor, std::forward<_ArgTypes>(__args)...);
  }
```

## notes
- 不带捕获值的 lambda 只占 1 字节；
- 成员函数指针占用 2 个指针大小。

## refs:
 - [c++ 不能不知的技术内幕| std::function](https://mp.weixin.qq.com/s/sd1fPBqb8oBduSGtAWy7Sg)