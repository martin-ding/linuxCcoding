#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>

//回收子进程
void recycleChild()
{
    int status;
    // 有进程被回收
    if (waitpid(0, &status, WNOHANG) != -1) {
        if (WIFEXITED(status)) {
            printf("exited, status=%d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("killed by signal %d\n", WTERMSIG(status));
        }
    } else {
        perror("子进程回收失败");
        exit(1);
    }
}

int main ()
{
    int n = 10, i;
    pid_t pid;
    for (i = 0; i< n; i++) {
        pid = fork();
        if (pid == 0) {//子进程
            break;
        }
    }

    if (pid == 0) {
        printf("child %u\n", getpid());
        return i+1;
    } else {
        // 对SIGCHLD进行阻塞
        struct sigaction act, oldact;
        sigemptyset(&act.sa_mask);//不屏蔽任何信号
        act.sa_flags = 0;
        act.sa_handler = recycleChild;
        // 对SIGCHLD解除阻塞
        sigaction(SIGCHLD, &act, &oldact);
        while (1) {
            printf("parent printing ....\n");
            sleep(1);
        }
    }

    return 0;
};