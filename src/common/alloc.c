#include <combo.h>

#if defined(GAME_OOT)
static void* gHeap = (void*)0x80700000;
#else
static void* gHeap = (void*)0x80740000;
#endif

/* Dumb slab allocator */
void* malloc(size_t size)
{
    void* ret;

    ret = gHeap;
    size = (size + 15) & ~15;
    gHeap = (void*)((u32)gHeap + size);
    return ret;
}
