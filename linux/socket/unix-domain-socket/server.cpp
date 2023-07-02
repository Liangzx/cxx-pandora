#include <cstdio>
#include <sys/un.h>
#include <sys/socket.h>
#include <cerrno>
#include <unistd.h>

int main() {
    const char *sock_path = "/var/run/my/my-uds-sock";
    const int buf_size = 100;

    struct sockaddr_un addr{};

    char buf[buf_size];

    auto sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1) {
        fprintf(stderr, "socket error");
        return -1;
    }

    if (remove(sock_path) == -1 && errno != ENOENT) {
        fprintf(stderr, "remove socket:%s", sock_path);
        return -1;
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, sock_path, sizeof(addr.sun_path) - 1);
    if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1) {
        fprintf(stderr, "bind error.");
        return -1;
    }

    if (listen(sfd, 8) == -1) {
        fprintf(stderr, "listen error");
        return -1;
    }

    for (;;) {
        auto cfd = accept(sfd, nullptr, nullptr);
        if (cfd == -1) {
            fprintf(stderr, "accept error.");
            return -1;
        }

        auto n = read(cfd, buf, buf_size);

        if (n == -1) {
            fprintf(stderr, "read error.");
            return -1;
        }

        if (n > 0) {
            write(STDOUT_FILENO, buf, n);
        }

        if (close(cfd) == -1) {
            fprintf(stderr, "close error.");
            return -1;
        }
    }

    return 0;
}