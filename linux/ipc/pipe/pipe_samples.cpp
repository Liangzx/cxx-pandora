//
// Created by liangzx on 1/1/2023.
//
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

int read_write() {
    int pipe_fd[2];
    pid_t pid;
    char r_buf[100];
    char w_buf[4];
    char *p_wbuf;
    int r_num;
    int cmd;
    memset(r_buf, 0, sizeof(r_buf));
    memset(w_buf, 0, sizeof(r_buf));
    p_wbuf = w_buf;
    if (pipe(pipe_fd) < 0) {
        printf("pipe create error\n");
        return -1;
    }
    if ((pid = fork()) == 0) {
        printf("\n");
        close(pipe_fd[1]);// close write
        sleep(3);         //确保父进程关闭写端
        r_num = read(pipe_fd[0], r_buf, 100);
        printf("read num is %d the data read from the pipe is %d\n", r_num, atoi(r_buf));
        close(pipe_fd[0]);
        exit(0);
    } else if (pid > 0) {
        close(pipe_fd[0]);// close read
        strcpy(w_buf, "111");
        if (write(pipe_fd[1], w_buf, 4) != -1)
            printf("parent write over\n");
        close(pipe_fd[1]);//write
        printf("parent close fd[1] over\n");
        sleep(10);
    }
    return 0;
}

//
void handle_for_sigpipe() {
    struct sigaction sa;//信号处理结构体
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = SIG_IGN;//设置信号的处理回调函数 这个SIG_IGN宏代表的操作就是忽略该信号
    sa.sa_flags = 0;
    // 将信号和信号的处理结构体绑定
    if (sigaction(SIGPIPE, &sa, NULL)) {
        return;
    }
}

int write_dep_read() {
    int pipe_fd[2];
    pid_t pid;
    char r_buf[4];
    char *w_buf;
    int w_num;
    int cmd;
    memset(r_buf, 0, sizeof(r_buf));
    if (pipe(pipe_fd) < 0) {
        printf("pipe create error\n");
        return -1;
    }
    if ((pid = fork()) == 0) {
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        sleep(10);
        exit(0);
    } else if (pid > 0) {
        sleep(1);         // 等待子进程完成关闭读端的操作
        close(pipe_fd[0]);// close read
        w_buf = "111";
        if ((w_num = write(pipe_fd[1], w_buf, 4)) == -1) {
            printf("write to pipe error: err=%s \n", strerror(errno));
        }
    } else {
        printf("the bytes write to pipe is %d \n", w_num);
    }
    close(pipe_fd[1]);
}

int write_not_atomic() {
    int pipe_fd[2];
    if (pipe(pipe_fd) < 0) {
        printf("pipe create error\n");
        return -1;
    }
    char w_buf[4096 * 2];
    int writenum;
    char r_buf[4096];
    int rnum;
    pid_t pid;
    if ((pid = fork()) == 0) {
        close(pipe_fd[1]);// close write

        while (1) {
            sleep(1);
            rnum = read(pipe_fd[0], r_buf, 1000);
            if (rnum <= 0) {
                break;
            }
            printf("child: readnum is %d\n", rnum);
        }
        close(pipe_fd[0]);
        exit(-1);
    } else if (pid > 0) {

        close(pipe_fd[0]);//write
        memset(r_buf, 0, sizeof(r_buf));
        while (true) {
            if ((writenum = write(pipe_fd[1], w_buf, 1024)) == -1) {
                printf("write to pipe error\n");
                break;
            }

            printf("the bytes write to pipe is %d \n", writenum);
            write(pipe_fd[1], w_buf, 4096);
        }

        close(pipe_fd[1]);
    }


    return 0;
}

int pipe_in_threads() {
    int pipe_fd[2];
    if (pipe(pipe_fd) < 0) {
        printf("pipe create error\n");
        return -1;
    }
    std::thread t([pipe_fd]() {
        char r_buf[4]{};
        read(pipe_fd[0], r_buf, 4);
        close(pipe_fd[0]);
        printf("pipe read %s\n", r_buf);
    });
    write(pipe_fd[1], (char *) "hhxx", 4);
    close(pipe_fd[1]);
    t.join();
}
int main(int argc, char **argv) {
    //    read_write();
    //    handle_for_sigpipe();
    //    write_dep_read();
    //    write_not_atomic();
    pipe_in_threads();
    return 0;
}