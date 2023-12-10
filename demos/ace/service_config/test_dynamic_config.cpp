// test_dynamic_config.cpp/
#include "ace/OS.h"
#include "ace/Service_Config.h"
#include "ace/Event_Handler.h"
#include "ace/Reactor.h"

class Time_Handler : public ACE_Event_Handler {
public:
  // Method which is called back by the Reactor when timeout occurs.
  virtual int handle_timeout(const ACE_Time_Value &tv, const void *arg) {
    // 定时器需要重新设置，否则它在运行一次之后便会被删除！
    // stupid！schedule_timer第三个参数是delay，第四个参数为定时间隔
    // 不指定第四个参数，间隔的默认值会是0，这样定时器只能发挥一次作用
    // 然后就会被删除掉，所以这里才需要不断地重新设置

    ACE_Reactor::instance()->schedule_timer(this, NULL, ACE_Time_Value(5));
    ACE_DEBUG((LM_INFO, "regular timeout!!!\n"));
    ACE_Service_Config::reconfigure();
    return 0;
  }
};

int main(int argc, char *argv[]) {
  // Instantiate and start up the Signal Handler. This is uses to
  // handle re-configuration events.
  Time_Handler th;

  if (ACE_Service_Config::open(argc, argv) == -1)
    ACE_ERROR_RETURN((LM_ERROR, "%p\n", "ACE_Service_Config::open"), -1);

  ACE_Reactor::instance()->schedule_timer(
      &th, NULL, ACE_Time_Value(5)); // set timer to go off with delay

  while (1)
    ACE_Reactor::instance()->handle_events();
}

// g++ -o test_dynamic_config test_dynamic_config.cpp -lACE