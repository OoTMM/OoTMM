#ifndef COMBO_GFX_H
#define COMBO_GFX_H

#include <ultra64.h>

typedef struct PACKED ALIGNED(4)
{
    u32     size;
    Gfx*    ptr;
    Gfx*    append;
    Gfx*    end;
}
DisplayListBuffer;

#if defined (GAME_OOT)
# include <combo/oot/gfx.h>
#endif

#if defined (GAME_MM)
# include <combo/mm/gfx.h>
#endif

#define OPEN_DISPS(gfx)             \
    {                               \
        GfxContext* __gfx = (gfx);  \
        (void)__gfx;

#define CLOSE_DISPS()               \
    }

#define WORK_DISP       (__gfx->work.append)
#define POLY_OPA_DISP   (__gfx->polyOpa.append)
#define POLY_XLU_DISP   (__gfx->polyXlu.append)
#define OVERLAY_DISP    (__gfx->overlay.append)

void    InitListPolyOpa(GfxContext* gfx);
void    InitListPolyXlu(GfxContext* gfx);
void*   GetMatrixMV(GfxContext* gfx);
u32     GetSegment(GfxContext* gfx, int, int, int, int, int, int, int, int, int, int);

#endif
