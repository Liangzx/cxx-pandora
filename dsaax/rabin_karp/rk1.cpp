#include <cmath>
#include <iostream>
#include <string>
#include <vector>

class RabinHash {
private:
  const uint64_t base = 256; // 基数，通常选择256（字节最大值+1）
  const uint64_t prime = 101; // 大素数，实际应用中应选择更大的素数
  uint64_t max_power;         // 最高位幂值 base^(window_size-1) mod prime

public:
  // 预计算最高位幂值
  RabinHash(size_t window_size) {
    max_power = 1;
    for (size_t i = 1; i < window_size; ++i) {
      max_power = (max_power * base) % prime;
    }
  }

  // 计算初始窗口哈希
  uint64_t computeInitialHash(const std::string &str, size_t len) {
    uint64_t hash = 0;
    for (size_t i = 0; i < len; ++i) {
      hash = (hash * base + str[i]) % prime;
    }
    return hash;
  }

  // 滚动计算下一个哈希
  uint64_t rollHash(uint64_t prev_hash, char left_char, char right_char) {
    prev_hash = (prev_hash + prime - (left_char * max_power) % prime) % prime;
    return (prev_hash * base + right_char) % prime;
  }
};

// 使用示例
void rabinExample() {
  std::string text = "abcdefgh";
  size_t window_size = 3;

  RabinHash rh(window_size);
  uint64_t hash = rh.computeInitialHash(text, window_size);
  std::cout << "Initial hash (abc): " << hash << std::endl;

  for (size_t i = 1; i <= text.size() - window_size; ++i) {
    hash = rh.rollHash(hash, text[i - 1], text[i + window_size - 1]);
    std::cout << "Rolling hash (" << text.substr(i, window_size)
              << "): " << hash << std::endl;
  }
}

int main() {
    std::cout << "=== Rabin Hash Example ===" << std::endl;
    rabinExample();

    return 0;
}
