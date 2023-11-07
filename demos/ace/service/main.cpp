#include <signal.h>

#include "ace/Event_Handler.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"
#include "ace/Reactor.h"
#include "ace/Service_Config.h"

// The Signal Handler which is used to issue the reconfigure()
// call on the service configurator.
class Signal_Handler : public ACE_Event_Handler {
 public:
  int open() {
    // register the Signal Handler with the Reactor to handle
    // re-configuration signals
    ACE_Reactor::instance()->register_handler(SIGWINCH, this);
    return 0;
  }
  int handle_signal(int signum, siginfo_t *, ucontext_t *) {
    if (signum == SIGWINCH) ACE_Service_Config::reconfigure();
    return 0;
  }
};

int main(int argc, char *argv[]) {
  // Instantiate and start up the Signal Handler. This is uses to
  // handle re-configuration events.
  Signal_Handler sh;
  
  sh.open();
  if (ACE_Service_Config::open(argc, argv) == -1)
    ACE_ERROR_RETURN((LM_ERROR, "%p\n", "ACE_Service_Config::open"), -1);
  while (1) ACE_Reactor::instance()->handle_events();
}