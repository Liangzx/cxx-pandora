#include <iostream>
#include <string.h>

using namespace std;

#define BASE 256
#define MODULUS 101

void RabinKarp(char t[], char p[]) {
  int t_len = strlen(t);
  int p_len = strlen(p);

  // 哈希滚动之用
  int h = 1;
  for (int i = 0; i < p_len - 1; i++)
    h = (h * BASE) % MODULUS;

  int t_hash = 0;
  int p_hash = 0;
  for (int i = 0; i < p_len; i++) {
    t_hash = (BASE * t_hash + t[i]) % MODULUS;
    p_hash = (BASE * p_hash + p[i]) % MODULUS;
  }

  int i = 0;
  while (i <= t_len - p_len) {
    // 考虑到哈希碰撞的可能性，还需要用 memcmp 再比对一下
    if (t_hash == p_hash && memcmp(p, t + i, p_len) == 0)
      cout << p << " is found at index " << i << endl;

    // 哈希滚动
    t_hash = (BASE * (t_hash - t[i] * h) + t[i + p_len]) % MODULUS;

    // 防止出现负数
    if (t_hash < 0)
      t_hash = t_hash + MODULUS;

    i++;
  }
}

int main() {
  char t[100] = "It is a test, but not just a test";
  char p[10] = "test";

  RabinKarp(t, p);

  return 0;
}

// https://zhuanlan.zhihu.com/p/563551141
