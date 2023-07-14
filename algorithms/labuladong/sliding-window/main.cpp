#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>
#include <filesystem>

// https://leetcode.cn/problems/minimum-window-substring/
std::string minimumWindowSubstring(std::string s, std::string t) {
  std::unordered_map<char, int> need, window;

  for (auto c : t) {
    need[c]++;
  }
  int left = 0;
  int right = 0;
  int valid = 0;

  // 记录最小覆盖子串的起始索引及长度
  int start = 0;
  int len = std::numeric_limits<int>::max();

  while (right < s.size()) {
    // c 是将移入窗口的字符
    auto c = s[right];

    // 进行窗口内数据的一系列更新
    if (need.count(c)) {
      window[c]++;
      if (need[c] == window[c]) {
        valid++;
      }
    }
    // 右移窗口
    right++;

    // 判断左侧窗口是否要收缩 - 窗口字符满足查找字符
    while (valid == need.size()) {
      // 记录最小值
      if (right - left < len) {
        start = left;
        len = right - left;
      }

      // d 是将移出窗口的字符
      auto d = s[left];

      // 进行窗口内数据的一系列更新
      if (need.count(d)) {
        if (window[d] == need[d]) {
          valid--;
        }
        window[d]--;
      }
      // 左移窗口
      left++;
    }
  }

  // 返回最小覆盖子串
  return len == std::numeric_limits<int>::max() ? "" : s.substr(start, len);
}

// https://leetcode.cn/problems/longest-substring-without-repeating-characters/
int lengthOfLongestSubstring(const std::string &s) {
  int left = 0;
  int right = 0;
  int res = 0;
  int len = 0;
  int index = 0;
  std::unordered_map<char, int> m;
  while (right < s.size()) {
    auto c = s[right];
    if (m.find(c) != m.end() && m[c] >= left) {
      left = m[c] + 1;
      if (len <= right - left) {
        index = left;
      }
      len = right - left;
    }
    m[c] = right;
    right++;
    len++;

    res = std::max(res, len);
  }
  std::cout << "len=" << res << " index=" << index
            << " sub=" << s.substr(index, res);

  return res;
}

class Product
{
public:
  virtual ~Product() {}
  virtual std::string Operation() const = 0;
  virtual auto f3() const && -> float = delete;
};

int main() {
  lengthOfLongestSubstring("abcabcbb");

  return 0;
}