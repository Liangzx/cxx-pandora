#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>

#define MAX_EVENTS 10
#define PORT 8080

void *worker_thread(void *arg) {
    int epollfd, connfd, n;
    struct epoll_event events[MAX_EVENTS];
    int listenfd = *(int *)arg;

    // 创建 epoll 实例
    if ((epollfd = epoll_create1(0)) == -1) {
        perror("epoll_create1 error");
        exit(1);
    }

    // 注册监听套接字
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = listenfd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev) == -1) {
        perror("epoll_ctl error");
        exit(1);
    }

    while (1) {
        n = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (n == -1) {
            perror("epoll_wait error");
            exit(1);
        }

        for (int i = 0; i < n; i++) {
            if (events[i].data.fd == listenfd) {
                // 接收新连接
                struct sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
                if ((connfd = accept(listenfd, (struct sockaddr *)&client_addr, &client_len)) == -1) {
                    perror("accept error");
                    continue;
                }
                printf("New connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

                // 注册新连接
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = connfd;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &ev) == -1) {
                    perror("epoll_ctl error");
                    close(connfd);
                    continue;
                }
            } else {
                // 处理现有连接
                char buf[1024];
                ssize_t bytes_read = read(events[i].data.fd, buf, sizeof(buf));
                if (bytes_read <= 0) {
                    close(events[i].data.fd);
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                } else {
                    // 处理数据
                    write(events[i].data.fd, buf, bytes_read);
                }
            }
        }
    }

    close(epollfd);
    pthread_exit(NULL);
}

int main() {
    int listenfd, optval = 1;
    struct sockaddr_in servaddr;
    pthread_t threads[4];

    // 创建套接字
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket error");
        exit(1);
    }

    // 设置 SO_REUSEPORT 选项
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval)) < 0) {
        perror("setsockopt error");
        exit(1);
    }

    // 设置地址结构
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(PORT);

    // 绑定套接字
    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind error");
        exit(1);
    }

    // 开始监听
    if (listen(listenfd, 5) < 0) {
        perror("listen error");
        exit(1);
    }
    // 存在惊群效应 https://segmentfault.com/a/1190000039676522
    // https://cloud.tencent.com/developer/article/1842795
    // 启动多个线程
    for (int i = 0; i < 4; i++) {
        if (pthread_create(&threads[i], NULL, worker_thread, &listenfd) != 0) {
            perror("pthread_create error");
            exit(1);
        }
    }

    // 等待所有线程结束
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }

    close(listenfd);
    return 0;
}
