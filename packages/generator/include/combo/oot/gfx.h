#ifndef COMBO_OOT_GFX_H
#define COMBO_OOT_GFX_H

#include <combo/types.h>

typedef struct PACKED ALIGNED(4) GraphicsContext
{
    char                unk_000[0x1b4];
    DisplayListBuffer   work;
    char                unk_1c4[0xe4];
    DisplayListBuffer   overlay;
    DisplayListBuffer   polyOpa;
    DisplayListBuffer   polyXlu;
    u32                 displayListCounter;
    char                unk_2dc[0x14];
    PlayState*     play;
}
GraphicsContext;

_Static_assert(sizeof(GraphicsContext) == 0x2f4, "GraphicsContext is not the correct size");

Gfx* Gfx_TwoTexScroll(GraphicsContext* gfxCtx, s32 tile1, u32 x1, u32 y1, s32 width1, s32 height1, s32 tile2, u32 x2, u32 y2, s32 width2, s32 height2);
void Gfx_SetupDL_25Xlu(GraphicsContext* gfxCtx);
void Gfx_SetupDL_25Opa(GraphicsContext* gfxCtx);

#endif
