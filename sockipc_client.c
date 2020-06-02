#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define CLIENT_ADDR "client.socket"
#define SERVER_ADDR "server.socket"

#define LISTEN_BACKLOG 50

#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(int argc, char const *argv[])
{
    int cfd;
    struct sockaddr_un my_addr, server_addr;
    char buf[BUFSIZ] = {0};
    char rbuf[BUFSIZ] = {0};

    cfd = socket(AF_UNIX, SOCK_STREAM, 0);

    memset(&my_addr, 0, sizeof(struct sockaddr_un));
    /* Clear structure */
    my_addr.sun_family = AF_UNIX;
    strncpy(my_addr.sun_path, CLIENT_ADDR, sizeof(my_addr.sun_path) - 1);

    unlink(CLIENT_ADDR);

    if (bind(cfd, (struct sockaddr *) &my_addr,
             sizeof(struct sockaddr_un)) == -1)
        handle_error("bind");

    // if (listen(cfd, LISTEN_BACKLOG) == -1)
    //     handle_error("listen");


    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    /* Clear structure */
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SERVER_ADDR, sizeof(server_addr.sun_path) - 1);


    connect(cfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    perror("connect");


    while (fgets(buf, sizeof(buf), stdin) != NULL)
    {
        buf[strlen(buf) - 1] = '\0';
        write(cfd, buf, strlen(buf));
        read(cfd, rbuf, sizeof(rbuf));
        printf("%s\n", rbuf);
    }

    return 0;
}
