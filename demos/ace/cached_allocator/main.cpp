#include <string>

#include "ace/Log_Msg.h"
#include "ace/Malloc_T.h"
// A chunk of size 1K is created. In our case we decided to use a simple array
// as the type for the chunk. Instead of this we could use any struct or class
// that we think is appropriate.
typedef char MEMORY_BLOCK[1024];

// Create an ACE_Cached_Allocator which is passed in the type of the
// "chunk" that it must pre-allocate and assign on the free list.
// Since the Cached_Allocator is a template class we can pretty much
// pass it ANY type we think is appropriate to be a memory block.

typedef ACE_Cached_Allocator<MEMORY_BLOCK, ACE_SYNCH_MUTEX> Allocator;

class MessageManager {
 public:
  // The constructor is passed the number of chunks that the allocator
  // should pre-allocate and maintain on its free list.
  MessageManager(int n_blocks) : allocator_(n_blocks), message_count_(0) {
    mesg_array_ = new char *[n_blocks];
  }
  // Allocate memory for a message using the Allocator. Remember the message
  // in an array and then increase the message count of valid messages
  // on the message array.
  void allocate_msg(const char *msg) {
    mesg_array_[message_count_] =
        (char *)allocator_.malloc(ACE_OS::strlen(msg) + 1);
    ACE_OS::strcpy(mesg_array_[message_count_], msg);
    message_count_++;
  }
  // Free all the memory that was allocated. This will cause the chunks
  // to be returned to the allocator’s internal free list
  // and NOT to the OS.
  void free_all_msg() {
    for (int i = 0; i < message_count_; i++) {
      allocator_.free(mesg_array_[i]);
    }
    message_count_ = 0;
  }
  // Just show all the currently allocated messages in the message array.
  void display_all_msg() {
    for (int i = 0; i < message_count_; i++) {
      ACE_OS::printf("%s\n", mesg_array_[i]);
    }
  }

 private:
  char **mesg_array_;
  Allocator allocator_;
  int message_count_;
};

int main(int argc, char *argv[]) {
  if (argc < 2) {
    ACE_DEBUG((LM_DEBUG, "Usage: %s <Number of blocks>\n", argv[0]));
    exit(1);
  }
  int n_blocks = ACE_OS::atoi(argv[1]);
  
  char *message = new char[64];
  // Instantiate the Memory Manager class and pass in the number of blocks
  // you want on the internal free list.
  MessageManager mm(n_blocks);
  // Use the Memory Manager class to assign messages and free them.
  // Run this in your favorite debug environment and you will notice that the
  // amount of memory your program uses after Memory Manager has been
  // instantiated remains the same. That means the Cached Allocator
  // controls or manages all the memory for the application.
  // Do forever.
  while (1) {
    // allocate the messages somewhere
    ACE_DEBUG((LM_DEBUG, "\n\n\nAllocating Messages\n"));
    for (int i = 0; i < n_blocks; i++) {  
      ACE_OS::sprintf(message, "Message %d: Hi There", i);
      mm.allocate_msg(message);
    }
    // show the messages
    ACE_DEBUG((LM_DEBUG, "Displaying the messages\n"));
    ACE_OS::sleep(2);
    mm.display_all_msg();
    // free up the memory for the messages.
    ACE_DEBUG((LM_DEBUG, "Releasing Messages\n"));
    ACE_OS::sleep(2);
    mm.free_all_msg();
  }
  delete[] message;

  return 0;
}