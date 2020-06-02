#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include <unistd.h>

void hello()
{
    printf("-------\n");
}

int main()
{
    signal(SIGUSR1,hello);
    int r = wait(NULL);//等待子进程回收
//    sleep(30);
    if (r == -  1) {
        if (errno == ECHILD) {
            puts("------BB-----");
        } else {
            puts("-------SS------");
        }
    }
}