#include <iostream>
#include <set>
#include <vector>
#include <iterator>
#include <algorithm>
#include <functional>

#include <boost/range/algorithm/copy.hpp>
#include <boost/assign.hpp>


#include <boost/range/adaptors.hpp>

const std::vector<int> vec = { 0, 1, 2, 3, 4 };


/** Binary predicate that returns true as long as lhs != constructor_arg */
class lhs_neq {
public:
    explicit lhs_neq(int n) : _n(n) {}

    bool operator()(int lhs, int rhs) {
        bool result = (lhs != _n);
        std::cout << "  called lhs_neq(" << lhs << ", " << rhs << ") -> "
                  << (result ? "true" : "false") << std::endl;
        return result;
    }

private:
    int _n;
};


/** Print the results of the adjacent_filter calls in a readable way */
void print_result(int n, const std::set<int> &seen_values) {
    std::cout << "Pair where lhs = " << n << " removed. Result = {";
    for (int i : vec) {
        if (seen_values.count(i)) {
            std::cout << i << ", ";
        } else {
            std::cout << "   ";
        }
    }
    std::cout << "}" << std::endl;
}

void func() {
        for (int n : {0, 1, 2, 3, 4}) {
        std::set<int> seen_values;

        // adjacent_filter() calls the binary predicate for every consecutive
        // pair of values in the input range. If the predicate returns false,
        // the first value of the pair is removed from the result range.
        //
        // The last value in the input range is always included (there is no
        // pair where it's the first of the two values).
        for (int i : boost::adaptors::adjacent_filter(vec, lhs_neq(n))) {
            seen_values.insert(i);
        }
        print_result(n, seen_values);
    }
}

void func1() {
for (int n : {0, 1, 2, 3, 4}) {
        std::set<int> seen_values;

        // adjacent_filtered() calls the binary predicate for every consecutive
        // pair of values in the input range. If the predicate returns false,
        // the first value of the pair is removed from the result range.
        //
        // The last value in the input range is always included (there is no
        // pair where it's the first of the two values).
        for (int i : vec | boost::adaptors::adjacent_filtered(lhs_neq(n))) {
            seen_values.insert(i);
        }
        print_result(n, seen_values);
    }

}

void func2() {
    using namespace boost::assign;
    using namespace boost::adaptors;

    std::vector<int> input;
    input += 1,1,2,2,2,3,4,5,6;

    boost::copy(
        input | adjacent_filtered(std::not_equal_to<int>()),
        std::ostream_iterator<int>(std::cout, ","));

    auto bc = input | adjacent_filtered(std::not_equal_to<int>());

        // Print the transformed contacts.
    for (const auto& b : bc) {
        std::cout << b << std::endl;
    }

   return;
}

int main() {
    // func();
    // func1();
    func2();

    return 0;
}
