#ifndef COMBO_OOT_ACTOR_ENGIRLA_H
#define COMBO_OOT_ACTOR_ENGIRLA_H

#include <combo/common/actor.h>
#include <combo/util.h>
#include <combo/types.h>

/* Seems to be the same in MM? */
typedef struct
{
    Actor   base;
    char    unk_13c[0x54];
    u16     messageId2;
    s32     gi;
    u16     disabled;
    char    unk_19a[0x0e];
    u16     angle;
    char    unk_1aa[0x1a];
}
Actor_EnGirlA;

_Static_assert(sizeof(Actor_EnGirlA) == 0x1c4, "OoT Actor_EnGirlA size is wrong");

#endif
