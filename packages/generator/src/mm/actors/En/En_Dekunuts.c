#include <combo.h>

static void EnDekunuts_LeaveAfterHit(Actor* actor, PlayState* play)
{
    SkelAnime* skel = (SkelAnime*)((u8*)actor + 0x144);
    if (SkelAnime_Update(skel))
        Actor_Kill(actor);
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
    *(ActorFunc*)((u8*)actor + 0x188) = EnDekunuts_LeaveAfterHit;
    actor->flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
}

static void EnDekunuts_ShuffleAction(Actor* actor, PlayState* play)
{
    ActorFunc action = *(ActorFunc*)((u8*)actor + 0x188);

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
