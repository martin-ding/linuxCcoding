#include<event.h>
#include<stdio.h>
#include<unistd.h>
#include <event2/event.h>
#include<pthread.h> //Linux thread
#include<event2/thread.h>
 
struct event_base *base = NULL;
 
 
void pipe_cb(int fd, short events, void *arg)
{
    printf("in the cmd_cb\n");
}
 
void timeout_cb(int fd, short events, void *arg)
{
    printf("in the timeout_cb\n");
}

int pipe_fd[2] = {0};
 
void* thread_fn(void *arg)
{
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    char ch;
    scanf("%c", &ch); //just for wait
 
    struct event *ev = event_new(base, -1, EV_TIMEOUT | EV_PERSIST,
                                 timeout_cb, NULL);
 
    struct timeval tv = {2, 0};
    event_add(ev, &tv);//里面调用了evthread_notify_base()函数
    return NULL;
}
 
int main(int argc, char ** argv)
{
   

    if( argc >= 2 && argv[1][0] == 'y')
        evthread_use_pthreads();//里面调用了evthread_make_base_notifiable() 还设置了 thread_id 函数
 
    base = event_base_new();
    const char *x =  event_base_get_method(base); //获取IO多路复用的模型，linux一般为epoll

    printf("METHOD:%s\n", x);

	evthread_make_base_notifiable(base);
    // printf("%d\n", r);
 
    
    pipe(pipe_fd);
 
    struct event *ev = event_new(base, pipe_fd[0],
                                 EV_READ | EV_PERSIST, pipe_cb, NULL);
 
    event_add(ev, NULL);
 
    pthread_t thread;
    pthread_create(&thread, NULL, thread_fn, NULL);
 
    
    event_base_dispatch(base);
 
    return 0;
}