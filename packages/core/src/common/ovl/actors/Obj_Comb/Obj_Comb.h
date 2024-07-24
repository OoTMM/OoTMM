#ifndef OVL_OBJ_COMB_H
#define OVL_OBJ_COMB_H

#include <combo/actor.h>

typedef struct Actor_ObjComb Actor_ObjComb;

typedef void (*Actor_ObjCombFunc)(Actor_ObjComb*, GameState_Play*);

struct Actor_ObjComb
{
    Actor                   actor;
    Actor_ObjCombFunc       actionFunc;
    ColliderJntSph          collider;
    ColliderJntSphElement   colliderItems[1];
    s16                     unk_1B0;
    s16                     unk_1B2;
};

#endif
