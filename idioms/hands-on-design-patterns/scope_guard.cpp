// https://github.com/PacktPublishing/Hands-On-Design-Patterns-with-CPP/blob/master/Chapter11

#include <iostream>

class ScopeGuardBase {
public:
  ScopeGuardBase() : commit_(false) {}
  void commit() noexcept { commit_ = true; }
  ScopeGuardBase &operator=(const ScopeGuardBase &) = delete;
  ScopeGuardBase &operator=(const ScopeGuardBase &&) = delete;
  ScopeGuardBase(const ScopeGuardBase &other) = delete;

protected:
  ScopeGuardBase(ScopeGuardBase &&other) : commit_(other.commit_) {
    other.commit();
  }
  ~ScopeGuardBase() = default;
  bool commit_;
};

template <typename Func> class ScopeGuard : public ScopeGuardBase {
public:
  ScopeGuard(Func &&func) : func_(std::move(func)) {}
  ScopeGuard(const Func &func) : func_(func) {}
  ~ScopeGuard() {
    if (!commit_)
      func_();
  }
  ScopeGuard(ScopeGuard &&other)
      : ScopeGuardBase(std::move(other)), func_(other.func_) {}

private:
  Func func_;
};

template <typename Func> ScopeGuard<Func> MakeGuard(Func &&func) {
  return ScopeGuard<Func>(std::forward<Func>(func));
}

#define CONCAT2(x, y) x##y
#define CONCAT(x, y) CONCAT2(x, y)
#ifdef __COUNTER__
#define ANON_VAR(x) CONCAT(x, __COUNTER__)
#else
#define ANON_VAR(x) CONCAT(x, __LINE__)
#endif

struct ScopeGuardOnExit {};
template <typename Func>
ScopeGuard<Func> operator+(ScopeGuardOnExit, Func &&func) {
  return ScopeGuard<Func>(std::forward<Func>(func));
}
#define ON_SCOPE_EXIT                                                          \
  auto ANON_VAR(SCOPE_EXIT_STATE) = ScopeGuardOnExit{} + [&]()

#define ON_SCOPE_EXIT_ROLLBACK(NAME) auto NAME = ScopeGuardOnExit() + [&]()

void ff() { std::cout << "hello" << std::endl; }

#include <algorithm>
#include <type_traits>
#include <iterator>
int main() {
  ON_SCOPE_EXIT { ff(); };
  std::cout << "world" << std::endl;
}
