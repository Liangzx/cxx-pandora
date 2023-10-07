/*
#    Copyright By Schips, All Rights Reserved
#    https://gitee.com/schips/
#
#    File Name:  boardcast_server.c
#    Created  :  Mon 23 Mar 2020 04:29:48 PM CST
*/

// UDP 服务端
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h> /* See NOTES */
#include <unistd.h>

int main() {
  char send_buf[100] = "hello client  6666";
  char recv_buf[100];

  // 1 创建一个套接字，用于网络通信
  int sk_fd = socket(PF_INET, SOCK_DGRAM, 0);
  if (sk_fd == -1) {
    perror("socket");
    return -1;
  }

  // 设置广播使能
  int on = 1;
  setsockopt(sk_fd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));

  // 2 绑定服务的IP与端口
  struct sockaddr_in ser_addr;
  ser_addr.sin_family = PF_INET;
  ser_addr.sin_port = htons(6666);
  ser_addr.sin_addr.s_addr = inet_addr("192.168.1.88");
  int ret = bind(sk_fd, (struct sockaddr *)&ser_addr, sizeof(ser_addr));
  if (ret == -1) {
    perror("bind");
    return -1;
  }

  // 3 广播数据
  struct sockaddr_in client_addr;
  client_addr.sin_family = PF_INET;
  client_addr.sin_port = htons(6666);
  client_addr.sin_addr.s_addr = inet_addr("192.168.1.255");
  socklen_t size = sizeof(struct sockaddr_in);

  while (1) {
    ret = sendto(sk_fd, send_buf, sizeof(send_buf), 0,
                 (struct sockaddr *)&client_addr, size);
    if (ret == -1) {
      perror("sendto");
      return -1;
    }
  }

  // 4 关闭套接字
  close(sk_fd);

  return 0;
}
