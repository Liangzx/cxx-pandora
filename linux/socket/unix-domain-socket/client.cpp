#include <stdio.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>


int main() {
    ssize_t numRead;
    const int buf_size = 100;
    char buf[buf_size];

    auto sfd  = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1) {
        fprintf(stderr, "socket error");
        return -1;
    }

    struct sockaddr_un addr{};
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    const char *sock_path = "/var/run/my/my-uds-sock";
    strncpy(addr.sun_path, sock_path, sizeof(addr.sun_path) - 1);

    if (connect(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1) {
        fprintf(stderr, "connect error.");
        return -1;
    }

    while ((numRead = read(STDIN_FILENO, buf, buf_size)) > 0) {
        if (write(sfd, buf, numRead) != numRead) {
            fprintf(stderr, "partial/failed write");
            return -1;
        }
    }

    if (numRead == -1) {
        fprintf(stderr, "read error");
        return -1;
    }
    return 0;
}