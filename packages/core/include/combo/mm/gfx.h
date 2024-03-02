#ifndef COMBO_MM_GFX_H
#define COMBO_MM_GFX_H

#include <combo/types.h>

typedef struct PACKED ALIGNED(4)
{
    /* 0x000 */ Gfx* polyOpaBuffer;
    /* 0x004 */ Gfx* polyXluBuffer;
    /* 0x008 */ u8 pad8[0x8];
    /* 0x010 */ Gfx* overlayBuffer;
    /* 0x014 */ u8 pad14[0x24];
    /* 0x038 */ OSMesg taskMesg[8];
    /* 0x058 */ OSMesgQueue* unk58;
    /* 0x05C */ OSMesgQueue taskQueue;
    /* 0x074 */ u8 pad74[0x4];
    /* 0x078 */ u8 task[0x58]; /* OSScTask */
    /* 0x0D0 */ u8 padD0[0xD0];
    /* 0x1A0 */ Gfx* unk1A0;
    /* 0x1A4 */ DisplayListBuffer unk1A4;
    /* 0x1B4 */ Gfx* unk1B4;
    /* 0x1B8 */ DisplayListBuffer unk1B8;
    /* 0x1C8 */ u8 pad1C8[0xAC];
    /* 0x274 */ u32 unk274; /* OSViMode* */
    /* 0x278 */ void* zbuffer;
    /* 0x27C */ u8 pad27C[0x1C];
    /* 0x298 */ DisplayListBuffer overlay;
    /* 0x2A8 */ DisplayListBuffer polyOpa;
    /* 0x2B8 */ DisplayListBuffer polyXlu;
    /* 0x2C8 */ u32 displayListCounter;
    /* 0x2CC */ void* framebuffer;
    /* 0x2D0 */ u32 unk2D0;
    /* 0x2D4 */ u32 viConfigFeatures;
    /* 0x2D8 */ u8 pad2D8[0x3];
    /* 0x2DB */ u8 framebufferCounter;
    /* 0x2DC */ void* func2DC;
    /* 0x2E0 */ GameState_Play* play;
    /* 0x2E4 */ f32 viConfigXScale;
    /* 0x2E8 */ f32 viConfigYScale;
    /* 0x2EC */ u8 pad2EC[0x4];
}
GfxContext;

_Static_assert(sizeof(GfxContext) == 0x2f0, "MM GfxContext is not the correct size");

#endif
