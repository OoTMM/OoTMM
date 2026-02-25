/*
 * File: z_oceff_wipe6.c
 * Overlay: ovl_Oceff_Wipe6
 * Description: Song of Soaring Ocarina Effect
 */

#include "Custom_Oceff_Wipe6.h"
#include <assets/oot/objects/gameplay_keep.h>
#include <combo/magic.h>

#define FLAGS (ACTOR_FLAG_UPDATE_CULLING_DISABLED | ACTOR_FLAG_UPDATE_DURING_OCARINA)

void OceffWipe6_Init(Actor* thisx, PlayState* play);
void OceffWipe6_Destroy(Actor* thisx, PlayState* play);
void OceffWipe6_Update(Actor* thisx, PlayState* play);
void OceffWipe6_Draw(Actor* thisx, PlayState* play);

ActorProfile Oceff_Wipe6_Profile = {
    /**/ ACTOR_CUSTOM_OCEFF_WIPE6,
    /**/ ACTORCAT_ITEMACTION,
    /**/ FLAGS,
    /**/ OBJECT_GAMEPLAY_KEEP,
    /**/ sizeof(OceffWipe6),
    /**/ OceffWipe6_Init,
    /**/ OceffWipe6_Destroy,
    /**/ OceffWipe6_Update,
    /**/ OceffWipe6_Draw,
};

static Vtx gOceff6Vtx[] = {
#include "gOceff6Vtx.vtx.inc"
};

static Gfx gOceff6MaterialDL[] = {
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPSetCombineLERP(TEXEL1, PRIMITIVE, PRIM_LOD_FRAC, TEXEL0, TEXEL1, 0, PRIM_LOD_FRAC, TEXEL0, PRIMITIVE,
                       ENVIRONMENT, COMBINED, ENVIRONMENT, COMBINED, 0, SHADE, 0),
    gsDPSetPrimColor(0, 0x80, 255, 255, 255, 255),
    gsDPSetEnvColor(150, 150, 150, 255),
    gsDPSetRenderMode(G_RM_PASS, G_RM_AA_XLU_SURF2),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPLoadTextureBlock(gEffUnknown12Tex, G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
                         G_TX_NOMIRROR | G_TX_WRAP, 5, 5, 1, 1),
    gsDPLoadMultiBlock(gEffUnknown12Tex, 0x0000, 1, G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
                       G_TX_NOMIRROR | G_TX_WRAP, 5, 5, 15, G_TX_NOLOD),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH),
    gsSPEndDisplayList(),
};

static Gfx gOceff6ModelDL[] = {
    gsSPVertex(gOceff6Vtx, 22, 0),
    gsSP2Triangles(0, 1, 2, 0, 1, 3, 2, 0),
    gsSP2Triangles(4, 5, 0, 0, 5, 1, 0, 0),
    gsSP2Triangles(6, 7, 4, 0, 7, 5, 4, 0),
    gsSP2Triangles(8, 9, 6, 0, 9, 7, 6, 0),
    gsSP2Triangles(10, 11, 8, 0, 11, 9, 8, 0),
    gsSP2Triangles(12, 13, 10, 0, 13, 11, 10, 0),
    gsSP2Triangles(14, 15, 12, 0, 15, 13, 12, 0),
    gsSP2Triangles(16, 17, 18, 0, 19, 15, 14, 0),
    gsSP2Triangles(20, 21, 16, 0, 21, 17, 16, 0),
    gsSP2Triangles(2, 3, 20, 0, 3, 21, 20, 0),
    gsSPEndDisplayList(),
};

void OceffWipe6_Init(Actor* thisx, PlayState* play) {
    OceffWipe6* this = (OceffWipe6*)thisx;

    Actor_SetScale(&this->actor, 1.0f);
    this->counter = 0;
    this->actor.world.pos = GET_ACTIVE_CAM(play)->eye;
}

void OceffWipe6_Destroy(Actor* thisx, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Magic_Reset(play);
    if (gSaveContext.nayrusLoveTimer != 0) {
        player->state3 |= PLAYER_STATE3_RESTORE_NAYRUS_LOVE;
    }
}

void OceffWipe6_Update(Actor* thisx, PlayState* play) {
    OceffWipe6* this = (OceffWipe6*)thisx;

    this->actor.world.pos = GET_ACTIVE_CAM(play)->eye;
    if (this->counter < 100) {
        this->counter++;
    } else {
        Actor_Kill(&this->actor);
    }
}

void OceffWipe6_Draw(Actor* thisx, PlayState* play) {
    u32 scroll = play->state.frameCount & 0xFFF;
    OceffWipe6* this = (OceffWipe6*)thisx;
    f32 z;
    u8 alpha;
    s32 i;
    s32 counter;
    Vec3f activeCamEye = GET_ACTIVE_CAM(play)->eye;
    Vec3f quakeOffset = Camera_GetQuakeOffset(GET_ACTIVE_CAM(play));

    if (this->counter < 32) {
        counter = this->counter;
        z = Math_SinS(counter * 0x200) * 1220.0f;
    } else {
        z = 1220.0f;
    }

    if (this->counter >= 80) {
        alpha = 12 * (100 - this->counter);
    } else {
        alpha = 255;
    }

    for (i = 1; i < ARRAY_COUNT(gOceff6Vtx); i += 2) {
        gOceff6Vtx[i].v.cn[3] = alpha;
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
    gSPDisplayList(POLY_XLU_DISP++, &gOceff6MaterialDL);
    gSPDisplayList(POLY_XLU_DISP++, Gfx_TwoTexScroll(play->state.gfxCtx, 0, scroll * -1, scroll * -1, 32, 32, 1,
                                                     scroll * -1, scroll * -4, 32, 32));
    gSPDisplayList(POLY_XLU_DISP++, &gOceff6ModelDL);

    CLOSE_DISPS();
}

OVL_INFO_ACTOR(ACTOR_CUSTOM_OCEFF_WIPE6, Oceff_Wipe6_Profile);
