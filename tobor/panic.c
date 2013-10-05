#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <tobor/panic.h>

void _panic(const char* fmt, ...)
{
     va_list ap;

     va_start(ap, fmt);
     vfprintf(stderr, fmt, ap);
     va_end(ap);

     fputc('\n', stderr);

     exit(1);
}
