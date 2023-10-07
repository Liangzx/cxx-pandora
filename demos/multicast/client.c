// https://www.cnblogs.com/schips/p/12552534.html

/*
#    Copyright By Schips, All Rights Reserved
#    https://gitee.com/schips/
#
#    File Name:  group_client.c
#    Created  :  Mon 23 Mar 2020 04:00:49 PM CST
*/

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h> /* See NOTES */
#include <unistd.h>

#define IP_FOUND "IP_FOUND"
#define IP_FOUND_ACK "IP_FOUND_ACK"

/*
广播与多播只支持UDP协议，因为TCP协议是端到端，这与广播与多播的理念相冲突
广播是局域网中一个主机对所有主机的数据通信，而多播是一个主机对一组特定的主机进行通信.多播可以是因特网，而广播只能是局域网。多播常用于视频电话，网上会议等。

setsockopt设置套接字选项可以设置多播的一些相关信息

IP_MULTICAST_TTL //设置多播的跳数值
IP_ADD_MEMBERSHIP
//将主机的指定接口加入多播组，以后就从这个指定的接口发送与接收数据
IP_DROP_MEMBERSHIP //主机退出多播组
IP_MULTICAST_IF //获取默认的接口或设置多播接口
IP_MULTICAST_LOOP //设置或禁止多播数据回送，即多播的数据是否回送到本地回环接口

例子:
int ttl=255;
setsockopt(socket,IPPROTO_IP,IP_MULTICAST_TTL,&ttl,sizeof(ttl));//设置跳数

socket           -套接字描述符
PROTO_IP         -选项所在的协议层
IP_MULTICAST_TTL -选项名
&ttl             -设置的内存缓冲区
sizeof(ttl)      -设置的内存缓冲区长度

struct in_addr in;

setsockopt(socket,IPPROTO_IP,IP_MUTLICAST_IF,&in,sizeof(in));//设置组播接口

int yes=1;
setsockopt(socket,IPPROTO_IP,IP_MULTICAST_LOOP,&yes,sizeof(yes));//设置数据回送到本地回环接口

struct ip_mreq addreq;
setsockopt(socket,IPPROTO_IP,IP_ADD_MEMBERSHIP,&req,sizeof(req));//加入组播组

struct ip_mreq dropreq;
setsockopt(socket,IPPROTO_IP,IP_DROP_MEMBERSHIP,&dropreq,sizeof(dropreq));//离开组播组


*/

#define MCAST_ADDR "224.0.0.88"

int main(int argc, char **argv) {
  int ret, count;
  int sock_fd;
  char send_buf[20];
  char recv_buf[20];

  struct sockaddr_in server_addr;  // 多播地址
  struct sockaddr_in our_addr;
  struct sockaddr_in recvaddr;
  int so_broadcast = 1;

  socklen_t socklen;

  sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;

  // server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  server_addr.sin_addr.s_addr = inet_addr(MCAST_ADDR);  // 多播地址
  server_addr.sin_port = htons(6666);

  // 客户端绑定通信端口，否则系统自动分配
  memset(&our_addr, 0, sizeof(our_addr));
  our_addr.sin_family = AF_INET;
  our_addr.sin_port = htons(7777);
  our_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // MCAST_ADDR
  // 自定义地址如果为有效地址
  // 则协议栈将自定义地址与端口信息发送到接收方
  // 否则协议栈将使用默认的回环地址与自动端口
  // our_addr.sin_addr.s_addr = inet_addr("127.0.0.10");

  ret = bind(sock_fd, (struct sockaddr *)&our_addr, sizeof(our_addr));
  if (ret == -1) {
    perror("bind !");
  }

  socklen = sizeof(struct sockaddr);
  strncpy(send_buf, IP_FOUND, strlen(IP_FOUND) + 1);

  for (count = 0; count < 1; count++) {
    ret = sendto(sock_fd, send_buf, strlen(send_buf) + 1, 0,
                 (struct sockaddr *)&server_addr, socklen);
    if (ret != strlen(send_buf) + 1) {
      perror(" send to !");
    }

    ret = recvfrom(sock_fd, recv_buf, sizeof(recv_buf), 0,
                   (struct sockaddr *)&recvaddr, &socklen);
    if (ret < 0) {
      perror(" recv! ");
    }

    printf(" recv server addr : %s \n", (char *)inet_ntoa(recvaddr.sin_addr));
    printf(" recv server port : %d \n", ntohs(recvaddr.sin_port));
    printf(" recv server msg :%s \n", recv_buf);
  }

  close(sock_fd);

  return 0;
}