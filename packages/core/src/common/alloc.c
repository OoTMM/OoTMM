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

void* malloc(size_t size)
{
    HeapBlockHeader* best;
    HeapBlockHeader* block;

    /* Protect against overflow / obvious mistakes */
    if (size >= 0x10000000)
        return NULL;

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

void free(void* data)
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

static void* _realloc_shrink(HeapBlockHeader* block, size_t size)
{
    size_t diff;
    HeapBlockHeader* nextBlock;
    HeapBlockHeader* newBlock;

    diff = block->size - size;
    if (diff <= 16)
    {
        /* Not enough */
        return &block[1];
    }

    nextBlock = block->next;
    newBlock = (HeapBlockHeader*)((char*)block + size);
    block->size = size;
    block->next = newBlock;
    newBlock->free = 1;
    newBlock->prev = block;

    if (nextBlock && nextBlock->free)
    {
        newBlock->size = diff + nextBlock->size;
        newBlock->next = nextBlock->next;
    }
    else
    {
        newBlock->size = diff;
        newBlock->next = nextBlock;
    }

    if (newBlock->next)
        newBlock->next->prev = newBlock;

    return &block[1];
}

static void* _realloc_grow(HeapBlockHeader* block, size_t size)
{
    HeapBlockHeader* nextBlock;
    HeapBlockHeader  nextBlockBackup;
    size_t           diff;
    void*            dst;
    void*            src;

    nextBlock = block->next;
    diff = size - block->size;
    if (nextBlock && nextBlock->free && nextBlock->size >= diff)
    {
        /* Fast path, we can take over the next block */
        if (nextBlock->size - diff <= 16)
        {
            /* The resulting block would be too small, completely take over it */
            block->next = nextBlock->next;
            if (block->next)
                block->next->prev = block;
            block->size += nextBlock->size;
            return &block[1];
        }

        /* We need to split - copy the next block as it might alias */
        memcpy(&nextBlockBackup, nextBlock, sizeof(nextBlockBackup));
        block->size = size;
        nextBlock = (HeapBlockHeader*)((char*)block + block->size);
        nextBlock->prev = block;
        block->next = nextBlock;
        nextBlock->next = nextBlockBackup.next;
        if (nextBlock->next)
            nextBlock->next->prev = nextBlock;
        nextBlock->free = 1;
        nextBlock->size = nextBlockBackup.size - diff;
        return &block[1];
    }
    else
    {
        /* Slow path, we need to do a completely new alloc and free */
        src = &block[1];
        dst = malloc(size - sizeof(HeapBlockHeader));
        if (!dst)
            return NULL;
        memcpy(dst, src, block->size - sizeof(HeapBlockHeader));
        free(src);
        return dst;
    }
}

void* realloc(void* data, size_t size)
{
    size_t internalSize;
    HeapBlockHeader* block;

    /* Realloc to zero is a free */
    if (size == 0)
    {
        free(data);
        return NULL;
    }

    /* Realloc of NULL is a malloc */
    if (data == NULL)
        return malloc(size);

    /* Protect against overflow / obvious mistakes */
    if (size >= 0x10000000)
        return NULL;

    /* Round size */
    internalSize = (size + 15) & ~15;
    internalSize += sizeof(HeapBlockHeader);

    /* Fetch block */
    block = &(((HeapBlockHeader*)data)[-1]);

    if (block->size == internalSize)
    {
        /* Nothing to do, no-op realloc */
        return data;
    }

    /* Shrink */
    if (block->size > internalSize)
        return _realloc_shrink(block, internalSize);

    /* We need to enlarge the current block */
    return _realloc_grow(block, internalSize);
}

#if defined(DEBUG_ALLOC)
void malloc_check(void)
{
    HeapBlockHeader* block;
    u32 count;
    u32* base;

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
}
#endif
