#include <boost/format.hpp>
#include <initializer_list>
#include <iostream>

template<typename... Args>
inline std::string formatString(const std::string& format, Args&&... args)
{
  boost::format fmt(format);
  std::initializer_list<int>{(fmt % std::forward<Args>(args), 0)...};
  return fmt.str();
}

int main()
{
  std::string name = "John";
  int age = 30;
  double height = 5.9;

  std::string result =
    formatString("Name: %1%, Age: %2%, Height: %3%", "haha", age, height);

  std::cout << result << std::endl;

  return 0;
}

// g++ main.cpp -lboost_system -lboost_filesystem
