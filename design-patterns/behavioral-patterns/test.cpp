template <typename _Tp, typename _Alloc = std::allocator<_Tp>>
class vector : protected _Vector_base<_Tp, _Alloc> {
  // ...
  typedef _Vector_base<_Tp, _Alloc> _Base;
  typedef typename _Base::_Tp_alloc_type _Tp_alloc_type;
  typedef __gnu_cxx::__alloc_traits<_Tp_alloc_type> _Alloc_traits;

public:
  typedef _Tp value_type;
  typedef typename _Base::pointer pointer;
  typedef typename _Alloc_traits::const_pointer const_pointer;
  typedef typename _Alloc_traits::reference reference;
  typedef typename _Alloc_traits::const_reference const_reference;
  typedef __gnu_cxx::__normal_iterator<pointer, vector> iterator;
  typedef __gnu_cxx::__normal_iterator<const_pointer, vector> const_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef _Alloc allocator_type;
  // ...
};

template <typename _Tp, typename _Alloc> struct _Vector_base {
  typedef
      typename __gnu_cxx::__alloc_traits<_Alloc>::template rebind<_Tp>::other
          _Tp_alloc_type;
  typedef typename __gnu_cxx::__alloc_traits<_Tp_alloc_type>::pointer pointer;
  // ...
  struct _Vector_impl_data {
    pointer _M_start;
    pointer _M_finish;
    pointer _M_end_of_storage;

    // ...
    struct _Vector_impl : public _Tp_alloc_type,
                          public _Vector_impl_data { // ...
    };

    //...
  public:
    _Vector_impl _M_impl;
    _Vector_impl _M_impl;

    pointer _M_allocate(size_t __n) {
      typedef __gnu_cxx::__alloc_traits<_Tp_alloc_type> _Tr;
      return __n != 0 ? _Tr::allocate(_M_impl, __n) : pointer();
    }

    void _M_deallocate(pointer __p, size_t __n) {
      typedef __gnu_cxx::__alloc_traits<_Tp_alloc_type> _Tr;
      if (__p)
        _Tr::deallocate(_M_impl, __p, __n);
    }
  };
};

template <typename _Alloc, typename = typename _Alloc::value_type>
struct __alloc_traits : std::allocator_traits<_Alloc> {};

template <typename _Alloc> struct allocator_traits : __allocator_traits_base {
  // ...
  template <typename _Tp, typename _Up, typename = void>
  struct __rebind : __replace_first_arg<_Tp, _Up> {};

  template <typename _Tp, typename _Up>
  struct __rebind<_Tp, _Up,
                  __void_t<typename _Tp::template rebind<_Up>::other>> {
    using type = typename _Tp::template rebind<_Up>::other;
  };
};

template <typename _Tp> class allocator : public __allocator_base<_Tp> {
public:
  typedef _Tp value_type;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  // ...
  template <typename _Tp1> struct rebind {
    typedef allocator<_Tp1> other;
  };
};

template <typename _Tp> using __allocator_base = __gnu_cxx::new_allocator<_Tp>;

template <typename _Tp> class new_allocator {
public:
  typedef _Tp value_type;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  // ...
  template <typename _Tp1> struct rebind {
    typedef new_allocator<_Tp1> other;
  };
  // ...
  _GLIBCXX_NODISCARD _Tp *
  allocate(size_type __n, const void * = static_cast<const void *>(0)) {
    // ...
    return static_cast<_Tp *>(::operator new(__n * sizeof(_Tp)));
  }
  // __p is not permitted to be a null pointer.
  void deallocate(_Tp *__p, size_type __t __attribute__((__unused__))) {
    // ...
    ::operator delete(__p, ...);
  }
  // ...
  template <typename _Up, typename... _Args>
  void construct(_Up *__p, _Args &&...__args) noexcept(
      std::is_nothrow_constructible<_Up, _Args...>::value) {
    ::new ((void *)__p) _Up(std::forward<_Args>(__args)...);
  }
  //...
  template <typename _Up>
  void destroy(_Up *__p) noexcept(std::is_nothrow_destructible<_Up>::value) {
    __p->~_Up();
  }
};