#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	int fd = open("/root/libevent/sample/event.fifo", O_RDWR | O_APPEND);

	int n = 100;
	char str[10] = {0};

	while (1) {
		write(fd, "this is a number ", strlen("this is a number "));
		sprintf(str, "%d", n++);
		write(fd, str, strlen(str));
		sleep(1);
	}
	return 0;
}