#include <combo.h>
#include <combo/custom.h>
#include <combo/global.h>

static const Vtx kGlitterVtx[] = {
    {{ { -10, -10, 0 }, 0, { 0, 0 }, { 0xff, 0xff, 0xff, 0xff } }},
    {{ {  10, -10, 0 }, 0, { 0x200, 0 }, { 0xff, 0xff, 0xff, 0xff } }},
    {{ {  10,  10, 0 }, 0, { 0x200, 0x200 }, { 0xff, 0xff, 0xff, 0xff } }},
    {{ { -10,  10, 0 }, 0, { 0, 0x200 }, { 0xff, 0xff, 0xff, 0xff } }},
};

static const Gfx kDlistGlitter[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock_4b(0x06000000, G_IM_FMT_IA, 16, 16, 0, G_TX_MIRROR | G_TX_CLAMP, G_TX_MIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD),
    //gsDPLoadTextureBlock(0x06000000 | CUSTOM_KEEP_SF_TEXTURE_1, G_IM_FMT_IA, G_IM_SIZ_8b, 16, 16, 0, G_TX_MIRROR | G_TX_CLAMP, G_TX_MIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(TEXEL1, PRIMITIVE, PRIM_LOD_FRAC, TEXEL0, TEXEL1, 1, PRIM_LOD_FRAC, TEXEL0, PRIMITIVE, ENVIRONMENT, COMBINED, ENVIRONMENT, COMBINED, 0, PRIMITIVE, 0),
    gsDPSetRenderMode(G_RM_PASS, G_RM_ZB_CLD_SURF2),
    gsSPClearGeometryMode(G_CULL_BACK | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR | G_ZBUFFER),
    gsSPSetGeometryMode(G_FOG | G_LIGHTING),
    gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
    gsDPSetEnvColor(255, 255, 255, 255),
    gsSPVertex(&kGlitterVtx, 4, 0),
    gsSP2Triangles(0, 2, 1, 0, 0, 3, 2, 0),
    gsSPEndDisplayList(),
};

void EnWonderItem_DrawGlitter(Actor* this, GameState_Play* play)
{
    void* tex;

    /* Update the display list */
    tex = comboCacheGetFile(CUSTOM_GLITTER_ADDR);
    if (!tex)
        return;

    /* Prepare the Matrix */
    ModelViewTranslate(this->world.pos.x, this->world.pos.y, this->world.pos.z, MAT_SET);
    ModelViewUnkTransform(&play->billboardMtxF);
    //ModelViewTranslate(0, -60.f, -15.f, MAT_MUL);

    /* Draw the display list */
    OPEN_DISPS(play->gs.gfx);
    gSPMatrix(POLY_XLU_DISP++, GetMatrixMV(play->gs.gfx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPSegment(POLY_XLU_DISP++, 0x06, (u32)tex - 0x80000000);
    gSPDisplayList(POLY_XLU_DISP++, (u32)kDlistGlitter - 0x80000000);
    CLOSE_DISPS();
}

void EnWonderItem_InitWrapper(Actor* this, GameState_Play* play)
{
    void (*EnWonderItem_Init)(Actor*, GameState_Play*);

    /* Forward */
    EnWonderItem_Init = actorAddr(AC_EN_WONDER_ITEM, 0x80a663b0);
    EnWonderItem_Init(this, play);

    /* Use draw */
    this->draw = EnWonderItem_DrawGlitter;
}
