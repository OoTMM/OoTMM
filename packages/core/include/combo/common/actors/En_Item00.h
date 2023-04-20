#ifndef COMBO_ACTORS_EN_ITEM00_H
#define COMBO_ACTORS_EN_ITEM00_H

#include <combo/common/actor.h>

typedef struct
{
    Actor   base;
    char    unk_13c[0x4];
    u16     collectibleFlag;
    char    unk_142[0x5a];
#if defined(GAME_MM)
    char    unk_mm[0x4];
#endif
}
Actor_EnItem00;

#if defined(GAME_OOT)
_Static_assert(sizeof(Actor_EnItem00) == 0x19c, "OoT Actor_EnItem00 size is wrong");
#else
_Static_assert(sizeof(Actor_EnItem00) == 0x1a8, "MM Actor_EnItem00 size is wrong");
#endif

#endif
