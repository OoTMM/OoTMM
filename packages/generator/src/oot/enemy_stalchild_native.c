#include <combo.h>
#include <combo/enemy_native.h>
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
    Actor* actor;
    Actor* spawner;
    ActorFunc fini;
} FieldChild;
static FieldChild sFieldChildren[4];
static u32 sFieldStalchildChoices[4] __attribute__((section(".data")));
EXPORT_SYMBOL(ENEMY_FIELD_STALCHILD_CHOICES, sFieldStalchildChoices);

static void fieldChildFini(Actor* actor, PlayState* play)
{
    for (u32 i = 0; i < ARRAY_COUNT(sFieldChildren); ++i)
    {
        FieldChild child = sFieldChildren[i];
        if (child.actor != actor)
            continue;
        memset(&sFieldChildren[i], 0, sizeof(child));
        for (Actor* parent = play->actorCtx.actors[ACTORCAT_PROP].first; parent; parent = parent->next)
        {
            if (parent == child.spawner && parent->id == ACTOR_EN_ENCOUNT1 && parent->update)
            {
                s16* current = &((EnemyNativeEncounterSpawner*)parent)->curNumSpawn;
                if (*current > 0)
                    --*current;
                break;
            }
        }
        if (child.fini)
            child.fini(actor, play);
        break;
    }
}

static Actor* fieldStalchildSpawn(ActorContext* ctx, Actor* parent, PlayState* play, s16 id,
                                  f32 x, f32 y, f32 z, s16 rx, s16 ry, s16 rz, s32 params)
{
    u32 count = sFieldStalchildChoices[0];
    if (play->sceneId == 0x51 && id == STALCHILD_ID && count && count < ARRAY_COUNT(sFieldStalchildChoices))
    {
        for (u32 i = 0; i < ARRAY_COUNT(sFieldChildren); ++i)
        {
            if (sFieldChildren[i].actor)
                continue;
            u32 selected = MIN((u32)(Rand_ZeroOne() * count), count - 1);
            u32 choice = sFieldStalchildChoices[selected + 1];
            u8 lackSoul = g.roomEnemyLackSoul;
            Actor* actor = Actor_Spawn(ctx, play, choice >> 16, x, y, z, rx, ry, rz, (u16)choice);
            if (!actor && lackSoul == g.roomEnemyLackSoul)
                break;
            if (!actor || (!actor->init && !actor->update))
                return NULL;
            sFieldChildren[i] = (FieldChild){ actor, parent, actor->fini };
            actor->fini = fieldChildFini;
            return actor;
        }
    }
    return Actor_SpawnAsChild(ctx, parent, play, id, x, y, z, rx, ry, rz, params);
}

PATCH_CALL(0x8096fe90, fieldStalchildSpawn);
