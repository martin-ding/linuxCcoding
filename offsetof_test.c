#include <stdio.h>

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif


typedef struct std
{
	char name[108];
	int age;
	float score;
} std;

int main(int argc, char const *argv[])
{
	std st;
	int a = offsetof(std, age);
	printf("%d\n", a);//返回108
	return 0;
}