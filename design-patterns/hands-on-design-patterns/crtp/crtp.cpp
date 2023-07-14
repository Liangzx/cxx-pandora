//
// Created by dingjia on 2022/12/28.
//
#include <iostream>

template<typename D>
class B {
public:
    B() : i_(0) {}
    virtual ~B() = default;
    /**
     * 1. 从基类对象的角度来看，派生类对象其实就是本身
     * 2. 普通基类转派生类用的是dynamic_cast动态绑定
     * 3. 动态绑定的目的是为了确保你所转化的派生类是正确的，而对于CRTP来说，派生类是继承于模板类的参数，也就是派生类本身
     * Ps : D 继承于 B<D> ==> 这里的 this 为 B<D>* 此处将 B<D>* 转成 D* s使用 static_cast
     */
    void f(int i) { static_cast<D *>(this)->f(i); }
    [[nodiscard]] int get() const { return i_; }

protected:
    int i_;
};

class D : public B<D> {
public:
    void f(int i) {
        std::cout << "D: " << i << std::endl;
        i_ += i;
    }
};

int main() {
    B<D> b;
    b.f(123);
    std::cout << b.get() << std::endl;

    return 0;
}