#ifndef COMBO_OOT_OBJ_MURE2_H
#define COMBO_OOT_OBJ_MURE2_H

#include <combo/xflags.h>
#include <combo/common/actor.h>

typedef struct ALIGNED(4)
{
    Actor           base;
    void*           handler;
    Actor*          children[12];
    char            unk_170[8];

    /* Extended flags */
    Xflag xflag;
}
Actor_ObjMure2;

static const int x = sizeof(Actor_ObjMure2);

_Static_assert(sizeof(Actor_ObjMure2) == 0x180, "OoT Actor_ObjMure2 size is wrong");

#endif
