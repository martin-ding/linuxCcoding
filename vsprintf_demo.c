#include <stdarg.h>
#include <stdio.h>

void foo(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
}

int main(int argc, char const *argv[])
{
  foo("%s %s %d %c", "zhangsan", "jinnian", 15, 'A');
  return 0;
}