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

ssize_t readline(int fd, char *vptr, size_t maxlen) {
    ssize_t n, rc;
    char c, *ptr;

    ptr = vptr;
    for (n = 1; n < maxlen; n++) {
        if ((rc = read(fd, &c, 1)) == 1) {
            *ptr++ = c;
            if (c == '\n')
                break;    /* newline is stored, like fgets() */
        } else if (rc == 0) {
            *ptr = 0;
            return (n - 1);    /* EOF, n - 1 bytes were read */
        } else
            return (-1);        /* error, errno set by read() */
    }

    *ptr = 0;    /* null terminate like fgets() */
    return (n);
}

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    // create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(1);
    }
    // build server address
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(10086);
    if (inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) < 0) {
        perror("inet_pton error");
        exit(1);
    }
    // connect
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("connect error");
        exit(1);
    }

    const int MAX_LINE = 4096;
    char sendline[MAX_LINE];
    char recvline[MAX_LINE];

    // handle message
    while (fgets(sendline, MAX_LINE, stdin) != NULL) {
        write(sockfd, sendline, strlen(sendline));

        if (readline(sockfd, recvline, MAX_LINE) == 0) {
            perror("server terminated prematurely");
            exit(1);
        }

        if (fputs(recvline, stdout) == EOF) {
            perror("fputs error");
            exit(1);
        }
    }
    // close fd
    close(sockfd);
    return 0;
}
