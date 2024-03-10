#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

int main() {
  int sockfd;
  struct sockaddr_in server_addr;
  fd_set write_fds;
  struct timeval timeout;

  // 创建套接字
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  // 设置服务器地址信息
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(8080);
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  // 将套接字设置为非阻塞模式
  int flags = fcntl(sockfd, F_GETFL, 0);
  fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

  // 发起连接操作
  int ret =
      connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (ret == 0) {
    printf("Connection established immediately.\n");
  } else if (ret < 0 && errno == EINPROGRESS) {
    // 连接操作正在进行中，需要使用 select 函数等待连接完成
    FD_ZERO(&write_fds);
    FD_SET(sockfd, &write_fds);

    timeout.tv_sec = 5; // 设置超时时间为 5 秒
    timeout.tv_usec = 0;

    ret = select(sockfd + 1, NULL, &write_fds, NULL, &timeout);
    if (ret == -1) {
      perror("select failed");
      exit(EXIT_FAILURE);
    } else if (ret == 0) {
      printf("Connection timeout.\n");
      exit(EXIT_FAILURE);
    } else {
      if (FD_ISSET(sockfd, &write_fds)) {
        int error;
        socklen_t error_len = sizeof(error);
        getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &error_len);
        if (error == 0) {
          printf("Connection established after select.\n");
        } else {
          printf("Connection failed after select.\n");
        }
      }
    }
  } else {
    perror("connect failed");
    exit(EXIT_FAILURE);
  }

  // 将套接字设置回阻塞模式
  fcntl(sockfd, F_SETFL, flags);

  close(sockfd);

  return 0;
}
