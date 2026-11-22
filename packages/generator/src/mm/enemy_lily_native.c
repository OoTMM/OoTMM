#include <combo.h>
#include <combo/enemy_native.h>
#include <combo/global.h>

#define LilyWaterSurface ((s32 (*)(PlayState*, CollisionContext*, f32, f32, f32*, WaterBox**))0x800ca1ac)

typedef struct {
    DynaPolyActor dyna;
    u8 skeleton[0x230 - sizeof(DynaPolyActor)];
    ActorFunc actionFunc;
    u8 animation[0x39c - 0x234];
    s16 grabTarget;
    u8 counters[0x3c6 - 0x39e];
    s16 action;
} NativeLily;

_Static_assert(__builtin_offsetof(NativeLily, actionFunc) == 0x230, "Lily action function");
_Static_assert(__builtin_offsetof(NativeLily, grabTarget) == 0x39c, "Lily grab target");
_Static_assert(__builtin_offsetof(NativeLily, action) == 0x3c6, "Lily action");

static Actor* sSelectedLily;

void comboEnemyNativeLilyCategory(PlayState* play, ActorContext* ctx, Actor* actor, u8 category)
{
    if (actor != sSelectedLily)
        ((void (*)(PlayState*, ActorContext*, Actor*, u8))0x800bc154)(play, ctx, actor, category);
}

void comboEnemyNativeLilyThrow(PlayState* play, Actor* actor, f32 speed, s16 yaw, f32 yVelocity, u32 damage)
{
    if (actor == sSelectedLily)
        speed *= 2.f;
    ((void (*)(PlayState*, Actor*, f32, s16, f32, u32))0x800b8d50)(play, actor, speed, yaw, yVelocity, damage);
}

static u8 lilyBegin(void)
{
    u8 previous = gMmSave.info.weekEventReg[12] & 1;
    gMmSave.info.weekEventReg[12] &= ~1;
    return previous;
}

static void lilyEnd(u8 previous)
{
    gMmSave.info.weekEventReg[12] = (gMmSave.info.weekEventReg[12] & ~1) | previous;
}

static void lilyUpdate(Actor* actor, PlayState* play)
{
    NativeLily* lily = (NativeLily*)actor;
    ActorFunc update = actorAddr(0x13a, 0x80a181b4);
    ActorFunc dormant = actorAddr(0x13a, 0x80a180b4);
    Actor* previousActor = sSelectedLily;
    int detonated = lily->action == 4 && lily->actionFunc == dormant &&
                    (lily->grabTarget == 1 || lily->grabTarget == 2);
    u8 previousFlag = lilyBegin();
    sSelectedLily = actor;
    update(actor, play);
    sSelectedLily = previousActor;
    lilyEnd(previousFlag);
    if (detonated && lily->action == 7 && actor->update)
    {
        actor->colChkInfo.health = 0;
        Actor_Kill(actor);
    }
}

static void lilyDestroy(Actor* actor, PlayState* play)
{
    Player* player = GET_PLAYER(play);
    ActorFunc destroy = actorAddr(0x13a, 0x80a17018);
    if (player && player->actor.parent == actor)
    {
        player->actor.parent = NULL;
        player->actor.freezeTimer = 0;
        player->av2.actionVar2 = 1000;
    }
    destroy(actor, play);
}

static void lilyActivate(Actor* actor, PlayState* play)
{
    if (!actor->update)
        return;
    if (((NativeLily*)actor)->dyna.bgId == BGCHECK_SCENE)
    {
        g.roomEnemyLackSoul = 1;
        Actor_Kill(actor);
        return;
    }
    Actor_ChangeCategory(play, &play->actorCtx, actor, ACTORCAT_ENEMY);
    actor->flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
    actor->update = lilyUpdate;
    actor->fini = lilyDestroy;
}

static void lilyInit(Actor* actor, PlayState* play)
{
    ActorFunc init = actorAddr(0x13a, 0x80a16d90);
    u8 previous = lilyBegin();
    actor->init = NULL;
    init(actor, play);
    lilyEnd(previous);
    lilyActivate(actor, play);
}

Actor* comboEnemyNativeLilySpawn(ActorContext* ctx, ActorEntry* entry, PlayState* play)
{
    Actor* actor;
    u8 previous;
    if (!ctx || !entry || !play || ((u16)entry->id & 0x1fff) != 0x13a || (u16)entry->params != 0x0fff)
        return NULL;
    ActorEntry copy = *entry;
    Vec3f probe = { copy.pos.x, copy.pos.y + 80.f, copy.pos.z };
    CollisionPoly* floor = NULL;
    WaterBox* water;
    s32 bgId;
    f32 surface;
    f32 y = BgCheck_EntityRaycastFloor5_2(play, &play->colCtx, &floor, &bgId, NULL, &probe);
    if (LilyWaterSurface(play, &play->colCtx, probe.x, probe.z, &surface, &water) && surface > y)
        y = surface;
    if (y > -30000.f)
        copy.pos.y = y;
    entry = &copy;
    previous = lilyBegin();
    actor = ((Actor * (*)(ActorContext*, ActorEntry*, PlayState*))0x800bb2d0)(ctx, entry, play);
    lilyEnd(previous);
    if (actor && actor->init)
        actor->init = lilyInit;
    else if (actor)
        lilyActivate(actor, play);
    return actor;
}

EXPORT_SYMBOL(ENEMY_NATIVE_LILY_V1, comboEnemyNativeLilySpawn);
EXPORT_SYMBOL(ENEMY_NATIVE_LILY_CATEGORY_V1, comboEnemyNativeLilyCategory);

PATCH_CALL(0x80a17c10, comboEnemyNativeLilyCategory);
PATCH_CALL(0x80a17ca4, comboEnemyNativeLilyCategory);

EXPORT_SYMBOL(ENEMY_NATIVE_LILY_THROW_V1, comboEnemyNativeLilyThrow);
PATCH_CALL(0x80a1794c, comboEnemyNativeLilyThrow);
