#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <iostream>

#define PORT 10086
#define BACKLOG 5
#define BUFFER_SIZE 1024

int main() {
  // 创建守护进程
  if (daemon(0, 0) == -1) {
    perror("daemon failed");
    exit(EXIT_FAILURE);
  }

  int server_fd, new_socket;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  const char *welcome_msg = "Welcome to the TCP server!\n";

  // 创建 socket
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    syslog(LOG_ERR, "Socket creation failed");
    exit(EXIT_FAILURE);
  }

  // 设置 socket 选项
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    syslog(LOG_ERR, "Set socket options failed");
    exit(EXIT_FAILURE);
  }

  // 绑定 socket
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    syslog(LOG_ERR, "Bind failed");
    exit(EXIT_FAILURE);
  }

  // 开始监听
  if (listen(server_fd, BACKLOG) < 0) {
    syslog(LOG_ERR, "Listen failed");
    exit(EXIT_FAILURE);
  }

  syslog(LOG_INFO, "TCP server is running...");

  while (true) {
    // 接受连接
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0) {
      syslog(LOG_ERR, "Accept failed");
      continue;
    }

    // 发送欢迎消息
    send(new_socket, welcome_msg, strlen(welcome_msg), 0);
    syslog(LOG_INFO, "Sent welcome message to client");

    // 关闭连接
    close(new_socket);
  }

  // 关闭 server socket
  close(server_fd);
  return 0;
}
