#include <stdio.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_EVENTS  1024
#define SERVER_PORT 8888

struct my_events {
  
    int        m_fd;                                         //监听的文件描述符
    int        m_event;                                      //监听的事件
    int        m_status;                                     //是否在红黑树上, 1->在, 0->不在
    time_t     m_lasttime;                                   //最后放入红黑树的时间  
    void       *m_arg;                                       //泛型参数,这里的用法指向本结构体
    void       (*call_back)(int fd, int event, void *arg);   //回调函数
    char       m_buf[BUFSIZ];
    int        m_buf_len;
};


int                    ep_fd;                                //红黑树根
struct my_events       ep_events[MAX_EVENTS];   


/* 函数原型 */

/*初始化监听socket*/
void initlistensocket(int ep_fd, unsigned short port);
/*将结构体成员变量初始化*/
void eventsetinit(struct my_events *my_ev, int fd, void (*call_back)(int fd, int event, void *arg), void *event_arg);
/*向红黑树添加 文件描述符和对应的结构体*/
void eventadd(int ep_fd, int event, struct my_events *my_ev);
/*从红黑树上删除 文件描述符和对应的结构体*/
void eventdel(int ep_fd, struct my_events *ev);
/*发送数据*/
void senddata(int client_fd, int event, void *arg);
/*接收数据*/
void recvdata(int client_fd, int event, void *arg);
/*回调函数: 接收连接*/
void acceptconnect(int listen_fd, int event, void *arg);


int main(void)
{
   unsigned short port = SERVER_PORT;

   ep_fd = epoll_create(MAX_EVENTS);                         //创建红黑树,返回给全局变量ep_fd;
   if (ep_fd <= 0)
      printf("create ep_fd in %s error: %s \n", __func__, strerror(errno));
  
   /*初始化监听socket*/
   initlistensocket(ep_fd, port);

   int checkpos = 0;
   int i;
   struct epoll_event events[MAX_EVENTS];
   while (1)
   {
      /*超时验证,每次100个,60s内没有和服务器通信则关闭客户端连接*/
      long now = time(NULL);            //当前时间
      for (i=0; i<100; i++,checkpos++)
      {
         if (checkpos == MAX_EVENTS-1)
             checkpos = 0;
         if (ep_events[checkpos].m_status != 1)//不在树上
             continue;

         long spell_time = now - ep_events[checkpos].m_lasttime;       //客户端不活跃的时间
         if (spell_time >= 60)
         {
             printf("[fd= %d] timeout \n", ep_events[checkpos].m_fd);  
             // close(ep_events[i].m_fd);
             // eventdel(ep_fd, &ep_events[i]);
             close(ep_events[checkpos].m_fd);
             eventdel(ep_fd, &ep_events[checkpos]);
         }
        
      }
      /*监听红黑树,将满足条件的文件描述符加至ep_events数组*/ 
      int n_ready = epoll_wait(ep_fd, events, MAX_EVENTS, 1000); //1秒没事件满足则返回0
      if (n_ready < 0)
      {
          printf("epoll_wait error, exit \n");
          break;
      }
     // if (n_ready == 0)
     //     printf("\n n_ready == 0 \n");      

      for (i=0; i<n_ready; i++)
      {
           struct my_events *ev = (struct my_events *)events[i].data.ptr;
           if ((events[i].events & EPOLLIN) && (ev->m_event & EPOLLIN))  //读就绪事件
               ev->call_back(ev->m_fd, events[i].events, ev->m_arg);
           if ((events[i].events & EPOLLOUT) && (ev->m_event & EPOLLOUT)) //写就绪事件
               ev->call_back(ev->m_fd, events[i].events, ev->m_arg);
      }
   }

   return 0;
}     


/*初始化监听socket*/
void initlistensocket(int ep_fd, unsigned short port)
{
   int                  listen_fd;
   struct sockaddr_in   listen_socket_addr;
 
   printf("\n initlistensocket() \n");  
 
  
  /*申请一个socket*/
   listen_fd = socket(AF_INET, SOCK_STREAM, 0);
   fcntl(listen_fd, F_SETFL, O_NONBLOCK);                     //将socket设置为非阻塞模式,好处自行百度
   
   int opt = 1;
   setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));      //端口复用


   /*绑定前初始化*/
   bzero(&listen_socket_addr, sizeof(listen_socket_addr));
   listen_socket_addr.sin_family      = AF_INET;
   listen_socket_addr.sin_port        = htons(port);
   listen_socket_addr.sin_addr.s_addr = htonl(INADDR_ANY);
   /*绑定*/
   bind(listen_fd, (struct sockaddr *)&listen_socket_addr, sizeof(listen_socket_addr));
   /*设置监听上限*/
   listen(listen_fd, 128);

   /*将listen_fd初始化*/
   eventsetinit(&ep_events[MAX_EVENTS-1], listen_fd, acceptconnect, &ep_events[MAX_EVENTS-1]);    
   /*将listen_fd挂上红黑树*/
   eventadd(ep_fd, EPOLLIN, &ep_events[MAX_EVENTS-1]);

   return ;
}


/*将结构体成员变量初始化*/
void eventsetinit(struct my_events *my_ev, int fd, void (*call_back)(int fd, int event, void *arg), void *event_arg)
{
   my_ev->m_fd       = fd;
   my_ev->m_event    = 0;
   my_ev->m_status   = 0;
   my_ev->m_lasttime = time(NULL);//设置一下开始的时间，用于淘汰机制
   my_ev->m_arg      = event_arg;
   my_ev->call_back  = call_back;

   return ;
}
/*向红黑树添加 文件描述符和对应的结构体*/
void eventadd(int ep_fd, int event, struct my_events *my_ev)
{
  int op;
  struct epoll_event epv;
  epv.data.ptr = my_ev;
  epv.events   = my_ev->m_event = event;
  
  if (my_ev->m_status == 0)
  {
      op = EPOLL_CTL_ADD;
  }
  else
  {
    printf("\n add error: already on tree \n");
    return ;
  }

  if (epoll_ctl(ep_fd, op, my_ev->m_fd, &epv) < 0)
  {
     printf("\n event add false [fd= %d] [events= %d] \n", my_ev->m_fd, my_ev->m_event);
  }
  else
  {
     my_ev->m_status = 1;
     printf("\n event add ok [fd= %d] [events= %d] \n", my_ev->m_fd, my_ev->m_event);
  }

  return ;
}
/*从红黑树上删除 文件描述符和对应的结构体*/
void eventdel(int ep_fd, struct my_events *ev)
{
  if(ev->m_status != 1)
     return ;

  epoll_ctl(ep_fd, EPOLL_CTL_DEL, ev->m_fd, NULL);
  ev->m_status = 0;
 
  return ;
}



/*回调函数: 接收连接*/
void acceptconnect(int listen_fd, int event, void *arg)
{
  int                 connect_fd;
  int                 i;
  int                 flag=0;
  char                str[BUFSIZ];
  struct sockaddr_in  connect_socket_addr;
  socklen_t           connect_socket_len;

  if ( (connect_fd=accept(listen_fd, (struct sockaddr *)&connect_socket_addr, &connect_socket_len)) <0 )
  {
     if (errno != EAGAIN && errno != EINTR)
        {/*暂时不处理*/}
     printf("\n %s: accept, %s \n", __func__, strerror(errno));
     return ;
  }

  do
  {
    for(i=0; i<MAX_EVENTS; i++)
        if(ep_events[i].m_status == 0)
           break;
    if(i >= MAX_EVENTS)
     {
        printf("\n %s : max connect [%d] \n", __func__, MAX_EVENTS);
        break;
     }      
    
	/* 设置非阻塞 */
    if((flag = fcntl(connect_fd, F_SETFL, O_NONBLOCK)) <0)
    {
       printf("\n %s: fcntl nonblocking false, %s \n", __func__, strerror(errno));
       break;
    }

    eventsetinit(&ep_events[i], connect_fd, recvdata, &ep_events[i]);
    eventadd(ep_fd, EPOLLIN, &ep_events[i]);

  }while(0);

   printf("\n new connection [%s:%d]  [time:%ld]  [pos:%d] \n", inet_ntop(AF_INET, &connect_socket_addr.sin_addr, str, sizeof(str)), 
                                ntohs(connect_socket_addr.sin_port), ep_events[i].m_lasttime, i);
   return ;
}

/*接收数据*/
void recvdata(int client_fd, int event, void *arg)
{
  int              len;
  struct my_events *ev = (struct my_events *)arg;

  len = recv(client_fd, ev->m_buf, sizeof(ev->m_buf), 0);
  //从红黑树拿下
  eventdel(ep_fd, ev);                                      

  if (len >0)
  {
      ev->m_buf_len      = len;
      ev->m_buf[len] = '\0';           //手动添加结束标记
      printf("\n Client[%d]: %s \n", client_fd, ev->m_buf);

      eventsetinit(ev, client_fd, senddata, ev);            //放上红黑树,监听写事件
      eventadd(ep_fd, EPOLLOUT, ev); 
  }
  else if (len == 0)
  {
      close(ev->m_fd);
      eventdel(ep_fd, ev);
      printf("\n [Client:%d] disconnection \n", ev->m_fd);
  }
  else
  {
      close(ev->m_fd);
      eventdel(ep_fd, ev);
      printf("\n error: [Client:%d] disconnection\n", ev->m_fd);
  }
 
  return;
}

/*发送数据*/
void senddata(int client_fd, int event, void *arg)
{
  int              len; 
  struct my_events *ev = (struct my_events *)arg;
 
  len = send(client_fd, ev->m_buf, ev->m_buf_len, 0);   //回写

  if (len > 0)
  {
     printf("\n send[fd=%d], [len=%d] %s \n", client_fd, len, ev->m_buf);
     eventdel(ep_fd, ev);
     eventsetinit(ev, client_fd, recvdata, ev);
     eventadd(ep_fd, EPOLLIN, ev);  
  }
  else
  {
     close(ev->m_fd);
     eventdel(ep_fd, ev);
     printf("\n send[fd=%d] error \n", client_fd);
  }
  return ;
}
