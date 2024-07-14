#ifndef OVL_OBJ_KIBAKO_H
#define OVL_OBJ_KIBAKO_H

#include <combo/actor.h>

#define KIBAKO_COLLECTIBLE_ID(thisx) ((thisx)->actor.variable & 0x3F)
#define KIBAKO_COLLECTIBLE_FLAG(thisx) (((thisx)->actor.variable >> 0x8) & 0x7F)
#define KIBAKO_BANK_INDEX(thisx) (((thisx)->actor.variable >> 0xF) & 1)
#define KIBAKO_BOMBER_CAN_HIDE_IN_BOX(thisx) (((thisx)->actor.variable >> 7) & 1)

typedef struct Actor_ObjKibako Actor_ObjKibako;

typedef void (*Actor_ObjKibakoFunc)(Actor_ObjKibako*, GameState_Play*);

struct Actor_ObjKibako
{
    Actor               actor;
    Actor_ObjKibakoFunc actionFunc;
    ColliderCylinder    collider;
    s16                 timer;
    s16                 objectSlot;
    s8                  isDropCollected;
    s8                  unk199;
};

#endif
