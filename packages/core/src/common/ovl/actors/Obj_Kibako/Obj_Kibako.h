#ifndef OVL_OBJ_KIBAKO_H
#define OVL_OBJ_KIBAKO_H

#include <combo/actor.h>
#include <combo/xflags.h>

typedef struct Actor_ObjKibako Actor_ObjKibako;

typedef void (*Actor_ObjKibakoFunc)(Actor_ObjKibako*, GameState_Play*);

typedef struct Actor_ObjKibako
{
    Actor               actor;
    Actor_ObjKibakoFunc actionFunc;
    ColliderCylinder    collider;

#if defined(GAME_MM)
    s16     timer;
    s16     objectSlot;
    s8      isDropCollected;
    s8      unk199;
#endif

    Xflag xflag;
    u8    isExtended:1;
}
Actor_ObjKibako;

#endif
