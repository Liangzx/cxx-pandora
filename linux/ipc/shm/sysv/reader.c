#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

int main() {
    key_t key = ftok("/dev/shm/myshm1", 0);
    int shm_id = shmget(key, 0x400000, 0666);
    char *p = (char *) shmat(shm_id, NULL, 0);

    printf("%c %c %c %c .\n", p[0], p[1], p[2], p[3]);
    shmdt(p);

    // 使用shmctl删除共享内存段
    if (shmctl(shm_id, IPC_RMID, 0) == -1) {
        perror("Error in deleting the shared memory segment");
        return 1;// 返回错误
    }

    printf("Shared memory segment successfully removed.\n");
    return 0;// 成功
}
