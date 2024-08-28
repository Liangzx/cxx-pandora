# shared_ptr

## src

```c++
template<typename _Yp>
shared_ptr(const shared_ptr<_Yp>& __r, element_type* __p) noexcept
 : __shared_ptr<_Tp>(__r, __p) { }

// c++/11/bits/shared_ptr_base.h

// Aliasing constructor
template<typename _Yp>
__shared_ptr(const __shared_ptr<_Yp, _Lp>& __r,
      element_type* __p) noexcept
: _M_ptr(__p), _M_refcount(__r._M_refcount) // never throws
{

  element_type*    _M_ptr;              // Contained pointer.
  __shared_count<_Lp>  _M_refcount;     // Reference counter.
};

// c++/11/bits/shared_ptr_base.h
/// Exchange both the owned pointer and the stored pointer.
void
swap(__shared_ptr<_Tp, _Lp>& __other) noexcept
{
 // _M_ptr ==> stored pointer
 std::swap(_M_ptr, __other._M_ptr);
 // _M_refcount ==> owned pointer
 _M_refcount._M_swap(__other._M_refcount);
}

// /usr/include/c++/9/bits/shared_ptr_base.h
// __enable_shared_from_this
template <typename _Tp, _Lock_policy _Lp>
class __enable_shared_from_this {
protected:
  constexpr __enable_shared_from_this() noexcept {}
  // ...
  _M_weak_assign(_Tp1 *__p, const __shared_count<_Lp> &__n) const noexcept {
    _M_weak_this._M_assign(__p, __n);
  }
};

// __shared_ptr
template <typename _Tp, _Lock_policy _Lp>
class __shared_ptr : public __shared_ptr_access<_Tp, _Lp> {
  //
  __shared_ptr(_Sp_alloc_shared_tag<_Alloc> __tag, _Args &&...__args)
      : _M_ptr(), _M_refcount(_M_ptr, __tag, std::forward<_Args>(__args)...) {
    _M_enable_shared_from_this_with(_M_ptr);
  }
 // ...
template <typename _Yp, typename _Yp2 = typename remove_cv<_Yp>::type>
typename enable_if<__has_esft_base<_Yp2>::value>::type
_M_enable_shared_from_this_with(_Yp *__p) noexcept {
  if (auto __base = __enable_shared_from_this_base(_M_refcount, __p))
    __base->_M_weak_assign(const_cast<_Yp2 *>(__p), _M_refcount);
}
  // ...
  element_type *_M_ptr;            // Contained pointer.
  __shared_count<_Lp> _M_refcount; // Reference counter.
};

template<_Lock_policy _Lp>
class __shared_count
{
  // ...
  _Sp_counted_base<_Lp>*  _M_pi;
};

// 所以 shared_ptr赋值的时候计数会加1，且如果原来的不为空，会释放原来的
__shared_count&
      operator=(const __shared_count& __r) noexcept
      {
 _Sp_counted_base<_Lp>* __tmp = __r._M_pi;
 if (__tmp != _M_pi)
   {
     if (__tmp != 0)
       __tmp->_M_add_ref_copy();
     if (_M_pi != 0)  // _Sp_counted_base
       _M_pi->_M_release(); // _Sp_counted_base::_M_release
     _M_pi = __tmp;
   }
 return *this;
}

template<_Lock_policy _Lp = __default_lock_policy>
class _Sp_counted_base
: public _Mutex_base<_Lp>
{
  public:
    _Sp_counted_base() noexcept
      : _M_use_count(1), _M_weak_count(1) { }
     void
      _M_release() noexcept
      {
        // Be race-detector-friendly.  For more info see bits/c++config.
        _GLIBCXX_SYNCHRONIZATION_HAPPENS_BEFORE(&_M_use_count);
 if (__gnu_cxx::__exchange_and_add_dispatch(&_M_use_count, -1) == 1)
   {
            _GLIBCXX_SYNCHRONIZATION_HAPPENS_AFTER(&_M_use_count);
     _M_dispose();
     // There must be a memory barrier between dispose() and destroy()
     // to ensure that the effects of dispose() are observed in the
     // thread that runs destroy().
     // See http://gcc.gnu.org/ml/libstdc++/2005-11/msg00136.html
     if (_Mutex_base<_Lp>::_S_need_barriers)
       {
  __atomic_thread_fence (__ATOMIC_ACQ_REL);
       }

            // Be race-detector-friendly.  For more info see bits/c++config.
            _GLIBCXX_SYNCHRONIZATION_HAPPENS_BEFORE(&_M_weak_count);
     if (__gnu_cxx::__exchange_and_add_dispatch(&_M_weak_count,
             -1) == 1)
              {
                _GLIBCXX_SYNCHRONIZATION_HAPPENS_AFTER(&_M_weak_count);
         _M_destroy();
              }
   }
      }
  // ...
  Atomic_word  _M_use_count;     // #shared
  _Atomic_word  _M_weak_count;    // #weak + (#shared != 0)
};

// __weak_ptr
template <typename _Tp, _Lock_policy _Lp> class __weak_ptr {

private:
  // Used by __enable_shared_from_this.
  void _M_assign(_Tp *__ptr, const __shared_count<_Lp> &__refcount) noexcept {
    if (use_count() == 0) {
      _M_ptr = __ptr;
      _M_refcount = __refcount;
    }
  }

  element_type *_M_ptr;          // Contained pointer.
  __weak_count<_Lp> _M_refcount; // Reference counter.
};

template <typename _Tp, _Lock_policy _Lp> class __enable_shared_from_this {
public:
  __shared_ptr<_Tp, _Lp> shared_from_this() {
    return __shared_ptr<_Tp, _Lp>(this->_M_weak_this);
  }

  mutable __weak_ptr<_Tp, _Lp> _M_weak_this;
};

/*
 1. shared_ptr 在赋值时会对 __weak_ptr 进行赋值
 2. 如果用了 enable_shared_from_this, 那么对象应该使用 shared_ptr 进行管理
**/
```
