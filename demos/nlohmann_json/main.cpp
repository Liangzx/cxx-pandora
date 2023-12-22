#include <iostream>
#include "nlohmann/json.hpp"


using json = nlohmann::json;
using namespace nlohmann::literals;

/// \class ClassName
/// \brief Brief description of the class.
///
/// Detailed description of the class.
/// You can provide more information here, such as the purpose, usage, and any important details.
class ClassName {
    /// \brief Adds two numbers.
    ///
    /// This function adds the given numbers and returns the result.
    ///
    /// \param a The first number.
    /// \param b The second number.
    /// \return The sum of the two numbers.
    ///
    /// \example
    /// Usage:
    /// \code
    /// Calculator calc;
    /// int result = calc.add(5, 3);
    /// // result will be 8
    /// \endcode
    ///
    /// \example
    /// Usage with variables:
    /// \code
    /// Calculator calc;
    /// int x = 10;
    /// int y = 20;
    /// int result = calc.add(x, y);
    /// // result will be 30
    /// \endcode
    int Add(int a, int b) {
        return a + b;
    }
};

int main() {
    // create object from string literal
    auto j = R"({ "happy": true, "pi": 3.141 })"_json;

    // explicit conversion to string
    std::string s = j.dump();    // {"happy":true,"pi":3.141}

    // serialization with pretty printing
    // pass in the amount of spaces to indent
    std::cout << j.dump(4) << std::endl;

    return 0;
}
