#include <iostream>
#include <system_error>
#include <boost/safe_numerics/safe_integer.hpp>
#include <boost/safe_numerics/checked_result.hpp>
using namespace boost::safe_numerics;

int main() {
    // 定义安全整数（自动检测溢出）
    safe<int> a = 100000;
    safe<int> b = 200000;
    safe<uint64_t> c = 300000;

    try {
        // 第一步：加法运算
        safe<int> sum1 = a + b;  // 300000
        std::cout << "Step 1: a + b = " << sum1 << std::endl;

        // 第二步：乘法运算（可能溢出）
        safe<int> product = sum1 * c;  // 300000 * 300000 = 90000000000（超出int范围）
        std::cout << "Step 2: sum1 * c = " << product << std::endl;
    }
    catch (const std::exception& e) {
        // 捕获溢出异常
        std::cerr << "Overflow detected: " << e.what() << std::endl;
    }

    std::cout << std::numeric_limits<int>::max() << std::endl; // 输出int的最大值

    return 0;
}
