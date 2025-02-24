// shared_ptr中的owner_before解析（最易懂，最全面）

// shared_ptr中的owner_before成员函数的功能为“判断两个指针是否指向同一对象”
/**
 * shared_ptr中的owner_pointer和stored_pointer
 * 1. owner pointer所有权拥有指针
 *  所有权拥有指针顾名思义就是指“这个指针指向整个自定义数据类型的对象而非该对象的成员数据”
 * 2. stored pointer指向存储在内部的数据的指针
 *  看名称也可以很清晰的理解：这个指针指向自定义数据类型对象的一部分。
 */

/**
 * 什么是“C++中的弱序”？
 * 1. ptr和ptr1是不是同类型指针；
 * 2. ptr指针指向的地址小于ptr1指针指向的地址；
 * 满足以上两种条件，我们就可以称“ptr<ptr1”。我们很可能想到“求同存异”，
 * 这四个字很符合“C++中弱序”的特点，因此我们要理解弱序就必须要理解“弱序到底求得什么同,
 * 存的什么异”！
 */

#include <iostream>
using namespace std;
#include <memory>

struct A {
  int age;
  double mark;
};

// shared_ptr有两种属性，因此ptr与ptr1的搭配方式有四种
// 1. ptr1是stored_pointer而ptr是owner_pointer
void func1() {
  // ptr A类类型对象
  shared_ptr<A> ptr = make_shared<A>();
  // ptr1 A类类型对象中的age成员数据
  shared_ptr<int> ptr1(ptr, &ptr->age);
  // “弱序”中，将指向同一对象的shared_ptr指针作为同一类型的指针
  cout << "ptr指针指向的地址为" << ptr.get() << endl;
  cout << "ptr1指针指向的地址为" << ptr1.get() << endl;
  // 由于ptr和ptr1是同类型指针而且ptr指针等于ptr1指针指向地址
  cout << "ptr1.owner_before(ptr) = " << ptr1.owner_before(ptr) << endl;  // 0
  cout << "ptr.owner_before(ptr1) = " << ptr.owner_before(ptr1) << endl;  // 0
}

// 2. ptr和ptr1指针均为stored_pointer属性的shared_ptr指针
void func2() {
  shared_ptr<A> ptr = make_shared<A>();
  shared_ptr<int> ptr1(ptr, &ptr->age);
  shared_ptr<double> ptr2(ptr, &ptr->mark);
  // 地址不同
  cout << "ptr1指向的地址为" << ptr1.get() << endl;
  cout << "ptr2指向的地址为" << ptr2.get() << endl;
  // ptr1与ptr2指向的地址此时不一样了，但是无论参数位置如何，owner_before成员函数返回值仍为0,
  // 这说明“指向同一个自定义类类型对象中不同数据成员的两个stored_pointer属性的shared_ptr指针在‘弱序’中属于同一类型的指针”.
  cout << "ptr1.owner_before(ptr2) = " << ptr1.owner_before(ptr2) << endl;  // 0
  cout << "ptr2.owner_before(ptr1) = " << ptr2.owner_before(ptr1) << endl;  // 0
}

// 3. ptr与ptr1均属于owner_pointer属性的shared_ptr指针
struct B {
  int ageB;
  double markB;
};

struct AB : public A, public B {};
void func3() {
  shared_ptr<B> ptr1 = make_shared<B>();
  shared_ptr<A> ptr = make_shared<A>();
  shared_ptr<AB> ptr2 = make_shared<AB>();
  cout << "ptr指向的地址为" << ptr.get() << endl;
  cout << "ptr1指向的地址为" << ptr1.get() << endl;
  cout << "ptr2指向的地址为" << ptr2.get() << endl;
  cout << "ptr(指向class A的对象)与ptr1(指向class B的对象)" << endl;
  cout << "ptr > ptr1 = " << ((int *)ptr.get() > (int *)ptr1.get()) << endl;
  cout << "ptr < ptr1 = " << ((int *)ptr.get() < (int *)ptr1.get()) << endl;
  cout << "ptr.owner_before(ptr1)" << ptr.owner_before(ptr1) << endl;
  cout << "ptr1.owner_before(ptr)" << ptr1.owner_before(ptr) << endl;

  cout << "ptr(指向class A的对象)与ptr2(指向class AB的对象)" << endl;
  cout << "ptr > ptr2 = " << ((int *)ptr.get() > (int *)ptr2.get()) << endl;
  cout << "ptr < ptr2 = " << ((int *)ptr.get() < (int *)ptr2.get()) << endl;
  cout << "ptr.owner_before(ptr2)" << ptr.owner_before(ptr2) << endl;
  cout << "ptr2.owner_before(ptr)" << ptr2.owner_before(ptr) << endl;

  cout << "ptr1(指向class B的对象)与ptr2(指向class AB的对象)" << endl;
  cout << "ptr1 > ptr2 = " << ((int *)ptr1.get() > (int *)ptr2.get()) << endl;
  cout << "ptr1 < ptr2 = " << ((int *)ptr1.get() < (int *)ptr2.get()) << endl;
  cout << "ptr1.owner_before(ptr2)" << ptr1.owner_before(ptr2) << endl;
  cout << "ptr2.owner_before(ptr1)" << ptr2.owner_before(ptr1) << endl;
}

// 对owner_before成员函数进行总结
/**
 * 我们使用“shared_ptr/weak_ptr中的owner_before成员函数“去判断两个指针是否指向”
 * 同一对象“并不想知道”两个指针指向地址的先后顺序“，因此我们可以这样做”先使用C++逻辑
 * 运算中的逻辑或操作(||)，然后再总体上取个非(!)操作“：
 */

/**
 * 当我们将ptr.owner_before(ptr1)的返回结果与ptr1.owner_before(ptr)的返回结果进行逻辑或(||)操作后，
 * (ptr.owner_before(ptr1)||ptr1.owner_before(ptr))这个整体返回的值就排除了“ptr与ptr1两个指针指向地址先后“的干扰，因此如果”
 * (ptr.owner_before(ptr1)||ptr1.owner_before(ptr))
“返回0就代表ptr与ptr1指针指向同一个类型的对象。类比：a 不小 b，且 b 不小于 a 则 a == b
* 但是这样的看有些不爽，我们想要“当“ptr与ptr1指针指向同一个类型的对象”时，返回true，
* 你却此时给我返回false，那么我就取个反就OK了，
* 因此最终用于判断两个指针是否指向同一个类型对象的代码为“!(ptr.owner_before(ptr1)||ptr1.owner_before(ptr))”。
 */

void func4() {
  shared_ptr<A> ptr = make_shared<A>();
  shared_ptr<int> ptr1(ptr, &ptr->age);
  cout << "ptr与ptr1指针是否指向同一对象："
       << !(ptr.owner_before(ptr1) || ptr1.owner_before(ptr)) << endl;
}

int main() {
  func4();

  return 0;
}

// https://www.cnblogs.com/youxue-helloworld/p/12993810.html
