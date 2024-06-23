#include "ace/OS.h"
#include "ace/Shared_Memory_Pool.h"
#include "ace/Shared_Memory_SV.h"
#include "ace/Thread_Mutex.h"
#include "ace/RW_Thread_Mutex.h"
#include "ace/Log_Msg.h"
#include "ace/Mem_Map.h"
#include "ace/Shared_Memory_MM.h"

const char *shm_name = "my_shared_memory";
const size_t shm_size = sizeof(ACE_RW_Thread_Mutex);// 使用RW锁，也可以是Mutex

int create_and_init_mutex_in_shm() {
    ACE_Mem_Map mmap;
    ACE_Shared_Memory_SV shm(shm_name, shm_size, ACE_DEFAULT_PROTECTION);
    if (!shm.valid()) {
        ACE_ERROR_RETURN((LM_ERROR, "Unable to create shared memory segment.\n"), -1);
    }

    ACE_RW_Thread_Mutex *rwlock = reinterpret_cast<ACE_RW_Thread_Mutex *>(shm.addr());
    rwlock->init();// 初始化锁

    return 0;
}

void use_mutex_from_shm() {
    ACE_Shared_Memory_Pool shm_pool;
    void *shm_addr = shm_pool.attach(shm_name);
    if (shm_addr == 0) {
        ACE_ERROR((LM_ERROR, "Unable to attach to shared memory segment.\n"));
        return;
    }

    ACE_RW_Thread_Mutex *rwlock = reinterpret_cast<ACE_RW_Thread_Mutex *>(shm_addr);
    rwlock->acquire_read();// 获取读锁
    // 进行读操作...
    rwlock->release_read();// 释放读锁

    // 或者
    rwlock->acquire_write();// 获取写锁
    // 进行写操作...
    rwlock->release_write();// 释放写锁

    shm_pool.detach();
}

int main(int argc, char **argv) {
    if (create_and_init_mutex_in_shm() != 0)
        return 1;

    // 在多个进程中使用use_mutex_from_shm()

    // 当不再需要时，删除共享内存段
    ACE_Shared_Memory_Seg shm(shm_name);
    shm.remove();

    return 0;
}
