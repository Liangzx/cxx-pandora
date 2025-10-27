#include <bits/stdc++.h>
#include <iostream>

using namespace std;

//从下向上的扫描线 那么就是横向的线段
struct Segment
{
  //每一条线段的左端点坐标 右端点坐标 纵坐标。
  //其中纵坐标的意义是在读到它对应的线段
  //也就是对于一个矩形的下底 读到它的上底时更改val
  long long l, r, h;
  // val的意义就是在下底时这一个线段权值加1 表示扫描线向下扫的时候
  // 保证这条线段的下方是覆盖的 那么在遇到上底时权值为-1
  //也就是说覆盖结束了 对于整个平面 扫描线所加的面积就是扫过的
  //并且有权值的位置
  int val;
  //重载运算符 使得纵坐标小的线段 也就是每个矩形的下底优先被扫描线读到
  bool operator<(const Segment &k) const { return h < k.h; }
} Seg[800010]; //空间要开足够大 每个矩形最少有两条线段 所以最少应该开两倍

struct SegTree //线段树部分
{
  //线段树的左右节点分别用来存储一条横向线段的编号
  //这里注意线段树每个节点存储的不是线段的左右端点
  //而是对于一对左右端点截出的线段 并且
  //这里的左右节点并不严格在输入时对应一条线段 但它们之间一定是有一条线段
  /*就比如说
    3号节点所存的线段可能是第二条线段的右端点和第三条线段的右端点所截成的线段
    因为第二条线段和第三条线段的纵坐标相同 所以它们有交集但不完全重合
    可能会出现这样的情况*/
  int l, r;
  //这里的sum表示线段树节点的权值 也就是说被覆盖的次数
  //但由于是求面积并 所以被覆盖一次还是两次并没有区别
  //只是有权值和没权值的区别 有权值的线段
  //在扫描线扫过的时候就会计算面积 否则就不会计算面积
  int sum;
  //这是每一个线段树节点所代表的线段的长度计算面积时使用
  long long len;
} Tree[1600010]; //空间要开足够大  一个线段树四倍 最好开到八倍会好一点

long long X[800010];
// 不用y1,跟标准库冲突
long long n, x1, x2, yy, y2, ans;
void Build(int k, long long l, long long r) {
  Tree[k].l = l;
  Tree[k].r = r;
  //扫描线没来之前没有权值 权值变更是在扫描线到达某一条线段时修改
  Tree[k].sum = 0;
  //同上
  std::cout << "------k-----" << k << std::endl;
  Tree[k].len = 0;
  if (l == r) {
    return;
  }
  int mid = l + r >> 1;
  Build(k << 1, l, mid);
  Build(k << 1 | 1, mid + 1, r);
}

// 更新线段树的 len 值
void pushup(int k) {
  int l = Tree[k].l;
  int r = Tree[k].r;
  //这就是上面所说的 只有被覆盖和没被覆盖的区别
  if (Tree[k].sum) {
    //如果线段被扫描线扫到了 则更新扫到的线段长度
    Tree[k].len = X[r + 1] - X[l];
    std::cout << "------Tree[k].sum--------k:" << k << " l: " << l << " r:" << r
              << " len:" << Tree[k].len << std::endl;
  } else {
    //如果没被扫到 则从以前扫过的线段更新
    Tree[k].len = Tree[k << 1].len + Tree[k << 1 | 1].len;
    std::cout << "------Tree[k].sum---NO-----k:" << k << " l: " << l
              << " r:" << r << " len:" << Tree[k].len << std::endl;
  }
}

//当扫描线扫到一条线段 执行此操作
// L,R分别记录线段的左右端点坐标（输入的矩形的沿着x的边） val代表这是下底还是上底
void update(int k, long long L, long long R, int val) {
  int l = Tree[k].l; // Tree 保存的是线段树编号 比如第一次进来 l = 1 r = 3
  int r = Tree[k].r;
  std::cout << "update:"
            << "k:" << k << " l:" << l << " r:" << r << std::endl;
  //要更新的线段不属于这个线段树节点 算是个剪枝吧
  if (X[r + 1] <= L || R <= X[l]) { // X[0] = 0, X[1] = 10, X[2] = 15, X[3] =
                                    // 20, X[4] = 30, L1=10, R1=20
    std::cout << "------no update-----"
              << ":"
              << "k:" << k << " l:" << l << " r:" << r << " L:" << L
              << " R:" << R << " X[r + 1]:" << X[r + 1] << "  X[l]:" << X[l]
              << std::endl;
    return;
  }

  //更新 如果是下底 那么sum就有了值 代表扫描线往上的地方需要计算面积
  //如果是上底 那么sum不一定没有值 只是减少
  //代表它对应的矩形面积计算完毕,L、R代表了矩形的左右端点坐标，当L，R包含了当前线段树的区间
  if (L <= X[l] && X[r + 1] <= R) {
    Tree[k].sum += val;
    pushup(k);
    return;
  }

  //由于有剪枝 我们就不用判断了 直接修改子树就好了
  update(k << 1, L, R, val);
  update(k << 1 | 1, L, R, val);
  pushup(k);
}
int main() {
  scanf("%d", &n);
  for (int i = 1; i <= n; i++) {
    scanf("%lld%lld%lld%lld", &x1, &yy, &x2, &y2);
    X[2 * i - 1] = x1;
    X[2 * i] = x2;
    //记录线段的信息
    //按照这个操作 开两倍是正常的
    //入边赋值1，出边赋值-1
    Seg[2 * i - 1] = (Segment){x1, x2, yy, 1};
    Seg[2 * i] = (Segment){x1, x2, y2, -1};
  }
  //线段数是矩形数二倍
  n <<= 1;
  //按照纵坐标升序排序
  sort(Seg + 1, Seg + n + 1);
  sort(X + 1, X + n + 1);
  //相同的横坐标我们只需要一次
  //因此通过离散化来确定不同横坐标的数量
  int cnt = unique(X + 1, X + n + 1) - X - 1;
  //这里cnt要-1 因为我们cnt代表横坐标的数量
  //而线段树存储的是线段数量 根据植树问题 线段数=点数-1
  Build(1, 1, cnt - 1);
  //最后一条边不需要计算和更新了吧
  std::cout << "--------------" << std::endl;
  for (int i = 1; i < n; i++) {
    std::cout << "------seg--------" << i << std::endl;
    update(1, Seg[i].l, Seg[i].r, Seg[i].val);
    std::cout << "len:" << Tree[1].len << " "
              << "hi:" << Seg[i + 1].h - Seg[i].h << std::endl;
    //计算面积
    ans += Tree[1].len * (Seg[i + 1].h - Seg[i].h);
  }
  printf("%lld\n", ans);
  return 0;
}

// https://www.cnblogs.com/Rakan-LoveJ/p/11401851.html
// https://blog.csdn.net/Zz_0913/article/details/135128515
// https://zhuanlan.zhihu.com/p/103616664
// https://oi-wiki.org/geometry/scanning/

// 2, [(10, 10),(20, 20)],[(15, 15),(30, 30)]
/*
------seg--------1
update:k:1 l:1 r:3
update:k:2 l:1 r:2
------Tree[k].sum--------k:2 l: 1 r:2 len:10
update:k:3 l:3 r:3
------no update-----:k:3 l:3 r:3 L:10 R:20 X[r + 1]:30  X[l]:20 // // X[r + 1] <= L || R <= X[l]
------Tree[k].sum---NO-----k:1 l: 1 r:3 len:10
len:10 hi:5
------seg--------2
update:k:1 l:1 r:3
update:k:2 l:1 r:2
update:k:4 l:1 r:1
------no update-----:k:4 l:1 r:1 L:15 R:30 X[r + 1]:15  X[l]:10
update:k:5 l:2 r:2
------Tree[k].sum--------k:5 l: 2 r:2 len:5
------Tree[k].sum--------k:2 l: 1 r:2 len:10
update:k:3 l:3 r:3
------Tree[k].sum--------k:3 l: 3 r:3 len:10
------Tree[k].sum---NO-----k:1 l: 1 r:3 len:20
len:20 hi:5
------seg--------3
update:k:1 l:1 r:3
update:k:2 l:1 r:2
------Tree[k].sum---NO-----k:2 l: 1 r:2 len:5
update:k:3 l:3 r:3
------no update-----:k:3 l:3 r:3 L:10 R:20 X[r + 1]:30  X[l]:20
------Tree[k].sum---NO-----k:1 l: 1 r:3 len:15
len:15 hi:10
300
*/
