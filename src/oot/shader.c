#include <combo.h>

void    InitListPolyOpa(GfxContext* gfx);
void*   GetMatrixMV(GfxContext* gfx);

void Shader_Opa01(GameState* gs, u16 shaderId)
{
    const Shader* shader;
    shader = &kShaders[shaderId];

    OPEN_DISPS(gs->gfx);
    InitListPolyOpa(gs->gfx);
    gSPMatrix(POLY_OPA_DISP++, GetMatrixMV(gs->gfx), G_MTX_PUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, shader->lists[0]);
    gSPDisplayList(POLY_OPA_DISP++, shader->lists[1]);
    CLOSE_DISPS();
}
