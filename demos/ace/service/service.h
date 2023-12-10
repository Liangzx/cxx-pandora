// The Services Header File.
#if !defined(MY_SERVICE_H)
#define MY_SERVICE_H
#include "ace/OS.h"
#include "ace/Synch.h"
#include "ace/Task_T.h"

// A Time Service class. ACE_Task_Base already derives from
// ACE_Service_Object and thus we don’t have to subclass from
// ACE_Service_Object in this case.
class TimeService : public ACE_Task_Base {
 public:
  virtual int init(int argc, char *argv[]) override;
  virtual int fini(void) override;
  virtual int suspend(void) override;
  virtual int resume(void) override;
  virtual int svc(void) override;

 private:
  int canceled_;
  ACE_Condition<ACE_Thread_Mutex> *cancel_cond_;
  ACE_Thread_Mutex *mutex_;
};
#endif