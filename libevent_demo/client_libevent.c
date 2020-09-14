#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{


    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    perror("sfd");
    struct sockaddr_in *addr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
    addr->sin_family = AF_INET;
    addr->sin_port = htons(6666);
    addr->sin_addr.s_addr = htonl(INADDR_ANY);

    connect(sfd, (struct sockaddr *) addr, sizeof(struct sockaddr_in));
    perror("connect");
    while (1) {
	    write(sfd, "abcde\0fefad\n", 12);//其中包含\0
	   
	    perror("write");
	   	sleep(1);

	    char buf[BUFSIZ] = {0};
	    int n = read(sfd, buf, sizeof(buf));
	    int i;
	    for (i = 0; i < n; ++i)
	    {
	        printf("%c ", buf[i]);
	    }


    }

     close(sfd);

    return 0;
}