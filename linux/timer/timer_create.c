#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

void timer_expired(int signum) { printf("Timer expired: %d\n", signum); }

int main() {
  struct sigevent sev;
  timer_t timerid;

  /* Initialize sigevent structure */
  sev.sigev_notify = SIGEV_SIGNAL;
  sev.sigev_signo = SIGRTMIN;
  sev.sigev_value.sival_ptr = &timerid;

  /* Create the timer */
  if (timer_create(CLOCK_REALTIME, &sev, &timerid) == -1) {
    perror("timer_create");
    exit(EXIT_FAILURE);
  }

  /* Set the timer */
  struct itimerspec its;
  its.it_value.tv_sec = 5; /* 5 seconds from now */
  its.it_value.tv_nsec = 0;
  its.it_interval.tv_sec = 0; /* Non-repeating timer */
  its.it_interval.tv_nsec = 0;

  if (timer_settime(timerid, 0, &its, NULL) == -1) {
    perror("timer_settime");
    exit(EXIT_FAILURE);
  }

  /* Register signal handler */
  signal(SIGRTMIN, timer_expired);

  /* Wait for timer expiration */
  pause();

  /* Delete the timer */
  if (timer_delete(timerid) == -1) {
    perror("timer_delete");
    exit(EXIT_FAILURE);
  }

  return 0;
}
