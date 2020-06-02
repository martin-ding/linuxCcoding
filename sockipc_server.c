#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>



#define SERVER_ADDR  "server.socket"

#define LISTEN_BACKLOG 50

#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(int argc, char *argv[])
{
    int sfd, cfd;
    struct sockaddr_un my_addr, peer_addr;
    socklen_t peer_addr_size;
    char buf[BUFSIZ];

    sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1)
        handle_error("socket");

    memset(&my_addr, 0, sizeof(struct sockaddr_un));
    /* Clear structure */
    my_addr.sun_family = AF_UNIX;
    strncpy(my_addr.sun_path, SERVER_ADDR, sizeof(my_addr.sun_path) - 1);

    unlink(SERVER_ADDR);

    if (bind(sfd, (struct sockaddr *) &my_addr,
             sizeof(struct sockaddr_un)) == -1)
        handle_error("bind");


    if (listen(sfd, LISTEN_BACKLOG) == -1)
        handle_error("listen");

    /* Now we can accept incoming connections one
       at a time using accept(2) */

    peer_addr_size = sizeof(struct sockaddr_un);
    cfd = accept(sfd, (struct sockaddr *) &peer_addr,
                 &peer_addr_size);
    if (cfd == -1)
        handle_error("accept");

    printf("####%s\n", peer_addr.sun_path);

    int n, i ;
    while (1)
    {
        n = read(cfd, buf, BUFSIZ);
        for (i = 0; i < n; ++i)
        {
            buf[i] = toupper(buf[i]);
            /* code */
        }

        write(cfd, buf, n);//此时已经关闭了
        printf("--------\n");
    }

    /* Code to deal with incoming connection(s)... */

    /* When no longer required, the socket pathname, MY_SOCK_PATH
       should be deleted using unlink(2) or remove(3) */


}