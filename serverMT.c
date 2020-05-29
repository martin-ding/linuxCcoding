#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <ctype.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "wrap.h"

#include <pthread.h>


#define PORT 6666
#define BACKLOG 1	


//子线程
void * child_thread(void * arg)
{
	int cfd = (int) arg;
	printf("%d\n", cfd);
	int n, i;
	char buf[BUFSIZ] = {0};

	while (1) {
		n = Read(cfd, buf, BUFSIZ);//处理了-1 EINTR 的情况
		if (n == 0) {//读取结束
			close(cfd);
			return 0;
		} else if (n == -1) {// 返回-1真的出错
			perror("read error");
			exit(EXIT_FAILURE);
		} else {
			for (i = 0; i < n; ++i)
				buf[i] = toupper(buf[i]);
			
			Write(cfd, buf, n);
		}
	}
	close(cfd);
	pthread_exit(NULL);
}

int main()
{
	struct sockaddr_in saddr, caddr;
	socklen_t addrlen;
	
	pthread_t tid;
	int ret;
	int sfd, cfd;

	sfd = Socket(AF_INET, SOCK_STREAM, 0);

	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(PORT);//port占16位一共可以有65535 个
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);

	Bind(sfd, (struct sockaddr *)&saddr, sizeof(saddr));

	Listen(sfd, BACKLOG);

	//父进程负责接客和回收子进程
	while (1) {
		addrlen = sizeof(caddr);
		cfd = Accept(sfd, (struct sockaddr *) &caddr, &addrlen);//addrlen传入传输参数，阻塞等待连接

		char ip[INET_ADDRSTRLEN] = {0};
		inet_ntop(AF_INET, &(caddr.sin_addr.s_addr), ip, INET_ADDRSTRLEN);

		printf("client address %d,  %s \n", ntohs(caddr.sin_port), ip);
		printf("%d---\n", cfd);
        // pthread_attr_t pattr;

       	//可以定义一个结构体用来存 cfd 等信息
		ret = pthread_create(&tid, NULL, child_thread, (void *) cfd);
		if (ret != 0) {
			printf("error %d, %s\n", ret, strerror(ret));
		}

		// pthread_detach(tid);//不需要回收
	}

	return 0;
}