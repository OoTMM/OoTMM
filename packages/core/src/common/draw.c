#include <combo.h>

void PreDraw1(Actor* actor, GameState_Play* play, int unk);
void PreDraw2(Actor* actor, GameState_Play* play, int unk);

void comboSetObjectSegment(GfxContext* gfx, void* buffer)
{
    /* Set the segment in the display list */
    OPEN_DISPS(gfx);
    gSPSegment(POLY_OPA_DISP++, 0x06, buffer);
    gSPSegment(POLY_XLU_DISP++, 0x06, buffer);
    CLOSE_DISPS();

    /* Set the segment on the CPU side - required for Moon tear */
    gSegments[0x06] = (u32)buffer - 0x80000000;
}

void comboDrawObject(GameState_Play* play, Actor* actor, u16 objectId, u16 shaderId, int flags)
{
    void* objBuffer;

    if (objectId & ~MASK_FOREIGN_OBJECT)
    {
        objBuffer = comboGetObject(objectId);
        comboSetObjectSegment(play->gs.gfx, objBuffer);
    }
    if (!(flags & DRAW_NO_PRE1))
        PreDraw1(actor, play, 0);
    if (!(flags & DRAW_NO_PRE2))
        PreDraw2(actor, play, 0);
    kShaders[shaderId - 1].func(play, shaderId - 1);
}

void comboDrawGI(GameState_Play* play, Actor* actor, int gi, int flags)
{
    const GetItem* giEntry;

    giEntry = kExtendedGetItems + gi - 1;
    comboDrawObject(play, actor, giEntry->objectId, giEntry->shaderId, flags);
}

void comboDrawInit2D(Gfx** dl)
{
    gSPLoadGeometryMode((*dl)++, 0);
    gDPSetAlphaDither((*dl)++, G_AD_DISABLE);
    gDPSetColorDither((*dl)++, G_CD_DISABLE);
    gDPSetAlphaCompare((*dl)++, G_AC_NONE);
    gDPSetDepthSource((*dl)++, G_ZS_PRIM);
    gDPSetCombineMode((*dl)++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    gDPSetCombineKey((*dl)++, G_CK_NONE);
    gDPSetTextureConvert((*dl)++, G_TC_FILT);
    gDPSetTextureDetail((*dl)++, G_TD_CLAMP);
    gDPSetTexturePersp((*dl)++, G_TP_NONE);
    gDPSetTextureLOD((*dl)++, G_TL_TILE);
    gDPSetTextureLUT((*dl)++, G_TT_NONE);
    gDPPipelineMode((*dl)++, G_PM_NPRIMITIVE);
    gDPSetCycleType((*dl)++, G_CYC_1CYCLE);
    gDPSetRenderMode((*dl)++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gDPSetTextureFilter((*dl)++, G_TF_BILERP);
}

void comboDrawBlit2D(Gfx** dl, u32 segAddr, int w, int h, float x, float y, float scale)
{
    float rScale;

    rScale = 1.f / scale;
    gDPPipeSync((*dl)++);
    gDPTileSync((*dl)++);
    gDPLoadTextureTile(
        (*dl)++,
        segAddr,
        G_IM_FMT_RGBA, G_IM_SIZ_32b,
        w, h,
        0, 0,
        w - 1, h - 1,
        0,
        G_TX_WRAP, G_TX_WRAP,
        G_TX_NOMASK, G_TX_NOMASK,
        G_TX_NOLOD, G_TX_NOLOD
    );
    gDPTileSync((*dl)++);
    gSPTextureRectangle(
        (*dl)++,
        x * 4, y * 4,
        x * 4 + (w * 4) * scale, y * 4 + (h * 4) * scale,
        0,
        0, 0,
        ((1 << 10) * rScale), ((1 << 10) * rScale)
    );
}

void comboDrawBlit2D_IA4(Gfx** dl, u32 segAddr, int w, int h, float x, float y, float scale)
{
    float rScale;

    rScale = 1.f / scale;
    gDPPipeSync((*dl)++);
    gDPTileSync((*dl)++);
    gDPLoadTextureTile_4b(
        (*dl)++,
        segAddr,
        G_IM_FMT_IA,
        w, h,
        0, 0,
        w - 1, h - 1,
        0,
        G_TX_WRAP, G_TX_WRAP,
        G_TX_NOMASK, G_TX_NOMASK,
        G_TX_NOLOD, G_TX_NOLOD
    );
    gDPTileSync((*dl)++);
    gSPTextureRectangle(
        (*dl)++,
        x * 4, y * 4,
        x * 4 + (w * 4) * scale, y * 4 + (h * 4) * scale,
        0,
        0, 0,
        ((1 << 10) * rScale), ((1 << 10) * rScale)
    );
}
