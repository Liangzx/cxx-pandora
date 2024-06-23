// 08_mutex_test.c
// gcc 08_mutex_test.c -lpthread
#include <pthread.h>
#include <stdio.h>
#include <sys/syscall.h>
int g_Count = 0;
pthread_mutex_t g_mutex;
void *func(void *arg) {
  int i = 0;
  for (i = 0; i < 10000000; i++) {
    pthread_mutex_lock(&g_mutex);
    g_Count++;
    pthread_mutex_unlock(&g_mutex);
  }
  return NULL;
}

int main() {
  pthread_mutex_init(&g_mutex, NULL);
  // 创建4个线程
  pthread_t threadId[4];
  int i = 0;
  for (i = 0; i < 4; i++) {
    pthread_create(&threadId[i], NULL, func, NULL);
  }

  for (i = 0; i < 4; i++) {
    pthread_join(threadId[i], NULL);
    printf("join threadId=%lx\n", threadId[i]);
  }
  printf("g_Count=%d\n", g_Count);

  pthread_mutex_destroy(&g_mutex);

  return 0;
}
