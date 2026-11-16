#include <combo.h>
#include <combo/sfx.h>

typedef struct
{
    Actor actor;
    SkelAnime skelAnime;
    ActorFunc actionFunc;
    s16 timer;
    Vec3s jointTable[18], morphTable[18];
    ColliderCylinder collider;
} ShuffleShopnuts;

_Static_assert(__builtin_offsetof(ShuffleShopnuts, actionFunc) == 0x180, "Shopnuts action");
_Static_assert(__builtin_offsetof(ShuffleShopnuts, collider) == 0x260, "Shopnuts collider");

static void EnShopnuts_SinkAfterLeave(Actor* actor, PlayState* play)
{
    ShuffleShopnuts* scrub = (ShuffleShopnuts*)actor;
    scrub->timer = MIN(scrub->timer + 1, 20);
    actor->shape.yOffset -= scrub->timer / actor->scale.y;
    actor->shape.rot.y += 0x2000;
    if (actor->shape.yOffset * actor->scale.y < -400.f)
        Actor_Kill(actor);
}

static void EnShopnuts_LeaveAfterHit(Actor* actor, PlayState* play)
{
    ShuffleShopnuts* scrub = (ShuffleShopnuts*)actor;
    if (SkelAnime_Update(&scrub->skelAnime))
    {
        Actor_PlaySfx(actor, NA_SE_EN_AKINDONUTS_HIDE);
        actor->shape.shadowAlpha = 0;
        scrub->timer = 0;
        scrub->actionFunc = EnShopnuts_SinkAfterLeave;
        actor->update = EnShopnuts_SinkAfterLeave;
    }
}

static void EnShopnuts_SpawnSalesman(Actor* actor, PlayState* play)
{
    ShuffleShopnuts* scrub = (ShuffleShopnuts*)actor;
    if (!SkelAnime_Update(&scrub->skelAnime))
    {
        Math_ApproachS(&actor->shape.rot.y, actor->yawTowardsPlayer, 2, 0xe38);
        return;
    }
    if ((u16)actor->params == 0x4000)
    {
        Animation_MorphToPlayOnce(&scrub->skelAnime, (AnimationHeader*)0x06004404, -3.f);
        scrub->actionFunc = EnShopnuts_LeaveAfterHit;
        actor->update = EnShopnuts_LeaveAfterHit;
        scrub->collider.base.acFlags &= ~(AC_ON | AC_HIT);
        scrub->collider.base.ocFlags1 &= ~OC1_ON;
        actor->flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
        actor->flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED | ACTOR_FLAG_DRAW_CULLING_DISABLED;
        return;
    }
    Actor_Spawn(&play->actorCtx, play, ACTOR_EN_DNS, actor->world.pos.x, actor->world.pos.y,
                actor->world.pos.z, actor->shape.rot.x, actor->shape.rot.y, actor->shape.rot.z, actor->params);
    Actor_Kill(actor);
}

PATCH_FUNC(0x80b40984, EnShopnuts_SpawnSalesman);
