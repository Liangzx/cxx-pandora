#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void ReadWhile(const std::string &pathname) {
    std::fstream ifs(pathname, std::ios::in | std::ios::binary);

    const auto buf_size = 1024;
    auto buf = new char[buf_size];

    while (!ifs.eof()) {
        auto num_read = ifs.read(buf, buf_size).gcount();
        if (0 == num_read) {
            break;
        }
        ifs.seekg(num_read, std::ios::cur);
    }
}

void ReadAll1() {
    std::ifstream ifs("", std::ios::binary);
    std::vector<char> buf(ifs.seekg(0, std::ios::end).tellg());
    ifs.seekg(0, std::ios::beg).read(&buf[0], static_cast<std::streamsize>(buf.size()));
}


void ReadAll2() {
    std::string buf;
    buf.reserve(100);
    auto pathname = "/root/i-056DB9B3.meta";
    std::fstream ifs;
    ifs.open(pathname, std::fstream::in);
    if (!ifs.good()) {
        std::cout << "error" << std::endl;
        return;
    }
    int i = 0;

    while (ifs >> buf) {
        i++;
        std::cout << buf << std::endl;
    }

    std::cout << buf << std::endl;
    std::cout << i << std::endl;
}

void ReadAll3() {
    std::ifstream ifs("/root/i-056DB9B3.meta");
    std::stringstream buf;
    buf << ifs.rdbuf();
    std::cout << buf.str() << std::endl;
    ifs.close();
}

void ReadLine() {
    char buffer[256];
    std::ifstream ifs("/root/i-056DB9B3.meta", std::fstream::in);
    if (!ifs.is_open()) {
        std::cout << "Error opening file";
        exit(1);
    }
    while (!ifs.eof()) {
        ifs.getline(buffer, 100);
        std::cout << buffer << std::endl;
    }
    ifs.close();
}

int main() {
    ReadAll2();
    //    ReadAll3();
    //    ReadLine();
    return 0;
}

// https://blog.csdn.net/a379039233/article/details/51491655
// https://www.cnblogs.com/redips-l/p/8258306.html