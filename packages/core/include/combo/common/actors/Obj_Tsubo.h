#ifndef COMBO_COMMON_ACTOR_OBJ_TSUBO_H
#define COMBO_COMMON_ACTOR_OBJ_TSUBO_H

#include <combo/xflags.h>
#include <combo/common/actor.h>

typedef struct ALIGNED(4)
{
    Actor   base;
    char    unk_13c[0x54];

#if defined(GAME_MM)
    char unk_mm[0x4];
#endif

    /* Extended flags */
    Xflag xflag;
    u8    isExtended;
}
Actor_ObjTsubo;

#if defined(GAME_OOT)
_Static_assert(sizeof(Actor_ObjTsubo) == 0x198, "OoT Actor_ObjTsubo size is wrong");
#else
_Static_assert(sizeof(Actor_ObjTsubo) == 0x1a4, "MM Actor_ObjTsubo size is wrong");
#endif

void ObjTsubo_Alias(Actor_ObjTsubo* this);
int  ObjTsubo_IsShuffled(Actor_ObjTsubo* this);

#endif
