#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define errhandler(msg) {perror(msg); exit(EXIT_FAILURE);}

int main(int argc, char * argv[]) {
    if (argc != 2) {
        printf("./client name\n");
        exit(1);
    }
    char name[4] = {0};
    strncpy(name, argv[1], 4);
    int i;
    for (i = 0; i < strlen(name); ++i) {
        printf("%x\n",name[i]);
    }

    char *serverfifo = "serverfifo";
    int writefd;
    if (access(serverfifo,F_OK) == 0) {
        writefd = open(serverfifo, O_RDWR|O_NONBLOCK);
    } else {
        writefd = mkfifo(serverfifo,0666);
        int flag = fcntl(writefd,F_GETFL);
        fcntl(writefd, flag|O_NONBLOCK);
    }

    // 标注输入
    int flag = fcntl(STDIN_FILENO,F_GETFL);
    fcntl(STDIN_FILENO, F_SETFL, flag|O_NONBLOCK);

    //从服务端获取数据
    int readfd;
    if (access(name,F_OK) == 0) {
        readfd = open(name, O_RDWR|O_NONBLOCK);
    } else {
        readfd = mkfifo(name,0666);
        int flag = fcntl(readfd,F_GETFL);
        fcntl(readfd, F_SETFL,flag|O_NONBLOCK);
    }
    if (readfd == -1) {
        errhandler("readfd");
    }

    char writebuff[100] = {0}, readbuffer[100] = {0};
    while (1) {
        //从标准输入读取数据写入到服务端
        int wlen = read(STDIN_FILENO, writebuff, sizeof(writebuff));
        if (wlen > 0) {
            write(writefd, writebuff, wlen);
        }

        int rlen = read(readfd, readbuffer, sizeof(readbuffer));
        if (rlen > 0) {
            write(STDOUT_FILENO, readbuffer, rlen);
        }

        sleep(1);
    }

    return 0;
}