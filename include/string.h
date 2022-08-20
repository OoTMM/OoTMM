#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <stdint.h>

void* memcpy(void* dst, const void* src, size_t size);
int   memcmp(const void* a, const void* b, size_t length);

size_t strlen(const char* str);

#endif /* STRING_H */
