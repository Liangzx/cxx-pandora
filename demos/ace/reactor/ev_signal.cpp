#include <signal.h>

#include "ace/Event_Handler.h"
#include "ace/Log_Msg.h"
#include "ace/Reactor.h"

// Create our subclass to handle the signal events
// that we wish to handle. Since we know that this particular
// event handler is going to be using signals we only overload the
// handle_signal method.
class MyEventHandler : public ACE_Event_Handler {
  int handle_signal(int signum, siginfo_t *, ucontext_t *) override {
    switch (signum) {
      case SIGWINCH:
        ACE_DEBUG((LM_DEBUG, "You pressed SIGWINCH \n"));
        break;
      case SIGINT:
        ACE_DEBUG((LM_DEBUG, "You pressed SIGINT \n"));
        break;
    }
    return 0;
  }
};

int main(int argc, char *argv[]) {
  // instantiate the handler
  MyEventHandler *eh = new MyEventHandler;
  // Register the handler asking to call back when either SIGWINCH
  // or SIGINT signals occur. Note that in both the cases we asked the
  // Reactor to callback the same Event_Handler i.e., MyEventHandler.
  // This is the reason why we had to write a switch statement in the
  // handle_signal() method above. Also note that the ACE_Reactor is
  // being used as a Singleton object (Singleton pattern)
  ACE_Reactor::instance()->register_handler(SIGWINCH, eh);
  ACE_Reactor::instance()->register_handler(SIGINT, eh);

  while (1) {
    // Start the reactors event loop
    ACE_Reactor::instance()->handle_events();
  }

  return 0;
}