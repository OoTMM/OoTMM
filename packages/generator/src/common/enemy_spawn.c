#include <combo/enemy_soul_preview.h>
#if defined(GAME_OOT)
#include <combo/oot_gameplay_memory.h>
#endif
#include <combo.h>
#include <combo/enemy.h>
#include <combo/enemy_ground.h>
#include <combo/enemy_placement.h>
#include <combo/enemy_keep.h>
#include <combo/enemy_room_models.h>
#include <combo/actor_ovl.h>
#include <combo/dma.h>
#include <combo/global.h>
#include <combo/souls.h>
#include <combo/enemy_native.h>

typedef struct
{
    ActorFunc init;
    ActorFunc fini;
    ActorFunc update;
    ActorFunc draw;
#if defined(GAME_MM)
    ForeignEnemyAudioState audio;
#endif
    EnemyDefeatTransition defeat;
    u8 hint;
    u8 live;
    u16 flags;
    s8 ownerRoom;
#if defined(DEBUG)
    u8 debugSpawn;
    u8 debugWater;
    f32 debugWaterSurface;
#endif
} ForeignEnemyInstance;

#if defined(DEBUG)
extern int gDebugEnemySpawn;
extern int gDebugEnemyWater;
extern f32 gDebugEnemyWaterSurface;
void comboDebugEnemyRunInWater(ActorFunc, Actor*, PlayState*, f32);
#endif

static Actor* sCurrentEnemyActor;
static u32 sCurrentEnemyCallback = ~0u;
Actor* comboEnemyCurrentActor(void)
{
    return sCurrentEnemyActor;
}

u32 comboEnemyCurrentCallback(void)
{
    return sCurrentEnemyCallback;
}

static ActorOvl sForeignEnemyOverlays[32];
static u16 sForeignEnemyIds[32];
static u32 sForeignEnemyFlags[32];
u32 gForeignEnemyRegistryVrom __attribute__((section(".data"))) = 0;
#if defined(GAME_OOT)
EXPORT_SYMBOL(FOREIGN_ENEMY_REGISTRY_OOT, gForeignEnemyRegistryVrom);
#else
EXPORT_SYMBOL(FOREIGN_ENEMY_REGISTRY_MM, gForeignEnemyRegistryVrom);
#endif

static u32 sSpawnFlags;

void comboEnemyOverlayReset(void)
{
    memset(sForeignEnemyOverlays, 0, sizeof(sForeignEnemyOverlays));
    memset(sForeignEnemyIds, 0, sizeof(sForeignEnemyIds));
    memset(sForeignEnemyFlags, 0, sizeof(sForeignEnemyFlags));
    sCurrentEnemyActor = NULL;
    sCurrentEnemyCallback = ~0u;
    sSpawnFlags = 0;
}

#if defined(GAME_OOT)
# define ENEMY_NATIVE_GAME 0u
# define EnemyArenaMalloc ((void* (*)(u32))0x80066c10)
# define EnemyArenaFree ((void (*)(void*))0x80066c90)
# define EnemyActorInit ((void (*)(Actor*, PlayState*))0x80020fdc)
# define EnemyActorAdd ((void (*)(ActorContext*, Actor*, u8))0x80024f98)
#else
# define ENEMY_NATIVE_GAME 1u
# define EnemyArenaMalloc ((void* (*)(u32))0x80102c60)
# define EnemyArenaFree ((void (*)(void*))0x80102ce0)
# define EnemyActorInit ((void (*)(Actor*, PlayState*))0x800b6834)
# define EnemyActorAdd ((void (*)(ActorContext*, Actor*, u8))0x800baab4)
#endif

void* comboEnemyArenaAlloc(u32 size)
{
#if defined(GAME_MM)
    return comboEnemyArenaAllocReserved(size);
#else
    void (*getSizes)(u32*, u32*, u32*) = (void*)0x80066d10;
    u32 largest, freeBytes, allocated;
    const u32 nativeSpawnReserve = comboEnemyOotGameplayReserve();
    void* allocation;

    if (size > 0x800000)
        return NULL;
    const u32 charge = ((size + 15) & ~15u) + 48;
    getSizes(&largest, &freeBytes, &allocated);
    if (charge > largest || charge > freeBytes || freeBytes - charge < nativeSpawnReserve)
        return NULL;

    allocation = EnemyArenaMalloc(size);
    if (allocation)
    {
        getSizes(&largest, &freeBytes, &allocated);
        if (largest < OOT_GAMEPLAY_CONTIGUOUS_RESERVE || freeBytes < nativeSpawnReserve)
        {
            EnemyArenaFree(allocation);
            return NULL;
        }
    }
    return allocation;
#endif
}

int comboIsForeignEnemyActor(s16 id)
{
    return id >= FOREIGN_ENEMY_ACTOR_BASE && id < FOREIGN_ENEMY_ACTOR_BASE + FOREIGN_ENEMY_ACTOR_COUNT;
}

static int enemyMetadataFile(u32 vrom, DmaEntry* entry)
{
    return vrom >= 0x08000000 && comboDmaLookup(entry, vrom) && entry->vstart == vrom &&
           entry->vend > entry->vstart && entry->pend == 0;
}

static int enemyLookupRegistration(s16 id, ForeignEnemyRegistration* out)
{
    DmaEntry entry;
    u32 header[4];
    u32 low;
    u32 high;
    u32 mid;
    u32 stride;

    if (!gForeignEnemyRegistryVrom || !enemyMetadataFile(gForeignEnemyRegistryVrom, &entry) ||
        entry.vend - entry.vstart < sizeof(header))
        return 0;
    if (comboDmaLoadFilePartial(header, gForeignEnemyRegistryVrom, 0, sizeof(header)) != sizeof(header) ||
        header[0] != 0x454e4d59 || (header[1] != 1 && header[1] != 2) || header[2] != ENEMY_NATIVE_GAME ||
        header[3] > FOREIGN_ENEMY_ACTOR_COUNT)
        return 0;
    stride = header[1] == 1 ? 12 : sizeof(*out);
    memset(out, 0, sizeof(*out));
    if (entry.vend - entry.vstart != sizeof(header) + header[3] * stride)
        return 0;
    low = 0;
    high = header[3];
    /* Batch the final candidates; this lookup also runs for repeat spawns. */
    enum { REGISTRATION_WINDOW = 16 };
    ALIGNED(16) u8 window[REGISTRATION_WINDOW * sizeof(*out)];
    u32 windowFirst = 0, windowCount = 0;
    while (low < high)
    {
        mid = low + (high - low) / 2;
        if (mid < windowFirst || mid - windowFirst >= windowCount)
        {
            windowFirst = mid;
            windowCount = 1;
            if (high - low <= REGISTRATION_WINDOW)
            {
                windowFirst = low;
                windowCount = high - low;
            }
            u32 bytes = windowCount * stride;
            if (comboDmaLoadFilePartial(window, gForeignEnemyRegistryVrom, sizeof(header) + windowFirst * stride, bytes) != bytes)
                return 0;
        }
        memcpy(out, window + (mid - windowFirst) * stride, stride);
        if (out->actorId == id)
            return !(out->flags & ~(FOREIGN_ENEMY_ROOT | FOREIGN_ENEMY_PART | FOREIGN_ENEMY_KEEP_MAIN | FOREIGN_ENEMY_KEEP_FIELD | FOREIGN_ENEMY_KEEP_DUNGEON | FOREIGN_ENEMY_SOURCE_DAMAGE | FOREIGN_ENEMY_SOURCE_AUDIO | FOREIGN_ENEMY_NATIVE_PORT | FOREIGN_ENEMY_GROUND_BOUND | FOREIGN_ENEMY_AIR_BOUND | FOREIGN_ENEMY_HOST_SOUL)) &&
                   (out->flags & FOREIGN_ENEMY_NATIVE_PORT) &&
                   (!(out->flags & FOREIGN_ENEMY_SOURCE_AUDIO) || ENEMY_NATIVE_GAME != 0) &&
                   (out->flags & (FOREIGN_ENEMY_KEEP_FIELD | FOREIGN_ENEMY_KEEP_DUNGEON)) != (FOREIGN_ENEMY_KEEP_FIELD | FOREIGN_ENEMY_KEEP_DUNGEON);
        if (out->actorId < id)
            low = mid + 1;
        else
            high = mid;
    }
    return 0;
}

static ActorOvl* enemyOverlaySlot(s16 id)
{
    ActorOvl* available = NULL;
    for (u32 i = 0; i < ARRAY_COUNT(sForeignEnemyOverlays); ++i)
    {
        ActorOvl* entry = &sForeignEnemyOverlays[i];
        if (sForeignEnemyIds[i] == id && entry->data)
            return entry;
        if (!available && !entry->data && !entry->count)
            available = entry;
    }
    if (available)
    {
        memset(available, 0, sizeof(*available));
        sForeignEnemyIds[available - sForeignEnemyOverlays] = id;
    }
    return available;
}

static ActorProfile* enemyLoadProfile(ActorOvl* entry, u32 manifestVrom, int* allocationFailed)
{
    DmaEntry dma;
    ForeignEnemyOverlay manifest;
    ActorProfile* profile;
    u32 capacity;
    void* code;

    *allocationFailed = 0;
    if (entry->data)
        return (ActorProfile*)((u8*)entry->data + (u32)entry->dataInit - entry->vramStart);
    if (!enemyMetadataFile(manifestVrom, &dma) || dma.vend - dma.vstart < sizeof(manifest))
        return NULL;
    if (comboDmaLoadFilePartial(&manifest, manifestVrom, 0, sizeof(manifest)) != sizeof(manifest) ||
        manifest.reserved || manifest.fileSize < sizeof(ActorProfile) || (manifest.fileSize & 3) ||
        manifest.fileSize > 0x800000 || manifest.memorySize < sizeof(ActorProfile) || manifest.memorySize > 0x800000 ||
        manifest.profileOffset > manifest.fileSize - sizeof(ActorProfile) || (manifest.profileOffset & 3))
        return NULL;
    if (sizeof(manifest) != dma.vend - dma.vstart)
        return NULL;
    capacity = MAX(manifest.fileSize, manifest.memorySize);
    code = comboEnemyArenaAlloc(capacity);
    if (!code)
    {
        *allocationFailed = 1;
        return NULL;
    }
    profile = comboLoadForeignEnemyOverlay(code, capacity, &manifest);
    if (!profile)
    {
        EnemyArenaFree(code);
        return NULL;
    }
#if defined(GAME_MM)
    comboEnemyArenaTrimOverlay(code, manifest.fileSize, manifest.memorySize);
#endif
    entry->vromStart = manifestVrom;
    entry->vromEnd = dma.vend;
    entry->vramStart = manifest.vramStart;
    entry->vramEnd = manifest.vramStart + manifest.memorySize;
    entry->dataInit = (void*)(manifest.vramStart + manifest.profileOffset);
    entry->data = code;
    return profile;
}

static void enemyReleaseUnused(ActorOvl* entry)
{
    if (entry && !entry->count && entry->data)
    {
        EnemyArenaFree(entry->data);
        entry->data = NULL;
    }
}

static void enemyActorInit(Actor* actor, PlayState* play);
static void enemyActorDestroy(Actor* actor, PlayState* play);
static void enemyActorUpdate(Actor* actor, PlayState* play);
static void enemyActorDraw(Actor* actor, PlayState* play);

static ForeignEnemyInstance* enemyInstance(Actor* actor, const ActorProfile* profile)
{
    return (ForeignEnemyInstance*)((u8*)actor + ((profile->instanceSize + 15) & ~15));
}

u16 comboEnemySourceKeepObject(u32 segment)
{
    Actor* actor = sCurrentEnemyActor;
    if (!actor || (segment != 4 && segment != 5))
        return 0;
    ActorOvl* entry = actor->ovl;
    if ((u32)entry < (u32)sForeignEnemyOverlays || (u32)entry >= (u32)(sForeignEnemyOverlays + ARRAY_COUNT(sForeignEnemyOverlays)) ||
        ((u32)entry - (u32)sForeignEnemyOverlays) % sizeof(*entry) || !entry->data)
        return 0;
    u32 flags = sForeignEnemyFlags[entry - sForeignEnemyOverlays];
    if (!(flags & FOREIGN_ENEMY_NATIVE_PORT))
        return 0;
    if (segment == 4)
        return 0x1001;
    if (flags & FOREIGN_ENEMY_KEEP_FIELD)
        return 0x1002;
    if (flags & FOREIGN_ENEMY_KEEP_DUNGEON)
        return 0x1003;
    return 0;
}

static int enemyRoomModelsLive(s32 room, u16 currentHalfDay)
{
    if (!gPlay || room < 0 || room >= 32)
        return 0;
    for (u32 category = 0; category < 12; ++category)
        for (Actor* actor = gPlay->actorCtx.actors[category].first; actor; actor = actor->next)
        {
            if (currentHalfDay)
            {
                u16 days = comboEnemySourceHalfDays(actor);
                if (!days || (days & currentHalfDay))
                    continue;
                if (!comboIsForeignEnemyActor(actor->id) && actor->room == room)
                    return 1;
            }
            if (!comboIsForeignEnemyActor(actor->id))
                continue;
            ActorOvl* entry = actor->ovl;
            if ((u32)entry < (u32)sForeignEnemyOverlays || (u32)entry >= (u32)(sForeignEnemyOverlays + ARRAY_COUNT(sForeignEnemyOverlays)) ||
                ((u32)entry - (u32)sForeignEnemyOverlays) % sizeof(*entry) || !entry->data || !entry->count)
                continue;
            if (sForeignEnemyIds[entry - sForeignEnemyOverlays] != actor->id)
                continue;
            ActorProfile* profile = (ActorProfile*)((u8*)entry->data + (u32)entry->dataInit - entry->vramStart);
            ForeignEnemyInstance* instance = enemyInstance(actor, profile);
            if (instance->live && instance->ownerRoom == room)
                return 1;
        }
    return 0;
}

int comboEnemyRoomModelsLive(s32 room)
{
    return enemyRoomModelsLive(room, 0);
}

int comboEnemyRoomHasRetiringActors(s32 room, u16 currentHalfDay)
{
    return currentHalfDay && enemyRoomModelsLive(room, currentHalfDay);
}

void* comboEnemyResolveKeepPointer(void* ptr)
{
    u16 objectId = comboEnemySourceKeepObject((u32)ptr >> 24);
    if (objectId)
    {
        void* asset = comboEnemyKeepAsset(objectId, (u32)ptr & 0xffffff);
        static const Gfx empty[] = { gsSPEndDisplayList() };
        if (asset)
            return asset;
        if (sCurrentEnemyCallback != 3 && sCurrentEnemyActor)
        {
            g.roomEnemyLackSoul = 1;
            Actor_Kill(sCurrentEnemyActor);
        }
        return (void*)empty;
    }
    return NULL;
}

int comboEnemyGroundBound(Actor* actor)
{
    if (!actor || !comboIsForeignEnemyActor(actor->id))
        return 0;
    ActorOvl* entry = actor->ovl;
    if ((u32)entry < (u32)sForeignEnemyOverlays || (u32)entry >= (u32)(sForeignEnemyOverlays + ARRAY_COUNT(sForeignEnemyOverlays)) ||
        ((u32)entry - (u32)sForeignEnemyOverlays) % sizeof(*entry) || !entry->data || !entry->count)
        return 0;
    u32 index = entry - sForeignEnemyOverlays;
    if (sForeignEnemyIds[index] != actor->id || !(sForeignEnemyFlags[index] & FOREIGN_ENEMY_GROUND_BOUND))
        return 0;
    ActorProfile* profile = (ActorProfile*)((u8*)entry->data + (u32)entry->dataInit - entry->vramStart);
    ForeignEnemyInstance* instance = enemyInstance(actor, profile);
    return instance->live && ((instance->flags & FOREIGN_ENEMY_ROOT) || actor->id == 0xce0) &&
           (actor == sCurrentEnemyActor || actor->update == enemyActorUpdate) &&
           (actor->colChkInfo.health || actor->id == 0xce0);
}

int comboEnemyAirBound(Actor* actor)
{
    if (!actor)
        return 0;
    if (!comboIsForeignEnemyActor(actor->id))
        return comboEnemyNativeAirBound(actor);
    ActorOvl* entry = actor->ovl;
    if ((u32)entry < (u32)sForeignEnemyOverlays || (u32)entry >= (u32)(sForeignEnemyOverlays + ARRAY_COUNT(sForeignEnemyOverlays)) ||
        ((u32)entry - (u32)sForeignEnemyOverlays) % sizeof(*entry) || !entry->data || !entry->count)
        return 0;
    u32 index = entry - sForeignEnemyOverlays;
    if (sForeignEnemyIds[index] != actor->id || !(sForeignEnemyFlags[index] & FOREIGN_ENEMY_AIR_BOUND))
        return 0;
    ActorProfile* profile = (ActorProfile*)((u8*)entry->data + (u32)entry->dataInit - entry->vramStart);
    ForeignEnemyInstance* instance = enemyInstance(actor, profile);
    return instance->live && (instance->flags & FOREIGN_ENEMY_ROOT) &&
           (actor == sCurrentEnemyActor || actor->update == enemyActorUpdate) &&
           actor->colChkInfo.health;
}

int comboEnemyUsesSourceDamage(Actor* actor)
{
    u32 address;
    ActorOvl* entry;
    if (!actor)
        return 0;
    entry = actor->ovl;
    address = (u32)entry;
    if (address < (u32)sForeignEnemyOverlays || address >= (u32)(sForeignEnemyOverlays + ARRAY_COUNT(sForeignEnemyOverlays)) ||
        (address - (u32)sForeignEnemyOverlays) % sizeof(*entry) || !entry->data)
        return 0;
    return !!(sForeignEnemyFlags[entry - sForeignEnemyOverlays] & FOREIGN_ENEMY_SOURCE_DAMAGE);
}

#if defined(GAME_OOT)
int comboEnemyUsesNativePort(Actor* actor)
{
    ActorOvl* entry;
    u32 address;
    if (!actor)
        return 0;
    entry = actor->ovl;
    address = (u32)entry;
    if (address < (u32)sForeignEnemyOverlays || address >= (u32)(sForeignEnemyOverlays + ARRAY_COUNT(sForeignEnemyOverlays)) ||
        (address - (u32)sForeignEnemyOverlays) % sizeof(*entry) || !entry->data)
        return 0;
    return !!(sForeignEnemyFlags[entry - sForeignEnemyOverlays] & FOREIGN_ENEMY_NATIVE_PORT);
}
#endif

#if defined(GAME_MM)
ForeignEnemyAudioState* comboEnemySourceAudioState(Actor* actor)
{
    ActorOvl* entry;
    ActorProfile* profile;
    u32 address, flags;
    if (!actor)
        return NULL;
    entry = actor->ovl;
    address = (u32)entry;
    if (address < (u32)sForeignEnemyOverlays || address >= (u32)(sForeignEnemyOverlays + ARRAY_COUNT(sForeignEnemyOverlays)) ||
        (address - (u32)sForeignEnemyOverlays) % sizeof(*entry) || !entry->data)
        return NULL;
    flags = sForeignEnemyFlags[entry - sForeignEnemyOverlays];
    if (!(flags & FOREIGN_ENEMY_SOURCE_AUDIO))
        return NULL;
    profile = (ActorProfile*)((u8*)entry->data + (u32)entry->dataInit - entry->vramStart);
    return &enemyInstance(actor, profile)->audio;
}

#endif

u32 comboEnemySourceHalfDays(Actor* actor)
{
#if defined(GAME_MM)
    return actor ? (u16)actor->halfDaysBits : 0x3ff;
#else
    (void)actor;
    return 0x3ff;
#endif
}

static void enemyWrapCallbacks(Actor* actor, ForeignEnemyInstance* instance)
{
    actor->init = instance->init ? enemyActorInit : NULL;
    actor->fini = enemyActorDestroy;
    actor->update = instance->update ? enemyActorUpdate : NULL;
    actor->draw = instance->draw ? enemyActorDraw : NULL;
}

static void enemyActorCallback(Actor* actor, PlayState* play, u32 callback)
{
    ActorOvl* entry = actor->ovl;
    ActorProfile* profile;
    ActorFunc function;
    ForeignEnemyInstance* instance;
    u32 flags;
    u32 saved[2];
    Actor* previousActor;
    u32 previousCallback;
#if defined(GAME_MM)
    u32 nativeAudioFlags = 0;
#endif
    int inDefeatAction;

    if ((u32)entry < (u32)sForeignEnemyOverlays || (u32)entry >= (u32)(sForeignEnemyOverlays + ARRAY_COUNT(sForeignEnemyOverlays)) ||
        ((u32)entry - (u32)sForeignEnemyOverlays) % sizeof(*entry) || !entry->data)
    {

        return;
    }
    profile = (ActorProfile*)((u8*)entry->data + (u32)entry->dataInit - entry->vramStart);
    instance = enemyInstance(actor, profile);
    if (callback == 1)
        instance->live = 0;
    if (actor->init != enemyActorInit)
        instance->init = actor->init;
    if (actor->fini != enemyActorDestroy)
        instance->fini = actor->fini;
    if (actor->update != enemyActorUpdate)
        instance->update = actor->update;
    if (actor->draw != enemyActorDraw)
        instance->draw = actor->draw;
    switch (callback)
    {
    case 0: function = instance->init; break;
    case 1: function = instance->fini; break;
    case 2: function = instance->update; break;
    default: function = instance->draw; break;
    }
    if (!function)
    {
        if (callback == 1)
        {
            comboEnemyCollisionRelease(actor);
            comboEnemyEncounterRelease(actor);
        }
        return;
    }
    flags = instance->flags;
    saved[0] = gSegments[4];
    saved[1] = gSegments[5];
    actor->init = instance->init;
    actor->fini = instance->fini;
    actor->update = instance->update;
    actor->draw = instance->draw;
    previousActor = sCurrentEnemyActor;
    previousCallback = sCurrentEnemyCallback;
    sCurrentEnemyActor = actor;
    sCurrentEnemyCallback = callback;
#if defined(GAME_MM)
    if (flags & FOREIGN_ENEMY_SOURCE_AUDIO)
    {
        nativeAudioFlags = comboEnemySfxEnter(actor, &instance->audio, callback);
    }
#endif
    comboEnemyHintEnter(actor, instance->hint);
    comboEnemyCollisionEnter(actor);
    inDefeatAction = callback == 2 && comboEnemyDefeatState(actor, &instance->defeat, entry->data, instance->defeat.from);
    EnemyGroundGuard ground;
    if (callback == 0 && (flags & FOREIGN_ENEMY_AIR_BOUND))
        comboEnemyAirPlacement(play, actor, 40.f);
    if (callback == 2)
        comboEnemyGroundGuardBegin(play, actor, &ground);
#if defined(DEBUG)
    int previousDebugSpawn = gDebugEnemySpawn;
    int previousWater = gDebugEnemyWater;
    f32 previousSurface = gDebugEnemyWaterSurface;
    int previousLackSoul = g.roomEnemyLackSoul;
    if (instance->debugSpawn)
    {
        gDebugEnemySpawn = 1;
        gDebugEnemyWater = instance->debugWater;
        gDebugEnemyWaterSurface = instance->debugWaterSurface;
    }
    if (instance->debugWater)
        comboDebugEnemyRunInWater(function, actor, play, instance->debugWaterSurface);
    else
#endif
        function(actor, play);
#if defined(DEBUG)
    gDebugEnemySpawn = previousDebugSpawn;
    gDebugEnemyWater = previousWater;
    gDebugEnemyWaterSurface = previousSurface;
    if (instance->debugSpawn)
        g.roomEnemyLackSoul = previousLackSoul;
#endif
    if (callback == 2)
        comboEnemyGroundGuardEnd(play, &ground);
    comboEnemyCollisionLeave(actor);
    instance->hint = comboEnemyHintLeave(actor);
#if defined(GAME_MM)
    if (flags & FOREIGN_ENEMY_SOURCE_AUDIO)
        comboEnemySfxLeave(actor, &instance->audio, nativeAudioFlags);
#endif
    if (callback == 2)
        comboEnemyEncounterAfterUpdate(actor, play, &instance->defeat, entry->data, inDefeatAction);
    sCurrentEnemyActor = previousActor;
    sCurrentEnemyCallback = previousCallback;
    instance->init = actor->init;
    instance->fini = actor->fini;
    instance->update = actor->update;
    instance->draw = actor->draw;
    enemyWrapCallbacks(actor, instance);
    gSegments[4] = saved[0];
    gSegments[5] = saved[1];
    if (callback == 1)
    {
        comboEnemyCollisionRelease(actor);
        comboEnemyEncounterRelease(actor);
    }
    if (callback == 0 && !actor->update && (flags & FOREIGN_ENEMY_ROOT))
        g.roomEnemyLackSoul = 1;
}

static void enemyActorInit(Actor* actor, PlayState* play)
{
    enemyActorCallback(actor, play, 0);
}

static void enemyActorDestroy(Actor* actor, PlayState* play)
{
    enemyActorCallback(actor, play, 1);
}

static void enemyActorUpdate(Actor* actor, PlayState* play)
{
    enemyActorCallback(actor, play, 2);
}

static void enemyActorDraw(Actor* actor, PlayState* play)
{
    enemyActorCallback(actor, play, 3);
}

#if defined(GAME_OOT)
static int enemyHiploopFragment(ActorContext* ctx, PlayState* play, Actor* parent, u16 params, const EnemyPlacement* split)
{
    Actor* owner = comboEnemyCurrentActor();
    int fragment = params == 8 || params == 10 || params == 13 || params == 17 || params == 23 || params == 26;
    /* The native port spawns its cosmetic death pieces in Update. */
    return fragment && !split && !parent && owner && owner->id == 0xde9 && (u16)owner->params <= 2 &&
           *(s16*)((u8*)owner + 0x2c0) == 11 && comboEnemyCurrentCallback() == 2 && play && ctx == &play->actorCtx && owner->room == play->roomCtx.curRoom.num;
}

static int enemyEenoSnowball(ActorContext* ctx, PlayState* play, Actor* parent, u16 params, const EnemyPlacement* split)
{
    Actor* owner = comboEnemyCurrentActor();
    return !split && !parent && owner && owner->id == 0xde6 &&
           (((owner->params == 0 || owner->params == 2) && params == 3) || (owner->params == 1 && params == 4)) &&
           comboEnemyCurrentCallback() == 2 && play && ctx == &play->actorCtx && owner->room == play->roomCtx.curRoom.num;
}
#endif

#if defined(GAME_OOT)
static int enemyBioBabaRoot(Actor* parent, u16 params)
{
    return !parent && (params == 0 || params == 1 || params == 4);
}
#endif

static int enemyClaimRootPlacement(PlayState* play, s16 actorId, u32 flags, Actor* parent,
                                   const EnemyPlacement* split, EnemyPlacement* placement)
{
    if (!split && (flags & FOREIGN_ENEMY_ROOT) && (parent || !comboEnemyTakePlacement(play, actorId, placement)))
    {
        g.roomEnemyLackSoul = 1;
        return 0;
    }
    return 1;
}

#if defined(GAME_MM)
static int enemyTailChildRequest(Actor* owner, s16 id, s16 rx, s16 ry, s16 rz, s32 params)
{
    if (!owner || owner->id != 0xc35 || id != 0x35 || rx || ry || rz)
        return 0;
    return (comboEnemyCurrentCallback() == 0 && owner->params == -1 && params == 0) ||
           (comboEnemyCurrentCallback() == 2 && owner->params == 11 && params == 10);
}
#endif

static int enemyHasSoul(const ForeignEnemyRegistration* registration)
{
    if (!registration->soulGi)
        return 1;
    return
#if defined(GAME_OOT)
        ((registration->flags & FOREIGN_ENEMY_HOST_SOUL) ? comboHasSoulOot(registration->soulGi) : comboHasSoulMm(registration->soulGi))
#else
        ((registration->flags & FOREIGN_ENEMY_HOST_SOUL) ? comboHasSoulMm(registration->soulGi) : comboHasSoulOot(registration->soulGi))
#endif
    ;
}

int comboEnemyForeignSoulCheck(s16 actorId)
{
    ForeignEnemyRegistration registration;
    if (!enemyLookupRegistration(actorId, &registration))
        return -1;
    return enemyHasSoul(&registration);
}

static Actor* enemySpawnRegistered(ActorContext* actorCtx, PlayState* play, s16 actorId,
                                   float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 params,
                                   u32 csId, u32 halfDays, Actor* parent, const EnemyPlacement* split, int debugSpawn, int inheritedSoul)
{
    ForeignEnemyRegistration registration;
    EnemyPlacement placement;
    ActorOvl* entry;
    ActorProfile* profile;
    Actor* actor;
    ForeignEnemyInstance* instance;
    u32 instanceSize;
    s32 objectSlot;
    u32 previousSegment;
    int allocationFailed, previousLackSoul;

    if (!comboIsForeignEnemyActor(actorId) || !enemyLookupRegistration(actorId, &registration))
    {
        g.roomEnemyLackSoul = 1;

        goto rejected;
    }
    sSpawnFlags = registration.flags;
#if defined(GAME_OOT)
    if (actorId == 0xc0b && params != 1)
    {
        g.roomEnemyLackSoul = 1;
        goto rejected;
    }
    if (actorId == 0xde9 && params != 0x0101 && params != 0 && !(split && params == 2))
    {
        Actor* owner = comboEnemyCurrentActor();
        int debugMaskDetach = debugSpawn && params == 2 && !parent && owner &&
                              owner->id == actorId && owner->params == 0 && comboEnemyCurrentCallback() == 2 &&
                              owner->room == play->roomCtx.curRoom.num;
        if (!debugMaskDetach && !enemyHiploopFragment(actorCtx, play, parent, params, split))
        {
            g.roomEnemyLackSoul = 1;
            goto rejected;
        }
        registration.flags &= ~FOREIGN_ENEMY_ROOT;
    }
    if (actorId == 0xde6 && params != 0 && params != 1 && params != 0xff00 && params != 0xff01)
    {
        Actor* owner = comboEnemyCurrentActor();
        int member = split && params == 2 && owner && owner->id == 0xde6 && owner->params == 1 &&
                     comboEnemyCurrentCallback() == 0 && (!parent || parent == owner) && (csId & 0x7f) == 0x7f;
        if (!member && !enemyEenoSnowball(actorCtx, play, parent, params, split))
        {
            g.roomEnemyLackSoul = 1;
            goto rejected;
        }
        registration.flags &= ~FOREIGN_ENEMY_ROOT;
    }
    if ((actorId == 0xd80 && params != 0x192 && (params < 10 || params > 14)) ||
        (actorId == 0xdd1 && params != 0) || (actorId == 0xc2d && params != 0xff) ||
        ((actorId == 0xe3f || actorId == 0xe91) && params != 0) ||
        (actorId == 0xe3d && params != 0x14) || (actorId == 0xd84 && params != 0x1fc0))
    {
        g.roomEnemyLackSoul = 1;
        goto rejected;
    }
    if (actorId == 0xe3f && parent && parent->id == actorId)
    {
        if (split || params != 0 || comboEnemyCurrentActor() != parent || comboEnemyCurrentCallback() != 2)
            goto rejected;
        registration.flags &= ~FOREIGN_ENEMY_ROOT;
    }
    if (actorId == 0xd15 && (params == 0x817f || params == 0x827f || params == 0x837f))
    {
        Actor* owner = comboEnemyCurrentActor();
        if (split || parent || !owner || owner->id != actorId || (u16)owner->params != 0x807f ||
            comboEnemyCurrentCallback() != 2)
        {

            goto rejected;
        }
        registration.flags &= ~FOREIGN_ENEMY_ROOT;
    }
    if (actorId == 0xd8e)
    {
        Actor* owner = comboEnemyCurrentActor();
        if (params > 1 || split || !owner || owner->id != 0xd84 || owner->params != 0x1fc0 ||
            comboEnemyCurrentCallback() != 2 || (parent && parent != owner) || (registration.flags & FOREIGN_ENEMY_ROOT))
        {
            g.roomEnemyLackSoul = 1;
            goto rejected;
        }
    }
    if (actorId == 0xd80 && params >= 10 && params <= 14)
    {
        Actor* owner = comboEnemyCurrentActor();
        if (split || !owner || owner->id != 0xd80 || owner->params != 0x192 ||
            owner->colChkInfo.health || comboEnemyCurrentCallback() != 2 ||
            (parent && parent != owner))
        {
            g.roomEnemyLackSoul = 1;
            goto rejected;
        }
        registration.flags &= ~FOREIGN_ENEMY_ROOT;
    }
    if (actorId == 0xd2d && !split && !enemyBioBabaRoot(parent, params))
    {
        g.roomEnemyLackSoul = 1;
        goto rejected;
    }
#else
    if (actorId == 0xcde)
    {
        if (params == 3)
        {
            Actor* owner = comboEnemyCurrentActor();
            if (split || !owner || owner->id != 0xcde || (u16)owner->params > 2 ||
                owner->colChkInfo.health || comboEnemyCurrentCallback() != 2 || (parent && parent != owner))
            {
                g.roomEnemyLackSoul = 1;
                goto rejected;
            }
            registration.flags &= ~FOREIGN_ENEMY_ROOT;
        }
        else if (params < 0xff00 || params > 0xff02 || parent || split)
        {
            g.roomEnemyLackSoul = 1;
            goto rejected;
        }
    }
#endif
#if defined(GAME_MM)
    if (actorId == 0xc1d && params == 1)
    {
        Actor* owner = comboEnemyCurrentActor();
        if (!owner || owner->id != 0xc1d || owner->params != 0 || comboEnemyCurrentCallback() != 2 || parent != owner)
        {
            g.roomEnemyLackSoul = 1;
            goto rejected;
        }
        registration.flags &= ~FOREIGN_ENEMY_ROOT;
    }
#endif
    if (debugSpawn)
    {
        registration.flags &= ~FOREIGN_ENEMY_ROOT;
        registration.defeat = (EnemyDefeatTransition){ 0 };
    }
    if (!enemyClaimRootPlacement(play, actorId, registration.flags, parent, split, &placement))
    {
        goto rejected;
    }
#if defined(GAME_OOT)
    if ((actorId == 0xd4b && (parent || (params != 0x0300 && params != 0x0f00))) ||
        (actorId == 0xd81 && (!parent || parent != comboEnemyCurrentActor() || parent->id != 0xd4b ||
                              comboEnemyCurrentCallback() != 0 || params < 1 || params > ((u16)parent->params >> 8))))
    {
        g.roomEnemyLackSoul = 1;
        goto rejected;
    }
    if (actorId == 0xd5b && ((!split && params != 0x0101 && params != 0x0102 && params != 0x0103 && params != 0x0134 && params != 0x019f) || play->roomCtx.curRoom.num < 0))
    {
        g.roomEnemyLackSoul = 1;
        goto rejected;
    }
#else
    if (actorId == 0xc07 && (registration.flags & FOREIGN_ENEMY_NATIVE_PORT) &&
        (!parent || parent != comboEnemyCurrentActor() || comboEnemyCurrentCallback() != 2 ||
         !((parent->id == 0xc4b && params == 0xffff) ||
           (parent->id == 0xc69 && (params == 3 || params == 11)) ||
           (parent->id == 0xdb0 && params == 1))))
    {
        g.roomEnemyLackSoul = 1;
        goto rejected;
    }
    if ((actorId == 0xc69 && params != 0xfd && params != 0x2fc && params != 0x2fb) ||
        (actorId == 0xc37 && params != 1) || (actorId == 0xc8a && params != 0x601) ||
        (actorId == 0xdb0 && params != 5 && params != 10) ||
        (actorId == 0xc1d && params != 0 && params != 1))
    {
        g.roomEnemyLackSoul = 1;
        goto rejected;
    }
    if ((actorId == 0xcf6 && (parent || params != 3)) ||
        (actorId == 0xce0 && (parent || !comboEnemyCurrentActor() || comboEnemyCurrentActor()->id != 0xcf6 ||
                              comboEnemyCurrentCallback() != 2 || params != 0)) ||
        (actorId == 0xce1 && (!comboEnemyCurrentActor() || comboEnemyCurrentActor()->id != 0xce0 || params != 0)) ||
        (actorId == 0xc3a && params != 10) ||
        (actorId == 0xc35 && !split && params != 0xffff &&
         !(debugSpawn && enemyTailChildRequest(comboEnemyCurrentActor(), 0x35, rx, ry, rz, params))) ||
        (actorId == 0xc2b && !split && params != 6 && params != 0x4006))
    {
        g.roomEnemyLackSoul = 1;
        goto rejected;
    }
#endif
    if (!split && (registration.flags & FOREIGN_ENEMY_ROOT) && placement.listVrom && Flags_GetClear(play, play->roomCtx.curRoom.num))
    {
        goto rejected;
    }

    if (!comboEnemyLoadRomData())
    {
        g.roomEnemyLackSoul = 1;
        goto rejected;
    }

    comboEnemySoulPreviewGateBegin();
    int soulAllowed = debugSpawn || inheritedSoul || enemyHasSoul(&registration);
    if (!comboEnemySoulPreviewGate(play, 1 - ENEMY_NATIVE_GAME, actorId - FOREIGN_ENEMY_ACTOR_BASE, params, x, y, z, ry, soulAllowed))
    {
        if (registration.flags & FOREIGN_ENEMY_ROOT)
            g.roomEnemyLackSoul = 1;
        goto rejected;
    }

#if defined(GAME_OOT)
    if (actorCtx->total >= 200)
#else
    if (actorCtx->totalLoadedActors >= 255)
#endif
        goto rejected;

    entry = enemyOverlaySlot(actorId);
    if (!entry)
        goto rejected;

    previousLackSoul = g.roomEnemyLackSoul;
    profile = enemyLoadProfile(entry, registration.manifestVrom, &allocationFailed);
    if (!profile || profile->id != actorId || profile->type >= 12 || entry->count == 255 ||
        !comboEnemyDefeatTransitionValid(&registration.defeat, registration.flags & FOREIGN_ENEMY_ROOT,
                                         profile->instanceSize,
                                         (u32)entry->dataInit - entry->vramStart) ||
        ((registration.flags & FOREIGN_ENEMY_ROOT) && profile->type != ACTORCAT_ENEMY && profile->type != ACTORCAT_BOSS))
    {
        if (allocationFailed)

            enemyReleaseUnused(entry);
        if (registration.flags & FOREIGN_ENEMY_ROOT)
            g.roomEnemyLackSoul = allocationFailed && !split && comboEnemyEncounterRetryAllocation(&placement) ? previousLackSoul : 1;
        goto rejected;
    }

    objectSlot = (registration.flags & FOREIGN_ENEMY_NATIVE_PORT) && profile->objectId >= 0x1001 && profile->objectId <= 0x1003
                     ? Object_GetSlot(&play->objectCtx, OBJECT_GAMEPLAY_KEEP)
                 : (registration.flags & FOREIGN_ENEMY_NATIVE_PORT) && !(registration.flags & FOREIGN_ENEMY_ROOT) &&
                         actorId == 0xc07 && params == 0xffff && profile->objectId == OBJECT_GAMEPLAY_KEEP
                     ? Object_GetSlot(&play->objectCtx, OBJECT_GAMEPLAY_KEEP)
                 : (registration.flags & FOREIGN_ENEMY_PART) ? rz
                                                             : comboEnemyObjectSlot(profile->objectId);
    if (objectSlot < 0 || objectSlot >= 128 ||
        (objectSlot < OBJECT_EXCHANGE_BANK_MAX && objectSlot >= play->objectCtx.num) ||
        !Object_IsLoaded(&play->objectCtx, objectSlot))
    {
        enemyReleaseUnused(entry);
        if (registration.flags & FOREIGN_ENEMY_ROOT)
            g.roomEnemyLackSoul = 1;
        goto rejected;
    }

    instanceSize = ((profile->instanceSize + 15) & ~15) + sizeof(ForeignEnemyInstance);
    previousLackSoul = g.roomEnemyLackSoul;
    actor = comboEnemyArenaAlloc(instanceSize);
    if (!actor)
    {
        enemyReleaseUnused(entry);
        if (registration.flags & FOREIGN_ENEMY_ROOT)
            g.roomEnemyLackSoul = !split && comboEnemyEncounterRetryAllocation(&placement) ? previousLackSoul : 1;
        goto rejected;
    }
    sForeignEnemyFlags[entry - sForeignEnemyOverlays] = debugSpawn ? sSpawnFlags : registration.flags;
    ++entry->count;
    memset(actor, 0, instanceSize);
    actor->ovl = entry;
    actor->id = profile->id;
    actor->actorIndex = !split && (registration.flags & FOREIGN_ENEMY_ROOT) ? placement.slot : 0xff;
    actor->flags = profile->flags;
    actor->objectSlot = objectSlot;
    if (registration.flags & FOREIGN_ENEMY_PART)
        rz = 0;
    instance = enemyInstance(actor, profile);
    instance->hint = 0xff;
    instance->live = 1;
#if defined(DEBUG)
    instance->debugSpawn = debugSpawn;
    instance->debugWater = debugSpawn && gDebugEnemyWater;
    instance->debugWaterSurface = gDebugEnemyWaterSurface;
#endif
    instance->flags = registration.flags;
    instance->ownerRoom = split ? split->room : (registration.flags & FOREIGN_ENEMY_ROOT) ? placement.room
                                                                                          : play->roomCtx.curRoom.num;
    Actor* owner = comboEnemyCurrentActor();
    ActorOvl* ownerOverlay = owner ? owner->ovl : NULL;
    if (!(registration.flags & FOREIGN_ENEMY_ROOT) && !split && owner && comboIsForeignEnemyActor(owner->id) && ownerOverlay && ownerOverlay->data)
    {
        ActorProfile* ownerProfile = (ActorProfile*)((u8*)ownerOverlay->data + (u32)ownerOverlay->dataInit - ownerOverlay->vramStart);
        instance->ownerRoom = enemyInstance(owner, ownerProfile)->ownerRoom;
    }
    instance->defeat = registration.defeat;
    instance->init = profile->init;
    instance->fini = profile->fini;
    instance->update = profile->update;
    instance->draw = profile->draw;
#if defined(GAME_MM)
    instance->audio.flags = profile->flags & ((1u << 19) | (1u << 20) | (1u << 21) | (1u << 28));
#endif
    enemyWrapCallbacks(actor, instance);
    actor->room = parent ? parent->room : play->roomCtx.curRoom.num;
    actor->parent = parent;
    if (parent)
        parent->child = actor;
    actor->home.pos.x = x;
    actor->home.pos.y = y;
    actor->home.pos.z = z;
    actor->home.rot.x = rx;
    actor->home.rot.y = ry;
    actor->home.rot.z = rz;
    actor->params = params;
#if defined(GAME_MM)
    actor->csId = (csId & 0x7f) == 0x7f ? -1 : csId & 0x7f;
    actor->halfDaysBits = halfDays ? halfDays : 0x3ff;
#endif

    if (split ? !comboEnemyEncounterRegisterMember(actor, split) : ((registration.flags & FOREIGN_ENEMY_ROOT) && !comboEnemyEncounterRegister(actor, &placement)))
    {
        if (parent && parent->child == actor)
            parent->child = NULL;
        --entry->count;
        EnemyArenaFree(actor);
        enemyReleaseUnused(entry);
        g.roomEnemyLackSoul = 1;
        goto rejected;
    }
    EnemyActorAdd(actorCtx, actor, profile->type);
    previousSegment = gSegments[6];
    EnemyActorInit(actor, play);
    gSegments[6] = previousSegment;
    sSpawnFlags = registration.flags;
    comboEnemyFamilySpawned(actor);
    return actor;
rejected:

    return NULL;
}

#if defined(DEBUG)
Actor* comboDebugSpawnForeignEnemy(PlayState* play, s16 actorId, u16 params, Vec3f* pos, s16 yaw)
{
    return enemySpawnRegistered(&play->actorCtx, play, actorId, pos->x, pos->y, pos->z,
                                0, yaw, 0, params, 0x7f, 0x3ff, NULL, NULL, 1, 0);
}
#endif

#if defined(GAME_OOT)
static Actor* enemyBioBabaMember(ActorContext* ctx, PlayState* play, Actor* parent,
                                 float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 params, u32 csId, u32 halfDays)
{
    EnemyPlacement group;
    Actor* child = NULL;
    Actor* owner = comboEnemyCurrentActor();
    if (!owner || owner->id != 0xd2d || !comboEnemyEncounterSplitPlacement(owner, &group))
    {
        g.roomEnemyLackSoul = 1;
        return NULL;
    }
    int valid = comboEnemyCurrentCallback() == 2 && parent == owner &&
                ((owner->params == 2 && (params == 3 || params == 10 || params == 11)) ||
                 (owner->params == 3 && params == 4) ||
                 (owner->params == 4 && params >= 10 && params < 24));
    if (valid && play && ctx == &play->actorCtx && play->roomCtx.curRoom.num == group.room)
        child = enemySpawnRegistered(ctx, play, 0xd2d, x, y, z, rx, ry, rz, params, csId, halfDays, parent, &group, 0, 0);
    if (!child || !child->update)
        comboEnemyEncounterBlock(&group);
    return child;
}
EXPORT_SYMBOL(FOREIGN_ENEMY_MM_SELF_FAMILIES_V2, enemyBioBabaMember);
EXPORT_SYMBOL(FOREIGN_ENEMY_MM_SELF_FAMILIES_V3, enemyBioBabaMember);
static Actor* enemyBadBatMember(ActorContext* ctx, PlayState* play, Actor* parent,
                                float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 params, u32 csId, u32 halfDays)
{
    EnemyPlacement group;
    Actor* child = NULL;
    Actor* owner = comboEnemyCurrentActor();
    if (!owner || owner->id != 0xd5b || !comboEnemyEncounterSplitPlacement(owner, &group))
    {
        g.roomEnemyLackSoul = 1;
        return NULL;
    }
    u8 flags = *((u8*)owner + 0x141);
    int valid = comboEnemyCurrentCallback() == 0 && !parent &&
                owner->params > 1 && owner->params <= 20 && flags <= 1 &&
                *(s16*)((u8*)owner + 0x142) == 1 && params == (0x0100 | (flags << 5)) &&
                (csId & 0x7f) == 0x7f;
    if (valid && play && ctx == &play->actorCtx && play->roomCtx.curRoom.num == group.room)
        child = enemySpawnRegistered(ctx, play, 0xd5b, x, y, z, rx, ry, rz, params, csId, halfDays, NULL, &group, 0, 0);
    if (!child || !child->update)
        comboEnemyEncounterBlock(&group);
    return child;
}
EXPORT_SYMBOL(FOREIGN_ENEMY_MM_BAD_BAT_FAMILY_V1, enemyBadBatMember);
static Actor* enemyHiploopMaskedMember(ActorContext* ctx, PlayState* play, Actor* parent,
                                       float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 params, u32 csId, u32 halfDays)
{
    EnemyPlacement group;
    Actor* child = NULL;
    Actor* owner = comboEnemyCurrentActor();
    if (!owner || owner->id != 0xde9 || !comboEnemyEncounterSplitPlacement(owner, &group))
    {
        g.roomEnemyLackSoul = 1;
        return NULL;
    }
    s16 action = *(s16*)((u8*)owner + 0x2c0);
    int valid = comboEnemyCurrentCallback() == 2 && !parent && owner->params == 0 &&
                action >= 0 && action < 7 && params == 2 && (csId & 0x7f) == 0x7f;
    if (valid && play && ctx == &play->actorCtx && play->roomCtx.curRoom.num == group.room)
        child = enemySpawnRegistered(ctx, play, 0xde9, x, y, z, rx, ry, rz, params, csId, halfDays, NULL, &group, 0, 0);
    if (!child || !child->update)
        comboEnemyEncounterBlock(&group);
    return child;
}
EXPORT_SYMBOL(FOREIGN_ENEMY_MM_HIPLOOP_MASK_V1, enemyHiploopMaskedMember);

static Actor* enemyEenoSplitMember(ActorContext* ctx, PlayState* play, Actor* parent,
                                   float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 params, u32 csId, u32 halfDays)
{
    EnemyPlacement group;
    Actor* child = NULL;
    Actor* owner = comboEnemyCurrentActor();
    if (!owner || owner->id != 0xde6 || !comboEnemyEncounterSplitPlacement(owner, &group))
    {
        g.roomEnemyLackSoul = 1;
        return NULL;
    }
    int valid = comboEnemyCurrentCallback() == 0 && owner->params == 1 && params == 2 &&
                (!parent || parent == owner) && (csId & 0x7f) == 0x7f;
    if (valid && play && ctx == &play->actorCtx && play->roomCtx.curRoom.num == group.room)
        child = enemySpawnRegistered(ctx, play, 0xde6, x, y, z, rx, ry, rz, params, csId, halfDays, parent, &group, 0, 0);
    if (!child || !child->update)
        comboEnemyEncounterBlock(&group);
    return child;
}
EXPORT_SYMBOL(FOREIGN_ENEMY_MM_EENO_FAMILY_V1, enemyEenoSplitMember);

#endif

Actor* comboSpawnForeignEnemy(ActorContext* actorCtx, PlayState* play, s16 actorId,
                              float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 params,
                              u32 csId, u32 halfDays, Actor* parent)
{
#if defined(DEBUG)
    if (gDebugEnemySpawn)
        return enemySpawnRegistered(actorCtx, play, actorId, x, y, z, rx, ry, rz,
                                    params, csId, halfDays, parent, NULL, 1, 0);
#endif
#if defined(GAME_OOT)
    if (actorId == 0xde6 && params == 2 && comboEnemyCurrentActor() && comboEnemyCurrentActor()->id == 0xde6)
        return enemyEenoSplitMember(actorCtx, play, parent, x, y, z, rx, ry, rz, params, csId, halfDays);
    if (actorId == 0xde9 && params == 2 && comboEnemyCurrentActor() && comboEnemyCurrentActor()->id == 0xde9)
        return enemyHiploopMaskedMember(actorCtx, play, parent, x, y, z, rx, ry, rz, params, csId, halfDays);
    if (actorId == 0xd5b && comboEnemyCurrentActor() && comboEnemyCurrentActor()->id == 0xd5b)
        return enemyBadBatMember(actorCtx, play, parent, x, y, z, rx, ry, rz, params, csId, halfDays);
    if (actorId == 0xd2d && comboEnemyCurrentActor() && comboEnemyCurrentActor()->id == 0xd2d)
        return enemyBioBabaMember(actorCtx, play, parent, x, y, z, rx, ry, rz, params, csId, halfDays);
#endif
    return enemySpawnRegistered(actorCtx, play, actorId, x, y, z, rx, ry, rz, params, csId, halfDays, parent, NULL, 0, 0);
}

typedef struct {
    u16 id, params;
} EnemySplitChild;

typedef struct {
    u32 key;
    EnemySplitChild children[3];
    u16 poolStart;
    u16 poolCount;
} EnemySplitRow;

_Static_assert(sizeof(EnemySplitRow) == 20, "enemy split metadata ABI");

static u32 sEnemySplitVrom __attribute__((section(".data"))) = 0;
#if defined(GAME_OOT)
EXPORT_SYMBOL(ENEMY_SPLIT_CHOICES_OOT, sEnemySplitVrom);
#else
EXPORT_SYMBOL(ENEMY_SPLIT_CHOICES_MM, sEnemySplitVrom);
#endif

static int enemySplitChild(PlayState* play, u32 key, u16* id, u16* params)
{
    static u32 previousKey, previousFrame;
    static u8 index;
    if (!key || !sEnemySplitVrom)
        return 0;

    if (previousKey != key || previousFrame != play->gameplayFrames || index == 3)
    {
        previousKey = key;
        previousFrame = play->gameplayFrames;
        index = 0;
    }
    u8 child = index++;

    DmaEntry file;
    u32 header[4];
    if (!comboDmaLookup(&file, sEnemySplitVrom) || file.vstart != sEnemySplitVrom || file.pend ||
        file.vend < file.vstart || file.vend - file.vstart < sizeof(header) ||
        comboDmaLoadFilePartial(header, sEnemySplitVrom, 0, sizeof(header)) != sizeof(header) ||
        header[0] != 0x454e4253 || (header[1] != 2 && header[1] != 3) || header[2] != sizeof(EnemySplitRow) || header[3] > 2048)
        return 0;

    u32 poolOffset = sizeof(header) + header[3] * sizeof(EnemySplitRow);
    u32 fileSize = file.vend - file.vstart;
    u32 stride = header[1] == 3 ? 8 : sizeof(EnemySplitChild);
    if (fileSize < poolOffset || (fileSize - poolOffset) % stride)
        return 0;
    u32 poolCount = (fileSize - poolOffset) / stride;
    u32 low = 0, high = header[3];
    while (low < high)
    {
        u32 mid = low + (high - low) / 2;
        EnemySplitRow row;
        if (comboDmaLoadFilePartial(&row, sEnemySplitVrom, sizeof(header) + mid * sizeof(row), sizeof(row)) != sizeof(row))
            return 0;
        if (row.key < key)
            low = mid + 1;
        else if (row.key > key)
            high = mid;
        else
        {
            EnemySplitChild selected = row.children[child];
            if (row.poolCount && (u32)row.poolStart + row.poolCount <= poolCount)
            {
                u32 roll = (u32)(Rand_ZeroOne() * row.poolCount);
                if (header[1] == 3)
                {
                    u32 random = (u32)(Rand_ZeroOne() * 0x1000000);
                    u32 first = 0, end = row.poolCount;
                    while (first < end)
                    {
                        u32 mid = first + (end - first) / 2;
                        u32 threshold;
                        u32 at = poolOffset + (row.poolStart + mid) * stride + 4;
                        if (comboDmaLoadFilePartial(&threshold, sEnemySplitVrom, at, 4) != 4)
                            return 0;
                        if (random < threshold)
                            end = mid;
                        else
                            first = mid + 1;
                    }
                    roll = first;
                }
                u32 at = poolOffset + (row.poolStart + MIN(roll, row.poolCount - 1u)) * stride;
                if (comboDmaLoadFilePartial(&selected, sEnemySplitVrom, at, sizeof(selected)) != sizeof(selected))
                    selected = row.children[child];
            }
            *id = selected.id;
            *params = selected.params;
            return *id != 0;
        }
    }
    return 0;
}

static int enemySplitChildPosition(PlayState* play, u16* id, Vec3f* pos)
{
    if (!(*id & 0x8000))
        return 1;
    *id &= 0x7fff;

    CollisionPoly* floor = NULL;
    s32 bgId;
    Vec3f start = { pos->x, pos->y + 8.f, pos->z };
    f32 y = BgCheck_EntityRaycastDown4(&play->colCtx, &floor, &bgId, NULL, &start);
    if (!floor || y > start.y || pos->y - y > 160.f)
        return 0;

    WaterBox* box;
    f32 waterY;
    if (WaterBox_GetSurface1_2(play, &play->colCtx, pos->x, pos->z, &waterY, &box) && waterY > y + 6.f)
        return 0;

    Vec3f center = { pos->x, y + 30.f, pos->z };
    static const Vec3f offsets[] = { { 18.f, 0.f, 0.f }, { -18.f, 0.f, 0.f }, { 0.f, 0.f, 18.f }, { 0.f, 0.f, -18.f } };
    for (int i = 0; i < ARRAY_COUNT(offsets); ++i)
    {
        Vec3f probe = { center.x + offsets[i].x, center.y, center.z + offsets[i].z };
        Vec3f hit;
        CollisionPoly* wall = NULL;
        if (BgCheck_EntityLineTest1(&play->colCtx, &center, &probe, &hit, &wall, 1, 0, 1, 0, &bgId))
            return 0;
        floor = NULL;
        f32 probeY = BgCheck_EntityRaycastDown4(&play->colCtx, &floor, &bgId, NULL, &probe);
        if (!floor || fabsf(probeY - y) > 12.f)
            return 0;
    }
    pos->y = y;
    return 1;
}

#if defined(GAME_MM)
int comboEnemyLoadSplitChildObject(PlayState*, u16);
#endif

typedef struct {
    Actor* owner;
    EnemyFamily family;
    ActorFunc fini;
    s16 scene;
    s8 room;
} EenoSplitMonitor;

static EenoSplitMonitor sEenoSplits[8];

static int enemySplitActorLive(PlayState* play, const Actor* target, s8 room)
{
    for (u32 category = 0; category < ARRAY_COUNT(play->actorCtx.actors); ++category)
        for (Actor* actor = play->actorCtx.actors[category].first; actor; actor = actor->next)
            if (actor == target)
                return actor->room == room && (actor->update || actor->init);
    return 0;
}

static void enemyEenoSplitUpdate(Actor* owner, PlayState* play)
{
    for (int i = 0; i < ARRAY_COUNT(sEenoSplits); ++i)
    {
        EenoSplitMonitor* monitor = &sEenoSplits[i];
        if (monitor->owner != owner)
            continue;
        if (!comboEnemyFamilyDone(&monitor->family))
            return;
        comboEnemyEncounterComplete(owner, play);
        Actor_Kill(owner);
        return;
    }
}

static void enemyEenoSplitFini(Actor* actor, PlayState* play)
{
    for (u32 i = 0; i < ARRAY_COUNT(sEenoSplits); ++i)
        if (sEenoSplits[i].owner == actor)
        {
            ActorFunc fini = sEenoSplits[i].fini;
            comboEnemyFamilyClear(&sEenoSplits[i].family);
            memset(&sEenoSplits[i], 0, sizeof(sEenoSplits[i]));
            actor->fini = fini;
            if (fini)
                fini(actor, play);
            return;
        }
}

static Actor* enemyDynamicChild(ActorContext* ctx, PlayState* play, s16 id, u16 params, Vec3f* pos, s16 yaw, EnemyFamily* family)
{
    ActorEntry entry = { .id = id, .params = params, .pos = { pos->x, pos->y, pos->z } };
#if defined(GAME_MM)
    entry.rot = (Vec3s){ 7, ((u16)(s16)(yaw * (360.f / 65536.f)) << 7) | 0x7f, 0x7f };
#else
    entry.rot.y = yaw;
#endif
    return family ? comboEnemyFamilySpawn(family, ctx, &entry, play, 0xffff) :
        comboEnemyDynamicSpawn(ctx, &entry, play, 0xffff);
}

static int enemyEenoReplaceSplit(Actor* owner, PlayState* play, u32 key)
{
    if (!key || !sEnemySplitVrom || !owner->parent || !owner->child ||
        owner->room != play->roomCtx.curRoom.num)
        return 0;

    EenoSplitMonitor* monitor = NULL;
    for (int i = 0; i < ARRAY_COUNT(sEenoSplits); ++i)
    {
        EenoSplitMonitor* entry = &sEenoSplits[i];
        if (!entry->owner || entry->scene != play->sceneId ||
            !enemySplitActorLive(play, entry->owner, entry->room))
        {
            monitor = entry;
            break;
        }
    }
    if (!monitor)
        return 0;

    Vec3f origin = owner->world.pos;
#if defined(GAME_MM)
    origin.x -= 40.f * Math_SinS(owner->shape.rot.y);
    origin.z -= 40.f * Math_CosS(owner->shape.rot.y);
#endif
    u16 targets[3], params[3];
    Vec3f positions[3];
    s16 yaws[3];
    for (int i = 0; i < ARRAY_COUNT(targets); ++i)
    {
        if (!enemySplitChild(play, key, &targets[i], &params[i]))
            return 0;
        yaws[i] = owner->shape.rot.y + i * 0x5555;
        positions[i] = origin;
        positions[i].x += 40.f * Math_SinS(yaws[i]);
        positions[i].z += 40.f * Math_CosS(yaws[i]);
        Vec3f from = origin;
        Vec3f to = positions[i];
        from.y += 20.f;
        to.y += 20.f;
        Vec3f hit;
        CollisionPoly* wall = NULL;
        s32 bgId;
        if (BgCheck_EntityLineTest1(&play->colCtx, &from, &to, &hit, &wall, 1, 0, 1, 0, &bgId))
            return 0;
        if (!enemySplitChildPosition(play, &targets[i], &positions[i]))
            return 0;
    }

    EnemyNativeSegments scope = { 0 };
    if (!comboEnemyNativeEnter(play, &scope, ENEMY_NATIVE_KEEP_MAIN))
        return 0;
    Actor* children[3] = { 0 };
    comboEnemyFamilyClear(&monitor->family);
    int previousLackSoul = g.roomEnemyLackSoul;
    int spawned = 0;
    for (; spawned < ARRAY_COUNT(children); ++spawned)
    {
        Vec3f* pos = &positions[spawned];
        children[spawned] = enemyDynamicChild(&play->actorCtx, play, targets[spawned], params[spawned], pos,
                                            yaws[spawned], &monitor->family);
        if (!children[spawned] || (!children[spawned]->update && !children[spawned]->init))
            break;
    }
    comboEnemyNativeLeave(&scope);
    if (spawned != ARRAY_COUNT(children))
    {
        comboEnemyFamilyClear(&monitor->family);
        g.roomEnemyLackSoul = previousLackSoul;
        return 0;
    }

    monitor->owner = owner;
    monitor->scene = play->sceneId;
    monitor->room = owner->room;
    monitor->fini = owner->fini;
    owner->fini = enemyEenoSplitFini;
    comboEnemyEncounterComplete(owner->parent, play);
    comboEnemyEncounterComplete(owner->child, play);
    Actor_Kill(owner->parent);
    Actor_Kill(owner->child);
    owner->parent = NULL;
    owner->child = NULL;
    owner->draw = NULL;
    owner->update = enemyEenoSplitUpdate;
    owner->flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    owner->flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
    return 1;
}

#if defined(GAME_OOT)
int FOREIGN_ENEMY_API_EENO_REPLACE_SPLIT(Actor* owner, PlayState* play)
{
    EnemyPlacement group;
    if (!sEnemySplitVrom || owner != comboEnemyCurrentActor() || comboEnemyCurrentCallback() != 2 ||
        owner->id != 0xde6 || !comboEnemyEncounterSplitPlacement(owner, &group))
        return 0;
    return enemyEenoReplaceSplit(owner, play, comboEnemyEncounterSplitKey(play, &group));
}
EXPORT_SYMBOL(FOREIGN_ENEMY_API_EENO_REPLACE_SPLIT, FOREIGN_ENEMY_API_EENO_REPLACE_SPLIT);
#else
void comboEnemyNativeEenoSplit(Actor* owner, PlayState* play)
{
    enemyEenoReplaceSplit(owner, play, comboEnemyNativeSplitKey(play, 0x1e6));
}
#endif

#if defined(GAME_OOT)
static Actor* enemyNativeBariSplit(ActorContext* ctx, PlayState* play, s16 id,
                                   float x, float y, float z, s16 rx, s16 ry, s16 rz, s32 params)
{
    u16 target, targetParams;
    Vec3f pos = { x, y, z };
    if (id == ACTOR_EN_BILI && !rx && !rz && !params &&
        enemySplitChild(play, comboEnemyNativeSplitKey(play, ACTOR_EN_VALI), &target, &targetParams) &&
        enemySplitChildPosition(play, &target, &pos))
    {
        int previousLackSoul = g.roomEnemyLackSoul;
        Actor* child = enemyDynamicChild(ctx, play, target, targetParams, &pos, ry, NULL);
        if (child)
            return child;
        if (g.roomEnemyLackSoul != previousLackSoul)
            return NULL;
    }
    if (id == ACTOR_EN_BILI && !rx && !rz && !params)
    {
        extern int gOotBariChildSpawn;
        ++gOotBariChildSpawn;
        Actor* child = Actor_Spawn(ctx, play, id, x, y, z, rx, ry, rz, params);
        --gOotBariChildSpawn;
        return child;
    }
    return Actor_Spawn(ctx, play, id, x, y, z, rx, ry, rz, params);
}
PATCH_CALL(0x8090bc3c, enemyNativeBariSplit);
#endif

#if defined(GAME_MM)
static Actor* enemySpawnMember(EnemyPlacement* group, PlayState* play, ActorContext* ctx, int valid, s16 id, int inheritedSoul,
                               float x, float y, float z, s16 rx, s16 ry, s16 rz, s32 params)
{
    Actor* child = NULL;
    if (valid && play && ctx == &play->actorCtx && play->roomCtx.curRoom.num == group->room)
        child = enemySpawnRegistered(ctx, play, FOREIGN_ENEMY_ACTOR_BASE + id, x, y, z, rx, ry, rz,
                                     params, 0x7f, 0x3ff, NULL, group, 0, inheritedSoul);
    if (!child || !child->update)
        comboEnemyEncounterBlock(group);
    return child;
}

static Actor* enemyTailMember(ActorContext* ctx, PlayState* play, s16 id,
                              float x, float y, float z, s16 rx, s16 ry, s16 rz, s32 params)
{
    Actor* owner = comboEnemyCurrentActor();
    EnemyPlacement group;
    if (!enemyTailChildRequest(owner, id, rx, ry, rz, params) || !play || ctx != &play->actorCtx)
    {
        g.roomEnemyLackSoul = 1;
        return NULL;
    }
#if defined(DEBUG)

    if (gDebugEnemySpawn)
        return enemySpawnRegistered(ctx, play, 0xc35, x, y, z, rx, ry, rz, params,
                                    0x7f, 0x3ff, NULL, NULL, 1, 1);
#endif
    if (!comboEnemyEncounterSplitPlacement(owner, &group))
    {
        g.roomEnemyLackSoul = 1;
        return NULL;
    }
    return enemySpawnMember(&group, play, ctx, 1, id, 1, x, y, z, rx, ry, rz, params);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_TAIL_MEMBER, enemyTailMember);

static Actor* enemyEggDebris(ActorContext* ctx, Actor* parent, PlayState* play, s16 id,
                             float x, float y, float z, s16 rx, s16 ry, s16 rz, s32 params)
{
    EnemyPlacement group;
    if (!parent || parent != comboEnemyCurrentActor() || parent->id != 0xc2b || parent->params != 6 ||
        comboEnemyCurrentCallback() != 2 || !comboEnemyEncounterSplitPlacement(parent, &group))
    {
        g.roomEnemyLackSoul = 1;
        return NULL;
    }
    Actor* child = enemySpawnMember(&group, play, ctx, id == 0x2b && params >= 10 && params <= 24 && !rx && !rz,
                                    id, 0, x, y, z, rx, ry, rz, params);
    if (child)
    {
        child->parent = parent;
        parent->child = child;
        if (child->room >= 0)
            child->room = parent->room;
    }
    return child;
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_EGG_DEBRIS, enemyEggDebris);

static Actor* enemyBariSplit(ActorContext* ctx, PlayState* play, s16 id,
                             float x, float y, float z, s16 rx, s16 ry, s16 rz, s32 params)
{
    Actor* owner = comboEnemyCurrentActor();
    EnemyPlacement group;
    if (!owner || owner->id != FOREIGN_ENEMY_ACTOR_BASE + 0x63 || comboEnemyCurrentCallback() != 2 ||
        !comboEnemyEncounterSplitPlacement(owner, &group))
    {
        g.roomEnemyLackSoul = 1;
        return NULL;
    }
    int valid = id == 0x34 && !rx && !rz && !params;
    u16 target, targetParams;
    Vec3f pos = { x, y, z };
    if (valid && enemySplitChild(play, comboEnemyEncounterSplitKey(play, &group), &target, &targetParams) &&
        enemySplitChildPosition(play, &target, &pos))
    {
        int previousLackSoul = g.roomEnemyLackSoul;
        Actor* child = enemyDynamicChild(ctx, play, target, targetParams, &pos, ry, NULL);
        if (child)
            return child;
        if (g.roomEnemyLackSoul != previousLackSoul)
        {
            comboEnemyEncounterBlock(&group);
            return NULL;
        }
    }
    return enemySpawnMember(&group, play, ctx, valid, id, valid, x, y, z, rx, ry, rz, params);
}
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_BARI_SPLIT, enemyBariSplit);
#endif

u32 gForeignEnemySpawnsVrom __attribute__((section(".data"))) = 0;
#if defined(GAME_OOT)
# define ENEMY_NAMESPACE_GAME 0u
# define ENEMY_NATIVE_ACTOR_LIMIT 0x1d7
EXPORT_SYMBOL(FOREIGN_ENEMY_SPAWNS_OOT, gForeignEnemySpawnsVrom);
#else
# define ENEMY_NAMESPACE_GAME 1u
# define ENEMY_NATIVE_ACTOR_LIMIT 0x2b2
EXPORT_SYMBOL(FOREIGN_ENEMY_SPAWNS_MM, gForeignEnemySpawnsVrom);
#endif

typedef struct
{
    u16 sourceActorId;
    u16 targetActorId;
    u16 matchParamsMask;
    u16 matchParamsValue;
    u16 matchRotXMask;
    u16 matchRotXValue;
    u16 paramsMask;
    u16 paramsValue;
    u16 rotMaskValue[6];
    u32 requireZeroRotation;
} EnemySpawnRule;

_Static_assert(sizeof(EnemySpawnRule) == 32, "Enemy spawn namespace wire format");

static EnemySpawnRule* sSpawnRows;
static u32 sSpawnCount;
static u32 sSpawnLoadedVrom;
static int enemyValidSpawnRule(const EnemySpawnRule* row);

static int enemyLoadNamespace(void)
{
    DmaEntry dma;
    EnemySpawnRule* rows = NULL;
    u32 header[4], count;
    if (sSpawnLoadedVrom && sSpawnLoadedVrom == gForeignEnemySpawnsVrom)
        return 1;
    if (!gForeignEnemySpawnsVrom || gForeignEnemySpawnsVrom < 0x08000000 ||
        !comboDmaLookup(&dma, gForeignEnemySpawnsVrom) || dma.vstart != gForeignEnemySpawnsVrom || dma.pend ||
        dma.vend < dma.vstart || dma.vend - dma.vstart < sizeof(header) ||
        comboDmaLoadFilePartial(header, gForeignEnemySpawnsVrom, 0, sizeof(header)) != sizeof(header) ||
        header[0] != 0x454e5350 || (header[1] != 1 && header[1] != 2) || header[2] != ENEMY_NAMESPACE_GAME || header[3] > 4096 ||
        dma.vend - dma.vstart != sizeof(header) + header[3] * sizeof(*rows))
        return 0;
    count = header[3];
    if (count)
    {
        rows = malloc(count * sizeof(*rows));
        if (!rows)
            return 0;
        if (comboDmaLoadFilePartial(rows, gForeignEnemySpawnsVrom, sizeof(header), count * sizeof(*rows)) != count * sizeof(*rows))
        {
            free(rows);
            return 0;
        }
        for (u32 i = 0; i < count; ++i)
            if (rows[i].sourceActorId >= FOREIGN_ENEMY_ACTOR_COUNT || !enemyValidSpawnRule(&rows[i]) ||
                (header[1] == 1 && rows[i].requireZeroRotation) ||
                (i && rows[i - 1].sourceActorId > rows[i].sourceActorId))
            {
                free(rows);
                return 0;
            }
    }
    if (sSpawnRows)
        free(sSpawnRows);
    sSpawnRows = rows;
    sSpawnCount = count;
    sSpawnLoadedVrom = gForeignEnemySpawnsVrom;
    return 1;
}

static int enemyReadSpawn(u32 index, EnemySpawnRule* out)
{
    if (index >= sSpawnCount)
        return 0;
    *out = sSpawnRows[index];
    return 1;
}

static int enemySpawnRange(s32 sourceId, u32* first, u32* count)
{
    u32 low = 0, high, mid;
    if (sourceId < 0 || sourceId >= FOREIGN_ENEMY_ACTOR_COUNT || !enemyLoadNamespace())
        return 0;
    high = sSpawnCount;
    while (low < high)
    {
        mid = low + (high - low) / 2;
        if (sSpawnRows[mid].sourceActorId < sourceId)
            low = mid + 1;
        else
            high = mid;
    }
    *first = low;
    *count = 0;
    for (; low < sSpawnCount && sSpawnRows[low].sourceActorId == sourceId; ++low)
        ++*count;
    return *count != 0;
}

static int enemyValidSpawnRule(const EnemySpawnRule* row)
{
    if (row->requireZeroRotation > 1 ||
        (row->requireZeroRotation && (row->matchRotXMask != 0xffff || row->matchRotXValue != 0)) || (row->paramsMask & row->paramsValue) ||
        (row->matchParamsValue & ~row->matchParamsMask) || (row->matchRotXValue & ~row->matchRotXMask))
        return 0;
    for (u32 i = 0; i < 6; i += 2)
        if (row->rotMaskValue[i] & row->rotMaskValue[i + 1])
            return 0;
    if (comboIsForeignEnemyActor(row->targetActorId))
        return row->targetActorId == FOREIGN_ENEMY_ACTOR_BASE + row->sourceActorId;
    return row->targetActorId < ENEMY_NATIVE_ACTOR_LIMIT;
}

static int enemyFindSpawn(s16 sourceId, u16 params, u16 rx, u16 ry, u16 rz, EnemySpawnRule* out)
{
    u32 first, count;
    if (!enemySpawnRange(sourceId, &first, &count))
        return 0;
    for (u32 i = 0; i < count; ++i)
    {
        if (!enemyReadSpawn(first + i, out) || !enemyValidSpawnRule(out))
            return 0;
        if ((params & out->matchParamsMask) == out->matchParamsValue && (rx & out->matchRotXMask) == out->matchRotXValue &&
            (!out->requireZeroRotation || (!ry && !rz)))
            return 1;
    }
    return 0;
}

static int enemyTargetBits(u16 target, u16 keep, u16 set, u16 matchMask, u16 matchValue)
{
    u16 mask = ~keep | (keep & matchMask);
    return (target & mask) == (set | (keep & matchValue));
}

static int enemyLookupMatches(Actor* actor, u32 first, u32 count)
{
    EnemySpawnRule row;
    for (u32 i = 0; i < count; ++i)
    {
        if (!enemyReadSpawn(first + i, &row) || !enemyValidSpawnRule(&row))
            return 0;
        if (actor->id != row.targetActorId)
            continue;
        if (comboIsForeignEnemyActor(row.targetActorId))
            return 1;
        if (enemyTargetBits(actor->params, row.paramsMask, row.paramsValue, row.matchParamsMask, row.matchParamsValue) &&
            enemyTargetBits(actor->world.rot.x, row.rotMaskValue[0], row.rotMaskValue[1], row.matchRotXMask, row.matchRotXValue) &&
            enemyTargetBits(actor->world.rot.y, row.rotMaskValue[2], row.rotMaskValue[3], row.requireZeroRotation ? 0xffff : 0, 0) &&
            enemyTargetBits(actor->world.rot.z, row.rotMaskValue[4], row.rotMaskValue[5], row.requireZeroRotation ? 0xffff : 0, 0))
            return 1;
    }
    return 0;
}

Actor* comboEnemyActorFind(ActorContext* ctx, s32 sourceId, s32 category)
{
    Actor* actor;
    u32 first = 0, count = 0;
    if (!ctx || category < 0 || category >= 12 || sourceId < 0 || sourceId >= FOREIGN_ENEMY_ACTOR_COUNT)
        return NULL;
    if (sourceId && !enemySpawnRange(sourceId, &first, &count))
        return NULL;
    for (actor = ctx->actors[category].first; actor; actor = actor->next)
        if (sourceId ? enemyLookupMatches(actor, first, count) : actor->id == 0)
            return actor;
    return NULL;
}

Actor* comboEnemyActorFindAfter(PlayState* play, Actor* start, u8 category, s16 sourceId)
{
    Actor* actor;
    u32 first = 0, count = 0;
    if (!play || category >= 12 || sourceId < 0 || sourceId >= FOREIGN_ENEMY_ACTOR_COUNT)
        return NULL;
    if (sourceId && !enemySpawnRange(sourceId, &first, &count))
        return NULL;
    for (actor = start ? start : play->actorCtx.actors[category].first; actor; actor = actor->next)
        if (sourceId ? enemyLookupMatches(actor, first, count) : actor->id == 0)
            return actor;
    return NULL;
}

Actor* comboEnemyActorFindNearby(PlayState* play, Actor* reference, s16 sourceId, u8 category, float distance)
{
    Actor* actor;
    u32 first = 0, count = 0;
    if (!play || !reference || category >= 12 || sourceId < -1 || sourceId >= FOREIGN_ENEMY_ACTOR_COUNT)
        return NULL;
    if (sourceId > 0 && !enemySpawnRange(sourceId, &first, &count))
        return NULL;
    for (actor = play->actorCtx.actors[category].first; actor; actor = actor->next)
        if (actor != reference && (sourceId == -1 || (sourceId == 0 ? actor->id == 0 : enemyLookupMatches(actor, first, count))) &&
            Math_Vec3f_DistXYZ(&reference->world.pos, &actor->world.pos) <= distance)
            return actor;
    return NULL;
}

static Actor* enemySpawn(ActorContext* ctx, PlayState* play, s16 sourceId,
                         float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 params,
                         u32 csId, u32 halfDays, Actor* parent)
{
    EnemySpawnRule rule;
#if defined(GAME_MM)
    Actor* actor;
#endif
    if (!enemyFindSpawn(sourceId, params, rx, ry, rz, &rule))
    {
        g.roomEnemyLackSoul = 1;
        return NULL;
    }
    params = (params & rule.paramsMask) | rule.paramsValue;
    rx = ((u16)rx & rule.rotMaskValue[0]) | rule.rotMaskValue[1];
    ry = ((u16)ry & rule.rotMaskValue[2]) | rule.rotMaskValue[3];
    rz = ((u16)rz & rule.rotMaskValue[4]) | rule.rotMaskValue[5];
    if (comboIsForeignEnemyActor(rule.targetActorId))
    {
#if defined(GAME_MM)
        if (rule.targetActorId == 0xce0 && (!parent || parent != comboEnemyCurrentActor() || parent->id != 0xcf6))
        {
            g.roomEnemyLackSoul = 1;
            return NULL;
        }
        if (rule.targetActorId == 0xc1d && sourceId == 0x1d && params == 1)
            return comboSpawnForeignEnemy(ctx, play, rule.targetActorId, x, y, z, rx, ry, rz, params, csId, halfDays, parent);
        if (rule.targetActorId == 0xc07 && sourceId == 7 &&
            parent && parent == comboEnemyCurrentActor() &&
            ((parent->id == 0xc4b && params == 0xffff) ||
             (parent->id == 0xc69 && (params == 3 || params == 11)) ||
             (parent->id == 0xdb0 && params == 1)))
            return comboSpawnForeignEnemy(ctx, play, rule.targetActorId, x, y, z, rx, ry, rz, params, csId, halfDays, parent);
        actor = comboSpawnForeignEnemy(ctx, play, rule.targetActorId, x, y, z, rx, ry, rz, params, csId, halfDays, NULL);
        if (actor && parent)
        {
            parent->child = actor;
            actor->parent = parent;
            if (actor->room >= 0)
                actor->room = parent->room;
        }
        return actor;
#else
        return comboSpawnForeignEnemy(ctx, play, rule.targetActorId, x, y, z, rx, ry, rz, params, csId, halfDays, parent);
#endif
    }
    if ((csId & 0x7f) != 0x7f)
    {
        g.roomEnemyLackSoul = 1;
        return NULL;
    }
    if (!parent && rule.targetActorId == ACTOR_EN_BOM)
        return Actor_Spawn(ctx, play, rule.targetActorId, x, y, z, rx, ry, rz, params);

    EnemyNativeSegments nativeScope = { 0 };
    Actor* spawned;
    if (!comboEnemyNativeEnter(play, &nativeScope, ENEMY_NATIVE_KEEP_MAIN))
    {
        g.roomEnemyLackSoul = 1;
        return NULL;
    }
    if (parent)
        spawned = Actor_SpawnAsChild(ctx, parent, play, rule.targetActorId, x, y, z, rx, ry, rz, params);
    else
        spawned = Actor_Spawn(ctx, play, rule.targetActorId, x, y, z, rx, ry, rz, params);
    comboEnemyNativeLeave(&nativeScope);
    return spawned;
}

Actor* comboEnemyActorSpawn(ActorContext* ctx, PlayState* play, s16 sourceId,
                            float x, float y, float z, s16 rx, s16 ry, s16 rz, s32 params)
{
    return enemySpawn(ctx, play, sourceId, x, y, z, rx, ry, rz, params, 0x7f, 0x3ff, NULL);
}

Actor* comboEnemyActorSpawnAsChild(ActorContext* ctx, Actor* parent, PlayState* play, s16 sourceId,
                                   float x, float y, float z, s16 rx, s16 ry, s16 rz, s32 params)
{
    if (!parent)
        return NULL;
    return enemySpawn(ctx, play, sourceId, x, y, z, rx, ry, rz, params, 0x7f,
#if defined(GAME_OOT)
        comboEnemySourceHalfDays(parent),
#else
        0x3ff,
#endif
        parent);
}

Actor* comboEnemyActorSpawnAsChildAndCutscene(ActorContext* ctx, PlayState* play, s16 sourceId,
                                              float x, float y, float z, s16 rx, s16 ry, s16 rz, s32 params, u32 csId, u32 halfDays, Actor* parent)
{
    return enemySpawn(ctx, play, sourceId, x, y, z, rx, ry, rz, params, csId, halfDays, parent);
}

EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SPAWN, comboEnemyActorSpawn);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SPAWN_CHILD, comboEnemyActorSpawnAsChild);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SPAWN_CUTSCENE, comboEnemyActorSpawnAsChildAndCutscene);

EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_FIND, comboEnemyActorFind);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_FIND_AFTER, comboEnemyActorFindAfter);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_FIND_NEARBY, comboEnemyActorFindNearby);

u32 comboEnemyCrc32(const u8* data, u32 size)
{
    static const u32 table[16] = {
        0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
        0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
        0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
        0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c,
    };
    u32 crc = 0xffffffff;
    for (u32 i = 0; i < size; ++i)
    {
        crc ^= data[i];
        crc = (crc >> 4) ^ table[crc & 0xf];
        crc = (crc >> 4) ^ table[crc & 0xf];
    }
    return ~crc;
}

static int enemyOverlayAddress(u32 value, u32 vram, u32 size)
{
    return value >= vram && value - vram < size;
}

static int enemyOverlayRelocate(u8* dst, u32 capacity, u32 fileSize, u32 vram, u32 memorySize, u32 profileOffset)
{
    u32 footerSize;
    u32 footerOffset;
    u32* footer;
    u32 sectionBase[4];
    u32 sectionSize[4];
    u32* hiRefs[32];
    u32 hiValues[32];
    u32 hiUsed[32];
    u32 hiRelocated[32];
    u32 count;
    u32 ram;
    u32 initializedSize;
    u32* word;
    u32 relocation;
    u32 section;
    u32 offset;
    u32 type;
    u32 reg;
    u32 value;
    u32 relocated;

    if (fileSize < 0x1c || (fileSize & 3) || fileSize > capacity)
        return 0;
    footerSize = *(u32*)(dst + fileSize - 4);
    if (footerSize < 0x18 || footerSize > fileSize || (footerSize & 3))
        return 0;
    footerOffset = fileSize - footerSize;
    footer = (u32*)(dst + footerOffset);
    if ((footer[0] | footer[1] | footer[2]) & 3)
        return 0;
    if (footer[0] > footerOffset || footer[1] > footerOffset - footer[0] ||
        footer[2] != footerOffset - footer[0] - footer[1])
        return 0;
    initializedSize = footerOffset;
    if (initializedSize < 0x20 || profileOffset < footer[0] || profileOffset > initializedSize - 0x20)
        return 0;
    if (footer[3] > capacity - fileSize || fileSize + footer[3] != memorySize)
        return 0;
    count = footer[4];
    if (count > (footerSize - 0x18) / 4)
        return 0;
    sectionBase[0] = 0;
    sectionBase[1] = 0;
    sectionBase[2] = footer[0];
    sectionBase[3] = footer[0] + footer[1];
    sectionSize[0] = 0;
    sectionSize[1] = footer[0];
    sectionSize[2] = footer[1];
    sectionSize[3] = footer[2];
    memset(hiRefs, 0, sizeof(hiRefs));
    memset(hiUsed, 0, sizeof(hiUsed));
    ram = (u32)dst;

    for (u32 i = 0; i < count; ++i)
    {
        relocation = footer[5 + i];
        section = relocation >> 30;
        offset = relocation & 0xffffff;
        type = (relocation >> 24) & 0x3f;
        if (section == 0 || (offset & 3) || sectionSize[section] < 4 || offset > sectionSize[section] - 4)
            return 0;
        word = (u32*)(dst + sectionBase[section] + offset);
        switch (type)
        {
        case 2:
            if (!enemyOverlayAddress(*word, vram, memorySize))
                return 0;
            *word = *word - vram + ram;
            break;
        case 4:
            if ((*word >> 26) != 2 && (*word >> 26) != 3)
                return 0;
            value = 0x80000000 | ((*word & 0x03ffffff) << 2);
            if (!enemyOverlayAddress(value, vram, memorySize))
                return 0;
            *word = (*word & 0xfc000000) | (((value - vram + ram) >> 2) & 0x03ffffff);
            break;
        case 5:
            if ((*word >> 26) != 0x0f)
                return 0;
            reg = (*word >> 16) & 31;
            if (hiRefs[reg] && !hiUsed[reg])
                return 0;
            hiRefs[reg] = word;
            hiValues[reg] = *word;
            hiUsed[reg] = 0;
            break;
        case 6:
            reg = (*word >> 21) & 31;
            if (!hiRefs[reg])
                return 0;
            value = (hiValues[reg] << 16) + (s16)*word;
            if (!enemyOverlayAddress(value, vram, memorySize))
                return 0;
            relocated = value - vram + ram;
            value = ((relocated + 0x8000) >> 16) & 0xffff;
            if (hiUsed[reg] && hiRelocated[reg] != value)
                return 0;
            hiRelocated[reg] = value;
            *hiRefs[reg] = (hiValues[reg] & 0xffff0000) | value;
            *word = (*word & 0xffff0000) | (relocated & 0xffff);
            hiUsed[reg] = 1;
            break;
        default:
            return 0;
        }
    }
    for (u32 i = 0; i < 32; ++i)
        if (hiRefs[i] && !hiUsed[i])
            return 0;
    memset(dst + fileSize, 0, memorySize - fileSize);
    return 1;
}

ActorProfile* comboLoadForeignEnemyOverlay(void* dst, u32 capacity,
                                           const ForeignEnemyOverlay* overlay)
{
    u32 vrom;
    ActorProfile* profile;
    DmaEntry entry;

    if (!dst || !overlay || ((u32)dst & 15) || !overlay->sourceVrom ||
        overlay->memorySize > capacity || overlay->fileSize > capacity ||
        overlay->fileSize < 0x20 || (overlay->fileSize & 3) ||
        overlay->memorySize > 0x800000 || (overlay->vramStart & 3) ||
        overlay->vramStart < 0x80000000 || overlay->vramStart >= 0x90000000 ||
        overlay->vramStart + overlay->memorySize < overlay->vramStart ||
        overlay->profileOffset > overlay->fileSize - 0x20 || (overlay->profileOffset & 3) ||
        overlay->reserved)
        return NULL;
    if (overlay->sourceVrom < ENEMY_ARCHIVE_VROM_START || overlay->sourceVrom >= ENEMY_ARCHIVE_VROM_END ||
        overlay->fileSize > ENEMY_ARCHIVE_VROM_END - overlay->sourceVrom)
        return NULL;
    vrom = overlay->sourceVrom;
    if (!comboDmaLookup(&entry, vrom) || entry.vstart != vrom ||
        entry.vend - entry.vstart != overlay->fileSize)
        return NULL;
    if (comboDmaLoadFilePartial(dst, vrom, 0, overlay->fileSize) != overlay->fileSize)
        return NULL;

    if (comboEnemyCrc32(dst, overlay->fileSize) != overlay->sourceCrc32)
        return NULL;

    if (!enemyOverlayRelocate(dst, capacity, overlay->fileSize, overlay->vramStart, overlay->memorySize, overlay->profileOffset))
        return NULL;
    profile = (ActorProfile*)((u8*)dst + overlay->profileOffset);
    if (profile->instanceSize < sizeof(Actor) || profile->instanceSize > 0xffff || !profile->init || !profile->update)
        return NULL;
    osWritebackDCache(dst, overlay->memorySize);
    osInvalICache(dst, overlay->memorySize);
    return profile;
}

EXPORT_SYMBOL(FOREIGN_ENEMY_NATIVE_PORTS_V1, comboLoadForeignEnemyOverlay);

void comboEnemyHintEnter(Actor* actor, u8 source)
{
    if (actor)
        actor->hintId = source;
}

u8 comboEnemyHintLeave(Actor* actor)
{
    u8 source;
    if (!actor)
        return 0xff;
    source = actor->hintId;
    actor->hintId = 0xff;
    return source;
}

#if defined(GAME_OOT)
# define ENEMY_DATA_CAPACITY 0x1000
#else
# define ENEMY_DATA_CAPACITY 0x40
#endif
static u8 sEnemyData[ENEMY_DATA_CAPACITY] __attribute__((aligned(16)));
static u32 sLoadedEnemyData;
u32 gForeignEnemyDataVrom __attribute__((section(".data"))) = 0;
#if defined(GAME_OOT)
# define ENEMY_DATA_NATIVE_GAME 0
_Static_assert(sizeof(gMmSave.day) == 4, "MM day scalar width");
EXPORT_SYMBOL(FOREIGN_ENEMY_STATE_MM_DAY, gMmSave.day);
EXPORT_SYMBOL(FOREIGN_ENEMY_DATA_BASE_OOT, sEnemyData);
EXPORT_SYMBOL(FOREIGN_ENEMY_DATA_CONFIG_OOT, gForeignEnemyDataVrom);
#else
# define ENEMY_DATA_NATIVE_GAME 1
EXPORT_SYMBOL(FOREIGN_ENEMY_DATA_BASE_MM, sEnemyData);
EXPORT_SYMBOL(FOREIGN_ENEMY_DATA_CONFIG_MM, gForeignEnemyDataVrom);
#endif

int comboEnemyLoadRomData(void)
{
    DmaEntry file;
    u32 header[5];
    u32 bitCount = 0;
    u32 payloadOffset = sizeof(header);
    if (!gForeignEnemyDataVrom)
        return 1;
    if (sLoadedEnemyData == gForeignEnemyDataVrom)
        return 1;
    if (gForeignEnemyDataVrom < 0x08000000 || !comboDmaLookup(&file, gForeignEnemyDataVrom) ||
        file.vstart != gForeignEnemyDataVrom || file.vend < file.vstart || file.pend ||
        file.vend - file.vstart < sizeof(header) ||
        comboDmaLoadFilePartial(header, gForeignEnemyDataVrom, 0, sizeof(header)) != sizeof(header) ||
        header[0] != 0x454e4441 || (header[1] != 1 && header[1] != 2) || header[2] != ENEMY_DATA_NATIVE_GAME ||
        !header[3] || header[3] > sizeof(sEnemyData) || (header[3] & 15) ||
        file.vend - file.vstart < sizeof(header) + header[3])
        return 0;
    if (header[1] == 2)
    {
        if (comboDmaLoadFilePartial(&bitCount, gForeignEnemyDataVrom, sizeof(header), sizeof(bitCount)) != sizeof(bitCount) ||
            !bitCount || bitCount > ENEMY_DATA_CAPACITY / 128)
            return 0;
        payloadOffset += sizeof(bitCount) + bitCount * 12;
    }
    if (file.vend - file.vstart != payloadOffset + header[3])
        return 0;
    if (comboDmaLoadFilePartial(sEnemyData, gForeignEnemyDataVrom, payloadOffset, header[3]) != header[3] ||
        comboEnemyCrc32(sEnemyData, header[3]) != header[4])
        goto invalid;
    for (u32 block = 0; block < bitCount; ++block)
    {
        u32 record[3];
        if (comboDmaLoadFilePartial(record, gForeignEnemyDataVrom, 24 + block * 12, sizeof(record)) != sizeof(record) ||
            record[2] != 5 || record[1] != 128 || (record[0] & 3) || header[3] < 128 || record[0] > header[3] - 128)
            goto invalid;
        const u32* words = (const u32*)(sEnemyData + record[0]);
        for (u32 bit = 0; bit < 32; ++bit)
            if (words[bit] != (1u << bit))
                goto invalid;
    }
    sLoadedEnemyData = gForeignEnemyDataVrom;
    return 1;
invalid:
    memset(sEnemyData, 0, sizeof(sEnemyData));
    sLoadedEnemyData = 0;
    return 0;
}
EXPORT_SYMBOL(FOREIGN_ENEMY_DATA_BITS_V2, comboEnemyLoadRomData);
