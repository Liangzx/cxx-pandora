#include "ace/Event_Handler.h"
#include "ace/Get_Opt.h"
#include "ace/OS.h"
#include "ace/Reactor.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Stream.h"
#include "ace/Thread_Manager.h"
#define BUFSIZE 128
class Client : public ACE_Event_Handler {
 public:
  ~Client() { ACE_DEBUG((LM_DEBUG, "Destructor \n")); }
  // Constructor
  Client(int argc, char *argv[]) : connector_(), stream_() {
    // The user must specify address and port number
    ACE_Get_Opt get_opt(argc, argv, "a:p:");
    for (int c; (c = get_opt()) != -1;) {
      switch (c) {
        case 'a':
          addr_ = get_opt.optarg;
          break;
        case 'p':
          port_ = ((u_short)ACE_OS::atoi(get_opt.optarg));
          break;
        default:
          break;
      }
    }
    address_.set(port_, addr_);
  }
  // Connect to the remote machine
  int connect() {
    connector_.connect(stream_, address_);
    ACE_Reactor::instance()->register_handler(this,
                                              ACE_Event_Handler::READ_MASK);
    return 0;
  }
  // Send a list_services command
  int list_services() {
    stream_.send_n("help", 5);
    return 0;
  }
  // Send the reconfiguration command
  int reconfigure() {
    stream_.send_n("reconfigure", 12);
    return 0;
  }
  // Handle both standard input and remote data from the
  // ACE_Service_Manager
  int handle_input(ACE_HANDLE h) {
    char buf[BUFSIZE];
    // Got command from the user
    if (h == ACE_STDIN) {
      int result = ACE_OS::read(h, buf, BUFSIZ);
      if (result == -1)
        ACE_ERROR((LM_ERROR, "can't read from STDIN"));
      else if (result > 0) {
        // Connect to the Service Manager
        this->connect();
        if (ACE_OS::strncmp(buf, "list", 4) == 0)
          this->list_services();
        else if (ACE_OS::strncmp(buf, "reconfigure", 11) == 0)
          this->reconfigure();
      }
      return 0;
    }
    // We got input from remote
    else {
      switch (stream_.recv(buf, BUFSIZE)) {
        case -1:
          // ACE_ERROR((LM_ERROR, "Error in receiving from remote\n"));
          ACE_Reactor::instance()->remove_handler(this,
                                                  ACE_Event_Handler::READ_MASK);
          return 0;
        case 0:
          return 0;
        default:
          ACE_OS::printf("%s", buf);
          return 0;
      }
    }
  }
  // Used by the Reactor Framework
  ACE_HANDLE get_handle() const { return stream_.get_handle(); }
  // Close down the underlying stream
  int handle_close(ACE_HANDLE, ACE_Reactor_Mask) { return stream_.close(); }

 private:
  ACE_SOCK_Connector connector_;
  ACE_SOCK_Stream stream_;
  ACE_INET_Addr address_;
  char *addr_;
  u_short port_;
};

int main(int argc, char *argv[]) {
  Client client(argc, argv);
  // Register the the client event handler as the standard
  // input handler
  Client::register_stdin_handler(&client, ACE_Reactor::instance(),
                                 ACE_Thread_Manager::instance());

  ACE_Reactor::run_event_loop();

  return 0;
}