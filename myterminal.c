#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define MAXARG 8
#define MAX 1024
#define MAXCMD 16

//存储命令
typedef struct commad {
    char *argv[MAXARG];//存储命令 ls -al -F -R >
    char *in, *out;//输入输出
} cmd_t;

int parse(char *buf,cmd_t *cmdp);
void show(cmd_t *cmdp, int cmdnum);

//解析参数
int parse(char *buf,cmd_t *cmdp)
{
    char *tmp = buf, *curcmd;
    int cmdnum = 0, i = 0, k = 0;

    if ((cmdp[cmdnum].argv[0] = strtok(tmp, "|"))) {
        //puts(cmdp[cmdnum].argv[0]);
        cmdnum++;
    }

    while ((cmdp[cmdnum].argv[0] = strtok(NULL, "|"))) {
        cmdnum++;
    }

    for (i = 0; i < cmdnum; i++) {
        while (*cmdp[i].argv[0] == ' ') {
            cmdp[i].argv[0]++;
            continue;
        }

        k = 0;
        curcmd = cmdp[i].argv[0];// 全命令
        cmdp[i].in = cmdp[i].out = NULL;

        while (*curcmd) {
            printf("%c \n", *curcmd);
            if (*curcmd == ' ') {
                *curcmd++ = '\0';
                continue;
            }

            if (*curcmd == '<') {
                *curcmd = '\0';
                while (*(++curcmd) == ' ');
                cmdp[i].in = curcmd;
                if (*curcmd == '\0')
                    return -1;
                continue;
            }

            if (*curcmd == '>') {
                *curcmd = '\0';
                while (*(++curcmd) == ' ');
                cmdp[i].in = curcmd;
                if (*curcmd == '\0')
                    return -1;
                continue;
            }
//            printf("%X， %X\n", curcmd , cmdp[i].argv[0]);

            if (curcmd != cmdp[i].argv[0] && *curcmd != ' ' && *(curcmd - 1) == '\0') {
                cmdp[i].argv[++k] = curcmd++;
                continue;
            }
            curcmd++;
        }
        //如果没输入结束
        if (*curcmd == '\0') {
            return -1;
        }

        cmdp[i].argv[k] = NULL;
        k = 0;
    }
    return cmdnum;
}

void show(cmd_t *cmdp, int cmdnum)
{
    int i,j;
    for (i = 0;  i < cmdnum; i++) {
        printf("cmd-show:%s,",cmdp[i].argv[0]);
    }
    for (j = 0; cmdp[i].argv[j]; ++j) {
        printf("argv:%s,", cmdp[i].argv[j]);
    }
    printf("in:%s, out:%s\n", cmdp[i].in, cmdp[i].out);
}

int main(int argc, char * argv[])
{

    char buf[MAX] = {0};//命令获取
    int cmdnum;
    cmd_t cmd[MAXCMD];

    while(1) {
        printf("sh%%");
        fflush(stdout);
        fgets(buf, MAX, stdin);//从标准输出中等待用户写数据

        if (buf[strlen(buf) - 1] == '\n') {
            buf[strlen(buf) - 1] = '\0';
        }
        if (strcasecmp(buf, "exit") == 0 || strcasecmp(buf, "EXIT") == 0 || strcasecmp(buf, "quit") == 0 || strcasecmp(buf, "bye") == 0) {
            break;
        }
        cmdnum = parse(buf, cmd);
        show(cmd, cmdnum);
        execlp(cmd[0].argv[0],cmd[0].argv[1], cmd[0].argv[2],NULL);
    }

    return 0;
}