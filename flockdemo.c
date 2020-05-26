#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>


int main()
{
	struct flock fl;
	fl.l_type = F_RDLCK;
	fl.l_whence = SEEK_SET;
	fl.l_start = 0;
	fl.l_len = 0;

	int fd = open("fix.txt", O_RDWR|O_CREAT, 0666);
	int r = fcntl(fd, F_SETLKW, &fl);
	if (r != 0) {
		perror("xxxxx");
		exit(EXIT_FAILURE);
	} else {
		printf("加锁成功\n");
	}

	write(fd, "this is common", strlen("this is common"));
	int i;
	for ( i= 0; i < 10; ++i)
	{
		printf("----------\n");
		sleep(1);
	}
	

	fl.l_type = F_UNLCK;
	fcntl(fd, F_SETLKW, &fl);

	printf("解锁成功\n");
	

	close(fd);

	return 0;
}