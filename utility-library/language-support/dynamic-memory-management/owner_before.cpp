#include <iostream>
#include <memory>
#include <set>

struct Foo
{
    int n1;
    int n2;
    Foo(int a, int b) : n1(a), n2(b) {}
};

void func1()
{
    auto p1 = std::make_shared<Foo>(1, 2);
    std::shared_ptr<int> p2(p1, &p1->n1);
    std::shared_ptr<int> p3(p1, &p1->n2);

    std::cout << std::boolalpha
              << "p2 < p3 " << (p2 < p3) << '\n'
              << "p3 < p2 " << (p3 < p2) << '\n'
              << "p2.owner_before(p3) " << p2.owner_before(p3) << '\n'
              << "p3.owner_before(p2) " << p3.owner_before(p2) << '\n';

    std::weak_ptr<int> w2(p2);
    std::weak_ptr<int> w3(p3);
    std::cout
        //            << "w2 < w3 " << (w2 < w3) << '\n' // won't compile
        //            << "w3 < w2 " << (w3 < w2) << '\n' // won't compile
        << "w2.owner_before(w3) " << w2.owner_before(w3) << '\n'
        << "w3.owner_before(w2) " << w3.owner_before(w2) << '\n';
}

struct T
{
    int x, y;
};

void func2()
{
    std::shared_ptr<T> a(new T{});
    std::shared_ptr<T> b = a;

    std::shared_ptr<int> c(a, &a->y); // aliasing constructor

    std::shared_ptr<T> d(a.get(), [](T *) {});           // null deleter to avoid no double delete
    std::shared_ptr<T> e(std::shared_ptr<T>(), a.get()); // "non-owning" empty shared_ptr with a non-null stored pointer
    /**
        a , b, and c are all equivalent according to owner_less because they share ownership.
        d doesn't share ownership with a (it has its own "control block"), so it's different according to owner_less.
        Likewise for e, which is empty (owns nothing) but has a non-null stored pointer.
        The default comparator simply compares the stored pointer, or get().
        It will see a, b, d, and e as all equivalent and c as different
        (assuming that you converted both sides to shared_ptr<void> so that they are comparable).
     */

    std::set<std::shared_ptr<T>> st;
    // std::set<std::shared_ptr<T>, std::owner_less<std::shared_ptr<T>>> st2;
    st.insert(a);
    st.insert(b);
    std::cout << (a == b) << std::endl;  //
    std::cout << st.size() << std::endl; // 1
    std::cout << "==============" << std::endl;
    st.insert(d);
    std::cout << st.size() << std::endl; // st -> 1 st2->2
}

//--------------
struct C
{
    int data_;
};

void func3()
{
    std::shared_ptr<C> obj(new C);             // obj是C类型对象的一个共享指针
    std::cout << obj.use_count() << std::endl; // 1
    std::shared_ptr<int> p9(obj, &obj->data_); // p9是obj的一个共享指针，但指向的是C对象的data数据成员
    std::cout << obj.use_count() << std::endl; // 2
    std::cout << p9.use_count() << std::endl;  // 2
    std::cout << *p9 << std::endl;             // 访问的是obj->data
    std::cout << *(p9.get()) << std::endl;     // 访问的是obj->data
}

int main()
{
    func3();

    return 0;
}
