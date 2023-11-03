#include <functional>
#include <iostream>

struct StuffClientOptions {
  int Retries; // number of times to retry the request before giving up
  int Timeout; // connection timeout in seconds
};

using StuffClientOption = std::function<void(StuffClientOptions *)>;

StuffClientOption WithRetries(int r) {
  auto f = [=](StuffClientOptions *o) { o->Retries = r; };
  return f;
}

int main() {
  std::cout << "hello" << std::endl;
  StuffClientOptions op;
  auto wr = WithRetries(12);
  wr(&op);
  std::cout << op.Retries << std::endl;

  return 0;
}