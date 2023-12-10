#include "ace/Synch.h"
#include "ace/Thread_Manager.h"

class DataType {
 public:
  DataType() : data(0) {}
  void increment() { data++; }
  void set(int new_data) { data = new_data; }
  void decrement() { data--; }
  int get() { return data; }

 private:
  int data;
};

ACE_TSS<DataType> data;

static void* thread1(void*) {
  data->set(10);
  ACE_DEBUG((LM_DEBUG, "(%t)The value of data is %d \n", data->get()));
  for (int i = 0; i < 5; i++) data->increment();
  ACE_DEBUG((LM_DEBUG, "(%t)The value of data is %d \n", data->get()));
  return 0;
}

static void* thread2(void*) {
  data->set(100);
  ACE_DEBUG((LM_DEBUG, "(%t)The value of data is %d \n", data->get()));
  for (int i = 0; i < 5; i++) data->increment();
  ACE_DEBUG((LM_DEBUG, "(%t)The value of data is %d \n", data->get()));
  return 0;
}

int main(int argc, char* argv[]) {

  // Spawn off the first thread
  ACE_Thread_Manager::instance()->spawn((ACE_THR_FUNC)thread1, 0,
                                        THR_NEW_LWP | THR_DETACHED);
  // Spawn off the second thread
  ACE_Thread_Manager::instance()->spawn((ACE_THR_FUNC)thread2, 0,
                                        THR_NEW_LWP | THR_DETACHED);
  // Wait for all threads in the manager to complete.
  ACE_Thread_Manager::instance()->wait();
  ACE_DEBUG((LM_DEBUG, "Both threads done.Exiting.. \n"));
}

/**
 * pthread_key_t和pthread_key_create()详解
 * https://www.cnblogs.com/klcf0220/p/5807148.html
 *
 */