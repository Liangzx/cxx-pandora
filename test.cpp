#include <iostream>
#include <string>
#include <exception>
#include <signal.h>



int main() {

  std::string hh = "123hda";
  hh.replace(hh.find("hda"), 3, "vda");
  std::cout << hh << std::endl;
}
