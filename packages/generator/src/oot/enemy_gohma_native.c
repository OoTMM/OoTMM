#include <combo.h>
#include <combo/actor_ovl.h>

typedef struct {
    Actor actor;
    SkelAnime skeleton;
    Vec3s joints[24], morph[24];
    ActorFunc action;
    s16 pitch, roll, type, eyePitch, eyeYaw, hatch, eggTimer, hurtTimer;
    s16 visual, detection, spawnNum, invincibility, timer;
} HatchedGohma;
ASSERT_OFFSET(HatchedGohma, type, 0x2a8);

static void hatchedPose(Actor* actor)
{
    HatchedGohma* larva = (HatchedGohma*)actor;
    if (!actor->update)
        return;
    larva->type = 0;
    larva->invincibility = 10;
    actor->gravity = -1.3f;
    ((void (*)(Actor*))OverlayAddr(0x808b4f7c))(actor);
}

static void hatchedInit(Actor* actor, PlayState* play)
{
    actor->init = NULL;
    ((ActorFunc)OverlayAddr(0x808b3fc0))(actor, play);
    hatchedPose(actor);
}

void comboEnemyGohmaHatchedWrap(Actor* actor, PlayState* play)
{
    (void)play;
    if (!actor)
        return;
    if (actor->init)
        actor->init = hatchedInit;
    else
        hatchedPose(actor);
}
