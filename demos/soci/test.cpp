
#include <iostream>

template<typename T>
class Collection {
public:
    T Filter() {
        std::cout << "Collection:Filter" << std::endl;
        return *static_cast<T *>(this);
    }
};

int main() {

    return 0;
}