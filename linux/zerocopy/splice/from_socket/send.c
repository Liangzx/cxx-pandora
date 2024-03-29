#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>
#define MAXLINE 4096

int abc(int argc, char** argv)
{
	int sockfd, n;
	char recvline[4096], sendline[4096];
	struct sockaddr_in servaddr;
	if (argc !=2) {
		printf("usage: ./client <ipaddress>\n");
		exit(0);
	}
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(6666);
 //   servaddr.sin_port = htons(80);
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
		printf("inet_pton error for %s\n", argv[1]);
		exit(0);
	}
	if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
		printf("connect error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}
//	printf("send msg to server: \n");
//	fgets(sendline, 4096, stdin);
	strcpy(sendline, "1234\n");
	if (send(sockfd, sendline, strlen(sendline), 0) < 0) {
		printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
	}
	//close(sockfd);
	return 0;
}

int main(int argc, char** argv)
{
	int T = 123;
	while (T--) {
		abc(argc, argv);
		sleep(2);
		//usleep(30000);
	}
	return 0;
}

