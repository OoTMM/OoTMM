#ifndef COMBO_OOT_GFX_H
#define COMBO_OOT_GFX_H

#include <combo/types.h>

typedef struct PACKED ALIGNED(4)
{
    char                unk_000[0x1b4];
    DisplayListBuffer   work;
    char                unk_1c4[0xe4];
    DisplayListBuffer   overlay;
    DisplayListBuffer   polyOpa;
    DisplayListBuffer   polyXlu;
    u32                 displayListCounter;
    char                unk_2dc[0x14];
    GameState_Play*     play;
}
GfxContext;

_Static_assert(sizeof(GfxContext) == 0x2f4, "GfxContext is not the correct size");

#endif
