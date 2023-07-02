#include <absl/cleanup/cleanup.h>
#include <absl/strings/cord.h>
#include <absl/strings/str_join.h>

#include <iostream>

void Cleanup() {
  auto cleanup = absl::MakeCleanup([]() { std::cout << "hello" << std::endl; });

  std::cout << "world" << std::endl;
}

void StrJoin() {
  //  std::vector<std::string> v = {"foo", "bar", "baz"};
  //  std::string s = absl::StrJoin(v, "-");
  std::string s = absl::StrJoin(std::make_tuple(-1, 2), "");

  std::cout << s << std::endl;
}

void StrCat() {
  const short s = -1;  // NOLINT(runtime/int)
  const uint16_t us = 2;
  std::cout << absl::StrCat(s, us) << std::endl;
}

void StrSplit() {}

//void StrCord() {
//  absl::Cord cord;
//  cord.Append("hello");
//  std::cout << cord.size() << std::endl;
//}

void Digits() {
  // https://stackoverflow.com/questions/747470/what-is-the-meaning-of-numeric-limitsdoubledigits10
  std::cout << std::numeric_limits<float>::digits10 << std::endl;  // 6
  auto f = (float)99999999;  // 8 digits -- 100000000
  std::cout.precision(17);
  std::cout << "The float is; " << f << std::endl;
  std::cout << std::numeric_limits<double>::digits10 << std::endl;  // 15
  auto d = (double)99999999;  // 8 digits -- 99999999
  std::cout << "The double is; " << d << std::endl;
}

// 状态错误: 临时快照已存在: vol-05FF322C tmpsnap-C7534266, 错误编码: 106854


int main() {
  //  StrJoin();
  //  StrCord();
  //  Digits();
  //  StrCat();
  //  Cleanup();
  std::string ss = "状态错误: 临时快照已存在: vol-05FF322C tmpsnap-C7534266, 错误编码: 106854";
  std::cout << ss.find("tmpsnap-") <<std::endl;
  auto tmp_id = std::move(ss.substr(ss.find("tmpsnap-")));
  tmp_id = std::move(tmp_id.substr(0, tmp_id.find_first_of(',')));
  std::cout << tmp_id << std::endl;

  return 0;
}
