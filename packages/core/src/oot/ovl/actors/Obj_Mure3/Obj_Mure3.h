#ifndef OVL_OBJ_MURE3_H
#define OVL_OBJ_MURE3_H

#include <combo.h>
#include <combo/xflags.h>

typedef struct Actor_ObjMure3 Actor_ObjMure3;

typedef void (*Actor_ObjMure3Func)(Actor_ObjMure3*, PlayState*);
typedef void (*Actor_ObjMure3SpawnFunc)(Actor_ObjMure3*, PlayState*);

typedef struct Actor_ObjMure3
{
    Actor               actor;
    Actor_ObjMure3Func  actionFunc;
    Actor_EnItem00*     unk_150[7];
    u16                 unk_16C;
    Xflag               xflag;
}
Actor_ObjMure3;

#endif
