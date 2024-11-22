#include <iostream>

class Top {  // 虚基类
 public:
  int i;
  Top(int ii) { i = ii; }
};

class Left : public virtual Top {
 public:
  int j;
  Left(int jj, int ii) : Top(ii) { j = jj; }
};

class Right : public virtual Top {
 public:
  int k;
  Right(int kk, int ii) : Top(ii) { k = kk; }
};

class Bottom : public Left, public Right {
 public:
  int l;
  Bottom(int ll, int jj, int kk, int ii)
      : Top(ii), Left(jj, ii), Right(kk, ii) {
    l = ll;
  }
};

int main() {
  // 64bit
  std::cout << "Top:" << sizeof(Top) << std::endl; // 4
  std::cout << "Left:" << sizeof(Left) << std::endl; // 16 = 4 + 4 + vbptr(pointer of vbtable)
  std::cout << "Right:" << sizeof(Right) << std::endl; // 16 = 4 + 4 + vbptr(pointer of vbtable)
  std::cout << "Bottom:" << sizeof(Bottom) << std::endl; // 40 = 4 + 4 + 4
  return 0;
  Bottom b(1, 2, 3, 4);
  Bottom* bp = &b;
  // 访问自身成员变量
  b.l = 1;
  bp->l = 2;
  // 访问父类Left的成员变量
  Left* lp = bp;
  b.j = 1;
  bp->j = 2;
  lp->j = 3;
  // 访问父类Right的成员变量
  Right* rp = bp;
  b.k = 1;
  bp->k = 2;
  rp->k = 3;
  // 访问虚基类Top的成员变量
  Top* tp = bp;
  b.i = 1;
  bp->i = 2;
  tp->i = 3;
};

// https://www.cnblogs.com/chaoguo1234/p/3149212.html
// g++ -fdump-lang-class virt_inherit.cpp
