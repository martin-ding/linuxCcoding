#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void * callThread(void *arg) {
	printf("child .....\n");

	pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{

	pthread_attr_t attr;
	pthread_t tid;
	int r = pthread_attr_init(&attr);
	if (r != 0) {
		printf("pthread_attr error %d, %s\n", r, strerror(r));
	}

	int m = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	// int m = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	if (m != 0) {
		printf("pthread_detach error %d, %s\n", m, strerror(m));
	}
	
	int c = pthread_create(&tid, &attr, callThread, NULL);
	if (c != 0) {
		printf("pthread_create error %d, %s\n", c, strerror(c));
	}

	int p = pthread_cancel(tid);
	if (p != 0) {
		printf("pthread_cancel error %d, %s\n", p, strerror(p));
	}

	void *ret;

	int j = pthread_join(tid, &ret);


	if (j != 0) {
		printf("pthread_join error %d, %s\n",j,strerror(j));
	} 

	if (ret == PTHREAD_CANCELED) {
		printf("cancel success\n");
	}

	printf("---------%d\n", j);	


	pthread_attr_destroy(&attr);

	return 0;
}