#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
using namespace std;

template <typename T> T &Sort(T &v) {
  sort(v.begin(), v.end());
  return v;
}

template <typename T> T &Reverse(T &v) {
  reverse(v.begin(), v.end());
  return v;
}

template <typename T> T &Print(T &v) {
  for (auto &i : v) {
    cout << i << " ";
  }
  cout << endl;
  return v;
}

template <typename T, typename U> T &operator|(T &v, U fn) {
  fn(v);
  return v;
}

int main() {
  vector<int> vec{3, 2, 1, 5, 4};

  vec | Sort<vector<int>> | Reverse<vector<int>> | Print<vector<int>>;

  return 0;
}

// https://www.zhihu.com/question/270780457