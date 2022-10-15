#ifndef COMBO_OOT_ACTORS_EN_ITEM00_H
#define COMBO_OOT_ACTORS_EN_ITEM00_H

#include <combo/common/actor.h>

typedef struct
{
    Actor   base;
    char    unk_13c[0x4];
    u16     collectibleFlag;
    char    unk_142[0x5a];
}
Actor_EnItem00;

_Static_assert(sizeof(Actor_EnItem00) == 0x19c, "Actor_EnItem00 size is wrong");

#endif
