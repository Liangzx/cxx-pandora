#include "ace/Thread_Manager.h"

#include "ace/Get_Opt.h"

static void* taskone(void*) {
  ACE_DEBUG((LM_DEBUG, "Thread:(%t)started Task one! \n"));
  ACE_OS::sleep(2);
  ACE_DEBUG((LM_DEBUG, "Thread:(%t)finished Task one!\n"));
  return 0;
}

static void* tasktwo(void*) {
  ACE_DEBUG((LM_DEBUG, "Thread:(%t)started Task two!\n"));
  ACE_OS::sleep(1);
  ACE_DEBUG((LM_DEBUG, "Thread:(%t)finished Task two!\n"));
  return 0;
}
static void print_usage_and_die() {
  ACE_DEBUG((
      LM_DEBUG,
      "Usage program_name-a<num threads for pool1> -b<num threads for pool2>"));
  ACE_OS::exit(1);
}

int main(int argc, char* argv[]) {
  int num_task_1;
  int num_task_2;

  if (argc < 3) {
    print_usage_and_die();
  }

  ACE_Get_Opt get_opt(argc, argv, "a:b:");
  char c;
  while ((c = get_opt()) != EOF) {
    switch (c) {
      case 'a':
        num_task_1 = ACE_OS::atoi(get_opt.optarg);
        break;
      case 'b':
        num_task_2 = ACE_OS::atoi(get_opt.optarg);
        break;
      default:
        ACE_ERROR((LM_ERROR, "Unknown option\n"));
        ACE_OS::exit(1);
    }
  }
  // Spawn the first set of threads that work on task 1.
  if (ACE_Thread_Manager::instance()->spawn_n(
          num_task_1,
          (ACE_THR_FUNC)taskone,  // Execute task one
          0,                      // No arguments
          THR_NEW_LWP,            // New Light Weight Process
          ACE_DEFAULT_THREAD_PRIORITY,
          1) == -1)  // Group ID is 1
  {
    ACE_ERROR((LM_ERROR, "Failure to spawn first group of threads: %p \n"));
  }

  // Spawn second set of threads that work on task 2.
  if (ACE_Thread_Manager::instance()->spawn_n(
          num_task_2,
          (ACE_THR_FUNC)tasktwo,  // Execute task one
          0,                      // No arguments
          THR_NEW_LWP,            // New Light Weight Process
          ACE_DEFAULT_THREAD_PRIORITY,
          2) == -1)  // Group ID is 2
  {
    ACE_ERROR((LM_ERROR, "Failure to spawn second group of threads: %p \n"));
  }

  // Wait for all tasks in grp 1 to exit
  ACE_Thread_Manager::instance()->wait_grp(1);
  ACE_DEBUG((LM_DEBUG, "Tasks in group 1 have exited! Continuing \n"));
  // Wait for all tasks in grp 2 to exit
  ACE_Thread_Manager::instance()->wait_grp(2);
  ACE_DEBUG((LM_DEBUG, "Tasks in group 2 have exited! Continuing \n"));

  return 0;
}