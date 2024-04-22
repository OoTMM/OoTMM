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

ALIGNED(16) extern const Gfx kDListEmpty[];

Gfx*    Gfx_SetupDL(Gfx* gfx, u32 i);
Gfx*    Gfx_SetupDL57(Gfx* gfx);
void    InitListPolyOpa(GfxContext* gfx);
void    InitListPolyXlu(GfxContext* gfx);
void    Gfx_SetupDL_42Opa(GfxContext* gfx);
void*   GetMatrixMV(GfxContext* gfx);
u32     DisplaceTexture(GfxContext* gfx, int, int, int, int, int, int, int, int, int, int);
Gfx*    Gfx_TexScroll(GfxContext* ctx, u32 x, u32 y, s32 width, s32 height);
void    Gfx_DrawDListOpa(GameState_Play* play, Gfx* dlist);
void    Gfx_DrawDListXlu(GameState_Play* play, Gfx* dlist);
void    Gfx_DrawFlameColor(GameState_Play* play, u32 color, float scale, float offsetY);

#endif
