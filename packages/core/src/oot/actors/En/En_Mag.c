#include <combo.h>

void EnMag_BeforeUpdate(Actor* this, GameState_Play* play)
{
    if (gComboCtx.valid && play->transitionTrigger == 0)
    {
        *(u32*)(((char*)&gSaveContext) + 4956) = 2;
        play->transitionTrigger = TRANS_TYPE_NORMAL;
        play->transitionType = 4;
    }
}

void* gMmMag;

static void BlitTexture(GfxContext* gfx, u32 addr, s16 w, s16 h, s16 x, s16 y, float scale)
{
    float rScale;
    s16 hSlice;

    hSlice = 4;
    rScale = 1.f / scale;

    OPEN_DISPS(gfx);
    gDPPipeSync(OVERLAY_DISP++);
    gDPTileSync(OVERLAY_DISP++);

    for (int j = 0; j < h / hSlice; ++j)
    {
        gDPLoadTextureTile(
            OVERLAY_DISP++,
            addr + (4 * w * hSlice * j),
            G_IM_FMT_RGBA, G_IM_SIZ_32b,
            w, hSlice,
            0, 0,
            w - 1, hSlice - 1,
            0,
            G_TX_WRAP, G_TX_WRAP,
            G_TX_NOMASK, G_TX_NOMASK,
            G_TX_NOLOD, G_TX_NOLOD
        );
        gDPTileSync(OVERLAY_DISP++);
        gSPTextureRectangle(
            OVERLAY_DISP++,
            x * 4, y * 4 + (j * hSlice * 4) * scale,
            x * 4 + (w * 4) * scale, y * 4 + ((j + 1) * hSlice * 4) * scale,
            0,
            0, 0,
            ((1 << 10) * rScale), ((1 << 10) * rScale)
        );
    }
    CLOSE_DISPS();
}

void EnMag_AfterDraw(Actor* this, GameState_Play* play)
{
    u32 size;
    u32 objectId;
    float alpha;
    int iAlpha;

    if (!gMmMag)
    {
        objectId = MASK_FOREIGN_OBJECT | 0x115;
        size = comboLoadObject(NULL, objectId);
        gMmMag = malloc(size);
        comboLoadObject(gMmMag, objectId);
    }

    alpha = *(float*)((char*)this + 0xe2f0);
    iAlpha = (int)(alpha);

    OPEN_DISPS(play->gs.gfx);

    /* Init code */
    gSPLoadGeometryMode(OVERLAY_DISP++, 0);
    gDPSetAlphaDither(OVERLAY_DISP++, G_AD_DISABLE);
    gDPSetColorDither(OVERLAY_DISP++, G_CD_DISABLE);
    gDPSetAlphaCompare(OVERLAY_DISP++, G_AC_NONE);
    gDPSetDepthSource(OVERLAY_DISP++, G_ZS_PRIM);
    gDPSetCombineMode(OVERLAY_DISP++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    gDPSetCombineKey(OVERLAY_DISP++, G_CK_NONE);
    gDPSetTextureConvert(OVERLAY_DISP++, G_TC_FILT);
    gDPSetTextureDetail(OVERLAY_DISP++, G_TD_CLAMP);
    gDPSetTexturePersp(OVERLAY_DISP++, G_TP_NONE);
    gDPSetTextureLOD(OVERLAY_DISP++, G_TL_TILE);
    gDPSetTextureLUT(OVERLAY_DISP++, G_TT_NONE);
    gDPPipelineMode(OVERLAY_DISP++, G_PM_NPRIMITIVE);
    gDPSetCycleType(OVERLAY_DISP++, G_CYC_1CYCLE);
    gDPSetRenderMode(OVERLAY_DISP++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gDPSetTextureFilter(OVERLAY_DISP++, G_TF_BILERP);

    if (iAlpha)
    {
        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 0xff, 0xff, 0xff, iAlpha);

        /* OoT Logo */
        BlitTexture(play->gs.gfx, 0x6000300, 160, 160, 30, 50, 0.65f);

        /* MM Logo */
        gSPSegment(OVERLAY_DISP++, 0x6, gMmMag);
        BlitTexture(play->gs.gfx, 0x06011e48, 0x80, 0x70, 165, 65, 0.65f);
        BlitTexture(play->gs.gfx, 0x06000000, 0x90, 0x40, 190, 85, 0.65f);
    }
    CLOSE_DISPS();
}
