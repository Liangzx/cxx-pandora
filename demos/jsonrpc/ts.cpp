#include <map>
#include <iostream>

int main() {
  std::map<int, int> imap{};
  for(auto & v : imap) {
    std::cout << v.first << std::endl;
  }
  
  return 0;
}