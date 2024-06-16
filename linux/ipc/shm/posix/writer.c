#include <fcntl.h>
#include <fcntl.h> /* For O_* constants */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/stat.h> /* For mode constants */
#include <sys/types.h>
#include <unistd.h>

int main() {
    const char *shm_name = "posix_shm";
    size_t size = 1024 * 1024;// 1MB
    int shm_fd;
    void *shm_ptr;

    // 创建或打开共享内存对象
    shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    // 设置共享内存大小
    if (ftruncate(shm_fd, size) == -1) {
        perror("ftruncate");
        goto error;
    }

    // 将共享内存映射到当前进程地址空间
    shm_ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("mmap");
        goto error;
    }

    // 写入数据到共享内存
    strcpy((char *) shm_ptr, "Hello from shared memory!");

    // 模拟另一个进程的行为，读取共享内存中的数据
    printf("Read from shared memory: %s\n", (char *) shm_ptr);

    // 清理：取消映射
    if (munmap(shm_ptr, size) == -1) {
        perror("munmap");
        goto error;
    }

    // // 关闭文件描述符
    // if (close(shm_fd) == -1) {
    //     perror("close");
    //     goto error;
    // }

    // // 可选：如果不再需要共享内存，可以在这里删除它
    // shm_unlink(shm_name);// 注意这将立即释放共享内存资源

    return 0;

error:
    if (shm_ptr != MAP_FAILED) munmap(shm_ptr, size);
    if (shm_fd != -1) close(shm_fd);
    shm_unlink(shm_name);// 出错时立即删除，防止残留
    return EXIT_FAILURE;
}

// gcc -o sw writer.c -lrt
// ll -l /dev/shm/
