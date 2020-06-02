#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include <unistd.h>

void parent() {
    printf("----parent------\n");
}

void child() {
    printf("-----child-----\n");
}

int main()
{
    pid_t pid = fork();
    if (pid == 0) {
        //子进程屏蔽SIG
        printf("child\n");
        sleep(100);
    } else {
        struct sigaction act;

        act.sa_flags = SA_RESTART;
        act.sa_handler = parent;
        sigemptyset(&act.sa_mask);
        int a = sigaction(SIGINT, &act, NULL);

        int r  = (NULL);
    }
}