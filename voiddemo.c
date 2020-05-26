#include <stdio.h>

int callvoid(const void name) {
	printf("%d\n", (int)name);
}

int main(int argc, char const *argv[])
{
	callvoid((void)12);	
	return 0;
}