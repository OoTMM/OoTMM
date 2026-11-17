#include <combo.h>
#include <combo/enemy_native.h>
#include <combo/enemy.h>
#include <combo/dma.h>
#include <combo/actor_ovl.h>
#include <combo/actor_id.h>
#include <combo/rand.h>
#include <combo/enemy_actor_layout.h>

#ifndef NativeStalchildSpawnEntry
# define NativeStalchildSpawnEntry ((Actor* (*)(ActorContext*, ActorEntry*, PlayState*))0x800255c4)
#endif
#define STALCHILD_ID 0x1b0

static ActorProfile* stalchildProfile(Actor* actor)
{
    ActorOvl* ovl = actor->ovl;
    ActorProfile* profile;
    if (actor->id != STALCHILD_ID || !ovl || !ovl->data || ovl->vramEnd < ovl->vramStart ||
        (u32)ovl->dataInit < ovl->vramStart || (u32)ovl->dataInit > ovl->vramEnd ||
        ovl->vramEnd - (u32)ovl->dataInit < sizeof(*profile))
        return NULL;
    profile = (ActorProfile*)((u8*)ovl->data + (u32)ovl->dataInit - ovl->vramStart);
    return profile->id == STALCHILD_ID && profile->objectId == 0x184 ? profile : NULL;
}

static void stalchildUpdate(Actor* actor, PlayState* play)
{
    ActorProfile* profile = stalchildProfile(actor);
    ActorFunc update = profile ? profile->update : NULL;
    Vec3f home = actor->home.pos;
    u32 night = gSave.isNight;
    if (!update || update == stalchildUpdate)
    {
        g.roomEnemyLackSoul = 1;
        Actor_Kill(actor);
        return;
    }
    gSave.isNight = 1;
    actor->home.pos = GET_PLAYER(play)->actor.world.pos;
    actor->update = update;
    update(actor, play);
    actor->home.pos = home;
    gSave.isNight = night;
    if (actor->update == update)
        actor->update = stalchildUpdate;
}

static void stalchildInit(Actor* actor, PlayState* play)
{
    ActorProfile* profile = stalchildProfile(actor);
    if (!profile || !profile->init || profile->init == stalchildInit)
    {
        actor->init = NULL;
        g.roomEnemyLackSoul = 1;
        Actor_Kill(actor);
        return;
    }
    profile->init(actor, play);
    actor->init = NULL;
    if (actor->update == profile->update)
        actor->update = stalchildUpdate;
    else if (!actor->update)
        g.roomEnemyLackSoul = 1;
}

Actor* comboEnemyNativeStalchildSpawn(ActorContext* ctx, ActorEntry* entry, PlayState* play)
{
    Actor* actor;
    ActorProfile* profile;
    if (!ctx || !entry || !play || entry->id != STALCHILD_ID || (entry->params != 0 && entry->params != 5 && entry->params != 10))
        return NULL;
    actor = NativeStalchildSpawnEntry(ctx, entry, play);
    if (!actor || (!actor->init && !actor->update))
        return NULL;
    if (actor->init)
        actor->init = stalchildInit;
    else
    {
        profile = stalchildProfile(actor);
        if (!profile || actor->update != profile->update)
        {
            g.roomEnemyLackSoul = 1;
            Actor_Kill(actor);
            return NULL;
        }
        actor->update = stalchildUpdate;
    }
    return actor;
}
EXPORT_SYMBOL(ENEMY_NATIVE_STALCHILD_V1, comboEnemyNativeStalchildSpawn);

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

static void fieldSpawnerFini(Actor* actor, PlayState* play)
{
    ActorFunc fini = sFieldSpawnerFini;
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
    if (play->sceneId != 0x51 || id != STALCHILD_ID || !sFieldStalchildChoices[0].vrom)
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
        FieldChoice choice;
        if (!fieldChoice(i, &choice))
            return NULL;
        ActorEntry entry = { choice.id, { x, y, z }, { rx, ry, rz }, choice.params };
        u8 lackSoul = g.roomEnemyLackSoul;
        u32 segment = gSegments[6];
        int soul = comboIsForeignEnemyActor(choice.id) ? comboEnemyForeignSoulCheck(choice.id) :
            comboEnemyNativeSoulCheck(play, choice.id, choice.params);
        sFieldFamilies[i].complete = fieldComplete;
        Actor* actor = comboEnemyFamilySpawn(&sFieldFamilies[i], ctx, &entry, play, choice.operation);
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
