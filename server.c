#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

typedef struct suser {
    char name[20];
    struct suser *next;
}suser;

typedef struct loginuser {
    suser *head;
    suser *end;
}loginuser;

int insertUser(const char *name, loginuser *link);
int delUser(const char *name, loginuser *link);
loginuser * initLink();

loginuser * initLink()
{
    loginuser *link = (loginuser *) malloc(sizeof(loginuser));
    suser *s = (suser *)malloc(sizeof(suser));
    s->next = NULL;
    s->name[0] = '\0';
    link->head = link->end = s;
    return link;
}

int insertUser(const char *name, loginuser *link)
{
    suser *s = (suser *)malloc(sizeof(suser));
    s->next = NULL;
    strncpy(s->name, name, sizeof(s->name));
    link->end->next = s;
    link->end = s;
    return 0;
}

int delUser(const char *name, loginuser *link)
{
    suser *tmp = link->head;
    while(tmp->next != NULL) {
        if (strcmp(tmp->next->name, name) == 0) {
            suser *p = tmp->next;
            tmp->next = tmp->next->next;
            free(p);
        }
        tmp = tmp->next;
    }
    return 1;
}

int existUser(const char *name, loginuser *link)
{
    suser *tmp = link->head;
    while(tmp->next != NULL) {
        if (strcmp(tmp->next->name, name) == 0) {
            return 0;
        }
        tmp = tmp->next;
    }
    return 1;
}

int main()
{
    char *serverfifo = "serverfifo";
    int readfifo;
    if (access(serverfifo,F_OK) == 0) {
        readfifo = open(serverfifo, O_RDWR|O_NONBLOCK);
    } else {
        readfifo = mkfifo(serverfifo,0666);
        int flag = fcntl(readfifo,F_GETFL);
        fcntl(readfifo, flag|O_NONBLOCK);
    }
    char readbuff[100], prot[4] = {0}, sname[4] = {0},dstname[4] = {0}, content[100] = {0};
    int protocol;

    loginuser *link = initLink();

    while (1) {
        prot[0] = '\0';
        sname[0] = '\0';
        content[0] = '\0';

        int len = read(readfifo,readbuff, sizeof(readbuff));
        // 最少长度是5 4位协议号 4位source 4 dst 其他是content
        if (len > 4) {
            readbuff[len-1] = '\0';//会将\n也保留在readbuff中
            //printf("%s\n", readbuff);

            strncpy(prot,readbuff, 4);
            protocol = atoi(prot);
            switch (protocol) {
                case 1:{//登录
                    strncpy(sname,readbuff+4, 1);
                    insertUser(sname, link);
                    int writefd;
                    if (access(sname,F_OK) == 0) {
                        writefd = open(sname, O_RDWR);
                    } else {
                        writefd = mkfifo(sname,0666);
                    }
                    char info[30] = {0};
                    sprintf(info,"%s login success\n", sname);
                    write(writefd, info,strlen(info));
                    close(writefd);
                    break;
                }
                case 2:{//报告用户未登录
                    if (len < 6) {
                        break;
                    }
                    strncpy(sname,readbuff+4, 1);
                    strncpy(dstname,readbuff+5, 1);// 目的地用户
                    strcpy(content,readbuff+6);
                    int exist = existUser(dstname, link);
                    printf("exist %d\n", exist);
                    if (exist == 0) {//真实存在
                        int writefd;
                        if (access(dstname,F_OK) == 0) {
                            writefd = open(dstname, O_RDWR);
                        } else {
                            writefd = mkfifo(dstname,0666);
                        }
                        write(writefd, sname, strlen(sname));
                        write(writefd, " : ", 3);
                        write(writefd, content,strlen(content));
                        close(writefd);
                    } else {//不存在
                        int writefd;
                        if (access(sname,F_OK) == 0) {
                            writefd = open(sname, O_RDWR);
                        } else {
                            writefd = mkfifo(sname,0666);
                        }
                        char info[30] = {0};
                        sprintf(info,"%s user not login\n", dstname);
                        write(writefd, info,strlen(info));
                        close(writefd);
                    }

                    break;
                }
                case 3:{//通信
                    break;
                }
                case 4:{//退出
                    break;
                }
                default:{
                    printf("%s\n",readbuff);
                }

            }
        }
        sleep(1);
    }
}
