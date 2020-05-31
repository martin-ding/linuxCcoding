#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <errno.h>
#include <ctype.h>
#include <fcntl.h>

#include "wrap.h"

#define SERVER_PORT 8888
#define OPEN_MAX    5000

int main(void)
{
/*-----------------------------------------*/
  int                     listen_fd;
  int                     connect_fd;
  int                     temp_fd;
  struct sockaddr_in      listen_socket_addr;
  struct sockaddr_in      client_socket_addr;
  socklen_t               listen_socket_len;
  socklen_t               client_socket_len;
/*-----------------------------------------*/
  struct epoll_event     listen_ep_event;
  struct epoll_event     temp_ep_event;
  struct epoll_event     ep_event[OPEN_MAX];
  int                    ep_fd;
/*-----------------------------------------*/
  int                    i;
  int                    j;
  int                    flag;
  int                    n_ready;
  int                    n_data;
  char                   str[BUFSIZ];
  char                   buf[BUFSIZ];
/*-----------------------------------------*/
  /* 初始化 */
  listen_socket_len  = sizeof(listen_socket_addr);
  client_socket_len  = sizeof(client_socket_addr);

  /* 申请一个socket */
  listen_fd = Socket(AF_INET, SOCK_STREAM, 0);
  /* 绑定前的初始化 */
  bzero(&listen_socket_addr, listen_socket_len);
  listen_socket_addr.sin_family      = AF_INET;
  listen_socket_addr.sin_port        = htons(SERVER_PORT);
  listen_socket_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  /* 绑定 */
  Bind(listen_fd, (struct sockaddr *)&listen_socket_addr, listen_socket_len );
  /* 设置监听上限 */
  Listen(listen_fd, 128);

   
  ep_fd = epoll_create(OPEN_MAX);             //创建epoll模型,ep_fd指向红黑树根节点
  if(ep_fd == -1)
   {
     perror("epoll_create error");
     exit(-1);  
   }
//listen_ep_event.events  = EPOLLIN;          //指定监听读事件/////注意:默认为水平触发LT
  listen_ep_event.events  = EPOLLIN | EPOLLET;//边沿触发
  listen_ep_event.data.fd = listen_fd;  
                                                    //将listen_fd和对应的结构体设置到树上
  if((epoll_ctl(ep_fd, EPOLL_CTL_ADD, listen_fd, &listen_ep_event)) == -1 )
   {
     perror("epoll_ctl error");
     exit(-1);
   }


  while(1)
   { 
      /* epoll为server阻塞监听事件,ep_event是数组,装满足条件后的所有事件结构体 */
      n_ready = epoll_wait(ep_fd, ep_event, OPEN_MAX, -1); 
      if(n_ready == -1)
      {
         perror("n_ready == -1 : error");
         exit(-1);
      }

      for(i=0; i<n_ready; i++)
      {
         temp_fd = ep_event[i].data.fd;
         if(ep_event[i].events & EPOLLIN)
          {
            if(temp_fd == listen_fd) //说明有新连接到来
             {
               connect_fd = Accept(listen_fd, (struct sockaddr *)&client_socket_addr,
                                              &client_socket_len);

              /* 修改connect_fd为非阻塞读, 例如不是，当100字节的数据用readn读200，阻塞  */
               flag  = fcntl(connect_fd, F_GETFL);
               flag |= O_NONBLOCK;
               fcntl(connect_fd, F_SETFL, flag); 
              
               printf("\n received from %s port %d  \n", 
                           inet_ntop(AF_INET, &client_socket_addr.sin_addr, str, sizeof(str)),
                           ntohs(client_socket_addr.sin_port));
               temp_ep_event.events  = EPOLLIN;
               temp_ep_event.data.fd = connect_fd;
               if((epoll_ctl(ep_fd, EPOLL_CTL_ADD, connect_fd, &temp_ep_event)) == -1)
                {
                   perror("epoll_ctl error, connect_fd");
                   exit(-1);
                }
             }
            else                                 //有数据到来
             {
               n_data = Read(temp_fd , buf, sizeof(buf));

                if(n_data == 0)                  //客户端关闭
                {
                   if((epoll_ctl(ep_fd, EPOLL_CTL_DEL, temp_fd, NULL)) == -1)
                   {
                      perror("epoll_ctl error, del error");
                      exit(-1);
                   }
                   Close(temp_fd);
                   printf("\n close client[%d]  \n", temp_fd);
                }
                else if(n_data < 0)
                {
                   perror(" n_data<0");
                    if((epoll_ctl(ep_fd, EPOLL_CTL_DEL, temp_fd, NULL)) == -1)
                     {
                        perror("epoll_ctl error, del error, n_data < 0");
                        exit(-1);
                     }
                    Close(temp_fd);
                }            
                do
                 {
                   //处理数据
                   for(j=0; j<n_data; j++)
                    {
                       buf[j] = toupper(buf[j]);
                    }
                    Write(STDOUT_FILENO, buf, n_data);
                    Write(temp_fd, buf, n_data);
                 }while( (n_data = Read(temp_fd , buf, sizeof(buf))) >0 ) ;

             }
          }
         else if(ep_event[i].events & EPOLLOUT)
         {
                //处理写事件
         }
         else if(ep_event[i].events & EPOLLERR)
         {
                //处理异常事件
         }
      }      

   }
  Close(listen_fd);
  close(ep_fd);


}
















