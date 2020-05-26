#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct stu
{
	char *name;	
} stu;

void setStu(void * st)
{
	stu * s = (stu *) st;
	s->name = "zhangsan";
}

int main()
{
	stu st;
	st.name = "lisi";
	setStu((void *) &st);
	printf("%s\n", st.name);
}

