#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <streambuf>
#include <string>
#include <thread>
using namespace std;
stringbuf buf;
istream in(&buf);
ostream out(&buf);
bool flag = false;

void threadb() {
  char data;
  while (true) {
    if (flag) {
      in >> data;
      cout << "thread B recv:" << data << endl;
      flag = false;
    }
  }
}

int main() {
  thread consumer(threadb);
  char data;
  while (true) {
    cin >> data;
    out << data;
    flag = true;
  }
  return 0;
}
