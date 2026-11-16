#include <combo.h>
#include <combo/actor_ovl.h>
#include <combo/enemy_native.h>
#include <combo/enemy_actor_layout.h>

#if defined(GAME_OOT)
#define FROZEN_RD_ID       0x90
#define FROZEN_ICE_ID      0x11e
#define FROZEN_RD_INIT     0x80939a9c
#define FROZEN_RD_FINI     0x80939c88
#define FROZEN_ICE_INIT    0x80a787d0
#define FROZEN_ICE_UPDATE  0x80a78ec8
#define FROZEN_ICE_PARAMS  1
#define FROZEN_WALL_PARAMS 0x7f80
#define FROZEN_WALL_INIT   0x808718d0
#define FROZEN_WALL_FINI   0x80871a10
#define FROZEN_WALL_UPDATE 0x80872b14
#define FROZEN_WALL_RISE   0x80871cf8
extern void EnRd_DanceUpdate(Actor*, PlayState*);
#define FROZEN_RD_UPDATE EnRd_DanceUpdate

typedef struct
{
    Actor actor;
    ActorFunc action;
    u8 alpha, flag;
    s16 meltTimer;
    ColliderCylinder ice, hard;
} FrozenIce;

ASSERT_SIZE(FrozenIce, 0x1dc);
#else
#define FROZEN_RD_ID       0x4c
#define FROZEN_ICE_ID      0x8e
#define FROZEN_RD_INIT     0x808d3e20
#define FROZEN_RD_FINI     0x808d414c
#define FROZEN_ICE_PARAMS  0xff4b
#define FROZEN_WALL_PARAMS 0x7f81
#define FROZEN_WALL_INIT   0x80874810
#define FROZEN_WALL_FINI   0x80874a28
#define FROZEN_WALL_UPDATE 0x80875cf4
#define FROZEN_WALL_RISE   0x80875248
#define FROZEN_RD_UPDATE   ((ActorFunc)actorAddr(FROZEN_RD_ID, 0x808d6c10))
#endif

int gEnemyFrozenWallmasterIceSpawn;

#if defined(GAME_OOT)

static int sFrozenIceUpdate;
#endif

static int frozenIceOwned(Actor* ice)
{
    return ice->parent && ((ice->parent->id == FROZEN_RD_ID && (ice->parent->params & 0xff) == 4) ||
                           comboEnemyFrozenWallmaster(ice->parent));
}

static void frozenIceConfigure(Actor* actor)
{
#if defined(GAME_OOT)
    if (frozenIceOwned(actor) && actor->update)
    {
        FrozenIce* ice = (FrozenIce*)actor;
        f32 scale = comboEnemyFrozenWallmaster(actor->parent) ? .8f : .75f;
        Actor_SetScale(actor, scale);
        ice->ice.base.acFlags |= AC_TYPE_OTHER;
        ice->ice.dim.radius = ice->hard.dim.radius = 50.f * scale;
        ice->ice.dim.height = ice->hard.dim.height = 120.f * scale;
        Actor_SetFocus(actor, 30.f * scale);
    }
#else
    if (frozenIceOwned(actor))
        actor->csId = -1;
#endif
}

static int frozenEnemyRed(Actor* actor)
{
    return actor->id == FROZEN_RD_ID ? actor->params == 0x104 :
                                       ((EnemyNativeWallmaster*)actor)->switchFlag == 0x17e;
}

static Actor* frozenEnemyIce(Actor* actor, PlayState* play)
{
    int wallmaster = comboEnemyFrozenWallmaster(actor);
    int red = frozenEnemyRed(actor);
    s16 params = FROZEN_ICE_PARAMS;
#if defined(GAME_MM)
    if (wallmaster)
        params = 0xff50;
#endif
    int previous = gEnemyFrozenWallmasterIceSpawn;
    gEnemyFrozenWallmasterIceSpawn = wallmaster;
    if (red)
        params = 0x8040;
    Actor* ice = Actor_SpawnAsChild(&play->actorCtx, actor, play, red ? ACTOR_BG_ICE_SHELTER : FROZEN_ICE_ID,
                                    actor->world.pos.x, actor->world.pos.y - (wallmaster && !red ? 15.f : 0.f), actor->world.pos.z,
                                    actor->world.rot.x, actor->world.rot.y + (wallmaster ? 0x5900 : 0), actor->world.rot.z, params);
    gEnemyFrozenWallmasterIceSpawn = previous;
    if (ice)
    {
        actor->freezeTimer = 40;
        if (red && actor->id == FROZEN_RD_ID)
            actor->params = 4;
        if (!red && !ice->init)
            frozenIceConfigure(ice);
    }
    return ice;
}

static void frozenRedeadWait(Actor* actor, PlayState* play)
{
    actor->speed = 0;
    if (frozenEnemyIce(actor, play))
        actor->update = FROZEN_RD_UPDATE;
}

static void frozenRedeadInit(Actor* actor, PlayState* play)
{
    int red = (u16)actor->params == 0x7e04;
    int requested = (u16)actor->params == 0x7f04 || red;
    if (red)
        actor->params = 0x7f01;
    ((ActorFunc)actorAddr(FROZEN_RD_ID, FROZEN_RD_INIT))(actor, play);
    if (!requested || !actor->update)
        return;
    if (red)
        actor->params = 0x104;
#if defined(GAME_MM)

    if (actor->child && actor->child->id == FROZEN_ICE_ID)
    {
        actor->freezeTimer = 40;
        if (!actor->child->init)
            frozenIceConfigure(actor->child);
        return;
    }
#endif
    if (!frozenEnemyIce(actor, play))
        actor->update = frozenRedeadWait;
}

static void frozenEnemyRemoveIce(Actor* actor, PlayState* play)
{
    for (u32 category = 0; category < 12; category++)
    {
        for (Actor* child = play->actorCtx.actors[category].first; child; child = child->next)
        {
            if ((child->id == FROZEN_ICE_ID || child->id == ACTOR_BG_ICE_SHELTER) && child->parent == actor)
            {
                child->parent = NULL;
                if (actor->child == child)
                    actor->child = NULL;
                Actor_Kill(child);
            }
        }
    }
}

static void frozenRedeadDestroy(Actor* actor, PlayState* play)
{
    if ((actor->params & 0xff) == 4)
        frozenEnemyRemoveIce(actor, play);
    ((ActorFunc)actorAddr(FROZEN_RD_ID, FROZEN_RD_FINI))(actor, play);
}

static void frozenWallmasterDestroy(Actor* actor, PlayState* play)
{
    frozenEnemyRemoveIce(actor, play);
    ((ActorFunc)actorAddr(ACTOR_EN_WALLMAS, FROZEN_WALL_FINI))(actor, play);
}

static void frozenWallmasterWait(Actor* actor, PlayState* play)
{
    if (frozenEnemyIce(actor, play))
        actor->update = (ActorFunc)actorAddr(ACTOR_EN_WALLMAS, FROZEN_WALL_UPDATE);
}

static void frozenWallmasterInit(Actor* actor, PlayState* play)
{
    int red = (u16)actor->params == (FROZEN_WALL_PARAMS ^ 0x100);
    int requested = (u16)actor->params == FROZEN_WALL_PARAMS || red;
    Vec3f position = actor->world.pos;
    ActorFunc draw = actor->draw;
    f32 floorHeight = actor->floorHeight;
    int manualIce = red;
#if defined(GAME_OOT)
    manualIce = requested;
#endif
    if (manualIce)
        actor->params &= ~0x80;
    int previous = gEnemyFrozenWallmasterIceSpawn;
    gEnemyFrozenWallmasterIceSpawn = requested;
    ((ActorFunc)actorAddr(ACTOR_EN_WALLMAS, FROZEN_WALL_INIT))(actor, play);
    gEnemyFrozenWallmasterIceSpawn = previous;
    if (!requested || !actor->update)
        return;
    ((EnemyNativeWallmaster*)actor)->switchFlag = red ? 0x17e : 0x17f;
    if (manualIce)
    {
        actor->world.pos = position;
        actor->draw = draw;
        actor->floorHeight = floorHeight;
        actor->flags |= ACTOR_FLAG_ATTENTION_ENABLED;
        ((void (*)(Actor*))actorAddr(ACTOR_EN_WALLMAS, FROZEN_WALL_RISE))(actor);
    }
    actor->fini = frozenWallmasterDestroy;
    if (actor->child && actor->child->id == FROZEN_ICE_ID && actor->child->update)
    {
        actor->freezeTimer = 40;
        if (!actor->child->init)
            frozenIceConfigure(actor->child);
    }
    else if (!frozenEnemyIce(actor, play))
        actor->update = frozenWallmasterWait;
}
#if defined(GAME_OOT)

static void frozenIceInit(Actor* actor, PlayState* play)
{
    ((ActorFunc)actorAddr(FROZEN_ICE_ID, FROZEN_ICE_INIT))(actor, play);
    frozenIceConfigure(actor);
}

static void frozenIceUpdate(Actor* actor, PlayState* play)
{
    int previous = sFrozenIceUpdate;
    sFrozenIceUpdate = frozenIceOwned(actor);
#if defined(GAME_OOT)
    if (sFrozenIceUpdate)
    {
        FrozenIce* ice = (FrozenIce*)actor;
        Actor* water = ice->ice.base.ac;
        if ((ice->ice.base.acFlags & AC_HIT) && water && water->id == ACTOR_CUSTOM_OBJ_AQUA)
        {
            ice->ice.base.acFlags &= ~AC_HIT;
            if (water->params == 1 && ice->action == (ActorFunc)actorAddr(FROZEN_ICE_ID, 0x80a789e0))
            {
                ice->meltTimer = 40;
                ice->action = (ActorFunc)actorAddr(FROZEN_ICE_ID, 0x80a78c10);
                Actor_PlaySfx(actor, NA_SE_EV_ICE_MELT);
            }
        }
    }
#endif

    ((ActorFunc)actorAddr(FROZEN_ICE_ID, FROZEN_ICE_UPDATE))(actor, play);
#if defined(GAME_OOT)
    if (sFrozenIceUpdate && ((FrozenIce*)actor)->action == (ActorFunc)actorAddr(FROZEN_ICE_ID, 0x80a78c10))
        if (((FrozenIce*)actor)->meltTimer >= 0)
            actor->scale.y = (comboEnemyFrozenWallmaster(actor->parent) ? .8f : .75f) *
                             (.5f + ((FrozenIce*)actor)->meltTimer * .0125f);
#endif
    sFrozenIceUpdate = previous;
}

static s16 frozenIceCutscene(PlayState* play, s16 id, s16 timer, Actor* actor, s16 camera)
{
    if (sFrozenIceUpdate)
        return -1;
    return ((s16(*)(PlayState*, s16, s16, Actor*, s16))0x8006b6fc)(play, id, timer, actor, camera);
}

PATCH_CALL(0x80a78a30, frozenIceCutscene);
PATCH_VROM_U32(0xcd9670, frozenRedeadInit);
PATCH_VROM_U32(0xcd99d0, 0);
PATCH_VROM_U32(0xcd9674, frozenRedeadDestroy);
PATCH_VROM_U32(0xcd99d4, 0);
PATCH_VROM_U32(0xdfb990, frozenIceInit);
PATCH_VROM_U32(0xdfbaac, 0);
PATCH_VROM_U32(0xdfb998, frozenIceUpdate);
PATCH_VROM_U32(0xdfbab4, 0);
PATCH_VROM_U32(0xc10940, frozenWallmasterInit);
PATCH_VROM_U32(0xc10b90, 0);
#else
PATCH_VROM_U32(0xd512a0, frozenRedeadInit);
PATCH_VROM_U32(0xd516a0, 0);
PATCH_VROM_U32(0xd512a4, frozenRedeadDestroy);
PATCH_VROM_U32(0xd516a4, 0);
PATCH_VROM_U32(0xcf0570, frozenWallmasterInit);
PATCH_VROM_U32(0xcf0874, 0);
#endif
EXPORT_SYMBOL(ENEMY_NATIVE_FROZEN_REDEAD_V1, frozenRedeadInit);
