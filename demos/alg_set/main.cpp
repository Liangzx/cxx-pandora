#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

void SetIntersection() {
    std::vector<int> v1;
    std::vector<int> v2;

    for (int i = 0; i < 10; i++) {
        v1.push_back(i);    // 范围0-9
        v2.push_back(i + 5);// 范围5-15
    }

    std::vector<int> v3;

    std::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(v3));

    std::for_each(v3.begin(), v3.end(), [](int v) { std::cout << v << " "; });
    std::cout << std::endl;
}

void SetDifference() {
    std::vector<int> v1;
    std::vector<int> v2;

    for (int i = 0; i < 10; i++) {
        v1.push_back(i);    // 范围0-9
        v2.push_back(i + 5);// 范围5-15
    }

    std::vector<int> v3;

    std::set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(v3));

    std::for_each(v3.begin(), v3.end(), [](int v) { std::cout << v << " "; });
    std::cout << std::endl;
}

void SetUnion() {
    std::vector<int> v1;
    std::vector<int> v2;

    for (int i = 0; i < 10; i++) {
        v1.push_back(i);    // [0,10)
        v2.push_back(i + 5);// [5-15)
    }

    std::vector<int> v3;

    std::set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(v3));

    std::for_each(v3.begin(), v3.end(), [](int v) { std::cout << v << " "; });
    std::cout << std::endl;
}

int main() {
    //    SetDifference();
    //    SetUnion();
    const int n = 100;
    int *p = const_cast<int *>(&n);
    *p = 234;
    std::cout << "n = " << n << std::endl;
    std::cout << "*p = " << *p << std::endl;

    return 0;
}