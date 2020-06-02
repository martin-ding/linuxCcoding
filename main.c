#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

void as(){
}

unsigned int mysleep(unsigned int seconds) {
    unsigned int ret = 0;

    //1. 捕捉信号 回调函数，要先于alarm否则如果alarm函数调用之后，CPU被占用 再回来就是默认的信号处理方式
    struct sigaction act, oldact;
    act.sa_handler = as;
    act.sa_flags = 0;//默认情况下，会在函数执行期间屏蔽相同信号
    sigemptyset(&act.sa_mask);
//    sigaddset(&sigset,SIGQUIT);//运行期间屏蔽掉3号信号
    int res = sigaction(SIGALRM, &act, &oldact);
    if (res == -1) {
        perror("err");
        exit(1);
    }

    //2. 屏蔽信号ALARM
    sigset_t sigset, oldsigset, sgsigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGALRM);
    sigprocmask(SIG_BLOCK, &sigset, &oldsigset);

    //3 .倒计时
    alarm(seconds);
    //期间CPU切换就不怕了，因为及时真的停了seconds之后信号还是没有被处理
//    int a = pause();
//    if (a == -1 && errno == EINTR)//成功
//        printf("pause success\n");
    // 调用阻塞等待信号来临，如果之前CPU调度，并且信号已经到了，
    // 此时屏蔽状态，所以不会处理，所以转而执行程序，此时程序按照sgsigset 屏蔽字执行等待

    //    sigfillset(&sgsigset);
    //    sigdelset(&sgsigset, SIGALRM);
    //    int a = sigsuspend(&sigdelset);

    //4.恢复捕捉SIGALRM，并且让sigsuspend能检测到，并且调用响应捕捉函数
    // 老师的代码从oldsigset中去掉了SIGALRM 不会影响其他信号的屏蔽状态
    sgsigset = oldsigset;
    sigdelset(&sgsigset, SIGALRM);
    int b = sigsuspend(&sigdelset);

    //5.恢复之前的信号屏蔽字
    sigprocmask(SIG_BLOCK, &oldsigset, NULL);

    //6.防止如果期间描述还没到已经收到了SIGALRM，被sigsuspend 处理之后，
    // 或者其他信号到来的情况，将秒数清空
    ret = alarm(0);//当你的进程  中途收到了一个信号pause已经返回-1了，此时的倒计时没有用到需要清除，为了程序的健壮考虑

    //7.恢复默认的信号的处理
    sigaction(SIGALRM, &oldact, NULL);//不影响正常的系统
    return ret;
}



int main() {



//    while (1) {
//        int a = mysleep(4);
//        printf("ww-----www %d\n", a);
//    }

    pause();
    printf("---------\n");
}