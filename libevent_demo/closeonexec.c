#include <stdio.h>

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define _GNU_SOURCE


int main(int argc, char const *argv[])
{
	int fd = open("foohua.c", O_RDWR|O_CREAT|O_APPEND|O_CLOEXEC, 666);

	pid_t pid =  fork();
	if (pid == 0) {
		write(fd, "dhu\n", 4);
		close(fd);
	} else {
		sleep(1);
		write(fd, "bas\n", 4);
		close(fd);
	}

	return 0;
}