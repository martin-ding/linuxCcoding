#include <stdlib.h>
#include <stdio.h>

#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include "wrap.h"
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#define PORT 6666
#define EPOLLSIZE 20

int main() 
{
	struct sockaddr_in addr, con_addr;
	int epnum, i, j, readn;
	char buf[4] = {0};

	socklen_t addrlen;

	int sfd = Socket(AF_INET, SOCK_STREAM, 0);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int opt = 1;
	setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));//设置地址重用

	Bind(sfd, (struct sockaddr *) &addr, sizeof(addr));

	Listen(sfd, 128);

	//epoll 
	int epfd = epoll_create(EPOLLSIZE);
	struct epoll_event evt, evts[EPOLLSIZE];//地址
	evt.events = EPOLLIN | EPOLLET;//edge trigger
	evt.data.fd = sfd;

    epoll_ctl(epfd, EPOLL_CTL_ADD, sfd, &evt);

    while (1) {
    	epnum = epoll_wait(epfd, evts, EPOLLSIZE, -1);//永远阻塞
    	if (epnum <= 0) {//不可能出现0的情况
    		perror("epoll_wait error");
    		exit(1);
    	}

    	//对evts数组进行遍历
    	for (i = 0; i < epnum; ++i)
    	{
    		// 如果有读操作，这里只关心读操作
    		if (evts[i].events & EPOLLIN) {
    			if (evts[i].data.fd == sfd) {//服务器socket fd 上面有请求
    				addrlen = sizeof(con_addr);

    				int confd = Accept(sfd, (struct sockaddr *)&con_addr, &addrlen);
    				//设置为nonblocking 
    				int flag = fcntl(confd, F_GETFL);
    				fcntl(confd, F_SETFL, flag | O_NONBLOCK);
    				char ip[INET_ADDRSTRLEN] = {0};
    				printf("connect port %d, ip %s \n", ntohs(con_addr.sin_port), inet_ntop(AF_INET, &con_addr.sin_addr.s_addr, ip, INET_ADDRSTRLEN));
    				//将confd 加入到 epoll中
    				struct epoll_event evtcon;//地址
					evtcon.events = EPOLLIN | EPOLLET;//edge trigger
					evtcon.data.fd = confd;

					epoll_ctl(epfd, EPOLL_CTL_ADD, confd, &evtcon);
    			} else {//客户端的读
    				while((readn = Read(evts[i].data.fd, buf, sizeof(buf))) > 0) {
    					for (j = 0; j < readn; ++j)
    					{
    						buf[j] = toupper(buf[j]);
    					}
    					sleep(1);
    					Write(evts[i].data.fd, buf, readn);
    				}

    				//要么关闭要么出错
    				if (readn <= 0) {
    					if (errno == EAGAIN || errno == EWOULDBLOCK) {
    						continue;
    					}
    					epoll_ctl(epfd, EPOLL_CTL_DEL, evts[i].data.fd, NULL);
    					close(evts[i].data.fd);
    				}
    			}
    		}
    	}
    }

	close(epfd);

}