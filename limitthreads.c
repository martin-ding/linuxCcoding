#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


void * threadCall(void * arg)
{
	while(1) {
		printf("child --\n");
		sleep(1);
	}
}

void * threadCall2(void * arg)
{
	pid_t pid = fork();
	if (pid == -1) {
		perror("process err");
		exit(EXIT_FAILURE);
	}
	if (pid == 0) {
		execlp("ls", "ls", "-l", NULL);	
	} else {
		// pthread_exit(NULL);
	}
}


int main()
{
	pthread_t tid;
	int count = 0;
	int a;

	a = pthread_create(&tid, NULL, threadCall, NULL);
	if (a != 0) {
		printf("=====%d=====\n", a);
	}

	//这个线程中执行fork
	a = pthread_create(&tid, NULL, threadCall2, NULL);
	if (a != 0) {
		printf("=====%d=====\n", a);
	}

	sleep(1);
	while (1) {
		printf("-----%d-----\n", count++);
		sleep(1);
	}

}