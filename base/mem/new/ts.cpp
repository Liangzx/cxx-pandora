

#include <complex>
#include <map>
#include <tuple>

void func(int* a) {}

int main() {
    //   std::map<std::string, std::complex<double>> scp;
    //   scp.emplace("hello", 1, 2);
    // auto std::tuple<std::string> t1 = std::make_tuple(std::string("hello"));
    auto t2 = std::make_tuple(1.0, 2.0);
    std::pair<std::string, std::complex<double>> scp("", t2);
    return 0;
}
