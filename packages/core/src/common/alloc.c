#include <combo.h>

#if defined(GAME_OOT)
# define HEAP_BASE 0x80700000
# define HEAP_SIZE 0x100000
#else
# define HEAP_BASE 0x80700000
# define HEAP_SIZE 0x40000
#endif

#define SENTINEL 0x99999999

typedef struct HeapBlockHeader HeapBlockHeader;

struct HeapBlockHeader
{
    HeapBlockHeader*    next;
    HeapBlockHeader*    prev;
    u32                 size;
    u32                 free;
};

static HeapBlockHeader* gHeapFirstBlock;

void initHeap(void)
{
    gHeapFirstBlock = (HeapBlockHeader*)HEAP_BASE;
    gHeapFirstBlock->next = NULL;
    gHeapFirstBlock->prev = NULL;
    gHeapFirstBlock->size = HEAP_SIZE;
    gHeapFirstBlock->free = 1;

#if defined(DEBUG_ALLOC)
    memset((char*)HEAP_BASE + 0x10, 0x99, HEAP_SIZE - 0x10);
#endif
}

/* Dumb linear allocator */
static void* _malloc(size_t size)
{
    HeapBlockHeader* best;
    HeapBlockHeader* block;

    /* Round up to multiple of 16 */
    size = (size + 15) & ~15;

    /* Add header size */
    size += sizeof(HeapBlockHeader);

    /* Search for the smallest block */
    best = NULL;
    block = gHeapFirstBlock;
    while (block)
    {
        /* Check for a smaller block */
        if (block->free && block->size >= size && (!best || block->size < best->size))
            best = block;

        /* Next block */
        block = block->next;
    }

    /* No block found */
    if (!best)
        return NULL;

    /* If the block is an exact match, or splitting would make the next block be smaller than 32 bytes, return it directly */
    if (best->size - size <= 32)
    {
        best->free = 0;
        return &best[1];
    }

    /* We need to split the block */
    block = (HeapBlockHeader*)((u32)best + size);
    if (best->next)
    {
        best->next->prev = block;
        block->next = best->next;
    }
    else
    {
        block->next = NULL;
    }
    block->prev = best;
    best->next = block;

    block->size = best->size - size;
    block->free = 1;
    best->size = size;
    best->free = 0;

    return &best[1];
}

static void mergeFreeBlocks(HeapBlockHeader* block)
{
    for (;;)
    {
        /* Check for end of allocs or a non-free block */
        if (!block->next || !block->next->free)
            return;

        /* The next block can be merged */
        block->size += block->next->size;
        block->next = block->next->next;
        if (block->next)
        {
            block->next->prev = block;
        }
    }
}

static void _free(void* data)
{
    HeapBlockHeader* block;

    if (!data)
        return;

    /* Get the block header */
    block = (HeapBlockHeader*)data - 1;

    /* Mark the block as free */
    block->free = 1;

    /* If the previous block is free, merge from it */
    if (block->prev && block->prev->free)
        block = block->prev;
    mergeFreeBlocks(block);

#if defined(DEBUG_ALLOC)
    memset((char*)block + 16, 0x99, block->size - sizeof(HeapBlockHeader));
#endif
}

void* malloc(size_t size)
{
    u64 mask;
    void* ret;

    mask = osSetIntMask(1);
    ret = _malloc(size);
    osSetIntMask(mask);

    return ret;
}

void free(void* data)
{
    u64 mask;

    mask = osSetIntMask(1);
    _free(data);
    osSetIntMask(mask);
}

#if defined(DEBUG_ALLOC)
void malloc_check(void)
{
    u64 mask;
    HeapBlockHeader* block;
    u32 count;
    u32* base;

    mask = osSetIntMask(1);
    block = gHeapFirstBlock;
    while (block)
    {
        if (!block->free)
        {
            block = block->next;
            continue;
        }

        /* Check the sentinel */
        count = (block->size - sizeof(HeapBlockHeader)) / 4;
        base = (u32*)&block[1];
        for (u32 i = 0; i < count; ++i)
        {
            if (base[i] != SENTINEL)
            {
                for (;;) {}
            }
        }

        block = block->next;
    }
    osSetIntMask(mask);
}
#endif
