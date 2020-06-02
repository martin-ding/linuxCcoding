#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    // bash 1675（父）  1677  1677  1677
    printf("pid %u\n", getpid());//2889
    printf("group id %u\n", getpgid(0));//2889
    printf("session id %u\n", getsid(0));//1677
    pid_t pid = fork();
    if (pid == 0) {
        printf("child pid %u\n", getpid());//2890
        printf("child group id %u\n", getpgid(0));//2889 父进程id

        printf("child session id %u\n", getsid(0));//2889 父进程id

        sleep(1);
        setsid();

        printf("child pid %u\n", getpid());//2890
        printf("child group id %u\n", getpgid(0));//2890
        printf("child session id %u\n", getsid(0));//2890
    }
    return 0;
}