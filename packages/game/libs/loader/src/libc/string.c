#include <string.h>

char* strchr(const char* str, int c) {
    char ch = c;

    while (*str != ch) {
        if (*str == 0) {
            return NULL;
        }
        str++;
    }
    return (char*)str;
}

size_t strlen(const char* str) {
    const char* ptr = str;

    while (*ptr) {
        ptr++;
    }
    return ptr - str;
}

int strcmp(const char* str1, const char* str2) {
    unsigned char c1;
    unsigned char c2;

    do {
        c1 = *str1++;
        c2 = *str2++;

        if (c1 != c2) {
            return c1 - c2;
        }
    } while (c1);

    return 0;
}

char* strcpy(char* dst, const char* src) {
    char* _dst = dst;

    while (*src != '\0') {
        *_dst++ = *src++;
    }
    *_dst = '\0';

    return dst;
}

__attribute__((optimize("-fno-tree-loop-distribute-patterns")))
void* memcpy(void* dst, const void* src, size_t n) {
    char* _dst = dst;
    const char* _src = src;

    while (n > 0) {
        *_dst++ = *_src++;
        n--;
    }
    return dst;
}

__attribute__((optimize("-fno-tree-loop-distribute-patterns")))
void* memmove(void* dst, const void* src, size_t size) {
    unsigned char* _dst = dst;
    const unsigned char* _src = src;
    register size_t rem;

    if (_dst == _src) {
        return dst;
    }
    if (_dst < _src) {
        for (rem = size--; rem != 0; rem = size--) {
            *_dst++ = *_src++;
        }
    } else {
        _dst += size - 1;
        _src += size - 1;
        for (rem = size--; rem != 0; rem = size--) {
            *_dst-- = *_src--;
        }
    }
    return dst;
}

__attribute__((optimize("-fno-tree-loop-distribute-patterns")))
void* memset(void* ptr, int val, size_t size) {
    unsigned char* dst = ptr;
    register size_t rem;

    for (rem = size--; rem != 0; rem = size--) {
        *dst++ = val;
    }

    return ptr;
}

__attribute__((optimize("-fno-tree-loop-distribute-patterns")))
int memcmp(const void* s1, const void* s2, size_t n) {
    const char* m1 = s1;
    const char* m2 = s2;
    unsigned char v1;
    unsigned char v2;
    size_t i;

    for (i = 0; i < n; i++) {
        v1 = (unsigned char)m1[i];
        v2 = (unsigned char)m2[i];
        if (v1 < v2) {
            return -1;
        } else if (v1 > v2) {
            return 1;
        }
    }

    return 0;
}
