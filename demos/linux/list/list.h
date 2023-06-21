/*
注：这个list.h 是为了配合示例程序而建的，内容来自：linux/include/linux/list.h
和相关文件
*/
#ifndef _LINUX_LIST_H
#define _LINUX_LIST_H

struct list_head {
  struct list_head *next, *prev;
};

#define LIST_HEAD_INIT(name) \
  { &(name), &(name) }

#define offsetof(TYPE, MEMBER) ((size_t) & ((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member)                \
  ({                                                   \
    const typeof(((type *)0)->member) *__mptr = (ptr); \
    (type *)((char *)__mptr - offsetof(type, member)); \
  })

static inline void INIT_LIST_HEAD(struct list_head *list) {
  list->next = list;
  list->prev = list;
}

static inline void __list_add(struct list_head *new, struct list_head *prev,
                              struct list_head *next) {
  next->prev = new;
  new->next = next;
  new->prev = prev;
  prev->next = new;
}

/**
 * head 为第一个，head.prev 总是指向第一个插入的，每次添加到 head 后面
 */
//
static inline void list_add(struct list_head *new, struct list_head *head) {
  __list_add(new, head, head->next);
}

// head 为最后一个，head.next 总是指向第一插入的，每次添加到 head 前面
static inline void list_add_tail(struct list_head *new,
                                 struct list_head *head) {
  __list_add(new, head->prev, head);
}

static inline void __list_del(struct list_head *prev, struct list_head *next) {
  next->prev = prev;
  prev->next = next;
}

static inline void list_del(struct list_head *entry) {
  __list_del(entry->prev, entry->next);
  entry->next = NULL;
  entry->prev = NULL;
}

#define prefetch(x) __builtin_prefetch(x)

//注：这里prefetch 是gcc的一个优化，也可以不要
#define list_for_each(pos, head) \
  for (pos = (head)->next; prefetch(pos->next), pos != (head); pos = pos->next)

#define list_for_each_prev(pos, head) \
  for (pos = (head)->prev; pos != (head); pos = pos->prev)

#define list_entry(ptr, type, member) container_of(ptr, type, member)

static inline int list_empty(const struct list_head *head) {
  return head->next == head;
}

#endif

// ref: https://www.cnblogs.com/zhuyp1015/archive/2012/06/02/2532262.html