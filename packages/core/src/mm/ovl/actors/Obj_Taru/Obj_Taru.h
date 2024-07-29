#ifndef OVL_OBJ_TARU_H
#define OVL_OBJ_TARU_H

#include <combo/actor.h>

typedef struct Actor_ObjTaru Actor_ObjTaru;

typedef void (*Actor_ObjTaruFunc)(Actor_ObjTaru*, GameState_Play*);

struct Actor_ObjTaru
{
    DynaPolyActor       dyna;
    ColliderCylinder    collider;
    Actor_ObjTaruFunc   actionFunc;
    s8                  unk_1AC;
};

#endif

