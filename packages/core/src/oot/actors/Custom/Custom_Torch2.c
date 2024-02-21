/*
 * File: z_en_torch2.c
 * Overlay: ovl_En_Torch2
 * Description: Elegy of Emptiness Shell
 */

#include <combo.h>
#include <combo/custom.h>

#define FLAGS (1 << 4) // (ACTOR_FLAG_10)

#define THIS ((EnTorch2*)thisx)

void EnTorch2_Init(Actor* thisx, GameState_Play* play);
void EnTorch2_Destroy(Actor* thisx, GameState_Play* play);
void EnTorch2_Update(Actor* thisx, GameState_Play* play);
void EnTorch2_Draw(Actor* thisx, GameState_Play* play2);

void EnTorch2_UpdateIdle(Actor* thisx, GameState_Play* play);
void EnTorch2_UpdateDeath(Actor* thisx, GameState_Play* play);

extern EnTorch2* gElegyShells[];

ActorInit En_Torch2_InitVars = {
    /**/ AC_CUSTOM_TORCH2,
    /**/ ACTORCAT_ITEMACTION,
    /**/ FLAGS,
    /**/ 1, // GAMEPLAY_KEEP,
    /**/ sizeof(EnTorch2),
    /**/ EnTorch2_Init,
    /**/ EnTorch2_Destroy,
    /**/ EnTorch2_Update,
    /**/ EnTorch2_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_METAL,
        AT_NONE,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER | OC1_TYPE_1 | OC1_TYPE_2,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK2,
        { 0x00100000, 0, 0 },
        { 0xF7CFFFFF, 0, 0 },
        TOUCH_NONE,
        BUMP_ON | BUMP_HOOKABLE,
        OCELEM_ON,
    },
    { 20, 60, 0, { 0, 0, 0 } },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(colChkInfo.mass, MASS_IMMOVABLE, ICHAIN_STOP),
};

const u32 gElegyShellHumanMouthTex = 0x08000000 | CUSTOM_KEEP_ELEGY_SHELL_HUMAN_MOUTH_TEXTURE;
const u32 gElegyShellHumanEyeAndNoseTex = 0x08000000 | CUSTOM_KEEP_ELEGY_SHELL_HUMAN_EYE_AND_NOSE_TEXTURE;
const u32 gElegyShellHumanNostrilsAndSkinTex = 0x08000000 | CUSTOM_KEEP_ELEGY_SHELL_HUMAN_NOSTRILS_AND_SKIN_TEXTURE;
const u32 gElegyShellHumanHairTex = 0x08000000 | CUSTOM_KEEP_ELEGY_SHELL_HUMAN_HAIR_TEXTURE;
const u32 gElegyShellBeltAndTunicTex = 0x08000000 | CUSTOM_KEEP_ELEGY_SHELL_BELT_AND_TUNIC_TEXTURE;
const u32 gElegyShellHumanBootsTex = 0x08000000 | CUSTOM_KEEP_ELEGY_SHELL_HUMAN_BOOTS_TEXTURE;
const u32 gElegyShellHumanPlatformTex = 0x08000000 | CUSTOM_KEEP_ELEGY_SHELL_HUMAN_PLATFORM_TEXTURE;

static Vtx sElegyShellHumanVtx[360] = {
#include "sElegyShellHumanVtx.vtx.inc"
};

static Gfx gElegyShellHumanDL[] = {
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, COMBINED, 0, ENVIRONMENT, 0),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsSPDisplayList(0x0C000000),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPLoadTextureBlock(gElegyShellHumanMouthTex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_MIRROR | G_TX_CLAMP,
                         G_TX_MIRROR | G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetPrimColor(0, 0x80, 255, 255, 255, 255),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(sElegyShellHumanVtx, 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 1, 0, 0),
    gsSP2Triangles(2, 4, 5, 0, 6, 7, 8, 0),
    gsSP2Triangles(9, 10, 11, 0, 12, 8, 7, 0),
    gsSP2Triangles(0, 13, 3, 0, 8, 13, 0, 0),
    gsSP2Triangles(14, 15, 16, 0, 15, 12, 16, 0),
    gsSP2Triangles(13, 15, 14, 0, 15, 8, 12, 0),
    gsSP2Triangles(13, 8, 15, 0, 3, 13, 14, 0),
    gsSP2Triangles(17, 7, 18, 0, 19, 7, 17, 0),
    gsSP2Triangles(12, 19, 16, 0, 12, 7, 19, 0),
    gsSP2Triangles(20, 18, 21, 0, 22, 23, 24, 0),
    gsSP2Triangles(23, 25, 26, 0, 8, 27, 6, 0),
    gsSP1Triangle(28, 29, 30, 0),
    gsSPVertex(&sElegyShellHumanVtx[31], 29, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 2, 4, 0),
    gsSP2Triangles(5, 6, 7, 0, 8, 9, 10, 0),
    gsSPDisplayList(0x0C000000),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPLoadTextureBlock(gElegyShellHumanEyeAndNoseTex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 64, 0, G_TX_MIRROR |
                         G_TX_CLAMP, G_TX_MIRROR | G_TX_CLAMP, 5, 6, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetPrimColor(0, 0x80, 255, 255, 255, 255),
    gsSP2Triangles(11, 12, 13, 0, 11, 14, 12, 0),
    gsSP2Triangles(11, 15, 14, 0, 15, 16, 14, 0),
    gsSP2Triangles(17, 14, 16, 0, 17, 12, 14, 0),
    gsSP2Triangles(17, 18, 12, 0, 18, 17, 19, 0),
    gsSP2Triangles(20, 18, 19, 0, 15, 21, 16, 0),
    gsSP2Triangles(22, 23, 24, 0, 25, 18, 20, 0),
    gsSP2Triangles(25, 26, 18, 0, 12, 18, 26, 0),
    gsSP2Triangles(27, 12, 26, 0, 22, 27, 26, 0),
    gsSP2Triangles(27, 22, 24, 0, 27, 24, 28, 0),
    gsSP2Triangles(12, 27, 28, 0, 13, 12, 28, 0),
    gsSPDisplayList(0x0C000000),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPLoadTextureBlock(gElegyShellHumanNostrilsAndSkinTex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 8, 0, G_TX_MIRROR |
                         G_TX_CLAMP, G_TX_MIRROR | G_TX_CLAMP, 4, 3, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetPrimColor(0, 0x80, 255, 255, 255, 255),
    gsSPVertex(&sElegyShellHumanVtx[60], 7, 0),
    gsSP2Triangles(0, 1, 2, 0, 1, 3, 2, 0),
    gsSP1Triangle(4, 5, 6, 0),
    gsSPDisplayList(0x0C000000),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPLoadTextureBlock(gElegyShellHumanHairTex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 16, 0, G_TX_NOMIRROR | G_TX_WRAP,
                         G_TX_NOMIRROR | G_TX_WRAP, 4, 4, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetPrimColor(0, 0x80, 255, 255, 255, 255),
    gsSPVertex(&sElegyShellHumanVtx[67], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 0, 2, 0),
    gsSP2Triangles(4, 5, 6, 0, 7, 5, 4, 0),
    gsSP2Triangles(8, 9, 10, 0, 11, 12, 13, 0),
    gsSP2Triangles(11, 1, 12, 0, 14, 13, 12, 0),
    gsSP2Triangles(15, 16, 17, 0, 18, 1, 11, 0),
    gsSP2Triangles(19, 9, 8, 0, 19, 20, 9, 0),
    gsSP2Triangles(21, 20, 19, 0, 19, 22, 21, 0),
    gsSP2Triangles(23, 24, 25, 0, 4, 6, 7, 0),
    gsSP2Triangles(26, 27, 28, 0, 21, 29, 30, 0),
    gsSP1Triangle(30, 31, 21, 0),
    gsSPVertex(&sElegyShellHumanVtx[99], 21, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 0, 2, 0),
    gsSP2Triangles(4, 5, 6, 0, 7, 8, 9, 0),
    gsSP2Triangles(10, 11, 12, 0, 10, 5, 4, 0),
    gsSP2Triangles(11, 10, 4, 0, 13, 0, 3, 0),
    gsSP2Triangles(14, 15, 16, 0, 17, 15, 14, 0),
    gsSP2Triangles(18, 19, 20, 0, 18, 5, 19, 0),
    gsSPDisplayList(0x0C000000),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPLoadTextureBlock(gElegyShellBeltAndTunicTex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 16, 0, G_TX_MIRROR | G_TX_CLAMP,
                         G_TX_MIRROR | G_TX_CLAMP, 5, 4, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetPrimColor(0, 0x80, 255, 255, 255, 255),
    gsSPVertex(&sElegyShellHumanVtx[120], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(3, 6, 7, 0, 3, 7, 8, 0),
    gsSP2Triangles(3, 8, 9, 0, 3, 9, 4, 0),
    gsSP2Triangles(7, 6, 10, 0, 11, 7, 10, 0),
    gsSP2Triangles(11, 10, 12, 0, 10, 13, 12, 0),
    gsSP2Triangles(14, 15, 16, 0, 14, 17, 15, 0),
    gsSP2Triangles(18, 19, 20, 0, 20, 13, 18, 0),
    gsSP2Triangles(18, 21, 0, 0, 2, 18, 0, 0),
    gsSP2Triangles(10, 21, 13, 0, 2, 22, 19, 0),
    gsSP2Triangles(19, 22, 23, 0, 3, 24, 6, 0),
    gsSP2Triangles(5, 24, 3, 0, 1, 22, 2, 0),
    gsSP2Triangles(21, 25, 0, 0, 25, 21, 10, 0),
    gsSP2Triangles(18, 13, 21, 0, 19, 18, 2, 0),
    gsSP2Triangles(25, 24, 0, 0, 0, 24, 1, 0),
    gsSP2Triangles(25, 10, 6, 0, 6, 24, 25, 0),
    gsSP2Triangles(1, 5, 22, 0, 5, 1, 24, 0),
    gsSP2Triangles(5, 4, 22, 0, 4, 26, 22, 0),
    gsSP2Triangles(23, 22, 26, 0, 27, 28, 29, 0),
    gsSPVertex(&sElegyShellHumanVtx[150], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(6, 7, 8, 0, 1, 0, 9, 0),
    gsSP2Triangles(6, 8, 10, 0, 11, 8, 7, 0),
    gsSP2Triangles(12, 13, 14, 0, 15, 16, 17, 0),
    gsSP2Triangles(18, 19, 20, 0, 20, 21, 18, 0),
    gsSP2Triangles(20, 22, 21, 0, 4, 23, 24, 0),
    gsSP2Triangles(4, 25, 23, 0, 26, 27, 28, 0),
    gsSP2Triangles(25, 29, 23, 0, 29, 30, 23, 0),
    gsSPVertex(&sElegyShellHumanVtx[181], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(6, 7, 5, 0, 8, 9, 10, 0),
    gsSP2Triangles(9, 11, 10, 0, 9, 12, 11, 0),
    gsSP2Triangles(13, 14, 15, 0, 9, 16, 12, 0),
    gsSP2Triangles(8, 10, 17, 0, 18, 19, 20, 0),
    gsSP2Triangles(21, 22, 13, 0, 20, 23, 24, 0),
    gsSP2Triangles(25, 26, 27, 0, 28, 25, 27, 0),
    gsSP1Triangle(29, 30, 31, 0),
    gsSPVertex(&sElegyShellHumanVtx[213], 9, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(4, 3, 6, 0, 7, 3, 1, 0),
    gsSP1Triangle(8, 1, 3, 0),
    gsSPDisplayList(0x0C000000),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPLoadTextureBlock(gElegyShellHumanNostrilsAndSkinTex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 8, 0, G_TX_MIRROR |
                         G_TX_CLAMP, G_TX_MIRROR | G_TX_CLAMP, 4, 3, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetPrimColor(0, 0x80, 255, 255, 255, 255),
    gsSPVertex(&sElegyShellHumanVtx[222], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 1, 3, 2, 0),
    gsSP2Triangles(4, 5, 6, 0, 7, 8, 9, 0),
    gsSP2Triangles(10, 11, 12, 0, 13, 12, 11, 0),
    gsSP2Triangles(14, 15, 16, 0, 6, 17, 4, 0),
    gsSP2Triangles(11, 18, 19, 0, 10, 18, 11, 0),
    gsSP2Triangles(20, 21, 22, 0, 23, 20, 22, 0),
    gsSP2Triangles(23, 22, 24, 0, 25, 3, 1, 0),
    gsSP2Triangles(25, 1, 26, 0, 27, 28, 29, 0),
    gsSPVertex(&sElegyShellHumanVtx[252], 31, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(4, 6, 5, 0, 4, 7, 6, 0),
    gsSP2Triangles(8, 9, 10, 0, 9, 11, 10, 0),
    gsSP2Triangles(9, 12, 11, 0, 13, 7, 4, 0),
    gsSP2Triangles(14, 13, 4, 0, 14, 4, 3, 0),
    gsSP2Triangles(15, 16, 17, 0, 18, 16, 15, 0),
    gsSP2Triangles(15, 19, 20, 0, 19, 15, 17, 0),
    gsSP2Triangles(21, 22, 23, 0, 21, 23, 24, 0),
    gsSP2Triangles(25, 23, 26, 0, 23, 22, 26, 0),
    gsSP2Triangles(27, 28, 29, 0, 27, 30, 28, 0),
    gsSPDisplayList(0x0C000000),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPLoadTextureBlock(gElegyShellHumanBootsTex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 16, 0, G_TX_MIRROR | G_TX_WRAP,
                         G_TX_MIRROR | G_TX_CLAMP, 4, 4, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetPrimColor(0, 0x80, 255, 255, 255, 255),
    gsSPVertex(&sElegyShellHumanVtx[283], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(6, 7, 8, 0, 6, 9, 7, 0),
    gsSP2Triangles(10, 11, 12, 0, 13, 4, 3, 0),
    gsSP2Triangles(14, 15, 16, 0, 17, 18, 19, 0),
    gsSP2Triangles(20, 18, 17, 0, 2, 21, 0, 0),
    gsSP2Triangles(22, 23, 24, 0, 25, 23, 22, 0),
    gsSP2Triangles(26, 27, 28, 0, 29, 27, 26, 0),
    gsSPVertex(&sElegyShellHumanVtx[313], 24, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 1, 0, 0),
    gsSP2Triangles(4, 5, 6, 0, 7, 8, 9, 0),
    gsSP2Triangles(10, 11, 12, 0, 13, 10, 12, 0),
    gsSP2Triangles(14, 8, 7, 0, 15, 16, 17, 0),
    gsSP2Triangles(18, 19, 20, 0, 21, 22, 23, 0),
    gsSPDisplayList(0x0C000000),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPLoadTextureBlock(gElegyShellHumanPlatformTex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 16, 0, G_TX_NOMIRROR | G_TX_WRAP,
                         G_TX_NOMIRROR | G_TX_WRAP, 3, 4, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetPrimColor(0, 0x80, 255, 255, 255, 255),
    gsSPVertex(&sElegyShellHumanVtx[337], 23, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
    gsSP2Triangles(3, 2, 8, 0, 3, 8, 9, 0),
    gsSP2Triangles(7, 6, 10, 0, 7, 10, 11, 0),
    gsSP2Triangles(12, 13, 3, 0, 4, 14, 12, 0),
    gsSP2Triangles(12, 14, 11, 0, 3, 15, 12, 0),
    gsSP2Triangles(16, 17, 18, 0, 19, 17, 20, 0),
    gsSP2Triangles(18, 17, 21, 0, 20, 17, 22, 0),
    gsSPEndDisplayList(),
};

// Shells for each of Link's different forms
// (Playing elegy as Fierce Deity puts down a human shell)
static Gfx* sShellDLists[] = {
    gElegyShellHumanDL, // gElegyShellGoronDL, gElegyShellZoraDL, gElegyShellDekuDL, gElegyShellHumanDL,
};

void EnTorch2_Init(Actor* thisx, GameState_Play* play) {
    EnTorch2* this = THIS;

    Actor_ProcessInitChain(&this->base, sInitChain);
    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, &this->base, &sCylinderInit);

    if (this->base.variable != TORCH2_PARAM_DEKU) {
        this->base.flags |= (1 << 26); // ACTOR_FLAG_CAN_PRESS_SWITCH;

        // All OoT Elegy Statues are human, for now
        // Also no switches require a "heavy" actor, and rusty switches work completely differently
        // if (this->base.variable == TORCH2_PARAM_GORON) {
        //     this->base.flags |= ACTOR_FLAG_CAN_PRESS_HEAVY_SWITCH;
        // }
    }
    this->framesUntilNextState = 20;
}

void EnTorch2_Destroy(Actor* thisx, GameState_Play* play) {
    EnTorch2* this = THIS;

    Collider_DestroyCylinder(play, &this->collider);

    gElegyShells[this->base.variable] = NULL;
}

#define fabsf(f) __builtin_fabsf((f32)(f))

void EnTorch2_Update(Actor* thisx, GameState_Play* play) {
    EnTorch2* this = THIS;
    u16 targetAlpha;
    u16 remainingFrames;

    if (this->state == TORCH2_STATE_IDLE) {
        this->base.update = EnTorch2_UpdateIdle;
        return;
    }

    this->base.gravity = -1.0f;
    ActorUpdateVelocity(&this->base);
    Actor_SetCollisionCylinder(play, &this->base, 30.0f, 20.0f, 70.0f, UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2);

    if (this->framesUntilNextState == 0) {
        remainingFrames = 0;
    } else {
        remainingFrames = --this->framesUntilNextState;
    }

    if (remainingFrames == 0) {
        if (this->state == TORCH2_STATE_INITIALIZED) {
            // Spawn in
            if (this->alpha == 0) {
                Math_Vec3f_Copy(&this->base.world.pos, &this->base.home.pos);
                this->base.rot2.y = this->base.home.rot.y;
                this->state = TORCH2_STATE_FADING_IN;
            }
            targetAlpha = 0;
        } else if (this->state == TORCH2_STATE_FADING_IN) {
            // Stay semitransparent until the player moves away
            if ((this->base.xzDistanceFromLink > 32.0f) || (fabsf(this->base.yDistanceFromLink) > 70.0f)) {
                this->state = TORCH2_STATE_SOLID;
            }
            targetAlpha = 60;
        } else {
            // Once the player has moved away, update collision and become opaque
            Collider_UpdateCylinder(&this->base, &this->collider);
            CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
            targetAlpha = 255;
        }
        Math_StepToS(&this->alpha, targetAlpha, 8);
    }
}

void EnTorch2_UpdateIdle(Actor* thisx, GameState_Play* play) {
    EnTorch2* this = THIS;

    if (this->state == TORCH2_STATE_DYING) {
        // Start death animation
        this->base.update = EnTorch2_UpdateDeath;
        this->base.velocity.y = 0.0f;
    }
}

void EnTorch2_UpdateDeath(Actor* thisx, GameState_Play* play) {
    EnTorch2* this = THIS;

    // Fall down and become transparent, then delete once invisible
    if (Math_StepToS(&this->alpha, 0, 8)) {
        ActorDestroy(&this->base);
        return;
    }

    this->base.gravity = -1.0f;
    ActorUpdateVelocity(&this->base);
}

void EnTorch2_Draw(Actor* thisx, GameState_Play* play2) {
    static Gfx renderModeSetNoneDL[] = {
        gsSPEndDisplayList(),
        gsSPEndDisplayList(),
    };
    static Gfx renderModeSetXluSingleCycleDL[] = {
        gsDPSetRenderMode(AA_EN | Z_CMP | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | ZMODE_XLU | FORCE_BL |
                              GBL_c1(G_BL_CLR_FOG, G_BL_A_SHADE, G_BL_CLR_IN, G_BL_1MA),
                          G_RM_AA_ZB_XLU_SURF2),
        gsSPEndDisplayList(),
    };

    GameState_Play* play = play2;
    EnTorch2* this = THIS;
    Gfx* gfx = sShellDLists[this->base.variable];

    OPEN_DISPS(play->gs.gfx);

    if (this->alpha == 255) {
        gSPSegment(POLY_OPA_DISP++, 0x08, gCustomKeep);
        gSPSegment(POLY_OPA_DISP++, 0x0C, renderModeSetNoneDL);
        gDPSetEnvColor(POLY_OPA_DISP++, 255, 255, 255, 255);
        Gfx_DrawDListOpa(play, gfx);
    } else {
        gSPSegment(POLY_XLU_DISP++, 0x08, gCustomKeep);
        gSPSegment(POLY_XLU_DISP++, 0x0C, renderModeSetXluSingleCycleDL);
        gDPSetEnvColor(POLY_XLU_DISP++, 255, 255, 255, this->alpha);
        Gfx_DrawDListXlu(play, gfx);
    }

    CLOSE_DISPS();
}
