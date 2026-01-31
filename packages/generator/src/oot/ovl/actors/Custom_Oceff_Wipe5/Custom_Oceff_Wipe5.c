/*
 * File: z_oceff_wipe7.c
 * Overlay: ovl_Oceff_Wipe5
 * Description: Song of Healing Ocarina Effect
 */

#include "Custom_Oceff_Wipe5.h"
#include <assets/oot/objects/gameplay_keep.h>
#include <combo/magic.h>
#include <combo/custom.h>

#define FLAGS (ACTOR_FLAG_UPDATE_CULLING_DISABLED | ACTOR_FLAG_UPDATE_DURING_OCARINA)

void OceffWipe5_Init(Actor* thisx, PlayState* play);
void OceffWipe5_Destroy(Actor* thisx, PlayState* play);
void OceffWipe5_Update(Actor* thisx, PlayState* play);
void OceffWipe5_Draw(Actor* thisx, PlayState* play);

ActorProfile Oceff_Wipe5_Profile = {
    /**/ ACTOR_CUSTOM_OCEFF_WIPE5,
    /**/ ACTORCAT_ITEMACTION,
    /**/ FLAGS,
    /**/ CUSTOM_OBJECT_ID_OCEFF_WIPE5,
    /**/ sizeof(OceffWipe5),
    /**/ OceffWipe5_Init,
    /**/ OceffWipe5_Destroy,
    /**/ OceffWipe5_Update,
    /**/ OceffWipe5_Draw,
};

void OceffWipe5_Init(Actor* thisx, PlayState* play) {
    OceffWipe5* this = (OceffWipe5*)thisx;

    Actor_SetScale(&this->actor, 1.0f);
    this->counter = 0;
    this->actor.world.pos = GET_ACTIVE_CAM(play)->eye;
}

void OceffWipe5_Destroy(Actor* thisx, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Magic_Reset(play);
    if (gSaveContext.nayrusLoveTimer != 0) {
        player->state3 |= PLAYER_STATE3_RESTORE_NAYRUS_LOVE;
    }
}

void OceffWipe5_Update(Actor* thisx, PlayState* play) {
    OceffWipe5* this = (OceffWipe5*)thisx;

    this->actor.world.pos = GET_ACTIVE_CAM(play)->eye;
    if (this->counter < 100) {
        this->counter++;
    } else {
        Actor_Kill(&this->actor);
    }
}

static u8 sPrimColors[] = {
    255, 255, 200, 255, 255, 200, 200, 255, 255, 255, 255, 200, 255, 200, 255,
};

static u8 sEnvColors[] = {
    50, 200, 0, 255, 50, 0, 0, 100, 255, 255, 150, 0, 255, 0, 200,
};

void OceffWipe5_Draw(Actor* thisx, PlayState* play) {
    OceffWipe5* this = (OceffWipe5*)thisx;
    f32 z;
    s32 i;
    Vec3f activeCamEye = GET_ACTIVE_CAM(play)->eye;
    Vec3f quakeOffset = Camera_GetQuakeOffset(GET_ACTIVE_CAM(play));
    u8 alpha;
    s32 colorIndex = OCEFF_WIPE5_GET_SONG_TYPE(thisx) * 3;

    if (colorIndex >= 13) {
        colorIndex = 0;
    }

    if (this->counter < 32) {
        z = Math_SinS(this->counter * 0x200) * 1220.0f;
    } else {
        z = 1220.0f;
    }

    if (this->counter >= 80) {
        alpha = 12 * (100 - this->counter);
    } else {
        alpha = 255;
    }

    Vtx* oceff5Vtx = SEGMENTED_TO_VIRTUAL((void*)CUSTOM_OBJECT_OCEFF_WIPE5_1);

    for (i = 1; i < 22 /* ARRAY_COUNT(oceff5Vtx) */; i += 2) {
        oceff5Vtx[i].v.cn[3] = alpha;
    }

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    Matrix_Translate(activeCamEye.x + quakeOffset.x, activeCamEye.y + quakeOffset.y, activeCamEye.z + quakeOffset.z,
                     MTXMODE_NEW);
    Matrix_Scale(0.1f, 0.1f, 0.1f, MTXMODE_APPLY);
    Matrix_ReplaceRotation(&play->billboardMtxF);
    Matrix_RotateXS(0x708, MTXMODE_APPLY);
    Matrix_Translate(0.0f, 0.0f, -z, MTXMODE_APPLY);
    MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0x80, sPrimColors[colorIndex], sPrimColors[colorIndex + 1],
                    sPrimColors[colorIndex + 2], 255);
    gDPSetEnvColor(POLY_XLU_DISP++, sEnvColors[colorIndex], sEnvColors[colorIndex + 1], sEnvColors[colorIndex + 2],
                   255);
    AnimatedMat_Draw(play, SEGMENTED_TO_VIRTUAL((void*)CUSTOM_OBJECT_OCEFF_WIPE5_2));
    gSPDisplayList(POLY_XLU_DISP++, CUSTOM_OBJECT_OCEFF_WIPE5_0);

    CLOSE_DISPS();
}

OVL_INFO_ACTOR(ACTOR_CUSTOM_OCEFF_WIPE5, Oceff_Wipe5_Profile);
