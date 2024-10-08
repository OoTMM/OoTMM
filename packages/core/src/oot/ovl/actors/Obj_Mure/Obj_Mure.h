#ifndef OVL_OBJ_MURE_H
#define OVL_OBJ_MURE_H

#include <combo.h>

typedef struct Actor_ObjMure Actor_ObjMure;

typedef void (*Actor_ObjMureFunc)(Actor_ObjMure*, GameState_Play*);

#define OBJMURE_MAX_SPAWNS 15

struct Actor_ObjMure
{
    Actor               actor;
    Actor_ObjMureFunc   actionFunc;
    s16                 chNum;
    s16                 ptn;
    s16                 svNum;
    s16                 type;
    Actor*              children[OBJMURE_MAX_SPAWNS];
    u8                  childrenStates[OBJMURE_MAX_SPAWNS];
    s16                 unk_1A4;
    s16                 unk_1A6;
    s16                 unk_1A8;
    Xflag               xflag;
};

#endif
