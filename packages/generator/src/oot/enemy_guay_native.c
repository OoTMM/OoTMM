#include <combo.h>
#include <combo/enemy_native.h>
#include <combo/actor_ovl.h>
#include <combo/enemy_actor_layout.h>

#ifndef NativeGuaySpawnEntry
# define NativeGuaySpawnEntry ((Actor* (*)(ActorContext*, ActorEntry*, PlayState*))0x800255c4)
#endif
#define GUAY_ID 0x1c0
#define GUAY_INIT 0x80b72bd0
#define GUAY_UPDATE 0x80b73bd0
#define GUAY_RESPAWN 0x80b73a54
#define GUAY_COUNTER 0x80b74088

static void guayUpdate(Actor* actor, PlayState* play)
{
    u32* counter = actorAddr(GUAY_ID, GUAY_COUNTER);
    u32 saved = *counter;
    ((ActorFunc)actorAddr(GUAY_ID, GUAY_UPDATE))(actor, play);
    *counter = saved;
    if (actor->update && !actor->colChkInfo.health &&
        ((EnemyNativeGuay*)actor)->action == (ActorFunc)actorAddr(GUAY_ID, GUAY_RESPAWN))
        Actor_Kill(actor);
}

static void guayInit(Actor* actor, PlayState* play)
{
    u32* counter = actorAddr(GUAY_ID, GUAY_COUNTER);
    u32 saved = *counter;
    ((ActorFunc)actorAddr(GUAY_ID, GUAY_INIT))(actor, play);
    *counter = saved;
    comboEnemyNativeGuaySize(actor);
    if (actor->update)
        actor->update = guayUpdate;
    else
        g.roomEnemyLackSoul = 1;
}

Actor* comboEnemyNativeGuaySpawn(ActorContext* ctx, ActorEntry* entry, PlayState* play)
{
    if (!ctx || !entry || !play || entry->id != GUAY_ID || (entry->params != 0 && entry->params != 1))
        return NULL;
    void* overlay = gActorOvl[GUAY_ID].data;
    u32 saved = overlay ? *(u32*)actorAddr(GUAY_ID, GUAY_COUNTER) : 0;
    Actor* actor = NativeGuaySpawnEntry(ctx, entry, play);
    if (overlay && gActorOvl[GUAY_ID].data == overlay)
        *(u32*)actorAddr(GUAY_ID, GUAY_COUNTER) = saved;
    if (!actor || (!actor->init && !actor->update))
        return NULL;
    if (actor->init)
        actor->init = guayInit;
    else if (actor->update)
    {
        comboEnemyNativeGuaySize(actor);
        actor->update = guayUpdate;
    }
    return actor;
}

EXPORT_SYMBOL(ENEMY_NATIVE_GUAY_V1, comboEnemyNativeGuaySpawn);
