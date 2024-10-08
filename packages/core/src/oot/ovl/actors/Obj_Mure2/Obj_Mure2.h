#ifndef OVL_OBJ_MURE2_H
#define OVL_OBJ_MURE2_H

#include <combo.h>

typedef struct Actor_ObjMure2 Actor_ObjMure2;

typedef void (*Actor_ObjMure2Func)(Actor_ObjMure2*, GameState_Play*);

struct Actor_ObjMure2
{
    Actor               actor;
    Actor_ObjMure2Func  actionFunc;
    Actor*              actorSpawnPtrList[12];
    u16                 currentActorNum;
    float               unk_184;
};

#endif
