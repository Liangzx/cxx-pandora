#include <fcntl.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>

#define PORT 8080
#define BACKLOG 5
#define BUFFER_SIZE 1024

void daemonize() {
  pid_t pid = fork();
  if (pid < 0) {
    exit(EXIT_FAILURE);
  }
  if (pid > 0) {
    exit(EXIT_SUCCESS);  // 父进程退出
  }

  // 创建新的会话
  if (setsid() < 0) {
    exit(EXIT_FAILURE);
  }

  // 设置文件权限掩码
  umask(0);

  // 更改工作目录
  chdir("/");

  // 关闭文件描述符
  for (int x = sysconf(_SC_OPEN_MAX); x > 0; x--) {
    close(x);
  }

  // 重定向标准输入/输出/错误
  open("/dev/null", O_RDWR);  // stdin
  dup(0);                     // stdout
  dup(0);                     // stderr

  // 开始记录日志
  openlog("TcpDaemon", LOG_PID | LOG_CONS, LOG_DAEMON);
}

int main() {
  daemonize();

  int server_fd, new_socket;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  char buffer[BUFFER_SIZE] = {0};
  const char *welcome_msg = "Welcome to the TCP server!\n";

  // 创建 socket
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    syslog(LOG_ERR, "Socket creation failed");
    exit(EXIT_FAILURE);
  }

  // 绑定 socket
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    syslog(LOG_ERR, "Set socket options failed");
    exit(EXIT_FAILURE);
  }

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
  closelog();
  return 0;
}
