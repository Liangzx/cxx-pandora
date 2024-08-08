
#include "di.hpp"
#include <iostream>

namespace di = boost::di;

class Service {
public:
    void execute() {
        std::cout << "Service executing..." << std::endl;
    }
};

class Client {
private:
    std::shared_ptr<Service> service;

public:
    Client(std::shared_ptr<Service> srv) : service(srv) {}

    void doWork() {
        service->execute();
    }
};

int main() {
    auto injector = di::make_injector(
        di::bind<Service>().in(di::singleton)
    );

    auto client = injector.create<std::shared_ptr<Client>>();
    client->doWork();

    return 0;
}

// [C++中的依赖注入：提升代码可维护性的新策略](https://mp.weixin.qq.com/s/OY1SFcAbw3-iuHh9wZXFAA)
// https://boost-ext.github.io/di/index.html
