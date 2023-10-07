/*
#    Copyright By Schips, All Rights Reserved
#    https://gitee.com/schips/
#
#    File Name:  boardcast_client.c
#    Created  :  Mon 23 Mar 2020 04:29:48 PM CST
*/

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h> /* See NOTES */
#include <unistd.h>

int main() {
  char send_buf[100] = "GET Msg";
  char recv_buf[100];

  // 1 创建一个套接字，用于网络通信
  int sk_fd = socket(PF_INET, SOCK_DGRAM, 0);
  if (sk_fd == -1) {
    perror("socket");
    return -1;
  }

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
  // 3 等待 服务器广播
  struct sockaddr_in src_addr;
  socklen_t size = sizeof(ser_addr);
  ret = recvfrom(sk_fd, recv_buf, sizeof(recv_buf), 0,
                 (struct sockaddr *)&src_addr, &size);
  if (ret == -1) {
    perror("reveform");
    return -1;
  }
  printf("recv :%s\n", recv_buf);

  // 4 关闭套接字
  close(sk_fd);

  return 0;
}
