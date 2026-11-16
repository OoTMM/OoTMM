#include <combo.h>
#include <combo/sfx.h>

typedef struct
{
    Actor actor;
    SkelAnime skelAnime;
    ActorFunc actionFunc;
    u8 unk_18c, unk_18d, drawDmgEffType;
    s16 timer;
} ShuffleDekunuts;

_Static_assert(__builtin_offsetof(ShuffleDekunuts, actionFunc) == 0x188, "Deku Scrub action");
_Static_assert(__builtin_offsetof(ShuffleDekunuts, timer) == 0x190, "Deku Scrub timer");

static void EnDekunuts_SinkAfterLeave(Actor* actor, PlayState* play)
{
    ShuffleDekunuts* scrub = (ShuffleDekunuts*)actor;
    scrub->timer = MIN(scrub->timer + 1, 20);
    actor->shape.yOffset -= scrub->timer / actor->scale.y;
    actor->shape.rot.y += 0x2000;
    if (actor->shape.yOffset * actor->scale.y < -400.f)
        Actor_Kill(actor);
}

static void EnDekunuts_LeaveAfterHit(Actor* actor, PlayState* play)
{
    ShuffleDekunuts* scrub = (ShuffleDekunuts*)actor;
    if (SkelAnime_Update(&scrub->skelAnime))
    {
        Actor_PlaySfx(actor, NA_SE_EN_AKINDONUTS_HIDE);
        actor->shape.shadowAlpha = 0;
        scrub->timer = 0;
        scrub->actionFunc = EnDekunuts_SinkAfterLeave;
        actor->update = EnDekunuts_SinkAfterLeave;
    }
}

static void EnDekunuts_ShuffleHit(Actor* actor, PlayState* play)
{
    ColliderCylinder* collider = (ColliderCylinder*)((u8*)actor + 0x27c);
    u8* overlay = ((ActorOvl*)actor->ovl)->data;
    if (actor->params != 3)
    {
        ((ActorFunc)(overlay + (0x808be73c - 0x808bd1e0)))(actor, play);
        return;
    }
    if (!(collider->base.acFlags & AC_HIT))
        return;
    collider->base.acFlags &= ~AC_HIT;
    ((void (*)(Actor*))(overlay + (0x808bdcf0 - 0x808bd1e0)))(actor);
    ((ShuffleDekunuts*)actor)->actionFunc = EnDekunuts_LeaveAfterHit;
    actor->update = EnDekunuts_LeaveAfterHit;
    collider->base.ocFlags1 &= ~OC1_ON;
    actor->flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    actor->flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED | ACTOR_FLAG_DRAW_CULLING_DISABLED;
}

static void EnDekunuts_ShuffleAction(Actor* actor, PlayState* play)
{
    ActorFunc action = ((ShuffleDekunuts*)actor)->actionFunc;

    if (actor->params != 3)
    {
        action(actor, play);
        return;
    }
    if (actor->floorPoly && (actor->bgCheckFlags & BGCHECKFLAG_GROUND) &&
        fabsf(actor->floorHeight) < 30000.f &&
        fabsf(actor->world.pos.y - actor->floorHeight) <= 1.f)
        actor->home.pos.y = actor->floorHeight;
    actor->params = 0;
    action(actor, play);
    actor->params = 3;
}

PATCH_CALL(0x808bea64, EnDekunuts_ShuffleHit);
PATCH_CALL(0x808bea78, EnDekunuts_ShuffleAction);
PATCH_VROM_U32(0xd39380, 0);
