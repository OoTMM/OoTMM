#include <combo.h>

void    InitListPolyOpa(GfxContext* gfx);
void    InitListPolyXlu(GfxContext* gfx);
void*   GetMatrixMV(GfxContext* gfx);

u32 GetSegment(GfxContext*, int, int, int, int, int, int, int, int, int, int);

static const u64 kInitListMedallion[] = {
    0xe700000000000000, 0xd7000002ffffffff,
    0xfc11fe23fffff7fb, 0xef082c1000552078,
    0xd900000000220405, 0xdf00000000000000,
};

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

void Shader_Opa10_Xlu234(GameState* gs, s16 index)
{
    OPEN_DISPS(gs->gfx);
    InitListPolyOpa(gs->gfx);
    gSPMatrix(POLY_OPA_DISP++, GetMatrixMV(gs->gfx), G_MTX_PUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, kShaders[index].lists[1]);
    gSPDisplayList(POLY_OPA_DISP++, kShaders[index].lists[0]);
    InitListPolyXlu(gs->gfx);
    gSPMatrix(POLY_XLU_DISP++, GetMatrixMV(gs->gfx), G_MTX_PUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, kShaders[index].lists[2]);
    gSPDisplayList(POLY_XLU_DISP++, kShaders[index].lists[3]);
    gSPDisplayList(POLY_XLU_DISP++, kShaders[index].lists[4]);
    CLOSE_DISPS();
}

void Shader_Medallion(GameState* gs, s16 index)
{
    OPEN_DISPS(gs->gfx);
    gSPDisplayList(POLY_OPA_DISP++, (u32)(&kInitListMedallion));
    gSPMatrix(POLY_OPA_DISP++, GetMatrixMV(gs->gfx), G_MTX_PUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, kShaders[index].lists[0]);
    gSPDisplayList(POLY_OPA_DISP++, kShaders[index].lists[1]);
    CLOSE_DISPS();
}

void Shader_Spell(GameState* gs, s16 index)
{
    s32 fc = gs->frameCount;

    OPEN_DISPS(gs->gfx);
    InitListPolyXlu(gs->gfx);
    gSPSegment(POLY_XLU_DISP++, 8, GetSegment(gs->gfx, 0, fc * 2, fc * -6, 0x20, 0x20, 1, fc, fc * -2, 0x20, 0x20));
    gSPMatrix(POLY_XLU_DISP++, GetMatrixMV(gs->gfx), G_MTX_PUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, kShaders[index].lists[0]);
    gSPDisplayList(POLY_XLU_DISP++, kShaders[index].lists[1]);
    gSPDisplayList(POLY_XLU_DISP++, kShaders[index].lists[2]);
    CLOSE_DISPS();
}

void Shader_Scale(GameState* gs, s16 index)
{
    s32 fc = gs->frameCount;

    OPEN_DISPS(gs->gfx);
    InitListPolyXlu(gs->gfx);
    gSPSegment(POLY_XLU_DISP++, 8, GetSegment(gs->gfx, 0, fc * 2, fc * -2, 0x40, 0x40, 1, fc * 4, fc * -4, 0x20, 0x20));
    gSPMatrix(POLY_XLU_DISP++, GetMatrixMV(gs->gfx), G_MTX_PUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, kShaders[index].lists[2]);
    gSPDisplayList(POLY_XLU_DISP++, kShaders[index].lists[3]);
    gSPDisplayList(POLY_XLU_DISP++, kShaders[index].lists[1]);
    gSPDisplayList(POLY_XLU_DISP++, kShaders[index].lists[0]);
    CLOSE_DISPS();
}

void Shader_MirrorShield(GameState* gs, s16 index)
{

}

void Shader_SoldOut(GameState* gs, s16 index)
{

}

void Shader_BlueFire(GameState* gs, s16 index)
{

}
