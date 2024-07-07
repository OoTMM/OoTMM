#ifndef OVL_OBJ_KIBAKO2_H
#define OVL_OBJ_KIBAKO2_H

#include <combo/types.h>
#include <combo/actor.h>
#include <combo/collision.h>
#include <combo/misc.h>

typedef struct Actor_ObjKibako2 Actor_ObjKibako2;
typedef struct GameState_Play GameState_Play;

typedef void (*Actor_ObjKibako2Func)(Actor_ObjKibako2*, GameState_Play*);

typedef struct Actor_ObjKibako2
{
    DynaPolyActor           dyna;
    ColliderCylinder        collider;
    Actor_ObjKibako2Func    actionFunc;
    s16                     collectibleFlag;
}
Actor_ObjKibako2;

#endif
