#ifndef __wrap_head_
#define __wrap_head_ 1

#include <stdio.h>


int Socket(int domain, int type, int protocol);
int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int Listen(int sockfd, int backlog);
int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
ssize_t Read(int fd, void *buf, size_t count);
ssize_t Write(int fd, const void *buf, size_t count);
ssize_t Readn(int fd, void * vptr, size_t n);
ssize_t Writen(int fd, void * vptr, size_t n);
static ssize_t my_read(int fd, char *ptr);
static ssize_t readline(int fd, void *vptr, int maxlen);

#endif
