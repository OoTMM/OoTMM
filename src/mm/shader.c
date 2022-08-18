#include <combo.h>

void    InitListPolyOpa(GfxContext* gfx);
void    InitListPolyXlu(GfxContext* gfx);
void*   GetMatrixMV(GfxContext* gfx);

void Shader_Opa10_Xlu2(GameState* gs, s16 index)
{
    OPEN_DISPS(gs->gfx);
    InitListPolyOpa(gs->gfx);
    gSPMatrix(POLY_OPA_DISP++, GetMatrixMV(gs->gfx), G_MTX_PUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, kShaders[index].lists[1]);
    gSPDisplayList(POLY_OPA_DISP++, kShaders[index].lists[0]);
    InitListPolyXlu(gs->gfx);
    gSPMatrix(POLY_XLU_DISP++, GetMatrixMV(gs->gfx), G_MTX_PUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, kShaders[index].lists[2]);
    CLOSE_DISPS();
}
