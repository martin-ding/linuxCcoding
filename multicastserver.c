#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<strings.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<net/if.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_PORT 8000
#define MAXLINE 1024
#define CLIENT_PORT 9000

#define GROUP "239.0.0.2"//这个是一个随机的组播地址

int main(int argc,char* argv[])
{
    int sockfd;
    struct sockaddr_in servaddr,clieaddr;
    char buf[MAXLINE];
    // struct ip_mreqn group;
    //构造用于UDP通信的套接字
    sockfd = socket(AF_INET,SOCK_DGRAM,0);

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERVER_PORT);

    bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));

    // inet_pton(AF_INET,GROUP,&group.imr_multiaddr);//设置组地址
    // inet_pton(AF_INET,"0.0.0.0",&group.imr_address);//本地有效IP
    // group.imr_ifindex = if_nametoindex("eth0");//给出网卡名,转换为对应编号
    // setsockopt(sockfd,IPPROTO_IP,IP_MULTICAST_IF,&group,sizeof(group));//设置组播权限

    struct in_addr mi;
    /**
    **** /usr/include/linux/in.h +84
    #if __UAPI_DEF_IN_ADDR //Internet address. 

    struct in_addr {
        __be32  s_addr;
    };
    */
    //#endif
    inet_aton(GROUP,&mi);

    setsockopt(sockfd,IPPROTO_IP,IP_MULTICAST_IF,&mi,sizeof(mi));//UNP中说明是指定外出接口，指定一个组播IP即可

    bzero(&clieaddr,sizeof(clieaddr));
    clieaddr.sin_family = AF_INET;
    inet_pton(AF_INET,GROUP,&clieaddr.sin_addr.s_addr);
    clieaddr.sin_port = htons(CLIENT_PORT);

    int i = 1;
    while(1)
    {
        sprintf(buf,"服务端组播了 %d次\n",i++);
        //对这个组的某个端口进行组播
        sendto(sockfd,buf,strlen(buf),0,(struct sockaddr*)&clieaddr,sizeof(clieaddr));
        sleep(1);
    }
    close(sockfd);
    return 0;
}