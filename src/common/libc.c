#include <string.h>

void* memcpy(void* dst, const void* src, uint32_t size)
{
    for (uint32_t i = 0; i < size; ++i)
        ((char*)dst)[i] = ((char*)src)[i];
    return dst;
}
