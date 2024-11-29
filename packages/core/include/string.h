#ifndef _STRING_H
#define _STRING_H

#include <sys/_size_t.h>

void* memcpy(void* dest, const void* src, size_t n);
void* memmove(void* dest, const void* src, size_t n);
void* memset(void* dest, int c, size_t n);
int   memcmp(const void* s1, const void* s2, size_t n);

size_t strlen(const char* str);

#endif
