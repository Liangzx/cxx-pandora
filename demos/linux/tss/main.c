#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// 定义一个键，用于访问线程特定数据
static pthread_key_t thread_log_key;

// 写入线程日志的函数
void write_to_thread_log(const char *message)
{
    FILE *thread_log = (FILE *)pthread_getspecific(thread_log_key);
    fprintf(thread_log, "%s\n", message);
}

// 关闭线程日志文件的函数
void close_thread_log(void *thread_log)
{
    fclose((FILE *)thread_log);
}

// 线程函数
void *thread_function(void *arg)
{
    char thread_log_filename[256];
    FILE *thread_log;

    // 为每个线程创建唯一的日志文件名
    sprintf(thread_log_filename, "thread_%d.log", (int)pthread_self());

    // 打开日志文件
    thread_log = fopen(thread_log_filename, "w");

    // 将文件指针与键关联起来
    pthread_setspecific(thread_log_key, thread_log);

    // 使用线程特定数据
    write_to_thread_log("Thread starting...");

    // 模拟一些工作
    for (int i = 0; i < 5; i++) {
        char msg[100];
        sprintf(msg, "Processing item %d", i);
        write_to_thread_log(msg);
        sleep(1);
    }

    write_to_thread_log("Thread finishing...");

    return NULL;
}

int main()
{
    pthread_t threads[3];
    int i;

    // 创建一个键，并指定析构函数
    pthread_key_create(&thread_log_key, close_thread_log);

    // 创建多个线程
    for (i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, thread_function, NULL);
    }

    // 等待所有线程完成
    for (i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    // 删除键
    pthread_key_delete(thread_log_key);

    printf("All threads completed. Check the log files.\n");

    return 0;
}
