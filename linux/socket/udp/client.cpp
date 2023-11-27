#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>

void echo(FILE *fp, int sockfd, const struct sockaddr *pservaddr, socklen_t servlen) {
    const int MAXLINE = 1024;
    int n;
    char sendline[MAXLINE], recvline[MAXLINE + 1];

    if (connect(sockfd, (struct sockaddr *) pservaddr, sizeof(*pservaddr)) < 0) {
        perror("connect error");
        exit(1);
    }


    while (fgets(sendline, MAXLINE, fp) != NULL) {
        // if (sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen) < 0) {
        //     perror("sendto error");
        //     exit(1);
        // }
        // 使用connect 后可以直接使用 write 函数了
        write(sockfd, sendline, strlen(sendline));

        if ((n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL)) < 0) {
            perror("recvfrom error");
            exit(1);
        }

        recvline[n] = '\0';
        fputs(recvline, stdout);

    }
}

int main() {
    struct sockaddr_in sock_addr{};
    std::string server_address = "127.0.0.1";
    auto port = 6000;

    // build address
    bzero(&sock_addr, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, server_address.c_str(), &sock_addr.sin_addr) <= 0) {
        perror("inet_pton error");
        exit(1);
    }

    // create socket
    auto sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0) {
        perror("socket error");
        exit(1);
    }
    //
    echo(stdin, sock_fd, (struct sockaddr *) &sock_addr, sizeof(sock_addr));
    return 0;
}