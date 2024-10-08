#ifndef OVL_OBJ_MURE_H
#define OVL_OBJ_MURE_H

#include <combo.h>

typedef struct Actor_ObjMure Actor_ObjMure;

typedef void (*Actor_ObjMureFunc)(Actor_ObjMure*, GameState_Play*);

#define OBJ_MURE_GET_CHNUM(thisx) (((thisx)->params >> 12) & 0xF)
#define OBJ_MURE_GET_PTN(thisx) (((thisx)->params >> 8) & 0x7)
#define OBJ_MURE_GET_SVNUM(thisx) (((thisx)->params >> 5) & 0x3)
#define OBJ_MURE_GET_TYPE(thisx) ((thisx)->params & 0x1F)

typedef enum
{
    OBJMURE_TYPE_GRASS,
    OBJMURE_TYPE_UNDEFINED,
    OBJMURE_TYPE_FISH,
    OBJMURE_TYPE_BUGS,
    OBJMURE_TYPE_BUTTERFLY,
    OBJMURE_TYPE_MAX
}
ObjMureType;

#define OBJMURE_MAX_SPAWNS 15

typedef struct
{
    Actor       actor;
    char        unk_144[0x53];
    u8          unk_197;
}
ObjMureChild;

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
    s16                 unk_19C;
    s16                 unk_19E;
    s16                 unk_1A0;
    Xflag               xflag;
};

#endif
