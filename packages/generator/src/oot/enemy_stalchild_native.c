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
