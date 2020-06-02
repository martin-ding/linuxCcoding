#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int n = 0, flag = 0;

void parent_handler()
{
    printf("parent pid %u num: %d\n", getpid(), n);
    n = n+2;
    flag = 1;
//    sleep(1);
}

void child_handler()
{
    printf("child pid %u num: %d\n", getpid(),n);
    n = n+2;
    flag = 1;
//    sleep(1);
}
int main()
{
    pid_t pid = fork();
    sigset_t sigset, osigset;//设置屏蔽信号
    sigemptyset(&sigset);

    if (pid == 0) {//子进程
        struct sigaction act, oldact;
        act.sa_flags = 0;
        sigemptyset(&act.sa_mask);
        act.sa_handler = child_handler;
        sigaction(SIGUSR2,&act, &oldact);

        n = 2;
        sigaddset(&sigset, SIGUSR2);//屏蔽SIGUSR2

        while (1) {
//            if (flag == 1) {
//                sigprocmask(SIG_BLOCK, &sigset, &osigset);//真正设置屏蔽
                kill(getppid(), SIGUSR1);//给父进程发送SIGUSR1信号
//                flag = 0;
//                sigprocmask(SIG_SETMASK, &osigset, NULL);//恢复屏蔽
//            }
        }
        sigaction(SIGUSR2, &oldact, NULL);
    } else {//父亲进程
        struct sigaction act, oldact;
        act.sa_flags = 0;
        sigemptyset(&act.sa_mask);
        act.sa_handler = parent_handler;
        sigaction(SIGUSR1,&act, &oldact);

        sigaddset(&sigset, SIGUSR1);//屏蔽SIGUSR1

        sleep(1);//让子进程准备好捕捉函数
        n = 1;

        parent_handler();
        while (1) {
//            if (flag == 1) {
//                sigprocmask(SIG_BLOCK, &sigset, &osigset);//真正设置屏蔽
                kill(pid, SIGUSR2);//给子进程发送SIGUSR2信号
//                flag = 0;
//                sigprocmask(SIG_SETMASK, &osigset, NULL);//恢复屏蔽
                sleep(1);
                // 这个不用管
                if (n > 10000) {
                    kill(pid, SIGKILL);//给子进程发送SIGUSR2信号
                    wait(NULL);
                    break;
                }
//            }
        }

        sigaction(SIGUSR1, &oldact, NULL);
    }
    return 0;
}