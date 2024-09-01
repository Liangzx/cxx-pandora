# smartx ptr

## refs

[enable_shared_from_this用法分析](https://www.cnblogs.com/wangshaowei/p/10669646.html)
[智能指针shared_ptr新特性shared_from_this及weak_ptr](https://www.cnblogs.com/wangshaowei/p/10669646.html)
[enable_shared_from_this-overview-examples-and-internals](https://www.nextptr.com/tutorial/ta1414193955/enable_shared_from_this-overview-examples-and-internals)
[shared_ptr initialized with nullptr is null or empty?](https://www.nextptr.com/question/qa1372136808/shared_ptr-initialized-with-nullptr-is-null-or-empty)
[shared_ptr中的owner_before解析（最易懂，最全面）](https://blog.csdn.net/weixin_45590473/article/details/113040456)
[C++：为什么unique_ptr的Deleter是模板类型参数，而shared_ptr的Deleter不是？](https://www.cnblogs.com/fuzhe1989/p/7763623.html)
[std::shared_ptr 代码试读（一）：代码结构](https://github.com/Walton1128/STL-soruce-code-read/blob/main/shared_ptr%20%E4%BB%A3%E7%A0%81%E8%AF%95%E8%AF%BB%EF%BC%88%E4%B8%80%EF%BC%89%EF%BC%9A%E4%BB%A3%E7%A0%81%E7%BB%93%E6%9E%84.md)

## std::enable_shared_from_this

## enable_shared_from_this - overview, examples, and internals

```c++
void good() {
 auto p{new int(10)}; //p is int*
 std::shared_ptr<int> sp1{p};
 //Create additional shared_ptr from an existing shared_ptr
 auto sp2{sp1}; //OK. sp2 shares control block with sp1
}

void bad() {
 auto p{new int(10)};
 std::shared_ptr<int> sp1{p};
 std::shared_ptr<int> sp2{p}; //! Undefined Behavior
}

struct Egg {
 std::shared_ptr<Egg> getSelfPtr() {
  return std::shared_ptr<Egg>(this); //!! Bad
 }
 //...
};

void spam() {
 auto sp1 = std::make_shared<Egg>();
 auto sp2 = sp1->getSelfPtr(); //!! Undefined Behavior
 /*sp1 and sp2 have two different control blocks
     managing same Egg*/
}

// std::enable_shared_from_this<T>
//some API's header
struct Thing;
void some_api(const std::shared_ptr<Thing>& tp);

//------- Different file ----
//Note: public inheritance
struct Thing : std::enable_shared_from_this<Thing> {
 void method() {
  some_api(shared_from_this()); //!! Good.
  //...
 }
};

void foo() {
 auto sp = std::make_shared<Thing>();
 sp->method();
 //..
}

// Why shared_ptr from 'this'?
```
