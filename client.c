#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>


#define SPORT 6666
#define SIP "127.0.0.1"

int main()
{
	int cfd;
	struct sockaddr_in saddr;
	char buf[BUFSIZ] = {0};
	// char bufr[BUFSIZ] = {0};
	
	// unsigned int ip;
	// unsigned char ip[sizeof(struct in_addr)] = {0};

	cfd = socket(AF_INET, SOCK_STREAM, 0);
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(SPORT);
	inet_pton(AF_INET, SIP, (void *)&saddr.sin_addr.s_addr);

	printf("--------\n");


	connect(cfd, (struct sockaddr *)&saddr, sizeof(saddr));

	while (1) {
		
		fgets(buf, sizeof(buf), stdin);
		
		// buf[strlen(buf)-1] = 0;//fgets 会将\n也记录到buf中

		// write(cfd, buf, strlen(buf)+1);	//strlen不包含\0 我传过去的时候希望传，就加一 
		write(cfd, buf,strlen(buf));
		exit(1);
		// perror("sdasda");

		// memset(bufr, 0, BUFSIZ);
		read(cfd, buf, BUFSIZ);
		
		printf("read %s\n", buf);
	}

	close(cfd);
	return 0;
}