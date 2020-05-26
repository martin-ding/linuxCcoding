#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

pthread_mutex_t mutex;

void * callThread (void* arg)
{
	srand(time(NULL));
	while (1) {
		pthread_mutex_lock(&mutex);
		printf("hello ");
		sleep(rand() % 3);//模拟长时间占用CPU
		printf("world\n");
		pthread_mutex_unlock(&mutex);
		sleep(rand() % 3);
	}
}

int main(int argc, char const *argv[])
{
	pthread_t tid;
	

	pthread_mutex_init(&mutex, NULL);


	pthread_create(&tid, NULL, callThread, NULL);

	srand(time(NULL));

	while (1) {
		//占用锁
		pthread_mutex_lock(&mutex);
		printf("HELLO ");
		sleep(rand() % 3);//模拟长时间占用CPU
		printf("WORLD\n");
		pthread_mutex_unlock(&mutex);
		sleep(rand() % 3);
		//释放锁
	}

	pthread_mutex_destroy(&mutex);
	return 0;
}