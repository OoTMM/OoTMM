#include <combo.h>
#include <combo/enemy_native.h>
#include <combo/enemy_actor_layout.h>

extern int comboEnemyClampWall(PlayState*, Actor*, Vec3f);

#if defined(GAME_OOT)
# define NativePeahatSpawnEntry ((Actor* (*)(ActorContext*, ActorEntry*, PlayState*))0x800255c4)
# define PEAHAT_ACTOR 0x1d
# define PEAHAT_OBJECT 0x18
#else
# define NativePeahatSpawnEntry ((Actor* (*)(ActorContext*, ActorEntry*, PlayState*))0x800bb2d0)
# define PEAHAT_ACTOR 0x14
# define PEAHAT_OBJECT 0x14
#endif

static void nativePeahatUpdate(Actor* actor, PlayState* play)
{
    ActorProfile* profile = comboEnemyNativeProfile(actor, PEAHAT_ACTOR, PEAHAT_OBJECT);
    ActorFunc update = profile ? profile->update : NULL;
    u32 night;
    if (!update || update == nativePeahatUpdate)
    {
        Actor_Kill(actor);
        return;
    }
    Vec3f from = actor->world.pos;
    night = gSave.isNight;
    gSave.isNight = 0;
    actor->update = update;
    update(actor, play);
    gSave.isNight = night;
    if (actor->update == update)
    {
        int flying;
#if defined(GAME_OOT)
        s32 state = ((EnemyNativePeahat*)actor)->state;
        flying = state == 5 || state == 12 || state == 14 || state == 15;
#else
        ActorFunc action = *(ActorFunc*)((u8*)actor + 0x2a8);
        flying = action == actorAddr(PEAHAT_ACTOR, 0x80897910) ||
                 action == actorAddr(PEAHAT_ACTOR, 0x80897f44) ||
                 action == actorAddr(PEAHAT_ACTOR, 0x80898144);
#endif
        if (flying)
        {
            f32 target = GET_PLAYER(play)->actor.world.pos.y + 55.f;
            if (actor->floorPoly && target < actor->floorHeight + 30.f)
                target = actor->floorHeight + 30.f;
            comboEnemyClampWall(play, actor, from);
            Math_ApproachF(&actor->world.pos.y, target, 0.2f, 5.f);
            Actor_UpdateBgCheckInfo(play, actor, 25.f, 30.f, 30.f,
                                    UPDBGCHECKINFO_FLAG_WALL | UPDBGCHECKINFO_FLAG_CEILING);
        }
        actor->update = nativePeahatUpdate;
    }
}

static void nativePeahatInit(Actor* actor, PlayState* play)
{
    ActorProfile* profile = comboEnemyNativeProfile(actor, PEAHAT_ACTOR, PEAHAT_OBJECT);
    ActorFunc init = profile ? profile->init : NULL;
    u32 night;
    if (!init || init == nativePeahatInit)
    {
        actor->init = NULL;
        Actor_Kill(actor);
        return;
    }
    night = gSave.isNight;
    gSave.isNight = 0;
    actor->init = init;
    init(actor, play);
    actor->init = NULL;
    gSave.isNight = night;
    if (actor->update == profile->update)
        actor->update = nativePeahatUpdate;
}

Actor* comboEnemyNativePeahatSpawn(ActorContext* ctx, ActorEntry* entry, PlayState* play)
{
    Actor* actor;
    u32 night;
    if (!ctx || !entry || !play)
        return NULL;
#if defined(GAME_OOT)
    if (entry->id != PEAHAT_ACTOR || (entry->params != 0 && (u16)entry->params != 0xffff))
        return NULL;
#else
    if (((u16)entry->id & 0x1fff) != PEAHAT_ACTOR || entry->params != 0)
        return NULL;
#endif
    night = gSave.isNight;
    gSave.isNight = 0;
    actor = NativePeahatSpawnEntry(ctx, entry, play);
    gSave.isNight = night;
    if (actor && actor->init)
        actor->init = nativePeahatInit;
    else if (actor && actor->update)
    {
        ActorProfile* profile = comboEnemyNativeProfile(actor, PEAHAT_ACTOR, PEAHAT_OBJECT);
        if (!profile || actor->update != profile->update)
            Actor_Kill(actor);
        else
            actor->update = nativePeahatUpdate;
    }
    return actor;
}
EXPORT_SYMBOL(ENEMY_NATIVE_PEAHAT_V1, comboEnemyNativePeahatSpawn);
