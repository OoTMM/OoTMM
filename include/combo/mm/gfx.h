#ifndef COMBO_MM_GFX_H
#define COMBO_MM_GFX_H

#include <combo/types.h>

typedef struct PACKED ALIGNED(4)
{
    char                unk_000[0x298];
    DisplayListBuffer   overlay;
    DisplayListBuffer   polyOpa;
    DisplayListBuffer   polyXlu;
    char                unk_2c8[0x8];
}
GfxContext;

_Static_assert(sizeof(GfxContext) == 0x2d0, "MM GfxContext is not the correct size");

#endif
