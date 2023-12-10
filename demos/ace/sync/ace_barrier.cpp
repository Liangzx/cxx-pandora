#include "ace/Synch.h"
#include "ace/Thread.h"
static int number = 0;
static int seed = 0;
class Args {
 public:
  Args(ACE_Barrier *barrier) : barrier_(barrier) {}
  ACE_Barrier *barrier_;
};

static void *worker(void *arguments) {
  Args *arg = (Args *)arguments;
  ACE_DEBUG((LM_DEBUG, "Thread (%t) Created to do some work\n"));
  ::number++;
  // Work
  ACE_OS::sleep(ACE_OS::rand() % 2);
  // Exiting now
  ACE_DEBUG(
      (LM_DEBUG, "\tThread (%t) Done! \n\tThe number is now: %d\n", number));
  // Let the barrier know we are done.
  arg->barrier_->wait();
  ACE_DEBUG((LM_DEBUG, "Thread (%t) is exiting \n"));
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    ACE_DEBUG((LM_DEBUG, "Usage: %s <number of threads>\n", argv[0]));
    ACE_OS::exit(1);
  }
  int n_threads = ACE_OS::atoi(argv[1]);
  ACE_DEBUG((LM_DEBUG, "Preparing to spawn %d threads", n_threads));
  // Setup the random number generator
  ACE_OS::srand(::seed);
  // Setup arguments for threads
  ACE_Barrier barrier(n_threads);
  Args arg(&barrier);
  // Spawn off n_threads number of threads
  for (int i = 0; i < n_threads; i++) {
    if (ACE_Thread::spawn((ACE_THR_FUNC)worker, (void *)&arg,
                          THR_DETACHED | THR_NEW_LWP) == -1)
      ACE_DEBUG((LM_DEBUG, "Error in spawning thread\n"));
  }
  // Wait for all the other threads to let the main thread
  //  know that they are done using the barrier
  barrier.wait();
  ACE_DEBUG((LM_DEBUG, "(%t)Other threads are finished. Program exiting..\n"));
  ACE_OS::sleep(2);
  
  return 0;
}