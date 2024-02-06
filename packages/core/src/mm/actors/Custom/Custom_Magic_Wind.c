/*
 * File: z_magic_wind.c
 * Overlay: ovl_Magic_Wind
 * Description: Farore's Wind
 */

#include "Custom_Magic_Wind.h"
#include <combo/custom.h>

#define FLAGS ((1 << 4) | (1 << 25)) // (ACTOR_FLAG_4 | ACTOR_FLAG_25)

void MagicWind_Init(Actor* thisx, GameState_Play* play);
void MagicWind_Destroy(Actor* thisx, GameState_Play* play);
void MagicWind_Update(Actor* thisx, GameState_Play* play);
void MagicWind_Draw(Actor* thisx, GameState_Play* play);

void MagicWind_Shrink(MagicWind* this, GameState_Play* play);
void MagicWind_WaitForTimer(MagicWind* this, GameState_Play* play);
void MagicWind_FadeOut(MagicWind* this, GameState_Play* play);
void MagicWind_WaitAtFullSize(MagicWind* this, GameState_Play* play);
void MagicWind_Grow(MagicWind* this, GameState_Play* play);

ActorInit Magic_Wind_InitVars = {
    /**/ AC_CUSTOM_SPELL_WIND,
    /**/ ACTORCAT_ITEMACTION,
    /**/ FLAGS,
    /**/ 1, // OBJECT_GAMEPLAY_KEEP
    /**/ sizeof(MagicWind),
    /**/ MagicWind_Init,
    /**/ MagicWind_Destroy,
    /**/ MagicWind_Update,
    /**/ MagicWind_Draw,
};

static Vtx sCylinderVtx[36] = {
#include "sCylinderVtx.vtx.inc"
};

static Gfx sInnerCylinderDL[33] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(0x06000000 | CUSTOM_KEEP_MAGIC_WIND_TEXTURE, G_IM_FMT_I, G_IM_SIZ_8b, 64, 64, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP,
                         6, 6, G_TX_NOLOD, G_TX_NOLOD),
    gsDPLoadMultiBlock(0x06000000 | CUSTOM_KEEP_MAGIC_WIND_TEXTURE, 0x0100, 1, G_IM_FMT_I, G_IM_SIZ_8b, 64, 64, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR |
                       G_TX_WRAP, 6, 6, 14, 14),
    gsDPSetCombineLERP(TEXEL1, PRIMITIVE, PRIM_LOD_FRAC, TEXEL0, TEXEL1, TEXEL0, PRIM_LOD_FRAC, TEXEL0, PRIMITIVE,
                       ENVIRONMENT, COMBINED, ENVIRONMENT, COMBINED, 0, SHADE, 0),
    gsDPSetRenderMode(G_RM_PASS, G_RM_AA_ZB_XLU_SURF2),
    gsSPClearGeometryMode(G_CULL_BACK | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsDPSetPrimColor(0, 0x80, 255, 255, 170, 255),
    gsDPSetEnvColor(150, 255, 0, 0),
    gsSPDisplayList(0x08000000),
    gsSPVertex(sCylinderVtx, 18, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 3, 1, 0),
    gsSP2Triangles(4, 5, 6, 0, 4, 7, 5, 0),
    gsSP2Triangles(3, 8, 1, 0, 3, 9, 8, 0),
    gsSP2Triangles(10, 2, 11, 0, 10, 0, 2, 0),
    gsSP2Triangles(9, 12, 8, 0, 9, 13, 12, 0),
    gsSP2Triangles(7, 14, 5, 0, 7, 15, 14, 0),
    gsSP2Triangles(15, 11, 14, 0, 15, 10, 11, 0),
    gsSP2Triangles(13, 16, 12, 0, 13, 17, 16, 0),
    gsSPEndDisplayList(),
};

static Gfx sOuterCylinderDL[33] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(0x06000000 | CUSTOM_KEEP_MAGIC_WIND_TEXTURE, G_IM_FMT_I, G_IM_SIZ_8b, 64, 64, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP,
                         6, 6, G_TX_NOLOD, G_TX_NOLOD),
    gsDPLoadMultiBlock(0x06000000 | CUSTOM_KEEP_MAGIC_WIND_TEXTURE, 0x0100, 1, G_IM_FMT_I, G_IM_SIZ_8b, 64, 64, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR |
                       G_TX_WRAP, 6, 6, 15, 15),
    gsDPSetCombineLERP(TEXEL1, PRIMITIVE, PRIM_LOD_FRAC, TEXEL0, TEXEL1, TEXEL0, PRIM_LOD_FRAC, TEXEL0, PRIMITIVE,
                       ENVIRONMENT, COMBINED, ENVIRONMENT, COMBINED, 0, SHADE, 0),
    gsDPSetRenderMode(G_RM_PASS, G_RM_AA_ZB_XLU_SURF2),
    gsSPClearGeometryMode(G_CULL_BACK | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsDPSetPrimColor(0, 0x80, 255, 255, 170, 255),
    gsDPSetEnvColor(0, 150, 0, 0),
    gsSPDisplayList(0x09000000),
    gsSPVertex(&sCylinderVtx[18], 18, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 3, 1, 0),
    gsSP2Triangles(4, 5, 6, 0, 4, 7, 5, 0),
    gsSP2Triangles(3, 8, 1, 0, 3, 9, 8, 0),
    gsSP2Triangles(10, 2, 11, 0, 10, 0, 2, 0),
    gsSP2Triangles(9, 12, 8, 0, 9, 13, 12, 0),
    gsSP2Triangles(7, 14, 5, 0, 7, 15, 14, 0),
    gsSP2Triangles(15, 11, 14, 0, 15, 10, 11, 0),
    gsSP2Triangles(13, 16, 12, 0, 13, 17, 16, 0),
    gsSPEndDisplayList(),
};

static u8 ovl_Magic_WindCurveAnime_Ref_001BD0[27] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
};

static s16 ovl_Magic_WindCurveAnime_Copy_001BEC[25] = {
      1024,   1024,   1024,      0,      0,      0,      0,      0,
         0,    512,    512,      0,      0,      0,      0,      0,
         0,    717,    717,      0,      0,      0,      0,      0,
         0,
};

static CurveInterpKnot ovl_Magic_WindCurveAnime_CurveInterpKnot_001C20[4] = {
    { 0x000C, 0x0001, 1, 1, 0.000000f },
    { 0x0014, 0x003C, 0, 0, 1.500000f },
    { 0x000C, 0x0001, 1, 1, 0.000000f },
    { 0x0014, 0x003C, 0, 0, 1.000000f },
};

static CurveAnimationHeader sAnim = {
    ovl_Magic_WindCurveAnime_Ref_001BD0,
    ovl_Magic_WindCurveAnime_CurveInterpKnot_001C20,
    ovl_Magic_WindCurveAnime_Copy_001BEC,
    1, 60
};

static SkelCurveLimb sRootLimb = {
    0x01, LIMB_DONE,
    { NULL, NULL }
};

static SkelCurveLimb sInnerCylinderLimb = {
    LIMB_DONE, 0x02,
    { NULL, sInnerCylinderDL }
};

static SkelCurveLimb sOuterCylinderLimb = {
    LIMB_DONE, LIMB_DONE,
    { NULL, sOuterCylinderDL }
};

static SkelCurveLimb* sSkelLimbs[3] = {
    &sRootLimb,
    &sInnerCylinderLimb,
    &sOuterCylinderLimb,
};

static CurveSkeletonHeader sSkel = {
    sSkelLimbs, ARRAY_SIZE(sSkelLimbs)
};

static u8 sAlphaUpdVals[] = {
    0x00, 0x03, 0x04, 0x07, 0x09, 0x0A, 0x0D, 0x0F, 0x11, 0x12, 0x15, 0x16, 0x19, 0x1B, 0x1C, 0x1F, 0x21, 0x23,
};

void MagicWind_SetupAction(MagicWind* this, MagicWindFunc actionFunc) {
    this->actionFunc = actionFunc;
}

void MagicWind_Init(Actor* thisx, GameState_Play* play) {
    MagicWind* this = (MagicWind*)thisx;
    Actor_Player* player = GET_LINK(play);

    if (!SkelCurve_Init(play, &this->skelCurve, &sSkel, &sAnim)) {
        // "Magic_Wind_Actor_ct (): Construct failed"
    }
    this->actor.room = -1;
    switch (this->actor.variable) {
        case 0:
            SkelCurve_SetAnim(&this->skelCurve, &sAnim, 0.0f, 60.0f, 0.0f, 1.0f);
            this->timer = 29;
            MagicWind_SetupAction(this, MagicWind_WaitForTimer);
            break;
        case 1:
            SkelCurve_SetAnim(&this->skelCurve, &sAnim, 60.0f, 0.0f, 60.0f, -1.0f);
            MagicWind_SetupAction(this, MagicWind_Shrink);
            Player_PlaySfx(player, 0x87B); // NA_SE_PL_MAGIC_WIND_WARP
            break;
    }
}

void MagicWind_Destroy(Actor* thisx, GameState_Play* play) {
    MagicWind* this = (MagicWind*)thisx;
    SkelCurve_Destroy(play, &this->skelCurve);
    Magic_Reset(play);
}

void MagicWind_UpdateAlpha(f32 alpha) {
    s32 i;

    for (i = 0; i < ARRAY_SIZE(sAlphaUpdVals); i++) {
        sCylinderVtx[sAlphaUpdVals[i]].n.a = alpha * 255.0f;
    }
}

void MagicWind_WaitForTimer(MagicWind* this, GameState_Play* play) {
    Actor_Player* player = GET_LINK(play);

    if (this->timer > 0) {
        this->timer--;
        return;
    }

    Player_PlaySfx(player, 0x87A); // NA_SE_PL_MAGIC_WIND_NORMAL
    MagicWind_UpdateAlpha(1.0f);
    MagicWind_SetupAction(this, MagicWind_Grow);
    SkelCurve_Update(play, &this->skelCurve);
}

void MagicWind_Grow(MagicWind* this, GameState_Play* play) {
    if (SkelCurve_Update(play, &this->skelCurve)) {
        MagicWind_SetupAction(this, MagicWind_WaitAtFullSize);
        this->timer = 50;
    }
}

void MagicWind_WaitAtFullSize(MagicWind* this, GameState_Play* play) {
    if (this->timer > 0) {
        this->timer--;
    } else {
        MagicWind_SetupAction(this, MagicWind_FadeOut);
        this->timer = 30;
    }
}

void MagicWind_FadeOut(MagicWind* this, GameState_Play* play) {
    if (this->timer > 0) {
        MagicWind_UpdateAlpha((f32)this->timer * (1.0f / 30.0f));
        this->timer--;
    } else {
        ActorDestroy(&this->actor);
    }
}

void MagicWind_Shrink(MagicWind* this, GameState_Play* play) {
    if (SkelCurve_Update(play, &this->skelCurve)) {
        ActorDestroy(&this->actor);
    }
}

void MagicWind_Update(Actor* thisx, GameState_Play* play) {
    MagicWind* this = (MagicWind*)thisx;

    // See `ACTOROVL_ALLOC_ABSOLUTE`
    //! @bug This condition is too broad, the actor will also be killed by warp songs. But warp songs do not use an
    //! actor which uses `ACTOROVL_ALLOC_ABSOLUTE`. There is no reason to kill the actor in this case.
    // if ((play->msgCtx.msgMode == MSGMODE_OCARINA_CORRECT_PLAYBACK) || (play->msgCtx.msgMode == MSGMODE_SONG_PLAYED)) {
    //     ActorDestroy(thisx);
    //     return;
    // }

    this->actionFunc(this, play);
}

s32 MagicWind_OverrideLimbDraw(GameState_Play* play, SkelCurve* skelCurve, s32 limbIndex, void* thisx)
{
    OPEN_DISPS(play->gs.gfx);

    if (limbIndex == 1) {
        gSPSegment(POLY_XLU_DISP++, 8,
                   DisplaceTexture(play->gs.gfx, G_TX_RENDERTILE, (play->gs.frameCount * 9) & 0xFF,
                                    0xFF - ((play->gs.frameCount * 0xF) & 0xFF), 0x40, 0x40, 1,
                                    (play->gs.frameCount * 0xF) & 0xFF, 0xFF - ((play->gs.frameCount * 0x1E) & 0xFF),
                                    0x40, 0x40));

    } else if (limbIndex == 2) {
        gSPSegment(POLY_XLU_DISP++, 9,
                   DisplaceTexture(play->gs.gfx, G_TX_RENDERTILE, (play->gs.frameCount * 3) & 0xFF,
                                    0xFF - ((play->gs.frameCount * 5) & 0xFF), 0x40, 0x40, 1,
                                    (play->gs.frameCount * 6) & 0xFF, 0xFF - ((play->gs.frameCount * 0xA) & 0xFF), 0x40,
                                    0x40));
    }

    CLOSE_DISPS();

    return 1;
}

void MagicWind_Draw(Actor* thisx, GameState_Play* play) {
    GfxContext* gfxCtx = play->gs.gfx;
    MagicWind* this = (MagicWind*)thisx;

    OPEN_DISPS(gfxCtx);

    gSPSegment(POLY_XLU_DISP++, 0x06, gCustomKeep);

    if (this->actionFunc != MagicWind_WaitForTimer) {
        POLY_XLU_DISP = Gfx_SetupDL(POLY_XLU_DISP, 25); // SETUPDL_25
        SkelCurve_Draw(&this->actor, play, &this->skelCurve, (void*)MagicWind_OverrideLimbDraw, NULL, 1, NULL);
    }

    CLOSE_DISPS();
}
