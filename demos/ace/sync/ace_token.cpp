#include "ace/Thread.h"
#include "ace/Token.h"
// Arguments that are to be passed to the worker thread are passed
// through this struct.
struct Args {
 public:
  Args(int iterations) : token_("myToken"), iterations_(iterations) {}
  ACE_Token token_;
  int iterations_;
};
// The starting point for the worker threads
static void* worker(void* arguments) {
  Args* arg = (Args*)arguments;
  for (int i = 0; i < arg->iterations_; i++) {
    ACE_DEBUG((LM_DEBUG, "(%t) Trying to get a hold of this iteration\n"));
    // This is our critical section
    arg->token_.acquire();
    ACE_DEBUG((LM_DEBUG, "(%t) This is iteration number %d\n", i));
    // work
    ACE_OS::sleep(2);
    arg->token_.release();
  }

  return 0;
}

int main(int argc, char* argv[]) {
  // same as previous examples..
}