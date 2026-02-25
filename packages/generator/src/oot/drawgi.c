#include <combo.h>

void DrawGi_Opa01(PlayState* play, s16 drawGiId)
{
    const DrawGi* drawGi;
    drawGi = &kDrawGi[drawGiId];

    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[0]);
    gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[1]);
    CLOSE_DISPS();
}

static Gfx kDummyList[] = {
    gsSPEndDisplayList(),
};

/* Halo isn't animated */
void DrawGi_MoonTear(PlayState* play, s16 drawGiId)
{
    const DrawGi* drawGi;
    drawGi = &kDrawGi[drawGiId];

    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x09, kDummyList);
    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[0]);
    gSPSegment(POLY_XLU_DISP++, 0x0a, kDummyList);
    ModelViewUnkTransform(&play->billboardMtxF);
    gSPMatrix(POLY_XLU_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, drawGi->lists[1]);
    CLOSE_DISPS();
}
