#include <cstdio>
#include <cstdlib>
#include <sys/socket.h>
#include <cstring>
#include <sys/types.h>
#include <netinet/ip.h>

const int SERV_PORT = 6000;
const int MAXLINE = 2048;

void echo(int sock_fd , struct sockaddr *cli_sock_addr , socklen_t len) {
    int n;
    struct sockaddr_in cliaddr{};
    socklen_t len = sizeof(cliaddr);
    char mesg[MAXLINE];

    for (;;) {

        if ((n = recvfrom(sock_fd, mesg, MAXLINE, 0, cli_sock_addr, &len)) < 0) {
            perror("recvfrom error");
            exit(1);
        }

        if ((n = sendto(sock_fd, mesg, n, 0, cli_sock_addr, len)) < 0) {
            perror("sendto error");
            exit(1);
        }
    }

}

int main() {
    struct sockaddr_in sock_addr{};
    // build address
    bzero(&sock_addr, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sock_addr.sin_port = htons(SERV_PORT);

    // create socket
    auto sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0) {
        perror("socket error");
        exit(1);
    }
    //
    if (bind(sock_fd, (struct sockaddr *) &sock_addr, sizeof(sock_addr))) {
        perror("bind error");
        exit(1);
    }
    //
    echo(sock_fd, );
    return 0;
}