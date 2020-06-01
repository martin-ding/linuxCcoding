#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<strings.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<net/if.h>

#define SERVER_PORT 8000
#define CLIENT_PORT 9000

#define GROUP "239.0.0.2"

int main(int argc,char* argv[])
{
    struct sockaddr_in localaddr;
    int confd;
    ssize_t len;
    char buf[BUFSIZ];

    struct ip_mreqn group;
    
    //struct ip_mreqn {
    //    struct in_addr  imr_multiaddr;          /* IP multicast address of group */
    //    struct in_addr  imr_address;            /* local IP address of interface */
    //    int             imr_ifindex;            /* Interface index */
    //};  

    confd = socket(AF_INET,SOCK_DGRAM,0);

    bzero(&localaddr,sizeof(localaddr));
    localaddr.sin_family = AF_INET;
    inet_pton(AF_INET,"0.0.0.0",&localaddr.sin_addr.s_addr);
    localaddr.sin_port = htons(CLIENT_PORT);

    bind(confd,(struct sockaddr*)&localaddr,sizeof(localaddr));

    //加入组播组
    inet_pton(AF_INET,GROUP,&group.imr_multiaddr);
    inet_pton(AF_INET,"0.0.0.0",&group.imr_address);
    group.imr_ifindex = if_nametoindex("eth0");

    setsockopt(confd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&group,sizeof(group));

    while(1)
    {
        len = recvfrom(confd,buf,sizeof(buf),0,NULL,0);
        write(STDOUT_FILENO,buf,len);
    }
    close(confd);
    return 0;
}