#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <ctype.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>


#define PORT 6666
#define BACKLOG 1	

int main()
{
	int sfd, cfd;
	struct sockaddr_in saddr, caddr;
	socklen_t addrlen;
	char buf[BUFSIZ] = {0};
	int n, i;

	signal(SIGPIPE, SIG_IGN);

	sfd = socket(AF_INET, SOCK_STREAM, 0);


	int opt = 1;
	setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));//设置地址重用

	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(PORT);//port占16位一共可以有65535 个
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(sfd, (struct sockaddr *)&saddr, sizeof(saddr));

	listen(sfd, BACKLOG);

	listen : 
	addrlen = sizeof(caddr);
	cfd = accept(sfd, (struct sockaddr *) &caddr, &addrlen);//addrlen传入传输参数，阻塞等待连接

	char ip[INET_ADDRSTRLEN] = {};
	inet_ntop(AF_INET, &(caddr.sin_addr.s_addr), ip, INET_ADDRSTRLEN);

	printf("client address %d,  %s \n", ntohs(caddr.sin_port), ip);
	while (1) {
		n = read(cfd, buf, BUFSIZ);
		if (n < 0) {goto listen;}// 这里瞎几把搞的
		for (i = 0; i < n; ++i)
		{
			buf[i] = toupper(buf[i]);
			/* code */
		}

		write(cfd, buf, n);//此时已经关闭了
		printf("--------\n");
	}

	close (sfd);
	close (cfd);

	return 0;
}