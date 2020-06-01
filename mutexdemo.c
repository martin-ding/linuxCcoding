 #include <pthread.h>
 #include <stdio.h>
 #include <stdlib.h>

int main(int argc, char const *argv[])
{
	pthread_mutex_t m;

	pthread_mutex_init(&m, NULL);
	pthread_mutex_lock(&m);
	int a =12;
	pthread_mutex_unlock(&m);

	return 0;
}