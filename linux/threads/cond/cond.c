#include <pthread.h>
#include <stdio.h>

int value = 0;
pthread_mutex_t mutex;
pthread_cond_t cond;

void *producer(void *arg) {
  pthread_mutex_lock(&mutex);
  value = 1;
  printf("Producer set value to 1\n");
  pthread_cond_signal(&cond);
  pthread_mutex_unlock(&mutex);
  return NULL;
}

void *consumer(void *arg) {
  pthread_mutex_lock(&mutex);
  while (value == 0) { // 循环检查条件
    pthread_cond_wait(&cond, &mutex);
  }
  printf("Consumer found value %d\n", value);
  pthread_mutex_unlock(&mutex);
  return NULL;
}

int main() {
  pthread_t prod_tid, cons_tid;
  pthread_cond_init(&cond, NULL);
  pthread_mutex_init(&mutex, NULL);

  pthread_create(&prod_tid, NULL, producer, NULL);
  pthread_create(&cons_tid, NULL, consumer, NULL);

  pthread_join(prod_tid, NULL);
  pthread_join(cons_tid, NULL);

  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);
  return 0;
}
