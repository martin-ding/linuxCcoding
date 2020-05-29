#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/select.h>		
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <strings.h>

/* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include "wrap.h"

#define PORT 6666

int main(int argc, char const *argv[])
{
	int maxfd, ret, connect_fd, clientSocket[FD_SETSIZE] = {-1}, maxi;
	fd_set readfds, rfds;
	struct sockaddr_in addr, con_addr;
	char ip[INET_ADDRSTRLEN];
	socklen_t addrlen;
	char buf[BUFSIZ] = {0};
	int i,j;


	int sfd = Socket(AF_INET, SOCK_STREAM, 0);//服务器端 server socket 文件描述符

	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	// return 0;
	// int r = inet_pton(AF_INET, INADDR_ANY, &(addr.sin_addr.s_addr));
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	// printf("%d ------\n", r);	

	int opt = 1;
	setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	Bind(sfd, (struct sockaddr *)&addr, sizeof(addr));
	
	Listen(sfd, 128);

	maxfd = sfd;//最大的文件描述符
	maxi = -1;
	
	FD_ZERO(&readfds);
	FD_SET(sfd, &readfds);

	while (1) {
		rfds = readfds;//备份
		ret = select(maxfd+1, &rfds, NULL, NULL, NULL);//一直阻塞
		if (ret < 0) {
			perror("select error");
			exit(1);
		}

		if (FD_ISSET(sfd, &rfds)) {//如果有新连接	
			bzero(&con_addr, sizeof(con_addr));
			addrlen = sizeof(con_addr);
			connect_fd = Accept(sfd, (struct sockaddr *)&con_addr, &addrlen);
			printf("port %d, ip %s\n", ntohs(con_addr.sin_port), inet_ntop(AF_INET, &(con_addr.sin_addr.s_addr), ip, INET_ADDRSTRLEN));

			int canSelect = 0;
			//将connect_fd 放入到数组中
			for (i = 0; i < FD_SETSIZE-1; ++i)
			{
				if (clientSocket[i] == -1) {
					canSelect = 1;
					clientSocket[i] = connect_fd;
					if (maxfd < connect_fd) {
						maxfd = connect_fd;
					}

					if (maxi < i) {
						maxi = i;
					}
					break; 
				}
			}

			if (canSelect == 0) {
				close(connect_fd);//超过限制取消连接
				continue;
			}

			FD_SET(connect_fd, &readfds);//将新的也加入到select监听队列中
			if (--ret <= 0) {
				continue;// 不用继续往下面循环找了
			}
		}

		//检查一下是否为非-1,maxi 刚开始可能还能减少for循环的查询之后就没有用了,可以使用链表的方式
		for (i = 0; i <= maxi; ++i)
		{
			// 如果没有设置并且有读的信号
			if (clientSocket[i] != -1 && FD_ISSET(clientSocket[i], &rfds)) {
				bzero(buf, sizeof(buf));
				int n = Read(clientSocket[i], buf, sizeof(buf));
				if (n == 0) {
					close(clientSocket[i]);
					FD_CLR(clientSocket[i], &readfds);
					clientSocket[i] = -1;
				} else if (n > 0) {
					for (j = 0; j < n; ++j)
					{
						buf[j] = toupper(buf[j]);
					}
					write(clientSocket[i], buf, sizeof(buf));
				}
			}
		}
	}

	return 0;
}