#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void * callThread(void *arg) {
	// while (1) {
		// sleep(1);
	// 	printf("child ....\n");
	// }
	
	printf("child .....\n");
	pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{

	pthread_t tid;
	int c = pthread_create(&tid, NULL, callThread, NULL);
	if (c != 0) {
		printf("pthread_create error %d, %s\n", c, strerror(c));
	}
	// sleep (3);
	int p = pthread_cancel(tid);
	if (p != 0) {
		printf("pthread_cancel error %d, %s\n", p, strerror(p));
	}

	void *ret;
	// while (1) {
		int j = pthread_join(tid, &ret);

	
		if (j != 0) {
			printf("pthread_join error %d, %s\n",j,strerror(j));
		} 

		if (ret == PTHREAD_CANCELED) {
			printf("cancel success\n");
		}

	printf("---------%d\n", j);	
	// }

	pthread_attr_destroy(&attr);

	return 0;
}