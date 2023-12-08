#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

int main() {
    const char* file_path = "hello.txt";
    const char* new_content = "This is the new content.";

    // 打开文件
    int fd = open(file_path, O_RDWR);
    if (fd == -1) {
        std::cerr << "Failed to open file." << std::endl;
        return 1;
    }

    // 获取文件大小
    struct stat file_info;
    if (fstat(fd, &file_info) == -1) {
        std::cerr << "Failed to get file size." << std::endl;
        close(fd);
        return 1;
    }
    off_t file_size = file_info.st_size;

    // 映射文件到内存
    void* file_memory = mmap(nullptr, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (file_memory == MAP_FAILED) {
        std::cerr << "Failed to mmap file." << std::endl;
        close(fd);
        return 1;
    }

    // 写入新内容到文件内存
    std::memcpy(file_memory, new_content, strlen(new_content));

    // 解除映射
    if (munmap(file_memory, file_size) == -1) {
        std::cerr << "Failed to unmap file." << std::endl;
    }

    // 关闭文件
    close(fd);

    return 0;
}
