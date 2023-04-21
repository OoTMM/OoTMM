#include <combo.h>

void Shader_Opa01(GameState_Play* play, s16 shaderId)
{
    const Shader* shader;
    shader = &kShaders[shaderId];

    OPEN_DISPS(play->gs.gfx);
    InitListPolyOpa(play->gs.gfx);
    gSPMatrix(POLY_OPA_DISP++, GetMatrixMV(play->gs.gfx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, shader->lists[0]);
    gSPDisplayList(POLY_OPA_DISP++, shader->lists[1]);
    CLOSE_DISPS();
}

static Gfx kDummyList[] = {
    gsSPEndDisplayList(),
};

/* Halo isn't animated */
void Shader_MoonTear(GameState_Play* play, s16 shaderId)
{
    const Shader* shader;
    shader = &kShaders[shaderId];

    OPEN_DISPS(play->gs.gfx);
    InitListPolyOpa(play->gs.gfx);
    InitListPolyXlu(play->gs.gfx);
    gSPSegment(POLY_OPA_DISP++, 0x09, kDummyList);
    gSPMatrix(POLY_OPA_DISP++, GetMatrixMV(play->gs.gfx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, shader->lists[0]);
    gSPSegment(POLY_XLU_DISP++, 0x0a, kDummyList);
    ModelViewUnkTransform((float*)((char*)play + 0x11da0));
    gSPMatrix(POLY_XLU_DISP++, GetMatrixMV(play->gs.gfx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, shader->lists[1]);
    CLOSE_DISPS();
}
