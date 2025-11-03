#include <iostream>
#include <optional>
#include <string>

void *operator new(std::size_t count) {
  std::cout << "allocating " << count << " bytes" << std::endl;
  return malloc(count);
}

void operator delete(void *ptr) noexcept {
  std::cout << "global op delete called" << std::endl;
  std::free(ptr);
}

class UserName {
public:
  explicit UserName() : mName("Default") {
    std::cout << "UserName::UserName(\'";
    std::cout << mName << "\')\n";
  }

  explicit UserName(const std::string &str) : mName(str) {
    std::cout << "UserName::UserName(\'";
    std::cout << mName << "\')\n";
  }

  ~UserName() {
    std::cout << "UserName::~UserName(\'";
    std::cout << mName << "\')\n";
  }

  UserName(const UserName &u) : mName(u.mName) {
    std::cout << "UserName::UserName(copy \'";
    std::cout << mName << "\')\n";
  }

  UserName(UserName &&u) : mName(std::move(u.mName)) {
    std::cout << "UserName::UserName(move \'";
    std::cout << mName << "\')\n";
  }

  UserName &operator=(const UserName &u) // copy assignment
  {
    mName = u.mName;

    std::cout << "UserName::=(copy \'";
    std::cout << mName << "\')\n";

    return *this;
  }

  UserName &operator=(UserName &&u) // move assignment
  {
    mName = std::move(u.mName);

    std::cout << "UserName::=(move \'";
    std::cout << mName << "\')\n";

    return *this;
  }

private:
  std::string mName;
};

int main() {
  // std::optional<UserName> opt(UserName{});
  // std::optional<UserName> opt({});
  // 没有调用 operator new​​：在使用 std::in_place构造 std::optional时（第1部分），
  // 输出显示直接调用了 MyClass的构造函数，但​​没有调用​​我们重载的 MyClass::operator new。
  // 这强烈表明 std::optional在内部已经预先管理好了内存（就像我们预先分配的 buffer），
  // 然后直接在该内存上构造对象，这正是 placement new 的核心思想
  std::optional<UserName> opt{std::in_place};
}

// https://mp.weixin.qq.com/s?__biz=MzU0OTkzNjU2OA==&mid=2247486464&idx=1&sn=3801f746d605215171e58574abbd6384&poc_token=HHV5BGmjQ_phWIa-8WjVCNdKHzetV5PZM_w-y8LL
