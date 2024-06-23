# 互斥量
## 互斥量的初始化

```c
// 将PTHREAD_MUTEX_INITIALIZER赋值给定义的互斥量,
// 这个方法没办法设置互斥量的属性，也不适用于动态分配的互斥量，比较少用
#include <pthread.h>
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// 使用 pthread_mutex_init 初始化互斥量
// 调用pthread_mutex_init之后， 互斥量处于没有加锁的状态
// pthread_mutexattr_t指针的入参，是用来设定互斥量的属性的
#include <pthread.h>
int pthread_mutex_init(pthread_mutex_t *restrict mutex,
				const pthread_mutexattr_t *restrict attr);

```

## 互斥量的销毁

```c
// 1. 使用PTHREAD_MUTEX_INITIALIZER初始化的互斥量无须销毁
// 2. 不要销毁一个已加锁的互斥量， 或者是真正配合条件变量使用的互斥量
// 3. 已经销毁的互斥量， 要确保后面不会有线程再尝试加锁
// 4. 当互斥量处于已加锁的状态， 或者正在和条件变量配合使用，
// 调用pthread_mutex_destroy函数会返回EBUSY错误码
#include <pthread.h>
int pthread_mutex_destroy(pthread_mutex_t *mutex);

```

## 互斥量的加锁和解锁

```c
#include <pthread.h>
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int pthread_mutex_timedlock (pthread_mutex_t * mutex, const struct timespec *abstime);
int pthread_mutex_unlock(pthread_mutex_t *mutex);

```

## 互斥量的属性

```c
int pthread_mutexattr_init (pthread_mutexattr_t *attr); // 初始化互斥量属性为默认属性
int pthread_mutexattr_destroy (pthread_mutexattr_t *attr);// 销毁互斥量属性

/* Get the process-shared flag of the mutex attribute ATTR.  */
int pthread_mutexattr_getpshared (const pthread_mutexattr_t * attr,int *pshared);

/* Set the process-shared flag of the mutex attribute ATTR.  */
int pthread_mutexattr_setpshared (pthread_mutexattr_t *attr, int shared)

```
