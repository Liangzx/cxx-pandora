#include <signal.h>
#include <stdio.h>
#include <unistd.h>
int main() {
    union sigval val;
    val.sival_int = 8888;


    sigqueue(3972, SIGUSR1, val);
}
