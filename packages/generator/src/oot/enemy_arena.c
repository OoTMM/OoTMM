#include <combo.h>
#include <combo/actor_ovl.h>
#include <combo/oot_gameplay_memory.h>

typedef struct EnemyArenaNode EnemyArenaNode;
struct EnemyArenaNode {
    u16 magic;
    u16 isFree;
    u32 size;
    EnemyArenaNode* next;
    EnemyArenaNode* previous;
    const char* file;
    s32 line;
    u32 thread;
    void* owner;
    u64 time;
    u8 padding[8];
};
typedef struct {
    EnemyArenaNode* head;
    void* start;
    u32 size;
    u32 failures;
} EnemyArena;
_Static_assert(sizeof(EnemyArenaNode) == 48, "Native OoT arena node");
_Static_assert(sizeof(EnemyArena) == 16, "Native OoT arena");

#ifndef EnemyArenaExtensionBase
#define EnemyArenaExtensionBase 0x80480000u
#endif
#define ENEMY_ARENA_EXTENSION_SIZE OOT_GAMEPLAY_ARENA_BYTES
_Static_assert(0x80480000u + ENEMY_ARENA_EXTENSION_SIZE == OOT_GAMEPLAY_CUSTOM_BASE, "OoT arena and gameplay pool boundary");
_Static_assert(OOT_GAMEPLAY_CUSTOM_BASE + OOT_GAMEPLAY_CUSTOM_RESERVE <= 0x80600000u, "OoT audio heap boundary");
#ifndef EnemyGameplayEffects
#define EnemyGameplayEffects ((const u32 (*)[7])0x800e7c40u)
#endif
#ifndef EnemyArenaNative
#define EnemyArenaNative ((EnemyArena*)0x8011bef0u)
#endif
#ifndef EnemyArenaOriginalInit
#define EnemyArenaOriginalInit ((void (*)(void*, u32))0x80066d6cu)
#endif
#ifndef EnemyArenaBlockInit
#define EnemyArenaBlockInit ((void (*)(EnemyArena*, void*, s32))0x800cdd90u)
#endif
#ifndef EnemyArenaFramebufferEnd
#define EnemyArenaFramebufferEnd ((void* (*)(void))0x800a4400u)
#endif
#ifndef EnemyArenaPayloadEnd
extern u8 __combo_payload_end[];
#define EnemyArenaPayloadEnd ((u32)__combo_payload_end)
#endif
#ifndef EnemyArenaMemorySize
#define EnemyArenaMemorySize (*(volatile u32*)0x80000318u)
#endif

static u32 sEnemyArenaEnabled __attribute__((section(".data"))) = 0;
EXPORT_SYMBOL(ENEMY_NATIVE_ARENA_OOT_ENABLE, sEnemyArenaEnabled);
static u32 sEnemyArenaBytes;
EXPORT_SYMBOL(ENEMY_NATIVE_ARENA_OOT_BYTES, sEnemyArenaBytes);

static void enemyArenaInit(void* start, u32 size)
{
    EnemyArena* arena = EnemyArenaNative;
    EnemyArena extra;
    EnemyArenaNode* tail;
    EnemyArenaNode* previous = NULL;
    EnemyArenaNode* barrier = (EnemyArenaNode*)EnemyArenaExtensionBase;
    u32 begin, end;
    sEnemyArenaBytes = 0;
    EnemyArenaOriginalInit(start, size);
    if (sEnemyArenaEnabled != 1 || EnemyArenaMemorySize < 0x800000u ||
        (u32)EnemyArenaFramebufferEnd() > 0x80400000u || EnemyArenaPayloadEnd > 0x80480000u)
        return;
    begin = (u32)arena->start;
    if (!begin || size > 0x400000u || begin >= 0x80400000u ||
        arena->size > 0x80400000u - begin)
        return;
    end = begin + arena->size;
    tail = arena->head;
    for (u32 count = 0;; ++count)
    {
        u32 address = (u32)tail;
        if (count >= 4096 || !tail || address < begin || address > end ||
            sizeof(*tail) > end - address || tail->magic != 0x7373 ||
            tail->previous != previous || tail->size > end - address - sizeof(*tail))
            return;
        if (!tail->next)
            break;
        previous = tail;
        tail = tail->next;
    }
    memset(barrier, 0, sizeof(*barrier));
    EnemyArenaBlockInit(&extra, barrier + 1, ENEMY_ARENA_EXTENSION_SIZE - sizeof(*barrier));
    if (extra.head != barrier + 1 || extra.head->magic != 0x7373 || !extra.head->isFree ||
        extra.head->size != ENEMY_ARENA_EXTENSION_SIZE - 2 * sizeof(*barrier))
        return;
    barrier->magic = 0x7373;
    barrier->previous = tail;
    barrier->next = extra.head;
    barrier->owner = arena;
    extra.head->previous = barrier;
    tail->next = barrier;
    arena->size += ENEMY_ARENA_EXTENSION_SIZE;
    sEnemyArenaBytes = ENEMY_ARENA_EXTENSION_SIZE;
}
PATCH_CALL(0x8009ade8, enemyArenaInit);
ALIGNED(16) static const u32 kEnemyArenaOotV1[] = {
    0x45415231,
    2,
    EnemyArenaExtensionBase,
    ENEMY_ARENA_EXTENSION_SIZE,
    0x8009ade8,
    (u32)enemyArenaInit,
};
EXPORT_SYMBOL(ENEMY_NATIVE_ARENA_OOT_V1, kEnemyArenaOotV1);

int comboEnemyOotHeadroomEnabled(void)
{
    return sEnemyArenaEnabled == 1;
}

u32 comboEnemyOotGameplayReserve(void)
{
    static const u16 sizes[][2] = {
#include "gameplay_actor_sizes.inc"
    };
    const u32 (*effects)[7] = EnemyGameplayEffects;
    u32 used = 0;
    if (!comboEnemyOotHeadroomEnabled())
        return OOT_GAMEPLAY_CONTIGUOUS_RESERVE;
    for (u32 i = 0; i < ARRAY_COUNT(sizes); ++i)
    {
        ActorOvl* ovl = &gActorOvl[sizes[i][0]];
        if (ovl->data && ovl->vramStart && !(ovl->type & 1))
            used += ((ovl->vramEnd - ovl->vramStart + 15) & ~15u) + 48;
    }
    if (gPlay)
    {
        if (gPlay->actorCtx.absoluteSpace)
            used += 0x24e0 + 48;
        for (u32 category = 0; category < 12; ++category)
        {
            Actor* actor = gPlay->actorCtx.actors[category].first;
            for (u32 n = 0; actor && n < 201; ++n, actor = actor->next)
            {
                u32 low = 0, high = ARRAY_COUNT(sizes);
                while (low < high)
                {
                    u32 mid = (low + high) / 2;
                    if ((u16)actor->id > sizes[mid][0])
                        low = mid + 1;
                    else
                        high = mid;
                }
                if (low < ARRAY_COUNT(sizes) && actor->id == sizes[low][0])
                    used += ((sizes[low][1] + 15) & ~15u) + 48;
            }
        }
    }
    for (u32 i = 0; i < 37; ++i)
        if (effects[i][4] && effects[i][2])
            used += ((effects[i][3] - effects[i][2] + 15) & ~15u) + 48;
    if (used >= OOT_GAMEPLAY_NATIVE_RESERVE - OOT_GAMEPLAY_FRAGMENT_RESERVE)
        return OOT_GAMEPLAY_FRAGMENT_RESERVE;
    return OOT_GAMEPLAY_NATIVE_RESERVE - used;
}

ALIGNED(16) static const u32 kGameplayReserveOotV1[] = {
    0x47504d31,
    1,
    OOT_GAMEPLAY_NATIVE_RESERVE,
    OOT_GAMEPLAY_CUSTOM_RESERVE,
    OOT_GAMEPLAY_FRAGMENT_RESERVE,
    OOT_GAMEPLAY_CONTIGUOUS_RESERVE,
    OOT_GAMEPLAY_ACTOR_SLOTS,
    OOT_GAMEPLAY_CUSTOM_BASE,
};
EXPORT_SYMBOL(ENEMY_GAMEPLAY_RESERVE_OOT_V1, kGameplayReserveOotV1);
