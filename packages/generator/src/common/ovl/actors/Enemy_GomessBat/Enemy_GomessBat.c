#include <combo/enemy_gomess_bat.h>
#include <combo/common/actors/En_Item00.h>
#include <combo/effect.h>
#include <combo/sfx_source.h>
#include <combo/matrix.h>

#if defined(GAME_MM)
#define BatSfx Actor_PlaySfx
#define BatWorldSfx SfxSource_PlaySfxAtFixedWorldPos
#define BatLoopSfx Actor_PlaySfx_Flagged
#define BatStunMask (DMG_DEKU_NUT | DMG_UNK_0x12)
#define BatReceiveMask 0xf7cfffff
#define BatObject 0x52
#else
void FOREIGN_ENEMY_API_SFX_ACTOR(Actor*, u16);
#define BatSfx FOREIGN_ENEMY_API_SFX_ACTOR
void FOREIGN_ENEMY_API_SFX_WORLD(PlayState*, Vec3f*, s32, u16);
#define BatWorldSfx FOREIGN_ENEMY_API_SFX_WORLD
void FOREIGN_ENEMY_API_SFX_FLAGGED(Actor*, u16);
#define BatLoopSfx FOREIGN_ENEMY_API_SFX_FLAGGED
#define BatStunMask DMG_DEKU_NUT
#define BatReceiveMask (~(u32)(DMG_SHIELD | DMG_MIR_RAY | DMG_UNBLOCKABLE))
#define BatObject 0x1052
#define NA_SE_EV_BAT_FLY 0x29a7
#define NA_SE_EN_COMMON_EXTINCT_LEV 0x3a1f
#endif

static Vec3f Bat_EffectVelocity(s16 pitch, s16 yaw)
{
    f32 horizontal = 3.f * Math_CosS(pitch);
    return (Vec3f){ horizontal * Math_SinS(yaw), 3.f * Math_SinS(pitch), horizontal * Math_CosS(yaw) };
}

static void Bat_ResetEffects(EnemyGomessBat* bat)
{
    for (int i = 0; i < ARRAY_COUNT(bat->effects); ++i)
    {
        bat->effectOffset[i] =
            (Vec3f){ Rand_ZeroOne() * 80.f - 40.f, Rand_ZeroOne() * 40.f, Rand_ZeroOne() * 80.f - 40.f };
        bat->effectFrame[i] = (u8)(Rand_ZeroOne() * 9.f);
        s16 pitch = (s32)Rand_Next() >> 17;
        s16 yaw = (s32)Rand_Next() >> 16;
        bat->effectVelocity[i] = Bat_EffectVelocity(pitch, yaw);
    }
}

static int Bat_ClearArea(const CollisionContext* col, const Vec3f* pos, f32 halfExtent)
{
    if (!col->lookupTbl || !col->colHeader)
        return 0;
    Vec3f offset = { pos->x - col->minBounds.x, pos->y - col->minBounds.y, pos->z - col->minBounds.z };
    if (!(offset.x >= halfExtent && offset.y >= halfExtent && offset.z >= halfExtent &&
          pos->x + halfExtent < col->maxBounds.x && pos->y + halfExtent < col->maxBounds.y &&
          pos->z + halfExtent < col->maxBounds.z))
        return 0;
    s32 x = offset.x * col->subdivLengthInv.x;
    s32 y = offset.y * col->subdivLengthInv.y;
    s32 z = offset.z * col->subdivLengthInv.z;
    if (x < 0 || x >= col->subdivAmount.x || y < 0 || y >= col->subdivAmount.y || z < 0 || z >= col->subdivAmount.z)
        return 0;
    f32 dx = offset.x - x * col->subdivLength.x;
    f32 dy = offset.y - y * col->subdivLength.y;
    f32 dz = offset.z - z * col->subdivLength.z;
    if (!(dx > halfExtent && dx < col->subdivLength.x - halfExtent && dy > halfExtent &&
          dy < col->subdivLength.y - halfExtent && dz > halfExtent && dz < col->subdivLength.z - halfExtent))
        return 0;
    const StaticLookup* cell = &col->lookupTbl[x + col->subdivAmount.x * (y + col->subdivAmount.y * z)];
    if ((cell->floor.head & cell->wall.head & cell->ceiling.head) == 0xffff)
        return 1;
    if (!col->polyNodes.tbl || !col->colHeader->polyList)
        return 0;
    u16 lists[] = { cell->floor.head, cell->wall.head, cell->ceiling.head };
    int budget = 8;
    for (int list = 0; list < ARRAY_COUNT(lists); ++list)
    {
        for (u16 index = lists[list]; index != 0xffff; index = col->polyNodes.tbl[index].next)
        {
            if (!budget-- || index >= col->polyNodes.count)
                return 0;
            s32 polyId = col->polyNodes.tbl[index].polyId;
            if (polyId < 0 || polyId >= col->colHeader->numPolygons)
                return 0;
            const CollisionPoly* poly = &col->colHeader->polyList[polyId];
            if (poly->flags_vIA & 0x4000)
                continue;
            f32 distance =
                pos->x * poly->normal.x + pos->y * poly->normal.y + pos->z * poly->normal.z + poly->dist * 32767.f;
            f32 projected =
                (halfExtent + 2.f) * (ABS((s32)poly->normal.x) + ABS((s32)poly->normal.y) + ABS((s32)poly->normal.z));
            if (fabsf(distance) <= projected)
                return 0;
        }
    }
    return 1;
}

static u32 sBatClearCacheGeneration;

static u8 sBatDynamicSlots[BG_ACTOR_MAX];
static int sBatDynamicCount;
static const PlayState* sBatDynamicPlay;
static u32 sBatDynamicFrame;
static u32 sBatDynamicGeneration;

static int Bat_HasDynamicGeometry(const PlayState* play, const Vec3f* pos, f32 halfExtent)
{
    const DynaCollisionContext* dyna = &play->colCtx.dyna;
    if (!dyna->polyNodes.count)
        return 0;
    if (sBatDynamicPlay != play || sBatDynamicFrame != play->gameplayFrames ||
        sBatDynamicGeneration != sBatClearCacheGeneration)
    {
        sBatDynamicPlay = play;
        sBatDynamicFrame = play->gameplayFrames;
        sBatDynamicGeneration = sBatClearCacheGeneration;
        sBatDynamicCount = 0;
        for (int i = 0; i < BG_ACTOR_MAX; ++i)
        {
            u16 flags = dyna->bgActorFlags[i];
            if (!(flags & 1))
                continue;
#if defined(GAME_MM)
            if (flags & 2)
                continue;
#endif
            sBatDynamicSlots[sBatDynamicCount++] = i;
        }
    }
    f32 minY = pos->y - halfExtent;
    f32 maxY = pos->y + halfExtent;
    for (int i = 0; i < sBatDynamicCount; ++i)
    {
        const BgActor* bg = &dyna->bgActors[sBatDynamicSlots[i]];
        if (maxY < bg->minY || minY > bg->maxY)
            continue;
        const Sphere16* sphere = &bg->boundingSphere;
        f32 radius = ABS((s32)sphere->radius) + 1.f;
        f32 dx = MAX(0.f, fabsf(pos->x - sphere->center.x) - halfExtent);
        f32 dz = MAX(0.f, fabsf(pos->z - sphere->center.z) - halfExtent);
        if (dx > radius || dz > radius)
            continue;
        f32 dy = MAX(0.f, fabsf(pos->y - sphere->center.y) - halfExtent);
        if (dx * dx + dy * dy + dz * dz > radius * radius)
            continue;
        return 1;
    }
    return 0;
}

static int Bat_ClearStaticEffectArea(PlayState* play, const Vec3f* pos, f32 fallbackExtent)
{
    enum { BUCKET_SIZE = 8, CACHE_SIZE = 128 };
    typedef struct {
        s32 x, y, z;
        u8 clearExtent;
        u8 used;
        u8 checkedShort;
        u8 checkedMedium;
        u8 checkedWide;
        u8 checkedExact;
        f32 exactClearExtent;
        f32 exactBlockedExtent;
        Vec3f exactPos;
    } ClearEntry;
    static ClearEntry cache[CACHE_SIZE];
    static PlayState* owner;
    static const CollisionHeader* header;
    static const StaticLookup* lookup;
    static u32 generation;
    static s16 scene;
    static s8 room;
    if (fallbackExtent > 48.f)
        return Bat_ClearArea(&play->colCtx, pos, fallbackExtent);
    if (owner != play || generation != sBatClearCacheGeneration || scene != play->sceneId ||
        room != play->roomCtx.curRoom.num || header != play->colCtx.colHeader || lookup != play->colCtx.lookupTbl)
    {
        memset(cache, 0, sizeof(cache));
        owner = play;
        header = play->colCtx.colHeader;
        lookup = play->colCtx.lookupTbl;
        generation = sBatClearCacheGeneration;
        scene = play->sceneId;
        room = play->roomCtx.curRoom.num;
    }
    s32 x = (s32)pos->x / BUCKET_SIZE;
    s32 y = (s32)pos->y / BUCKET_SIZE;
    s32 z = (s32)pos->z / BUCKET_SIZE;
    u32 hash = (u32)x * 73856093u ^ (u32)y * 19349663u ^ (u32)z * 83492791u;
    ClearEntry* entry = &cache[hash & (CACHE_SIZE - 1)];
    if (!entry->used || entry->x != x || entry->y != y || entry->z != z)
    {
        entry->x = x;
        entry->y = y;
        entry->z = z;
        entry->used = 1;
        entry->clearExtent = 0;
        entry->checkedShort = 0;
        entry->checkedMedium = 0;
        entry->checkedWide = 0;
        entry->checkedExact = 0;
    }
    if (fallbackExtent <= entry->clearExtent)
        return 1;
    int extent = fallbackExtent <= 8.f ? 8 : fallbackExtent <= 24.f ? 24 : 48;
    u8* checked = extent == 8 ? &entry->checkedShort : extent == 24 ? &entry->checkedMedium : &entry->checkedWide;
    if (!*checked)
    {
        Vec3f anchor = { x * BUCKET_SIZE, y * BUCKET_SIZE, z * BUCKET_SIZE };
        *checked = 1;
        if (Bat_ClearArea(&play->colCtx, &anchor, extent + BUCKET_SIZE))
        {
            entry->clearExtent = extent;
            return 1;
        }
    }
    if (!entry->checkedExact || entry->exactPos.x != pos->x || entry->exactPos.y != pos->y ||
        entry->exactPos.z != pos->z)
    {
        entry->exactPos = *pos;
        entry->exactClearExtent = 0.f;
        entry->exactBlockedExtent = 49.f;
        entry->checkedExact = 1;
    }
    if (fallbackExtent <= entry->exactClearExtent)
        return 1;
    if (fallbackExtent >= entry->exactBlockedExtent)
        return 0;
    if (Bat_ClearArea(&play->colCtx, pos, fallbackExtent))
    {
        entry->exactClearExtent = fallbackExtent;
        return 1;
    }
    entry->exactBlockedExtent = fallbackExtent;
    return 0;
}

static int Bat_ClearEffectArea(PlayState* play, const Vec3f* pos, f32 halfExtent)
{
    return Bat_ClearStaticEffectArea(play, pos, halfExtent) && !Bat_HasDynamicGeometry(play, pos, halfExtent);
}

static int Bat_ClearStaticSegment(const CollisionContext* col, const Vec3f* start, const Vec3f* end)
{
    if (!col->lookupTbl || !col->colHeader || !col->polyNodes.tbl || !col->colHeader->polyList)
        return 0;

    Vec3i first = { (start->x - col->minBounds.x) * col->subdivLengthInv.x,
                    (start->y - col->minBounds.y) * col->subdivLengthInv.y,
                    (start->z - col->minBounds.z) * col->subdivLengthInv.z };
    Vec3i last = { (end->x - col->minBounds.x) * col->subdivLengthInv.x,
                   (end->y - col->minBounds.y) * col->subdivLengthInv.y,
                   (end->z - col->minBounds.z) * col->subdivLengthInv.z };
    Vec3i min = { MIN(first.x, last.x), MIN(first.y, last.y), MIN(first.z, last.z) };
    Vec3i max = { MAX(first.x, last.x), MAX(first.y, last.y), MAX(first.z, last.z) };
    if (min.x < 0 || max.x >= col->subdivAmount.x || min.y < 0 || max.y >= col->subdivAmount.y || min.z < 0 ||
        max.z >= col->subdivAmount.z)
        return 0;

    s32 crossings = max.x - min.x + max.y - min.y + max.z - min.z;
    if (crossings > 1)
        return 0;
    s32 stride = max.x != min.x ? 1 : max.y != min.y ? col->subdivAmount.x : col->subdivAmount.x * col->subdivAmount.y;
    const StaticLookup* cell = &col->lookupTbl[min.x + col->subdivAmount.x * (min.y + col->subdivAmount.y * min.z)];
    u16 lists[6] = { cell->floor.head, cell->wall.head, cell->ceiling.head };
    if (crossings)
    {
        cell += stride;
        lists[3] = cell->floor.head;
        lists[4] = cell->wall.head;
        lists[5] = cell->ceiling.head;
    }
    int budget = 24;
    for (int list = 0; list < 3 * (crossings + 1); ++list)
    {
        for (u16 index = lists[list]; index != 0xffff; index = col->polyNodes.tbl[index].next)
        {
            if (!budget-- || index >= col->polyNodes.count)
                return 0;
            s32 polyId = col->polyNodes.tbl[index].polyId;
            if (polyId < 0 || polyId >= col->colHeader->numPolygons)
                return 0;
            const CollisionPoly* poly = &col->colHeader->polyList[polyId];
            if (poly->flags_vIA & 0x4000)
                continue;

            f32 origin = poly->dist * 32767.f;
            f32 a = start->x * poly->normal.x + start->y * poly->normal.y + start->z * poly->normal.z + origin;
            f32 b = end->x * poly->normal.x + end->y * poly->normal.y + end->z * poly->normal.z + origin;
            if (!((a > 1024.f && b > 1024.f) || (a < -1024.f && b < -1024.f)))
                return 0;
        }
    }
    return 1;
}

static int Bat_ClearSegment(PlayState* play, const Vec3f* start, const Vec3f* end, f32 extent)
{
    Vec3f midpoint = { start->x + (end->x - start->x) * .5f, start->y + (end->y - start->y) * .5f,
                       start->z + (end->z - start->z) * .5f };
    f32 halfExtent = extent * .5f + .1f;
    return (Bat_ClearStaticEffectArea(play, &midpoint, halfExtent) ||
            Bat_ClearStaticSegment(&play->colCtx, start, end)) &&
           !Bat_HasDynamicGeometry(play, &midpoint, halfExtent);
}

static int Bat_ClearPath(PlayState* play, Vec3f* start, Vec3f* end)
{
    f32 extent = MAX(fabsf(end->x - start->x), MAX(fabsf(end->y - start->y), fabsf(end->z - start->z))) + .1f;
    return extent == .1f || Bat_ClearSegment(play, start, end, extent) || enemyFlightClear(play, start, end);
}

static int Bat_ClearLaunch(PlayState* play, const Vec3f* center)
{
    static PlayState* owner;
    static u32 frame;
    static u32 generation;
    static const StaticLookup* lookup;
    static const CollisionHeader* header;
    static Vec3f cachedCenter;
    static int clear;

    if (owner != play || frame != play->gameplayFrames || generation != sBatClearCacheGeneration ||
        lookup != play->colCtx.lookupTbl || header != play->colCtx.colHeader || center->x != cachedCenter.x ||
        center->y != cachedCenter.y || center->z != cachedCenter.z)
    {
        owner = play;
        frame = play->gameplayFrames;
        generation = sBatClearCacheGeneration;
        lookup = play->colCtx.lookupTbl;
        header = play->colCtx.colHeader;
        cachedCenter = *center;
        clear = Bat_ClearArea(&play->colCtx, center, 60.1f);
    }
    return clear && !Bat_HasDynamicGeometry(play, center, 60.1f);
}

static void Bat_PlaceEffect(EnemyGomessBat* bat, PlayState* play, int index, int* clearCell, int checkGeometry)
{
    Vec3f* offset = &bat->effectOffset[index];
    Vec3f* pos = &bat->actor.world.pos;
    Vec3f next = { pos->x + offset->x, pos->y + offset->y, pos->z + offset->z };
    if (!checkGeometry || (next.x == pos->x && next.y == pos->y && next.z == pos->z))
    {
        bat->effects[index] = next;
        return;
    }

    Vec3f hit;
    CollisionPoly* poly;
    s32 bgId;
    f32 extent = MAX(fabsf(next.x - pos->x), MAX(fabsf(next.y - pos->y), fabsf(next.z - pos->z))) + .1f;
    if (extent <= 48.f && *clearCell < 0)
        *clearCell = Bat_ClearEffectArea(play, pos, 48.f);
    int clear = extent <= 48.f && *clearCell > 0;
    if (!clear)
        clear = Bat_ClearSegment(play, pos, &next, extent);
    if (!clear && BgCheck_EntityLineTest1(&play->colCtx, pos, &next, &hit, &poly, 1, 1, 1, 0, &bgId))
    {
        next.x = pos->x + (hit.x - pos->x) * .8f;
        next.y = pos->y + (hit.y - pos->y) * .8f;
        next.z = pos->z + (hit.z - pos->z) * .8f;
    }
    bat->effects[index] = next;
}

static void Bat_MoveEffect(EnemyGomessBat* bat, PlayState* play, int index, f32 limit, int* clearCell,
                           int checkGeometry)
{
    Vec3f* offset = &bat->effectOffset[index];
    Vec3f* velocity = &bat->effectVelocity[index];
    offset->x += velocity->x;
    offset->y += velocity->y;
    offset->z += velocity->z;
    f32 radiusSq = offset->x * offset->x + offset->z * offset->z;
    if (offset->y < 0.f || offset->y > limit || radiusSq > limit * limit)
    {
        Vec3f center = { 0.f, 0.f, 0.f };
        s16 pitch = -gomessBatAtan2S(offset->y, sqrtf(radiusSq)) + ((s32)Rand_Next() >> 19);
        s16 yaw = Math_Vec3f_Yaw(offset, &center) + ((s32)Rand_Next() >> 19);
        *velocity = Bat_EffectVelocity(pitch, yaw);
    }

    Bat_PlaceEffect(bat, play, index, clearCell, checkGeometry);
    if (++bat->effectFrame[index] == 9)
        bat->effectFrame[index] = 0;
}

static void Bat_Init(Actor* actor, PlayState* play)
{
    ++sBatClearCacheGeneration;
    static ColliderJntSphElementInit elements[3] = {
        [0 ... 2] = {
            { ELEM_MATERIAL_UNK0, {0xf7cfffff, 0, 4}, {BatReceiveMask, 0, 0},
              ATELEM_ON | ATELEM_SFX_HARD, ACELEM_ON | ACELEM_NO_DAMAGE, OCELEM_NONE },
            {0, {{0, 0, 0}, 15}, 100},
        },
    };
    static ColliderJntSphInit init = {
        { COL_MATERIAL_NONE, AT_ON | AT_TYPE_ENEMY, AC_ON | AC_TYPE_PLAYER, OC1_NONE | OC1_TYPE_ALL, OC2_TYPE_1,
          COLSHAPE_JNTSPH },
        ARRAY_COUNT(elements),
        elements,
    };
    EnemyGomessBat* bat = (EnemyGomessBat*)actor;
    actor->cullingVolumeScale = 100.f;
    actor->cullingVolumeDownward = 100.f;
    Collider_InitJntSph(play, &bat->body);
    Collider_SetJntSph(play, &bat->body, actor, &init, bat->elements);
    for (int i = 0; i < ARRAY_COUNT(bat->elements); ++i)
        bat->elements[i].dim.worldSphere.radius = 15;
    bat->yaw = actor->world.rot.y;
    bat->deathAlpha = 255;
    bat->crowdState = 1;
    Bat_ResetEffects(bat);
}

static void Bat_Destroy(Actor* actor, PlayState* play)
{
    EnemyGomessBat* bat = (EnemyGomessBat*)actor;
    if (bat->ownerSlot)
        *bat->ownerSlot = NULL;
    Collider_DestroyJntSph(play, &bat->body);
}

static void Bat_Extinguish(EnemyGomessBat* bat, PlayState* play, Vec3f* pos)
{
    Vec3f zero = { 0.f, 0.f, 0.f };
#if defined(GAME_MM)
    func_800B3030(play, pos, &zero, &zero, 100, 0, 0);
#else

    EffectSsDeadDb_Spawn(play, pos, &zero, &zero, 100, 0, 255, 255, 255, 255, 255, 0, 0, 0, 9, 0);
#endif
    BatWorldSfx(play, pos, 11, NA_SE_EN_EXTINCT);
    if (bat->dropCooldown && !*bat->dropCooldown && Rand_ZeroOne() < .3f)
    {
        Item_DropCollectible(play, pos, Rand_ZeroOne() < .5f ? ITEM00_ARROWS_10 : ITEM00_MAGIC_LARGE);
        *bat->dropCooldown = 800;
    }
}

static void Bat_DeathUpdate(EnemyGomessBat* bat, PlayState* play)
{
    Actor* actor = &bat->actor;
    Vec3f* pos = &actor->world.pos;
    if (bat->deathStage == GOMESS_BAT_DEATH_LANDED)
    {
        int moving = 0;
        for (int i = 0; i < ARRAY_COUNT(bat->effects); ++i)
        {
            if (bat->effectOffset[i].y == 0.f)
                continue;
            Math_StepToF(&bat->effectOffset[i].y, 0.f, 7.f);
            bat->effects[i].y = pos->y + bat->effectOffset[i].y;
            moving |= bat->effectOffset[i].y != 0.f;
        }
        if (!moving)
            bat->deathStage = GOMESS_BAT_DEATH_SETTLED;
        return;
    }
    int flying = bat->deathStage == GOMESS_BAT_DEATH_FLY;
    Math_StepToF(&bat->speed, flying ? 5.f : 0.f, .2f);
    if (flying)
    {
        Math_ScaledStepToS(&bat->yaw, bat->scatterYaw, 0x480);
        if (Math_ScaledStepToS(&bat->pitch, -bat->scatterPitch, 0x480))
        {
            if (pos->y < actor->parent->home.pos.y + 50.f)
                bat->scatterPitch = -0x800 - (Rand_Next() >> 20);
            else if (pos->y > actor->parent->home.pos.y + 200.f)
                bat->scatterPitch = 0x800 + (Rand_Next() >> 20);
        }
    }
    else
    {
        Math_ScaledStepToS(&bat->pitch, -0x4000, 0x480);
        bat->deathVelocityY -= .5f;
    }

    Vec3f next = *pos;
    if (bat->speed != 0.f)
    {
        f32 horizontal = Math_CosS(bat->pitch) * bat->speed;
        next.x += Math_SinS(bat->yaw) * horizontal;
        next.y += Math_SinS(bat->pitch) * bat->speed;
        next.z += Math_CosS(bat->yaw) * horizontal;
    }
    next.y += bat->deathVelocityY;
    Vec3f hit;
    CollisionPoly* poly;
    s32 bgId;
    f32 extent = MAX(fabsf(next.x - pos->x), MAX(fabsf(next.y - pos->y), fabsf(next.z - pos->z))) + .1f;
    int blocked = extent != .1f && !Bat_ClearSegment(play, pos, &next, extent) &&
                  BgCheck_EntityLineTest1(&play->colCtx, pos, &next, &hit, &poly, 1, 1, 1, 0, &bgId);
    if (blocked && !flying && poly->normal.y <= 0x4000)
    {
        next.x = pos->x;
        next.z = pos->z;
        blocked = next.y != pos->y && !Bat_ClearSegment(play, pos, &next, fabsf(next.y - pos->y) + .1f) &&
                  BgCheck_EntityLineTest1(&play->colCtx, pos, &next, &hit, &poly, 0, 1, 0, 0, &bgId);
    }
    if (blocked)
    {
        if (!flying && poly->normal.y > 0x4000 && next.y <= pos->y)
        {
            *pos = hit;
            bat->speed = 0.f;
            bat->deathStage = GOMESS_BAT_DEATH_LANDED;
        }
        else if (flying)
        {
            bat->scatterYaw = gomessBatAtan2S(poly->normal.x, poly->normal.z) + ((s32)Rand_Next() >> 18);
        }
    }
    else if (next.y < actor->parent->home.pos.y - 250.f)
    {
        bat->deathStage = GOMESS_BAT_DEATH_LANDED;
    }
    else
    {
        *pos = next;
    }

    int clearCell = -1;
    for (int i = 0; i < ARRAY_COUNT(bat->effects); ++i)
    {
        if (flying)
        {
            Bat_MoveEffect(bat, play, i, 40.f, &clearCell, 1);
        }
        else
        {
            Vec3f* offset = &bat->effectOffset[i];
            bat->effects[i] = (Vec3f){ pos->x + offset->x, pos->y + offset->y, pos->z + offset->z };
        }
    }
}

static Vec3f Bat_FormationTarget(EnemyGomessBat* bat, PlayState* play, Vec3f* center, s16 angle)
{
    Actor* parent = bat->actor.parent;
    Vec3f target = parent->world.pos;
    target.x += 60.f * Math_SinS(angle);
    target.y += 15.f + 80.f * bat->actor.params / GOMESS_BAT_COUNT;
    target.z += 60.f * Math_CosS(angle);

    /* A shuffled arena can put a formation slot behind a wall. Regrouping
     * must finish before fallen bats recover, so use a reachable point on
     * the parent's side of that wall instead of waiting forever. */
    f32 extent = MAX(fabsf(target.x - center->x), MAX(fabsf(target.y - center->y), fabsf(target.z - center->z))) + .1f;
    Vec3f hit;
    CollisionPoly* poly;
    s32 bgId;
    if (!Bat_ClearSegment(play, center, &target, extent) &&
        BgCheck_EntityLineTest1(&play->colCtx, center, &target, &hit, &poly, 1, 1, 1, 0, &bgId))
    {
        target.x = center->x + (hit.x - center->x) * .8f;
        target.y = center->y + (hit.y - center->y) * .8f;
        target.z = center->z + (hit.z - center->z) * .8f;
    }
    return target;
}

static void Bat_Update(Actor* actor, PlayState* play)
{
    EnemyGomessBat* bat = (EnemyGomessBat*)actor;
    Actor* parent = actor->parent;
    if (!parent || !parent->update)
    {
        Actor_Kill(actor);
        return;
    }
    if (bat->dropCooldown && *bat->dropCooldown)
        --*bat->dropCooldown;
    if (bat->deathStage >= GOMESS_BAT_DEATH_DIVE)
    {
        BatLoopSfx(actor, NA_SE_EN_COMMON_EXTINCT_LEV - SFX_FLAG);
        if (bat->deathStage == GOMESS_BAT_DEATH_SETTLED)
            return;
    }
    else if (!bat->loopSfxFrame || *bat->loopSfxFrame != play->gameplayFrames)
    {
        for (int i = 0; i < ARRAY_COUNT(bat->effects); ++i)
        {
            if (!bat->hitRecovery[i])
            {
                BatLoopSfx(actor, NA_SE_EV_BAT_FLY - SFX_FLAG);
                if (bat->loopSfxFrame)
                    *bat->loopSfxFrame = play->gameplayFrames;
                break;
            }
        }
    }
    if (bat->deathStage != GOMESS_BAT_ALIVE)
    {
        Bat_DeathUpdate(bat, play);
        return;
    }
    Vec3f center = parent->focus.pos;
    Vec3f* pos = &actor->world.pos;
    int leader = actor->params == 0 || actor->params == GOMESS_BAT_COUNT / 2;
    int launched = bat->launch;
    if (launched)
    {
        bat->gather = bat->launch = 0;
        Bat_ResetEffects(bat);
        bat->attackTimer = leader ? 0 : 100;
        bat->leaderTimer = leader ? 10 : 0;
        if (!leader)
        {
            bat->crowdState = 0;
            int firstGroup = actor->params < GOMESS_BAT_COUNT / 2;
            int index = (firstGroup ? GOMESS_BAT_COUNT / 2 - 1 : GOMESS_BAT_COUNT - 1) - actor->params;
            int side = firstGroup ? -1 : 1;
            s16 yaw = parent->shape.rot.y + side * (0x4000 + 0x2000 * index);
            s16 pitch = 0xc00 + (firstGroup ? 0x200 : 0) - 0x400 * index;
            Vec3f start = { center.x + 60.f * Math_SinS(yaw), center.y - 60.f * Math_SinS(pitch),
                            center.z + 60.f * Math_CosS(yaw) };
            Vec3f hit;
            CollisionPoly* poly;
            s32 bgId;
            if (Bat_ClearLaunch(play, &center) || Bat_ClearSegment(play, &center, &start, 60.1f) ||
                !BgCheck_EntityLineTest1(&play->colCtx, &center, &start, &hit, &poly, 1, 1, 1, 0, &bgId))
                *pos = start;
            else
            {
                pos->x = center.x + (hit.x - center.x) * .8f;
                pos->y = center.y + (hit.y - center.y) * .8f;
                pos->z = center.z + (hit.z - center.z) * .8f;
            }
            bat->yaw = yaw;
            bat->pitch = -pitch;
        }
        bat->targetSlot = bat->followerSlot && *bat->followerSlot ? bat->followerSlot : NULL;
        bat->recallDistance = 50;
        bat->returning = bat->regrouping = 0;
        bat->following = !leader;
        bat->returnRoute = (EnemyFlightReturn){ 0 };
        bat->scatterTimer = 0;
        for (int i = 0; i < ARRAY_COUNT(bat->hitRecovery); ++i)
            bat->hitRecovery[i] = bat->falling[i] = 0;
        bat->speed = leader ? 0.f : 8.f;
    }
    if (bat->body.base.acFlags & AC_HIT)
    {
        bat->body.base.acFlags &= ~AC_HIT;
        int leading = leader && (bat->leaderTimer ||
                                 (bat->followerSlot && *bat->followerSlot && (*bat->followerSlot)->following));
        int effectsHit = 0;
        int crowdHit = !leading && !bat->gather && !bat->attackTimer && !bat->scatterTimer && !bat->returning;
        for (int i = 0; i < ARRAY_COUNT(bat->elements); ++i)
        {
            ColliderElement* element = &bat->elements[i].base;
            if (!(element->acElemFlags & ACELEM_HIT))
                continue;
            element->acElemFlags &= ~ACELEM_HIT;
            ColliderElement* hit = element->acHitElem;
            u32 flags = hit ? hit->atDmgInfo.dmgFlags : 0;
            if (flags & BatStunMask)
            {
                if (bat->following)
                {
                    for (int predecessor = actor->params - 1; predecessor >= 0; --predecessor)
                    {
                        EnemyGomessBat* other = bat->ownerSlot[predecessor - actor->params];
                        if (other && other->targetSlot == bat->ownerSlot)
                        {
                            other->targetSlot = bat->targetSlot;
                            break;
                        }
                    }
                    bat->targetSlot = NULL;
                    bat->attackTimer = 0;
                    bat->returning = 1;
                }
                else if (crowdHit)
                {
                    if (bat->scatterRequest)
                        *bat->scatterRequest = MAX(*bat->scatterRequest, 5);
                }
            }
            else
            {
                bat->hitRecovery[i] = 61;
                element->atElemFlags &= ~ATELEM_HIT;
                bat->falling[i] = 1;
                bat->fallSpeed[i] = -1.f;
                bat->fallYaw[i] = bat->yaw;
                if ((flags & (1u << 13)) && crowdHit && bat->scatterRequest)
                    *bat->scatterRequest = 200;
                effectsHit = 1;
            }
        }
        if (effectsHit)
        {
            if (bat->crowdState)
            {
                int inactive = 0;
                for (int effect = 0; effect < ARRAY_COUNT(bat->hitRecovery); ++effect)
                {
                    if (bat->hitRecovery[effect] && ++inactive == 2)
                    {
                        bat->crowdState = 0;
                        break;
                    }
                }
            }
            if (bat->deathSfxFrame && *bat->deathSfxFrame != play->gameplayFrames)
            {
                *bat->deathSfxFrame = play->gameplayFrames;
                BatSfx(actor, NA_SE_EN_FFLY_DEAD);
            }
        }
    }
    int attackHit = bat->body.base.atFlags & AT_HIT;
    if (attackHit)
    {
        bat->body.base.atFlags &= ~AT_HIT;
        bat->attackCooldown = 40;
        if (!launched && bat->attackTimer > 1 && bat->swarmHits)
            ++*bat->swarmHits;
    }
    s16 angle = actor->params * 0x3a00 + parent->shape.rot.y;
    Vec3f target = center;
    if (bat->attackTimer && !--bat->attackTimer)
        bat->returning = 1;
    if (bat->leaderTimer)
        --bat->leaderTimer;
    int leading =
        leader && (bat->leaderTimer || (bat->followerSlot && *bat->followerSlot && (*bat->followerSlot)->following));
    s16 yawOffset = 0;
    int arrived = 0;
    f32 recallDistanceSq = 0.f;
    int scattering = bat->scatterTimer != 0;
    int scatterYawAligned = 0;
    if (bat->gather)
    {
        target = center;
    }
    else if (leading)
    {
        target = center;
        target.x += 20.f * Math_SinS(parent->shape.rot.y);
        target.z += 20.f * Math_CosS(parent->shape.rot.y);
    }
    else if (bat->attackTimer)
    {
        if (!bat->targetSlot)
        {
            target = GET_PLAYER(play)->actor.focus.pos;
            f32 x = target.x - pos->x;
            f32 z = target.z - pos->z;
            f32 y = target.y - pos->y;
            if (x * x + y * y + z * z > 200.f * 200.f)
            {
                yawOffset = actor->params < GOMESS_BAT_COUNT / 2 ? -0x1c00 : 0x1c00;
                target.y += 50.f;
            }
        }
        else if (*bat->targetSlot && (*bat->targetSlot)->following)
            target = (*bat->targetSlot)->actor.world.pos;
        else
            target = center;
    }
    else if (scattering)
    {
        scatterYawAligned = Math_ScaledStepToS(&bat->yaw, bat->scatterYaw, 0x480);
        if (Math_ScaledStepToS(&bat->pitch, -bat->scatterPitch, 0x480))
        {
            if (pos->y < parent->home.pos.y + 50.f)
                bat->scatterPitch = -0x800 - (Rand_Next() >> 20);
            else if (pos->y > parent->home.pos.y + 200.f)
                bat->scatterPitch = 0x800 + (Rand_Next() >> 20);
        }
        if (!--bat->scatterTimer)
            bat->returning = 1;
    }
    else if (bat->returning)
    {
        target = center;
        f32 x = target.x - pos->x;
        f32 y = target.y - pos->y;
        f32 z = target.z - pos->z;
        recallDistanceSq = x * x + y * y + z * z;
        if (recallDistanceSq < bat->recallDistance * bat->recallDistance)
        {
            f32 extent = MAX(fabsf(x), MAX(fabsf(y), fabsf(z))) + .1f;
            arrived = recallDistanceSq == 0.f || Bat_ClearSegment(play, pos, &target, extent) ||
                      enemyFlightClear(play, pos, &target);
        }
        if (arrived)
        {
            bat->returning = bat->following = 0;
            bat->regrouping = 1;
            bat->crowdState = 1;
            bat->speed = 0.f;
            bat->returnRoute = (EnemyFlightReturn){ 0 };
            for (int i = 0; i < ARRAY_COUNT(bat->hitRecovery); ++i)
            {
                if (bat->hitRecovery[i])
                    bat->hitRecovery[i] = 1;
            }
        }
        else if (bat->following)
        {
            if (bat->targetSlot && *bat->targetSlot && (*bat->targetSlot)->following)
            {
                target = (*bat->targetSlot)->actor.world.pos;
            }
            else if (!bat->targetSlot && recallDistanceSq > 200.f * 200.f)
            {
                yawOffset = actor->params < GOMESS_BAT_COUNT / 2 ? -0x1c00 : 0x1c00;
                target.y += 50.f;
            }
        }
    }
    else
    {
        target = Bat_FormationTarget(bat, play, &center, angle);
    }
    int crowding = !leading && !bat->gather && !bat->attackTimer && !scattering && !bat->returning;
    int inFormation = crowding && !bat->regrouping;
    int moving = 1;
    Vec3f delta = { 0 };
    f32 distance = 60.f;
    f32 horizontalSq = 0.f;
    if (!scattering && !arrived)
    {
        delta = (Vec3f){ target.x - pos->x, target.y - pos->y, target.z - pos->z };
        horizontalSq = delta.x * delta.x + delta.z * delta.z;
        f32 distanceSq = horizontalSq + delta.y * delta.y;
        if (inFormation)
            moving = distanceSq > .01f * .01f;
        else
            distance = sqrtf(distanceSq);
    }
    int straight = leading || bat->gather || crowding;
    int aligned = 0;
    if (crowding && !arrived)
    {
        if (bat->regrouping)
        {
            aligned = Math_ScaledStepToS(&bat->yaw, angle, 0x800);
            Math_ScaledStepToS(&bat->pitch, 0, 0x800);
            bat->speed = 10.f;
        }
        else
        {
            bat->yaw = parent->shape.rot.y;
            bat->pitch = 0;
            bat->speed = 0.f;
        }
    }
    else if (straight && !arrived)
        bat->speed = 0.f;
    else if (scattering)
        Math_StepToF(&bat->speed, 6.f, .5f);
    else if (bat->following)
        Math_StepToF(&bat->speed, 6.f, .2f);
    else if (!arrived)
        Math_StepToF(&bat->speed, 6.f * MIN(1.f, distance * (1.f / 60.f)), .5f);
    if (moving && distance > .01f && !arrived)
    {
        if (!straight && !scattering)
        {
            s16 yaw = Math_Vec3f_Yaw(pos, &target) + yawOffset;
            s16 pitch = gomessBatAtan2S(delta.y, sqrtf(horizontalSq));
            if (bat->returning && !bat->following)
            {
                Math_ScaledStepToS(&bat->yaw, yaw, 0x800);
                Math_ScaledStepToS(&bat->pitch, pitch, 0x800);
            }
            else
            {
                Math_SmoothStepToS(&bat->yaw, yaw, 5, 0xc00, 0x80);
                Math_SmoothStepToS(&bat->pitch, pitch, 5, 0xc00, 0x80);
            }
        }
        Vec3f next = target;
        if (!inFormation)
        {
            f32 step = leading || bat->gather ? 5.f : bat->speed;
            f32 speed = bat->following && bat->attackTimer ? step : MIN(distance, step);
            if (speed == 0.f)
            {
                next = *pos;
            }
            else if (straight)
            {
                f32 travel = speed == distance ? 1.f : speed / distance;
                next = (Vec3f){ pos->x + delta.x * travel, pos->y + delta.y * travel, pos->z + delta.z * travel };
            }
            else
            {
                f32 horizontal = Math_CosS(bat->pitch) * speed;
                next = (Vec3f){ pos->x + Math_SinS(bat->yaw) * horizontal, pos->y + Math_SinS(bat->pitch) * speed,
                                pos->z + Math_CosS(bat->yaw) * horizontal };
            }
        }
        Vec3f hit;
        CollisionPoly* poly;
        s32 bgId;
        f32 extent = MAX(fabsf(next.x - pos->x), MAX(fabsf(next.y - pos->y), fabsf(next.z - pos->z))) + .1f;
        if (inFormation)
        {
            *pos = next;
        }
        else if (bat->returning && !scattering)
        {
            int clear = extent == .1f ||
                        (!bat->returnRoute.hasWaypoint && extent <= 48.f && Bat_ClearSegment(play, pos, &next, extent));
            int search = 0;
            if (!clear && !bat->returnRoute.hasWaypoint && !bat->returnRoute.retry)
            {
                u32 parentAddress = (u32)parent;
                u32 phase = actor->params + ((parentAddress >> 4) ^ (parentAddress >> 10));
                u32 interval = bat->returnRoute.stalledFrames >= 240 ? 32 : 8;
                search = ((play->gameplayFrames - phase) & (interval - 1)) == 0;
            }
            enemyFlightReturn(play, &bat->returnRoute, pos, &center, &next, recallDistanceSq, search, clear,
                              Bat_ClearPath);
        }
        else if (extent == .1f || Bat_ClearSegment(play, pos, &next, extent) ||
                 !BgCheck_EntityLineTest1(&play->colCtx, pos, &next, &hit, &poly, 1, 1, 1, 0, &bgId))
        {
            *pos = next;
        }
        else
        {
            if (scattering && scatterYawAligned && poly->normal.y <= (s16)(.5f * 32767.f) &&
                poly->normal.y >= (s16)(-.8f * 32767.f))
            {
                bat->scatterYaw = gomessBatAtan2S(poly->normal.x, poly->normal.z) + ((s32)Rand_Next() >> 18);
            }
            else if (!scattering && !straight)
            {
                bat->yaw += 0x4000;
            }
            enemyFlightSlideBlocked(play, pos, &next, poly, Bat_ClearPath);
        }
    }
    if (bat->regrouping && crowding && aligned)
    {
        f32 dx = pos->x - target.x;
        f32 dy = pos->y - target.y;
        f32 dz = pos->z - target.z;
        if (dx * dx + dy * dy + dz * dz < 20.f * 20.f)
        {
            bat->regrouping = 0;
            bat->speed = 0.f;
            bat->pitch = 0;
        }
    }
    int submit = !crowding || bat->regrouping || (play->gameplayFrames & 3) == (actor->params & 3);
    int attack = !bat->attackCooldown && !parent->shape.rot.x;
    int receive = submit && !bat->gather;
    attack &= submit;
    int activeSpheres = 0;
    int restoredEffects = 0;
    int clearCell = -1;
    for (int i = 0; i < ARRAY_COUNT(bat->elements); ++i)
    {
        ColliderJntSphElement* element = &bat->elements[i];
        int restored = 0;
        if (bat->falling[i] || bat->hitRecovery[i])
        {
            if (bat->falling[i])
            {
                Vec3f next = bat->effects[i], hit;
                CollisionPoly* poly;
                s32 bgId;
                next.y += bat->fallSpeed[i];
                bat->fallSpeed[i] -= 1.f;
                bat->fallYaw[i] += 0x1800;
                f32 fallExtent = fabsf(next.y - bat->effects[i].y) + .1f;
                if (next.y != bat->effects[i].y && !Bat_ClearSegment(play, &bat->effects[i], &next, fallExtent) &&
                    BgCheck_EntityLineTest1(&play->colCtx, &bat->effects[i], &next, &hit, &poly, 0, 1, 0, 0, &bgId))
                {
                    bat->effects[i] = hit;
                    Bat_Extinguish(bat, play, &hit);
                    bat->falling[i] = 0;
                }
                else if (next.y < parent->home.pos.y - 250.f)
                {
                    bat->falling[i] = 0;
                }
                else
                {
                    bat->effects[i] = next;
                }
            }
            else if (bat->hitRecovery[i] > 1)
            {
                --bat->hitRecovery[i];
            }
            else if (crowding && !bat->regrouping)
            {
                bat->hitRecovery[i] = 0;
                bat->effectOffset[i] = (Vec3f){ center.x - pos->x, center.y - pos->y, center.z - pos->z };
                restored = 1;
            }
            if (!restored)
            {
                element->base.acElemFlags = 0;
                element->base.atElemFlags = 0;
                continue;
            }
        }
        if (restored)
        {
            restoredEffects = 1;
            Bat_PlaceEffect(bat, play, i, &clearCell, !inFormation);
            if (++bat->effectFrame[i] == 9)
                bat->effectFrame[i] = 0;
        }
        else
        {
            Bat_MoveEffect(bat, play, i, leading || bat->gather ? 20.f : 40.f, &clearCell, !inFormation);
        }
        if (receive || attack)
        {
            Vec3f* effect = &bat->effects[i];
            element->dim.worldSphere.center = (Vec3s){ effect->x, effect->y, effect->z };
        }
        ++activeSpheres;
        element->base.acElemFlags = ACELEM_ON | ACELEM_NO_DAMAGE;
        element->base.atElemFlags = ATELEM_ON | ATELEM_SFX_HARD;
    }
    if (restoredEffects && activeSpheres >= 2)
        bat->crowdState = 1;
    if (attackHit)
        BatSfx(actor, NA_SE_EN_FFLY_ATTACK);
    if (activeSpheres)
    {
        if (attack)
            CollisionCheck_SetAT(play, &play->colChkCtx, &bat->body.base);
        if (receive)
            CollisionCheck_SetAC(play, &play->colChkCtx, &bat->body.base);
    }
    if (bat->attackCooldown)
        --bat->attackCooldown;
}

static ActorProfile sBatProfile = {
    ACTOR_ENEMY_GOMESS_BAT,
    ACTORCAT_PROP,
    ACTOR_FLAG_UPDATE_CULLING_DISABLED,
    BatObject,
    sizeof(EnemyGomessBat),
    Bat_Init,
    Bat_Destroy,
    Bat_Update,
    NULL,
};
OVL_INFO_ACTOR(ACTOR_ENEMY_GOMESS_BAT, sBatProfile);
