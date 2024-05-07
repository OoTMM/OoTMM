#include <combo.h>
#include <combo/custom.h>
#include <combo/global.h>

const Vtx sGauntletVtx[] = {
#include "data/gauntlets.vtx.inc"
};

const u32 gLinkAdultGauntletPlate1Tex = 0x08000000 | CUSTOM_KEEP_GAUNTLET1_TEXTURE;

Gfx gLinkAdultLeftGauntletPlate1DL[] = {
    gsSPMatrix(0x0D0002C0, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(gLinkAdultGauntletPlate1Tex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 32, 0, G_TX_MIRROR | G_TX_CLAMP,
                         G_TX_MIRROR | G_TX_CLAMP, 4, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, COMBINED, 0, ENVIRONMENT, 0, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsSPClearGeometryMode(G_CULL_BOTH | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_FOG | G_LIGHTING),
    gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
    gsSPVertex(&sGauntletVtx[0], 24, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSP2Triangles(4, 5, 6, 0, 5, 4, 7, 0),
    gsSP2Triangles(8, 9, 10, 0, 11, 12, 13, 0),
    gsSP2Triangles(11, 13, 14, 0, 8, 15, 9, 0),
    gsSP2Triangles(16, 17, 18, 0, 19, 20, 21, 0),
    gsSP2Triangles(18, 22, 16, 0, 19, 21, 23, 0),
    gsSPEndDisplayList(),
};

const u32 gLinkAdultGauntletPlate2Tex = 0x08000000 | CUSTOM_KEEP_GAUNTLET2_TEXTURE;

const u32 gHilite2Tex = 0x04000000 | 0xca30;

/* gLinkAdultLeftGauntletPlate2DL is omitted because we always render the "closed hand" variant so it doesn't clip into the link's hand */

Gfx gLinkAdultLeftGauntletPlate3DL[] = {
    gsSPMatrix(0x0D000300, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(gLinkAdultGauntletPlate2Tex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_NOMIRROR |
                         G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, COMBINED, 0, ENVIRONMENT, 0, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsSPClearGeometryMode(G_CULL_BOTH | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_FOG | G_LIGHTING),
    gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
    gsSPVertex(&sGauntletVtx[24], 26, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSP2Triangles(4, 5, 6, 0, 7, 8, 9, 0),
    gsSP2Triangles(10, 11, 12, 0, 6, 5, 13, 0),
    gsSP2Triangles(2, 1, 14, 0, 11, 15, 12, 0),
    gsSP2Triangles(15, 11, 16, 0, 8, 7, 17, 0),
    gsSP2Triangles(18, 19, 20, 0, 20, 21, 18, 0),
    gsSP2Triangles(22, 23, 24, 0, 24, 25, 22, 0),
    gsDPPipeSync(),
    gsSPTexture(0x2710, 0x2710, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(gHilite2Tex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 16, 0, G_TX_MIRROR | G_TX_WRAP, G_TX_MIRROR |
                         G_TX_WRAP, 4, 4, 1, 15),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, 1, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsSPSetGeometryMode(G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPDisplayList(0x0C000000),
    gsDPSetPrimColor(0, 0, 254, 15, 15, 255),
    gsSPVertex(&sGauntletVtx[50], 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPVertex(&sGauntletVtx[53], 9, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP1Triangle(6, 7, 8, 0),
    gsSPEndDisplayList(),
};

Gfx gLinkAdultRightGauntletPlate1DL[] = {
    gsSPMatrix(0x0D000380, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(gLinkAdultGauntletPlate1Tex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 32, 0, G_TX_MIRROR | G_TX_CLAMP,
                         G_TX_MIRROR | G_TX_CLAMP, 4, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, COMBINED, 0, ENVIRONMENT, 0, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsSPClearGeometryMode(G_CULL_BOTH | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_FOG | G_LIGHTING),
    gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
    gsSPVertex(&sGauntletVtx[62], 24, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 0, 2, 0),
    gsSP2Triangles(4, 5, 6, 0, 7, 6, 5, 0),
    gsSP2Triangles(8, 9, 10, 0, 11, 12, 13, 0),
    gsSP2Triangles(14, 11, 13, 0, 9, 15, 10, 0),
    gsSP2Triangles(16, 17, 18, 0, 19, 20, 21, 0),
    gsSP2Triangles(18, 22, 16, 0, 23, 19, 21, 0),
    gsSPEndDisplayList(),
};

/* gLinkAdultRightGauntletPlate2DL is omitted because we always render the "closed hand" variant so it doesn't clip into the link's hand */

Gfx gLinkAdultRightGauntletPlate3DL[] = {
    gsSPMatrix(0x0D0003C0, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(gLinkAdultGauntletPlate2Tex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_NOMIRROR |
                         G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, COMBINED, 0, ENVIRONMENT, 0, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsSPClearGeometryMode(G_CULL_BOTH | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_FOG | G_LIGHTING),
    gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
    gsSPVertex(&sGauntletVtx[86], 26, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 0, 2, 0),
    gsSP2Triangles(4, 5, 6, 0, 7, 8, 9, 0),
    gsSP2Triangles(10, 11, 12, 0, 13, 5, 4, 0),
    gsSP2Triangles(14, 1, 0, 0, 10, 15, 11, 0),
    gsSP2Triangles(16, 11, 15, 0, 17, 9, 8, 0),
    gsSP2Triangles(18, 19, 20, 0, 20, 21, 18, 0),
    gsSP2Triangles(22, 23, 24, 0, 24, 25, 22, 0),
    gsDPPipeSync(),
    gsSPTexture(0x2710, 0x2710, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(gHilite2Tex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 16, 0, G_TX_MIRROR | G_TX_WRAP, G_TX_MIRROR |
                         G_TX_WRAP, 4, 4, 1, 15),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, 1, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsSPSetGeometryMode(G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPDisplayList(0x0C000000),
    gsDPSetPrimColor(0, 0, 254, 15, 15, 255),
    gsSPVertex(&sGauntletVtx[112], 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPVertex(&sGauntletVtx[115], 9, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP1Triangle(6, 7, 8, 0),
    gsSPEndDisplayList(),
};

void DrawChildGauntlets(GameState_Play* play)
{
    OPEN_DISPS(play->gs.gfx);
    gSPSegment(POLY_OPA_DISP++, 0x08, g.customKeep);
    gSPDisplayList(POLY_OPA_DISP++, gLinkAdultLeftGauntletPlate1DL);
    gSPDisplayList(POLY_OPA_DISP++, gLinkAdultRightGauntletPlate1DL);
    gSPDisplayList(POLY_OPA_DISP++, gLinkAdultLeftGauntletPlate3DL);
    gSPDisplayList(POLY_OPA_DISP++, gLinkAdultRightGauntletPlate3DL);
    CLOSE_DISPS();
}
