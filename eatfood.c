#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


//想法在只有同时占据了左右两个筷子的才可以吃饭

#define NUM 5 //5个哲学家吃饭
#define errh(msg) {perror(msg); exit(EXIT_FAILURE);}

typedef struct shared {
	int num;//现在已经多少个人在吃饭
	pthread_mutexattr_t attr;
	pthread_mutex_t mutex1, mutex2;
	pthread_mutex_t mutex[NUM];//锁数组
} shared;

int main () {
	srand(time(NULL));
	shared *mm = (shared*) malloc(sizeof(shared));//分配空间
	memset(mm, 0, sizeof(shared));//初始化

	//设置排它锁
	pthread_mutexattr_init(&mm->attr);
	pthread_mutexattr_setpshared(&mm->attr, PTHREAD_PROCESS_SHARED);

	pthread_mutex_init(&mm->mutex1, &mm->attr);
	pthread_mutex_init(&mm->mutex2, &mm->attr);
	
	int k;
	for (k = 0; k < NUM; ++k)
	{
		pthread_mutex_init(&mm->mutex[k], &mm->attr);		
	}

	pid_t pid;
	int i,n, left, right;
	for (i = 0; i < NUM; ++i)
	{
		pid = fork();
		if (pid == -1) {
			errh("error pid");
		}

		if (pid == 0) {
			break;
		}
	}

	if (i == NUM) {
		while(wait(NULL));//父进程一直等待回收子进程，可以注册信号捕捉函数处理
	} else {
		while (1) {
			

			// 吃饭 左边筷子i，右边筷子 (i+1)%NUM
			left = i;
			right = (i+1) % NUM;

			pthread_mutex_lock(&mm->mutex1);
	
			pthread_mutex_lock(&mm->mutex[right]);
			pthread_mutex_lock(&mm->mutex[left]);

			pthread_mutex_unlock(&mm->mutex1);	

			printf("=== %d 哲学家吃饭.....\n", i);
			sleep(rand() % 2);

			pthread_mutex_unlock(&mm->mutex[left]);
			pthread_mutex_unlock(&mm->mutex[right]);

		}
	}
	return 0;

}