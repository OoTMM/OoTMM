#include <combo.h>
#include <combo/enemy_native.h>
#if defined(GAME_OOT)
# define NativeBabaSpawnEntry ((Actor* (*)(ActorContext*, ActorEntry*, PlayState*))0x800255c4)
#else
# define NativeBabaSpawnEntry ((Actor* (*)(ActorContext*, ActorEntry*, PlayState*))0x800bb2d0)

static ActorProfile* nativeMiniBabaProfile(Actor* actor)
{
    if (actor->params != 2)
        return NULL;
    return comboEnemyNativeProfile(actor, 0x66, 0x31);
}

static void nativeMiniBabaUpdate(Actor* actor, PlayState* play)
{
    ActorProfile* profile = nativeMiniBabaProfile(actor);
    ActorFunc update = profile ? profile->update : NULL;
    ActorFunc before;
    if (!update || update == nativeMiniBabaUpdate)
    {
        Actor_Kill(actor);
        return;
    }
    before = *(ActorFunc*)((u8*)actor + 0x188);
    actor->update = update;
    update(actor, play);
    if (actor->update && actor->category == ACTORCAT_ENEMY &&
        before == (ActorFunc)actorAddr(0x66, 0x808f200c) &&
        *(ActorFunc*)((u8*)actor + 0x188) == (ActorFunc)actorAddr(0x66, 0x808f254c))
        Actor_ChangeCategory(play, &play->actorCtx, actor, ACTORCAT_MISC);
    if (actor->update == update)
        actor->update = nativeMiniBabaUpdate;
}

static void nativeMiniBabaInit(Actor* actor, PlayState* play)
{
    ActorProfile* profile = nativeMiniBabaProfile(actor);
    ActorFunc init = profile ? profile->init : NULL;
    if (!init || init == nativeMiniBabaInit)
    {
        actor->init = NULL;
        Actor_Kill(actor);
        return;
    }
    actor->init = init;
    init(actor, play);
    actor->init = NULL;
    if (actor->update == profile->update)
        actor->update = nativeMiniBabaUpdate;
}
#endif

Actor* comboEnemyNativeRenewableBabaSpawn(ActorContext* ctx, ActorEntry* entry, PlayState* play)
{
    if (!ctx || !entry || !play)
        return NULL;
#if defined(GAME_OOT)
    if (entry->id != 0xc7 || entry->params != 1)
        return NULL;
    return NativeBabaSpawnEntry(ctx, entry, play);
#else
    Actor* actor;
    if (((u16)entry->id & 0x1fff) != 0x66 || (entry->params != 1 && entry->params != 2))
        return NULL;
    actor = NativeBabaSpawnEntry(ctx, entry, play);
    if (actor && entry->params == 2)
    {
        if (actor->init)
            actor->init = nativeMiniBabaInit;
        else if (actor->update)
        {
            ActorProfile* profile = nativeMiniBabaProfile(actor);
            if (!profile || actor->update != profile->update)
                Actor_Kill(actor);
            else
                actor->update = nativeMiniBabaUpdate;
        }
    }
    return actor;
#endif
}
EXPORT_SYMBOL(ENEMY_NATIVE_RENEWABLE_BABA_V1, comboEnemyNativeRenewableBabaSpawn);
