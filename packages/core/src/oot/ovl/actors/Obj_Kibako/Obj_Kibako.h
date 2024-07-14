#ifndef OVL_OBJ_KIBAKO_H
#define OVL_OBJ_KIBAKO_H

#include <combo/actor.h>

typedef struct Actor_ObjKibako Actor_ObjKibako;

typedef void (*Actor_ObjKibakoFunc)(Actor_ObjKibako*, GameState_Play*);

typedef struct Actor_ObjKibako
{
    Actor               actor;
    Actor_ObjKibakoFunc actionFunc;
    ColliderCylinder    collider;
}
Actor_ObjKibako;

#endif
