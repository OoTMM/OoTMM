#include <combo.h>
#include <combo/actor_ovl.h>
#include <combo/enemy_native.h>

#if defined(GAME_OOT)
#define FROZEN_RD_ID 0x90
#define FROZEN_ICE_ID 0x11e
#define FROZEN_RD_INIT 0x80939a9c
#define FROZEN_RD_FINI 0x80939c88
#define FROZEN_ICE_INIT 0x80a787d0
#define FROZEN_ICE_UPDATE 0x80a78ec8
#define FROZEN_ICE_PARAMS 1
extern void EnRd_DanceUpdate(Actor*, PlayState*);
#define FROZEN_RD_UPDATE EnRd_DanceUpdate
typedef struct {
    Actor actor;
    ActorFunc action;
    u8 alpha, flag;
    s16 meltTimer;
    ColliderCylinder ice, hard;
} FrozenIce;
ASSERT_SIZE(FrozenIce, 0x1dc);
#else
#define FROZEN_RD_ID 0x4c
#define FROZEN_ICE_ID 0x8e
#define FROZEN_RD_INIT 0x808d3e20
#define FROZEN_RD_FINI 0x808d414c
#define FROZEN_ICE_PARAMS 0xff4b
#define FROZEN_RD_UPDATE ((ActorFunc)actorAddr(FROZEN_RD_ID, 0x808d6c10))
#endif

#if defined(GAME_OOT)

static int sFrozenIceUpdate;
#endif

static int frozenIceOwned(Actor* ice)
{
    return ice->parent && ice->parent->id == FROZEN_RD_ID && (ice->parent->params & 0xff) == 4;
}

static void frozenIceConfigure(Actor* actor)
{
#if defined(GAME_OOT)
    if (frozenIceOwned(actor) && actor->update)
    {
        FrozenIce* ice = (FrozenIce*)actor;
        Actor_SetScale(actor, .75f);
        ice->ice.base.acFlags |= AC_TYPE_OTHER;
        ice->ice.dim.radius = ice->hard.dim.radius = 37;
        ice->ice.dim.height = ice->hard.dim.height = 90;
        Actor_SetFocus(actor, 22.5f);
    }
#else
    if (frozenIceOwned(actor))
        actor->csId = -1;
#endif
}

static Actor* frozenRedeadIce(Actor* actor, PlayState* play)
{
    Actor* ice = Actor_SpawnAsChild(&play->actorCtx, actor, play, FROZEN_ICE_ID,
                                    actor->world.pos.x, actor->world.pos.y, actor->world.pos.z,
                                    actor->world.rot.x, actor->world.rot.y, actor->world.rot.z, FROZEN_ICE_PARAMS);
    if (ice)
    {
        actor->freezeTimer = 40;
        if (!ice->init)
            frozenIceConfigure(ice);
    }
    return ice;
}

static void frozenRedeadWait(Actor* actor, PlayState* play)
{
    actor->speed = 0;
    if (frozenRedeadIce(actor, play))
        actor->update = FROZEN_RD_UPDATE;
}

static void frozenRedeadInit(Actor* actor, PlayState* play)
{
    int requested = (u16)actor->params == 0x7f04;
    ((ActorFunc)actorAddr(FROZEN_RD_ID, FROZEN_RD_INIT))(actor, play);
    if (!requested || !actor->update)
        return;
#if defined(GAME_MM)

    if (actor->child && actor->child->id == FROZEN_ICE_ID)
    {
        actor->freezeTimer = 40;
        if (!actor->child->init)
            frozenIceConfigure(actor->child);
        return;
    }
#endif
    if (!frozenRedeadIce(actor, play))
        actor->update = frozenRedeadWait;
}

static void frozenRedeadDestroy(Actor* actor, PlayState* play)
{
    if ((actor->params & 0xff) == 4)
    {
        for (u32 category = 0; category < 12; category++)
        {
            for (Actor* child = play->actorCtx.actors[category].first; child; child = child->next)
            {
                if (child->id == FROZEN_ICE_ID && child->parent == actor)
                {
                    child->parent = NULL;
                    Actor_Kill(child);
                }
            }
        }
    }
    ((ActorFunc)actorAddr(FROZEN_RD_ID, FROZEN_RD_FINI))(actor, play);
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
            actor->scale.y = .75f * (.5f + ((FrozenIce*)actor)->meltTimer * .0125f);
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
#else
PATCH_VROM_U32(0xd512a0, frozenRedeadInit);
PATCH_VROM_U32(0xd516a0, 0);
PATCH_VROM_U32(0xd512a4, frozenRedeadDestroy);
PATCH_VROM_U32(0xd516a4, 0);
#endif
EXPORT_SYMBOL(ENEMY_NATIVE_FROZEN_REDEAD_V1, frozenRedeadInit);
