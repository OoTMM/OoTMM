#ifndef BUFFERS_H
#define BUFFERS_H

#include "assert.h"
#include "gfx.h"
#include "macros.h"
#include "stack.h"
#include "z64save.h"

typedef union {
    u16 framebufferHiRes[HIRES_BUFFER_HEIGHT][HIRES_BUFFER_WIDTH] ALIGNED(64);
    struct {
        u16 framebuffer[SCREEN_HEIGHT][SCREEN_WIDTH] ALIGNED(64);
        u8 skyboxBuffer[0x5A360] ALIGNED(16);
    };
} BufferLow;

// Equivalent to gLoBuffer.framebufferHiRes, but a different symbol is required to match
extern u16 gFramebufferHiRes1[HIRES_BUFFER_WIDTH][HIRES_BUFFER_HEIGHT];

extern BufferLow gLoBuffer;


extern u8 gGfxSPTaskYieldBuffer[OS_YIELD_DATA_SIZE];
extern STACK(gGfxSPTaskStack, 0x400);
extern GfxPool gGfxPools[2];
extern u8 gAudioHeap[0x138000];
extern u8 gSystemHeap[];


typedef union {
    u16 framebufferHiRes[HIRES_BUFFER_HEIGHT][HIRES_BUFFER_WIDTH] ALIGNED(64);
    struct {
        u8 pictoPhotoI8[PICTO_PHOTO_SIZE] ALIGNED(64);
        u8 D_80784600[0x56200] ALIGNED(64);
        u16 framebuffer[SCREEN_HEIGHT][SCREEN_WIDTH] ALIGNED(64);
    };
} BufferHigh;

// Equivalent to gHiBuffer.framebufferHiRes, but a different symbol is required to match
extern u16 gFramebufferHiRes0[HIRES_BUFFER_HEIGHT][HIRES_BUFFER_WIDTH];

extern BufferHigh gHiBuffer;

#endif
