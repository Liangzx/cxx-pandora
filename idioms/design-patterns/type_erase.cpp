#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>

// https://zhuanlan.zhihu.com/p/125168286
template <typename T> class smartptr_te {
  struct deleter_base {             // deleter基类
    virtual void apply(void *) = 0; // 虚方法，子类实现
    virtual ~deleter_base() = default;
  };

  template <typename Deleter>
  struct deleter : public deleter_base { // 子类deleter
    explicit deleter(Deleter d) : d_(d) {}
    void apply(void *p) override { d_(static_cast<T *>(p)); }
    Deleter d_;
  };

public:
  template <typename Deleter>
  smartptr_te(T *p, Deleter d)
      : p_(p), d_(new deleter<Deleter>(d)) {} // Deleter类型被隐藏
  ~smartptr_te() {
    d_->apply(p_);
    delete d_;
  } // 析构函数。deleter被调用，deleter的类型被擦除。
  T *operator->() { return p_; }
  const T *operator->() const { return p_; }

private:
  T *p_;
  deleter_base *d_; // deleter对象，使用基类作为类型
};

template <typename T> class smartptr_te_lb {
  struct deleter_base {
    virtual void apply(void *) = 0;
    virtual ~deleter_base() {}
  };
  template <typename Deleter> struct deleter : public deleter_base {
    deleter(Deleter d) : d_(d) {}
    virtual void apply(void *p) { d_(static_cast<T *>(p)); }
    Deleter d_;
  };

public:
  template <typename Deleter>
  smartptr_te_lb(T *p, Deleter d)
      : p_(p),
        d_(sizeof(Deleter) > sizeof(buf_) ? new deleter<Deleter>(d)
                                          : new(buf_) deleter<Deleter>(d)) {}
  ~smartptr_te_lb() {
    d_->apply(p_);
    if (static_cast<void *>(d_) == static_cast<void *>(buf_)) {
      d_->~deleter_base();
    } else {
      delete d_;
    }
  }
  T *operator->() { return p_; }
  const T *operator->() const { return p_; }

private:
  T *p_;
  deleter_base *d_;
  char buf_[16];
};

// https://blog.csdn.net/qq_40946921/article/details/90646022

class Any {
public:
  template <typename T>
  explicit Any(T t) : base(new Data<T>(t)) {} // 模板构造函数
  template <typename T> T Cast() {
    return dynamic_cast<Data<T> *>(base.get())->value; // 强制转换
  }

private:
  class Base {
  public:
    virtual ~Base() = default; // 确定Base为多态类型
  };

  template <typename T> class Data : public Base {
  public:
    explicit Data(T t) : value(std::move(t)) {}
    T value;
  };
  std::unique_ptr<Base> base; // 基类指针
};

template <typename T, typename Deleter> class SmartPtr {

public:
  SmartPtr(T *p, Deleter d) : p_(p), d_(d) {}
  ~SmartPtr() { d_(p_); }

private:
  T *p_;
  Deleter d_;
};

template <typename T> class smartptr_te_lb {
  struct deleter_base {
    virtual void apply(void *) = 0;
    virtual ~deleter_base() {}
  };
  template <typename Deleter> struct deleter : public deleter_base {
    deleter(Deleter d) : d_(d) {}
    virtual void apply(void *p) { d_(static_cast<T *>(p)); }
    Deleter d_;
  };

public:
  template <typename Deleter>
  smartptr_te_lb(T *p, Deleter d)
      : p_(p), d_(sizeof(Deleter) > sizeof(buf_)
                      ? new deleter<Deleter>(d)
                      : new(buf_) deleter<Deleter>(d)) // 本地缓存操作
  {}
  ~smartptr_te_lb() {
    d_->apply(p_);
    if (static_cast<void *>(d_) == static_cast<void *>(buf_)) { // 本地缓存操作
      d_->~deleter_base();
    } else {
      delete d_;
    }
  }
  T *operator->() { return p_; }
  const T *operator->() const { return p_; }

private:
  T *p_;
  deleter_base *d_;
  char buf_[16]; // 本地缓存量
};

int main() {
  std::string res = "\njdaufhdau\n\n";

  return 0;
  Any a(std::string("s123"));
  Any b(1);
  Any c{12.0};
  //  smartptr_te<int> str(new int{3}, [](int *) {});
  auto pl = [](const int *p) { delete p; };
  SmartPtr<int, decltype(pl)> sp(new int(13), pl);
  std::map<int, int> mi;
  return 0;
}
