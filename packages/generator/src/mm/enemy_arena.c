#include <combo.h>
#include <combo/enemy_keep.h>
#include <combo/actor_ovl.h>

#define EnemyArenaNative ((void*)0x801f5100u)
#define EnemyArenaAlloc enemyArenaBestFit
static void* enemyArenaBestFit(void* arena, u32 size);
#define EnemyArenaAllocR ((void* (*)(void*, u32))0x80087408u)
#define EnemyArenaFree ((void (*)(void*, void*))0x800874ecu)
#define EnemyArenaGetSizes ((void (*)(void*, u32*, u32*, u32*))0x80087714u)
#define EnemyArenaRealloc ((void* (*)(void*, void*, u32))0x800875e4u)
#define EnemyArenaAddHeap ((void (*)(void*, void*, u32))0x8008725cu)
#define EnemyArenaCleanup ((void (*)(void*))0x800872fcu)

#define ENEMY_ARENA_CHUNK 0x10000u
#define ENEMY_ARENA_MIN_CHUNK 0x4000u
#define ENEMY_ARENA_MAX 0x30000u
#define ENEMY_CUSTOM_RESERVE 0x8000u
#define ENEMY_NATIVE_RESERVE 0x5000u
#define ENEMY_NATIVE_CONTIGUOUS 0x4100u
static u8 sEnemyToolsPrepared;
static u32 sEnemyOverlayTailSaved;
static void* sEnemyArenaChunks[ENEMY_ARENA_MAX / ENEMY_ARENA_MIN_CHUNK];
static u32 sEnemyArenaCount;
static u32 sEnemyArenaBytes;
EXPORT_SYMBOL(ENEMY_NATIVE_ARENA_MM_BYTES, sEnemyArenaBytes);

int comboEnemyArenaExpand(u32 size)
{
    if (!size || size > ENEMY_ARENA_MAX - 0x10 || sEnemyArenaCount >= ARRAY_COUNT(sEnemyArenaChunks))
        return 0;
    u32 required = (size + 0x1f) & ~0xfu;
    u32 remaining = ENEMY_ARENA_MAX - sEnemyArenaBytes;
    if (required > remaining)
        return 0;
    u32 capacity = MIN(MAX(ENEMY_ARENA_CHUNK, required), remaining);
    void* chunk;
    for (;;)
    {
        chunk = mallocWithReserve(capacity, ENEMY_CUSTOM_RESERVE);
        if (chunk || capacity == required)
            break;
        capacity = MAX(required, (capacity / 2) & ~0xfu);
    }
    if (!chunk)
        return 0;
    EnemyArenaAddHeap(EnemyArenaNative, chunk, capacity);
    sEnemyArenaChunks[sEnemyArenaCount++] = chunk;
    sEnemyArenaBytes += capacity;
    return 1;
}

static int enemyArenaPrepareTools(void)
{
    static const s16 tools[] = { ACTOR_EN_BOM, ACTOR_EN_ARROW, ACTOR_EN_M_THUNDER, ACTOR_EN_BOM_CHU, ACTOR_EN_CLEAR_TAG };
    if (sEnemyToolsPrepared)
        return 1;
    if (!gPlay)
        return 0;
    gActorOvl[ACTOR_EN_CLEAR_TAG].type = 2;
    for (u32 i = 0; i < ARRAY_COUNT(tools); ++i)
        if (!Actor_LoadOverlay(&gPlay->actorCtx, tools[i]))
            return 0;
    sEnemyToolsPrepared = 1;
    return 1;
}

static void* enemyArenaTryReserved(u32 size)
{
    u32 largest, freeBytes, allocated;
    void* reserve;
    void* ptr;

    EnemyArenaGetSizes(EnemyArenaNative, &largest, &freeBytes, &allocated);
    if (largest < ENEMY_NATIVE_CONTIGUOUS ||
        freeBytes < size + ENEMY_NATIVE_RESERVE + 0x20)
        return NULL;

    reserve = EnemyArenaAllocR(EnemyArenaNative, ENEMY_NATIVE_CONTIGUOUS);
    if (!reserve)
        return NULL;
    ptr = EnemyArenaAlloc(EnemyArenaNative, size);
    EnemyArenaFree(EnemyArenaNative, reserve);
    if (!ptr)
    {
        reserve = EnemyArenaAlloc(EnemyArenaNative, ENEMY_NATIVE_CONTIGUOUS);
        if (!reserve)
            return NULL;
        ptr = EnemyArenaAlloc(EnemyArenaNative, size);
        EnemyArenaFree(EnemyArenaNative, reserve);
    }
    if (ptr)
    {
        EnemyArenaGetSizes(EnemyArenaNative, &largest, &freeBytes, &allocated);
        if (largest < ENEMY_NATIVE_CONTIGUOUS || freeBytes < ENEMY_NATIVE_RESERVE)
        {
            EnemyArenaFree(EnemyArenaNative, ptr);
            return NULL;
        }
    }
    return ptr;
}

void* comboEnemyArenaAllocReserved(u32 size)
{
    u32 largest, freeBytes, allocated;
    void* ptr;

    if (!size || size > 0x800000u || !enemyArenaPrepareTools())
        return NULL;
    size = (size + 15) & ~15u;
    ptr = enemyArenaTryReserved(size);
    if (ptr)
        return ptr;

    EnemyArenaGetSizes(EnemyArenaNative, &largest, &freeBytes, &allocated);
    u32 required = size;
    if (largest < ENEMY_NATIVE_CONTIGUOUS)
        required += ENEMY_NATIVE_CONTIGUOUS + 0x10;
    if (freeBytes < size + ENEMY_NATIVE_RESERVE + 0x20)
        required = MAX(required, size + ENEMY_NATIVE_RESERVE + 0x20 - freeBytes);
    if (comboEnemyArenaExpand(required))
        return enemyArenaTryReserved(size);
    return NULL;
}

static void* enemyArenaAlloc(void* arena, u32 size)
{
    void* ptr;
    if (sEnemyToolsPrepared && size < ENEMY_NATIVE_CONTIGUOUS)
    {
        void* reserve = EnemyArenaAllocR(arena, ENEMY_NATIVE_CONTIGUOUS);
        if (reserve)
        {
            ptr = EnemyArenaAlloc(arena, size);
            if (!ptr && comboEnemyArenaExpand(size))
                ptr = EnemyArenaAlloc(arena, size);
            EnemyArenaFree(arena, reserve);
            if (ptr)
                return ptr;
        }
    }
    ptr = EnemyArenaAlloc(arena, size);
    if (!ptr && comboEnemyArenaExpand(size))
        ptr = EnemyArenaAlloc(arena, size);
    return ptr;
}

static void* enemyArenaAllocR(void* arena, u32 size)
{
    void* ptr = EnemyArenaAllocR(arena, size);
    if (!ptr && comboEnemyArenaExpand(size))
        ptr = EnemyArenaAllocR(arena, size);
    return ptr;
}

static void* enemyArenaRealloc(void* arena, void* old, u32 size)
{
    void* ptr = EnemyArenaRealloc(arena, old, size);
    if (!ptr && size && comboEnemyArenaExpand(size))
        ptr = EnemyArenaRealloc(arena, old, size);
    return ptr;
}

static void enemyArenaCleanup(void* arena)
{
    EnemyArenaCleanup(arena);
    while (sEnemyArenaCount)
        free(sEnemyArenaChunks[--sEnemyArenaCount]);
    sEnemyArenaBytes = 0;
    sEnemyToolsPrepared = 0;
    sEnemyOverlayTailSaved = 0;
}

PATCH_CALL(0x80102c70, enemyArenaAlloc);
PATCH_CALL(0x80102c98, enemyArenaAllocR);
PATCH_CALL(0x80102cc8, enemyArenaRealloc);
PATCH_CALL(0x80102d2c, enemyArenaAlloc);
PATCH_CALL(0x80102df8, enemyArenaCleanup);

typedef struct EnemyArenaNode {
    u16 magic, free;
    u32 size;
    struct EnemyArenaNode* next;
    struct EnemyArenaNode* prev;
} EnemyArenaNode;
_Static_assert(sizeof(EnemyArenaNode) == 16, "MM US arena node");
static void* enemyArenaBestFit(void* arena, u32 size)
{
    if (size > 0x800000u)
        return NULL;
    size = ALIGN16(size);
    OSMesgQueue* lock = (OSMesgQueue*)((u8*)arena + 8);
    EnemyArenaNode* best = NULL;
    osSendMesg(lock, NULL, OS_MESG_BLOCK);
    for (EnemyArenaNode* node = *(EnemyArenaNode**)arena; node; node = node->next)
    {
        if (node->free && node->size >= size && (!best || node->size < best->size))
        {
            best = node;
            if (node->size == size)
                break;
        }
    }
    if (best)
    {
        if (size + sizeof(*best) < best->size)
        {
            EnemyArenaNode* tail = (EnemyArenaNode*)((u8*)(best + 1) + size);
            *tail = (EnemyArenaNode){ 0x7373, 1, best->size - size - sizeof(*tail), best->next, best };
            if (tail->next)
                tail->next->prev = tail;
            best->next = tail;
            best->size = size;
        }
        best->free = 0;
    }
    osRecvMesg(lock, NULL, OS_MESG_BLOCK);
    return best ? best + 1 : NULL;
}

static u32 enemyArenaTrim(void* data, u32 retained)
{
    EnemyArenaNode* node = (EnemyArenaNode*)data - 1;
    OSMesgQueue* lock = (OSMesgQueue*)((u8*)EnemyArenaNative + 8);
    u32 released = 0;
    EnemyArenaNode* tail = NULL;
    osSendMesg(lock, NULL, OS_MESG_BLOCK);
    if (node->magic == 0x7373 && !node->free && retained + 32 <= node->size)
    {
        released = node->size - retained;
        tail = (EnemyArenaNode*)((u8*)data + retained);
        *tail = (EnemyArenaNode){ 0x7373, 0, released - sizeof(*tail), node->next, node };
        if (tail->next)
            tail->next->prev = tail;
        node->next = tail;
        node->size = retained;
    }
    osRecvMesg(lock, NULL, OS_MESG_BLOCK);
    if (tail)
        EnemyArenaFree(EnemyArenaNative, tail + 1);
    return released;
}

EXPORT_SYMBOL(ENEMY_OVERLAY_TAIL_SAVED_MM, sEnemyOverlayTailSaved);
void comboEnemyArenaTrimOverlay(void* data, u32 fileSize, u32 memorySize)
{
    if (!data || fileSize < 24 || (fileSize & 15) || memorySize != fileSize ||
        (gPlay && *(void**)((u8*)&gPlay->actorCtx + 0x250) == data))
        return;
    u32 tail = *(u32*)((u8*)data + fileSize - 4);
    if (tail < 24 || tail >= fileSize || (tail & 3))
        return;
    const u32* reloc = (const u32*)((u8*)data + fileSize - tail);
    u32 retained = fileSize - tail;
    if (reloc[3] || reloc[0] > retained || reloc[1] > retained - reloc[0] ||
        reloc[2] != retained - reloc[0] - reloc[1] ||
        reloc[4] > (tail - 24) / 4 || !retained)
        return;
    retained = ALIGN16(retained);
    sEnemyOverlayTailSaved += enemyArenaTrim(data, retained);
}

static u32 enemyActorOverlayLoad(u32 vromStart, u32 vromEnd, void* vramStart,
                                 void* vramEnd, void* data)
{
    u32 result = ((u32(*)(u32, u32, void*, void*, void*))0x8008501c)(
        vromStart, vromEnd, vramStart, vramEnd, data);
    comboEnemyArenaTrimOverlay(data, vromEnd - vromStart, (u32)vramEnd - (u32)vramStart);
    return result;
}
PATCH_CALL(0x800badc8, enemyActorOverlayLoad);
