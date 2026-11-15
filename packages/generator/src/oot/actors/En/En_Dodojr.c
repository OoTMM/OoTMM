#include <combo.h>
#include <combo/enemy_baby_dodongo.h>

static void EnDodojr_MoveTerrain(Actor* actor)
{
    ActorFunc action = *(ActorFunc*)((char*)actor + 0x180);
    ActorFunc wait = actorAddr(ACTOR_EN_DODOJR, 0x808bb5e0);
    enemyBabyDodongoTerrain(gPlay, actor,
                            action == wait || action == actorAddr(ACTOR_EN_DODOJR, 0x808bb6f8) ||
                                action == actorAddr(ACTOR_EN_DODOJR, 0x808bbc3c),
                            action == actorAddr(ACTOR_EN_DODOJR, 0x808bb7c0));
    if (action == wait && actor->floorPoly)
        actor->world.pos.y = actor->floorHeight;
    Actor_MoveXZGravity(actor);
}

PATCH_CALL(0x808bbef0, EnDodojr_MoveTerrain);
