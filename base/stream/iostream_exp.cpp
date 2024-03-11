#include <vector>
#include <iostream>
#include <sstream>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int func1() {
  int i;
  char str[20];
  string s1;
  cout << "请输入一个字符串：";
  cin >> s1;
  cout << "s1 = " << s1;
  cout << "请输入一个整型及一个字符串：";
  cin >> i >> str;
  cout << "i = " << i << endl;
  cout << "str = " << str << endl;
  char str1[20];
  cout << "请输入一个字符串：";
  cin >> str1;
  cout << "str1 = " << str1 << endl;
  return 0;
}

int func2() {
  char szBuf[60];
  char szBuf1[60];
  cout << "请输入一个字符串：";
  //   int n = cin.get(); // 特殊情况可使用get提取'\n'
  //   cin.get(szBuf1, 60);
  cin.getline(szBuf, 60);
  //   cout << n << endl;
  cout << "The received string is:" << szBuf << endl;
  //   cout << "The received string is:" << szBuf1 << endl;
  return 0;
}

// get 和 getline 区别
// 当遇到输入流中的界定符（delim）时，get（）停止执行，但是并不从输入流中提取界定符，直接在字符串缓冲区尾部加结束标志'\0';
// 函数getline()相反，它将从输入流中提取界定符，但不会把它存储到结果缓冲区中。
int func3() {
  char buf[80];
  ifstream in{"./a.txt"};
  if (!in) {
    cout << "fail to open a.txt" << endl;
    return EXIT_FAILURE;
  }
  while (in.getline(buf, sizeof(buf))) {
    //   while (in.get(buf, sizeof(buf))) {
    cout << buf << endl;
  }
  in.close();
  return EXIT_SUCCESS;
}

int func4() {
  std::string s = {"hello"};
  s.append(5, '\0');
  std::cout << s.size() << std::endl;
  std::stringstream ss;
  ss.write(s.c_str(), s.size());
  // NOTE: 注意这里s3的初始化，否则s3[0]为nullptr
  std::string s3(s.length(), '\0');
//   std::string s3;
  ss.seekg(0, std::ios::beg);
  ss.read(&s3[0], s.length());
  std::cout << "s3len:" << s3.length() << std::endl;

  return 0;
}

int main() {
  //   func1();
  //   func2();
  //   func3();
  func4();

  return 0;
}
