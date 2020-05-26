#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void * thread_call(void *args) 
{
	int n = 3;
	while (n--) {
		printf("thread call %d\n", n);
	}
	// pthread_exit();
	return (void *)1;
}


int main(int argc, char const *argv[])
{
	pthread_t tid;
	
	void * p;

	pthread_create(&tid, NULL, thread_call, NULL);
	// pthread_detach(tid);

	while (1) {
		int err = pthread_join(tid, &p);
		if (err != 0) {
			printf("tid %lu error with no: %d  %s\n", tid, err, strerror(err));
		} else {
			printf("tid %lu success join return %d\n", tid, (int) p);
		}
		sleep(1);
	}

	return 0;
}