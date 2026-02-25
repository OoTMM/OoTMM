#ifndef COMBO_OOT_EN_DOOR_H
#define COMBO_OOT_EN_DOOR_H

#include <combo/actor.h>

typedef struct Actor_EnDoor Actor_EnDoor;

typedef void (*EnDoorActionFunc)(struct Actor_EnDoor*, PlayState*);

struct Actor_EnDoor
{
    Actor actor;
    SkelAnime skelAnime;
    u8 openAnim;
    u8 playerIsOpening;
    u8 unk_192;
    s8 requiredObjectSlot;
    s8 dListIndex;
    s16 lockTimer;
    Vec3s jointTable[5];
    Vec3s morphTable[5];
    EnDoorActionFunc actionFunc;
};

_Static_assert(sizeof(Actor_EnDoor) == 0x01c8, "OoT Actor_EnDoor size is wrong");

#endif
