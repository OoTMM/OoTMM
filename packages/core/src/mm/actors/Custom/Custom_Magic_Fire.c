/*
 * File: z_magic_fire.c
 * Overlay: ovl_Magic_Fire
 * Description: Din's Fire
 */

#include "Custom_Magic_Fire.h"
#include <combo/custom.h>

#define FLAGS ((1 << 4) | (1 << 25)) /* (ACTOR_FLAG_4 | ACTOR_FLAG_25) */

void MagicFire_Init(Actor* thisx, GameState_Play* play);
void MagicFire_Destroy(Actor* thisx, GameState_Play* play);
void MagicFire_Update(Actor* thisx, GameState_Play* play);
void MagicFire_Draw(Actor* thisx, GameState_Play* play);

void MagicFire_UpdateBeforeCast(Actor* thisx, GameState_Play* play);

typedef enum {
    /* 0x00 */ DF_ACTION_INITIALIZE,
    /* 0x01 */ DF_ACTION_EXPAND_SLOWLY,
    /* 0x02 */ DF_ACTION_STOP_EXPANDING,
    /* 0x03 */ DF_ACTION_EXPAND_QUICKLY
} MagicFireAction;

typedef enum {
    /* 0x00 */ DF_SCREEN_TINT_NONE,
    /* 0x01 */ DF_SCREEN_TINT_FADE_IN,
    /* 0x02 */ DF_SCREEN_TINT_MAINTAIN,
    /* 0x03 */ DF_SCREEN_TINT_FADE_OUT,
    /* 0x04 */ DF_SCREEN_TINT_FINISHED
} MagicFireScreenTint;

ActorInit Magic_Fire_InitVars = {
    /**/ AC_CUSTOM_SPELL_FIRE,
    /**/ ACTORCAT_ITEMACTION,
    /**/ FLAGS,
    /**/ 1, /* OBJECT_GAMEPLAY_KEEP */
    /**/ sizeof(MagicFire),
    /**/ MagicFire_Init,
    /**/ MagicFire_Destroy,
    /**/ MagicFire_Update,
    /**/ MagicFire_Draw,
};

static Vtx sSphereVtx[76] = {
#include "sSphereVtx.vtx.inc"
};

static Gfx sMaterialDL[5] = {
    gsDPPipeSync(),
    gsDPSetCombineLERP(TEXEL1, PRIMITIVE, PRIM_LOD_FRAC, TEXEL0, TEXEL1, 1, PRIM_LOD_FRAC, TEXEL0, PRIMITIVE,
                       ENVIRONMENT, COMBINED, ENVIRONMENT, COMBINED, 0, SHADE, 0),
    gsDPSetRenderMode(G_RM_PASS, G_RM_AA_ZB_XLU_SURF2),
    gsSPClearGeometryMode(G_CULL_BACK | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPEndDisplayList(),
};

static Gfx sModelDL[45] = {
    gsSPVertex(sSphereVtx, 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 1, 0, 0),
    gsSP2Triangles(3, 4, 1, 0, 5, 4, 3, 0),
    gsSP2Triangles(6, 7, 8, 0, 9, 7, 6, 0),
    gsSP2Triangles(9, 10, 7, 0, 11, 10, 9, 0),
    gsSP2Triangles(11, 12, 10, 0, 12, 11, 13, 0),
    gsSP2Triangles(14, 0, 15, 0, 16, 0, 14, 0),
    gsSP2Triangles(16, 3, 0, 0, 17, 3, 16, 0),
    gsSP2Triangles(17, 5, 3, 0, 18, 5, 17, 0),
    gsSP2Triangles(18, 19, 5, 0, 20, 19, 18, 0),
    gsSP2Triangles(21, 11, 9, 0, 11, 21, 22, 0),
    gsSP2Triangles(23, 14, 24, 0, 25, 14, 23, 0),
    gsSP2Triangles(25, 16, 14, 0, 26, 16, 25, 0),
    gsSP2Triangles(26, 17, 16, 0, 27, 17, 26, 0),
    gsSP2Triangles(27, 18, 17, 0, 28, 18, 27, 0),
    gsSP2Triangles(28, 20, 18, 0, 20, 28, 29, 0),
    gsSP1Triangle(30, 23, 31, 0),
    gsSPVertex(&sSphereVtx[32], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 3, 1, 0),
    gsSP2Triangles(4, 3, 0, 0, 4, 5, 3, 0),
    gsSP2Triangles(6, 5, 4, 0, 6, 7, 5, 0),
    gsSP2Triangles(8, 7, 6, 0, 8, 9, 7, 0),
    gsSP2Triangles(9, 8, 10, 0, 11, 2, 12, 0),
    gsSP2Triangles(13, 2, 11, 0, 13, 0, 2, 0),
    gsSP2Triangles(14, 0, 13, 0, 14, 4, 0, 0),
    gsSP2Triangles(15, 4, 14, 0, 15, 6, 4, 0),
    gsSP2Triangles(16, 6, 15, 0, 16, 8, 6, 0),
    gsSP2Triangles(8, 16, 17, 0, 18, 11, 19, 0),
    gsSP2Triangles(20, 11, 18, 0, 20, 13, 11, 0),
    gsSP2Triangles(21, 13, 20, 0, 21, 14, 13, 0),
    gsSP2Triangles(22, 14, 21, 0, 22, 15, 14, 0),
    gsSP2Triangles(23, 15, 22, 0, 23, 16, 15, 0),
    gsSP2Triangles(16, 23, 24, 0, 25, 18, 26, 0),
    gsSP2Triangles(27, 18, 25, 0, 27, 20, 18, 0),
    gsSP2Triangles(28, 20, 27, 0, 28, 21, 20, 0),
    gsSP2Triangles(29, 21, 28, 0, 29, 22, 21, 0),
    gsSP2Triangles(30, 22, 29, 0, 30, 23, 22, 0),
    gsSP1Triangle(23, 30, 31, 0),
    gsSPVertex(&sSphereVtx[64], 12, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 1, 0, 0),
    gsSP2Triangles(3, 4, 1, 0, 5, 4, 3, 0),
    gsSP2Triangles(5, 6, 4, 0, 7, 6, 5, 0),
    gsSP2Triangles(7, 8, 6, 0, 9, 8, 7, 0),
    gsSP2Triangles(9, 10, 8, 0, 10, 9, 11, 0),
    gsSPEndDisplayList(),
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_PLAYER,
        AC_NONE,
        OC1_NONE,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000800, 0x00, 0x02 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NONE,
        BUMP_NONE,
        OCELEM_NONE,
    },
    { 9, 9, 0, { 0, 0, 0 } },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F(scale, 0, ICHAIN_STOP),
};

static u8 sVertexIndices[] = {
    3,  4,  5,  6,  7,  8,  9,  10, 16, 17, 18, 19, 25, 26, 27, 32, 35, 36, 37, 38,
    39, 45, 46, 47, 52, 53, 54, 59, 60, 61, 67, 68, 69, 70, 71, 72, 0,  1,  11, 12,
    14, 20, 21, 23, 28, 30, 33, 34, 40, 41, 43, 48, 50, 55, 57, 62, 64, 65, 73, 74,
};

void MagicFire_Init(Actor* thisx, GameState_Play* play) {
    MagicFire* this = (MagicFire*)thisx;

    Actor_ProcessInitChain(&this->actor, sInitChain);
    this->action = 0;
    this->screenTintBehaviour = 0;
    this->actionTimer = 0;
    this->alphaMultiplier = -3.0f;
    ActorSetScale(&this->actor, 0.0f);
    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    Collider_UpdateCylinder(&this->actor, &this->collider);
    this->actor.update = MagicFire_UpdateBeforeCast;
    this->actionTimer = 20;
    this->actor.room = -1;
}

void MagicFire_Destroy(Actor* thisx, GameState_Play* play) {
    Magic_Reset(play);
}

void MagicFire_UpdateBeforeCast(Actor* thisx, GameState_Play* play) {
    MagicFire* this = (MagicFire*)thisx;
    Actor_Player* player = GET_LINK(play);

    /* See `ACTOROVL_ALLOC_ABSOLUTE` */
    /*! @bug This condition is too broad, the actor will also be killed by warp songs. But warp songs do not use an */
    /*! actor which uses `ACTOROVL_ALLOC_ABSOLUTE`. There is no reason to kill the actor in this case. */
    /* if ((play->msgCtx.msgMode == MSGMODE_OCARINA_CORRECT_PLAYBACK) || (play->msgCtx.msgMode == MSGMODE_SONG_PLAYED)) { */
    /*     Actor_Kill(&this->actor); */
    /*     return; */
    /* } */

    if (this->actionTimer > 0) {
        this->actionTimer--;
    } else {
        this->actor.update = MagicFire_Update;
        Player_PlaySfx(player, 0x879); /* NA_SE_PL_MAGIC_FIRE */
    }
    this->actor.world.pos = player->base.world.pos;
}

void MagicFire_Update(Actor* thisx, GameState_Play* play) {
    MagicFire* this = (MagicFire*)thisx;
    Actor_Player* player = GET_LINK(play);

    this->actor.world.pos = player->base.world.pos;

    /* See `ACTOROVL_ALLOC_ABSOLUTE` */
    /*! @bug This condition is too broad, the actor will also be killed by warp songs. But warp songs do not use an */
    /*! actor which uses `ACTOROVL_ALLOC_ABSOLUTE`. There is no reason to kill the actor in this case. */
    /* if ((play->msgCtx.msgMode == MSGMODE_OCARINA_CORRECT_PLAYBACK) || (play->msgCtx.msgMode == MSGMODE_SONG_PLAYED)) { */
    /*     Actor_Kill(&this->actor); */
    /*     return; */
    /* } */

    Collider_UpdateCylinder(&this->actor, &this->collider);
    this->collider.dim.radius = (this->actor.scale.x * 325.0f);
    this->collider.dim.height = (this->actor.scale.y * 450.0f);
    this->collider.dim.yShift = (this->actor.scale.y * -225.0f);
    CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);

    switch (this->action) {
        case DF_ACTION_INITIALIZE:
            this->actionTimer = 30;
            this->actor.scale.x = this->actor.scale.y = this->actor.scale.z = 0.0f;
            this->actor.world.rot.x = this->actor.world.rot.y = this->actor.world.rot.z = 0;
            this->actor.rot2.x = this->actor.rot2.y = this->actor.rot2.z = 0;
            this->alphaMultiplier = 0.0f;
            this->scalingSpeed = 0.08f;
            this->action++;
            break;
        case DF_ACTION_EXPAND_SLOWLY: /* Fire sphere slowly expands out of player for 30 frames */
            Math_StepToF(&this->alphaMultiplier, 1.0f, 1.0f / 30.0f);
            if (this->actionTimer > 0) {
                Math_SmoothStepToF(&this->actor.scale.x, 0.4f, this->scalingSpeed, 0.1f, 0.001f);
                this->actor.scale.y = this->actor.scale.z = this->actor.scale.x;
            } else {
                this->actionTimer = 25;
                this->action++;
            }
            break;
        case DF_ACTION_STOP_EXPANDING: /* Sphere stops expanding and maintains size for 25 frames */
            if (this->actionTimer <= 0) {
                this->actionTimer = 15;
                this->action++;
                this->scalingSpeed = 0.05f;
            }
            break;
        case DF_ACTION_EXPAND_QUICKLY: /* Sphere beings to grow again and quickly expands out until killed */
            this->alphaMultiplier -= 8.0f / 119.00001f;
            this->actor.scale.x += this->scalingSpeed;
            this->actor.scale.y += this->scalingSpeed;
            this->actor.scale.z += this->scalingSpeed;
            if (this->alphaMultiplier <= 0.0f) {
                this->action = 0;
                ActorDestroy(&this->actor);
            }
            break;
    }
    switch (this->screenTintBehaviour) {
        case DF_SCREEN_TINT_NONE:
            if (this->screenTintBehaviourTimer <= 0) {
                this->screenTintBehaviourTimer = 20;
                this->screenTintBehaviour = DF_SCREEN_TINT_FADE_IN;
            }
            break;
        case DF_SCREEN_TINT_FADE_IN:
            this->screenTintIntensity = 1.0f - (this->screenTintBehaviourTimer / 20.0f);
            if (this->screenTintBehaviourTimer <= 0) {
                this->screenTintBehaviourTimer = 45;
                this->screenTintBehaviour = DF_SCREEN_TINT_MAINTAIN;
            }
            break;
        case DF_SCREEN_TINT_MAINTAIN:
            if (this->screenTintBehaviourTimer <= 0) {
                this->screenTintBehaviourTimer = 5;
                this->screenTintBehaviour = DF_SCREEN_TINT_FADE_OUT;
            }
            break;
        case DF_SCREEN_TINT_FADE_OUT:
            this->screenTintIntensity = (this->screenTintBehaviourTimer / 5.0f);
            if (this->screenTintBehaviourTimer <= 0) {
                this->screenTintBehaviour = DF_SCREEN_TINT_FINISHED;
            }
            break;
    }
    if (this->actionTimer > 0) {
        this->actionTimer--;
    }
    if (this->screenTintBehaviourTimer > 0) {
        this->screenTintBehaviourTimer--;
    }
}

void MagicFire_Draw(Actor* thisx, GameState_Play* play) {
    MagicFire* this = (MagicFire*)thisx;
    u32 gameplayFrames = play->gameplayFrames;
    s32 i;
    u8 alpha;

    if (this->action > 0) {
        OPEN_DISPS(play->gs.gfx);
        gSPSegment(POLY_XLU_DISP++, 0x08, gCustomKeep);
        POLY_XLU_DISP = Gfx_SetupDL57(POLY_XLU_DISP);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, (u8)(s32)(60 * this->screenTintIntensity),
                        (u8)(s32)(20 * this->screenTintIntensity), (u8)(s32)(0 * this->screenTintIntensity),
                        (u8)(s32)(120 * this->screenTintIntensity));
        gDPSetAlphaDither(POLY_XLU_DISP++, G_AD_DISABLE);
        gDPSetColorDither(POLY_XLU_DISP++, G_CD_DISABLE);
        gDPFillRectangle(POLY_XLU_DISP++, 0, 0, 319, 239);
        InitListPolyXlu(play->gs.gfx);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 128, 255, 200, 0, (u8)(this->alphaMultiplier * 255));
        gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 0, (u8)(this->alphaMultiplier * 255));
        ModelViewScale(0.15f, 0.15f, 0.15f, MAT_MUL);
        gSPMatrix(POLY_XLU_DISP++, GetMatrixMV(play->gs.gfx),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gDPPipeSync(POLY_XLU_DISP++);
        gSPTexture(POLY_XLU_DISP++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
        gDPSetTextureLUT(POLY_XLU_DISP++, G_TT_NONE);
        gDPLoadTextureBlock(POLY_XLU_DISP++, 0x08000000 | CUSTOM_KEEP_MAGIC_FIRE_TEXTURE, G_IM_FMT_I, G_IM_SIZ_8b, 64, 64, 0, G_TX_NOMIRROR | G_TX_WRAP,
                            G_TX_NOMIRROR | G_TX_WRAP, 6, 6, 15, G_TX_NOLOD);
        gDPSetTile(POLY_XLU_DISP++, G_IM_FMT_I, G_IM_SIZ_8b, 8, 0, 1, 0, G_TX_NOMIRROR | G_TX_WRAP, 6, 14,
                   G_TX_NOMIRROR | G_TX_WRAP, 6, 14);
        gDPSetTileSize(POLY_XLU_DISP++, 1, 0, 0, 63 << 2, 63 << 2);
        gSPDisplayList(POLY_XLU_DISP++, sMaterialDL);
        gSPDisplayList(POLY_XLU_DISP++,
                       DisplaceTexture(play->gs.gfx, G_TX_RENDERTILE, (gameplayFrames * 2) % 512,
                                        511 - ((gameplayFrames * 5) % 512), 64, 64, 1, (gameplayFrames * 2) % 256,
                                        255 - ((gameplayFrames * 20) % 256), 32, 32));
        gSPDisplayList(POLY_XLU_DISP++, sModelDL);
        CLOSE_DISPS();

        alpha = (s32)(this->alphaMultiplier * 255);
        for (i = 0; i < 36; i++) {
            sSphereVtx[sVertexIndices[i]].n.a = alpha;
        }

        alpha = (s32)(this->alphaMultiplier * 76);
        for (i = 36; i < 60; i++) {
            sSphereVtx[sVertexIndices[i]].n.a = alpha;
        }
    }
}
