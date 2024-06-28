#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>

int value = 0;
pthread_mutex_t mutex;
pthread_cond_t cond;

void *producer(void *arg) {
  sleep(5); // 生产者延迟5秒再设置值
  pthread_mutex_lock(&mutex);
  value = 1;
  printf("Producer set value to 1\n");
  pthread_cond_signal(&cond);
  pthread_mutex_unlock(&mutex);
  return NULL;
}

void *consumer(void *arg) {
  pthread_mutex_lock(&mutex);
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  ts.tv_sec += 3; // 设置3秒的超时时间

  while (value == 0) {
    int ret = pthread_cond_timedwait(&cond, &mutex, &ts);
    if (ret == ETIMEDOUT) {
      printf("Consumer timed out\n");
      break;
    }
  }
  if (value == 1) {
    printf("Consumer found value %d\n", value);
  }
  pthread_mutex_unlock(&mutex);
  return NULL;
}

void *wait_timeout() {
  pthread_mutex_lock(&mutex);
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  ts.tv_sec += 10; // 设置3秒的超时时间
  while (value == 0) {
    int ret = pthread_cond_timedwait(&cond, &mutex, &ts);
    if (ret == ETIMEDOUT) {
      printf("Consumer timed out\n");
      break;
    }
  }
  pthread_mutex_unlock(&mutex);
  return NULL;
}

int main() {
  pthread_t prod_tid, cons_tid;
  pthread_cond_init(&cond, NULL);
  pthread_mutex_init(&mutex, NULL);

  // --------------
  // pthread_create(&prod_tid, NULL, producer, NULL);
  // pthread_create(&cons_tid, NULL, consumer, NULL);

  // pthread_join(prod_tid, NULL);
  // pthread_join(cons_tid, NULL);
  // --------------
  wait_timeout();

  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);

  return 0;
}
