#include <combo.h>
#include <combo/gi.h>
#include <combo/z64/draw.h>
#include <gfx.h>
#include <gfx_setupdl.h>
#include <z64play.h>
#include "sys_matrix.h"

#if defined(__OOT__)
# include "macros.h"
# define Gfx_SetupDL25_Opa Gfx_SetupDL_25Opa
# define Gfx_SetupDL25_Xlu Gfx_SetupDL_25Xlu
# define XOPEN_DISPS(ctx) OPEN_DISPS(ctx, __FILE__, __LINE__)
# define XCLOSE_DISPS(ctx) CLOSE_DISPS(ctx, __FILE__, __LINE__)
# define XMATRIX_FINALIZE_AND_LOAD(pkt, gfxCtx) MATRIX_FINALIZE_AND_LOAD(pkt, gfxCtx, __FILE__, __LINE__)
#endif

#if defined(__MM__)
# define XOPEN_DISPS(ctx) OPEN_DISPS(ctx)
# define XCLOSE_DISPS(ctx) CLOSE_DISPS(ctx)
# define XMATRIX_FINALIZE_AND_LOAD(pkt, gfxCtx) MATRIX_FINALIZE_AND_LOAD(pkt, gfxCtx)
#endif

void GetItem_DrawEx(PlayState* play, s16 gi)
{
    void* data;

    if (gi == GI_NONE)
        return;

    /* Load the deku mask file */
    data = File_CacheLoad(0x100);
    if (!data)
        return;

    /* Draw the deku mask */
    XOPEN_DISPS(play->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x06, data);
    gSPSegment(POLY_XLU_DISP++, 0x06, data);
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    XMATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);
    gSPDisplayList(POLY_OPA_DISP++, 0);
    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    XMATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
    gSPDisplayList(POLY_XLU_DISP++, 0);
    XCLOSE_DISPS(play->state.gfxCtx);

}
