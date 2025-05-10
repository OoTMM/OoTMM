#ifndef OVL_OBJ_MURE3_H
#define OVL_OBJ_MURE3_H

#include <combo.h>
#include <combo/xflags.h>

#define OBJMURE3_GET_SWITCH_FLAG(thisx)     ((thisx)->params & 0x7F)
#define OBJMURE3_PARAM_RUPEEINDEX(thisx)    (((thisx)->params >> 13) & 7)

typedef struct Actor_ObjMure3 Actor_ObjMure3;

typedef void (*Actor_ObjMure3Func)(Actor_ObjMure3*, PlayState*);
typedef void (*Actor_ObjMure3SpawnFunc)(Actor_ObjMure3*, PlayState*);

typedef struct Actor_ObjMure3
{
    Actor               actor;
    Actor_ObjMure3Func  actionFunc;
    Actor_EnItem00*     children[7];
    u16                 childrenBits;
}
Actor_ObjMure3;

#endif
