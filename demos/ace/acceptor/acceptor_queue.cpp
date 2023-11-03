#include "ace/Acceptor.h"
#include "ace/Reactor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Svc_Handler.h"
#include "ace/Synch.h"
#include "ace/Thread.h"
#include "ace/Timer_Queue.h"

#define NETWORK_SPEED 3
class MyServiceHandler;  // forward declaration
typedef ACE_Singleton<ACE_Reactor, ACE_Null_Mutex> Reactor;
typedef ACE_Acceptor<MyServiceHandler, ACE_SOCK_ACCEPTOR> Acceptor;

class MyServiceHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH> {
  // The message sender and creator threads are handled here.
  ACE_thread_t thread_ids[2];

 public:
  int open(void *) {
    ACE_DEBUG((LM_DEBUG, "Acceptor: received new connection \n"));
    // Register with the reactor to remember this handler..
    Reactor::instance()->register_handler(this, ACE_Event_Handler::READ_MASK);
    auto last_input_time =  this->reactor()->timer_queue()->gettimeofday();
    ACE_DEBUG((LM_DEBUG, "Acceptor: ThreadID:(%t) open\n"));
    // Create two new threads to create and send messages to the
    // remote machine.
    activate(THR_NEW_LWP,
             2,  // 2 new threads
             0, ACE_DEFAULT_THREAD_PRIORITY, -1, this, 0, 0, 0,
             thread_ids);  // identifiers in thread_handles
    // keep the service handler registered with the acceptor.
    return 0;
  }

  void send_message(void) {
    // Dequeue the message and send it off
    ACE_DEBUG((LM_DEBUG, "(%t)Sending message::>>"));
    // dequeue the message from the message queue
    ACE_Message_Block *mb;
    ACE_ASSERT(this->getq(mb) != -1);
    int length = mb->length();
    char *data = mb->rd_ptr();
    // Send the data to the remote peer
    ACE_DEBUG((LM_DEBUG, "%s \n", data, length));
    peer().send_n(data, length);
    // Simulate very SLOW network.
    ACE_OS::sleep(NETWORK_SPEED);
    // release the message block
    mb->release();
  }  // end send_message

  int construct_message(void) {  // A very fast message creation algorithm
    // would lead to the need for queuing messages..
    // here. These messages are created and then sent
    // using the SLOW send_message() routine which is
    // running in a different thread so that the message
    // construction thread isn’t blocked.
    ACE_DEBUG((LM_DEBUG, "(%t)Constructing message::>> "));
    // Create a new message to send
    ACE_Message_Block *mb;
    char *data = "Hello Connector";
    ACE_NEW_RETURN(
        mb,
        ACE_Message_Block(16,                          // Message 16 bytes long
                          ACE_Message_Block::MB_DATA,  // Set header to data
                          0,                           // No continuations.
                          data  // The data we want to send
                          ),
        0);
    mb->wr_ptr(16);  // Set the write pointer.
    // Enqueue the message into the message queue
    // we COULD have done a timed wait for enqueuing in case
    // someone else holds the lock to the queue so it doesn’t block
    // forever..
    ACE_ASSERT(this->putq(mb) != -1);
    ACE_DEBUG((LM_DEBUG, "Enqueued msg successfully\n"));
  }

  int svc(void) {
    ACE_DEBUG((LM_DEBUG, "(%t) Svc thread \n"));
    // call the message creator thread
    if (ACE_Thread::self() == thread_ids[0])
      while (1) construct_message();  // create messages forever
    else
      while (1) send_message();  // send messages forever
    return 0;                    // keep the compiler happy.
  }

  int handle_input(ACE_HANDLE) {
    ACE_DEBUG((LM_DEBUG, "(%t) handle_input ::"));
    char *data = new char[13];
    // Check if peer aborted the connection
    if (peer().recv_n(data, 12) == 0) {
      printf("Peer probably aborted connection");
      return -1;  // de-register from the Reactor.
    }
    // Show what you got..
    ACE_OS::printf("<< %s\n", data);
    // keep yourself registered
    return 0;
  }
};

int main(int argc, char *argv[]) {
  ACE_INET_Addr addr(10101);
  ACE_DEBUG((LM_DEBUG, "Thread: (%t) main"));
  // Prepare to accept connections
  Acceptor myacceptor(addr, Reactor::instance());
  // wait for something to happen.
  while (1) Reactor::instance()->handle_events();
  return 0;
}