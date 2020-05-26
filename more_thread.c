#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

int i = 1;
typedef struct 
{
	int a;
	char * b;
	void * p;//通用类型的指针	
}retval;

void * thread_call(void * args)
{
	retval *ret = (retval *)malloc(sizeof(retval));
	ret->a = (int) args;
	ret->b = "hello a!";
	ret->p = (void *)ret;
	printf("thread thread id %lu, pid %u\n", pthread_self(), getpid());

	pthread_exit((void *)ret);
}

int main(int argc, char const *argv[])
{
	pthread_t thread_ids[5];

	for (i = 0; i < 5; ++i)
	{
		int r = pthread_create(&thread_ids[i], NULL, thread_call, (void *)i);
		if (0 != r) {
			fprintf(stderr, "pthread_create error %s\n", strerror(r));
			exit(1);
		}
	}

	retval *retv = (retval *)malloc(sizeof(retval));
	retval *tmp = retv;//这里copy主要原因是要下面回收
	for (i = 0; i < 5; ++i)
	{
		pthread_join(th read_ids[i], (void **) &retv);

		printf("%d %s %p\n",retv->a, retv->b, retv->p);
		free((retval *)(retv->p));
	}

	free(tmp);
	// printf("%p %p\n", retv, tmp);
	pthread_exit((void*) 1);//退出的是主控线程，只有所有的子线程都执行完成，这个进程才完成
}
