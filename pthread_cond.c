#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct pro
{
	int val;
	struct pro *next;//指向下一个结点
} pro;

pro *tail, *head = NULL;



pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;//静态初始化
pthread_cond_t has_product = PTHREAD_COND_INITIALIZER;


void * produce(void *agc)
{
	while (1) {
		printf("--------\n");
		pthread_mutex_lock(&mutex);
		pro *pr = (pro *)malloc(sizeof(pro));//分配空间

		pr->val = rand() % 1000 + 1;
		printf("produce %d...\n", pr->val);
		pr->next = head;
		head = pr;//头插法

		pthread_cond_signal(&has_product);
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
	pthread_exit(NULL);
}

void * consume(void *agc)
{
	while (1) {
		pthread_mutex_lock(&mutex);

		while(head == NULL) {
			pthread_cond_wait(&has_product, &mutex);
		}

		pro *pr = head;
		head = head->next;
		printf("thread %d : consume %d...\n", (int) agc, pr->val);
		free(pr);
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
	pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
	srand(time(NULL));//随机数seeds
	/* code */
	//创建两个线程一个生产一个消费
	pthread_t p_tid, c_tid, c2_tid;
	pthread_create(&p_tid, NULL, produce, NULL);
	pthread_create(&c_tid, NULL, consume, (void *)1);
	pthread_create(&c2_tid, NULL, consume, (void *)2);
	
	pthread_join(p_tid, NULL);

	pthread_join(c_tid, NULL);	
	pthread_join(c2_tid, NULL);
	return 0;
}