#include <iostream>
#include <random>
#include <string>
#include <vector>

class GearHash {
private:
  std::vector<uint64_t> gear_table; // Gear查找表

public:
  // 初始化Gear表（实际应用中应使用固定预计算值）
  GearHash() {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dis;

    gear_table.resize(256);
    for (int i = 0; i < 256; ++i) {
      gear_table[i] = dis(gen);
    }
  }

  // 计算Gear哈希
  uint64_t computeHash(const std::string &str) {
    uint64_t hash = 0;
    for (char c : str) {
      hash = (hash << 1) + gear_table[static_cast<uint8_t>(c)];
    }
    return hash;
  }

  // 滚动计算Gear哈希
  uint64_t rollHash(uint64_t prev_hash, char left_char, char right_char,
                    size_t window_size) {
    uint64_t left_contribution = gear_table[static_cast<uint8_t>(left_char)]
                                 << (window_size - 1);
    return (prev_hash - left_contribution)
           << 1 + gear_table[static_cast<uint8_t>(right_char)];
  }
};

// 使用示例
void gearExample() {
  std::string text = "abcdefgh";
  size_t window_size = 3;

  GearHash gh;
  uint64_t hash = gh.computeHash(text.substr(0, window_size));
  std::cout << "Initial Gear hash (abc): " << hash << std::endl;

  for (size_t i = 1; i <= text.size() - window_size; ++i) {
    hash =
        gh.rollHash(hash, text[i - 1], text[i + window_size - 1], window_size);
    std::cout << "Rolling Gear hash (" << text.substr(i, window_size)
              << "): " << hash << std::endl;
  }
}

int main() {
    std::cout << "=== Rabin Hash Example ===" << std::endl;
    gearExample();

    return 0;
}
