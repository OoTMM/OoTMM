#ifndef STRING_H
#define STRING_H

#include <stdint.h>

void* memcpy(void* dst, const void* src, uint32_t size);
int   memcmp(const void* a, const void* b, uint32_t length);

#endif /* STRING_H */
