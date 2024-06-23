#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>
using namespace std;

template <typename T, template <typename...> class Container>
T &Sort(Container<T> &v) {
  sort(v.begin(), v.end());
  return v;
}

template <typename T, template <typename...> class Container>
T &Reverse(Container<T> &v) {
  reverse(v.begin(), v.end());
  return v;
}

template <typename T, template <typename...> class Container>
Container<T> &Print(Container<T> &v) {
  for (auto &i : v) {
    cout << i << " ";
  }
  cout << endl;
  return v;
}

template <typename T, template <typename...> class Container, typename U>
Container<T> &operator|(Container<T> &v, U fn) {
  fn(v);
  return v;
}

int main() {
  vector<int> vec{3, 2, 1, 5, 4};

  vec | Print<int, vector>;

  return 0;
}

// https://www.zhihu.com/question/270780457
