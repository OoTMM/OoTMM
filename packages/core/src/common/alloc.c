#include <combo.h>

#if defined(GAME_OOT)
# define HEAP_BASE 0x80700000
# define HEAP_SIZE 0x100000
#else
# define HEAP_BASE 0x80730000
# define HEAP_SIZE 0x10000
#endif

typedef struct
{
    u32     free:1;
    u32     size:31;
    void*   base;
}
HeapBlock;

typedef struct
{
    u32         count;
    HeapBlock   blocks[128];
}
Heap;

static Heap gHeap;

void initHeap(void)
{
    gHeap.count = 1;
    gHeap.blocks[0].free = 1;
    gHeap.blocks[0].size = HEAP_SIZE;
    gHeap.blocks[0].base = (void*)HEAP_BASE;
}

/* Very dumb linear allocator */
void* malloc(size_t size)
{
    HeapBlock* best;
    HeapBlock* newBlock;

    /* Round up to multiple of 16 */
    size = (size + 15) & ~15;

    /* Search for the smallest block */
    best = NULL;
    for (int i = 0; i < gHeap.count; ++i)
    {
        HeapBlock* block = &gHeap.blocks[i];
        if (block->free && block->size >= size)
        {
            if (!best || block->size < best->size)
                best = block;
        }
    }

    /* No block found */
    if (!best)
        return NULL;

    /* If the block is an exact match, mark it as used */
    if (best->size == size)
    {
        best->free = 0;
        return best->base;
    }

    /* Otherwise, split the block */
    newBlock = &gHeap.blocks[gHeap.count++];
    newBlock->free = 1;
    newBlock->size = best->size - size;
    newBlock->base = (void*)((u32)best->base + size);
    best->size = size;
    best->free = 0;
    return best->base;
}

static void removeBlock(int blockId)
{
    /* If we're removing the last block, just decrement the count */
    if (blockId == (gHeap.count - 1))
    {
        --gHeap.count;
        return;
    }

    /* Otherwise, move the last block to the removed block's position */
    memcpy(&gHeap.blocks[blockId], &gHeap.blocks[gHeap.count - 1], sizeof(HeapBlock));
    --gHeap.count;
}

static int mergeBlocks(int blockIdLeft, int blockIdRight)
{
    u32 size;
    void* base;
    int lowId;
    int highId;

    if (blockIdLeft < blockIdRight)
    {
        lowId = blockIdLeft;
        highId = blockIdRight;
    }
    else
    {
        lowId = blockIdRight;
        highId = blockIdLeft;
    }

    base = gHeap.blocks[blockIdLeft].base;
    size = gHeap.blocks[blockIdLeft].size + gHeap.blocks[blockIdRight].size;

    gHeap.blocks[lowId].base = base;
    gHeap.blocks[lowId].size = size;
    gHeap.blocks[lowId].free = 1;

    removeBlock(highId);

    return lowId;
}

void free(void* data)
{
    int blockId;
    int nextBlockId;

    /* Check for NULL */
    if (!data)
        return;

    /* Find the block */
    blockId = -1;
    for (int i = 0; i < gHeap.count; ++i)
    {
        if (gHeap.blocks[i].base == data)
        {
            blockId = i;
            break;
        }
    }

    /* Not found */
    if (blockId == -1)
        return;

    /* Mark the block as free */
    gHeap.blocks[blockId].free = 1;

    /* Merge right blocks */
    for (;;)
    {
        nextBlockId = -1;
        for (int i = 0; i < gHeap.count; ++i)
        {
            if (gHeap.blocks[i].base == (void*)((u32)gHeap.blocks[blockId].base + gHeap.blocks[blockId].size) && gHeap.blocks[i].free)
            {
                nextBlockId = i;
                break;
            }
        }

        if (nextBlockId == -1)
            break;

        blockId = mergeBlocks(blockId, nextBlockId);
    }

    /* Merge left blocks */
    for (;;)
    {
        nextBlockId = -1;
        for (int i = 0; i < gHeap.count; ++i)
        {
            if (gHeap.blocks[i].base == (void*)((u32)gHeap.blocks[blockId].base - gHeap.blocks[i].size) && gHeap.blocks[i].free)
            {
                nextBlockId = i;
                break;
            }
        }

        if (nextBlockId == -1)
            break;

        blockId = mergeBlocks(nextBlockId, blockId);
    }
}
