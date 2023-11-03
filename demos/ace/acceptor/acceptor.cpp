#include "ace/Acceptor.h"

#include <iostream>

#include "ace/Log_Msg.h"
#include "ace/Reactor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Svc_Handler.h"
#include "ace/Synch.h"

using namespace std;

// Create a Service Handler whose open() method will be called back
// automatically. This class MUST derive from ACE_Svc_Handler which is an
// interface and as can be seen is a template container class itself. The
// first parameter to ACE_Svc_Handler is the underlying stream that it
// may use for communication. Since we are using TCP sockets the stream
// is ACE_SOCK_STREAM. The second is the internal synchronization
// mechanism it could use. Since we have a single threaded application we
// pass it a "null" lock which will do nothing.

class My_Svc_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> {
  // the open method which will be called back automatically after the
  // connection has been established.
 public:
  int open(void*) { cout << "Connection established" << endl; }
};
// Create the acceptor as described above.
typedef ACE_Acceptor<My_Svc_Handler, ACE_SOCK_ACCEPTOR> MyAcceptor;

const int PORT_NUM = 10086;

int main(int argc, char* argv[]) {
  // create the address on which we wish to connect. The constructor takes
  // the port number on which to listen and will automatically take the
  // host’s IP address as the IP Address for the addr object
  ACE_INET_Addr addr(PORT_NUM);
  // instantiate the appropriate acceptor object with the address on which
  // we wish to accept and the Reactor instance we want to use. In this
  // case we just use the global ACE_Reactor singleton. (Read more about
  // the reactor in the previous chapter)
  MyAcceptor acceptor(addr, ACE_Reactor::instance());
  while (1)
    // Start the reactor’s event loop
    ACE_Reactor::instance()->handle_events();
}