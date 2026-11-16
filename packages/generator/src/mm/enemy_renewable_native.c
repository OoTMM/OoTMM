#include <combo.h>
#include <combo/actor_ovl.h>
#include <combo/enemy_native.h>
#include <combo/enemy_actor_layout.h>

typedef struct
{
    Actor actor;
    SkelAnime skelAnime;
    ActorFunc actionFunc;
} NativeGuay;

typedef struct
{
    Actor actor;
    ActorFunc actionFunc;
} NativeChuchu;

typedef struct
{
    Actor actor;
    SkelAnime skelAnime;
    Vec3s jointTable[16], morphTable[16];
    ActorFunc actionFunc;
} NativeBubble;

_Static_assert(__builtin_offsetof(NativeGuay, actionFunc) == 0x188, "MM Guay action");
_Static_assert(__builtin_offsetof(NativeChuchu, actionFunc) == 0x144, "MM ChuChu action");
_Static_assert(__builtin_offsetof(NativeBubble, actionFunc) == 0x248, "MM Blue Bubble action");

static void renewableUpdate(Actor* actor, PlayState* play)
{
    if (actor->id == ACTOR_EN_CROW)
    {
        s32* counter = actorAddr(ACTOR_EN_CROW, 0x8099c0cc);
        s32 saved = *counter;
        *counter = 0;
        ((ActorFunc)actorAddr(ACTOR_EN_CROW, 0x8099bb84))(actor, play);
        *counter = saved;
        if (actor->update && !actor->colChkInfo.health &&
            ((NativeGuay*)actor)->actionFunc == (ActorFunc)actorAddr(ACTOR_EN_CROW, 0x8099b9e8))
            Actor_Kill(actor);
    }
    else if (actor->id == ACTOR_EN_BB)
    {
        ((ActorFunc)actorAddr(ACTOR_EN_BB, 0x808c30a0))(actor, play);
        if (actor->update && !actor->colChkInfo.health &&
            ((NativeBubble*)actor)->actionFunc == (ActorFunc)actorAddr(ACTOR_EN_BB, 0x808c2cb4))
            Actor_Kill(actor);
    }
    else
    {
        ((ActorFunc)actorAddr(ACTOR_EN_SLIME, 0x80a311e8))(actor, play);
        if (actor->update && !actor->colChkInfo.health &&
            ((NativeChuchu*)actor)->actionFunc == (ActorFunc)actorAddr(ACTOR_EN_SLIME, 0x80a30c2c))
            Actor_Kill(actor);
    }
}

static void renewableInit(Actor* actor, PlayState* play)
{
    if (actor->id == ACTOR_EN_CROW)
    {
        s32* counter = actorAddr(ACTOR_EN_CROW, 0x8099c0cc);
        s32 saved = *counter;
        ((ActorFunc)actorAddr(ACTOR_EN_CROW, 0x8099ab30))(actor, play);
        *counter = saved;
    }
    else if (actor->id == ACTOR_EN_BB)
    {
        ((ActorFunc)actorAddr(ACTOR_EN_BB, 0x808c1d40))(actor, play);
    }
    else
    {
        ((ActorFunc)actorAddr(ACTOR_EN_SLIME, 0x80a2eda0))(actor, play);
    }
    if (actor->id == ACTOR_EN_CROW)
        comboEnemyNativeGuaySize(actor);
    if (actor->update)
        actor->update = renewableUpdate;
}

Actor* comboEnemyNativeRenewableSpawn(ActorContext* ctx, ActorEntry* entry, PlayState* play)
{
    if (!ctx || !entry || !play)
        return NULL;
    s16 id = (u16)entry->id & 0x1fff;
    if (id != ACTOR_EN_CROW && id != ACTOR_EN_SLIME && id != ACTOR_EN_BB)
        return NULL;
    if ((id == ACTOR_EN_CROW && entry->params != 0 && entry->params != 1) ||
        (id == ACTOR_EN_SLIME && ((u16)entry->params & 0xff) > 3))
        return NULL;
    void* overlay = gActorOvl[id].data;
    s32 saved = id == ACTOR_EN_CROW && overlay ? *(s32*)actorAddr(id, 0x8099c0cc) : 0;
    Actor* actor = ((Actor * (*)(ActorContext*, ActorEntry*, PlayState*))0x800bb2d0)(ctx, entry, play);
    if (id == ACTOR_EN_CROW && overlay && gActorOvl[id].data == overlay)
        *(s32*)actorAddr(id, 0x8099c0cc) = saved;
    if (actor && actor->init)
        actor->init = renewableInit;
    else if (actor && actor->update)
    {
        if (id == ACTOR_EN_CROW)
            comboEnemyNativeGuaySize(actor);
        actor->update = renewableUpdate;
    }
    return actor;
}

EXPORT_SYMBOL(ENEMY_NATIVE_RENEWABLE_V1, comboEnemyNativeRenewableSpawn);
