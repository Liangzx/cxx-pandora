#include "ace/Log_Msg.h"
#include "ace/Reactor.h"
#include "ace/SOCK_Acceptor.h"

#define PORT_NO 19998

typedef ACE_SOCK_Acceptor Acceptor;
// forward declaration
class My_Accept_Handler;

class My_Input_Handler : public ACE_Event_Handler {
 public:
  // Constructor
  My_Input_Handler() { ACE_DEBUG((LM_DEBUG, "Constructor\n")); }
  // Called back to handle any input received
  int handle_input(ACE_HANDLE) {
    // receive the data
    this->peer().recv_n(data, 12);
    ACE_DEBUG((LM_DEBUG, "% s\n", data));
    // do something with the input received.
    // ...
    // keep yourself registered with the reactor
    return 0;
  }
  // Used by the reactor to determine the underlying handle
  ACE_HANDLE get_handle() { return this->peer().get_handle(); }
  // Returns a reference to the underlying stream.
  ACE_SOCK_Stream &peer() { return this->peer_; }

 private:
  ACE_SOCK_Stream peer_;
  char data[12];
};

class My_Accept_Handler : public ACE_Event_Handler {
 public:
  // Constructor
  My_Accept_Handler(ACE_Addr &addr) { this->open(addr); }
  // Open the peer_acceptor so it starts to "listen"
  // for incoming clients.
  int open(ACE_Addr &addr) {
    peer_acceptor.open(addr);
    return 0;
  }
  // Overload the handle input method
  int handle_input(ACE_HANDLE handle) {
    // Client has requested connection to server.
    // Create a handler to handle the connection
    My_Input_Handler *eh = new My_Input_Handler();
    // Accept the connection "into" the Event Handler
    if (this->peer_acceptor.accept(eh->peer(),  // stream
                                   0,           // remote address
                                   0,           // timeout
                                   1) == -1)    // restart if interrupted
      ACE_DEBUG((LM_ERROR, "Error in connection\n"));
    ACE_DEBUG((LM_DEBUG, "Connection established\n"));
    // Register the input event handler for reading
    ACE_Reactor::instance()->register_handler(eh, ACE_Event_Handler::READ_MASK);
    // Unregister as the acceptor is not expecting new clients
    return -1;
  }
  // Used by the reactor to determine the underlying handle
  ACE_HANDLE get_handle(void) const { return this->peer_acceptor.get_handle(); }

 private:
  Acceptor peer_acceptor;
};

int main(int argc, char *argv[]) {
  // Create an address on which to receive connections
  ACE_INET_Addr addr(PORT_NO);
  // Create the Accept Handler which automatically begins to “listen"
  // for client requests for connections
  My_Accept_Handler *eh = new My_Accept_Handler(addr);
  // Register the reactor to call back when incoming client connects
  ACE_Reactor::instance()->register_handler(eh, ACE_Event_Handler::ACCEPT_MASK);
  // Start the event loop
  while (1) ACE_Reactor::instance()->handle_events();
}