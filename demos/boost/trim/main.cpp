#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/trim_all.hpp>
 
#include <string>
#include <iostream>
#include <memory>

class Signer;

class SangforSCP_Client {
    // Signer
public:
    std::unique_ptr<Signer> signer_;
    std::string name_;
    std::string pwd_;
    int a_;
};

class Signer {
public:
    Signer(SangforSCP_Client & client):client_(client){}
    SangforSCP_Client &client_;
};
 
int main(int argc, char* [])
{
    SangforSCP_Client p = {
        .signer_ = nullptr,
        .name_ = "xx",
    };
    // std::string h = "hello\n";
    // SangforSCP_Client cli;
    // Signer sig(cli);
    // boost::trim_right(h);
    // std::cout << h << ":" << h.size() << std::endl;
    return 0;
}
