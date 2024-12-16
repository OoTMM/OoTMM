#ifndef _STDLIB_H
#define _STDLIB_H

#include <sys/_size_t.h>

void* malloc(size_t size);
void free(void* ptr);

int abs(int c);

#endif
