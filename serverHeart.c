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
#include <string.h>
#include <pthread.h>

// 服务器端：
#define BACKLOG 128	

#define PORT 6666

//这里只是简单的使用一个链表来记录客户端的状态，可以使用平衡树，类似于epoll模型的红黑树，减少查询次数
//多个线程操作的时候最好加一个锁
typedef struct session {
    char peerip[16];
    char name[10];
    int sockfd;
    int count;
    struct session *next;
} s_t;

s_t *shead = NULL;


//这个就类似于一个协议
typedef struct package
{
	int data_type;//数据包的类型
	char name[10];//对应的客户端的名称
} package;

#define HEART 1

/*
**功能：处理用户心跳包事件，将其会话记录中的计数器清零
**参数：套接字和数据包指针
**返回值：无
*/
void heart_handler(package *pd)
{
    s_t *cur = shead->next;  // shead为用户在线会话记录全局变量头指针
    while( NULL != cur)
    {
        if(strcmp(cur->name, pd->name) == 0)
        {
        	//最好加一个排它锁
            cur->count = 0;  //将计数器清零，表明用户名为pd->name的客户端还活着
            printf("客户端IP： %s ：用户 %s 连接正常\n", cur->peerip, pd->name);
        }
        cur = cur->next;
    }
}

/*
**功能：心跳检测处理函数
**参数：无
**返回值：无
*/
void check_handler()
{
    s_t *temp = NULL;  // 用于释放结点
    s_t *ppNode = shead->next;
    s_t *pre = shead;//用于删除专用

    while(NULL != ppNode)
    {
    	printf("%d==\n", ppNode->count);
        if(ppNode->count == 5) {
            printf("客户端IP： %s ：用户 %s 已经掉线！！\n", ppNode->peerip, ppNode->name);
            close(ppNode->sockfd); //关闭对端套接字
            temp = ppNode; //存储本结点地址
            ppNode = ppNode->next; // 下一个成员指针
            pre->next = ppNode;

            free(temp);  //释放结点
            temp = NULL;
        } else if(ppNode->count > 0) {
            printf("客户端IP： %s ：用户 %s 连接异常！\n", ppNode->peerip, ppNode->name);
            ppNode->count++;
            printf("count = %d\n", ppNode->count); //查看计数器内容
            ppNode = ppNode->next; // 下一个成员指针
           
        }else if(ppNode->count == 0) {
            ppNode->count++;
            printf("count = %d\n", ppNode->count); //查看计数器内容
            ppNode = ppNode->next; // 下一个成员指针
        }
    }
}


/*
**功能：心跳检测线程函数
**参数：无
**返回值：无
*/
void *heart_check(void *p)
{
    printf("心跳检测线程已开启！\n");

    while(1)
    {
    	printf("------心跳检测轮回------\n");
        check_handler(); // 心跳检测处理函数
        sleep(2); //定时2秒
    }
    return NULL;
}


//初始socket 只返回一个client sfd
int initsocket()
{
	int sfd, cfd;
	struct sockaddr_in saddr, caddr;
	socklen_t addrlen;

	sfd = socket(AF_INET, SOCK_STREAM, 0);
	int opt = 1;
	setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));//设置地址重用

	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(PORT);//port占16位一共可以有65535 个
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(sfd, (struct sockaddr *)&saddr, sizeof(saddr));

	listen(sfd, BACKLOG);

	addrlen = sizeof(caddr);
	cfd = accept(sfd, (struct sockaddr *) &caddr, &addrlen);//addrlen传入传输参数，阻塞等待连接

	char ip[INET_ADDRSTRLEN] = {};
	inet_ntop(AF_INET, &(caddr.sin_addr.s_addr), ip, INET_ADDRSTRLEN);

	printf("client address %d,  %s \n", ntohs(caddr.sin_port), ip);

	//将客户端插入到链表中
	s_t *node = (s_t *) malloc(sizeof(s_t));
	memcpy(node->peerip, ip, strlen(ip));
	strcpy(node->name, "client1");//这个要么是一个随机数要么是时间轴这样唯一确认，我这边写死了
	node->sockfd = cfd;
	node->count = 0;

	node->next = shead->next;
	shead->next = node;
	printf("xxxxxx%s   %s \n", shead->next->peerip, ip);

	return cfd;
}

int main()
{
	shead = (s_t*)malloc(sizeof(s_t));
	shead->next = NULL;//头
	
	
	char buf[BUFSIZ] = {0};
	int readn;
	int cfd = initsocket();

	pthread_t tid;

	//子线程专门负责判断心跳
	pthread_create(&tid, NULL, heart_check, NULL);

	while (1) {
		readn = read(cfd, buf, BUFSIZ);
		printf("%d---\n", readn);
		if (readn <= 0) {//有异常，或者对端socket关闭要将本端的socket也关闭
			//TODO 从链表中删除结点
			//close(cfd);
		}

		//如果收到的是一个心跳包、、、、、
		if (readn == sizeof(package)) {
			package *d = (package *) buf;
			if (d->data_type == HEART) {
				heart_handler(d);
			}
		} else {
			printf("string: %s\n", buf);
		}
	}
}