#ifndef COMBO_MM_GFX_H
#define COMBO_MM_GFX_H

#include <types.h>

typedef struct PACKED ALIGNED(4)
{
    u32     size;
    u64*    ptr;
    u64*    append;
    u64*    end;
}
DisplayListBuffer;

typedef struct PACKED ALIGNED(4)
{
    char                unk_000[0x2a8];
    DisplayListBuffer   polyOpa;
    DisplayListBuffer   polyXlu;
    char                unk_2c8[0x8];
}
GfxContext;

_Static_assert(sizeof(GfxContext) == 0x2d0, "MM GfxContext is not the correct size");

#define OPEN_DISPS(gfx)             \
    {                               \
        GfxContext* __gfx = (gfx);  \
        u32*        __dlist;        \
        (void)__gfx;                \
        (void)__dlist;

#define CLOSE_DISPS()               \
    }

#define WORK_DISP       (__gfx->work.append)
#define POLY_OPA_DISP   (__gfx->polyOpa.append)
#define POLY_XLU_DISP   (__gfx->polyXlu.append)
#define OVERLAY_DISP    (__gfx->overlay.append)

#endif
