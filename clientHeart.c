#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <pthread.h>


typedef struct package
{
	int data_type;//数据包的类型
	char name[10];//对应的客户端的名称
} package;


#define HEART 1
#define SPORT 6666
#define SIP "127.0.0.1"

/*
**功能：客户端心跳包发送线程函数
**参数：线程传参（可传套接字）
**返回值：空
*/

int cfd;


void *send_heart(void *addr)
{
	package *pd = (package *)malloc(sizeof(package));
    while(1)
    {
        pd->data_type = HEART;  //HEART:数据包类型,pd为数据包结构体指针
        strcpy(pd->name,"client1");
        write(cfd, pd, sizeof(package));
        sleep(7); //定时3秒
    }
    return NULL;
}

void initsocket()
{
	struct sockaddr_in saddr;

	cfd = socket(AF_INET, SOCK_STREAM, 0);
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(SPORT);
	inet_pton(AF_INET, SIP, (void *)&saddr.sin_addr.s_addr);

	printf("--------\n");

	connect(cfd, (struct sockaddr *)&saddr, sizeof(saddr));
}

int main() {
	initsocket();
	pthread_t tid;
	//连接socket
	pthread_create(&tid, NULL, send_heart, NULL);

	while (1) {
		sleep(2);
		write(cfd, "hello world", strlen("hello world"));//模拟客户端干别的事情
		// printf(" client ........\n");
	}
}

