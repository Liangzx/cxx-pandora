# condition mutex

## Refs

[Linux C | 多线程编程】线程同步 | 条件变量](https://blog.csdn.net/wkd_007/article/details/137504491)

## 条件变量的初始化

```c
// PTHREAD_COND_INITIALIZER 赋值给定义的条件变量
#include <pthread.h>
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// 用 pthread_cond_init 初始化条件变量
#include <pthread.h>
int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr);

// 注意：永远不要用一个条件变量对另一个条件变量赋值，
// 即pthread_cond_t cond_b=cond_a不合法， 这种行为是未定义的

```

## 条件变量的销毁

```c
// 1. 使用PTHREAD_COND_INITIALIZE静态初始化的条件变量，不需要被销毁
// 2. 要调用pthread_cond_destroy销毁的条件变量可以调用pthread_cond_init重新进行初始化
// 3. 不要引用已经销毁的条件变量， 这种行为是未定义的
#include <pthread.h>
int pthread_cond_destroy(pthread_cond_t *cond);

```

## 条件变量的等待

```c
#include <pthread.h>
int pthread_cond_wait(pthread_cond_t *restrict cond,pthread_mutex_t *restrict mutex);
int pthread_cond_timedwait(pthread_cond_t *restrict cond,pthread_mutex_t *restrict mutex,
  const struct timespec *restrict abstime);

// 函数描述：这两个函数都是让指定的条件变量进入等待状态，其工作机制是先解锁传入的互斥量，
// 再让条件变量等待，从而使所在线程处于阻塞状态。这两个函数返回时，系统会确保该线程再次持有互斥量(加锁).
// 注意：pthread_cond_timedwait指定的时间是绝对时间，而不是相对时间
struct timeval now;
struct timespec outtime;
memset(&outtime,0,sizeof(outtime));
memset(&now,0,sizeof(now));

gettimeofday(&now, NULL);
outtime.tv_sec = now.tv_sec + sec;
outtime.tv_nsec = now.tv_usec * 1000;

// pthread_cond_timedwait 执行后，先加锁pMutex、再等待；被唤醒后会加锁pMutex
if(pthread_cond_timedwait(pCond, pMutex, &outtime) == ETIMEDOUT)
{
	ret = -1;
}

```

## 条件变量的唤醒

```c
#include <pthread.h>
int pthread_cond_signal(pthread_cond_t *cond);
int pthread_cond_broadcast(pthread_cond_t *cond);
// pthread_cond_signal 负责唤醒等待在条件变量上的一个线程，如果有多个线程等待，
// 是唤醒哪一个呢？Linux内核会为每个条件变量维护一个等待队列，调用了
// pthread_cond_wait 或 pthread_cond_timedwait 的线程会按照调用时间先后添加到该队列中。
// pthread_cond_signal会唤醒该队列的第一个.

// thread_cond_broadcast，就是广播唤醒等待在条件变量上的所有线程。
// 前面说过，条件等待的两个函数返回时，系统会确保该线程再次持有互斥量(加锁)，
// 所有，这里被唤醒的所有线程都会去争夺互斥锁，没抢到的线程会继续等待，
// 拿到锁后同样会从条件等待函数返回。所以，被唤醒的线程第一件事就是再次判断条件是否满足！

```
