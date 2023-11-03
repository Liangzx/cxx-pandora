#include "ace/Synch.h"
#include "ace/Thread.h"
// Arguments that are to be passed to the worker thread are passed
// through this struct.
struct Args {
 public:
  Args(int iterations) : mutex_(), iterations_(iterations) {}
  ACE_Thread_Mutex mutex_;
  int iterations_;
};
// The starting point for the worker threads
static void *worker(void *arguments) {
  Args *arg = (Args *)arguments;
  for (int i = 0; i < arg->iterations_; i++) {
    ACE_DEBUG((LM_DEBUG, "(%t) Trying to get a hold of this iteration\n"));
    // This is our critical section
    arg->mutex_.acquire();
    ACE_DEBUG((LM_DEBUG, "(%t) This is iteration number %d\n", i));
    ACE_OS::sleep(2);
    // simulate critical work
    arg->mutex_.release();
  }
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    ACE_OS::printf("Usage: %s <number_of_threads> <number_of_iterations>\n",
                   argv[0]);
    ACE_OS::exit(1);
  }
  Args arg(ACE_OS::atoi(argv[2]));
  // Setup the arguments
  int n_threads = ACE_OS::atoi(argv[1]);
  // determine the number of threads to be spawned.
  ACE_thread_t *threadID = new ACE_thread_t[n_threads];
  ACE_hthread_t *threadHandles = new ACE_hthread_t[n_threads];
  if (ACE_Thread::spawn_n(threadID,   // id’s for each of the threads
                          n_threads,  // number of threads to spawn
                          (ACE_THR_FUNC)worker,  // entry point for new thread
                          &arg,                  // args to worker
                          THR_JOINABLE | THR_NEW_LWP,  // flags
                          ACE_DEFAULT_THREAD_PRIORITY, 0, 0,
                          threadHandles) == -1)
    ACE_DEBUG((LM_DEBUG, "Error in spawning thread\n"));
  // spawn n_threads
  for (int i = 0; i < n_threads; i++) ACE_Thread::join(threadHandles[i]);
  // Wait for all the threads to exit before you let the main fall through
  // and have the process exit.
  return 0;
}