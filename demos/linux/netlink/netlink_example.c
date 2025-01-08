#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>

#define NETLINK_USER 31

int create_and_bind_netlink_socket(void) {
    struct sockaddr_nl local;
    int sock_fd;

    // 创建 netlink socket
    sock_fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    if (sock_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 设置本地地址
    memset(&local, 0, sizeof(local));
    local.nl_family = AF_NETLINK;
    local.nl_pid = getpid();  // 使用自己的 PID 作为 ID
    local.nl_groups = 0;      // 不加入任何多播组

    // 绑定 socket
    if (bind(sock_fd, (struct sockaddr *)&local, sizeof(local)) < 0) {
        perror("bind");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    return sock_fd;
}

void send_rtm_getlink_request(int sock_fd) {
    struct nlmsghdr *nlh;
    struct ifinfomsg *ifi;
    char request[4096];
    struct iovec iov;
    struct msghdr msg;
    struct sockaddr_nl dest_addr;

    // 初始化请求头
    nlh = (struct nlmsghdr *)request;
    nlh->nlmsg_len = NLMSG_LENGTH(sizeof(struct ifinfomsg));
    nlh->nlmsg_type = RTM_GETLINK;
    nlh->nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP;
    nlh->nlmsg_seq = time(NULL);
    nlh->nlmsg_pid = getpid();

    // 初始化接口信息结构
    ifi = (struct ifinfomsg *)NLMSG_DATA(nlh);
    ifi->ifi_family = AF_UNSPEC;

    // 设置目标地址（内核）
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.nl_family = AF_NETLINK;
    dest_addr.nl_pid = 0;  // 内核的 PID 是 0
    dest_addr.nl_groups = 0;

    // 准备消息
    iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;
    msg.msg_name = (void *)&dest_addr;
    msg.msg_namelen = sizeof(dest_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    // 发送消息
    if (sendmsg(sock_fd, &msg, 0) < 0) {
        perror("sendmsg");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
}

void receive_and_parse_response(int sock_fd) {
    char buffer[8192];
    struct sockaddr_nl src_addr;
    struct iovec iov = {buffer, sizeof(buffer)};
    struct msghdr msg = { (void *)&src_addr, sizeof(src_addr), &iov, 1, NULL, 0, 0};
    struct nlmsghdr *nh;
    int received_bytes;

    while ((received_bytes = recvmsg(sock_fd, &msg, 0)) > 0) {
        for (nh = (struct nlmsghdr *)buffer; NLMSG_OK(nh, received_bytes); nh = NLMSG_NEXT(nh, received_bytes)) {
            if (nh->nlmsg_type == NLMSG_DONE)
                return;
            if (nh->nlmsg_type == NLMSG_ERROR) {
                fprintf(stderr, "Received error message\n");
                return;
            }

            if (nh->nlmsg_type == RTM_NEWLINK) {
                struct ifinfomsg *ifi = (struct ifinfomsg *)NLMSG_DATA(nh);
                struct rtattr *tb;
                int len = nh->nlmsg_len - NLMSG_LENGTH(sizeof(*ifi));

                printf("Interface Index: %d\n", ifi->ifi_index);

                for (tb = IFLA_RTA(ifi); RTA_OK(tb, len); tb = RTA_NEXT(tb, len)) {
                    if (tb->rta_type == IFLA_IFNAME)
                        printf("Name: %s\n", (char *)RTA_DATA(tb));
                    else if (tb->rta_type == IFLA_ADDRESS && RTA_PAYLOAD(tb) >= 6) {
                        unsigned char *mac = (unsigned char *)RTA_DATA(tb);
                        printf("MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n",
                               mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
                    }
                }
                printf("\n");
            }
        }
    }

    if (received_bytes < 0) {
        perror("recvmsg");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
}

int main() {
    int sock_fd;

    // 创建并绑定 Netlink socket
    sock_fd = create_and_bind_netlink_socket();

    // 发送 RTM_GETLINK 请求
    send_rtm_getlink_request(sock_fd);

    // 接收并解析响应
    receive_and_parse_response(sock_fd);

    // 关闭 socket
    close(sock_fd);

    return 0;
}
