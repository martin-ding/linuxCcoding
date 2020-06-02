#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

void int_handler (int signum)
{
    printf ("int handler %d\n",signum);
}

int main(int argc, char **argv)
{
    char buf[100];
//    struct sigaction act, oldact;
//    act.sa_handler = int_handler;
//    act.sa_flags=0;
//    act.sa_flags |= SA_RESTART;
//    sigemptyset(&act.sa_mask);
//
//    if (-1 == sigaction(SIGINT,&act,&oldact))
//    {
//        printf("sigaction failed!\n");
//        return -1;
//    }

    bzero(buf,100);

    int a = read(STDIN_FILENO,buf,10);//阻塞
    perror("xxx");

    return 0;
}