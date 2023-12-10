#include <iostream>
#include <queue>
#include <vector>

struct ListNode {
  int val_;
  ListNode *next_;
  ListNode() : val_(0), next_(nullptr) {}
  ListNode(int v) : val_(v), next_(nullptr) {}
  ListNode(int v, ListNode *next) : val_(v), next_(next) {}
};

ListNode *mergeKLists(std::vector<ListNode *> &lists) {
  if (lists.empty()) {
    return nullptr;
  }

  auto cmp = [](ListNode *l, ListNode *r) { return l->val_ > r->val_; };
  std::priority_queue<ListNode *, std::vector<ListNode *>, decltype(cmp)> pq(
      cmp);

  for (auto head : lists) {
    if (nullptr != head) {
      pq.push(head);
    }
  }

  ListNode *dummy = new ListNode(-1);
  ListNode *p = dummy;

  while (!pq.empty()) {
    auto top = pq.top();
    pq.pop();
    p->next_ = top;
    if (nullptr != top->next_) {
      pq.push(top->next_);
    }
    p = p->next_;
  }

  auto r = dummy->next_;
  delete dummy;
  return r;
}

int main() {
  // mergeKLists(nullptr);
  std::vector<int> v(5);
  std::cout << v.size() << std::endl;
  return 0;
}