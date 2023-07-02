#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

int main() {
    // server addr
    struct sockaddr_in srv_addr{};
    // cli connect addr
    struct sockaddr_in cli_addr{};
    // server listen fd
    int listen_fd{};
    // client fd
    int conn_fd;
    pid_t child_pid;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (listen_fd < 0) {
        perror("socket error");
        exit(1);
    }
    //
    bzero(&srv_addr, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    srv_addr.sin_port = htons(10086);

    if (bind(listen_fd, reinterpret_cast<const sockaddr *>(&srv_addr), sizeof(srv_addr)) < 0) {
        perror("bind error");
        exit(1);
    }

    if (listen(listen_fd, 1024) < 0) {
        perror("listen error");
        exit(1);
    }

    for (;;) {
        auto cli_len = sizeof(cli_addr);
        conn_fd = accept(listen_fd, reinterpret_cast<sockaddr *>(&cli_addr),
                         reinterpret_cast<socklen_t *>(cli_len));

        if (conn_fd < 0) {
            perror("accept error");
            exit(1);
        }
        //
        if ((child_pid = fork()) == 0) {
            close(listen_fd);
            ssize_t n;
            auto max_len = 4096;
            char buff[max_len];

            while ((n = read(conn_fd, buff, max_len)) > 0) {
                write(conn_fd, buff, n);
            }
            exit(0);
        }
        waitpid(child_pid, nullptr, WNOHANG);
        close(conn_fd);
    }

    close(listen_fd);

    return 0;
}