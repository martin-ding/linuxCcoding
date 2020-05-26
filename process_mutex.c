#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <pthread.h>


typedef struct share
{
	int num;
	pthread_mutexattr_t pmt;
	pthread_mutex_t mutex;
} share;

int main()
{
	srand(time(NULL));
	share *mm = (share *)mmap(NULL,sizeof(share), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	memset(mm, 0, sizeof(share));
	mm->num = 0;//其实这一步多余

	pthread_mutexattr_init(&mm->pmt);
	pthread_mutexattr_setpshared(&mm->pmt, PTHREAD_PROCESS_PRIVATE);

	pthread_mutex_init(&mm->mutex, &mm->pmt);

	pid_t pid = fork();

	if (pid == 0) {
		while (1) {
		pthread_mutex_lock(&mm->mutex);
		mm->num++;
		int n = mm->num;
		pthread_mutex_unlock(&mm->mutex);
		printf("------child (num++) %d\n", n);
		sleep(rand() % 3);
	}
	} else {
		while (1) {
		pthread_mutex_lock(&mm->mutex);

		mm->num--;
		int n = mm->num;
		pthread_mutex_unlock(&mm->mutex);
		printf("------parent (num--) %d\n", n);
		sleep(rand() % 3);
		}


	}
	// pthread_mutexattr_destroy(&pmt);//销毁attr和
	
}