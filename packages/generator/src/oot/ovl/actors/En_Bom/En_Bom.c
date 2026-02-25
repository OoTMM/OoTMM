/*
 * File: z_en_bom.c
 * Overlay: ovl_En_Bom
 * Description: Bomb
 */

#include "En_Bom.h"
#include <assets/oot/objects/gameplay_keep.h>
#include <combo/math.h>


#define FLAGS (ACTOR_FLAG_UPDATE_CULLING_DISABLED | ACTOR_FLAG_OOT_5)

void EnBom_Init(EnBom* this, PlayState* play);
void EnBom_Destroy(EnBom* this, PlayState* play);
void EnBom_Update(EnBom* this, PlayState* play2);
void EnBom_Draw(EnBom* this, PlayState* play);

void EnBom_Move(EnBom* this, PlayState* play);
void EnBom_WaitForRelease(EnBom* this, PlayState* play);

static ColliderCylinderInit sCylinderInit = {
    {
        COL_MATERIAL_HIT0,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER | AC_TYPE_OTHER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK2,
        { 0x00000000, 0x00, 0x00 },
        { 0x0003F828, 0x00, 0x00 },
        ATELEM_NONE,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 6, 11, 14, { 0, 0, 0 } },
};

static ColliderJntSphElementInit sJntSphElementsInit[1] = {
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0x00000008, 0x00, 0x08 },
            { 0x00000000, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NONE,
            ACELEM_NONE,
            OCELEM_NONE,
        },
        { 0, { { 0, 0, 0 }, 0 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COL_MATERIAL_HIT0,
        AT_ON | AT_TYPE_ALL,
        AC_NONE,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_JNTSPH,
    },
    1,
    sJntSphElementsInit,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F(scale, 0, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 2000, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, -4000, ICHAIN_STOP),
};

void EnBom_SetupAction(EnBom* this, EnBomActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

void EnBom_Init(EnBom* this, PlayState* play) {
    Actor_ProcessInitChain(&this->actor, sInitChain);
    ActorShape_Init(&this->actor.shape, 700.0f, ActorShadow_DrawCircle, 16.0f);
    this->actor.colChkInfo.mass = 200;
    this->actor.colChkInfo.cylRadius = 5;
    this->actor.colChkInfo.cylHeight = 10;
    this->timer = 70;
    this->flashSpeedScale = 7;
    Collider_InitCylinder(play, &this->bombCollider);
    Collider_InitJntSph(play, &this->explosionCollider);
    Collider_SetCylinder(play, &this->bombCollider, &this->actor, &sCylinderInit);
    Collider_SetJntSph(play, &this->explosionCollider, &this->actor, &sJntSphInit, &this->explosionColliderItems[0]);
    this->explosionColliderItems[0].base.atDmgInfo.damage += (this->actor.shape.rot.z & 0xFF00) >> 8;

    this->actor.shape.rot.z &= 0xFF;
    if (this->actor.shape.rot.z & 0x80) {
        this->actor.shape.rot.z |= 0xFF00;
    }

    EnBom_SetupAction(this, EnBom_Move);
}

void EnBom_Destroy(EnBom* this, PlayState* play) {
    Collider_DestroyJntSph(play, &this->explosionCollider);
    Collider_DestroyCylinder(play, &this->bombCollider);
}

void EnBom_Move(EnBom* this, PlayState* play) {
    // if bomb has a parent actor, the bomb hasnt been released yet
    if (Actor_HasParent(&this->actor, play)) {
        EnBom_SetupAction(this, EnBom_WaitForRelease);
        this->actor.room = -1;
        return;
    }

    if ((this->actor.velocity.y > 0.0f) && (this->actor.bgCheckFlags & BGCHECKFLAG_CEILING)) {
        this->actor.velocity.y = -this->actor.velocity.y;
    }

    // rebound bomb off the wall it hits
    if ((this->actor.speed != 0.0f) && (this->actor.bgCheckFlags & BGCHECKFLAG_WALL)) {
        s16 yawDiff = this->actor.wallYaw - this->actor.world.rot.y;
        if (ABS(yawDiff) > 0x4000) {
            this->actor.world.rot.y = ((this->actor.wallYaw - this->actor.world.rot.y) + this->actor.wallYaw) - 0x8000;
        }
        Actor_PlaySfx(&this->actor, NA_SE_EV_BOMB_BOUND);
        Actor_MoveXZGravity(&this->actor);
        this->actor.speed *= 0.7f;
        this->actor.bgCheckFlags &= ~BGCHECKFLAG_WALL;
    }

    if (!(this->actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
        Math_StepToF(&this->actor.speed, 0.0f, 0.08f);
    } else {
        Math_StepToF(&this->actor.speed, 0.0f, 1.0f);
        if ((this->actor.bgCheckFlags & BGCHECKFLAG_GROUND_TOUCH) && (this->actor.velocity.y < -3.0f)) {
            Actor_PlaySfx_SurfaceBomb(play, &this->actor);
            this->actor.velocity.y *= -0.3f;
            this->actor.bgCheckFlags &= ~BGCHECKFLAG_GROUND_TOUCH;
        } else if (this->timer >= 4) {
            Actor_OfferCarry(&this->actor, play);
        }
    }

    Actor_MoveXZGravity(&this->actor);
}

void EnBom_WaitForRelease(EnBom* this, PlayState* play) {
    // if parent is NULL bomb has been released
    if (Actor_HasNoParent(&this->actor, play)) {
        EnBom_SetupAction(this, EnBom_Move);
        EnBom_Move(this, play);
    }
}

void EnBom_Explode(EnBom* this, PlayState* play) {
    Player* player;

    if (this->explosionCollider.elements[0].dim.modelSphere.radius == 0) {
        this->actor.flags |= ACTOR_FLAG_OOT_5;
        Rumble_Request(this->actor.xzDistToPlayer, 255, 20, 150);
    }

    this->explosionCollider.elements[0].dim.worldSphere.radius += this->actor.shape.rot.z + 8;

    if (this->actor.params == BOMB_EXPLOSION) {
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->explosionCollider.base);
    }

    if (play->envCtx.adjLight1Color[0] != 0) {
        play->envCtx.adjLight1Color[0] -= 25;
    }

    if (play->envCtx.adjLight1Color[1] != 0) {
        play->envCtx.adjLight1Color[1] -= 25;
    }

    if (play->envCtx.adjLight1Color[2] != 0) {
        play->envCtx.adjLight1Color[2] -= 25;
    }

    if (play->envCtx.adjAmbientColor[0] != 0) {
        play->envCtx.adjAmbientColor[0] -= 25;
    }

    if (play->envCtx.adjAmbientColor[1] != 0) {
        play->envCtx.adjAmbientColor[1] -= 25;
    }

    if (play->envCtx.adjAmbientColor[2] != 0) {
        play->envCtx.adjAmbientColor[2] -= 25;
    }

    if (this->timer == 0) {
        player = GET_PLAYER(play);

        if ((player->stateFlags1 & PLAYER_STATE1_ACTOR_CARRY) && (player->heldActor == &this->actor)) {
            player->heldActor = NULL;
            // Emtpy bomb fix
            player->heldItemAction = 0;
            player->heldItemId = 0;
            player->itemAction = 0;
            player->actor.child = NULL;
            player->interactRangeActor = NULL;
            player->stateFlags1 &= ~PLAYER_STATE1_ACTOR_CARRY;
        }

        Actor_Kill(&this->actor);
    }
}

void EnBom_Update(EnBom* this, PlayState* play) {
    Vec3f effVelocity = { 0.0f, 0.0f, 0.0f };
    Vec3f bomb2Accel = { 0.0f, 0.1f, 0.0f };
    Vec3f effAccel = { 0.0f, 0.0f, 0.0f };
    Vec3f effPos;
    Vec3f dustAccel = { 0.0f, 0.6f, 0.0f };
    Color_RGBA8 dustColor = { 255, 255, 255, 255 };

    this->actor.gravity = -1.2f;

    if (this->timer != 0) {
        this->timer--;
    }

    if (this->timer == 67) {
        Actor_PlaySfx(&this->actor, NA_SE_PL_TAKE_OUT_SHIELD);
        Actor_SetScale(&this->actor, 0.01f);
    }

    if ((this->actor.xzDistToPlayer >= 20.0f) || (ABS(this->actor.yDistanceFromLink) >= 80.0f)) {
        this->colliderSetOC = true;
    }

    this->actionFunc(this, play);

    Actor_UpdateBgCheckInfo(play, &this->actor, 5.0f, 10.0f, 15.0f,
                            UPDBGCHECKINFO_FLAG_OOT_0 | UPDBGCHECKINFO_FLAG_OOT_1 | UPDBGCHECKINFO_FLAG_OOT_2 |
                                UPDBGCHECKINFO_FLAG_OOT_3 | UPDBGCHECKINFO_FLAG_OOT_4);

    if (this->actor.params == BOMB_BODY) {
        if (this->timer < 63) {
            dustAccel.y = 0.2f;

            // spawn spark effect on even frames
            effPos = this->actor.world.pos;
            effPos.y += 17.0f;
            if ((play->gameplayFrames % 2) == 0) {
                EffectSsGSpk_SpawnFuse(play, &this->actor, &effPos, &effVelocity, &effAccel);
            }

            Actor_PlaySfx(&this->actor, NA_SE_IT_BOMB_IGNIT - SFX_FLAG);

            effPos.y += 3.0f;
            func_8002829C(play, &effPos, &effVelocity, &dustAccel, &dustColor, &dustColor, 50, 5);
        }

        if ((this->bombCollider.base.acFlags & AC_HIT) || ((this->bombCollider.base.ocFlags1 & OC1_HIT) &&
                                                           (this->bombCollider.base.oc->type == ACTORCAT_ENEMY))) {
            this->timer = 0;
            this->actor.shape.rot.z = 0;
        } else {
            // if a lit stick touches the bomb, set timer to 100
            // these bombs never have a timer over 70, so this isn't used
            if ((this->timer > 100) && Player_IsBurningStickInRange(play, &this->actor.world.pos, 30.0f, 50.0f)) {
                this->timer = 100;
            }
        }

        dustAccel.y = 0.2f;
        effPos = this->actor.world.pos;
        effPos.y += 10.0f;

        // double bomb flash speed and adjust red color at certain times during the countdown
        if ((this->timer == 3) || (this->timer == 20) || (this->timer == 40)) {
            this->actor.shape.rot.z = 0;
            this->flashSpeedScale >>= 1;
        }

        if ((this->timer < 100) && ((this->timer & (this->flashSpeedScale + 1)) != 0)) {
            Math_SmoothStepToF(&this->flashIntensity, 140.0f, 1.0f, 140.0f / this->flashSpeedScale, 0.0f);
        } else {
            Math_SmoothStepToF(&this->flashIntensity, 0.0f, 1.0f, 140.0f / this->flashSpeedScale, 0.0f);
        }

        if (this->timer < 3) {
            Actor_SetScale(&this->actor, this->actor.scale.x + 0.002f);
        }

        if (this->timer == 0) {
            effPos = this->actor.world.pos;

            effPos.y += 10.0f;
            if (Actor_HasParent(&this->actor, play)) {
                effPos.y += 30.0f;
            }

            EffectSsBomb2_SpawnLayered(play, &effPos, &effVelocity, &bomb2Accel, 100, (this->actor.shape.rot.z * 6) + 19);

            effPos.y = this->actor.floorHeight;
            if (this->actor.floorHeight > BGCHECK_Y_MIN) {
                EffectSsBlast_SpawnWhiteShockwave(play, &effPos, &effVelocity, &effAccel);
            }

            Actor_PlaySfx(&this->actor, NA_SE_IT_BOMB_EXPLOSION);

            play->envCtx.adjLight1Color[0] = play->envCtx.adjLight1Color[1] = play->envCtx.adjLight1Color[2] = 250;

            play->envCtx.adjAmbientColor[0] = play->envCtx.adjAmbientColor[1] = play->envCtx.adjAmbientColor[2] = 250;

            Camera_RequestQuake(&play->mainCamera, 2, 11, 8);
            this->actor.params = BOMB_EXPLOSION;
            this->timer = 10;
            this->actor.flags |= ACTOR_FLAG_OOT_5;
            EnBom_SetupAction(this, EnBom_Explode);
        }
    }

    Actor_SetFocus(&this->actor, 20.0f);

    if (this->actor.params <= BOMB_BODY) {
        Collider_UpdateCylinder(&this->actor, &this->bombCollider);

        // if link is not holding the bomb anymore and conditions for OC are met, subscribe to OC
        if (!Actor_HasParent(&this->actor, play) && this->colliderSetOC) {
            CollisionCheck_SetOC(play, &play->colChkCtx, &this->bombCollider.base);
        }

        CollisionCheck_SetAC(play, &play->colChkCtx, &this->bombCollider.base);
    }

    if ((this->actor.scale.x >= 0.01f) && (this->actor.params != BOMB_EXPLOSION)) {
        if (this->actor.depthInWater >= 20.0f) {
            EffectSsDeadSound_SpawnStationary(play, &this->actor.projectedPos, NA_SE_IT_BOMB_UNEXPLOSION, true,
                                              DEADSOUND_REPEAT_MODE_OFF, 10);
            Actor_Kill(&this->actor);
            return;
        }
        if (this->actor.bgCheckFlags & BGCHECKFLAG_WATER_TOUCH) {
            this->actor.bgCheckFlags &= ~BGCHECKFLAG_WATER_TOUCH;
            Actor_PlaySfx(&this->actor, NA_SE_EV_BOMB_DROP_WATER);
        }
    }
}

void EnBom_Draw(EnBom* this, PlayState* play) {
    OPEN_DISPS(play->state.gfxCtx);

    if (this->actor.params == BOMB_BODY) {
        Gfx_SetupDL_25Opa(play->state.gfxCtx);
        Matrix_ReplaceRotation(&play->billboardMtxF);
        PreDraw1(&this->actor, play, 0);

        MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);
        gSPDisplayList(POLY_OPA_DISP++, gBombCapDL);
        Matrix_RotateZYX(0x4000, 0, 0, MTXMODE_APPLY);
        MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);
        gDPPipeSync(POLY_OPA_DISP++);
        gDPSetEnvColor(POLY_OPA_DISP++, (s16)this->flashIntensity, 0, 40, 255);
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, (s16)this->flashIntensity, 0, 40, 255);
        gSPDisplayList(POLY_OPA_DISP++, gBombBodyDL);
        Collider_UpdateSpheres(0, &this->explosionCollider);
    }

    CLOSE_DISPS();
}

ActorProfile En_Bom_Profile = {
    ACTOR_EN_BOM,
    ACTORCAT_EXPLOSIVE,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnBom),
    (ActorFunc)EnBom_Init,
    (ActorFunc)EnBom_Destroy,
    (ActorFunc)EnBom_Update,
    (ActorFunc)EnBom_Draw,
};

OVL_INFO_ACTOR(ACTOR_EN_BOM, En_Bom_Profile);
