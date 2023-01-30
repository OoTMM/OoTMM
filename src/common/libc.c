#include <string.h>

int toupper(int c)
{
    if (c >= 'a' && c <= 'z')
    {
        c -= 'a';
        c += 'A';
    }
    return c;
}

void* memcpy(void* dst, const void* src, size_t size)
{
    for (size_t i = 0; i < size; ++i)
        ((char*)dst)[i] = ((char*)src)[i];
    return dst;
}

int memcmp(const void* a, const void* b, size_t length)
{
    for (size_t i = 0; i < length; ++i)
        if (((char*)a)[i] != ((char*)b)[i])
            return ((char*)a)[i] - ((char*)b)[i];
    return 0;
}

size_t strlen(const char* str)
{
    size_t i = 0;
    while (str[i] != '\0')
        ++i;
    return i;
}
