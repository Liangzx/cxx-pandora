#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define FIFO_SERVER "/tmp/fifoserver"

int main() {
    int fd;
    char w_buf[4096 * 2];
    int real_wnum;
    memset(w_buf, 0, 4096 * 2);
    if ((mkfifo(FIFO_SERVER, O_CREAT | O_EXCL) < 0) && (errno != EEXIST)) {
        printf("cannot create fifoserver\n");
    }
    fd = open(FIFO_SERVER, O_WRONLY | O_NONBLOCK, 0);
    real_wnum = write(fd, w_buf, 2048);
    if (real_wnum == -1) {
        if (errno == EAGAIN) {
            printf("write to fifo error; try later\n");
        }
    } else {
        printf("real write num is %d\n", real_wnum);
    }
    real_wnum = write(fd, w_buf, 5000);
    if (real_wnum == -1) {
        if (errno == EAGAIN) {
            printf("try later\n");
        }
    }

    return 0;
}