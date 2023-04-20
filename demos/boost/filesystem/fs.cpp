#include <boost/filesystem.hpp>
#include <iostream>
#include <iterator/transform_iterator.hpp>
#include <numeric>

void Traverse(const std::string &path) {
    for (boost::filesystem::directory_entry &file : boost::filesystem::directory_iterator{path}) {
        if (boost::filesystem::is_regular_file(file.status())) {
            std::string file_name = absolute(file.path()).string();
            std::cout << file_name << std::endl;
        } else if (boost::filesystem::is_directory(file.status())) {
            std::string file_name = absolute(file.path()).string();
            std::cout << file_name << std::endl;
        }
    }
}

void RecursiveTraverse(const std::string &path) {
    for (boost::filesystem::directory_entry &file : boost::filesystem::recursive_directory_iterator{path}) {
        std::string file_name = absolute(file.path()).string();
        std::cout << file_name << std::endl;
    }
}

void Remove(const std::string &path) {
    boost::system::error_code ec;
    boost::filesystem::remove_all(path, ec);
    if (ec) {
        std::cout << "rm failed:" << ec.message() << std::endl;
    }
}

void Space(const std::string &path) {
    auto space_info = boost::filesystem::space(path);
    std::cout << "capacity: " << space_info.capacity << std::endl;
    std::cout << "available: " << space_info.available << std::endl;
    std::cout << "free: " << space_info.free << std::endl;
}

//namespace fs = boost::filesystem;
//boost::uintmax_t dir_size( const boost::filesystem::path &p)
//{
//    return std::accumulate(
//            boost::make_transform_iterator(&boost::filesystem::file_size, boost::filesystem::recursive_directory_iterator(p)),
//            boost::make_transform_iterator(&boost::filesystem::file_size, boost::filesystem::recursive_directory_iterator()),
//            static_cast <boost::uintmax_t>(0));
//}
#include <vector>
int main(int argc, char **argv) {
    auto path = argv[1];
    //    Traverse(path);
    //    RecursiveTraverse(path);
//    Space(path);
    Remove(path);

    std::vector<int> v1{1, 2, 3};
    std::vector<int> v2{1, 2, 3};
    v1.insert(v1.end(), v2.begin(), v2.end());

    return 0;
}
// https://www.cnblogs.com/sssblog/p/9707734.html