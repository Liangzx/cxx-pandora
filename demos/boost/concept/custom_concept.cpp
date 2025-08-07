#include <boost/concept_check.hpp>
#include <iostream>

// 自定义概念：必须有print()方法
template<typename T>
struct Printable
{
    BOOST_CONCEPT_USAGE(Printable)
    {
        obj.print();
    }

private:
    T obj;
};

// 使用概念的模板函数
template<typename T>
void print_object(const T& obj)
{
    BOOST_CONCEPT_ASSERT((Printable<T>));
    obj.print();
}

// 符合概念的类型
class MyPrintable
{
public:
    void print() const
    {
        std::cout << "MyPrintable object" << std::endl;
    }
};

// 不符合概念的类型
class NonPrintable
{
};

int main()
{
    MyPrintable p;
    print_object(p); // 正常编译

    // NonPrintable np;
    // print_object(np);  // 编译错误，不符合Printable概念

    return 0;
}
