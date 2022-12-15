#include <combo.h>

#if defined(GAME_MM)
static void* gHeap = (void*)0x80740000;

/* Dumb slab allocator for MM */
void* malloc(size_t size)
{
    void* ret;

    ret = gHeap;
    size = (size + 15) & ~15;
    gHeap = (void*)((u32)gHeap + size);
    return ret;
}
#endif
