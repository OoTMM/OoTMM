#include <string.h>
#include <combo.h>

#if defined(__OOT__)
# define HEAP_SIZE 0x100000
#else
# define HEAP_SIZE 0x80000
#endif

static char sCustomHeap[HEAP_SIZE] __attribute__((aligned(16))) = { 0 };

typedef struct CustomHeapBlockHeader CustomHeapBlockHeader;

#define HEAP_FIRST_BLOCK ((CustomHeapBlockHeader*)sCustomHeap)

struct CustomHeapBlockHeader
{
    CustomHeapBlockHeader*  next;
    CustomHeapBlockHeader*  prev;
    u32                     size;
    u32                     free;
};

void CustomHeap_Init(void)
{
    CustomHeapBlockHeader* blk;

    blk = HEAP_FIRST_BLOCK;
    blk->next = NULL;
    blk->prev = NULL;
    blk->size = HEAP_SIZE;
    blk->free = 1;
}

void* CustomHeap_Alloc(size_t size)
{
    CustomHeapBlockHeader* best;
    CustomHeapBlockHeader* block;

    /* Protect against overflow / obvious mistakes */
    if (size >= 0x10000000)
        return NULL;

    /* Round up to multiple of 16 */
    size = (size + 15) & ~15;

    /* Add header size */
    size += sizeof(CustomHeapBlockHeader);

    /* Search for the smallest block */
    best = NULL;
    block = HEAP_FIRST_BLOCK;
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
    block = (CustomHeapBlockHeader*)((u32)best + size);
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

static void mergeFreeBlocks(CustomHeapBlockHeader* block)
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

void CustomHeap_Free(void* data)
{
    CustomHeapBlockHeader* block;

    if (!data)
        return;

    /* Get the block header */
    block = (CustomHeapBlockHeader*)data - 1;

    /* Mark the block as free */
    block->free = 1;

    /* If the previous block is free, merge from it */
    if (block->prev && block->prev->free)
        block = block->prev;
    mergeFreeBlocks(block);
}

static void* _realloc_shrink(CustomHeapBlockHeader* block, size_t size)
{
    size_t diff;
    CustomHeapBlockHeader* nextBlock;
    CustomHeapBlockHeader* newBlock;

    diff = block->size - size;
    if (diff <= 16)
    {
        /* Not enough */
        return &block[1];
    }

    nextBlock = block->next;
    newBlock = (CustomHeapBlockHeader*)((char*)block + size);
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

static void* _realloc_grow(CustomHeapBlockHeader* block, size_t size)
{
    CustomHeapBlockHeader*  nextBlock;
    CustomHeapBlockHeader   nextBlockBackup;
    size_t                  diff;
    void*                   dst;
    void*                   src;

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
        nextBlock = (CustomHeapBlockHeader*)((char*)block + block->size);
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
        dst = CustomHeap_Alloc(size - sizeof(CustomHeapBlockHeader));
        if (!dst)
            return NULL;
        memcpy(dst, src, block->size - sizeof(CustomHeapBlockHeader));
        CustomHeap_Free(src);
        return dst;
    }
}

void* CustomHeap_Realloc(void* data, size_t size)
{
    size_t                  internalSize;
    CustomHeapBlockHeader*  block;

    /* Realloc to zero is a free */
    if (size == 0)
    {
        CustomHeap_Free(data);
        return NULL;
    }

    /* Realloc of NULL is a malloc */
    if (data == NULL)
        return CustomHeap_Alloc(size);

    /* Protect against overflow / obvious mistakes */
    if (size >= 0x10000000)
        return NULL;

    /* Round size */
    internalSize = (size + 15) & ~15;
    internalSize += sizeof(CustomHeapBlockHeader);

    /* Fetch block */
    block = &(((CustomHeapBlockHeader*)data)[-1]);

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
