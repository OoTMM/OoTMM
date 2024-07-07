#include <combo.h>
#include <combo/player.h>
#include <combo/draw.h>
#include <combo/csmc.h>
#include <combo/custom.h>

ALIGNED(16) const Gfx kDListEmpty[] = {
    gsSPEndDisplayList(),
};

void PreDraw1(Actor* actor, GameState_Play* play, int unk);
void PreDraw2(Actor* actor, GameState_Play* play, int unk);

void Draw_SetObjectSegment(GfxContext* gfx, void* buffer)
{
    /* Set the segment in the display list */
    OPEN_DISPS(gfx);
    gSPSegment(POLY_OPA_DISP++, 0x06, buffer);
    gSPSegment(POLY_XLU_DISP++, 0x06, buffer);
    CLOSE_DISPS();

    /* Set the segment on the CPU side - required for Moon tear */
    gSegments[0x06] = (u32)buffer - 0x80000000;
}

static u8 playerDrawGiParam;

static u8 paramForGi(s16 gi)
{
    return kGetItemDrawGiParam[gi - 1];
}

static void drawGiParamDrawId(GameState_Play* play, u8 drawGiId, u8 param)
{
    if (drawGiId == DRAWGI_NONE)
        return;

    kDrawGi[drawGiId - 1].func(play, drawGiId - 1, param);
}

static void drawGiParam(GameState_Play* play, s16 gi)
{
    const GetItem* giEntry;
    u16 drawGiId;
    u8 param;

    giEntry = kExtendedGetItems + gi - 1;
    drawGiId = giEntry->drawGiId;
    param = paramForGi(gi);
    drawGiParamDrawId(play, drawGiId, param);
}

void Draw_Gi(GameState_Play* play, Actor* actor, s16 gi, int flags)
{
    const GetItem* giEntry;
    void* objBuffer;
    u16 objectId;

    giEntry = kExtendedGetItems + gi - 1;
    objectId = giEntry->objectId;

    if (objectId & ~MASK_FOREIGN_OBJECT)
    {
        objBuffer = comboGetObject(objectId);
        Draw_SetObjectSegment(play->gs.gfx, objBuffer);
    }
    if (!(flags & DRAW_NO_PRE1))
        PreDraw1(actor, play, 0);
    if (!(flags & DRAW_NO_PRE2))
        PreDraw2(actor, play, 0);
    drawGiParam(play, gi);
}

void comboPlayerDrawGI(GameState_Play* play, int drawGiMinusOne)
{
    drawGiParamDrawId(play, (u8)(drawGiMinusOne + 1), playerDrawGiParam);
}

void comboPlayerSetDrawGi(Actor_Player* link)
{
    playerDrawGiParam = paramForGi(link->gi);
    link->drawGiId = kExtendedGetItems[link->gi - 1].drawGiId;
}

void Draw_Init2D(Gfx** dl)
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

void Draw_Blit2D_RGBA32(Gfx** dl, u32 segAddr, int w, int h, float x, float y, float scale)
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


void Draw_Blit2D_RGBA16(Gfx** dl, u32 segAddr, int w, int h, float x, float y, float scale)
{
    float rScale;

    rScale = 1.f / scale;
    gDPPipeSync((*dl)++);
    gDPTileSync((*dl)++);
    gDPLoadTextureTile(
        (*dl)++,
        segAddr,
        G_IM_FMT_RGBA, G_IM_SIZ_16b,
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

void Draw_Blit2D_IA4(Gfx** dl, u32 segAddr, int w, int h, float x, float y, float scale)
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

static const Vtx kGlitterVtx[] = {
    {{ { -10, -10, 0 }, 0, { 0, 0 }, { 0xff, 0xff, 0xff, 0xff } }},
    {{ {  10, -10, 0 }, 0, { 0x200, 0 }, { 0xff, 0xff, 0xff, 0xff } }},
    {{ {  10,  10, 0 }, 0, { 0x200, 0x200 }, { 0xff, 0xff, 0xff, 0xff } }},
    {{ { -10,  10, 0 }, 0, { 0, 0x200 }, { 0xff, 0xff, 0xff, 0xff } }},
};

static const Gfx kDlistGlitter[] = {
    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_2CYCLE),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock_4b(0x06000000, G_IM_FMT_IA, 16, 16, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineMode(G_CC_MODULATEIA_PRIM, G_CC_PASS2),
    gsDPSetRenderMode(G_RM_PASS, G_RM_AA_ZB_XLU_SURF2),
    gsSPClearGeometryMode(G_CULL_BACK | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPVertex(&kGlitterVtx, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 0, 3, 2, 0),
    gsSPEndDisplayList(),
};

static u32 Draw_GetGlitterColor(s16 gi)
{
    int type;

    if (!csmcEnabled())
        type = CSMC_SPIDER;
    else
        type = csmcFromItem(gi);

    switch (type)
    {
    case CSMC_NORMAL: return 0x4a230d;
    case CSMC_BOSS_KEY: return 0x0000ff;
    case CSMC_MAJOR: return 0xffff00;
    case CSMC_KEY: return 0x222222;
    case CSMC_SPIDER: return 0xffffff;
    case CSMC_FAIRY: return 0xfa89ef;
    case CSMC_HEART: return 0xff0000;
    case CSMC_SOUL: return 0x5e0cc9;
    case CSMC_MAP_COMPASS: return 0xc75000;
    default: return 0xffffff;
    }
}

void Draw_GlitterGi(GameState_Play* play, Actor* actor, s16 gi)
{
    void* tex;
    float alpha;
    float alphaNoZ;
    u32 color;
    u8 r;
    u8 g;
    u8 b;

    /* Get color */
    color = Draw_GetGlitterColor(gi);
    r = (color >> 16) & 0xff;
    g = (color >>  8) & 0xff;
    b = (color >>  0) & 0xff;

    /* Compute alpha */
    if (actor->xzDistToPlayer > 1000.f)
        return;
    else if (actor->xzDistToPlayer <= 300.f)
        alpha = 1.f;
    else
        alpha = (1000.f - actor->xzDistToPlayer) * (1.f / 700.f);

    if (actor->xzDistToPlayer > 600.f)
        alphaNoZ = 0.f;
    else if (actor->xzDistToPlayer <= 300.f)
        alphaNoZ = 1.f;
    else
        alphaNoZ = (600.f - actor->xzDistToPlayer) * (1.f / 300.f);

    /* Compute the texture */
    tex = comboCacheGetFile(CUSTOM_GLITTER_ADDR);
    if (!tex)
        return;
    if (play->gs.frameCount & 4)
        tex = (void*)((u32)tex + 16 * 8);

    /* Prepare the Matrix */
    ModelViewTranslate(actor->world.pos.x, actor->world.pos.y, actor->world.pos.z, MAT_SET);
    ModelViewUnkTransform(&play->billboardMtxF);
    ModelViewTranslate(0, 20.f, 0.f, MAT_MUL);

    /* Draw the display list */
    OPEN_DISPS(play->gs.gfx);
    gSPMatrix(POLY_XLU_DISP++, GetMatrixMV(play->gs.gfx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPSegment(POLY_XLU_DISP++, 0x06, (u32)tex - 0x80000000);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, r, g, b, (alpha * 0.90f) * 255);
    gSPDisplayList(POLY_XLU_DISP++, (u32)kDlistGlitter - 0x80000000);
    if (alphaNoZ > 0.f)
    {
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, r, g, b, (alphaNoZ * 0.25f) * 255);
        gSPClearGeometryMode(POLY_XLU_DISP++, G_ZBUFFER);
        gSPDisplayList(POLY_XLU_DISP++, (u32)kDlistGlitter - 0x80000000);
        gSPSetGeometryMode(POLY_XLU_DISP++, G_ZBUFFER);
    }
    CLOSE_DISPS();
}
