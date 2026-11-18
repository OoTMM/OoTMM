#include <combo.h>
#include <combo/enemy_native.h>
#if defined(GAME_OOT)
# define NativeBabaSpawnEntry ((Actor* (*)(ActorContext*, ActorEntry*, PlayState*))0x800255c4)
# define BABA_ID 0xc7
# define BABA_OBJECT 0x39
# define BABA_ACTION_OFFSET 0x180
# define BABA_DEAD 0x809bafa8
#else
# define NativeBabaSpawnEntry ((Actor* (*)(ActorContext*, ActorEntry*, PlayState*))0x800bb2d0)
# define BABA_ID 0x66
# define BABA_OBJECT 0x31
# define BABA_ACTION_OFFSET 0x188
# define BABA_DEAD 0x808f254c
#endif

static void nativeBabaUpdate(Actor* actor, PlayState* play)
{
    ActorProfile* profile = comboEnemyNativeProfile(actor, BABA_ID, BABA_OBJECT);
    ActorFunc update = profile ? profile->update : NULL;
    if (!update || update == nativeBabaUpdate)
    {
        Actor_Kill(actor);
        return;
    }
    actor->update = update;
    update(actor, play);
    if (actor->update && *(ActorFunc*)((u8*)actor + BABA_ACTION_OFFSET) == (ActorFunc)actorAddr(BABA_ID, BABA_DEAD))
        Actor_Kill(actor);
    if (actor->update == update)
        actor->update = nativeBabaUpdate;
}

static void nativeBabaInit(Actor* actor, PlayState* play)
{
    ActorProfile* profile = comboEnemyNativeProfile(actor, BABA_ID, BABA_OBJECT);
    ActorFunc init = profile ? profile->init : NULL;
    if (!init || init == nativeBabaInit)
    {
        actor->init = NULL;
        Actor_Kill(actor);
        return;
    }
    actor->init = init;
    init(actor, play);
    actor->init = NULL;
    if (actor->update == profile->update)
        actor->update = nativeBabaUpdate;
}

Actor* comboEnemyNativeRenewableBabaSpawn(ActorContext* ctx, ActorEntry* entry, PlayState* play)
{
    Actor* actor;
    if (!ctx || !entry || !play)
        return NULL;
#if defined(GAME_OOT)
    if (entry->id != 0xc7 || entry->params != 1)
        return NULL;
#else
    if (((u16)entry->id & 0x1fff) != 0x66 || (entry->params != 1 && entry->params != 2))
        return NULL;
#endif
    actor = NativeBabaSpawnEntry(ctx, entry, play);
    if (actor)
    {
        if (actor->init)
            actor->init = nativeBabaInit;
        else if (actor->update)
        {
            ActorProfile* profile = comboEnemyNativeProfile(actor, BABA_ID, BABA_OBJECT);
            if (!profile || actor->update != profile->update)
                Actor_Kill(actor);
            else
                actor->update = nativeBabaUpdate;
        }
    }
    return actor;
}
EXPORT_SYMBOL(ENEMY_NATIVE_RENEWABLE_BABA_V1, comboEnemyNativeRenewableBabaSpawn);
