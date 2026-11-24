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

static u32 nativePeahatNight(PlayState* play)
{
#if defined(GAME_OOT)
    u16 scene = play->sceneId;
    if (scene <= 0x0f || (scene >= 0x11 && scene <= 0x19) ||
        (scene >= 0x70 && scene <= 0x79) || scene == 0x7b || scene == 0x7d)
        return 0;
#else
    (void)play;
#endif
    return gSave.isNight;
}

static void peahatLimitCeiling(Actor* actor, PlayState* play, Vec3f from)
{
    Vec3f to = actor->world.pos;
    Vec3f start = from;
    Vec3f end = to;
    Vec3f hit;
    CollisionPoly* poly;
    s32 bgId;
    const f32 clearance = 90.f;
    int movedXZ = from.x != to.x || from.z != to.z;

    if (movedXZ)
    {
        start.y += clearance;
        end.y += clearance;
        if (BgCheck_EntityLineTest1(&play->colCtx, &start, &end, &hit, &poly,
                                    1, 0, 1, 0, &bgId))
        {
            actor->world.pos.x = from.x;
            actor->world.pos.z = from.z;
            if (actor->world.pos.y > from.y)
                actor->world.pos.y = from.y;
            if (actor->velocity.y > 0.f)
                actor->velocity.y = 0.f;
        }
    }

    if (actor->world.pos.y >= from.y)
    {
        start = actor->world.pos;
        start.y = from.y;
        end = actor->world.pos;
        end.y += clearance;
        if (BgCheck_EntityLineTest1(&play->colCtx, &start, &end, &hit, &poly,
                                    0, 0, 1, 0, &bgId))
        {
            f32 limit = hit.y - clearance;
            if (limit < from.y)
                limit = from.y;
            if (actor->world.pos.y > limit)
                actor->world.pos.y = limit;
            if (actor->velocity.y > 0.f)
                actor->velocity.y = 0.f;
        }
    }
}

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
    gSave.isNight = nativePeahatNight(play);
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
        if (flying && !nativePeahatNight(play))
        {
            f32 target = GET_PLAYER(play)->actor.world.pos.y + 55.f;
            if (actor->floorPoly && target < actor->floorHeight + 30.f)
                target = actor->floorHeight + 30.f;
            comboEnemyClampWall(play, actor, from);
            Math_ApproachF(&actor->world.pos.y, target, 0.2f, 5.f);
            Actor_UpdateBgCheckInfo(play, actor, 25.f, 30.f, 30.f,
                                    UPDBGCHECKINFO_FLAG_WALL | UPDBGCHECKINFO_FLAG_CEILING);
        }
        peahatLimitCeiling(actor, play, from);
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
    gSave.isNight = nativePeahatNight(play);
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
    gSave.isNight = nativePeahatNight(play);
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
