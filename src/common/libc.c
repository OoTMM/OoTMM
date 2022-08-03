#include <string.h>

void* memcpy(void* dst, const void* src, uint32_t size)
{
    for (uint32_t i = 0; i < size; ++i)
        ((char*)dst)[i] = ((char*)src)[i];
    return dst;
}

int memcmp(const void* a, const void* b, uint32_t length)
{
    for (uint32_t i = 0; i < length; ++i)
        if (((char*)a)[i] != ((char*)b)[i])
            return ((char*)a)[i] - ((char*)b)[i];
    return 0;
}
