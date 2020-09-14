#include<unistd.h>
#include<stdio.h>
#include<event2/event.h>
#include<pthread.h>
#include<event2/thread.h>
 

void cmd_cb(int fd, short events, void *arg)
{
    char buf[1024];
    printf("in the cmd_cb\n");
 
    int n = read(fd, buf, sizeof(buf));
    printf("%s, %d\n", buf, n);
}
 
 

void cmd_cb2(int fd, short events, void *arg)
{
    char buf[1024];
    printf("in the cmd_cb2\n");
 
    int n = read(fd, buf, sizeof(buf));
    printf("%s, %d\n", buf, n);
}
 

int main()
{
    evthread_use_pthreads();
 
    //使用默认的event_base配置
    struct event_base *base = event_base_new();
 	// printf("%d, %d\n", base->event_count_active, base->nactivequeues);
 	// printf("%d\n", (base));

    struct event *cmd_ev = event_new(base, STDIN_FILENO, EV_READ | EV_PERSIST, cmd_cb, NULL);
    
                                    
 
    event_add(cmd_ev, NULL); //没有超时

 	event_assign(cmd_ev,base, STDIN_FILENO, EV_READ | EV_PERSIST, cmd_cb2, NULL);//
    event_base_dispatch(base);
    

    

 
    return 0;
}
