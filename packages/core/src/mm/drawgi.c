#include <combo.h>

static const u64 kInitListMedallion[] = {
    0xe700000000000000, 0xd7000002ffffffff,
    0xfc11fe23fffff7fb, 0xef082c1000552078,
    0xd900000000220405, 0xdf00000000000000,
};

void DrawGi_Opa10_Xlu2(GameState_Play* play, s16 index)
{
    OPEN_DISPS(play->gs.gfx);
    InitListPolyOpa(play->gs.gfx);
    gSPMatrix(POLY_OPA_DISP++, GetMatrixMV(play->gs.gfx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, kDrawGi[index].lists[1]);
    gSPDisplayList(POLY_OPA_DISP++, kDrawGi[index].lists[0]);
    InitListPolyXlu(play->gs.gfx);
    gSPMatrix(POLY_XLU_DISP++, GetMatrixMV(play->gs.gfx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, kDrawGi[index].lists[2]);
    CLOSE_DISPS();
}

void DrawGi_Opa10_Xlu234(GameState_Play* play, s16 index)
{
    OPEN_DISPS(play->gs.gfx);
    InitListPolyOpa(play->gs.gfx);
    gSPMatrix(POLY_OPA_DISP++, GetMatrixMV(play->gs.gfx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, kDrawGi[index].lists[1]);
    gSPDisplayList(POLY_OPA_DISP++, kDrawGi[index].lists[0]);
    InitListPolyXlu(play->gs.gfx);
    gSPMatrix(POLY_XLU_DISP++, GetMatrixMV(play->gs.gfx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, kDrawGi[index].lists[2]);
    gSPDisplayList(POLY_XLU_DISP++, kDrawGi[index].lists[3]);
    gSPDisplayList(POLY_XLU_DISP++, kDrawGi[index].lists[4]);
    CLOSE_DISPS();
}

void DrawGi_Medallion(GameState_Play* play, s16 index)
{
    OPEN_DISPS(play->gs.gfx);
    gSPDisplayList(POLY_OPA_DISP++, (u32)(&kInitListMedallion));
    gSPMatrix(POLY_OPA_DISP++, GetMatrixMV(play->gs.gfx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, kDrawGi[index].lists[0]);
    gSPDisplayList(POLY_OPA_DISP++, kDrawGi[index].lists[1]);
    CLOSE_DISPS();
}

void DrawGi_Spell(GameState_Play* play, s16 index)
{
    s32 fc = play->gs.frameCount;

    OPEN_DISPS(play->gs.gfx);
    InitListPolyXlu(play->gs.gfx);
    gSPSegment(POLY_XLU_DISP++, 8, DisplaceTexture(play->gs.gfx, 0, fc * 2, fc * -6, 0x20, 0x20, 1, fc, fc * -2, 0x20, 0x20));
    gSPMatrix(POLY_XLU_DISP++, GetMatrixMV(play->gs.gfx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, kDrawGi[index].lists[0]);
    gSPDisplayList(POLY_XLU_DISP++, kDrawGi[index].lists[1]);
    gSPDisplayList(POLY_XLU_DISP++, kDrawGi[index].lists[2]);
    CLOSE_DISPS();
}

void DrawGi_Scale(GameState_Play* play, s16 index)
{
    s32 fc = play->gs.frameCount;

    OPEN_DISPS(play->gs.gfx);
    InitListPolyXlu(play->gs.gfx);
    gSPSegment(POLY_XLU_DISP++, 8, DisplaceTexture(play->gs.gfx, 0, fc * 2, fc * -2, 0x40, 0x40, 1, fc * 4, fc * -4, 0x20, 0x20));
    gSPMatrix(POLY_XLU_DISP++, GetMatrixMV(play->gs.gfx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, kDrawGi[index].lists[2]);
    gSPDisplayList(POLY_XLU_DISP++, kDrawGi[index].lists[3]);
    gSPDisplayList(POLY_XLU_DISP++, kDrawGi[index].lists[1]);
    gSPDisplayList(POLY_XLU_DISP++, kDrawGi[index].lists[0]);
    CLOSE_DISPS();
}

void DrawGi_MirrorShield(GameState_Play* play, s16 index)
{
    s32 fc = play->gs.frameCount;

    OPEN_DISPS(play->gs.gfx);
    InitListPolyOpa(play->gs.gfx);
    gSPSegment(POLY_OPA_DISP++, 8, DisplaceTexture(play->gs.gfx, 0, 0, (fc & 0x7f) << 1, 0x40, 0x40, 1, 0, (fc & 0x7f), 0x20, 0x20));
    gSPMatrix(POLY_OPA_DISP++, GetMatrixMV(play->gs.gfx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, kDrawGi[index].lists[0]);
    InitListPolyXlu(play->gs.gfx);
    gSPMatrix(POLY_XLU_DISP++, GetMatrixMV(play->gs.gfx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, kDrawGi[index].lists[1]);
    CLOSE_DISPS();
}

void DrawGi_SoldOut(GameState_Play* play, s16 index)
{
    static const u32 kInitListSoldOut[] = {
        0xE7000000, 0x00000000, 0xD7000002, 0xFFFFFFFF,
        0xFC11FE23, 0xFFFFF7FB, 0xEF082C10, 0x005049D8,
        0xD9000000, 0x00220405, 0xDF000000, 0x00000000,
    };
    const DrawGi* drawGi;
    drawGi = &kDrawGi[index];

    OPEN_DISPS(play->gs.gfx);
    gSPDisplayList(POLY_XLU_DISP++, (u32)kInitListSoldOut & 0xffffff);
    gSPMatrix(POLY_XLU_DISP++, GetMatrixMV(play->gs.gfx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, drawGi->lists[0]);
    CLOSE_DISPS();
}

void DrawGi_BlueFire(GameState_Play* play, s16 index)
{
    const DrawGi* drawGi;
    s32 fc;

    drawGi = &kDrawGi[index];
    fc = play->gs.frameCount;

    OPEN_DISPS(play->gs.gfx);
    InitListPolyOpa(play->gs.gfx);
    gSPMatrix(POLY_OPA_DISP++, GetMatrixMV(play->gs.gfx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, drawGi->lists[0]);

    InitListPolyXlu(play->gs.gfx);
    gSPSegment(POLY_XLU_DISP++, 8, DisplaceTexture(play->gs.gfx, 0, 0, 0, 0x10, 0x20, 1, fc, -8 * fc, 0x10, 0x20));
    MatrixStackDup();
    ModelViewTranslate(-8.f, -2.f, 0.f, MAT_MUL);
    ModelViewUnkTransform(&play->billboardMtxF);
    gSPMatrix(POLY_XLU_DISP++, GetMatrixMV(play->gs.gfx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, drawGi->lists[1]);
    MatrixStackPop();
    CLOSE_DISPS();
}
