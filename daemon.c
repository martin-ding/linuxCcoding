#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define errhand(msg){perror();exit(EXIT_FAILURE);}
int main()
{
    //1.创建一个子进程
    pid_t pid = fork();
    if (pid > 0) {
        return 0;
    }

    //2.创建一个新会话
    setsid();
    //3.设置工作目录
    chdir("/");

    //4.设置文件掩码
    umask(0002);//8进制数

    //5.关闭或者重定向0，1，2文件描述符
    close(STDIN_FILENO);//关闭输入
    int fd = open("/dev/null", O_RDWR);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);

    // 6.设置该干的事情
    while(1) {

    }
    //7.如果要退出一般设置信号捕捉

}