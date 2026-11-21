#include <combo.h>
#include <combo/actor_id.h>
#include <combo/dma.h>
#include <combo/enemy.h>
#include <combo/enemy_actor_layout.h>
#include <combo/enemy_gold.h>
#include <combo/enemy_native.h>
#include <combo/enemy_placement.h>
#include <combo/global.h>
#include <combo/rand.h>

#if defined(GAME_OOT)
#define STALCHILD_ID 0x1b0
#define LEEVER_ID 0x1c

typedef struct
{
    u16 id;
    u16 params;
    u16 operation;
    u16 enabled;
} BurialEnemyChoice;

static BurialEnemyChoice sBurialChoices[21] __attribute__((section(".data")));
EXPORT_SYMBOL(ENEMY_OOT_BURIAL_CHOICES, sBurialChoices);

static Actor* burialSpawn(s32 slot, ActorContext* ctx, PlayState* play, s16 id,
                          f32 x, f32 y, f32 z, s16 rx, s16 ry, s16 rz, s32 params)
{
    if (slot < 0 || !sBurialChoices[slot].enabled)
        return Actor_Spawn(ctx, play, id, x, y, z, rx, ry, rz, params);

    const BurialEnemyChoice* choice = &sBurialChoices[slot];
    ActorEntry entry = { choice->id, { x, y, z }, { rx, ry, rz }, choice->params };
    if (choice->operation & ENEMY_NATIVE_GROUND)
    {
        ActorEntry grounded;
        comboEnemyNativeFloorEntry(play, &entry, &grounded);
        entry = grounded;
    }
    u32 segment = gSegments[6];
    Actor* actor = comboEnemyDynamicSpawn(ctx, &entry, play, choice->operation & ~ENEMY_NATIVE_GROUND);
    gSegments[6] = segment;
    return actor;
}

static s32 coffinIndex(PlayState* play, f32 x, f32 z)
{
    if (play->sceneId != 8 || play->roomCtx.curRoom.num != 2)
        return -1;
    if (x > -1946.f && x < -1944.f && z > -641.f && z < -599.f)
        return 0;
    if (x > -2346.f && x < -2344.f && z > -881.f && z < -839.f)
        return 1;
    if (x > -2346.f && x < -2344.f && z > -641.f && z < -599.f)
        return 2;
    return -1;
}

static Actor* coffinFirst(ActorContext* ctx, PlayState* play, s16 id,
                          f32 x, f32 y, f32 z, s16 rx, s16 ry, s16 rz, s32 params)
{
    s32 index = coffinIndex(play, x, z);
    return burialSpawn(index * 2, ctx, play, id, x, y, z, rx, ry, rz, params);
}

static Actor* coffinSecond(ActorContext* ctx, PlayState* play, s16 id,
                           f32 x, f32 y, f32 z, s16 rx, s16 ry, s16 rz, s32 params)
{
    s32 index = coffinIndex(play, x, z);
    return burialSpawn(index * 2 + 1, ctx, play, id, x, y, z, rx, ry, rz, params);
}

static Actor* gravePoe(ActorContext* ctx, PlayState* play, s16 id,
                       f32 x, f32 y, f32 z, s16 rx, s16 ry, s16 rz, s32 params)
{
    static const s16 graves[][2] = {
        { -458, -336 }, { -348, 59 }, { -348, 299 }, { -348, 419 }, { -348, 539 },
        { -338, -336 }, { -218, -336 }, { -98, -336 }, { 22, -336 }, { 155, -177 },
        { 155, -57 }, { 155, 219 }, { 155, 459 }, { 155, 579 }, { 155, 699 },
    };
    if (play->sceneId == 0x53 && play->roomCtx.curRoom.num == 1)
        for (u32 i = 0; i < ARRAY_COUNT(graves); ++i)
            if (x == graves[i][0] && z == graves[i][1])
                return burialSpawn(6 + i, ctx, play, id, x, y, z, rx, ry, rz, params);
    return Actor_Spawn(ctx, play, id, x, y, z, rx, ry, rz, params);
}

PATCH_CALL(0x8099e318, coffinFirst);
PATCH_CALL(0x8099e3cc, coffinSecond);
PATCH_CALL(0x8095b7a8, gravePoe);

typedef struct
{
    u16 id;
    u16 params;
    u16 operation;
    u16 enabled;
} ForestBowChoice;

static ForestBowChoice sChoices[3] __attribute__((section(".data")));
static EnemyFamily sFamilies[3];
static Actor* sPlatform;
static ActorFunc sPlatformFini;
EXPORT_SYMBOL(ENEMY_FOREST_BOW_CHOICES, sChoices);

static void forestBowComplete(EnemyFamily* family, PlayState* play)
{
    if (sPlatform && comboEnemyFamilyDone(family) && sPlatform->home.rot.z > 0)
        --sPlatform->home.rot.z;
}

static void forestBowFini(Actor* actor, PlayState* play)
{
    ActorFunc fini = sPlatformFini;
    sPlatform = NULL;
    sPlatformFini = NULL;
    for (u32 i = 0; i < ARRAY_COUNT(sFamilies); ++i)
        comboEnemyFamilyClear(&sFamilies[i]);
    if (fini)
        fini(actor, play);
}

static Actor* forestBowSpawn(ActorContext* ctx, Actor* parent, PlayState* play, s16 id,
                            f32 x, f32 y, f32 z, s16 rx, s16 ry, s16 rz, s32 params)
{
    u32 slot = params == 1 ? 0 : x < 100.f ? 1 : 2;
    const ForestBowChoice* choice = &sChoices[slot];
    if (play->sceneId != 3 || parent->room != 6 || !choice->enabled)
        return Actor_SpawnAsChild(ctx, parent, play, id, x, y, z, rx, ry, rz, params);
    if (sPlatform && sPlatform != parent)
        return parent;
    if (!sPlatform)
    {
        sPlatform = parent;
        sPlatformFini = parent->fini;
        parent->fini = forestBowFini;
    }

    EnemyFamily* family = &sFamilies[slot];
    ActorEntry entry = { choice->id, { x, y, z }, { rx, ry, rz }, choice->params };
    u32 segment = gSegments[6];
    u8 lackSoul = g.roomEnemyLackSoul;
    family->complete = forestBowComplete;
    Actor* actor = comboEnemyFamilySpawn(family, ctx, &entry, play, choice->operation);
    gSegments[6] = segment;
    g.roomEnemyLackSoul |= lackSoul;
    if (actor)
        actor->room = parent->room;
    return actor ? actor : parent;
}

PATCH_CALL(0x8092731c, forestBowSpawn);
PATCH_CALL(0x809275c4, forestBowSpawn);
PATCH_CALL(0x80927624, forestBowSpawn);

typedef struct {
    u16 id, params, operation, weight;
} FieldChoice;

typedef struct {
    u32 vrom;
    u16 count, weight;
} FieldPool;

static EnemyFamily sFieldFamilies[2];
static FieldPool sFieldStalchildChoices[2] __attribute__((section(".data")));
static Actor* sFieldSpawner;
static ActorFunc sFieldSpawnerFini;
EXPORT_SYMBOL(ENEMY_FIELD_STALCHILD_CHOICES, sFieldStalchildChoices);

static void fieldComplete(EnemyFamily* family, PlayState* play)
{
    if (sFieldSpawner)
    {
        EnemyNativeEncounterSpawner* spawner = (EnemyNativeEncounterSpawner*)sFieldSpawner;
        if (spawner->curNumSpawn > 0)
            --spawner->curNumSpawn;
    }
    memset(family, 0, sizeof(*family));
}

static void fieldPreservedComplete(EnemyFamily* family, PlayState* play)
{
    /* The native child already decrements its parent's live count. */
    memset(family, 0, sizeof(*family));
}

static void detachSpawnerChildren(Actor* parent, PlayState* play)
{
    /* Native children may be destroyed after their spawner's allocation is
     * freed. Clear those links before the deferred family cleanup. */
    for (u32 category = 0; category < ARRAY_COUNT(play->actorCtx.actors); ++category)
        for (Actor* child = play->actorCtx.actors[category].first; child; child = child->next)
            if (child->parent == parent)
                child->parent = NULL;
    parent->child = NULL;
}

static void fieldSpawnerFini(Actor* actor, PlayState* play)
{
    ActorFunc fini = sFieldSpawnerFini;
    detachSpawnerChildren(actor, play);
    sFieldSpawner = NULL;
    sFieldSpawnerFini = NULL;
    for (u32 i = 0; i < ARRAY_COUNT(sFieldFamilies); ++i)
        comboEnemyFamilyClear(&sFieldFamilies[i]);
    if (fini)
        fini(actor, play);
}

static int fieldChoice(u32 slot, FieldChoice* out)
{
    const FieldPool* pool = &sFieldStalchildChoices[slot];
    if (!pool->vrom || !pool->count || !pool->weight)
        return 0;
    u32 roll = MIN((u32)(Rand_ZeroOne() * pool->weight), pool->weight - 1);
    for (u32 i = 0; i < pool->count; i += 8)
    {
        FieldChoice choices[8];
        u32 count = MIN(8, pool->count - i);
        u32 size = count * sizeof(*choices);
        if (comboDmaLoadFilePartial(choices, pool->vrom, i * sizeof(*choices), size) != size)
            return 0;
        for (u32 j = 0; j < count; ++j)
            if (roll < choices[j].weight)
            {
                *out = choices[j];
                return 1;
            }
    }
    return 0;
}

static Actor* fieldStalchildSpawn(ActorContext* ctx, Actor* parent, PlayState* play, s16 id,
                                f32 x, f32 y, f32 z, s16 rx, s16 ry, s16 rz, s32 params)
{
    int enabled = 0;
    for (u32 i = 0; i < ARRAY_COUNT(sFieldStalchildChoices); ++i)
        enabled |= sFieldStalchildChoices[i].vrom != 0;
    if (play->sceneId != 0x51 || id != STALCHILD_ID || !enabled)
        return Actor_SpawnAsChild(ctx, parent, play, id, x, y, z, rx, ry, rz, params);
    if (sFieldSpawner && sFieldSpawner != parent)
        return NULL;
    if (!sFieldSpawner)
    {
        sFieldSpawner = parent;
        sFieldSpawnerFini = parent->fini;
        parent->fini = fieldSpawnerFini;
    }
    for (u32 i = 0; i < ARRAY_COUNT(sFieldStalchildChoices); ++i)
    {
        if (sFieldFamilies[i].live)
            continue;
        int preserved = !sFieldStalchildChoices[i].vrom;
        FieldChoice choice = { id, params, 0, 1 };
        if (!preserved && !fieldChoice(i, &choice))
            return NULL;
        ActorEntry entry = { choice.id, { x, y, z }, { rx, ry, rz }, choice.params };
        u8 lackSoul = g.roomEnemyLackSoul;
        u32 segment = gSegments[6];
        int soul = comboIsForeignEnemyActor(choice.id) ? comboEnemyForeignSoulCheck(choice.id) :
            comboEnemyNativeSoulCheck(play, choice.id, choice.params);
        sFieldFamilies[i].flags = preserved ? 0 : ENEMY_FAMILY_FIELD_STALCHILD;
        sFieldFamilies[i].complete = preserved ? fieldPreservedComplete : fieldComplete;
        Actor* actor = preserved
            ? comboEnemyFamilySpawnVanillaChild(&sFieldFamilies[i], ctx, parent, play, id, x, y, z, rx, ry, rz, params)
            : comboEnemyFamilySpawn(&sFieldFamilies[i], ctx, &entry, play, choice.operation);
        g.roomEnemyLackSoul = lackSoul;
        gSegments[6] = segment;
        if (!actor && !soul)
            sFieldFamilies[i].live = 1;
        if (!actor || (!actor->init && !actor->update))
        {
            if (sFieldFamilies[i].live)
                return parent;
            ((EnemyNativeEncounterSpawner*)parent)->fieldSpawnTimer = 30;
            return NULL;
        }
        actor->room = parent->room;
        return actor;
    }
    return NULL;
}

PATCH_CALL(0x8096fe90, fieldStalchildSpawn);

typedef struct
{
    u16 id, params, operation, enabled;
} DesertLeeverChoice;

static DesertLeeverChoice sDesertChoices[8] __attribute__((section(".data")));
EXPORT_SYMBOL(ENEMY_DESERT_LEEVER_CHOICES, sDesertChoices);

typedef struct
{
    EnemyFamily family;
    Actor* spawner;
    Actor* spawned;
} DesertLeeverChild;

static DesertLeeverChild sDesertChildren[8];

static void desertComplete(EnemyFamily* family, PlayState* play)
{
    DesertLeeverChild* child = (DesertLeeverChild*)family;
    for (Actor* parent = play->actorCtx.actors[ACTORCAT_PROP].first; parent; parent = parent->next)
        if (parent == child->spawner && parent->id == ACTOR_EN_ENCOUNT1 && parent->update)
        {
            EnemyNativeEncounterSpawner* spawner = (EnemyNativeEncounterSpawner*)parent;
            if (spawner->curNumSpawn > 0)
                --spawner->curNumSpawn;
            if (spawner->bigLeever == child->spawned)
                spawner->bigLeever = NULL;
            break;
        }
    memset(child, 0, sizeof(*child));
}

static void desertPreservedComplete(EnemyFamily* family, PlayState* play)
{
    /* Preserve the native Leever's counter and large-Leever bookkeeping. */
    memset((DesertLeeverChild*)family, 0, sizeof(DesertLeeverChild));
}

static void desertSpawnerFini(Actor* actor, PlayState* play)
{
    detachSpawnerChildren(actor, play);
    for (u32 i = 0; i < ARRAY_COUNT(sDesertChildren); ++i)
        if (sDesertChildren[i].spawner == actor)
        {
            comboEnemyFamilyClear(&sDesertChildren[i].family);
            sDesertChildren[i].spawner = NULL;
        }
}

static Actor* desertLeeverSpawn(ActorContext* ctx, Actor* parent, PlayState* play, s16 id,
                                f32 x, f32 y, f32 z, s16 rx, s16 ry, s16 rz, s32 params)
{
    EnemyNativeEncounterSpawner* spawner = (EnemyNativeEncounterSpawner*)parent;
    int first = play->sceneId == 0x5c ? 0 : 5;
    int count = first == 0 ? 5 : 3;
    int enabled = 0;
    for (int i = first; i < first + count; ++i)
        enabled |= sDesertChoices[i].enabled;
    if ((play->sceneId == 0x5c || play->sceneId == 0x5e) && id == ACTOR_EN_REEBA &&
        enabled)
    {
        for (int i = first; i < first + count; ++i)
        {
            if (sDesertChildren[i].spawner)
                continue;
            const DesertLeeverChoice original = { id, params, 0, 0 };
            int preserved = !sDesertChoices[i].enabled;
            const DesertLeeverChoice* choice = preserved ? &original : &sDesertChoices[i];
            ActorEntry entry = { .id = choice->id, .params = choice->params, .pos = { x, y, z }, .rot = { rx, ry, rz } };
            u8 lackSoul = g.roomEnemyLackSoul;
            u32 segment = gSegments[6];
            int missingSoul = (comboIsForeignEnemyActor(choice->id) ? comboEnemyForeignSoulCheck(choice->id) :
                comboEnemyNativeSoulCheck(play, choice->id, choice->params)) <= 0;
            sDesertChildren[i].family.flags = preserved ? 0 : ENEMY_FAMILY_DESERT_LEEVER;
            sDesertChildren[i].family.complete = preserved ? desertPreservedComplete : desertComplete;
            Actor* actor = preserved
                ? comboEnemyFamilySpawnVanillaChild(&sDesertChildren[i].family, ctx, parent, play, id, x, y, z, rx, ry, rz, params)
                : comboEnemyFamilySpawn(&sDesertChildren[i].family, ctx, &entry, play, choice->operation);
            g.roomEnemyLackSoul = lackSoul;
            gSegments[6] = segment;
            if (!actor && missingSoul)
            {
                sDesertChildren[i].spawner = parent;
                parent->fini = desertSpawnerFini;
                return parent;
            }
            if (!actor || (!actor->init && !actor->update))
            {
                spawner->timer = 30;
                return NULL;
            }
            actor->room = parent->room;
            if (preserved)
                ((EnemyNativeLeeverAim*)actor)->aimType = spawner->leeverIndex;
            sDesertChildren[i].spawner = parent;
            sDesertChildren[i].spawned = actor;
            parent->fini = desertSpawnerFini;
            return actor;
        }
        return NULL;
    }
    Actor* actor = Actor_SpawnAsChild(ctx, parent, play, id, x, y, z, rx, ry, rz, params);
    if (actor)
        ((EnemyNativeLeeverAim*)actor)->aimType = spawner->leeverIndex;
    return actor;
}

PATCH_CALL(0x8096f830, desertLeeverSpawn);
#else

typedef struct
{
    EnemyFamily family;
    Actor* parent;
} KeetaGuard;

static KeetaGuard sGuards[16];
static u16 sGuardTarget[4] __attribute__((section(".data")));
EXPORT_SYMBOL(ENEMY_MM_KEETA_GUARDS_V1, sGuardTarget);

void comboEnemyKeetaGuardsReset(void)
{
    for (u32 i = 0; i < ARRAY_COUNT(sGuards); ++i)
        comboEnemyFamilyClear(&sGuards[i].family);
    memset(sGuards, 0, sizeof(sGuards));
}

static void guardComplete(EnemyFamily* family, PlayState* play)
{
    KeetaGuard* guard = (KeetaGuard*)family;
    for (Actor* parent = play->actorCtx.actors[ACTORCAT_PROP].first; parent; parent = parent->next)
    {
        if (parent != guard->parent || parent->id != ACTOR_EN_ENCOUNT4 || !parent->update)
            continue;
        s16* live = (s16*)((u8*)parent + 0x14c);
        s16* defeated = (s16*)((u8*)parent + 0x14e);
        if (*live > 0)
            --*live;
        if (comboEnemyFamilyDone(family))
            ++*defeated;
        else
            *live = *defeated = 0;
        break;
    }
    memset(guard, 0, sizeof(*guard));
}

static Actor* keetaGuardSpawn(ActorContext* ctx, Actor* parent, PlayState* play, s16 id,
                              f32 x, f32 y, f32 z, s16 rx, s16 ry, s16 rz, s32 params)
{
    if (!sGuardTarget[3] || play->sceneId != 0x43 || !parent || parent->id != ACTOR_EN_ENCOUNT4 || id != ACTOR_EN_SKB)
        return Actor_SpawnAsChild(ctx, parent, play, id, x, y, z, rx, ry, rz, params);
    for (u32 i = 0; i < ARRAY_COUNT(sGuards); ++i)
    {
        KeetaGuard* guard = &sGuards[i];
        if (guard->parent)
            continue;
        ActorEntry entry = { .id = sGuardTarget[0], .params = sGuardTarget[1],
            .pos = { x, y, z }, .rot = { 7, ((u16)(s16)(ry * (360.f / 65536.f)) << 7) | 0x7f, 0x7f } };
        guard->family.complete = guardComplete;
        Actor* actor = comboEnemyFamilySpawn(&guard->family, ctx, &entry, play, sGuardTarget[2]);
        if (!actor || (!actor->init && !actor->update))
            return NULL;
        guard->parent = parent;
        return actor;
    }
    return NULL;
}

PATCH_CALL(0x809c454c, keetaGuardSpawn);

typedef struct {
    u16 scene, params, id, targetParams, air, operation;
} LilyChoice;
_Static_assert(sizeof(LilyChoice) == 12, "Lily source metadata ABI");
typedef struct LilySource {
    struct LilySource* next;
    Actor* platform;
    ActorFunc init, update, fini;
    EnemyFamily family;
    LilyChoice choice;
    u8 defeated, retry;
} LilySource;
_Static_assert(sizeof(LilySource) + 16 <= 128, "Planner lily monitor allowance");
static LilySource* sSources;
static LilySource* sPrepared;
static int sSpawning;
static u32 sChoicesVrom __attribute__((section(".data"))) = 0;
EXPORT_SYMBOL(ENEMY_LILY_CHOICES_MM_V1, sChoicesVrom);
int comboEnemyLilySpawning(void)
{
    return sSpawning;
}

static int lilyChoice(PlayState* play, u16 params, LilyChoice* choice)
{
    DmaEntry file;
    u32 header[2];
    if ((params >> 12) != 0 || !sChoicesVrom || !comboDmaLookup(&file, sChoicesVrom) ||
        file.vstart != sChoicesVrom || file.pend || file.vend - file.vstart < 8 ||
        comboDmaLoadFilePartial(header, sChoicesVrom, 0, 8) != 8 || header[0] != 0x454c5331 ||
        header[1] > (file.vend - file.vstart - 8) / sizeof(*choice))
        return 0;
    u32 low = 0, high = header[1];
    while (low < high)
    {
        u32 mid = low + (high - low) / 2;
        if (comboDmaLoadFilePartial(choice, sChoicesVrom, 8 + mid * sizeof(*choice), sizeof(*choice)) != sizeof(*choice))
            return 0;
        if (choice->scene == play->sceneId && choice->params == params)
            return 1;
        if (choice->scene < play->sceneId || (choice->scene == play->sceneId && choice->params < params))
            low = mid + 1;
        else
            high = mid;
    }
    return 0;
}

u16 comboEnemyLilyPrepare(PlayState* play, u16 params)
{
    LilyChoice choice;
    if (!lilyChoice(play, params, &choice))
        return params;
    LilySource* source = malloc(sizeof(*source));
    if (!source)
        return params;
    memset(source, 0, sizeof(*source));
    source->choice = choice;
    sPrepared = source;
    return params | 0x1000;
}

static LilySource* lilyOwner(Actor* actor)
{
    for (LilySource* s = sSources; s; s = s->next)
        if (s->platform == actor)
            return s;
    return NULL;
}

static void lilyComplete(EnemyFamily* family, PlayState* play)
{
    for (LilySource* source = sSources; source; source = source->next)
        if (&source->family == family && comboEnemyFamilyDone(family))
        {
            source->defeated = 1;
            if ((source->choice.params & 0x7f) != 0x7f)
                Flags_SetSwitch(play, source->choice.params & 0x7f);
            return;
        }
}

static void lilySourceFini(Actor* actor, PlayState* play)
{
    LilySource* s = lilyOwner(actor);
    if (!s)
        return;
    comboEnemyFamilyClear(&s->family);
    if (!s->init && s->fini)
        s->fini(actor, play);
    LilySource** link = &sSources;
    while (*link && *link != s)
        link = &(*link)->next;
    if (*link)
        *link = s->next;
    free(s);
}

static void lilySourceUpdate(Actor* actor, PlayState* play)
{
    LilySource* s = lilyOwner(actor);
    if (!s)
    {
        Actor_Kill(actor);
        return;
    }
    if (s->update)
        s->update(actor, play);
    if (s->defeated || s->family.started || actor->room != play->roomCtx.curRoom.num)
        return;

    if ((gMmSave.info.weekEventReg[12] & 1) || ((s->choice.params & 0x7f) != 0x7f && Flags_GetSwitch(play, s->choice.params & 0x7f)))
        return;
    if (s->retry)
    {
        --s->retry;
        return;
    }
    Vec3f pos = actor->world.pos;
    CollisionPoly* floor = NULL;
    s32 bgId;
    pos.y += 80.f;
    f32 y = BgCheck_EntityRaycastFloor5_2(play, &play->colCtx, &floor, &bgId, NULL, &pos);
    if (!floor || y < actor->world.pos.y - 20.f)
    {
        s->retry = 15;
        return;
    }
    pos.y = y + (s->choice.air ? 40.f : 1.f);
    ActorEntry entry = { .id = s->choice.id, .pos = { pos.x, pos.y, pos.z }, .rot = { 7, 0x7f, 0x7f }, .params = s->choice.targetParams };
    int previous = sSpawning;
    sSpawning = 1;
    s->family.flags = ENEMY_FAMILY_NO_CLEAR;
    s->family.complete = lilyComplete;
    Actor* target = comboEnemyFamilySpawn(&s->family, &play->actorCtx, &entry, play, s->choice.operation);
    sSpawning = previous;
    if (!target || (!target->init && !target->update))
    {
        s->retry = 30;
        return;
    }
    target->room = actor->room;
    Actor_ChangeCategory(play, &play->actorCtx, target, ACTORCAT_NPC);

}

static void lilySourceInit(Actor* actor, PlayState* play)
{
    LilySource* s = lilyOwner(actor);
    if (!s)
    {
        Actor_Kill(actor);
        return;
    }
    actor->init = NULL;
    actor->update = s->update;
    actor->fini = s->fini;
    if (s->init)
        s->init(actor, play);
    s->init = NULL;
    s->update = actor->update;
    s->fini = actor->fini;
    if (actor->update)
        actor->update = lilySourceUpdate;
    actor->fini = lilySourceFini;
}

void comboEnemyLilyWrap(Actor* actor, PlayState* play, u16 params)
{
    LilySource* s = sPrepared;
    sPrepared = NULL;
    if (!s)
        return;
    if (!actor || (!actor->init && !actor->update))
    {
        free(s);
        return;
    }
    s->platform = actor;
    s->init = actor->init;
    s->update = actor->update;
    s->fini = actor->fini;
    s->next = sSources;
    sSources = s;
    actor->flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
    if (actor->init)
        actor->init = lilySourceInit;
    if (actor->update)
        actor->update = lilySourceUpdate;
    actor->fini = lilySourceFini;
}
#endif
