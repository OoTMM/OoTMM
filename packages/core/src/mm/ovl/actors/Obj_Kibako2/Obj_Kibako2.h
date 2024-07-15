#ifndef OVL_OBJ_KIBAKO2_H
#define OVL_OBJ_KIBAKO2_H

#include <combo/actor.h>

#define KIBAKO2_COLLECTIBLE_ID(thisx) ((thisx)->dyna.actor.variable & 0x3F)
#define KIBAKO2_COLLECTIBLE_FLAG(thisx) (((thisx)->dyna.actor.variable >> 0x8) & 0x7F)
#define KIBAKO2_SKULLTULA_SPAWN_PARAM(thisx) ((((thisx)->dyna.actor.variable & 0x1F) << 2) | 0xFF01)
#define KIBAKO2_CONTENTS(thisx) (((thisx)->dyna.actor.variable >> 0xF) & 1)

typedef enum
{
    OBJKIBAKO2_CONTENTS_COLLECTIBLE,
    OBJKIBAKO2_CONTENTS_SKULLTULA
}
Actor_ObjKibako2Contents;

typedef struct Actor_ObjKibako2 Actor_ObjKibako2;

typedef void (*Actor_ObjKibako2Func)(Actor_ObjKibako2*, GameState_Play*);

struct Actor_ObjKibako2
{
    DynaPolyActor           dyna;
    ColliderCylinder        collider;
    Actor_ObjKibako2Func    actionFunc;
    s8                      unk_1AC;
    s8                      skulltulaNoiseTimer;
};

#endif
