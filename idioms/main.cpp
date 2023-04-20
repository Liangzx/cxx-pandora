#include <algorithm>
#include <iostream>
#include <vector>

void PrintVec(const std::vector<std::string>& vec) {
    std::cout << "------------------" << std::endl;
    for (const auto& v : vec) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    std::vector<std::string> o{"/dev/vdc", "/dev/vda", "/dev/vde", "/dev/vdb"};
    std::sort(o.begin(), o.end());
    PrintVec(o);
    std::vector<std::string> n{"/dev/vdc", "/dev/vda", "/dev/vde", "/dev/vdd", "/dev/vdb"};
    std::sort(n.begin(), n.end());
    PrintVec(n);
    std::vector<std::string> c;
    std::set_difference(n.begin(), n.end(), o.begin(), o.end(), std::back_inserter(c));
    PrintVec(c);
    std::string vd = "/dev/vdc";
    vd[vd.size() - 1] = (char) (vd[vd.size() - 1] + 1);
    std::cout << vd << std::endl;
    return 0;
}
