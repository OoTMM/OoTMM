#ifndef COMBO_OOT_EN_OSSAN_H
#define COMBO_OOT_EN_OSSAN_H

#include <combo/common/actor.h>

typedef struct Actor_EnGirlA Actor_EnGirlA;

typedef struct
{
    Actor           base;
    char            unk_13c[0xb4];
    Actor_EnGirlA*  items[8];
    char            unk_210[0x32];
    u8              itemIndex;
    char            unk_243[0x85];
}
Actor_EnOssan;

_Static_assert(sizeof(Actor_EnOssan) == 0x2c8, "OoT Actor_EnOssan size is wrong");

#endif
