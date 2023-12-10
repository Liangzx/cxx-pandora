#include <type_traits>
#include <iostream>
#include <string>

using namespace std;

/********* enable_if应用于函数模板: 典型应用是作为函数模板的返回类型 **********/
template <typename T> typename std::enable_if<(sizeof(T) > 2)>::type func() {}

/************************* enable_if用于类模板中 ***************************/
class Human {
  // 给类型模板定义一个别名(别名模板)
  template <typename T>
  using StrProcType =
      std::enable_if_t<std::is_convertible<T, std::string>::value>;

public:
  // 构造函数模板
  template <typename T, typename = StrProcType<T>>
  
  Human(T &&tmpname) : m_sname(std::forward<T>(tmpname)) {
    cout << "Human(T&& tmpname)执行" << endl;
  }
  // 拷贝构造函数
  Human(const Human &th) : m_sname(th.m_sname) {
    cout << "Human(const Human& th)拷贝构造函数执行" << endl;
  }

  // 移动构造函数
  Human(Human &&th) : m_sname(std::move(th.m_sname)) {
    cout << "Human(Human&& th)移动构造函数执行" << endl;
  }

private:
  string m_sname;
};

/**************************************************************************/
int main() {
  func<int>();
  string sname = "ZhangSan";
  Human myhuman1(sname);

  Human myhuman3(myhuman1);
  return 0;
}