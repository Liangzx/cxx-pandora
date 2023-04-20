#include <iostream>


// https://www.ruanyifeng.com/blog/2013/05/Knuth%E2%80%93Morris%E2%80%93Pratt_algorithm.html
// https://www.jianshu.com/p/fe107b4e4271
int kmp(const std::string &str, const std::string &pat) {

}

// https://www.jianshu.com/p/9828bf488a39
int BruteForce(const std::string &str, const std::string &pat) {
  auto m = str.size();
  auto n = pat.size();
  int i = 0;
  int j = 0;
  while (i < m && j < n) {
    if (str[i] == pat[j]) {
      ++i;
      ++j;
    } else {
      i = i - j + 1;
      j = 0;
    }
  }
  if (j == n) {
    return i - j;
  }

  return -1;
}

int main() {
  std::string txt = "hdahelloufhda";
  std::string pat = "hello";

  std::cout << BruteForce(txt, pat) << std::endl;
  return 0;
}