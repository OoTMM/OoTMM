#include <combo.h>
#include <combo/souls.h>
#include <combo/dma.h>
#include <combo/global.h>
#include <combo/rand.h>
#include <combo/enemy_placement.h>
#include <combo/enemy_ground.h>
#include <combo/enemy_soul_preview.h>
#include <combo/enemy.h>
#include <combo/enemy_room_models.h>
#include <combo/enemy_resource.h>
#include <combo/enemy_native.h>
#if defined(GAME_OOT)
#include <combo/actor_ovl.h>
#include <combo/enemy_oot_enemizer.h>
#endif
#if defined(GAME_MM)
#include <combo/enemy_scripted.h>
#endif

typedef struct
{
    u32 vrom;
    u32 operation;
    ActorEntry original;
    ActorEntry replacement;
    s16 targetId;
    u16 targetParams;
} NativePlacement;

_Static_assert(sizeof(NativePlacement) == 44, "native placement metadata ABI");

typedef struct
{
    Actor* actor;
    ActorFunc init;
    ActorFunc update;
    ActorFunc fini;
    u32 visit;
    s16 room;
    u32 splitPlacementVrom;
#if defined(GAME_OOT)
    s16 targetId;
    u16 targetParams;
#endif
    u8 operation;
    u8 hadHealth;
    u8 virtualFloor;
#if defined(GAME_OOT)
    u32 generation;
    u8 arenaDefeated;
    u8 arenaBlocked;
    u8 deadHandHandsPlaced;
#endif
    EnemyNativeResource resource;
} NativePlacementState;

#if defined(GAME_OOT)
static u32 sNativeArenaGeneration;
#endif

#define NATIVE_PLACEMENT_STATE_CAPACITY 64
static NativePlacementState* sNativeStates;
static EnemyFamily* sNativeFamilies;

static int nativePlacementAllocStates(void)
{
    if (!sNativeStates)
    {
        u32 bytes = NATIVE_PLACEMENT_STATE_CAPACITY * (sizeof(*sNativeStates) + sizeof(*sNativeFamilies));
        sNativeStates = malloc(bytes);
        if (!sNativeStates)
            return 0;
        memset(sNativeStates, 0, bytes);
        sNativeFamilies = (EnemyFamily*)(sNativeStates + NATIVE_PLACEMENT_STATE_CAPACITY);
    }
    return 1;
}
#define NATIVE_PLACEMENT_STATE_COUNT (sNativeStates ? NATIVE_PLACEMENT_STATE_CAPACITY : 0)
static u32 sNativeVisits[32];
static u32 sNativeBlockedVisits[32];
static NativePlacementState* sNativeScope;

int comboEnemyNativeMusicSuppressed(void)
{
    if (!sNativeScope || sNativeScope->operation == ENEMY_NATIVE_ARENA)
        return 0;
#if defined(GAME_MM)
    if (sNativeScope->operation == ENEMY_SCRIPTED_MM)
        return 0;
#endif
    return 1;
}

static u32 sNativePlacementVrom __attribute__((section(".data"))) = 0;
static u32 sChaoticPlacementVrom __attribute__((section(".data"))) = 0;
static u32 sVirtualFloorPlacementVrom __attribute__((section(".data"))) = 0;
#if defined(GAME_OOT)
EXPORT_SYMBOL(ENEMY_VIRTUAL_FLOOR_PLACEMENTS_OOT_V1, sVirtualFloorPlacementVrom);
#else
EXPORT_SYMBOL(ENEMY_VIRTUAL_FLOOR_PLACEMENTS_MM_V1, sVirtualFloorPlacementVrom);
#endif

static int virtualFloorPlacement(u32 key)
{
    u32 count;
    if (!sVirtualFloorPlacementVrom || comboDmaLoadFilePartial(&count, sVirtualFloorPlacementVrom, 0, 4) != 4 || count > 4096)
        return 0;
    u32 low = 0, high = count;
    while (low < high)
    {
        u32 mid = low + (high - low) / 2, value;
        if (comboDmaLoadFilePartial(&value, sVirtualFloorPlacementVrom, 4 + mid * 4, 4) != 4)
            return 0;
        if (value == key) return 1;
        if (value < key) low = mid + 1;
        else high = mid;
    }
    return 0;
}
#if defined(GAME_OOT)
#define NativePlacementSpawnEntry ((Actor * (*)(ActorContext*, ActorEntry*, PlayState*))0x800255c4)
#define NATIVE_PLACEMENT_GAME     0u
EXPORT_SYMBOL(ENEMY_NATIVE_PLACEMENTS_OOT, sNativePlacementVrom);
EXPORT_SYMBOL(ENEMY_CHAOTIC_PLACEMENTS_OOT, sChaoticPlacementVrom);
#else
#define NativePlacementSpawnEntry ((Actor * (*)(ActorContext*, ActorEntry*, PlayState*))0x800bb2d0)
#define NATIVE_PLACEMENT_GAME     1u
EXPORT_SYMBOL(ENEMY_NATIVE_PLACEMENTS_MM, sNativePlacementVrom);
EXPORT_SYMBOL(ENEMY_CHAOTIC_PLACEMENTS_MM, sChaoticPlacementVrom);
#endif
EXPORT_SYMBOL(ENEMY_NATIVE_PLACEMENTS_V1, comboEnemyNativePlacementSpawn);
EXPORT_SYMBOL(ENEMY_GROUND_PLACEMENT_V1, comboEnemyNativePlacementSpawn);
EXPORT_SYMBOL(ENEMY_NATIVE_PLACEMENT_TRAITS_V1, comboEnemyNativePlacementSpawn);

static NativePlacement* sNativeRows;
static u32 sNativeRowCount;
static u32 sNativeRowsVrom;
static u32 sNativeFirstKey;
static u32 sNativeLastKey;
static u8 sNativeRowsValid;
static int nativePlacementHeader(u32* count);
typedef struct
{
    ALIGNED(16) NativePlacement rows[16];
    u32 first;
    u32 count;
} NativePlacementReadWindow;

static s32 nativePlacementLowerBound(u32 key, u32 count, NativePlacementReadWindow* window);

static int nativePlacementLookup(u32 vrom, NativePlacement* out)
{
    u32 count, low = 0, high, mid;
    if (sNativeRowsValid && sNativeRowsVrom == sNativePlacementVrom && vrom >= sNativeFirstKey && vrom <= sNativeLastKey)
    {
        high = sNativeRowCount;
        while (low < high)
        {
            mid = low + (high - low) / 2;
            if (sNativeRows[mid].vrom < vrom)
                low = mid + 1;
            else if (sNativeRows[mid].vrom > vrom)
                high = mid;
            else
            {
                *out = sNativeRows[mid];
                return 1;
            }
        }
        return 0;
    }
    if (!nativePlacementHeader(&count))
        return 0;
    NativePlacementReadWindow window;
    window.first = window.count = 0;
    s32 index = nativePlacementLowerBound(vrom, count, &window);
    if (index < 0 || (u32)index >= count)
        return 0;
    if ((u32)index >= window.first && (u32)index - window.first < window.count)
        *out = window.rows[index - window.first];
    else if (comboDmaLoadFilePartial(out, sNativePlacementVrom, 16 + index * sizeof(*out), sizeof(*out)) != sizeof(*out))
        return 0;
    return out->vrom == vrom;
}

static int nativePlacementHeader(u32* count)
{
    DmaEntry file;
    u32 header[4];
    if (!sNativePlacementVrom || !comboDmaLookup(&file, sNativePlacementVrom) ||
        file.vstart != sNativePlacementVrom || file.pend || file.vend < file.vstart || file.vend - file.vstart < 16 ||
        comboDmaLoadFilePartial(header, sNativePlacementVrom, 0, 16) != 16 ||
        header[0] != 0x454e5031 || header[1] != 1 || header[2] != NATIVE_PLACEMENT_GAME ||
        header[3] > 0x10000 || file.vend - file.vstart != 16 + header[3] * sizeof(NativePlacement))
        return 0;
    *count = header[3];
    return 1;
}

static s32 nativePlacementLowerBound(u32 key, u32 count, NativePlacementReadWindow* window)
{
    u32 low = 0, high = count;
    while (low < high)
    {
        u32 mid = low + (high - low) / 2;
        if (mid < window->first || mid - window->first >= window->count)
        {
            /* Read the remaining candidates together once the search narrows. */
            u32 first = mid;
            u32 rows = 1;
            if (high - low <= ARRAY_COUNT(window->rows))
            {
                first = low;
                rows = high - low;
            }
            u32 bytes = rows * sizeof(*window->rows);
            if (comboDmaLoadFilePartial(window->rows, sNativePlacementVrom, 16 + first * sizeof(*window->rows), bytes) != bytes)
                return -1;
            window->first = first;
            window->count = rows;
        }
        if (window->rows[mid - window->first].vrom < key)
            low = mid + 1;
        else
            high = mid;
    }
    return low;
}

static void nativePlacementClearRows(void)
{
    free(sNativeRows);
    sNativeRows = NULL;
    sNativeRowsValid = 0;
    sNativeRowCount = 0;
}

void comboEnemyNativePlacementSetActorList(PlayState* play, ActorEntry* list, u32 count)
{
    u32 first, last, tableCount, stride = sizeof(*list);
    s32 begin, end, room;
    NativePlacement* rows = NULL;
    nativePlacementClearRows();
    if (!sNativePlacementVrom || !play || !list || !count || count > 255)
        return;
#if defined(GAME_OOT)
    if (comboMqEnemyPlacementKey(play, list, &first))
    {
        if (!comboMqEnemyPlacementKey(play, list + count - 1, &last))
            return;
        stride = 1;
    }
    else
#endif
    {
        room = play->roomCtx.curRoom.num;
        u32 segment = (u32)play->roomCtx.curRoom.segment;
        if (room < 0 || room >= play->numRooms || !play->roomList || !segment || (u32)list < segment)
            return;
        RomFile* files = play->roomList;
        u32 start = (u32)files[room].vromStart, finish = (u32)files[room].vromEnd;
        u32 offset = (u32)list - segment;
        if (!start || finish < start || offset > finish - start || count * sizeof(*list) > finish - start - offset)
            return;
        first = start + offset;
#if defined(GAME_MM)
        if (play->sceneId == SCE_MM_GROTTOS && room == 4 && (gGrottoData & 0x1f) >= 0x13)
            first = 0xe0000000u + ((gGrottoData & 0x1f) - 0x13) * 0x10000 + offset;
#endif
        last = first + (count - 1) * sizeof(*list);
    }
    if (last < first || last == 0xffffffffu || last - first != (count - 1) * stride || !nativePlacementHeader(&tableCount))
        return;
    NativePlacementReadWindow window;
    window.first = window.count = 0;
    begin = nativePlacementLowerBound(first, tableCount, &window);
    end = nativePlacementLowerBound(last + 1, tableCount, &window);
    if (begin < 0 || end < begin || (u32)(end - begin) > count)
        return;
    u32 found = end - begin;
    if (found)
    {
        rows = malloc(found * sizeof(*rows));
        if (!rows)
            return;
        if (comboDmaLoadFilePartial(rows, sNativePlacementVrom, 16 + begin * sizeof(*rows), found * sizeof(*rows)) != found * sizeof(*rows))
        {
            free(rows);
            return;
        }
        for (u32 i = 0; i < found; ++i)
            if (rows[i].vrom < first || rows[i].vrom > last || (rows[i].vrom - first) % stride || (i && rows[i - 1].vrom >= rows[i].vrom))
            {
                free(rows);
                return;
            }
    }
    sNativeRows = rows;
    sNativeRowCount = found;
    sNativeRowsVrom = sNativePlacementVrom;
    sNativeFirstKey = first;
    sNativeLastKey = last;
    sNativeRowsValid = 1;
}

void comboEnemyNativePlacementReset(void)
{
    nativePlacementClearRows();
    if (sNativeFamilies)
        for (u32 i = 0; i < NATIVE_PLACEMENT_STATE_CAPACITY; ++i)
            comboEnemyFamilyClear(&sNativeFamilies[i]);
    free(sNativeStates);
    sNativeStates = NULL;
    sNativeFamilies = NULL;
    memset(sNativeVisits, 0, sizeof(sNativeVisits));
    memset(sNativeBlockedVisits, 0, sizeof(sNativeBlockedVisits));
    sNativeScope = NULL;
#if defined(GAME_OOT)

    sNativeArenaGeneration = 0;
#endif
}

void comboEnemyNativePlacementBeginRoom(PlayState* play)
{
    nativePlacementClearRows();
    s32 room = play->roomCtx.curRoom.num;
    if (room < 0 || room >= 32)
        return;
    if (sNativeVisits[room] != 0xffffffffu)
        ++sNativeVisits[room];
    for (u32 i = 0; i < NATIVE_PLACEMENT_STATE_COUNT; ++i)
        if (!sNativeStates[i].actor && sNativeStates[i].resource.state &&
            (sNativeStates[i].room != room || sNativeStates[i].visit != sNativeVisits[room]))
            memset(&sNativeStates[i], 0, sizeof(sNativeStates[i]));
}

#if defined(GAME_MM)
void comboEnemyNativePlacementBlock(s32 room)
{
    if (room >= 0 && room < 32 && sNativeVisits[room])
        sNativeBlockedVisits[room] = sNativeVisits[room];
}

EXPORT_SYMBOL(ENEMY_SCRIPTED_MM_V1, comboEnemyNativePlacementBlock);
#endif

int comboEnemyNativePlacementPending(s32 room)
{
    if (room < 0 || room >= 32 || !sNativeVisits[room])
        return 0;
    if (sNativeBlockedVisits[room] == sNativeVisits[room])
        return 1;
    for (u32 i = 0; i < NATIVE_PLACEMENT_STATE_COUNT; ++i)
    {
        NativePlacementState* state = &sNativeStates[i];
        if (state->room != room || state->visit != sNativeVisits[room])
            continue;
#if defined(GAME_MM)
        if (state->operation == 5 || ((state->operation == 15 || state->operation == 21 || state->operation == ENEMY_NATIVE_GARAHNA) && state->init))
            return 1;
        if (state->operation == ENEMY_SCRIPTED_MM && state->actor && ((EnemyScriptedBinding*)state->actor)->pending)
            return 1;
#endif
        if (state->operation == ENEMY_NATIVE_RESOURCE &&
            (state->resource.state == ENEMY_RESOURCE_WAITING || state->resource.state == ENEMY_RESOURCE_PENDING))
            return 1;
#if defined(GAME_OOT)
        if (state->operation == ENEMY_NATIVE_ARENA && (!state->arenaDefeated || state->arenaBlocked))
            return 1;
#endif
    }
    return 0;
}

static NativePlacementState* nativePlacementState(Actor* actor)
{
    for (u32 i = 0; i < NATIVE_PLACEMENT_STATE_COUNT; ++i)
        if (sNativeStates[i].actor == actor)
            return &sNativeStates[i];
    return NULL;
}
int comboEnemyFlyingAttackInRange(Actor* actor, PlayState* play, f32 range)
{
    Vec3f* player = &GET_PLAYER(play)->actor.world.pos;
    f32 dx = player->x - actor->world.pos.x;
    f32 dy = player->y - actor->world.pos.y;
    f32 dz = player->z - actor->world.pos.z;
    return dx * dx + dy * dy + dz * dz < range * range;
}

void comboEnemyFlyingAttackAim(Actor* actor, PlayState* play, int launch)
{
    Vec3f* player = &GET_PLAYER(play)->actor.world.pos;
    f32 dx = player->x - actor->world.pos.x;
    f32 dy = player->y + 30.f - actor->world.pos.y;
    f32 dz = player->z - actor->world.pos.z;
    f32 xz = sqrtf(dx * dx + dz * dz);
    f32 distance = sqrtf(dx * dx + dy * dy + dz * dz);
    if (distance < 1.f)
        return;
    if (launch)
    {
        actor->world.rot.y = actor->yawTowardsPlayer;
        actor->speed = 10.f * xz / distance;
        actor->velocity.y = 10.f * dy / distance;
    }
    else
    {
        /* Half the yaw correction and vertical tracking, without slowing flight. */
        Math_SmoothStepToS(&actor->world.rot.y, actor->yawTowardsPlayer, 4, 0x400, 1);
        f32 targetY = 10.f * dy / distance;
        actor->velocity.y += 0.5f * (targetY - actor->velocity.y);
        actor->velocity.y = MAX(-10.f, MIN(10.f, actor->velocity.y));
        actor->speed = sqrtf(MAX(0.f, 100.f - SQ(actor->velocity.y)));
    }
}

int comboEnemyNativePlacementIsShuffled(Actor* actor)
{
    return actor != NULL && nativePlacementState(actor) != NULL;
}
#if defined(GAME_MM)
static void shuffledDexihandThrow(PlayState* play, Actor* actor, f32 speed, s16 yaw, f32 lift, u32 damage)
{
    if (actor->params == 0 && sNativeScope && sNativeScope->actor == actor)
    {
        speed = lift = 8.f;
        yaw = Rand_Next() >> 16;
        Vec3f* pos = &GET_PLAYER(play)->actor.world.pos;
        pos->x += 2.f * (speed * Math_SinS(yaw) - actor->velocity.x);
        pos->y += 2.f * (lift - actor->velocity.y);
        pos->z += 2.f * (speed * Math_CosS(yaw) - actor->velocity.z);
        speed *= 2.f;
        lift *= 1.5f;
    }
    ((void (*)(PlayState*, Actor*, f32, s16, f32, u32))0x800b8d50)(play, actor, speed, yaw, lift, damage);
}

PATCH_CALL(0x80af55f4, shuffledDexihandThrow);
#endif

static int nativePlacementCurrent(NativePlacementState* state, PlayState* play)
{
    return state && state->room >= 0 && state->room < 32 &&
           state->room == play->roomCtx.curRoom.num && state->visit == sNativeVisits[state->room];
}

#if defined(GAME_MM)
int comboEnemyNativeScriptedCurrent(Actor* monitor, PlayState* play)
{
    NativePlacementState* state = nativePlacementState(monitor);
    return state && state->operation == ENEMY_SCRIPTED_MM && nativePlacementCurrent(state, play);
}
#endif

Actor* comboEnemyNativeArenaOwner(PlayState* play)
{
    return sNativeScope && sNativeScope->operation == ENEMY_NATIVE_ARENA ? sNativeScope->actor : NULL;
}
#if defined(GAME_OOT)
Actor* comboEnemyNativeDryAquaticOwner(PlayState* play, f32* originY)
{
    if (!sNativeScope || sNativeScope->operation != ENEMY_NATIVE_DRY_AQUATIC ||
        !nativePlacementCurrent(sNativeScope, play))
        return NULL;
    *originY = sNativeScope->resource.original.pos.y;
    if (sNativeScope->actor)
        return sNativeScope->actor;
    Actor* spawned = play->actorCtx.actors[ACTORCAT_ENEMY].first;
    return spawned && spawned->id == sNativeScope->resource.original.id ? spawned : NULL;
}
#else
static int nativeDrySkullfish(void)
{
    return sNativeScope && sNativeScope->operation == ENEMY_NATIVE_DRY_MM_AQUATIC &&
           (sNativeScope->resource.original.id & 0xfff) == ACTOR_EN_PR2;
}

static void nativeDrySkullfishBg(PlayState* play, Actor* actor, f32 height, f32 radius, f32 ceiling, s32 flags)
{
    Actor_UpdateBgCheckInfo(play, actor, height, radius, ceiling, flags);
    if (nativeDrySkullfish() && !(actor->bgCheckFlags & BGCHECKFLAG_WATER))
    {
        actor->bgCheckFlags |= BGCHECKFLAG_WATER;
        actor->depthInWater = actor->home.pos.y + 60.f - actor->world.pos.y;
    }
}

static s32 nativeDrySkullfishWater(PlayState* play, CollisionContext* col, f32 x, f32 z, f32* y, void** box)
{
    s32 water = ((s32(*)(PlayState*, CollisionContext*, f32, f32, f32*, void**))0x800ca1ac)(play, col, x, z, y, box);
    if (water || !nativeDrySkullfish())
        return water;
    *y = sNativeScope->resource.original.pos.y + 60.f;
    return 1;
}

PATCH_CALL(0x80a741f8, nativeDrySkullfishBg);
PATCH_CALL(0x80a75780, nativeDrySkullfishBg);
PATCH_CALL(0x80a750c4, nativeDrySkullfishWater);
PATCH_CALL(0x80a7547c, nativeDrySkullfishWater);

static s32 shuffledHiploopWater(PlayState* play, CollisionContext* col, f32 x, f32 z, f32* y, WaterBox** box)
{
    if (sNativeScope && sNativeScope->actor && sNativeScope->actor->id == ACTOR_EN_PP)
        return 0;
    return WaterBox_GetSurface1(play, col, x, z, y, box);
}

/* EnPp_Update uses this query to trigger instant death below the surface. */
PATCH_CALL(0x80b20c7c, shuffledHiploopWater);
#endif
int comboEnemyNativeArenaCurrent(PlayState* play)
{
    return sNativeScope && sNativeScope->operation == ENEMY_NATIVE_ARENA && sNativeScope->actor &&
           nativePlacementCurrent(sNativeScope, play);
}

int comboEnemyNativeArenaTarget(PlayState* play, s16* actorId, u16* params)
{
#if defined(GAME_OOT)
    if (!actorId || !params || !comboEnemyNativeArenaOwner(play))
        return 0;
    *actorId = sNativeScope->targetId;
    *params = sNativeScope->targetParams;
    return 1;
#else
    return 0;
#endif
}

static void nativeFamilyComplete(EnemyFamily* family, PlayState* play)
{
    NativePlacementState* owner = &sNativeStates[family - sNativeFamilies];
    if (!nativePlacementCurrent(owner, play))
        return;
    if (owner->operation == ENEMY_NATIVE_RESOURCE)
    {
        if (comboEnemyFamilyDone(family))
            comboEnemyNativeResourceDefeated(&owner->resource);
        else
            sNativeBlockedVisits[owner->room] = owner->visit;
    }
#if defined(GAME_OOT)
    else if (owner->operation == ENEMY_NATIVE_ARENA)
    {
        owner->arenaDefeated = comboEnemyFamilyDone(family);
        owner->arenaBlocked = family->blocked;
    }
#endif
}

#if defined(GAME_OOT)
Actor* comboEnemyNativeArenaSpawn(ActorContext* ctx, ActorEntry* entry, PlayState* play)
{
    NativePlacementState* owner = sNativeScope;
    if (!owner || owner->operation != ENEMY_NATIVE_ARENA || !nativePlacementCurrent(owner, play))
        return NULL;
    EnemyFamily* family = &sNativeFamilies[owner - sNativeStates];
    family->complete = nativeFamilyComplete;
    if (owner->virtualFloor && !comboEnemyFamilySetVirtualFloor(family, play, entry)) return NULL;
    return comboEnemyFamilySpawn(family, ctx, entry, play, 0xffff);
}

int comboEnemyNativeArenaTrackTarget(PlayState* play, Actor* actor)
{
    return actor && sNativeScope && sNativeFamilies[sNativeScope - sNativeStates].started;
}

EXPORT_SYMBOL(ENEMY_NATIVE_ARENA_LIFETIME_V1, comboEnemyNativeArenaTrackTarget);

int comboEnemyNativeArenaDefeated(PlayState* play)
{
    return sNativeScope && sNativeScope->operation == ENEMY_NATIVE_ARENA && nativePlacementCurrent(sNativeScope, play) &&
           sNativeScope->arenaDefeated && !sNativeScope->arenaBlocked;
}
#endif

u32 comboEnemyNativeSplitKey(PlayState* play, s16 actorId)
{
    if (!sNativeScope || !sNativeScope->actor || sNativeScope->actor->id != actorId ||
        !nativePlacementCurrent(sNativeScope, play))
        return 0;
    return sNativeScope->splitPlacementVrom;
}

#if defined(GAME_OOT)
int comboEnemyShuffledDeadHand(Actor* actor, PlayState* play)
{
    NativePlacementState* state = actor && actor->id == ACTOR_EN_DH ? nativePlacementState(actor) : NULL;
    return state && nativePlacementCurrent(state, play);
}

static void shuffledDeadHandBgCheck(PlayState* play, Actor* actor, f32 wallHeight, f32 wallRadius,
                                   f32 ceilingHeight, s32 flags)
{
    if (comboEnemyShuffledDeadHand(actor, play))
    {
        wallRadius = 24.f;
        flags &= ~(1 << 4);
    }
    Actor_UpdateBgCheckInfo(play, actor, wallHeight, wallRadius, ceilingHeight, flags);
}

PATCH_CALL(0x8096ce74, shuffledDeadHandBgCheck);

int comboEnemyNativePlacementActive(Actor* actor)
{
    return sNativeScope && sNativeScope->actor == actor && nativePlacementCurrent(sNativeScope, gPlay);
}
#endif

int comboEnemyNativePlacementOrigin(Actor* actor, Vec3f* origin)
{
    NativePlacementState* state = nativePlacementState(actor);
    if (!state || state->operation != 17 || !origin)
        return 0;
    origin->x = state->resource.original.pos.x;
    origin->y = state->resource.original.pos.y;
    origin->z = state->resource.original.pos.z;
    return 1;
}

int comboEnemyNativeAirBound(Actor* actor)
{
    NativePlacementState* state = nativePlacementState(actor);
    return state && actor->colChkInfo.health && actor->update && (state->hadHealth & 2) && nativePlacementCurrent(state, gPlay);
}

#if defined(GAME_OOT)
#define DEAD_HAND_SPAWN_ARM Actor_SpawnAsChild
#include "ovl/dead_hand_placement.c"
#undef DEAD_HAND_SPAWN_ARM
#endif

static void nativePlacementUpdate(Actor*, PlayState*);
static void nativePlacementDefeated(Actor*, PlayState*);
#if defined(GAME_MM)
static Actor* nativePlacementForeignSpawn(ActorContext*, ActorEntry*, PlayState*);
#endif
static void nativePlacementFini(Actor*, PlayState*);

static void nativePlacementInit(Actor* actor, PlayState* play)
{
    NativePlacementState* state = nativePlacementState(actor);
    NativePlacementState* previous = sNativeScope;
    if (!state)
    {
        Actor_Kill(actor);
        return;
    }
    actor->init = state->init;
    actor->update = state->update;
    actor->fini = state->fini;
    sNativeScope = state;
    if (state->hadHealth & 2)
        comboEnemyAirPlacement(play, actor, 40.f);
    if (state->init)
        state->init(actor, play);
    sNativeScope = previous;
    state->init = NULL;
    if (state->operation == 15 && actor->update)
        Actor_ChangeCategory(play, &play->actorCtx, actor, ACTORCAT_ENEMY);
#if defined(GAME_OOT)
    if (actor->id == ACTOR_EN_TORCH2 && actor->update)
        Actor_ChangeCategory(play, &play->actorCtx, actor, ACTORCAT_ENEMY);
#endif
    state->update = actor->update;
    state->fini = actor->fini;
    state->hadHealth |= actor->colChkInfo.health != 0;
    actor->init = NULL;
    if (!actor->update)
    {
        if (state->operation == ENEMY_NATIVE_RESOURCE && state->resource.original.id != 0x197)
            comboEnemyNativeResourceDefeated(&state->resource);
#if defined(GAME_MM)
        else if (state->operation == ENEMY_NATIVE_GARAHNA || state->operation == 21 ||
                 (state->operation == ENEMY_NATIVE_DRY_MM_AQUATIC && actor->id == 0x23f))
        {
            comboEnemyNativePlacementBlock(state->room);
            g.roomEnemyLackSoul = 1;
        }
#endif
#if defined(GAME_OOT)
        else if (state->operation == ENEMY_NATIVE_ARENA)
            state->arenaBlocked = 1;
#endif
    }
    else
        actor->update = nativePlacementUpdate;
    actor->fini = nativePlacementFini;
}

static void nativePlacementFini(Actor* actor, PlayState* play)
{
    NativePlacementState* state = nativePlacementState(actor);
    NativePlacementState* previous = sNativeScope;
    if (!state)
        return;
    sNativeScope = state;
    comboEnemyFamilyClear(&sNativeFamilies[state - sNativeStates]);
    actor->fini = state->fini;
    if (!state->init && state->fini)
        state->fini(actor, play);
    sNativeScope = previous;
    if (state->operation == ENEMY_NATIVE_RESOURCE && state->resource.state == ENEMY_RESOURCE_PENDING && nativePlacementCurrent(state, play))
        comboEnemyNativeResourceUpdate(play, &state->resource);
    if (state->room >= 0 && state->room < 32 && state->visit == sNativeVisits[state->room] &&
        ((state->operation == ENEMY_NATIVE_RESOURCE && state->resource.state != ENEMY_RESOURCE_RESTORED)
#if defined(GAME_OOT)
         || (state->operation == ENEMY_NATIVE_ARENA && !state->arenaDefeated)
#else
         || state->operation == 5 || (state->operation == ENEMY_SCRIPTED_MM && ((EnemyScriptedBinding*)actor)->pending)
#endif
             ))
    {
        if (!(state->operation == ENEMY_NATIVE_RESOURCE && state->resource.state == ENEMY_RESOURCE_PENDING && nativePlacementCurrent(state, play)))
            sNativeBlockedVisits[state->room] = state->visit;
    }
    state->actor = NULL;
    if (state->operation == ENEMY_NATIVE_RESOURCE && state->resource.state == ENEMY_RESOURCE_PENDING && nativePlacementCurrent(state, play))
        return;
    memset(state, 0, sizeof(*state));
}

void comboEnemyNativeCancelInit(Actor* actor, PlayState* play)
{
    NativePlacementState* state = nativePlacementState(actor);
    if (state && state->init)
        nativePlacementFini(actor, play);
}

#if defined(GAME_OOT)
static void (*sDarkLinkPlayerUpdate)(Player*, PlayState*, Input*);
static Actor* sStandaloneDarkLink;

static s32 nativeDarkLinkOverrideLimbDraw(PlayState* play, s32 limb, Gfx** dList,
                                          Vec3f* pos, Vec3s* rot, Player* player, Gfx** gfx)
{
    u32 age = gSave.age;
    s32 result;

    gSave.age = 0;
    result = ((s32(*)(PlayState*, s32, Gfx**, Vec3f*, Vec3s*, Player*))0x8007a740)(
        play, limb, dList, pos, rot, player);
    gSave.age = age;
    return result;
}

PATCH_FUNC(0x808bf70c, nativeDarkLinkOverrideLimbDraw);

static int nativeDarkLinkSafeFloor(PlayState* play, CollisionPoly* floor, s32 bgId)
{
    if (!floor || bgId != BGCHECK_SCENE)
        return 0;
    u32 (*exitIndex)(CollisionContext*, CollisionPoly*, s32) = (void*)0x800343ac;
    u32 (*floorProperty)(CollisionContext*, CollisionPoly*, s32) = (void*)0x80034540;
    u32 property = floorProperty(&play->colCtx, floor, bgId);
    return property != 5 && property != 12 && !exitIndex(&play->colCtx, floor, bgId) &&
           SurfaceType_GetFloorType(&play->colCtx, floor, bgId) != 9;
}

static int nativeDarkLinkRecoverFloor(Player* player, PlayState* play, Vec3f position)
{
    Actor* actor = &player->actor;
    CollisionPoly* floor = NULL;
    s32 bgId;
    position.y += 40.f;
    f32 height = BgCheck_EntityRaycastDown4(&play->colCtx, &floor, &bgId, actor, &position);
    if (!nativeDarkLinkSafeFloor(play, floor, bgId) || height < position.y - 100.f)
        return 0;

    position.y = height;
    Vec3f feet = position;
    Vec3f head = position;
    Vec3f hit;
    CollisionPoly* obstruction;
    s32 obstructionBgId;
    feet.y += 2.f;
    head.y += Player_GetHeight(player);
    if (BgCheck_EntityLineTest1(&play->colCtx, &feet, &head, &hit, &obstruction,
                                1, 0, 1, 0, &obstructionBgId))
        return 0;

    actor->world.pos = actor->prevPos = position;
    actor->floorHeight = height;
    actor->floorPoly = floor;
    actor->floorBgId = bgId;
    actor->bgCheckFlags &= ~(BGCHECKFLAG_CRUSHED | BGCHECKFLAG_WALL | BGCHECKFLAG_CEILING);
    actor->bgCheckFlags |= BGCHECKFLAG_GROUND;
    actor->speed = player->speedXZ = 0.f;
    actor->velocity.y = 0.f;
    player->fallStartHeight = height;
    player->fallDistance = 0;
    player->stateFlags1 &= ~(PLAYER_STATE1_29 | PLAYER_STATE1_31);
    player->stateFlags2 &= ~PLAYER_STATE2_31;
    player->av1.actionVar1 = player->av2.actionVar2 = 0;
    return 1;
}

static void nativeDarkLinkRecover(Player* player, PlayState* play)
{
    Actor* actor = &player->actor;
    if (nativeDarkLinkRecoverFloor(player, play, actor->home.pos))
        return;
    Player* link = GET_PLAYER(play);
    if (link && link != player && (link->actor.bgCheckFlags & BGCHECKFLAG_GROUND) &&
        (link->actor.room == actor->room || actor->room < 0 ||
         (link->actor.room < 0 && actor->room == play->roomCtx.curRoom.num)) &&
        nativeDarkLinkRecoverFloor(player, play, link->actor.world.pos))
        return;

    Vec3f origin = actor->home.pos;
    origin.y += 24.f;
    for (int distance = 32; distance <= 96; distance += 32)
    {
        for (int direction = 0; direction < 8; ++direction)
        {
            s16 yaw = direction * 0x2000;
            Vec3f position = actor->home.pos;
            position.x += distance * Math_SinS(yaw);
            position.z += distance * Math_CosS(yaw);
            Vec3f probe = { position.x, origin.y, position.z };
            Vec3f hit;
            CollisionPoly* wall;
            s32 bgId;
            if (!BgCheck_EntityLineTest1(&play->colCtx, &origin, &probe, &hit, &wall, 1, 0, 0, 0, &bgId) &&
                nativeDarkLinkRecoverFloor(player, play, position))
                return;
        }
    }
}

static s32 nativeDarkLinkPlayerDamage(Player* player, PlayState* play)
{
    s32 (*damage)(Player*, PlayState*) = OverlayAddr(0x808363dc);
    if (player->actor.id == ACTOR_EN_TORCH2 && player != GET_PLAYER(play))
    {
        s8* environmentalDamage = (s8*)((u8*)player + 0xa76);
        f32 crushHeight = *(f32*)((u8*)player + 0x6b4);
        s32 floorType = *(s32*)OverlayAddr(0x808514a4);
        *environmentalDamage = 0;
        if (Player_GetHeight(player) - 8.f < crushHeight * player->actor.scale.y ||
            (player->actor.bgCheckFlags & BGCHECKFLAG_CRUSHED) || floorType == 9 ||
            (player->stateFlags2 & PLAYER_STATE2_31))
        {
            nativeDarkLinkRecover(player, play);
            return 1;
        }
    }
    return damage(player, play);
}

PATCH_CALL(0x80847a64, nativeDarkLinkPlayerDamage);

static void nativeDarkLinkPlayerFallSetup(PlayState* play, Player* player)
{
    if (player->actor.id == ACTOR_EN_TORCH2 && player != GET_PLAYER(play))
    {
        nativeDarkLinkRecover(player, play);
        return;
    }
    void (*setup)(PlayState*, Player*) = OverlayAddr(0x80837060);
    setup(play, player);
}

PATCH_CALL(0x808370f8, nativeDarkLinkPlayerFallSetup);
PATCH_CALL(0x80837684, nativeDarkLinkPlayerFallSetup);

static int nativeDarkLinkCanHop(Player* player, PlayState* play)
{
    Actor* actor = &player->actor;
    if (!(actor->bgCheckFlags & BGCHECKFLAG_GROUND) || (actor->bgCheckFlags & BGCHECKFLAG_WALL) ||
        player->speedXZ < 4.f || actor->xzDistToPlayer < 80.f ||
        ABS((s16)(actor->yawTowardsPlayer - player->yaw)) >= 0x1000)
        return 0;

    f32 dx = Math_SinS(player->yaw);
    f32 dz = Math_CosS(player->yaw);
    Vec3f near = { actor->world.pos.x + 22.f * dx, actor->world.pos.y + 24.f, actor->world.pos.z + 22.f * dz };
    Vec3f landing = { actor->world.pos.x + 52.f * dx, actor->world.pos.y + 24.f, actor->world.pos.z + 52.f * dz };
    CollisionPoly* floor = NULL;
    s32 bgId;
    f32 height = BgCheck_EntityRaycastDown4(&play->colCtx, &floor, &bgId, actor, &near);
    if (floor && height >= actor->world.pos.y - 20.f)
        return 0;
    height = BgCheck_EntityRaycastDown4(&play->colCtx, &floor, &bgId, actor, &landing);
    if (!nativeDarkLinkSafeFloor(play, floor, bgId) || fabsf(height - actor->world.pos.y) > 20.f)
        return 0;

    Vec3f from = actor->world.pos;
    Vec3f hit;
    from.y += Player_GetHeight(player) + 16.f;
    landing.y = from.y;
    if (BgCheck_EntityLineTest1(&play->colCtx, &from, &landing, &hit, &floor, 1, 0, 1, 0, &bgId))
        return 0;
    from.y = actor->world.pos.y + 12.f;
    landing.y = height + 12.f;
    return !BgCheck_EntityLineTest1(&play->colCtx, &from, &landing, &hit, &floor, 1, 0, 0, 0, &bgId);
}

static void nativeDarkLinkPlayerUpdate(Player* player, PlayState* play, Input* input)
{
    Actor* actor = &player->actor;
    if (actor == sStandaloneDarkLink)
    {
        if (!actor->colChkInfo.health)
        {
            player->cv.haltActorsDuringCsAction = 0;
        }
        else if (player->ledgeClimbType >= 2 && player->yDistToLedge > 0.f &&
                 (actor->bgCheckFlags & (BGCHECKFLAG_GROUND | BGCHECKFLAG_WALL)) ==
                     (BGCHECKFLAG_GROUND | BGCHECKFLAG_WALL) &&
                 GET_PLAYER(play)->actor.world.pos.y > actor->world.pos.y + player->yDistToLedge - 12.f &&
                 ABS((s16)(actor->yawTowardsPlayer - actor->shape.rot.y)) < 0x1800 &&
                 !player->invincibilityTimer && !actor->colorFilterTimer &&
                 !(input->press.button & (BTN_A | BTN_B)) &&
                 (input->cur.stick_x || input->cur.stick_y))
        {
            Input climb = *input;
            climb.cur.button |= BTN_A;
            climb.press.button |= BTN_A;
            sDarkLinkPlayerUpdate(player, play, &climb);
            return;
        }
        else if (!player->invincibilityTimer && !actor->colorFilterTimer &&
                 !(input->press.button & (BTN_A | BTN_B)) &&
                 (input->cur.stick_x || input->cur.stick_y) && nativeDarkLinkCanHop(player, play))
        {
            s32 (*autoJump)(Player*, PlayState*) = OverlayAddr(0x808385cc);
            autoJump(player, play);
        }
    }
    sDarkLinkPlayerUpdate(player, play, input);
}

static u8* nativeDarkLinkOverlay(Actor* actor)
{
    ActorOvl* overlay = actor->ovl;
    return overlay && overlay->vramStart == 0x808bd8e0 && overlay->vramEnd > 0x808c0075 ? overlay->data : NULL;
}

static void nativeDarkLinkDraw(Actor* actor, PlayState* play)
{
    u8* overlay = nativeDarkLinkOverlay(actor);
    if (!overlay)
        return;
    u8* opacity = overlay + 0x2795;
    u8 saved = *opacity;
    *opacity = MIN(saved, 160);
    ((ActorFunc)(overlay + (0x808bf75c - 0x808bd8e0)))(actor, play);
    *opacity = saved;
}

static void nativeDarkLinkUpdate(Actor* actor, PlayState* play, void (*update)(Actor*, PlayState*))
{
    u8* overlay = nativeDarkLinkOverlay(actor);
    Player* link = GET_PLAYER(play);
    int preserveRecoil = link->speedXZ == -18.f &&
                         !comboEnemyDarkLinkClashedWithPlayer((Player*)actor, link);
    if (actor->draw)
        actor->draw = nativeDarkLinkDraw;
    ((Player*)actor)->ageProperties = OverlayAddr(0x808512e8);
    if (overlay && actor->colChkInfo.health)
    {
        u8* opacity = overlay + 0x2795;
        if (*opacity < 255)
            *opacity = MIN(255, *opacity + 4);
    }
    sStandaloneDarkLink = actor;
    sDarkLinkPlayerUpdate = play->playerUpdate;
    play->playerUpdate = nativeDarkLinkPlayerUpdate;
    update(actor, play);
    play->playerUpdate = sDarkLinkPlayerUpdate;
    sDarkLinkPlayerUpdate = NULL;
    sStandaloneDarkLink = NULL;

    /* The standalone draw always uses XLU, even after the native combat alpha
     * reaches 255. Route its damage/stun fog to that same display list. */
    if (actor->colorFilterTimer)
        actor->colorFilterParams |= COLORFILTER_BUFFLAG_XLU;

    if (preserveRecoil)
        link->speedXZ = -18.f;

    if (actor->update && actor->colChkInfo.health &&
        (actor->world.pos.y < -4000.f ||
         (actor->floorPoly && actor->world.pos.y < actor->floorHeight - 180.f) ||
         (actor->world.pos.y < actor->home.pos.y - 180.f &&
          (!actor->floorPoly || actor->floorHeight < actor->home.pos.y - 180.f))))
        nativeDarkLinkRecover((Player*)actor, play);

    if (actor->update && actor->colChkInfo.health &&
        (actor->bgCheckFlags & BGCHECKFLAG_GROUND) && ((Player*)actor)->invincibilityTimer <= 0)
    {
        actor->shape.yOffset = MAX(actor->shape.yOffset, 300.f);
        if (actor->floorHeight > -30000.f && actor->world.pos.y < actor->floorHeight &&
            actor->world.pos.y > actor->floorHeight - 30.f)
            actor->world.pos.y = actor->floorHeight;
    }
}

#if defined(DEBUG)
static void debugDarkLinkUpdate(Actor* actor, PlayState* play)
{
    u8* overlay = nativeDarkLinkOverlay(actor);
    if (!overlay)
        return;
    nativeDarkLinkUpdate(actor, play, (void*)(overlay + 0x450));
}

void comboDebugTrackOotDarkLink(Actor* actor)
{
    if (!actor || actor->id != ACTOR_EN_TORCH2)
        return;
    u8* overlay = nativeDarkLinkOverlay(actor);
    if (overlay && actor->update == (void*)(overlay + 0x450))
        actor->update = debugDarkLinkUpdate;
}
#endif
#endif

#if defined(GAME_OOT)
#define REDEAD_ACTION_OFFSET  0x2f8
#define REDEAD_ATTEMPT_FREEZE 0x8093aee0
#define REDEAD_SETUP_IDLE     0x80939d38
#else
#define REDEAD_ACTION_OFFSET  0x188
#define REDEAD_ATTEMPT_FREEZE 0x808d5ccc
#define REDEAD_SETUP_IDLE     0x808d4308
#endif

typedef struct
{
    Actor actor;
    u8 pad[REDEAD_ACTION_OFFSET - sizeof(Actor)];
    ActorFunc action;
} NativeRedead;

_Static_assert(__builtin_offsetof(NativeRedead, action) == REDEAD_ACTION_OFFSET, "ReDead action offset");

void comboEnemyRedeadUpdate(Actor* actor, PlayState* play, ActorFunc update)
{
    if (!update)
        return;
    if (actor->id != ACTOR_EN_RD)
    {
        update(actor, play);
        return;
    }
    Actor* player = &GET_PLAYER(play)->actor;
    f32 dx = player->world.pos.x - actor->world.pos.x;
    f32 dy = player->world.pos.y - actor->world.pos.y;
    f32 dz = player->world.pos.z - actor->world.pos.z;
    f32 distance = actor->xzDistToPlayer;
    if (dx * dx + dy * dy + dz * dz > 150.f * 150.f)
    {
        NativeRedead* redead = (NativeRedead*)actor;
        if (redead->action == (ActorFunc)actorAddr(ACTOR_EN_RD, REDEAD_ATTEMPT_FREEZE))
            ((void (*)(Actor*))actorAddr(ACTOR_EN_RD, REDEAD_SETUP_IDLE))(actor);
        actor->xzDistToPlayer = MAX(distance, 151.f);
    }
    update(actor, play);
    actor->xzDistToPlayer = distance;
}

static void nativeResourceCheckDefeat(Actor* actor, PlayState* play, NativePlacementState* state)
{
    if (state->operation != ENEMY_NATIVE_RESOURCE || !(state->hadHealth & 1) ||
        actor->colChkInfo.health || !nativePlacementCurrent(state, play))
        return;
#if defined(GAME_OOT)
    if (state->resource.original.id == ACTOR_EN_GELDB)
    {
        if (state->resource.state == ENEMY_RESOURCE_WAITING)
        {
            Vec3f pos = actor->world.pos;
            if (actor->floorPoly && actor->floorHeight > -30000.f)
                pos.y = MAX(pos.y, actor->floorHeight);
            state->resource.original.pos = (Vec3s){ pos.x, pos.y, pos.z };
        }
        comboEnemyNativeResourceDefeated(&state->resource);
        return;
    }
#endif
    if (!actor->update)
        comboEnemyNativeResourceDefeated(&state->resource);
}

#if defined(GAME_OOT)
typedef struct
{
    Actor actor;
    ActorFunc action;
    s16 spin;
} NativeFlyingTile;

_Static_assert(__builtin_offsetof(NativeFlyingTile, action) == 0x13c, "tile action");

static void nativeFlyingTileUpdate(Actor* actor, PlayState* play, ActorFunc update)
{
    NativeFlyingTile* tile = (NativeFlyingTile*)actor;
    ActorOvl* ovl = actor->ovl;
    if (!ovl || !ovl->data || ovl->vramStart > 0x809189ec || ovl->vramEnd <= 0x80918b24)
    {
        update(actor, play);
        return;
    }
    ActorFunc wait = (void*)((u8*)ovl->data + 0x809189ec - ovl->vramStart);
    ActorFunc fly = (void*)((u8*)ovl->data + 0x80918ac4 - ovl->vramStart);
    ActorFunc action = tile->action;
    if (action == wait && !comboEnemyFlyingAttackInRange(actor, play, 400.f))
        return;
    if (action == fly)
        comboEnemyFlyingAttackAim(actor, play, 0);
    update(actor, play);
    if (actor->update && tile->action == fly)
    {
        if (action != fly)
            comboEnemyFlyingAttackAim(actor, play, 1);
        Actor_UpdateBgCheckInfo(play, actor, 5.f, 20.f, 8.f,
            UPDBGCHECKINFO_FLAG_WALL | UPDBGCHECKINFO_FLAG_FLOOR | UPDBGCHECKINFO_FLAG_CEILING);
        if (actor->bgCheckFlags & (BGCHECKFLAG_WALL | BGCHECKFLAG_GROUND | BGCHECKFLAG_CEILING))
            tile->action = (void*)((u8*)ovl->data + 0x80918b24 - ovl->vramStart);
    }
}
#endif

static void nativePlacementUpdate(Actor* actor, PlayState* play)
{
    NativePlacementState* state = nativePlacementState(actor);
    NativePlacementState* previous = sNativeScope;
    if (!state)
    {
        Actor_Kill(actor);
        return;
    }
    EnemyGroundGuard guard;
    if (actor->colChkInfo.health)
        state->hadHealth |= 1;
    actor->update = state->update;
    actor->fini = state->fini;
    sNativeScope = state;
    comboEnemyGroundGuardBegin(play, actor, &guard);
#if defined(GAME_OOT)
    if (actor->id == ACTOR_EN_DODOJR && actor->colChkInfo.health)
        comboEnemyGroundGuardBeginNative(actor, &guard);
    if (actor->id == 0xa4 && actor->params == 0 && actor->colChkInfo.health &&
        nativePlacementCurrent(state, play))
    {
        if (!state->deadHandHandsPlaced)
        {
            state->deadHandHandsPlaced = 1;
            DeadHand_PlaceHands(actor, play);
        }
        Actor* player = &GET_PLAYER(play)->actor;
        float dx = player->world.pos.x - actor->home.pos.x;
        float dz = player->world.pos.z - actor->home.pos.z;
        float dy = player->world.pos.y - actor->home.pos.y;
        actor->world.pos = actor->home.pos;
        if (dx * dx + dz * dz < 200.f * 200.f && dy > -80.f && dy < 100.f)
            actor->params = 5;
    }
#endif
#if defined(GAME_OOT)
    if (actor->id == ACTOR_EN_TORCH2 && state->update)
        nativeDarkLinkUpdate(actor, play, state->update);
    else
#endif
#if defined(GAME_MM)
        if (state->update && nativeDrySkullfish())
    {
        Player* player = GET_PLAYER(play);
        u32 swim = player->stateFlags1 & 0x08000000;
        player->stateFlags1 |= 0x08000000;
        state->update(actor, play);
        player->stateFlags1 = (player->stateFlags1 & ~0x08000000u) | swim;
    }
    else
#endif
        if (state->update)
    {
#if defined(GAME_MM)
        int largeEeno = actor->id == 0x1e6 && actor->params == 1;
#endif
#if defined(GAME_OOT)
        if (actor->id == 0x6b)
            nativeFlyingTileUpdate(actor, play, state->update);
        else
#endif
            comboEnemyRedeadUpdate(actor, play, state->update);
#if defined(GAME_MM)
        if (largeEeno && actor->params == 2 && actor->update)
        {
            void comboEnemyNativeEenoSplit(Actor*, PlayState*);
            comboEnemyNativeEenoSplit(actor, play);
        }
#endif
    }
    comboEnemyGroundGuardEnd(play, &guard);
    if (actor->id == ACTOR_EN_IK)
    {
        actor->shape.rot.x = 0;
        actor->shape.rot.z = 0;
    }
#if defined(GAME_MM)
    if (actor->id == 0x23f && actor->update)
        Actor_SetFocus(actor, 0.f);
#endif
    sNativeScope = previous;
    nativeResourceCheckDefeat(actor, play, state);
    if (state->operation == 13 && !actor->update && (state->hadHealth & 1))
        nativePlacementDefeated(actor, play);
    state->update = actor->update;
    state->fini = actor->fini;
    if (actor->update)
        actor->update = nativePlacementUpdate;
    actor->fini = nativePlacementFini;
}

static Actor* nativePlacementPolicySpawn(ActorContext* ctx, ActorEntry* entry, PlayState* play, u32 operation)
{
#if defined(GAME_OOT)
    ActorEntry adjusted;
    if (entry->id == ACTOR_EN_MB && entry->params == 2)
    {
        adjusted = *entry;
        adjusted.params = 0;
        entry = &adjusted;
    }
#endif
    switch (operation)
    {
    case 0:
        return NativePlacementSpawnEntry(ctx, entry, play);
    case 15:
        return NativePlacementSpawnEntry(ctx, entry, play);
    case 4:
        return comboEnemyNativePeahatSpawn(ctx, entry, play);
    case 6:
        return comboEnemyNativeRenewableBabaSpawn(ctx, entry, play);
    case 10:
        return comboEnemyNativeLeeverSpawn(ctx, entry, play);
    case 11:
        return comboEnemyNativeBigPoeSpawn(ctx, entry, play);
    case 12:
        return comboEnemyNativeFighterSpawn(ctx, entry, play);
    case 13:
        return comboEnemyNativePoeSisterSpawn(ctx, entry, play);
#if defined(GAME_MM)
    case 30:
        return comboEnemyNativeRenewableSpawn(ctx, entry, play);
    case 31:
        return comboEnemyNativeLilySpawn(ctx, entry, play);
    case 25:
        return NativePlacementSpawnEntry(ctx, entry, play);
    case 5:
        return comboEnemyNativeSkullwalltulaSpawn(ctx, entry, play);
    case 9:
        return comboEnemyNativeGaroSpawn(ctx, entry, play);
    case 18:
        return comboEnemyNativeTakkuriSpawn(ctx, entry, play);
    case 19:
        return comboEnemyNativeEyegoreSpawn(ctx, entry, play);
    case 21:
    case ENEMY_NATIVE_GARAHNA:
    case ENEMY_NATIVE_DRY_MM_AQUATIC:
        return NativePlacementSpawnEntry(ctx, entry, play);
#else
    case ENEMY_NATIVE_DRY_AQUATIC:
        return NativePlacementSpawnEntry(ctx, entry, play);
    case 20:
        return comboEnemyNativeTentacleSpawn(ctx, entry, play);
    case 17:
        return comboEnemyNativeBubbleSpawn(ctx, entry, play);
    case 3:
        return comboEnemyNativeStalchildSpawn(ctx, entry, play);
    case 7:
        return comboEnemyNativeGuaySpawn(ctx, entry, play);
    case ENEMY_NATIVE_IRON_KNUCKLE:
        return comboEnemyNativeIronKnuckleSpawn(ctx, entry, play);
    case ENEMY_NATIVE_SKULL_KID:
        return comboEnemyNativeSkullKidSpawn(ctx, entry, play);
#endif
    default:
        return NULL;
    }
}

Actor* comboEnemyNativeStandaloneSpawn(ActorContext* ctx, ActorEntry* entry, PlayState* play, u32 operation)
{
    return nativePlacementPolicySpawn(ctx, entry, play, operation);
}

static Actor* nativePlacementTraitsSpawn(ActorContext* ctx, ActorEntry* entry, PlayState* play, u32 operation, u32 key)
{
    NativePlacementState* state = NULL;
    Actor* actor;
    s32 room = play->roomCtx.curRoom.num;
    if (room < 0 || room >= 32)
        return NULL;
    if (!nativePlacementAllocStates())
        return NULL;
    ActorEntry copy = *entry;
    u32 traits = operation & (ENEMY_NATIVE_AIR | ENEMY_NATIVE_UPRIGHT);
    u32 base = operation & ~(ENEMY_NATIVE_AIR | ENEMY_NATIVE_UPRIGHT | ENEMY_NATIVE_FLOOR);
    for (u32 i = 0; i < NATIVE_PLACEMENT_STATE_COUNT; ++i)
        if (!sNativeStates[i].actor && !sNativeStates[i].operation)
        {
            state = &sNativeStates[i];
            break;
        }
    if (!state || sNativeVisits[room] == 0xffffffffu)
    {
        return NULL;
    }
#if defined(GAME_OOT)
    if ((operation & ENEMY_NATIVE_FLOOR) && !comboEnemyVirtualFloorActive())
        comboEnemyNativeFloorEntry(play, entry, &copy);
#endif
    if (traits & ENEMY_NATIVE_UPRIGHT)
    {
#if defined(GAME_MM)
        copy.rot.x &= 0x7f;
        copy.rot.z &= 0x7f;
#else
        copy.rot.x = copy.rot.z = 0;
#endif
    }
    if (traits & ENEMY_NATIVE_AIR)
    {
        Actor proxy;
        memset(&proxy, 0, sizeof(proxy));
        proxy.home.pos.x = proxy.world.pos.x = copy.pos.x;
        proxy.home.pos.y = proxy.world.pos.y = copy.pos.y;
        proxy.home.pos.z = proxy.world.pos.z = copy.pos.z;
        comboEnemyAirPlacement(play, &proxy, 40.f);
        copy.pos.y = proxy.home.pos.y;
    }
#if defined(GAME_MM)
    if ((copy.id & 0x1fff) == 0x16f)
    {
        Actor probe = { 0 };
        CollisionPoly* floor = NULL;
        s32 bgId;
        Vec3f start = { copy.pos.x, copy.pos.y + 40.f, copy.pos.z };
        f32 y = BgCheck_EntityRaycastFloor5_2(play, &play->colCtx, &floor, &bgId, &probe, &start);
        if (floor && y > -30000.f && y < 30000.f)
            copy.pos.y = (s16)y + 1;
    }
#endif
    state->operation = base ? base : 14;
    state->resource.original = copy;
    NativePlacementState* previousScope = sNativeScope;
    if (base == ENEMY_NATIVE_DRY_AQUATIC || base == ENEMY_NATIVE_DRY_MM_AQUATIC)
    {
        state->room = room;
        state->visit = sNativeVisits[room];
        sNativeScope = state;
    }
    actor = nativePlacementPolicySpawn(ctx, &copy, play, base);
    sNativeScope = previousScope;
    if (!actor || (!actor->update && !actor->init))
    {
        memset(state, 0, sizeof(*state));
        return NULL;
    }
    if (base == 15 && !actor->init)
        Actor_ChangeCategory(play, &play->actorCtx, actor, ACTORCAT_ENEMY);
#if defined(GAME_OOT)
    if (actor->id == ACTOR_EN_TORCH2 && !actor->init)
        Actor_ChangeCategory(play, &play->actorCtx, actor, ACTORCAT_ENEMY);
#endif
    state->actor = actor;
    state->room = room;
    state->visit = sNativeVisits[room];
    state->splitPlacementVrom = key;
#if defined(GAME_MM)
    if (actor->id == 0xc)
        actor->flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
#else
    if (actor->id == 0x13 || actor->id == 0xa4)
        actor->flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
#endif
#if defined(GAME_MM)
    if ((copy.id & 0x1fff) == 0x16f)
    {
        Actor probe = { 0 };
        CollisionPoly* floor = NULL;
        s32 bgId;
        Vec3f start = { copy.pos.x, copy.pos.y + 40.f, copy.pos.z };
        f32 y = BgCheck_EntityRaycastFloor5_2(play, &play->colCtx, &floor, &bgId, &probe, &start);
        if (floor && y > -30000.f && y < 30000.f)
            copy.pos.y = (s16)y + 1;
    }
#endif
    state->operation = base ? base : 14;
    state->hadHealth = (actor->colChkInfo.health != 0) | ((traits & ENEMY_NATIVE_AIR) ? 2 : 0) | ((traits & ENEMY_NATIVE_UPRIGHT) ? 4 : 0);
    state->init = actor->init;
    state->update = actor->update;
    state->fini = actor->fini;
    if (actor->init)
        actor->init = nativePlacementInit;
    if (actor->update)
        actor->update = nativePlacementUpdate;
    actor->fini = nativePlacementFini;
    return actor;
}

static int nativeRoomRecord(PlayState* play, const ActorEntry* entry, NativePlacement* record, int* instancePlacement)
{
    s32 room = play->roomCtx.curRoom.num;
    u32 segment = (u32)play->roomCtx.curRoom.segment;
    u32 address = (u32)entry, start, end, key;
    RomFile* files = play->roomList;
    *instancePlacement = 0;
    if (!sNativePlacementVrom || room < 0 || room >= 32 || room >= play->numRooms || !files || !segment)
        return 0;
#if defined(GAME_OOT)
    if (comboMqEnemyPlacementKey(play, entry, &key))
        return nativePlacementLookup(key, record);
#endif
    if (address < segment)
        return 0;
    start = (u32)files[room].vromStart;
    end = (u32)files[room].vromEnd;
    if (!start || end <= start || end - start < sizeof(*entry) || address - segment > end - start - sizeof(*entry))
        return 0;
    key = start + address - segment;
#if defined(GAME_MM)
    u32 grotto = gGrottoData & 0x1f;
    if (play->sceneId == SCE_MM_GROTTOS && room == 4 && grotto >= 0x13)
    {
        *instancePlacement = 1;
        key = 0xe0000000u + (grotto - 0x13) * 0x10000 + address - segment;
    }
#endif
    return nativePlacementLookup(key, record);
}

int comboEnemyNativePlacementSpawn(ActorContext* ctx, ActorEntry* entry, PlayState* play)
{
    NativePlacement record;
    NativePlacement seedRecord;
    ActorEntry* seedEntry = entry;
    ActorEntry adjusted;
    ActorEntry instanceEntry;
    int instancePlacement;
    NativePlacementState* state = NULL;
    Actor* actor;
    s32 room = play->roomCtx.curRoom.num;
    if (!nativeRoomRecord(play, entry, &record, &instancePlacement))
        return 0;
    if (memcmp(entry, instancePlacement ? &record.original : &record.replacement, sizeof(*entry)))
    {
        if (!comboIsForeignEnemyActor(entry->id & 0x1fff))
            return 0;
        g.roomEnemyLackSoul = 1;
        return 1;
    }
    if (instancePlacement)
    {
        instanceEntry = record.replacement;
        entry = &instanceEntry;
        seedEntry = entry;
    }
    int chaotic = 0;
    int hadLackSoul = g.roomEnemyLackSoul;
    if (chaotic)
    {
        NativePlacement choice = seedRecord;
        seedRecord = record;
        record = choice;
        entry = &record.replacement;
    }
retrySeed:;
    int virtualFloor = virtualFloorPlacement(record.vrom);
    if (record.operation & ENEMY_NATIVE_GROUND)
    {
        Vec3f pos = { entry->pos.x, entry->pos.y, entry->pos.z };
        adjusted = *entry;
        /* Hanging sources can overhang a wall or a narrow ledge. Keep the
         * nearby search first, then widen it with the same safety checks. */
        if (!virtualFloor && !comboEnemyVirtualFloorActive() && (comboEnemyFindFloor(play, NULL, &pos, 0.f, 50, 1) ||
            comboEnemyFindFloor(play, NULL, &pos, 0.f, 150, 1) ||
            comboEnemyFindFloor(play, NULL, &pos, 0.f, 512, 1)))
            adjusted.pos = (Vec3s){ pos.x, pos.y, pos.z };

#if defined(GAME_MM)
        adjusted.rot.x &= 0x7f;
        adjusted.rot.z &= 0x7f;
#endif
        entry = &adjusted;
        record.operation &= ~(ENEMY_NATIVE_GROUND | ENEMY_NATIVE_FLOOR);
    }
#if defined(GAME_MM)
    if (!record.operation && comboIsForeignEnemyActor(entry->id & 0x1fff))
    {
        if (!nativePlacementForeignSpawn(ctx, entry, play))
            g.roomEnemyLackSoul = 1;
        return 1;
    }
#endif
    if (!nativePlacementAllocStates())
    {
#if defined(GAME_MM)
        comboEnemyNativePlacementBlock(room);
#else
        if (sNativeVisits[room])
            sNativeBlockedVisits[room] = sNativeVisits[room];
#endif
        g.roomEnemyLackSoul = 1;
        return 1;
    }
#if defined(GAME_MM)
    if (!record.operation && (entry->id & 0x1fff) == ACTOR_EN_PP)
    {
        if (!nativePlacementTraitsSpawn(ctx, entry, play, 0, record.vrom))
        {
            if (chaotic)
                goto fallbackSeed;
            g.roomEnemyLackSoul = 1;
        }
        return 1;
    }
#endif
    if (record.operation & (ENEMY_NATIVE_AIR | ENEMY_NATIVE_UPRIGHT) || record.operation == 9 || record.operation == 10 ||
        record.operation == 15 || record.operation == 20 || record.operation == ENEMY_NATIVE_DRY_AQUATIC ||
        record.operation == ENEMY_NATIVE_DRY_MM_AQUATIC)
    {
        if (Flags_GetClear(play, room))
            return 1;
        ActorEntry copy = *entry;
        u32 operation = record.operation;
#if defined(GAME_OOT)
        if (operation & ENEMY_NATIVE_FLOOR)
        {
            if (!virtualFloor && !comboEnemyVirtualFloorActive())
                comboEnemyNativeFloorEntry(play, entry, &copy);
            operation &= ~ENEMY_NATIVE_FLOOR;
        }
        if (!comboOotEnemizerCanSpawn(play, &copy))
        {
            if (chaotic)
                goto fallbackSeed;
            return 1;
        }
#endif
        actor = nativePlacementTraitsSpawn(ctx, &copy, play, operation, record.vrom);
        if (!actor)
        {
            if (chaotic)
                goto fallbackSeed;
            g.roomEnemyLackSoul = 1;
        }
        return 1;
    }
#if defined(GAME_OOT)
    if (record.operation & ENEMY_NATIVE_FLOOR)
    {
        if (!virtualFloor && !comboEnemyVirtualFloorActive())
        {
            comboEnemyNativeFloorEntry(play, entry, &adjusted);
            entry = &adjusted;
        }
        record.operation &= ~ENEMY_NATIVE_FLOOR;
    }
    if (!comboOotEnemizerCanSpawn(play, entry))
    {
        if (chaotic)
            goto fallbackSeed;
        return 1;
    }
    if (!record.operation)
    {
        if (!NativePlacementSpawnEntry(ctx, entry, play) && chaotic)
            goto fallbackSeed;
        return 1;
    }
    if (record.operation == 6)
    {
        if (!comboEnemyNativeRenewableBabaSpawn(ctx, entry, play))
            g.roomEnemyLackSoul = 1;
        return 1;
    }
    if (record.operation == 4)
    {
        if (!comboEnemyNativePeahatSpawn(ctx, entry, play))
            g.roomEnemyLackSoul = 1;
        return 1;
    }
    if (record.operation == 3)
    {
        if (!comboEnemyNativeStalchildSpawn(ctx, entry, play))
            g.roomEnemyLackSoul = 1;
        return 1;
    }
    if (record.operation == 129)
    {
        if (comboIsForeignEnemyActor(entry->id))
            NativePlacementSpawnEntry(ctx, entry, play);
        else if (!comboEnemyDynamicSpawn(ctx, entry, play, 0xffff))
            g.roomEnemyLackSoul = 1;
        return 1;
    }
    if (record.operation == 7)
    {
        if (!comboEnemyNativeGuaySpawn(ctx, entry, play))
            g.roomEnemyLackSoul = 1;
        return 1;
    }
    if (record.operation == ENEMY_NATIVE_IRON_KNUCKLE)
    {
        if (entry->id != 0x113 || record.targetId != 0x113 ||
            record.targetParams != (u16)entry->params ||
            !comboEnemyNativeIronKnuckleSpawn(ctx, entry, play))
            g.roomEnemyLackSoul = 1;
        return 1;
    }
    if (record.operation == ENEMY_NATIVE_SKULL_KID)
    {
        if (entry->id != 0x115 || (u16)entry->params != 0xffff ||
            record.targetId != 0x115 || record.targetParams != 0xffff ||
            !comboEnemyNativeSkullKidSpawn(ctx, entry, play))
            g.roomEnemyLackSoul = 1;
        return 1;
    }
    if (record.operation == ENEMY_NATIVE_SUPPRESS && record.original.id == 0xa5 &&
        !memcmp(&record.original, &record.replacement, sizeof(*entry)))
        return 1;
    if (record.operation == ENEMY_NATIVE_RESOURCE ? (record.targetId != entry->id || record.targetParams != (u16)entry->params ||
                                                     (record.targetId < 0 || record.targetId >= 0x1000)) :
                                                    (record.operation != ENEMY_NATIVE_ARENA || (record.original.id != 0x136 && record.original.id != 0xc8) ||
                                                     record.targetId < 0 || record.targetId >= 0x1000))
    {
        g.roomEnemyLackSoul = 1;
        return 1;
    }
    if (record.operation == ENEMY_NATIVE_ARENA && Flags_GetClear(play, room))
    {
        NativePlacementSpawnEntry(ctx, &record.original, play);
        return 1;
    }
#else
    if (!record.operation)
    {
        if (!NativePlacementSpawnEntry(ctx, entry, play) && chaotic)
            goto fallbackSeed;
        return 1;
    }
    if (record.operation == 5)
    {
        if (Flags_GetClear(play, room))
            return 1;
        if ((entry->id & 0x1fff) != 0x50 || (u16)entry->params != 0xff00 ||
            record.targetId != 0x50 || record.targetParams != 0xff00)
        {
            comboEnemyNativePlacementBlock(room);
            g.roomEnemyLackSoul = 1;
            return 1;
        }
        for (u32 i = 0; i < NATIVE_PLACEMENT_STATE_COUNT; ++i)
            if (!sNativeStates[i].actor && !sNativeStates[i].operation)
            {
                state = &sNativeStates[i];
                break;
            }
        if (!state || sNativeVisits[room] == 0xffffffffu)
        {
            comboEnemyNativePlacementBlock(room);
            g.roomEnemyLackSoul = 1;
            return 1;
        }
        state->room = room;
        state->visit = sNativeVisits[room];
        state->operation = 5;
        actor = comboEnemyNativeSkullwalltulaSpawn(ctx, entry, play);
        if (!actor || (!actor->update && !actor->init))
        {
            memset(state, 0, sizeof(*state));
            comboEnemyNativePlacementBlock(room);
            g.roomEnemyLackSoul = 1;
            return 1;
        }
        state->actor = actor;
        state->init = actor->init;
        state->update = actor->update;
        state->fini = actor->fini;
        if (actor->init)
            actor->init = nativePlacementInit;
        if (actor->update)
            actor->update = nativePlacementUpdate;
        actor->fini = nativePlacementFini;
        return 1;
    }
    if (record.operation == 6)
    {
        if (!comboEnemyNativeRenewableBabaSpawn(ctx, entry, play))
            g.roomEnemyLackSoul = 1;
        return 1;
    }
    if (record.operation == 4)
    {
        if (!comboEnemyNativePeahatSpawn(ctx, entry, play))
            g.roomEnemyLackSoul = 1;
        return 1;
    }
    if (record.operation == ENEMY_SCRIPTED_MM)
    {
        const u16 id = record.original.id & 0x1fff;
        if ((id != 7 && id != 0x65 && id != 0x115 && id != 0x12c && id != 0x182 && id != 0x184 && id != 0x43 && id != 0x28f && id != 0x15d) ||
            memcmp(&record.original, &record.replacement, sizeof(*entry)) || record.targetId < 0 || record.targetId >= 0x1000)
        {
            g.roomEnemyLackSoul = 1;
            return 1;
        }
        if (Flags_GetClear(play, room))
        {
            NativePlacementSpawnEntry(ctx, &record.original, play);
            return 1;
        }
        for (u32 i = 0; i < NATIVE_PLACEMENT_STATE_COUNT; ++i)
            if (!sNativeStates[i].actor && !sNativeStates[i].operation)
            {
                state = &sNativeStates[i];
                break;
            }
        if (!state)
        {
            NativePlacementSpawnEntry(ctx, &record.original, play);
            return 1;
        }
        ActorEntry monitor = record.original;
        monitor.id = (monitor.id & 0xe000) | ACTOR_CUSTOM_ENEMY_ARENA;
        monitor.params = 0;
        const u8 index = gCurrentSpawnActorNum;
        EnemyScriptedBinding* binding = (EnemyScriptedBinding*)NativePlacementSpawnEntry(ctx, &monitor, play);
        if (!binding)
        {
            NativePlacementSpawnEntry(ctx, &record.original, play);
            return 1;
        }
        binding->original = record.original;
        binding->targetId = record.targetId;
        binding->targetParams = record.targetParams;
        binding->actorIndex = index;
        binding->pending = 1;
        binding->binding = ENEMY_SCRIPTED_MAGIC;
        state->actor = &binding->actor;
        state->room = room;
        state->visit = sNativeVisits[room];
        state->operation = ENEMY_SCRIPTED_MM;
        state->virtualFloor = virtualFloor;
        state->init = binding->actor.init;
        state->update = binding->actor.update;
        state->fini = binding->actor.fini;
        if (state->init)
            binding->actor.init = nativePlacementInit;
        else if (state->update)
            binding->actor.update = nativePlacementUpdate;
        binding->actor.fini = nativePlacementFini;
        if (!comboEnemyScriptedCapturePlacement(&binding->actor, play, entry))
        {
            g.roomEnemyLackSoul = 1;
            Actor_Kill(&binding->actor);
            binding->actor.init = NULL;
        }
        return 1;
    }
    if (record.operation != ENEMY_NATIVE_RESOURCE || record.targetId != (entry->id & 0x1fff) ||
        record.targetParams != (u16)entry->params || (record.targetId < 0 || record.targetId >= 0x1000))
    {
        g.roomEnemyLackSoul = 1;
        return 1;
    }
#endif
    for (u32 i = 0; i < NATIVE_PLACEMENT_STATE_COUNT; ++i)
        if (!sNativeStates[i].actor && !sNativeStates[i].operation)
        {
            state = &sNativeStates[i];
            break;
        }
    if (!state || sNativeVisits[room] == 0xffffffffu)
    {
        if (!NativePlacementSpawnEntry(ctx, &record.original, play))
            g.roomEnemyLackSoul = 1;
        return 1;
    }
    if (record.operation == ENEMY_NATIVE_RESOURCE && !comboEnemyNativeResourceInit(&state->resource, &record.original, room))
    {
        g.roomEnemyLackSoul = 1;
        return 1;
    }
    if (record.operation == ENEMY_NATIVE_RESOURCE && comboEnemyNativeResourceAlreadyComplete(play, &state->resource))
    {
        memset(state, 0, sizeof(*state));
#if defined(GAME_OOT)
        if (record.original.id == ACTOR_EN_GELDB)
            return 1;
#endif
        NativePlacementSpawnEntry(ctx, &record.original, play);
        return 1;
    }
    if (record.operation == ENEMY_NATIVE_RESOURCE)
    {
        state->room = room;
        state->visit = sNativeVisits[room];
        state->operation = ENEMY_NATIVE_RESOURCE;
        EnemyFamily* family = &sNativeFamilies[state - sNativeStates];
        family->complete = nativeFamilyComplete;
        if ((virtualFloor && !comboEnemyFamilySetVirtualFloor(family, play, entry)) ||
            !comboEnemyFamilySpawn(family, ctx, entry, play, 0xffff))
        {
            sNativeBlockedVisits[room] = sNativeVisits[room];
            g.roomEnemyLackSoul = 1;
        }
        return 1;
    }
    actor = NativePlacementSpawnEntry(ctx, entry, play);
    if (!actor || (!actor->update && !actor->init))
    {
        if (record.operation == ENEMY_NATIVE_RESOURCE && state->resource.original.id != 0x197)
        {
            state->room = room;
            state->visit = sNativeVisits[room];
            state->operation = record.operation;
            comboEnemyNativeResourceDefeated(&state->resource);
        }
        else
            memset(state, 0, sizeof(*state));
        return 1;
    }
    state->actor = actor;
#if defined(GAME_OOT)
    if (record.operation == ENEMY_NATIVE_ARENA && sNativeArenaGeneration == 0xffffffffu)
    {
        g.roomEnemyLackSoul = 1;
        memset(state, 0, sizeof(*state));
        return 1;
    }
    if (record.operation == ENEMY_NATIVE_ARENA)
        state->generation = ++sNativeArenaGeneration;
#endif
    state->init = actor->init;
    state->update = actor->update;
    state->fini = actor->fini;
    state->room = room;
    state->visit = sNativeVisits[room];
    state->operation = record.operation;
    state->virtualFloor = virtualFloor;
#if defined(GAME_OOT)
    state->targetId = record.targetId;
    state->targetParams = record.targetParams;
#endif
    state->hadHealth = actor->colChkInfo.health != 0;
    if (actor->init)
        actor->init = nativePlacementInit;
    if (actor->update)
        actor->update = nativePlacementUpdate;
    actor->fini = nativePlacementFini;
    return 1;

fallbackSeed:
    if (g.roomEnemyLackSoul != hadLackSoul)
        return 1;
    record = seedRecord;
    entry = seedEntry;
    state = NULL;
    chaotic = 0;
    goto retrySeed;
}

void comboEnemyNativePlacementUpdate(PlayState* play)
{
    for (u32 i = 0; i < NATIVE_PLACEMENT_STATE_COUNT; ++i)
    {
        NativePlacementState* state = &sNativeStates[i];
#if defined(GAME_MM)
        if (state->operation == 5 && state->actor && !state->init && !state->actor->init &&
            state->actor->update && nativePlacementCurrent(state, play) && sNativeScope != state)
        {
            for (Actor* member = play->actorCtx.actors[ACTORCAT_ENEMY].first; member; member = member->next)
                if (member == state->actor)
                {
                    state->actor->update = state->update;
                    state->actor->fini = state->fini;
                    memset(state, 0, sizeof(*state));
                    break;
                }
        }
#endif
        if (state->operation == ENEMY_NATIVE_RESOURCE)
        {
            if (!nativePlacementCurrent(state, play))
            {
                if (!state->actor)
                {
                    comboEnemyFamilyClear(&sNativeFamilies[state - sNativeStates]);
                    memset(state, 0, sizeof(*state));
                }
                continue;
            }
            comboEnemyNativeResourceUpdate(play, &state->resource);
            if (!state->actor && state->resource.state == ENEMY_RESOURCE_RESTORED)
                memset(state, 0, sizeof(*state));
        }
    }
}

static int enemyGroundActive(Actor* actor)
{
    return actor && actor->update && (actor->category == ACTORCAT_ENEMY || actor->category == ACTORCAT_BOSS ||
        (actor->category == ACTORCAT_NPC && comboEnemyFamilyIsGold(actor)));
}

static int enemyGroundLive(Actor* actor)
{
    return enemyGroundActive(actor) && comboEnemyGroundBound(actor);
}

static void enemyGroundCapture(Actor* actor, EnemyGroundStep* step, int native)
{
    f32 gap;
    step->active = 0;
    if (actor && actor->colChkInfo.health)
    {
        NativePlacementState* state = nativePlacementState(actor);
        if (state && state->operation == 11)
            return;
    }
    if (enemyGroundActive(actor) && comboEnemyAirBound(actor) &&
        actor->floorHeight > -30000.f && actor->floorHeight < 30000.f)
    {
        step->actor = actor;
        step->position = actor->world.pos;
        step->floorHeight = actor->floorHeight;
        step->room = actor->room;
        step->category = actor->category;
        step->active = 2;
        return;
    }
    if ((!native && !enemyGroundLive(actor)) || !actor->floorPoly ||
        !(actor->floorHeight > -30000.f && actor->floorHeight < 30000.f))
        return;
    if (!(actor->world.pos.x >= -32768.f && actor->world.pos.x <= 32767.f &&
          actor->world.pos.z >= -32768.f && actor->world.pos.z <= 32767.f))
        return;
    gap = actor->world.pos.y - actor->floorHeight;
    if (!(gap >= -20.f && gap <= 100.f))
        return;
    step->actor = actor;
    step->position = actor->world.pos;
    step->floor = actor->floorPoly;
    step->floorHeight = actor->floorHeight;
    step->floorBgId = actor->floorBgId;
    step->room = actor->room;
    step->category = actor->category;
    step->active = native ? 3 : 1;
}

void comboEnemyGroundGuardBegin(PlayState* play, Actor* actor, EnemyGroundGuard* guard)
{
    (void)play;
    enemyGroundCapture(actor, &guard->steps[0], 0);
    enemyGroundCapture(actor ? actor->child : NULL, &guard->steps[1], 0);
}

void comboEnemyGroundGuardBeginNative(Actor* actor, EnemyGroundGuard* guard)
{
    enemyGroundCapture(actor, &guard->steps[0], 1);
    guard->steps[1].active = 0;
}

static void enemyGroundConstrain(PlayState* play, EnemyGroundStep* step)
{
    Actor* actor;
    CollisionPoly* floor = NULL;
    Vec3f probe;
    f32 height;
    s32 bgId;
    if (!step->active)
        return;
    actor = step->actor;
    if (step->active == 2)
    {
        if (comboEnemyAirBound(actor) && actor->update && actor->room == step->room &&
            actor->category == step->category &&
            !(actor->floorHeight > -30000.f && actor->floorHeight < 30000.f))
        {
            actor->floorHeight = step->floorHeight;
            if (actor->world.pos.y < -30000.f || step->position.y - actor->world.pos.y > 1000.f)
            {
                actor->world.pos.y = step->position.y;
                actor->prevPos.y = step->position.y;
            }
        }
        return;
    }
    if (!actor->update || (step->active == 1 && !enemyGroundLive(actor)) || actor->room != step->room || actor->category != step->category ||
        (actor->world.pos.x == step->position.x && actor->world.pos.z == step->position.z))
        return;
    probe = actor->world.pos;
    if (!(probe.x >= -32768.f && probe.x <= 32767.f && probe.z >= -32768.f && probe.z <= 32767.f &&
          probe.y >= -32768.f && probe.y <= 32767.f))
        goto restore;
    if (probe.y < step->position.y)
        probe.y = step->position.y;
    probe.y += 50.f;
#if defined(GAME_MM)
    height = BgCheck_EntityRaycastFloor5_2(play, &play->colCtx, &floor, &bgId, actor, &probe);
#else
    height = BgCheck_EntityRaycastDown4(&play->colCtx, &floor, &bgId, actor, &probe);
#endif
    if (floor && height > -30000.f && height < 30000.f && step->floorHeight - height <= (step->active == 3 ? 35.f : 60.f))
        return;
restore:
    actor->world.pos.x = step->position.x;
    actor->world.pos.z = step->position.z;
    if (actor->velocity.y < 0.f && actor->world.pos.y <= step->floorHeight + 5.f)
        actor->velocity.y = 0.f;
    actor->floorPoly = step->floor;
    actor->floorHeight = step->floorHeight;
    actor->floorBgId = step->floorBgId;
}

void comboEnemyGroundGuardEnd(PlayState* play, EnemyGroundGuard* guard)
{
    enemyGroundConstrain(play, &guard->steps[0]);
    enemyGroundConstrain(play, &guard->steps[1]);
}

int comboEnemyAirPlacement(PlayState* play, Actor* actor, f32 clearance)
{
    CollisionPoly* floor = NULL;
    Vec3f probe;
    f32 height;
    s32 bgId;
    if (!play || !actor || !(clearance > 0.f && clearance <= 200.f))
        return 0;
    probe = actor->home.pos;
    if (!(probe.x >= -32768.f && probe.x <= 32767.f &&
          probe.z >= -32768.f && probe.z <= 32767.f &&
          probe.y >= -30000.f && probe.y <= 30000.f))
        return 0;
    probe.y += 20.f;
#if defined(GAME_MM)
    height = BgCheck_EntityRaycastFloor5_2(play, &play->colCtx, &floor, &bgId, actor, &probe);
#else
    height = BgCheck_EntityRaycastDown4(&play->colCtx, &floor, &bgId, actor, &probe);
#endif
    if (!floor || !(height > -30000.f && height < 30000.f) ||
        height + clearance <= actor->home.pos.y)
        return 0;
    actor->home.pos.y = height + clearance;
    actor->world.pos.y = height + clearance;
    actor->prevPos.y = height + clearance;
    actor->floorPoly = floor;
    actor->floorHeight = height;
    actor->floorBgId = bgId;
    return 1;
}

extern u8 gCurrentSpawnActorNum;

#if defined(GAME_OOT)
#define NativeEncounterSpawnEntry     Actor_SpawnEntry
#define NativeEncounterDelete         ((Actor * (*)(ActorContext*, Actor*, PlayState*))0x80025648)
#define NativeEncounterGetTemp        ((s32(*)(PlayState*, s32))0x80020694)
#define NativeEncounterSetPermanent   ((void (*)(PlayState*, s32))0x80020658)
#define NativeEncounterUnsetPermanent ((void (*)(PlayState*, s32))0x80020674)
#define NativeEncounterUnsetTemp      ((void (*)(PlayState*, s32))0x800206c8)
#else
#define NativeEncounterSpawnEntry     ((Actor * (*)(ActorContext*, ActorEntry*, PlayState*))0x800bb2d0)
#define NativeEncounterDelete         ((Actor * (*)(ActorContext*, Actor*, PlayState*))0x800bb498)
#define NativeEncounterGetTemp        ((s32(*)(PlayState*, s32))0x800b5d18)
#define NativeEncounterSetPermanent   ((void (*)(PlayState*, s32))0x800b5cdc)
#define NativeEncounterUnsetPermanent ((void (*)(PlayState*, s32))0x800b5cf8)
#define NativeEncounterUnsetTemp      ((void (*)(PlayState*, s32))0x800b5d4c)
#endif

typedef struct
{
    Actor* actor;

    union
    {
        EnemyPlacement placement;

        struct
        {
            u32 listVrom, visit;
            u8 room, slot, complete, shared;
        };
    };
} EnemyEncounter;

_Static_assert(sizeof(EnemyEncounter) == 16, "Enemy encounter state");

typedef struct
{
    u32 listVrom;
    u32 visit;
    u32 remaining[8];
    int hasForeign;
} EnemyRoom;

static u32 sDeferredRoomActors[8];
static int sDeferRoomActors;
static int sAllocationRetryPhase;
static EnemyPlacement sAllocationRetryPlacement;
static ActorEntry* sAllocationRetryList;
static EnemyRoom sRooms[32];
static EnemyEncounter sEncounters[256];
static ActorEntry* sRoomList;
static u32 sRoomCount;
static s32 sListRoom = -1;
static EnemyPlacement sNextPlacement;
static s16 sNextActor = -1;
static u32 sPendingRooms;
static u32 sBlockedRooms;
static u32 sPersistentRequests;
static s32 sCompletingRoom = -1;

#if defined(GAME_MM)
static Actor* nativePlacementForeignSpawn(ActorContext* ctx, ActorEntry* entry, PlayState* play)
{
    s32 room = play->roomCtx.curRoom.num;
    u32 slot = gCurrentSpawnActorNum;
    if (room < 0 || room >= 32 || room != sListRoom || slot >= sRoomCount ||
        !sRooms[room].listVrom || !sRooms[room].visit)
        return NULL;
    EnemyPlacement previous = sNextPlacement;
    s16 previousId = sNextActor;
    sRooms[room].hasForeign = 1;
    sRooms[room].remaining[slot >> 5] |= 1u << (slot & 31);
    sNextPlacement = (EnemyPlacement){ sRooms[room].listVrom, sRooms[room].visit, room, slot };
    sNextActor = entry->id & 0x1fff;
    Actor* actor = NativePlacementSpawnEntry(ctx, entry, play);
    sNextPlacement = previous;
    sNextActor = previousId;
    return actor;
}
#endif

int comboEnemyDefeatTransitionValid(const EnemyDefeatTransition* t, u32 root, u32 privateEnd, u32 profileOffset)
{
    if (!t->field && !t->from && !t->to)
        return 1;
    return root && privateEnd >= sizeof(Actor) + 4 && t->field >= sizeof(Actor) && t->field <= privateEnd - 4 &&
           !((t->field | t->from | t->to) & 3) && t->from != t->to && t->from < profileOffset && t->to < profileOffset;
}

int comboEnemyDefeatState(const Actor* actor, const EnemyDefeatTransition* t, const void* overlay, u32 action)
{
    return t->field && *(const u32*)((const u8*)actor + t->field) == (u32)overlay + action;
}

#if defined(GAME_OOT)
typedef struct
{
    ActorEntry* list;
    u32 firstKey;
    u16 count;
    u16 scene;
    u8 mq;
} OotEnemizerRoomKeys;

static OotEnemizerRoomKeys sOotEnemizerRoomKeys[32];

static void ootEnemizerCaptureKeys(PlayState* play, ActorEntry* list, u32 count)
{
    s32 room;
    u32 key, address, segment, start, end;
    OotEnemizerRoomKeys* state;
    RomFile* files;
    if (!play)
        return;
    room = play->roomCtx.curRoom.num;
    if (room < 0 || room >= 32)
        return;
    if (play->sceneId == 1 && room == 3)
        comboOotEnemizerResetLizalfos();
    state = &sOotEnemizerRoomKeys[room];
    memset(state, 0, sizeof(*state));
    if (!list || !count || count > 255)
        return;
    if (comboMqEnemyPlacementKey(play, list, &key))
    {
        *state = (OotEnemizerRoomKeys){ list, key, count, play->sceneId, 1 };
        return;
    }
    files = play->roomList;
    segment = (u32)play->roomCtx.curRoom.segment;
    address = (u32)list;
    if (!files || room >= play->numRooms || !segment || address < segment)
        return;
    start = (u32)files[room].vromStart;
    end = (u32)files[room].vromEnd;
    if (!start || end <= start || count * sizeof(*list) > end - start || address - segment > end - start - count * sizeof(*list))
        return;
    *state = (OotEnemizerRoomKeys){ list, start + address - segment, count, play->sceneId, 0 };
}

static int ootEnemizerPlacementKey(PlayState* play, u32 room, u32 slot, u32* out)
{
    const OotEnemizerRoomKeys* state;
    if (!play || !out || room >= 32 || slot == 0xff)
        return 0;
    state = &sOotEnemizerRoomKeys[room];
    if (!state->list || !state->firstKey || state->scene != play->sceneId || slot >= state->count)
        return 0;
    *out = state->firstKey + slot * (state->mq ? 1 : sizeof(ActorEntry));
    return 1;
}

typedef struct
{
    u32 visit;
    u32 slots[8];
    u16 geometries;
} OotHallwayWait;
static OotHallwayWait sOotHallwayWait[32];

static void ootHallwayBegin(PlayState* play, ActorEntry* list, u32 count)
{
    s32 room = play->roomCtx.curRoom.num;
    if (room < 0 || room >= 32)
        return;
    OotHallwayWait* wait = &sOotHallwayWait[room];
    memset(wait, 0, sizeof(*wait));
    if (play->sceneId != 3 || !list || count > 255 || sOotEnemizerRoomKeys[room].list != list)
        return;
    wait->visit = sNativeVisits[room];
    for (u32 i = 0; i < count; ++i)
        if (list[i].id == 0x68 && !(list[i].params & 0x4000))
            ++wait->geometries;
}

static int ootHallwayPending(s32 room)
{
    if (room < 0 || room >= 32)
        return 0;
    OotHallwayWait* wait = &sOotHallwayWait[room];
    if (!wait->geometries || !wait->visit || wait->visit != sNativeVisits[room])
        return 0;
    for (u32 i = 0; i < ARRAY_COUNT(wait->slots); ++i)
        if (wait->slots[i])
            return 1;
    return 0;
}

static int ootHallwayDefer(PlayState* play, ActorEntry* entry)
{
    s32 room = play->roomCtx.curRoom.num;
    if (!sDeferRoomActors || room < 0 || room >= 32 || Flags_GetClear(play, room) || entry->id == 0x68)
        return 0;
    OotHallwayWait* wait = &sOotHallwayWait[room];
    OotEnemizerRoomKeys* keys = &sOotEnemizerRoomKeys[room];
    if (!wait->geometries || wait->visit != sNativeVisits[room] || !keys->list || keys->scene != play->sceneId)
        return 0;
    u32 distance = (u32)entry - (u32)keys->list;
    if (distance % sizeof(*entry) || distance / sizeof(*entry) >= keys->count)
        return 0;
    u32 slot = distance / sizeof(*entry), key;
    NativePlacement record;
    if (!comboIsForeignEnemyActor(entry->id) &&
        (!ootEnemizerPlacementKey(play, room, slot, &key) || !nativePlacementLookup(key, &record) ||
         memcmp(entry, &record.replacement, sizeof(*entry))))
        return 0;
    wait->slots[slot >> 5] |= 1u << (slot & 31);
    NativeEncounterUnsetTemp(play, room);
    return 1;
}

static void ootHallwayPostFrame(PlayState* play)
{
    s32 room = play->roomCtx.curRoom.num;
    if (!ootHallwayPending(room))
        return;
    OotHallwayWait* wait = &sOotHallwayWait[room];
    OotEnemizerRoomKeys* keys = &sOotEnemizerRoomKeys[room];
    if (!keys->list || keys->scene != play->sceneId || sDeferRoomActors)
        return;
    NativeEncounterUnsetTemp(play, room);
    u32 ready = 0;
    for (u32 i = 0; i < BG_ACTOR_MAX; ++i)
    {
        BgActor* bg = &play->colCtx.dyna.bgActors[i];
        if ((play->colCtx.dyna.bgActorFlags[i] & 5) == 1 && bg->colHeader && bg->colHeader->numPolygons &&
            bg->actor && bg->actor->update && bg->actor->id == 0x68 && bg->actor->room == room)
            ++ready;
    }
    if (ready < wait->geometries || !play->colCtx.dyna.polyList)
        return;
    if (!comboEnemyRoomModelsPreload(play))
        return;
    u8 previousActor = gCurrentSpawnActorNum;
    ActorEntry* previousList = sRoomList;
    u32 previousCount = sRoomCount;
    s32 previousRoom = sListRoom;
    if (gForeignEnemyRegistryVrom)
    {
        if (!sRooms[room].visit || !sRooms[room].listVrom)
            return;
        sRoomList = keys->list;
        sRoomCount = keys->count;
        sListRoom = room;
    }
    for (u32 slot = 0; slot < keys->count; ++slot)
    {
        u32 mask = 1u << (slot & 31);
        if (!(wait->slots[slot >> 5] & mask))
            continue;
        wait->slots[slot >> 5] &= ~mask;
        gCurrentSpawnActorNum = slot;
        comboEnemySpawnRoomEntry(&play->actorCtx, &keys->list[slot], play);
    }
    sRoomList = previousList;
    sRoomCount = previousCount;
    sListRoom = previousRoom;
    gCurrentSpawnActorNum = previousActor;
}

int comboEnemyActorPlacementKey(Actor* actor, PlayState* play, u32* out)
{
    if (!actor || !ootEnemizerPlacementKey(play, actor->room, actor->actorIndex, out))
        return 0;
    return sOotEnemizerRoomKeys[actor->room].list[actor->actorIndex].id == actor->id;
}
#endif

static void nativePlacementDefeated(Actor* actor, PlayState* play)
{
    NativePlacementState* state = nativePlacementState(actor);
    if (!state || !nativePlacementCurrent(state, play) || actor->init || actor->update || comboEnemyFamilyNoClear(actor))
        return;
#if defined(GAME_OOT)
    u32 key = state->splitPlacementVrom;
    if (key || comboEnemyActorPlacementKey(actor, play, &key))
        comboOotEnemizerDefeated(play, key);
#endif
    sPendingRooms |= 1u << state->room;
}

void comboEnemyEncounterReset(void)
{
    comboEnemyNativePlacementReset();
#if defined(GAME_MM)
    comboEnemyDependenciesReset();
#endif
    sDeferRoomActors = sAllocationRetryPhase = 0;
    sAllocationRetryList = NULL;
    memset(sDeferredRoomActors, 0, sizeof(sDeferredRoomActors));
    memset(sEncounters, 0, sizeof(sEncounters));
    memset(sRooms, 0, sizeof(sRooms));
#if defined(GAME_OOT)
    memset(sOotEnemizerRoomKeys, 0, sizeof(sOotEnemizerRoomKeys));
    memset(sOotHallwayWait, 0, sizeof(sOotHallwayWait));
#endif
    sRoomList = NULL;
    sRoomCount = 0;
    sListRoom = sNextActor = sCompletingRoom = -1;
    sPendingRooms = sBlockedRooms = sPersistentRequests = 0;
}

static s16 enemyRoomActorId(const ActorEntry* entry)
{
#if defined(GAME_MM)
    return entry->id & 0x1fff;
#else
    return entry->id;
#endif
}

void comboEnemyEncounterBeginRoom(PlayState* play, const void* cmd, ActorEntry* list, u32 count)
{
    s32 room;
    u32 segment, start, end, offset, bit;
    RomFile* files;
    EnemyRoom* state;

    comboEnemyNativePlacementBeginRoom(play);
    comboEnemyNativePlacementSetActorList(play, list, count);
#if defined(GAME_MM)
    comboEnemyDependenciesBeginRoom(play, list, count);
#endif
#if defined(GAME_OOT)
    ootEnemizerCaptureKeys(play, list, count);
    ootHallwayBegin(play, list, count);
#endif
#if defined(GAME_OOT)
    if (!gForeignEnemyRegistryVrom)
        return;
#endif
    room = play->roomCtx.curRoom.num;
    sListRoom = sNextActor = -1;
    if (room < 0 || room >= 32)
        return;
    segment = (u32)play->roomCtx.curRoom.segment;
    files = play->roomList;
    if (room >= play->numRooms || !files || !segment || (u32)cmd < segment)
        goto unverified;
    start = (u32)files[room].vromStart;
    end = (u32)files[room].vromEnd;
    offset = (u32)cmd - segment;
    if (!start || end <= start || end - start < 8 || offset > end - start - 8 || offset % 4 ||
        *(const u8*)cmd != 1 || count != ((const u8*)cmd)[1] || (count && !list))
        goto unverified;

    state = &sRooms[room];
    bit = 1u << room;
    if (state->visit == 0xffffffffu)
    {
        sBlockedRooms |= bit;
        return;
    }
    ++state->visit;
    state->listVrom = start + offset;
    memset(state->remaining, 0, sizeof(state->remaining));
    state->hasForeign = 0;
    sPendingRooms &= ~bit;
    sPersistentRequests &= ~bit;
    sBlockedRooms &= ~bit;
    sListRoom = room;
    sRoomList = list;
    sRoomCount = count;
    if (Flags_GetClear(play, room))
        return;
    for (u32 i = 0; i < count; ++i)
    {
        if (!comboIsForeignEnemyActor(enemyRoomActorId(&list[i])))
            continue;
        state->hasForeign = 1;
        state->remaining[i >> 5] |= 1u << (i & 31);
#if defined(GAME_MM)
        u32 days = ((list[i].rot.x & 7) << 7) | (list[i].rot.z & 0x7f);
        if (days && days != 0x3ff)
            sBlockedRooms |= bit;
#endif
    }
    if (state->hasForeign)
        NativeEncounterUnsetTemp(play, room);
    return;

unverified:
    if (!count)
        return;
    if (!list || count > 255)
    {
        sBlockedRooms |= 1u << room;
        return;
    }
    for (u32 i = 0; i < count; ++i)
        if (comboIsForeignEnemyActor(enemyRoomActorId(&list[i])))
        {
            sBlockedRooms |= 1u << room;
            return;
        }
}

static int enemyCurrentPlacement(const EnemyPlacement* p)
{
    return p->room < 32 && p->visit && sRooms[p->room].visit == p->visit && (!p->listVrom || sRooms[p->room].listVrom == p->listVrom);
}

#if defined(GAME_MM)
s16 comboEnemyMmStandaloneMinibossSlot(s16 id, u16 params);

static int enemyRoomStandaloneMiniboss(const ActorEntry* entry)
{
    const s16 id = entry->id & 0x1fff;
    return comboEnemyMmStandaloneMinibossSlot(id, entry->params) != id;
}
#endif

void comboEnemyBeginSpawnBatch(void)
{
#if defined(GAME_MM)
    comboEnemyDependenciesSpawnActors(gPlay);
#endif
    sAllocationRetryPhase = 0;
    memset(sDeferredRoomActors, 0, sizeof(sDeferredRoomActors));
    sDeferRoomActors = 1;
}

void comboEnemyEndSpawnBatch(ActorContext* ctx, PlayState* play)
{
    u8 previous = gCurrentSpawnActorNum;
    sDeferRoomActors = 0;
#if defined(GAME_MM)
    if (sListRoom >= 0 && sListRoom == play->roomCtx.curRoom.num && sRoomCount <= 255)
    {
        for (u32 slot = 0; slot < sRoomCount; ++slot)
        {
            u32 mask = 1u << (slot & 31);
            if ((sDeferredRoomActors[slot >> 5] & mask) &&
                enemyRoomStandaloneMiniboss(&sRoomList[slot]))
            {
                sDeferredRoomActors[slot >> 5] &= ~mask;
                gCurrentSpawnActorNum = slot;
                comboEnemySpawnRoomEntry(ctx, &sRoomList[slot], play);
            }
        }
    }
#endif
    sAllocationRetryPhase = sListRoom >= 0 && sListRoom < 32 && sListRoom == play->roomCtx.curRoom.num && sRoomCount <= 255;
    if (sAllocationRetryPhase)
    {
        sAllocationRetryPlacement = (EnemyPlacement){ sRooms[sListRoom].listVrom, sRooms[sListRoom].visit, sListRoom, 0 };
        sAllocationRetryList = sRoomList;
    }
    int previousLackSoul = g.roomEnemyLackSoul;
    if (!comboEnemyRoomModelsPreload(play))
    {
#if defined(GAME_MM)
        if (sAllocationRetryPhase)
        {
            for (u32 slot = 0; slot < sRoomCount; ++slot)
            {
                u32 mask = 1u << (slot & 31);
                if (!(sDeferredRoomActors[slot >> 5] & mask) ||
                    comboIsForeignEnemyActor(enemyRoomActorId(&sRoomList[slot])))
                    continue;
                sDeferredRoomActors[slot >> 5] &= ~mask;
                gCurrentSpawnActorNum = slot;
                comboEnemySpawnRoomEntry(ctx, &sRoomList[slot], play);
            }
        }
#endif
        if (sAllocationRetryPhase && comboEnemyRoomModelsOnlyOom())
        {
            g.roomEnemyLackSoul = previousLackSoul;
            sAllocationRetryPhase = 2;
        }
        else
        {
            g.roomEnemyLackSoul = 1;
            sAllocationRetryPhase = 0;
            memset(sDeferredRoomActors, 0, sizeof(sDeferredRoomActors));
        }
        gCurrentSpawnActorNum = previous;
        return;
    }
    for (u32 slot = 0; slot < sRoomCount && slot < 256; ++slot)
        if (sDeferredRoomActors[slot >> 5] & (1u << (slot & 31)))
        {
            sDeferredRoomActors[slot >> 5] &= ~(1u << (slot & 31));
            gCurrentSpawnActorNum = slot;
            comboEnemySpawnRoomEntry(ctx, &sRoomList[slot], play);
        }
    sAllocationRetryPhase = sAllocationRetryPhase ? 2 : 0;
    gCurrentSpawnActorNum = previous;
}

static int virtualRoomFloorEntry(PlayState* play, const ActorEntry* entry, ActorEntry* floorEntry)
{
    NativePlacement record;
    int instancePlacement;
    if (!sVirtualFloorPlacementVrom || !nativeRoomRecord(play, entry, &record, &instancePlacement) ||
        !virtualFloorPlacement(record.vrom) ||
        memcmp(entry, instancePlacement ? &record.original : &record.replacement, sizeof(*entry)))
        return 0;
    u32 operation = record.operation & ~(ENEMY_NATIVE_GROUND | ENEMY_NATIVE_FLOOR | ENEMY_NATIVE_AIR | ENEMY_NATIVE_UPRIGHT);
    /* These own genuine scripted families and set the floor when the target
     * is created. Do not wrap the monitor or arena geometry as an enemy. */
    if (operation == ENEMY_NATIVE_RESOURCE || operation == ENEMY_NATIVE_ARENA || operation == 128)
        return 0;
    *floorEntry = record.replacement;
#if defined(GAME_MM)
    floorEntry->id &= 0x1fff;
#endif
    return 1;
}

void comboEnemySpawnRoomEntry(ActorContext* ctx, ActorEntry* entry, PlayState* play)
{
    const s32 room = play->roomCtx.curRoom.num;
    const s16 id = enemyRoomActorId(entry);
    const u32 distance = (u32)entry - (u32)sRoomList;
    int defer = comboIsForeignEnemyActor(id);
#if defined(GAME_OOT)
    if (ootHallwayDefer(play, entry))
        return;
#endif
#if defined(GAME_MM)
    defer |= enemyRoomStandaloneMiniboss(entry);
    if (!defer && sDeferRoomActors && sNativePlacementVrom && room >= 0 &&
        room == sListRoom && room < play->numRooms && play->roomList)
    {
        u32 segment = (u32)play->roomCtx.curRoom.segment;
        RomFile* files = play->roomList;
        u32 start = (u32)files[room].vromStart;
        u32 end = (u32)files[room].vromEnd;
        u32 offset = (u32)entry - segment;
        NativePlacement record;
        u32 key = start + offset;
        int grottoInstance = play->sceneId == SCE_MM_GROTTOS && room == 4 &&
                             (gGrottoData & 0x1f) >= 0x13;
        if (grottoInstance)
            key = 0xe0000000u + ((gGrottoData & 0x1f) - 0x13) * 0x10000 + offset;
        if (segment && (u32)entry >= segment && end > start &&
            end - start >= sizeof(*entry) && offset <= end - start - sizeof(*entry) &&
            nativePlacementLookup(key, &record) &&
            !memcmp(entry, grottoInstance ? &record.original : &record.replacement, sizeof(*entry)))
            defer = 1;
    }

#endif
    if (sDeferRoomActors && defer && room >= 0 && room == sListRoom &&
        distance % sizeof(*entry) == 0 && distance / sizeof(*entry) < sRoomCount && sRoomCount <= 255)
    {
        u32 slot = distance / sizeof(*entry);
        sDeferredRoomActors[slot >> 5] |= 1u << (slot & 31);
        return;
    }
    EnemyVirtualFloorScope floor = {0};
    ActorEntry floorEntry;
    if (virtualRoomFloorEntry(play, entry, &floorEntry) && !comboEnemyVirtualFloorBegin(&floor, play, &floorEntry))
    {
        g.roomEnemyLackSoul = 1;
        return;
    }
    if (!gForeignEnemyRegistryVrom)
    {
        if (!comboEnemyNativePlacementSpawn(ctx, entry, play))
            NativeEncounterSpawnEntry(ctx, entry, play);
        comboEnemyVirtualFloorEnd(&floor, NULL);
        return;
    }
    EnemyPlacement previous = sNextPlacement;
    s16 previousId = sNextActor;

    sNextActor = -1;
    if (comboIsForeignEnemyActor(id) && room >= 0 && room == sListRoom &&
        distance % sizeof(*entry) == 0 && distance / sizeof(*entry) < sRoomCount)
    {
        u32 slot = distance / sizeof(*entry);
        sNextPlacement = (EnemyPlacement){ sRooms[room].listVrom, sRooms[room].visit, room, slot };
        sNextActor = id;
    }
    if (!comboEnemyNativePlacementSpawn(ctx, entry, play))
        NativeEncounterSpawnEntry(ctx, entry, play);
    comboEnemyVirtualFloorEnd(&floor, NULL);
    sNextPlacement = previous;
    sNextActor = previousId;
}

#if defined(GAME_MM)
int comboEnemyScriptedCapturePlacement(Actor* monitor, PlayState* play, const ActorEntry* original)
{
    EnemyScriptedBinding* binding = (EnemyScriptedBinding*)monitor;
    binding->placementVisit = 0;
    return comboEnemyNativeScriptedCurrent(monitor, play) && binding->binding == ENEMY_SCRIPTED_MAGIC;
}

Actor* comboEnemyScriptedSpawnTarget(Actor* monitor, PlayState* play, f32 x, f32 y, f32 z, s16 yaw, EnemyFamily* family)
{
    EnemyScriptedBinding* binding = (EnemyScriptedBinding*)monitor;
    if (!comboEnemyNativeScriptedCurrent(monitor, play) || binding->binding != ENEMY_SCRIPTED_MAGIC)
        return NULL;
    ActorEntry entry = { .id = binding->targetId, .params = binding->targetParams,
        .pos = { x, y, z }, .rot = { 7, ((u16)(s16)(yaw * (360.f / 65536.f)) << 7) | 0x7f, 0x7f } };
    NativePlacementState* state = nativePlacementState(monitor);
    if (state && state->virtualFloor && !comboEnemyFamilySetVirtualFloor(family, play, &entry)) return NULL;
    return comboEnemyFamilySpawn(family, &play->actorCtx, &entry, play, 0xffff);
}

EXPORT_SYMBOL(ENEMY_SCRIPTED_MM_LEASE_V1, comboEnemyScriptedSpawnTarget);
#endif

int comboEnemyTakePlacement(PlayState* play, s16 actorId, EnemyPlacement* out)
{
    if (sNextActor < 0 || sNextActor != actorId || play->roomCtx.curRoom.num != sNextPlacement.room ||
        !enemyCurrentPlacement(&sNextPlacement))
        return 0;
    *out = sNextPlacement;
    sNextActor = -1;
    return 1;
}

static u32 sDynamicChoices __attribute__((section(".data")));
#if defined(GAME_OOT)
EXPORT_SYMBOL(ENEMY_DYNAMIC_CHOICES_OOT, sDynamicChoices);
#else
EXPORT_SYMBOL(ENEMY_DYNAMIC_CHOICES_MM, sDynamicChoices);
#endif

static u32 dynamicOperation(s16 id, u16 params)
{
    u32 count;
    if (!sDynamicChoices || comboDmaLoadFilePartial(&count, sDynamicChoices, 0, 4) != 4 || count > 2048)
        return 0;
    u32 low = 0, high = count;
    while (low < high)
    {
        u32 mid = low + (high - low) / 2;
        u16 choice[4];
        if (comboDmaLoadFilePartial(choice, sDynamicChoices, 4 + mid * 8, 8) != 8)
            return 0;
        if (choice[0] == (u16)id && choice[1] == params)
            return choice[2];
        if (choice[0] < (u16)id || (choice[0] == (u16)id && choice[1] < params))
            low = mid + 1;
        else
            high = mid;
    }
    return 0;
}

static Actor* enemyDynamicSpawnImpl(ActorContext* ctx, ActorEntry* entry, PlayState* play, u32 operation)
{
    s16 id = entry->id;
#if defined(GAME_MM)
    id &= 0x1fff;
#endif
    if (operation == 0xffff)
        operation = dynamicOperation(id, entry->params);
    if (!comboIsForeignEnemyActor(id))
        return nativePlacementTraitsSpawn(ctx, entry, play, operation, 0);
    s32 room = play->roomCtx.curRoom.num;
    if (room < 0 || room >= 32 || !sRooms[room].visit)
        return NULL;
    EnemyPlacement previous = sNextPlacement;
    s16 previousId = sNextActor;
    u32 slot;
    for (slot = 0; slot < 256; ++slot)
    {
        int used = 0;
        for (u32 i = 0; i < ARRAY_COUNT(sEncounters); ++i)
            if (sEncounters[i].actor && !sEncounters[i].placement.listVrom &&
                sEncounters[i].placement.room == room && sEncounters[i].placement.visit == sRooms[room].visit &&
                sEncounters[i].placement.slot == slot)
                used = 1;
        if (!used)
            break;
    }
    if (slot == 256)
        return NULL;
    sNextPlacement = (EnemyPlacement){ 0, sRooms[room].visit, room, slot };
    sNextActor = id;
    Actor* actor = nativePlacementPolicySpawn(ctx, entry, play, 0);
    sNextPlacement = previous;
    sNextActor = previousId;
    return actor;
}

Actor* comboEnemyDynamicSpawn(ActorContext* ctx, ActorEntry* entry, PlayState* play, u32 operation)
{
    if (operation == 0xffff || !(operation & ENEMY_DYNAMIC_VIRTUAL_FLOOR))
        return enemyDynamicSpawnImpl(ctx, entry, play, operation);
    EnemyVirtualFloorScope floor;
    if (!comboEnemyVirtualFloorBegin(&floor, play, entry))
    {
        g.roomEnemyLackSoul = 1;
        return NULL;
    }
    Actor* actor = enemyDynamicSpawnImpl(ctx, entry, play, operation & ~ENEMY_DYNAMIC_VIRTUAL_FLOOR);
    comboEnemyVirtualFloorEnd(&floor, actor);
    return actor;
}

static EnemyEncounter* enemyEncounter(Actor* actor)
{
    for (u32 i = 0; i < ARRAY_COUNT(sEncounters); ++i)
        if (actor && sEncounters[i].actor == actor)
            return &sEncounters[i];
    return NULL;
}

static int enemySamePlacement(const EnemyPlacement* a, const EnemyPlacement* b)
{
    return a->room == b->room && a->visit == b->visit && a->listVrom == b->listVrom && a->slot == b->slot;
}

int comboEnemyEncounterRetryAllocation(const EnemyPlacement* placement)
{
    if (sAllocationRetryPhase != 1 || !enemyCurrentPlacement(placement) ||
        sAllocationRetryList != sRoomList || placement->room != sAllocationRetryPlacement.room ||
        placement->visit != sAllocationRetryPlacement.visit || placement->listVrom != sAllocationRetryPlacement.listVrom ||
        placement->slot >= sRoomCount || !(sRooms[placement->room].remaining[placement->slot >> 5] & (1u << (placement->slot & 31))))
        return 0;
    for (u32 i = 0; i < ARRAY_COUNT(sEncounters); ++i)
        if (sEncounters[i].actor && enemySamePlacement(&sEncounters[i].placement, placement))
            return 0;
    sDeferredRoomActors[placement->slot >> 5] |= 1u << (placement->slot & 31);
    return 1;
}

void comboEnemyEncounterPostFrame(PlayState* play)
{
#if defined(GAME_MM)
    comboEnemyDependenciesSpawnActors(play);
#endif
    if (sAllocationRetryPhase == 2 && !comboEnemyCurrentActor() && sCompletingRoom < 0)
    {
        u32 retry[8];
        u8 previous = gCurrentSpawnActorNum;
        EnemyPlacement identity = sAllocationRetryPlacement;
        ActorEntry* list = sAllocationRetryList;
        sAllocationRetryPhase = 0;
        memcpy(retry, sDeferredRoomActors, sizeof(retry));
        memset(sDeferredRoomActors, 0, sizeof(sDeferredRoomActors));
        int pending = 0;
        for (u32 i = 0; i < ARRAY_COUNT(retry); ++i)
            pending |= retry[i];
        if (pending && enemyCurrentPlacement(&identity) && list == sRoomList && identity.room == play->roomCtx.curRoom.num)
        {
            if (!comboEnemyRoomModelsPreload(play))
            {
                g.roomEnemyLackSoul = 1;
            }
            else
                for (u32 slot = 0; slot < sRoomCount && slot < 256; ++slot)
                {
                    identity.slot = slot;
                    if (!(retry[slot >> 5] & (1u << (slot & 31))) || !enemyCurrentPlacement(&identity) || list != sRoomList ||
                        identity.room != play->roomCtx.curRoom.num || !(sRooms[identity.room].remaining[slot >> 5] & (1u << (slot & 31))))
                        continue;
                    u32 i;
                    for (i = 0; i < ARRAY_COUNT(sEncounters); ++i)
                        if (sEncounters[i].actor && enemySamePlacement(&sEncounters[i].placement, &identity))
                            break;
                    if (i != ARRAY_COUNT(sEncounters))
                        continue;
                    gCurrentSpawnActorNum = slot;
                    comboEnemySpawnRoomEntry(&play->actorCtx, &list[slot], play);
                }
        }
        gCurrentSpawnActorNum = previous;
    }
#if defined(GAME_OOT)
    ootHallwayPostFrame(play);
#endif
    comboEnemyEncounterReconcile(play);
}
#if defined(GAME_MM)
void comboEnemyScriptedCancelPlacement(Actor* monitor, PlayState* play)
{
    EnemyScriptedBinding* binding = (EnemyScriptedBinding*)monitor;
    if (!comboEnemyNativeScriptedCurrent(monitor, play) || !binding->placementVisit)
        return;
    EnemyPlacement placement = { binding->placementVrom, binding->placementVisit, binding->placementRoom, binding->placementSlot };
    if (!enemyCurrentPlacement(&placement))
        return;
    for (u32 i = 0; i < ARRAY_COUNT(sEncounters); ++i)
        if (sEncounters[i].actor && enemySamePlacement(&sEncounters[i].placement, &placement))
        {
            comboEnemyEncounterBlock(&placement);
            return;
        }
    sRooms[placement.room].remaining[placement.slot >> 5] &= ~(1u << (placement.slot & 31));
    sPendingRooms |= 1u << placement.room;
    binding->placementVisit = 0;
}
#endif

int comboEnemyEncounterSplitPlacement(Actor* owner, EnemyPlacement* out)
{
    EnemyEncounter* e = enemyEncounter(owner);
    if (!e || e->complete || !enemyCurrentPlacement(&e->placement))
        return 0;
    e->shared = 1;
    *out = e->placement;
    return 1;
}

u32 comboEnemyEncounterSplitKey(PlayState* play, const EnemyPlacement* placement)
{
    if (!placement->listVrom)
        return 0;
#if defined(GAME_OOT)
    u32 key;
    if (ootEnemizerPlacementKey(play, placement->room, placement->slot, &key))
        return key;
#endif
    return placement->listVrom + placement->slot * sizeof(ActorEntry);
}

void comboEnemyEncounterBlock(const EnemyPlacement* p)
{
    if (p->listVrom && enemyCurrentPlacement(p))
        sBlockedRooms |= 1u << p->room;
}

static int enemyRegisterMember(Actor* actor, const EnemyPlacement* p, int split)
{
    EnemyEncounter* available = NULL;
    int hasOwner = 0;
    if (!actor || !p || !enemyCurrentPlacement(p) || (p->listVrom && !(sRooms[p->room].remaining[p->slot >> 5] & (1u << (p->slot & 31)))))
        return 0;
    for (u32 i = 0; i < ARRAY_COUNT(sEncounters); ++i)
    {
        EnemyEncounter* e = &sEncounters[i];
        if (!e->actor)
        {
            if (!available)
                available = e;
            continue;
        }
        if (split && e->shared && !e->complete && enemySamePlacement(&e->placement, p))
            hasOwner = 1;
        if (e->actor == actor || (!split && enemySamePlacement(&e->placement, p)))
            return 0;
    }
    if (!available || (split && !hasOwner))
        return 0;
    *available = (EnemyEncounter){ .actor = actor, .placement = *p };
    available->complete = 0;
    available->shared = split;
    return 1;
}

int comboEnemyEncounterRegister(Actor* actor, const EnemyPlacement* p)
{
    return enemyRegisterMember(actor, p, 0);
}

int comboEnemyEncounterRegisterMember(Actor* actor, const EnemyPlacement* p)
{
    return enemyRegisterMember(actor, p, 1);
}

void comboEnemyEncounterRelease(Actor* actor)
{
    EnemyEncounter* e = enemyEncounter(actor);
    if (!e)
        return;
    if (e->shared && !e->complete)
        comboEnemyEncounterBlock(&e->placement);
    e->actor = NULL;
}

#if defined(GAME_MM)
int comboEnemyRoomSlotComplete(s32 room, u32 slot)
{
    return room >= 0 && room < 32 && slot < 256 && sRooms[room].visit &&
           !(sRooms[room].remaining[slot >> 5] & (1u << (slot & 31)));
}
#endif

int comboEnemyEncounterPending(s32 room)
{
    if (room < 0 || room >= 32)
        return 0;
    if (comboEnemyNativePlacementPending(room))
        return 1;
#if defined(GAME_OOT)
    if (ootHallwayPending(room))
        return 1;
#endif
    if (sBlockedRooms & (1u << room))
        return 1;
    for (u32 i = 0; i < ARRAY_COUNT(sRooms[room].remaining); ++i)
        if (sRooms[room].remaining[i])
            return 1;
    return 0;
}

void comboEnemyEncounterReconcile(PlayState* play)
{
    comboEnemyNativePlacementUpdate(play);
    u32 pending;
    if (g.roomEnemyLackSoul && play->roomCtx.curRoom.num >= 0 && play->roomCtx.curRoom.num < 32 && sRooms[play->roomCtx.curRoom.num].hasForeign)
        sBlockedRooms |= 1u << play->roomCtx.curRoom.num;
    pending = sPendingRooms & ~sBlockedRooms;
    if (!pending)
        return;
    s32 activeRoom = sCompletingRoom >= 0 ? sCompletingRoom : play->roomCtx.curRoom.num;
    for (Actor* actor = play->actorCtx.actors[ACTORCAT_ENEMY].first; actor; actor = actor->next)
    {
        if (!actor->update || (actor->room >= 0 && actor->room != activeRoom))
            continue;
        EnemyEncounter* entry = enemyEncounter(actor);
        if (!entry || !entry->complete)
            return;
    }
    if ((u32)activeRoom >= 32)
        return;
    u32 bit = 1u << activeRoom;
    if (!(pending & bit) || comboEnemyEncounterPending(activeRoom))
        return;
    Flags_SetClear(play, activeRoom);
    if (sPersistentRequests & bit)
        NativeEncounterSetPermanent(play, activeRoom);
    sPersistentRequests &= ~bit;
    sPendingRooms &= ~bit;
}

static int enemyWinningCleanup(void)
{
    Actor* actor = comboEnemyCurrentActor();
    for (u32 count = 0; actor && count < 256; ++count, actor = actor->parent)
    {
        EnemyEncounter* owner = enemyEncounter(actor);
        if (owner && enemyCurrentPlacement(&owner->placement) && owner->complete)
            return 1;
    }
    return 0;
}

void comboEnemyEncounterComplete(Actor* actor, PlayState* play)
{
    EnemyEncounter* entry = enemyEncounter(actor);
    s32 previousRoom;
    if (!entry || !enemyCurrentPlacement(&entry->placement) || entry->complete)
        return;
    if (comboEnemyCurrentCallback() == 1)
        return;
    if (comboEnemyCurrentCallback() == 0)
    {
        g.roomEnemyLackSoul = 1;
        return;
    }
    entry->complete = 1;
    if (!entry->placement.listVrom)
        return;
    if (entry->shared)
        for (u32 i = 0; i < ARRAY_COUNT(sEncounters); ++i)
            if (sEncounters[i].actor && !sEncounters[i].complete &&
                enemySamePlacement(&entry->placement, &sEncounters[i].placement))
                return;
#if defined(GAME_OOT)
    {
        u32 key;
        if (ootEnemizerPlacementKey(play, entry->placement.room, entry->placement.slot, &key))
            comboOotEnemizerDefeated(play, key);
    }
#endif
    sRooms[entry->placement.room].remaining[entry->placement.slot >> 5] &= ~(1u << (entry->placement.slot & 31));
    sPendingRooms |= 1u << entry->placement.room;
    previousRoom = sCompletingRoom;
    sCompletingRoom = entry->placement.room;
    comboEnemyEncounterReconcile(play);
    sCompletingRoom = previousRoom;
}

void comboEnemyEncounterAfterUpdate(Actor* actor, PlayState* play, const EnemyDefeatTransition* t,
                                    const void* overlay, int inDefeatAction)
{
#if defined(GAME_OOT)
    if (actor->id == 0xd2d && actor->params == 2 && comboEnemyCurrentCallback() == 2)
        comboEnemyEncounterComplete(actor, play);
#endif
    int transitioned = inDefeatAction && comboEnemyDefeatState(actor, t, overlay, t->to);
#if defined(GAME_OOT)
    if (transitioned && actor->id == 0xd64)
        transitioned = (comboEnemyUsesNativePort(actor) || (t->field == 0x13c && t->from == 0x10a4 && t->to == 0xf78)) &&
                       (actor->params == 0 || actor->params == 1) && (*((u8*)actor + 0x143) & 8);
    if (transitioned && actor->id == 0xd6f)
        transitioned = (comboEnemyUsesNativePort(actor) || (t->field == 0x180 && t->from == 0x194c && t->to == 0x10ec)) && actor->params == 1;
#endif
    if (!actor->update || transitioned)
    {
        EnemyEncounter* entry = enemyEncounter(actor);
        if (transitioned && actor->update && entry && !entry->complete && enemyCurrentPlacement(&entry->placement) &&
            (actor->category == ACTORCAT_ENEMY || actor->category == ACTORCAT_BOSS))
            Actor_ChangeCategory(play, &play->actorCtx, actor, ACTORCAT_MISC);
        comboEnemyEncounterComplete(actor, play);
    }
}

EXPORT_SYMBOL(FOREIGN_ENEMY_RESPAWN_DEFEAT_V1, comboEnemyEncounterAfterUpdate);

static s32 enemySourceRoom(s32 fallback)
{
    Actor* actor = comboEnemyCurrentActor();
    for (u32 count = 0; actor && count < 256; ++count, actor = actor->parent)
    {
        EnemyEncounter* entry = enemyEncounter(actor);
        if (entry)
            return entry->placement.listVrom ? entry->placement.room : -1;
    }
    actor = comboEnemyCurrentActor();
    return actor && actor->room >= 0 ? actor->room : fallback;
}

void enemyActorKill(Actor* actor)
{
    Actor_Kill(actor);
    comboEnemyEncounterComplete(actor, gPlay);
}

#if defined(GAME_MM)
static void enemyActorEscape(Actor* actor)
{
    EnemyEncounter* entry = enemyEncounter(actor);
    if (entry)
    {
        comboEnemyEncounterBlock(&entry->placement);
        comboEnemyEncounterRelease(actor);
    }
    Actor_Kill(actor);
}

EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_ESCAPE, enemyActorEscape);
#endif

static Actor* enemyActorDelete(ActorContext* ctx, Actor* actor, PlayState* play)
{
    comboEnemyEncounterComplete(actor, play);
    return NativeEncounterDelete(ctx, actor, play);
}

void enemyActorChangeCategory(PlayState* play, ActorContext* ctx, Actor* actor, u8 category)
{
    Actor_ChangeCategory(play, ctx, actor, category);
#if defined(GAME_OOT)
    if (actor->id == 0xd2d && actor->params == 2 && comboEnemyCurrentCallback() == 2)
        return;
#endif
    if (comboEnemyCurrentCallback() != 0 && category != ACTORCAT_ENEMY && category != ACTORCAT_BOSS)
        comboEnemyEncounterComplete(actor, play);
}

static s32 enemyFlagsGetClear(PlayState* play, s32 room)
{
    room = enemySourceRoom(room);
    if (room < 0 || room >= 32 || comboEnemyEncounterPending(room))
        return 0;
    return Flags_GetClear(play, room);
}

static void enemyFlagsSet(PlayState* play, s32 room, int persistent)
{
    Actor* actor = comboEnemyCurrentActor();
    for (u32 count = 0; actor && count < 256; ++count, actor = actor->parent)
        if (enemyEncounter(actor))
        {
            EnemyEncounter* entry = enemyEncounter(actor);
            if (!entry->placement.listVrom || !enemyCurrentPlacement(&entry->placement))
                return;
            if (comboEnemyCurrentCallback() != 0 && (comboEnemyCurrentCallback() != 1 || enemyWinningCleanup()))
            {
                if (persistent)
                    sPersistentRequests |= 1u << entry->placement.room;
                if (entry->complete)
                {
                    s32 previousRoom = sCompletingRoom;
                    sPendingRooms |= 1u << entry->placement.room;
                    sCompletingRoom = entry->placement.room;
                    comboEnemyEncounterReconcile(play);
                    sCompletingRoom = previousRoom;
                    return;
                }
            }
            comboEnemyEncounterComplete(actor, play);
            return;
        }
    g.roomEnemyLackSoul = 1;
}

static void enemyFlagsSetClear(PlayState* play, s32 room)
{
    enemyFlagsSet(play, room, 1);
}

static void enemyFlagsSetTempClear(PlayState* play, s32 room)
{
    enemyFlagsSet(play, room, 0);
}

static s32 enemyFlagsGetTempClear(PlayState* play, s32 room)
{
    room = enemySourceRoom(room);
    if (room < 0 || room >= 32 || comboEnemyEncounterPending(room))
        return 0;
    return NativeEncounterGetTemp(play, room);
}

static void enemyFlagsUnset(PlayState* play, s32 room, int persistent)
{
    Actor* actor = comboEnemyCurrentActor();
    for (u32 count = 0; actor && count < 256; ++count, actor = actor->parent)
    {
        EnemyEncounter* entry = enemyEncounter(actor);
        if (!entry)
            continue;
        if (!entry->placement.listVrom || !enemyCurrentPlacement(&entry->placement))
            return;
        entry->complete = 0;
        sRooms[entry->placement.room].remaining[entry->placement.slot >> 5] |= 1u << (entry->placement.slot & 31);
        sPendingRooms &= ~(1u << entry->placement.room);
        if (persistent)
            sPersistentRequests &= ~(1u << entry->placement.room);
        NativeEncounterUnsetTemp(play, entry->placement.room);
        if (persistent)
            NativeEncounterUnsetPermanent(play, entry->placement.room);
        return;
    }
    g.roomEnemyLackSoul = 1;
}

static void enemyFlagsUnsetClear(PlayState* play, s32 room)
{
    enemyFlagsUnset(play, room, 1);
}

static void enemyFlagsUnsetTempClear(PlayState* play, s32 room)
{
    enemyFlagsUnset(play, room, 0);
}

EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_KILL, enemyActorKill);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_DELETE, enemyActorDelete);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_CATEGORY, enemyActorChangeCategory);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_GET_CLEAR, enemyFlagsGetClear);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SET_CLEAR, enemyFlagsSetClear);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_GET_TEMP_CLEAR, enemyFlagsGetTempClear);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_SET_TEMP_CLEAR, enemyFlagsSetTempClear);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_UNSET_CLEAR, enemyFlagsUnsetClear);
EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_UNSET_TEMP_CLEAR, enemyFlagsUnsetTempClear);
