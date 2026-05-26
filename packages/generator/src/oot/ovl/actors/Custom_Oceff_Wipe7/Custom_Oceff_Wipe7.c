/*
 * File: z_oceff_wipe7.c
 * Overlay: ovl_Oceff_Wipe7
 * Description: Song of Healing Ocarina Effect
 */

#include "Custom_Oceff_Wipe7.h"
#include <assets/oot/objects/gameplay_keep.h>
#include <combo/magic.h>
#include <combo/custom.h>

#define FLAGS (ACTOR_FLAG_UPDATE_CULLING_DISABLED | ACTOR_FLAG_UPDATE_DURING_OCARINA)

void OceffWipe7_Init(Actor* thisx, PlayState* play);
void OceffWipe7_Destroy(Actor* thisx, PlayState* play);
void OceffWipe7_Update(Actor* thisx, PlayState* play);
void OceffWipe7_Draw(Actor* thisx, PlayState* play);

ActorProfile Oceff_Wipe7_Profile = {
    /**/ ACTOR_CUSTOM_OCEFF_WIPE7,
    /**/ ACTORCAT_ITEMACTION,
    /**/ FLAGS,
    /**/ CUSTOM_OBJECT_ID_OCEFF_WIPE7,
    /**/ sizeof(OceffWipe7),
    /**/ OceffWipe7_Init,
    /**/ OceffWipe7_Destroy,
    /**/ OceffWipe7_Update,
    /**/ OceffWipe7_Draw,
};

void OceffWipe7_Init(Actor* thisx, PlayState* play) {
    OceffWipe7* this = (OceffWipe7*)thisx;

    Actor_SetScale(&this->actor, 1.0f);
    this->counter = 0;
    this->actor.world.pos = GET_ACTIVE_CAM(play)->eye;
}

void OceffWipe7_Destroy(Actor* thisx, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Magic_Reset(play);
    if (gSaveContext.nayrusLoveTimer != 0) {
        player->state3 |= PLAYER_STATE3_RESTORE_NAYRUS_LOVE;
    }
}

void OceffWipe7_Update(Actor* thisx, PlayState* play) {
    OceffWipe7* this = (OceffWipe7*)thisx;

    this->actor.world.pos = GET_ACTIVE_CAM(play)->eye;
    if (this->counter < 100) {
        this->counter++;
    } else {
        Actor_Kill(&this->actor);
    }
}

void OceffWipe7_Draw(Actor* thisx, PlayState* play) {
    OceffWipe7* this = (OceffWipe7*)thisx;
    f32 z;
    u8 alpha;
    s32 i;
    Vec3f activeCamEye = GET_ACTIVE_CAM(play)->eye;
    Vec3f quakeOffset = Camera_GetQuakeOffset(GET_ACTIVE_CAM(play));

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

    Vtx* songofHealingEffectFrustrumVtx = SEGMENTED_TO_VIRTUAL((void*)CUSTOM_OBJECT_OCEFF_WIPE7_1);

    for (i = 1; i < 22 /* ARRAY_COUNT(songofHealingEffectFrustrumVtx) */; i += 2) {
        songofHealingEffectFrustrumVtx[i].v.cn[3] = alpha;
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
    AnimatedMat_Draw(play, SEGMENTED_TO_VIRTUAL((void*)CUSTOM_OBJECT_OCEFF_WIPE7_2));
    gSPDisplayList(POLY_XLU_DISP++, CUSTOM_OBJECT_OCEFF_WIPE7_0);

    CLOSE_DISPS();
}

OVL_INFO_ACTOR(ACTOR_CUSTOM_OCEFF_WIPE7, Oceff_Wipe7_Profile);
