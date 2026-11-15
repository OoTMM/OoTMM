#ifndef _STDLIB_H
#define _STDLIB_H

#include <sys/_size_t.h>

void* malloc(size_t size);
void* mallocGameplay(size_t size);
void* mallocWithReserve(size_t size, size_t reserve);
void free(void* ptr);

int abs(int c);

#endif
