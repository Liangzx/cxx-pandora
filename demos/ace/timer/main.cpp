#include "ace/Reactor.h"
#include "ace/Log_Msg.h"

class MyTimerHandler : public ACE_Event_Handler {
public:
    virtual int handle_timeout(const ACE_Time_Value &tv, const void *act) {
        ACE_DEBUG((LM_DEBUG, "MyTimerHandler::handle_timeout called at %d:%d\n",
                   tv.sec(), tv.usec()));
        return 0;
    }
};

int main(int argc, char *argv[]) {
    ACE_Reactor reactor;

    MyTimerHandler handler;
    ACE_Time_Value interval(3); // 3 seconds

    if (reactor.schedule_timer(&handler, 0, interval, interval) == -1) {
        ACE_ERROR((LM_ERROR, "Failed to schedule timer\n"));
        return 1;
    }

    reactor.run_reactor_event_loop();

    return 0;
}
