// https://mp.weixin.qq.com/s/JMiH5F8XuQf8VNn7PjMd3w

#include <cpprest/http_client.h>
#include <iostream>

using namespace web;
using namespace web::http;
using namespace web::http::client;

int main() {
    http_client client(U("http://example.com"));

    uri_builder builder(U("/api/data"));
    client.request(methods::GET, builder.to_uri()).then([](http_response response) {
        if (response.status_code() == status_codes::OK) {
            auto body = response.extract_string().get();
            std::cout << "Response: " << body << std::endl;
        } else {
            std::cout << "Request failed with status code: " << response.status_code() << std::endl;
        }
    }).wait();

    return 0;
}
