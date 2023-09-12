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

#define GRAPH_ALLOC(gfxCtx, size) ((void*)((gfxCtx)->polyOpa.end = (Gfx*)((u8*)(gfxCtx)->polyOpa.end - ALIGN(size, 16))))

void    InitListPolyOpa(GfxContext* gfx);
void    InitListPolyXlu(GfxContext* gfx);
void    Gfx_SetupDL_42Opa(GfxContext* gfx);
void*   GetMatrixMV(GfxContext* gfx);
u32     DisplaceTexture(GfxContext* gfx, int, int, int, int, int, int, int, int, int, int);

#endif
