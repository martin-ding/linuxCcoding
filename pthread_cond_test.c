#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct tt {
    int age;
    char [10] name;
} TT;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;//静态初始化
pthread_cond_t has_product = PTHREAD_COND_INITIALIZER;

int main(int argc, char const *argv[])
{
	int r = pthread_cond_wait(&has_product, &mutex);
	printf("------%d------\n", r);

	return 0;
}