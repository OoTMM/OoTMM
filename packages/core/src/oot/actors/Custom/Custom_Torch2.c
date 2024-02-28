/*
 * File: z_en_torch2.c
 * Overlay: ovl_En_Torch2
 * Description: Elegy of Emptiness Shell
 */

#include <combo.h>
#include <combo/custom.h>

#define FLAGS (1 << 4) /* (ACTOR_FLAG_10) */

void EnTorch2_Init(Actor_CustomEnTorch2 *this, GameState_Play *play);
void EnTorch2_Destroy(Actor_CustomEnTorch2 *this, GameState_Play *play);
void EnTorch2_Update(Actor_CustomEnTorch2 *this, GameState_Play *play);
void EnTorch2_Draw(Actor_CustomEnTorch2 *this, GameState_Play *play);

void EnTorch2_UpdateIdle(Actor_CustomEnTorch2 *this, GameState_Play *play);
void EnTorch2_UpdateDeath(Actor_CustomEnTorch2 *this, GameState_Play *play);

extern Actor_CustomEnTorch2 *gElegyShell;

ActorInit En_Torch2_InitVars = {
    AC_CUSTOM_TORCH2,
    ACTORCAT_ITEMACTION,
    FLAGS,
    CUSTOM_OBJECT_ID_OBJECT_TORCH2,
    sizeof(Actor_CustomEnTorch2),
    (ActorFunc)EnTorch2_Init,
    (ActorFunc)EnTorch2_Destroy,
    (ActorFunc)EnTorch2_Update,
    (ActorFunc)EnTorch2_Draw,
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
        {0x00100000, 0, 0},
        {0xF7CFFFFF, 0, 0},
        TOUCH_NONE,
        BUMP_ON | BUMP_HOOKABLE,
        OCELEM_ON,
    },
    {20, 60, 0, {0, 0, 0}},
};

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(colChkInfo.mass, MASS_IMMOVABLE, ICHAIN_STOP),
};

void EnTorch2_Init(Actor_CustomEnTorch2 *this, GameState_Play *play)
{
    Actor_ProcessInitChain(&this->base, sInitChain);
    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, &this->base, &sCylinderInit);

    this->base.flags |= (1 << 26); /* ACTOR_FLAG_CAN_PRESS_SWITCH; */

    this->framesUntilNextState = 20;
}

void EnTorch2_Destroy(Actor_CustomEnTorch2 *this, GameState_Play *play)
{
    Collider_DestroyCylinder(play, &this->collider);

    gElegyShell = NULL;
}

void EnTorch2_Update(Actor_CustomEnTorch2 *this, GameState_Play *play)
{
    u16 targetAlpha;
    u16 remainingFrames;

    if (this->state == TORCH2_STATE_IDLE)
    {
        this->base.update = EnTorch2_UpdateIdle;
        return;
    }

    this->base.gravity = -1.0f;
    ActorUpdateVelocity(&this->base);
    Actor_SetCollisionCylinder(play, &this->base, 30.0f, 20.0f, 70.0f, UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2);

    if (this->framesUntilNextState == 0)
    {
        remainingFrames = 0;
    }
    else
    {
        remainingFrames = --this->framesUntilNextState;
    }

    if (remainingFrames == 0)
    {
        if (this->state == TORCH2_STATE_INITIALIZED)
        {
            /* Spawn in */
            if (this->alpha == 0)
            {
                Math_Vec3f_Copy(&this->base.world.pos, &this->base.home.pos);
                this->base.rot2.y = this->base.home.rot.y;
                this->state = TORCH2_STATE_FADING_IN;
            }
            targetAlpha = 0;
        }
        else if (this->state == TORCH2_STATE_FADING_IN)
        {
            /* Stay semitransparent until the player moves away */
            if ((this->base.xzDistanceFromLink > 32.0f) || (fabsf(this->base.yDistanceFromLink) > 70.0f))
            {
                this->state = TORCH2_STATE_SOLID;
            }
            targetAlpha = 60;
        }
        else
        {
            /* Once the player has moved away, update collision and become opaque */
            Collider_UpdateCylinder(&this->base, &this->collider);
            CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
            targetAlpha = 255;
        }
        Math_StepToS(&this->alpha, targetAlpha, 8);
    }
}

void EnTorch2_UpdateIdle(Actor_CustomEnTorch2 *this, GameState_Play *play)
{
    if (this->state == TORCH2_STATE_DYING)
    {
        /* Start death animation */
        this->base.update = EnTorch2_UpdateDeath;
        this->base.velocity.y = 0.0f;
    }
}

void EnTorch2_UpdateDeath(Actor_CustomEnTorch2 *this, GameState_Play *play)
{
    /* Fall down and become transparent, then delete once invisible */
    if (Math_StepToS(&this->alpha, 0, 8))
    {
        ActorDestroy(&this->base);
        return;
    }

    this->base.gravity = -1.0f;
    ActorUpdateVelocity(&this->base);
}

void EnTorch2_Draw(Actor_CustomEnTorch2 *this, GameState_Play *play)
{
    static Gfx renderModeSetNoneDL[] = {
        gsSPEndDisplayList(),
        gsSPEndDisplayList(),
        gsSPEndDisplayList(),
        gsSPEndDisplayList(),
    };
    static Gfx renderModeSetXluSingleCycleDL[] = {
        gsDPSetRenderMode(AA_EN | Z_CMP | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | ZMODE_XLU | FORCE_BL |
                              GBL_c1(G_BL_CLR_IN, G_BL_0, G_BL_CLR_IN, G_BL_1),
                          G_RM_AA_ZB_XLU_SURF2),
        gsSPEndDisplayList(),
        gsDPSetRenderMode(AA_EN | Z_CMP | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | ZMODE_XLU | FORCE_BL |
                              GBL_c1(G_BL_CLR_FOG, G_BL_A_SHADE, G_BL_CLR_IN, G_BL_1MA),
                          G_RM_AA_ZB_XLU_SURF2),
        gsSPEndDisplayList(),
    };

    OPEN_DISPS(play->gs.gfx);

    if (this->alpha == 255)
    {
        gSPSegment(POLY_OPA_DISP++, 0x0C, renderModeSetNoneDL);
        gDPSetEnvColor(POLY_OPA_DISP++, 255, 255, 255, 255);
        Gfx_DrawDListOpa(play, (Gfx*)CUSTOM_OBJECT_OBJECT_TORCH2_0);
    }
    else
    {
        gSPSegment(POLY_XLU_DISP++, 0x0C, renderModeSetXluSingleCycleDL);
        gDPSetEnvColor(POLY_XLU_DISP++, 255, 255, 255, this->alpha);
        Gfx_DrawDListXlu(play, (Gfx*)CUSTOM_OBJECT_OBJECT_TORCH2_0);
    }

    CLOSE_DISPS();
}
