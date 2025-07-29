# epoll

epoll 的核心机制​​

epoll 是 Linux 内核提供的​​I/O 多路复用机制​​，用于高效监听多个文件描述符（FD）的事件（如可读、可写）。其核心原理包括：
​​红黑树管理 FD​​：快速增删改查 FD，时间复杂度为 O(log n)。
​​就绪队列（Ready List）​​：当 FD 就绪时，内核将其加入就绪队列，epoll_wait直接返回就绪的 FD，避免轮询开销。
​​事件回调机制​​：通过 epoll_ctl注册 FD 时，内核会为 FD 关联一个回调函数，当 FD 状态变化时触发回调，将 FD 加入就绪队列。

## Refs

[epoll高效的原理](https://www.cnblogs.com/xiaohexiansheng/p/16147452.html)
