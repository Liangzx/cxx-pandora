# 消息队列
***
## System V
### 消息队列基本概念
消息队列的本质是由Linux内核创建用于存放消息的链表，其功能是用来存放消息的，所以又称之为消息队列。
### 消息队列的数据结构

**消息队列的结构**

[msqid_ds](https://elixir.bootlin.com/linux/v6.1.1/source/include/uapi/linux/msg.h#L18)
```c
/* Obsolete, used only for backwards compatibility and libc5 compiles */
// msqid_ds 用来设置或返回消息队列的信息，存在于用户空间
struct msqid_ds {
	struct ipc_perm msg_perm;   // 消息队列的读写权限和所有者
	struct msg *msg_first;		/* first message on queue,unused  */
	struct msg *msg_last;		/* last message in queue,unused */
	__kernel_old_time_t msg_stime;	/* last msgsnd time */
	__kernel_old_time_t msg_rtime;	/* last msgrcv time */
	__kernel_old_time_t msg_ctime;	/* last change time */
	unsigned long  msg_lcbytes;	/* Reuse junk fields for 32 bit */
	unsigned long  msg_lqbytes;	/* ditto */
	unsigned short msg_cbytes;	/* current number of bytes on queue */
	unsigned short msg_qnum;	/* number of messages in queue */
	unsigned short msg_qbytes;	/* max number of bytes on queue */
	__kernel_ipc_pid_t msg_lspid;	/* pid of last msgsnd */
	__kernel_ipc_pid_t msg_lrpid;	/* last receive pid */
};
```
[msq_queue](https://elixir.bootlin.com/linux/v6.1.1/source/ipc/msg.c#L49)
```c
/* one msq_queue structure for each present queue on the system */
// struct msg_queue 是每个消息队列的队列头
struct msg_queue {
	struct kern_ipc_perm q_perm;
	time64_t q_stime;		/* last msgsnd time */
	time64_t q_rtime;		/* last msgrcv time */
	time64_t q_ctime;		/* last change time */
	unsigned long q_cbytes;		/* current number of bytes on queue */
	unsigned long q_qnum;		/* number of messages in queue */
	unsigned long q_qbytes;		/* max number of bytes on queue */
	struct pid *q_lspid;		/* pid of last msgsnd */
	struct pid *q_lrpid;		/* last receive pid */

	struct list_head q_messages;
	struct list_head q_receivers;
	struct list_head q_senders;
} __randomize_layout;
```
**消息队列节点的结构：**

[msg](https://elixir.bootlin.com/linux/2.2.22/source/include/linux/msg.h#L65)
```c
/* one msg structure for each message */
struct msg {
	struct msg *msg_next;   /* next message on queue */
	long  msg_type;          
	char *msg_spot;         /* message text address */
	time_t msg_stime;       /* msgsnd time */
	short msg_ts;           /* message text size */
};
```
[msg_msg](https://elixir.bootlin.com/linux/v6.1.1/source/include/linux/msg.h#L9)
```c
/* one msg_msg structure for each message */
struct msg_msg {
	struct list_head m_list;
	long m_type;
	size_t m_ts;		/* message text size */
	struct msg_msgseg *next;
	void *security;
	/* the actual message follows immediately */
};
```
[ipc_perm](https://elixir.bootlin.com/linux/v6.1.1/source/include/uapi/linux/ipc.h#L10)
```c
/* Obsolete, used only for backwards compatibility and libc5 compiles */
struct ipc_perm
{
	__kernel_key_t	key;
	__kernel_uid_t	uid;
	__kernel_gid_t	gid;
	__kernel_uid_t	cuid;
	__kernel_gid_t	cgid;
	__kernel_mode_t	mode; 
	unsigned short	seq;
};
```
[kern_ipc_perm](https://elixir.bootlin.com/linux/v6.1.1/source/include/linux/ipc.h#L12)
```c
/* used by in-kernel data structures */
struct kern_ipc_perm {
	spinlock_t	lock;
	bool		deleted;
	int		id;
	key_t		key;
	kuid_t		uid;
	kgid_t		gid;
	kuid_t		cuid;
	kgid_t		cgid;
	umode_t		mode;
	unsigned long	seq;
	void		*security;

	struct rhash_head khtnode;

	struct rcu_head rcu;
	refcount_t refcount;
} ____cacheline_aligned_in_smp __randomize_layout;
```
[msgbuf](https://elixir.bootlin.com/linux/v6.1.1/source/include/uapi/linux/msg.h#L38)
```c
/* message buffer for msgsnd and msgrcv calls */
struct msgbuf {
	__kernel_long_t mtype;          /* type of message */
	char mtext[1];                  /* message text */
};
```
[msg_ids](https://elixir.bootlin.com/linux/v2.6.18/source/ipc/msg.c#L72)
```c
static struct ipc_ids msg_ids; //struct ipc_ids msg_ids 是内核中记录消息队列的全局数据结构
```
[ipc_ids v2.6.18](https://elixir.bootlin.com/linux/v2.6.18/source/ipc/util.h#L23)
```c
struct ipc_ids {
	int in_use;
	int max_id;
	unsigned short seq;
	unsigned short seq_max;
	struct mutex mutex;
	struct ipc_id_ary nullentry;
	struct ipc_id_ary* entries;
};
```
[ipc_ids v6.1.1](https://elixir.bootlin.com/linux/v6.1.1/source/include/linux/ipc_namespace.h#L18)
```c
struct ipc_ids {
	int in_use;
	unsigned short seq;
	struct rw_semaphore rwsem;
	struct idr ipcs_idr;    // 使用了 idr
	int max_idx;
	int last_idx;	/* For wrap around detection */
#ifdef CONFIG_CHECKPOINT_RESTORE
	int next_id;
#endif
	struct rhashtable key_ht;
};
```
### System V 消息队列操作函数

```c

```

### 参考

- [linux c编程：System V消息队列一](https://www.cnblogs.com/zhanghongfeng/p/10229026.html)
- [进程间通信方式——消息队列](https://blog.csdn.net/skyroben/article/details/72520501)
- [【操作系统】进程间的通信——消息队列](https://zhuanlan.zhihu.com/p/557294193)
- [Linux系统编程—消息队列](https://www.jianshu.com/p/7e3045cf1ab8)
- [【Linux】消息队列MQ的实际应用(POSIX和 System V)](https://zhuanlan.zhihu.com/p/100484693)
- [linux c编程：Posix消息队列](https://www.cnblogs.com/zhanghongfeng/p/10013563.html)
