#ifndef COMBO_GFX_H
#define COMBO_GFX_H

typedef struct PACKED ALIGNED(4)
{
    u32     size;
    u64*    ptr;
    u64*    append;
    u64*    end;
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
        u32*        __dlist;        \
        (void)__gfx;                \
        (void)__dlist;

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
