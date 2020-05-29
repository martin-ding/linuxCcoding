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
#include <signal.h>

#include <sys/types.h>
#include <sys/wait.h>


#define PORT 6666
#define BACKLOG 1	

//回收子进程
void recycle_child(int signum)
{
	int status;
	pid_t ret;
	//这里直接一个while循环即可
	while ((ret = waitpid(0, &status, WNOHANG)) > 0) {
       if (WIFEXITED(status)) {
           printf("%u exited, status=%d\n", ret, WEXITSTATUS(status));
       } else if (WIFSIGNALED(status)) {
           printf("killed by signal %d\n", WTERMSIG(status));
       }
    }
}

int main()
{
	int sfd, cfd;
	struct sockaddr_in saddr, caddr;
	socklen_t addrlen;
	char buf[BUFSIZ] = {0};
	int n, i;
	pid_t pid;
	int ret;


	//注册捕捉函数
	struct sigaction act, oldact;
	bzero(&act, sizeof(act));
	bzero(&oldact, sizeof(oldact));

	act.sa_handler = recycle_child;
	sigemptyset(&act.sa_mask);
	// act.sa_flags = SA_RESTART;

	
	ret = sigaction(SIGCHLD, &act, &oldact);
	if (ret == -1) {
		perror("sigaction error");
		exit(EXIT_FAILURE);
	}

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

		char ip[INET_ADDRSTRLEN] = {};
		inet_ntop(AF_INET, &(caddr.sin_addr.s_addr), ip, INET_ADDRSTRLEN);

		printf("client address %d,  %s \n", ntohs(caddr.sin_port), ip);

		
		pid = fork();
		if (pid == -1) {
			perror("fork error");
			exit(EXIT_FAILURE);
		} else if (pid == 0) {
			close(sfd);
			sigaction(SIGCHLD, &oldact, NULL);//恢复默认SIGCHILD的捕捉函数
			break;// 子进程下面处理
		} else {// 父进程
			close(cfd);
		}
	}

	//子进程
	if (pid == 0) {
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
	}

	return 0;
}