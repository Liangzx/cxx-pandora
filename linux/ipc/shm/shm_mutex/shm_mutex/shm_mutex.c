#include <fcntl.h>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_KEY "/my_shared_mutex"
#define SHM_SIZE sizeof(pthread_mutex_t)

int main() {
    int shmid;
    void *shmaddr;
    pthread_mutex_t *mutex;

    // 创建或打开共享内存段
    int shmfd = shm_open(SHM_KEY, O_CREAT | O_RDWR, 0666);
    if (shmfd == -1) {
        perror("shm_open");
        return 1;
    }

    // 设置合适的权限和大小
    ftruncate(shmfd, SHM_SIZE);

    // 将共享内存段映射到当前进程的地址空间
    shmaddr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
    if (shmaddr == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    mutex = static_cast<pthread_mutex_t *>(shmaddr);

    // 初始化互斥锁
    if (pthread_mutex_init(mutex, NULL) != 0) {
        perror("pthread_mutex_init");
        return 1;
    }

    // 使用互斥锁
    pthread_mutex_lock(mutex);
    // 执行临界区代码...
    pthread_mutex_unlock(mutex);

    // 销毁互斥锁
    pthread_mutex_destroy(mutex);

    // 卸载共享内存
    if (munmap(shmaddr, SHM_SIZE) != 0) {
        perror("munmap");
        return 1;
    }

    // 关闭共享内存描述符
    close(shmfd);

    // 删除共享内存对象
    shm_unlink(SHM_KEY);

    return 0;
}
