## Linux 中不同的时钟类型
在 Linux 中，为了满足不同的需求，系统提供了多种时钟类型。这些时钟类型主要分为两大类：基于系统时间的时钟和基于单调递增时间的时钟。以下是一些主要的时钟类型及其特征：

### 系统时间相关的时钟类型

1. **CLOCK_REALTIME**
   - 这个时钟基于系统的真实时间，可以使用 `date` 命令或系统调用来修改。
   - 它会受到 NTP（网络时间协议）的影响，从而保持与互联网时间服务器的同步。
   - 当系统休眠时，系统时间会停止递增，但在系统恢复时，内核会进行补偿，以保持时间的连续性。

### 单调递增时间相关的时钟类型

2. **CLOCK_MONOTONIC**
   - 这个时钟提供了一个从系统启动开始就单调递增的时间值，不会因为系统时间调整而回退。
   - 当系统休眠时，`CLOCK_MONOTONIC` 的计数会停止，直到系统恢复。

3. **CLOCK_MONOTONIC_RAW**
   - 类似于 `CLOCK_MONOTONIC`，但 `CLOCK_MONOTONIC_RAW` 的计数完全不受 NTP 或其他系统时间调整的影响，提供了一个绝对单调递增的时间基准。
   - 这个时钟类型不受系统调用的影响，即使在 NTP 调整期间也继续递增。

4. **CLOCK_BOOTTIME**
   - 这个时钟也是从系统启动开始计时，但在系统休眠时，`CLOCK_BOOTTIME` 继续递增，不会停止，因此它反映了系统启动以来的真实流逝时间，包括休眠时间。

5. **CLOCK_PROCESS_CPUTIME_ID** 和 **CLOCK_THREAD_CPUTIME_ID**
   - 这些时钟分别记录当前进程或线程的 CPU 时间消耗，用于性能分析和计费等场景。

6. **CLOCK_VIRTUAL**
   - 此时钟记录的是进程的用户模式 CPU 时间。

7. **CLOCK_PROF**
   - 记录的是进程的用户模式和内核模式 CPU 时间的总和。

### 其他时钟类型

8. **CLOCK_REALTIME_COARSE** 和 **CLOCK_MONOTONIC_COARSE**
   - 这些时钟类型提供较低的精度，通常用于不需要高精度计时的场景，以节省系统资源。

9. **CLOCK_UPTIME**
   - 类似于 `CLOCK_BOOTTIME`，但没有包含休眠时间，只记录系统处于运行状态的时间。

10. **CLOCK_SGI_CYCLE**
    - 一些旧的 SGI 系统提供的时钟类型，现在已经很少使用。

### 总结

每种时钟类型都有其特定的用途和特性，选择哪种时钟类型取决于应用程序的具体需求，比如是否需要绝对的单调递增、是否需要考虑休眠时间、是否需要高精度等。在编写需要时间敏感的应用程序时，理解这些时钟类型之间的差异是非常重要的。


## Refs
[C++ 定时器任务类实现](https://www.cnblogs.com/zhanggaofeng/p/9410616.html)
[Linux epoll版定时器](https://www.cnblogs.com/zhanggaofeng/p/9410639.html)
[Linux不同时钟的区别](https://www.cnblogs.com/wanng/p/12677441.html)
[基于epoll的定时器](https://www.cnblogs.com/my_life/articles/7646824.html)
[Linux C++ 实现时间轮 优化超时检测机制](https://blog.csdn.net/haolipengzhanshen/article/details/52012096)
