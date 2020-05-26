#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <errno.h> /* superset of previous */
#include <stdlib.h> /* superset of previous */

#define errhand(msg) {perror(msg); exit(EXIT_FAILURE);}


int Socket(int domain, int type, int protocol)
{
	int ret = socket(domain, type, protocol);
	if (ret == -1) {
		errhand("socket error");
	}
	return ret;
}

int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	int ret = bind(sockfd, addr, addrlen);
	if (ret == -1) {
		errhand("bind error");
	}
	return ret;
}

int Listen(int sockfd, int backlog) 
{
	int ret = listen(sockfd, backlog);
	if (ret == -1) {
		errhand("listen error");
	}
	return ret;
}

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) 
{
	int ret = accept(sockfd, addr, addrlen);
	if (ret == -1) {
		errhand("accept error");
	}
	return ret;
}

//EINTR 重启
ssize_t Read(int fd, void *buf, size_t count) 
{
	ssize_t n = 0;
	while (n <= 0) {
		if ((n = read(fd, buf, count)) < 0) {
			if (errno == EINTR) {
				n = 0;
				continue;//重新读数据
			}
			return -1;
		} else {
			break;//0表示读到结尾
		}
	}
	return n;
}

ssize_t Write(int fd, const void *buf, size_t count) 
{
	ssize_t n = 0;
	while (n <= 0) {
		if ((n = write(fd, buf, count)) <= 0) {
			if (EINTR == errno && n == -1) {//被信号中断
				n = 0;
				continue;
			} else {
				return -1;//If count is  zero and fd refers to a file other than a regular file, the results are not specified.
			}
		} else {
			break;
		}
	}
	return n;
}

//一个以太帧有有大小限定比如1500字节，我要读取4096，
//就必须读取多个帧，正常的read的函数在读取一个之后就返回了
ssize_t Readn(int fd, void * vptr, size_t n)
{
	size_t nleft;
	ssize_t nread;
	char *ptr;

	ptr = vptr;
	nleft = n;

	while (nleft > 0) {
		if ((nread = read(fd, ptr, nleft)) < 0) {
			if (errno == EINTR) {
				nread = 0;
			} else {
				return -1;
			}
		} else if (nread == 0) {
			break;
		}

		nleft -= nread;
		ptr += nread;
	}
	return n - nleft;
}

//往里面写入数据也不可能是能一次性写进去的
ssize_t Writen(int fd, void * vptr, size_t n) {
	size_t nleft;
	ssize_t nwrite;

	char * ptr;

	ptr = vptr;

	while (1) {
		if ((nwrite = write(fd,  ptr, nleft)) <= 0) {// 写入的数据个数为0的时候就非普通文件而言就是错误
			if (errno == EINTR) {
				nwrite = 0;
			} else {
				return -1;// 错误返回
			}
		}

		nleft -= nwrite;
		ptr += nwrite;
	}
	return n - nleft;
}

//读取100个字符
static ssize_t my_read(int fd, char *ptr)
{
	static char buf[100];
	static int read_cnt = 0;//读取到的个数
	static char *read_ptr;
	
	if (read_cnt <= 0) {
		again:
		if((read_cnt = read(fd, buf, sizeof(buf))) < 0) {
			if (errno == EINTR) {
				goto again;//也有可能已经读取到数据了
			}
			return -1;//有错
		} else if (read_cnt == 0) {
			return 0;//文件结束
		}
		read_ptr = buf;
	}

	read_cnt--;
	*ptr = *read_ptr++;
	return 1;//成功
}


static ssize_t readline(int fd, void *vptr, int maxlen)
{
	int i, rc;
	char ch;
	char *ptr = vptr;

	for (i = 1; i < maxlen; ++i)
	{
		//读数据
		if ((rc = my_read(fd, &ch)) == 1) {
			*ptr++ = ch;
			if (ch == '\n') {
				break;
			}
		} else if (rc == 0){//文件读完
			*ptr = 0;
			return i-1;//去读到的字符串
		} else {
			return -1;
		}
	}
	*ptr = 0;

	return i;
}




