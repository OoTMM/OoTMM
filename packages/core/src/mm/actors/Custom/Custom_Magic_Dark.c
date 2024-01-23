/*
 * File: z_magic_dark.c
 * Overlay: ovl_Magic_Dark
 * Description: Nayru's Love
 */

#include "Custom_Magic_Dark.h"
#include <combo/custom.h>

#define FLAGS ((1 << 4) | (1 << 25)) // (ACTOR_FLAG_4 | ACTOR_FLAG_25)

void MagicDark_Init(Actor* thisx, GameState_Play* play);
void MagicDark_Destroy(Actor* thisx, GameState_Play* play);
void MagicDark_OrbUpdate(Actor* thisx, GameState_Play* play);
void MagicDark_OrbDraw(Actor* thisx, GameState_Play* play);
void MagicDark_DiamondUpdate(Actor* thisx, GameState_Play* play);
void MagicDark_DiamondDraw(Actor* thisx, GameState_Play* play);

void MagicDark_DimLighting(GameState_Play* play, f32 intensity);

ActorInit Magic_Dark_InitVars = {
    /**/ AC_CUSTOM_SPELL_LOVE,
    /**/ ACTORCAT_ITEMACTION,
    /**/ FLAGS,
    /**/ 1, // OBJECT_GAMEPLAY_KEEP,
    /**/ sizeof(MagicDark),
    /**/ MagicDark_Init,
    /**/ MagicDark_Destroy,
    /**/ MagicDark_OrbUpdate,
    /**/ MagicDark_OrbDraw,
};

static Vtx sDiamondVtx[20] = {
#include "sDiamondVtx.vtx.inc"
};

static Gfx sDiamondMaterialDL[22] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(0x04000000 | 0x28BB8, G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR
                         | G_TX_WRAP, 5, 5, G_TX_NOLOD, 1),
    gsDPLoadMultiBlock(0x08000000 | CUSTOM_KEEP_MAGIC_DARK_TEXTURE, 0x0100, 1, G_IM_FMT_I, G_IM_SIZ_8b, 32, 64, 0, G_TX_NOMIRROR | G_TX_WRAP,
                       G_TX_MIRROR | G_TX_WRAP, 5, 6, 13, 13),
    gsDPSetCombineLERP(TEXEL1, PRIMITIVE, ENV_ALPHA, TEXEL0, TEXEL1, TEXEL0, ENVIRONMENT, TEXEL0, PRIMITIVE,
                       ENVIRONMENT, COMBINED, ENVIRONMENT, COMBINED, 0, PRIMITIVE, 0),
    gsDPSetRenderMode(G_RM_PASS, G_RM_AA_ZB_XLU_SURF2),
    gsSPClearGeometryMode(G_FOG | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_CULL_BACK | G_LIGHTING),
    gsSPEndDisplayList(),
};

static Gfx sDiamondModelDL[8] = {
    gsSPVertex(sDiamondVtx, 20, 0),
    gsSP2Triangles(0, 1, 2, 0, 1, 3, 4, 0),
    gsSP2Triangles(3, 5, 6, 0, 5, 7, 8, 0),
    gsSP2Triangles(7, 9, 10, 0, 11, 0, 12, 0),
    gsSP2Triangles(13, 0, 11, 0, 14, 7, 5, 0),
    gsSP2Triangles(15, 5, 3, 0, 16, 3, 1, 0),
    gsSP2Triangles(17, 1, 0, 0, 16, 18, 19, 0),
    gsSPEndDisplayList(),
};

static f32 MagicDark_GetScale(Actor_Player* player)
{
    switch (player->transformation)
    {
    case MM_PLAYER_FORM_FIERCE_DEITY:
        return 0.8f;
    case MM_PLAYER_FORM_GORON:
        return 0.6f;
    case MM_PLAYER_FORM_ZORA:
        return 0.6f;
    case MM_PLAYER_FORM_DEKU:
        return 0.4f;
    case MM_PLAYER_FORM_HUMAN:
        return 0.4f;
    }
    return 0.0f;
}

void MagicDark_Init(Actor* thisx, GameState_Play* play) {
    MagicDark* this = (MagicDark*)thisx;
    Actor_Player* player = GET_LINK(play);

    this->scale = MagicDark_GetScale(player);

    thisx->position = player->base.position;
    ActorSetScale(&this->actor, 0.0f);
    thisx->room = -1;

    if (gSaveContext.nayrusLoveTimer != 0) {
        thisx->update = MagicDark_DiamondUpdate;
        thisx->draw = MagicDark_DiamondDraw;
        thisx->scale.x = thisx->scale.z = this->scale * 1.6f;
        thisx->scale.y = this->scale * 0.8f;
        this->timer = 0;
        this->primAlpha = 0;
    } else {
        this->timer = 0;
        gSaveContext.nayrusLoveTimer = 0;
    }
}

void MagicDark_Destroy(Actor* thisx, GameState_Play* play) {
    if (gSaveContext.nayrusLoveTimer == 0) {
        Magic_Reset(play);
    }
}

void MagicDark_DiamondUpdate(Actor* thisx, GameState_Play* play) {
    MagicDark* this = (MagicDark*)thisx;
    u8 phi_a0;
    Actor_Player* player = GET_LINK(play);
    s16 nayrusLoveTimer = gSaveContext.nayrusLoveTimer;
    // s32 msgMode = play->msgCtx.msgMode;

    // See `ACTOROVL_ALLOC_ABSOLUTE`
    //! @bug This condition is too broad, the actor will also be killed by warp songs. But warp songs do not use an
    //! actor which uses `ACTOROVL_ALLOC_ABSOLUTE`. There is no reason to kill the actor in this case.
    //! This happens with all magic effects actors, but is especially visible with Nayru's Love as it lasts longer than
    //! other magic actors, and the Nayru's Love actor is supposed to be spawned back after ocarina effects actors are
    //! done. But with warp songs, whether the player warps away or not, the actor won't be spawned back.
    // if ((msgMode == MSGMODE_OCARINA_CORRECT_PLAYBACK) || (msgMode == MSGMODE_SONG_PLAYED)) {
    //     ActorDestroy(thisx);
    //     return;
    // }

    if (nayrusLoveTimer >= 1200) {
        player->invincibilityTimer = 0;
        gSaveContext.nayrusLoveTimer = 0;
        ActorDestroy(thisx);
        return;
    }

    f32 scaleTarget = MagicDark_GetScale(player);
    Math_StepToF(&this->scale, scaleTarget, 0.01f);

    player->invincibilityTimer = -100;
    thisx->scale.x = thisx->scale.z = this->scale;

    if (this->timer < 20) {
        thisx->scale.x = thisx->scale.z = (1.6f - (this->timer * 0.03f)) * this->scale;
        thisx->scale.y = ((this->timer * 0.01f) + 0.8f) * this->scale;
    } else {
        thisx->scale.x = thisx->scale.z = this->scale;
        thisx->scale.y = this->scale;
    }

    thisx->scale.x *= 1.3f;
    thisx->scale.z *= 1.3f;

    phi_a0 = (this->timer < 20) ? (this->timer * 12) : 255;

    if (nayrusLoveTimer >= 1180) {
        this->primAlpha = 15595 - (nayrusLoveTimer * 13);
        if (nayrusLoveTimer & 1) {
            this->primAlpha = this->primAlpha >> 1;
        }
    } else if (nayrusLoveTimer >= 1100) {
        this->primAlpha = (u8)(nayrusLoveTimer << 7) + 127;
    } else {
        this->primAlpha = 255;
    }

    if (this->primAlpha > phi_a0) {
        this->primAlpha = phi_a0;
    }

    thisx->speedRot.y += 0x3E8;
    thisx->rot2.y = thisx->position.y + Camera_GetCamDirYaw(GET_ACTIVE_CAM(play));
    this->timer++;
    gSaveContext.nayrusLoveTimer = nayrusLoveTimer + 1;

    if (nayrusLoveTimer < 1100) {
        PlayLoopingSfxAtActor(thisx, 0x7C); // NA_SE_PL_MAGIC_SOUL_NORMAL
    } else {
        PlayLoopingSfxAtActor(thisx, 0xC0); // NA_SE_PL_MAGIC_SOUL_FLASH
    }
}

void MagicDark_DimLighting(GameState_Play* play, f32 intensity) {
    s32 i;
    f32 colorScale;
    f32 fogScale;

    if (play->roomCtx.curRoom.behaviorType1 != ROOM_BEHAVIOR_TYPE1_5) {
        intensity = CLAMP_MIN(intensity, 0.0f);
        intensity = CLAMP_MAX(intensity, 1.0f);
        fogScale = intensity - 0.2f;

        if (intensity < 0.2f) {
            fogScale = 0.0f;
        }

        play->envCtx.adjLightSettings.fogNear = (850.0f - play->envCtx.lightSettings.fogNear) * fogScale;

        if (intensity == 0.0f) {
            for (i = 0; i < ARRAY_SIZE(play->envCtx.adjLightSettings.fogColor); i++) {
                play->envCtx.adjLightSettings.fogColor[i] = 0;
            }
        } else {
            colorScale = intensity * 5.0f;

            if (colorScale > 1.0f) {
                colorScale = 1.0f;
            }

            for (i = 0; i < ARRAY_SIZE(play->envCtx.adjLightSettings.fogColor); i++) {
                play->envCtx.adjLightSettings.fogColor[i] = -(s16)(play->envCtx.lightSettings.fogColor[i] * colorScale);
            }
        }
    }
}

void MagicDark_OrbUpdate(Actor* thisx, GameState_Play* play) {
    MagicDark* this = (MagicDark*)thisx;
    Actor_Player* player = GET_LINK(play);

    PlayLoopingSfxAtActor(&this->actor, 0xC3); // NA_SE_PL_MAGIC_SOUL_BALL
    if (this->timer < 35) {
        MagicDark_DimLighting(play, this->timer * (1 / 45.0f));
        Math_SmoothStepToF(&thisx->scale.x, this->scale * (1 / 12.000001f), 0.05f, 0.01f, 0.0001f);
        ActorSetScale(&this->actor, thisx->scale.x);
    } else if (this->timer < 55) {
        ActorSetScale(&this->actor, thisx->scale.x * 0.9f);
        Math_SmoothStepToF(&this->orbOffset.y, player->bodyPartsPos[PLAYER_BODYPART_WAIST].y, 0.5f, 3.0f, 1.0f);
        if (this->timer > 48) {
            MagicDark_DimLighting(play, (54 - this->timer) * 0.2f);
        }
    } else {
        thisx->update = MagicDark_DiamondUpdate;
        thisx->draw = MagicDark_DiamondDraw;
        thisx->scale.x = thisx->scale.z = this->scale * 1.6f;
        thisx->scale.y = this->scale * 0.8f;
        this->timer = 0;
        this->primAlpha = 0;
    }

    this->timer++;
}

void MagicDark_DiamondDraw(Actor* thisx, GameState_Play* play) {
    MagicDark* this = (MagicDark*)thisx;
    u16 gameplayFrames = play->gameplayFrames;

    OPEN_DISPS(play->gs.gfx);
    gSPSegment(POLY_XLU_DISP++, 0x08, gCustomKeep);

    InitListPolyXlu(play->gs.gfx);

    {
        Actor_Player* player = GET_LINK(play);
        f32 heightDiff;

        this->actor.position.x = player->bodyPartsPos[PLAYER_BODYPART_WAIST].x;
        this->actor.position.z = player->bodyPartsPos[PLAYER_BODYPART_WAIST].z;
        f32 y = player->base.position.y + Player_GetHeight(player) * 0.5f;
        heightDiff = y - this->actor.position.y;
        if (heightDiff < -2.0f) {
            this->actor.position.y = y + 2.0f;
        } else if (heightDiff > 2.0f) {
            this->actor.position.y = y - 2.0f;
        }
        ModelViewTranslate(this->actor.position.x, this->actor.position.y, this->actor.position.z, MAT_SET);
        ModelViewScale(this->actor.scale.x, this->actor.scale.y, this->actor.scale.z, MAT_MUL);
        ModelViewRotateY(BINANG_TO_RAD(this->actor.rot2.y), MAT_MUL);
        gSPMatrix(POLY_XLU_DISP++, GetMatrixMV(play->gs.gfx),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 170, 255, 255, (s32)(this->primAlpha * 0.6f) & 0xFF);
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 100, 255, 128);
        gSPDisplayList(POLY_XLU_DISP++, sDiamondMaterialDL);
        gSPDisplayList(POLY_XLU_DISP++,
                       DisplaceTexture(play->gs.gfx, G_TX_RENDERTILE, gameplayFrames * 2, gameplayFrames * -4,
                                        32, 32, 1, 0, gameplayFrames * -16, 64, 32));
        gSPDisplayList(POLY_XLU_DISP++, sDiamondModelDL);
    }

    CLOSE_DISPS();
}

void MagicDark_OrbDraw(Actor* thisx, GameState_Play* play) {
    MagicDark* this = (MagicDark*)thisx;
    Vec3f pos;
    Actor_Player* player = GET_LINK(play);
    f32 sp6C = play->gs.frameCount & 0x1F;

    if (this->timer < 32) {
        pos.x =
            (player->bodyPartsPos[PLAYER_BODYPART_LEFT_HAND].x + player->bodyPartsPos[PLAYER_BODYPART_RIGHT_HAND].x) * 0.5f;
        pos.y =
            (player->bodyPartsPos[PLAYER_BODYPART_LEFT_HAND].y + player->bodyPartsPos[PLAYER_BODYPART_RIGHT_HAND].y) * 0.5f;
        pos.z =
            (player->bodyPartsPos[PLAYER_BODYPART_LEFT_HAND].z + player->bodyPartsPos[PLAYER_BODYPART_RIGHT_HAND].z) * 0.5f;
        if (this->timer > 20) {
            pos.y += (this->timer - 20) * 1.4f;
        }
        this->orbOffset = pos;
    } else if (this->timer < 130) {
        pos = this->orbOffset;
    } else {
        return;
    }

    pos.x -= (this->actor.scale.x * 300.0f * Math_SinS(Camera_GetCamDirYaw(GET_ACTIVE_CAM(play))) *
              Math_CosS(Camera_GetCamDirPitch(GET_ACTIVE_CAM(play))));
    pos.y -= (this->actor.scale.x * 300.0f * Math_SinS(Camera_GetCamDirPitch(GET_ACTIVE_CAM(play))));
    pos.z -= (this->actor.scale.x * 300.0f * Math_CosS(Camera_GetCamDirYaw(GET_ACTIVE_CAM(play))) *
              Math_CosS(Camera_GetCamDirPitch(GET_ACTIVE_CAM(play))));

    OPEN_DISPS(play->gs.gfx);

    InitListPolyXlu(play->gs.gfx);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0x80, 170, 255, 255, 255);
    gDPSetEnvColor(POLY_XLU_DISP++, 0, 150, 255, 255);
    ModelViewTranslate(pos.x, pos.y, pos.z, MAT_SET);
    ModelViewScale(this->actor.scale.x, this->actor.scale.y, this->actor.scale.z, MAT_MUL);
    ModelViewMult(&play->billboardMtxF, MAT_MUL);
    MatrixStackDup();
    gSPMatrix(POLY_XLU_DISP++, GetMatrixMV(play->gs.gfx),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    ModelViewRotateZ(sp6C * (M_PI / 32), MAT_MUL);
    gSPDisplayList(POLY_XLU_DISP++, 0x04000000 | 0x23210); // gEffFlash1DL
    MatrixStackPop();
    ModelViewRotateZ(-sp6C * (M_PI / 32), MAT_MUL);
    gSPMatrix(POLY_XLU_DISP++, GetMatrixMV(play->gs.gfx),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, 0x04000000 | 0x23210); // gEffFlash1DL

    CLOSE_DISPS();
}
