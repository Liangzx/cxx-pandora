#include <unistd.h>
#include <iostream>
#include <sys/epoll.h>

int main() {
    ;
    int nfds;
    //ev用于注册事件，数组用于返回要处理的事件
    struct epoll_event ev{}, events[5];
    int epfd = epoll_create(1);

    ev.data.fd = STDOUT_FILENO;
    ev.events = EPOLLOUT | EPOLLET;

    epoll_ctl(epfd, EPOLL_CTL_ADD, STDOUT_FILENO, &ev);//注册epoll事件
    for (;;) {
        nfds = epoll_wait(epfd, events, 5, -1);
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == STDOUT_FILENO)
                cout << "hello world!" << endl;
        }
    }
    return 0;
}

