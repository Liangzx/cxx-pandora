#include <iostream>

#include "ace/Acceptor.h"
#include "ace/Reactor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Svc_Handler.h"
#include "ace/Synch.h"

using namespace std;

#define PORT_NUM 10086
#define DATA_SIZE 12
// forward declaration
class My_Svc_Handler;
// Create the Acceptor class
typedef ACE_Acceptor<My_Svc_Handler, ACE_SOCK_ACCEPTOR> MyAcceptor;
// Create a service handler similar to as seen in example 1. Except this
// time include the handle_input() method which will be called back
// automatically by the reactor when new data arrives on the newly
// established connection
class My_Svc_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> {
 public:
  My_Svc_Handler() { data = new char[DATA_SIZE]; }
  int open(void*) {
    cout << "Connection established" << endl;
    // Register the service handler with the reactor
    ACE_Reactor::instance()->register_handler(this,
                                              ACE_Event_Handler::READ_MASK);
    return 0;
  }

  int handle_input(ACE_HANDLE) override {
    // After using the peer() method of ACE_Svc_Handler to obtain a
    // reference to the underlying stream of the service handler class
    // we call recv_n() on it to read the data which has been received.
    // This data is stored in the data array and then printed out
    bzero(data, DATA_SIZE);
    auto n = peer().recv(data, DATA_SIZE);
    if (n == 0) {
      peer().close();
      ACE_OS::printf("<<%p %s n=%d \n", this, data, n);
      return -1;
    }

    ACE_OS::printf("<<%p %s n=%d \n", this, data, n);
    // keep yourself registered with the reactor
    return 0;
  }

 private:
  char* data;
};

int main(int argc, char* argv[]) {
  ACE_INET_Addr addr(PORT_NUM);
  // create the acceptor
  MyAcceptor acceptor(addr,                      // address to accept on
                      ACE_Reactor::instance());  // the reactor to use
  while (1)
    // Start the reactor’s event loop
    ACE_Reactor::instance()->handle_events();
}