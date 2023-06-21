#include <stdio.h>
#include <string.h>

#include "list.h"

#define MAX_NAME_LEN 32
#define MAX_ID_LEN 10

typedef struct stud {
  struct list_head list;
  char name[MAX_NAME_LEN];
  char stu_number[MAX_ID_LEN];
} num_n_stu;

int main(void) {
  struct list_head head;
  num_n_stu stu_1;
  num_n_stu stu_2;
  num_n_stu *entry;

  struct list_head *p;
  // prev 和 next 指针分别指向 head
  INIT_LIST_HEAD(&head);

  strcpy(stu_1.name, "1");
  strcpy(stu_1.stu_number, "10000000");

  strcpy(stu_2.name, "2");
  strcpy(stu_2.stu_number, "10000001");
  list_add_tail(&stu_1.list, &head);
  list_add_tail(&stu_2.list, &head);
  //  list_del(&stu_2.list);
  list_for_each(p, &head) {
    entry = list_entry(p, num_n_stu, list);
    printf("name: %s\n", entry->name);
    printf("stu_number: %s\n", entry->stu_number);
  }
  printf("-------------------------------------\n");
  list_for_each_prev(p, &head) {
    entry = list_entry(p, num_n_stu, list);
    printf("name: %s\n", entry->name);
    printf("stu_number: %s\n", entry->stu_number);
  }
  list_del(&stu_1.list);
  return 0;
}