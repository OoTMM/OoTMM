#include <combo.h>
#include <combo/enemy_native.h>
#if defined(GAME_OOT)
#define LEEVER_ID              0x1c
#define LEEVER_LARGE           1
#define NativeLeeverSpawnEntry ((Actor * (*)(ActorContext*, ActorEntry*, PlayState*))0x800255c4)

static Actor* sLeeverActor;

s32 comboEnemyNativeLeeverFloor(CollisionContext* ctx, CollisionPoly* poly, s32 bgId)
{
    if (sLeeverActor && (!sLeeverActor->floorPoly || sLeeverActor->floorPoly == poly) && poly)
        return 4;
    return SurfaceType_GetFloorType(ctx, poly, bgId);
}

EXPORT_SYMBOL(ENEMY_NATIVE_LEEVER_FLOOR_V1, comboEnemyNativeLeeverFloor);
#else
#define LEEVER_ID              0x216
#define LEEVER_LARGE           0x8000
#define NativeLeeverSpawnEntry ((Actor * (*)(ActorContext*, ActorEntry*, PlayState*))0x800bb2d0)
#endif

#if defined(GAME_OOT)

static void leeverUpdate(Actor* actor, PlayState* play)
{
    ActorProfile* profile = comboEnemyNativeProfile(actor, LEEVER_ID, -1);
    Actor* previous = sLeeverActor;
    sLeeverActor = actor;
    if (profile && profile->update)
        profile->update(actor, play);
    sLeeverActor = previous;

    /* EnReeba_Die only kills actors with an En_Encount1 parent. Let its
     * shrinking/death effect and item drop finish, then retire a standalone
     * replacement before it can repeat the drop on the next frame. */
    if (actor->update && !actor->parent && !actor->colChkInfo.health && actor->scale.x < 0.01f)
        Actor_Kill(actor);
}
#else
static void leeverDraw(Actor* actor, PlayState* play)
{
    ActorProfile* profile = comboEnemyNativeProfile(actor, LEEVER_ID, -1);
    Gfx* first = play->state.gfxCtx->polyOpa.append;
    if (!profile || !profile->draw)
        return;
    profile->draw(actor, play);
    for (Gfx* gfx = first; gfx < play->state.gfxCtx->polyOpa.append; ++gfx)
    {
        if ((gfx->words.w0 >> 24) == G_SETPRIMCOLOR)
        {
            gfx->words.w1 = 0x9b37ffff;
            break;
        }
    }
}

static void leeverUpdate(Actor* actor, PlayState* play)
{
    ActorProfile* profile = comboEnemyNativeProfile(actor, LEEVER_ID, -1);
    if (profile && profile->update)
        profile->update(actor, play);
    if (actor->draw)
        actor->draw = leeverDraw;
}
#endif

static void leeverActivate(Actor* actor, PlayState* play)
{
    if (!actor || !actor->update)
        return;
    actor->flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
#if defined(GAME_OOT)
    Actor_ChangeCategory(play, &play->actorCtx, actor, ACTORCAT_ENEMY);
    actor->update = leeverUpdate;
#else
    void (*rise)(Actor*) = actorAddr(LEEVER_ID, 0x80b7ce34);
    if (rise)
        rise(actor);
    if ((u16)actor->params & LEEVER_LARGE)
    {
        actor->update = leeverUpdate;
        if (actor->draw)
            actor->draw = leeverDraw;
    }
#endif
}

static void leeverInit(Actor* actor, PlayState* play)
{
    ActorProfile* profile = comboEnemyNativeProfile(actor, LEEVER_ID, -1);
    ActorFunc init = profile ? profile->init : NULL;
#if defined(GAME_OOT)
    Actor* previous = sLeeverActor;
    sLeeverActor = actor;
#endif
    actor->init = NULL;
    if (init && init != leeverInit)
        init(actor, play);
#if defined(GAME_OOT)
    sLeeverActor = previous;
#endif
    leeverActivate(actor, play);
}

Actor* comboEnemyNativeLeeverSpawn(ActorContext* ctx, ActorEntry* entry, PlayState* play)
{
    Actor* actor;
    if (!ctx || !entry || !play || ((u16)entry->id & 0x1fff) != LEEVER_ID || (entry->params != 0 && (u16)entry->params != LEEVER_LARGE))
        return NULL;
#if defined(GAME_OOT)
    Actor sentinel;
    Actor* previous = sLeeverActor;
    memset(&sentinel, 0, sizeof(sentinel));
    sLeeverActor = &sentinel;
#endif
    actor = NativeLeeverSpawnEntry(ctx, entry, play);
#if defined(GAME_OOT)
    sLeeverActor = previous;
#endif
    if (actor && actor->init)
        actor->init = leeverInit;
    else
        leeverActivate(actor, play);
    return actor;
}

EXPORT_SYMBOL(ENEMY_NATIVE_LEEVER_V1, comboEnemyNativeLeeverSpawn);
