#include "ace/Log_Msg.h"
#include "ace/Reactor.h"
#include "ace/Thread_Manager.h"

#include <functional>
#include <iostream>
#include <thread>

class CBB : public ACE_Event_Handler {
private:
  int id_;

public:
  CBB(int id) : id_(id) {}
  virtual int handle_timeout(const ACE_Time_Value &current_time,
                             const void *act /* = 0 */) {
    ACE_TRACE(ACE_TEXT("CBB::handle_timeout "));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("Expiry handled by thread %t id=%d "), id_));
    return 0;
  }
};
// 要创建一个定时器回调处理器,在事件处理器上的handle_timeout方法被分派.
typedef ACE_Thread_Timer_Queue_Adapter<ACE_Timer_Heap>ActiveTimer;
int Start() {
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("The main thread %t has started ")));

  ActiveTimer atimer;
  atimer.activate();

  CBB cb1(1);
  CBB cb2(2);
  int arg1 = 1;
  int arg2 = 2;
  const ACE_Time_Value curr_tv = ACE_OS::gettimeofday();
  ACE_Time_Value interval = ACE_Time_Value(8, 1000);
  ACE_Time_Value tv(3);
  long id1 = atimer.schedule(&cb1, &arg1, curr_tv + tv, interval);

  /*ACE_Time_Value tvv(5);
  long id2 = atimer.schedule(&cb2,&arg2,curr_tv+tvv,interval);*/

  ACE_Thread_Manager::instance()->wait();

  return 0;
}
