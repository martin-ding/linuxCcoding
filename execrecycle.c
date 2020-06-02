#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <errno.h>

#define errh(msg) {perror(msg);exit(EXIT_FAILURE);}

void recycleChild()
{
    int status;
    //状态改变就会调用
    while(waitpid(0, &status, 0) > 0) {
        if (errno == EINTR) {
            printf("---------");
        }

        if (WIFEXITED(status)) {
            printf("child exit status %d\n", WEXITSTATUS(status));
        }
        if (WIFSIGNALED(status)) {
            printf("child kill by signal %d\n", WTERMSIG(status));
        }
    }
}

void sighand(int signum, siginfo_t *siginfo, void * context)
{
    printf("-----%d \n", siginfo->si_value.sival_int);
}

int main()
{
    pid_t pid = fork();
    if (pid == 0) {
        sigval_t sigvalu;
        sigvalu.sival_int = 199;
        int m = sigqueue(getppid(), SIGCHLD, sigvalu);//给父进程发送SIGCHLD信号
        if (m == -1) {
            errh("eeeee");
        }

//        printf("child----------\n");
    } else {//父进程
        //屏蔽信号
        sigset_t sigset, osigset;
        sigemptyset(&sigset);
        sigaddset(&sigset, SIGCHLD);
        sigprocmask(SIG_BLOCK, &sigset, &osigset);

        //设置捕捉函数
        struct sigaction act, oldact;
        sigemptyset(&act.sa_mask);
        act.sa_flags = SA_SIGINFO;
        act.sa_sigaction = sighand;
        int r = sigaction(SIGCHLD, &act,&oldact);
        if (r == -1) {
            errh("sig");
        }

        //解除屏蔽
        sigprocmask(SIG_SETMASK, &osigset, NULL);

        sleep(100);//留时间处理子进程回收
    }
    return 0;
}