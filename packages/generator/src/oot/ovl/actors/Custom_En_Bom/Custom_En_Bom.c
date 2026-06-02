#include "Custom_En_Bom.h"
#include "../En_Clear_Tag/En_Clear_Tag.h"
#include <combo/custom.h>

#define FLAGS (ACTOR_FLAG_UPDATE_CULLING_DISABLED | ACTOR_FLAG_DRAW_CULLING_DISABLED)

#define THIS ((EnBomMM*)thisx)

void EnBomMM_Init(Actor* thisx, PlayState* play);
void EnBomMM_Destroy(Actor* thisx, PlayState* play);
void EnBomMM_Update(Actor* thisx, PlayState* play);
void EnBomMM_Draw(Actor* thisx, PlayState* play);

void EnBomMM_Move(EnBomMM* this, PlayState* play);
void EnBomMM_WaitForRelease(EnBomMM* this, PlayState* play);

void EnBomMM_InitKegFuseSegments(PlayState* play, Vec3f* bombPos);
void EnBomMM_UpdateKegFuseSegments(PlayState* play, Vec3f* fuseRootPos, Vec3f* arg2, Vec3f* arg3, s32 timer);
void EnBomMM_DrawKegFuseSegments(PlayState* play, s32 fuseTimer);

typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ Vec3f velocity;
    /* 0x18 */ s16 rotY;
    /* 0x1A */ s16 rotX;
} PowderKegFuseSegment; // size = 0x1C

PowderKegFuseSegment sPowderKegFuseSegments[16];

static f32 enBomScales[] = { 0.01f, 0.03f };

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
        ATELEM_NONE | ATELEM_SFX_NORMAL,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 6, 11, 14, { 0, 0, 0 } },
};

static ColliderJntSphElementInit sJntSphElementsInit1[1] = {
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

static ColliderJntSphInit sJntSphInit1 = {
    {
        COL_MATERIAL_HIT0,
        AT_ON | AT_TYPE_ALL,
        AC_NONE,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sJntSphElementsInit1),
    sJntSphElementsInit1,
};

static ColliderJntSphElementInit sJntSphElementsInit2[1] = {
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0x40000008, 0x00, 0x08 },
            { 0x00000000, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NONE,
            ACELEM_NONE,
            OCELEM_NONE,
        },
        { 0, { { 0, 0, 0 }, 0 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit2 = {
    {
        COL_MATERIAL_HIT0,
        AT_ON | AT_TYPE_ALL,
        AC_NONE,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sJntSphElementsInit2),
    sJntSphElementsInit2,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F(scale, 0, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 2000, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, -4000, ICHAIN_STOP),
};

void EnBomMM_Init(Actor* thisx, PlayState* play)
{
    EnBomMM* this = THIS;

    Actor_ProcessInitChain(&this->actor, sInitChain);
    ActorShape_Init(&this->actor.shape, 700.0f, ActorShadow_DrawCircle, 16.0f);
    this->actor.colChkInfo.mass = 200;
    this->actor.colChkInfo.cylRadius = 5;
    this->actor.colChkInfo.cylHeight = 10;

    this->flashSpeedScale = 7;
    u8 bombType = ENBOMMM_GET_3(&this->actor);
    this->isPowderKeg = bombType == BOMB_EXPLOSIVE_TYPE_POWDER_KEG;
    if (this->isPowderKeg)
    {
        this->timer = gCustomSave.powderKegTimer;
    }
    else
    {
        this->timer = 70;
    }

    Collider_InitCylinder(play, &this->collider1);
    Collider_InitJntSph(play, &this->collider2);
    Collider_SetCylinder(play, &this->collider1, &this->actor, &sCylinderInit);

    if (!this->isPowderKeg)
    {
        Collider_SetJntSph(play, &this->collider2, &this->actor, &sJntSphInit1, this->collider2Elements);
        this->collider1.dim.radius = 6;
        this->collider1.dim.height = 11;
    }
    else
    {
        Collider_SetJntSph(play, &this->collider2, &this->actor, &sJntSphInit2, this->collider2Elements);
        this->collider1.dim.radius = 20;
        this->collider1.dim.height = 36;
        EnBomMM_InitKegFuseSegments(play, &this->actor.world.pos);
    }

    this->collider2Elements[0].base.atDmgInfo.damage += ENBOMMM_GET_FF00(thisx);
    this->actor.shape.rot.z &= 0xFF;
    if (ENBOMMM_GET_80(&this->actor))
    {
        this->actor.shape.rot.z |= 0xFF00;
    }

    this->collider2.elements[0].dim.worldSphere.center.x = this->actor.world.pos.x;
    this->collider2.elements[0].dim.worldSphere.center.y = this->actor.world.pos.y;
    this->collider2.elements[0].dim.worldSphere.center.z = this->actor.world.pos.z;

    this->actor.flags |= ACTOR_FLAG_UPDATE_DURING_OCARINA;

    if (bombType != BOMB_EXPLOSIVE_TYPE_BLAST)
    {
        this->actionFunc = EnBomMM_WaitForRelease;
        this->actor.room = -1;
        Actor_SetScale(&this->actor, enBomScales[this->isPowderKeg]);
    }
    else
    {
        this->actionFunc = EnBomMM_Move;
        gCustomSave.powderKegTimer = 0;
    }

    // if (Actor_HasParent(&this->actor, play)) {
    //     this->actionFunc = EnBomMM_WaitForRelease;
    //     this->actor.room = -1;
    //     Actor_SetScale(&this->actor, enBomScales[this->isPowderKeg]);
    // } else {
    //     this->actionFunc = EnBomMM_Move;
    //     gCustomSave.powderKegTimer = 0;
    // }
}

void EnBomMM_Destroy(Actor* thisx, PlayState* play)
{
    EnBomMM* this = THIS;

    Collider_DestroyJntSph(play, &this->collider2);
    Collider_DestroyCylinder(play, &this->collider1);
}

typedef struct BombPhysics {
    f32 deceleration;
    f32 bounceThreshold;
    f32 bounceAmount;
} BombPhysics;

void EnBomMM_Move(EnBomMM* this, PlayState* play)
{
    static BombPhysics sBombPhysics[] = {
        { 2.0f, -6.0f, -0.3f },
        { 1.5f, -5.0f, -0.6f },
        { 0.2f, -6.0f, -0.1f },
    };

    // if bomb has a parent actor, the bomb hasnt been released yet
    if (Actor_HasParent(&this->actor, play))
    {
        this->actionFunc = EnBomMM_WaitForRelease;
        this->actor.room = -1;
        return;
    }

    if ((this->actor.velocity.y > 0.0f) && (this->actor.bgCheckFlags & BGCHECKFLAG_CEILING))
    {
        this->actor.velocity.y = -this->actor.velocity.y;
    }

    // rebound bomb off the wall it hits
    if ((this->actor.speed != 0.0f) && (this->actor.bgCheckFlags & BGCHECKFLAG_WALL))
    {
        s16 yDiff = BINANG_SUB(this->actor.wallYaw, this->actor.world.rot.y);

        if (ABS_ALT(yDiff) > 0x4000)
        {
            this->actor.world.rot.y =
                BINANG_SUB(this->actor.wallYaw - this->actor.world.rot.y + this->actor.wallYaw, 0x8000);
        }

        Actor_PlaySfx(&this->actor, this->isPowderKeg ? NA_SE_EV_PUT_DOWN_WOODBOX : NA_SE_EV_BOMB_BOUND);
        Actor_MoveWithGravity(&this->actor);
        this->actor.speed *= 0.7f;
        this->actor.bgCheckFlags &= ~BGCHECKFLAG_WALL;
    }

    if (!(this->actor.bgCheckFlags & BGCHECKFLAG_GROUND))
    {
        Math_StepToF(&this->actor.speed, 0.0f, 0.08f);
    }
    else
    {
        BombPhysics* bombPhysics;
        FloorType floorType = SurfaceType_GetFloorType(&play->colCtx, this->actor.floorPoly, this->actor.floorBgId);
        Vec3f slopeNormal;
        s16 downwardSlopeYaw;
        f32 sp40;
        f32 sp3C;
        f32 sp38;

        bombPhysics = &sBombPhysics[this->isPowderKeg];

        // if (floorType == FLOOR_TYPE_5) {
        //     bombPhysics = &sBombPhysics[2];
        // }

        if (floorType == FLOOR_TYPE_4)
        {
            s16 sp36;

            Math_ApproachF(&this->actor.shape.yOffset, 0.0f, 0.1f, 50.0f);
            sp36 = this->actor.shape.shadowAlpha;
            Math_ApproachS(&sp36, 0, 1, 20);
            this->actor.shape.shadowAlpha = sp36;
        }
        else
        {
            Math_ApproachF(&this->actor.shape.yOffset, 700.0f, 1.0f, 700.0f);
        }

        sp40 = Math_SinS(this->actor.world.rot.y) * this->actor.speed;
        sp3C = Math_CosS(this->actor.world.rot.y) * this->actor.speed;
        Actor_GetSlopeDirection(this->actor.floorPoly, &slopeNormal, &downwardSlopeYaw);

        sp40 += 3.0f * slopeNormal.x;
        sp3C += 3.0f * slopeNormal.z;
        sp38 = sqrtf(SQ(sp40) + SQ(sp3C));

        if (sp38 < this->actor.speed)
        {
            if (sp38 > 16.0f)
            {
                this->actor.speed = 16.0f;
            }
            else
            {
                this->actor.speed = sp38;
            }
            this->actor.world.rot.y = Math_Atan2S(sp3C, sp40);
        }

        if (!Math_StepToF(&this->actor.speed, 0.0f, bombPhysics->deceleration))
        {
            s16 temp = this->actor.world.rot.y;

            if (ABS_ALT(BINANG_SUB(this->actor.world.rot.y, this->actor.shape.rot.y)) > 0x4000)
            {
                temp = BINANG_ROT180(temp);
            }
            Math_ScaledStepToS(&this->actor.shape.rot.y, temp, this->actor.speed * 100.0f);
            this->unk_1FA += TRUNCF_BINANG(this->actor.speed * 800.0f);
        }

        if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND_TOUCH)
        {
            /* TODO NA_SE_EV_PUT_DOWN_WOODBOX */
            Actor_PlaySfx(&this->actor, this->isPowderKeg ? NA_SE_EV_PUT_DOWN_WOODBOX : NA_SE_EV_BOMB_BOUND);
            if (this->actor.velocity.y < bombPhysics->bounceThreshold)
            {
                if ((floorType == FLOOR_TYPE_4) || (floorType == FLOOR_TYPE_14) || (floorType == FLOOR_TYPE_15))
                {
                    this->actor.velocity.y = 0.0f;
                }
                else
                {
                    this->actor.velocity.y *= bombPhysics->bounceAmount;
                }
                this->actor.bgCheckFlags &= ~BGCHECKFLAG_GROUND;
            }
        }
        else if (this->timer >= 4)
        {
            Actor_OfferCarry(&this->actor, play);
        }
    }

    Actor_MoveWithGravity(&this->actor);
}

void EnBomMM_WaitForRelease(EnBomMM* this, PlayState* play)
{
    // if parent is NULL bomb has been released
    if (Actor_HasNoParent(&this->actor, play))
    {
        this->actionFunc = EnBomMM_Move;
        this->actor.room = play->roomCtx.curRoom.num;
        this->actor.flags &= ~ACTOR_FLAG_UPDATE_DURING_OCARINA;
        this->actor.bgCheckFlags &= ~BGCHECKFLAG_GROUND;
        Math_Vec3s_ToVec3f(&this->actor.prevPos, &this->actor.home.rot);
        if (this->isPowderKeg)
        {
            gCustomSave.powderKegTimer = 0;
        }
        EnBomMM_Move(this, play);
    }
    else
    {
        Math_Vec3f_ToVec3s(&this->actor.home.rot, &this->actor.parent->world.pos);
        if (this->isPowderKeg)
        {
            gCustomSave.powderKegTimer = this->timer;
        }
    }
    Math_ScaledStepToS(&this->unk_1FA, 0, 0x7D0);
}

void EnBomMM_Explode(EnBomMM* this, PlayState* play)
{
    static s16 D_80872E8C[] = { 100, 200 };
    static Color_RGBA8 D_80872E90 = { 185, 140, 70, 255 };
    s32 i;
    FloorType floorType;
    f32 temp_f20;
    f32 spCC;
    Vec3f spC0;
    Vec3f spB4;
    CollisionPoly* spB0;
    s32 spAC;
    Vec3f spA0;
    Vec3f sp94;
    Vec3f sp88;
    Color_RGBA8 sp84;
    Color_RGBA8 sp80;

    if (this->collider2.elements->dim.modelSphere.radius == 0)
    {
        this->actor.flags |= ACTOR_FLAG_DRAW_CULLING_DISABLED;
        Rumble_Request(this->actor.xzDistToPlayer, 255, 20, 150);
    }

    this->collider2.elements->dim.worldSphere.radius = D_80872E8C[this->isPowderKeg];
    if (this->timer == 7)
    {
        this->collider2.base.atFlags &= ~AT_TYPE_ENEMY;
    }

    if (this->actor.params == BOMB_TYPE_EXPLOSION)
    {
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider2.base);
    }

    if (play->envCtx.adjLight1Color[0] != 0)
    {
        play->envCtx.adjLight1Color[0] -= 25;
    }

    if (play->envCtx.adjLight1Color[1] != 0)
    {
        play->envCtx.adjLight1Color[1] -= 25;
    }

    if (play->envCtx.adjLight1Color[2] != 0)
    {
        play->envCtx.adjLight1Color[2] -= 25;
    }

    if (play->envCtx.adjAmbientColor[0] != 0)
    {
        play->envCtx.adjAmbientColor[0] -= 25;
    }

    if (play->envCtx.adjAmbientColor[1] != 0)
    {
        play->envCtx.adjAmbientColor[1] -= 25;
    }

    if (play->envCtx.adjAmbientColor[2] != 0)
    {
        play->envCtx.adjAmbientColor[2] -= 25;
    }

    if (this->timer == 0)
    {
        func_80123590(play, &this->actor);
        Actor_Kill(&this->actor);
    }

    if ((this->timer & 1) == 0)
    {
        spCC = Rand_ZeroFloat(M_PIf);

        for (i = 0; i < 15; i++)
        {
            Matrix_RotateYF(((2.0f * (i * M_PIf)) / 15.0f) + spCC, MTXMODE_NEW);
            Matrix_MultVecZ((10 - this->timer) * 300.0f * 0.1f, &spC0);
            spB4.x = this->actor.world.pos.x + spC0.x;
            spB4.y = this->actor.world.pos.y + 500.0f;
            spB4.z = this->actor.world.pos.z + spC0.z;
            if (BgCheck_EntityRaycastDown3(&play->colCtx, &spB0, &spAC, &spB4) != BGCHECK_Y_MIN)
            {
                floorType = SurfaceType_GetFloorType(&play->colCtx, spB0, spAC);
                temp_f20 = BgCheck_EntityRaycastDown1(&play->colCtx, &spB0, &spB4);

                if (floorType == FLOOR_TYPE_4)
                {
                    sp84 = D_80872E90;
                    sp80 = D_80872E90;
                    Matrix_MultVecZ(5.0f, &sp94);
                    sp88.x = sp88.z = 0.0f;
                    sp94.y = 2.0f;
                    sp88.y = 0.2f;

                    spA0.x = spB4.x;
                    spA0.y = temp_f20;
                    spA0.z = spB4.z;

                    temp_f20 = Rand_ZeroFloat(250.0f);
                    func_8002843C(play, &spA0, &sp94, &sp88, &sp84, &sp80, temp_f20 + 950.0f, 10,
                                  Rand_ZeroFloat(5.0f) + 14.0f);
                }
            }
        }
    }
}

static s16 sQuakeY[] = { 3, 5 };
static s16 sQuakeDurations[] = { 10, 15 };

void EnBomMM_Update(Actor* thisx, PlayState* play)
{
    Vec3f effVelocity = { 0.0f, 0.0f, 0.0f };
    Vec3f effAccel = { 0.0f, 0.0f, 0.0f };
    Vec3f effPos;
    Vec3f dustAccel = { 0.0f, 0.6f, 0.0f };
    Color_RGBA8 dustColor = { 255, 255, 255, 255 };
    EnBomMM* this = THIS;
    Player* player = GET_PLAYER(play);

    if (player->stateFlags1 & PLAYER_STATE1_2)
    {
        return;
    }

    // if (Player_GetMask(play) == PLAYER_MASK_GIANT)
    // {
    //     Actor_Kill(thisx);
    //     return;
    // }

    if (this->unk_1FC != 0)
    {
        this->unk_1FC--;
        Math_ApproachZeroF(&thisx->speed, 1.0f, 1.0f);
        Actor_MoveWithGravity(thisx);
        Actor_UpdateBgCheckInfo(play, thisx, 35.0f, 10.0f, 36.0f, UPDBGCHECKINFO_FLAG_FLOOR);
        if (this->unk_1FC == 0)
        {
            if (this->isPowderKeg)
            {
                gCustomSave.powderKegTimer = 0;
            }
            Actor_Kill(thisx);
        }
    }
    else
    {
        thisx->gravity = -1.2f;
        if (this->timer != 0)
        {
            if (!this->isPowderKeg || (EnBomMM_Explode == this->actionFunc) || !Play_InCsMode(play))
            {
                this->timer--;
            }
        }

        if ((!this->isPowderKeg && (this->timer == 67)) || (this->isPowderKeg && (this->timer <= 2400)))
        {
            Actor_PlaySfx(thisx, NA_SE_PL_TAKE_OUT_SHIELD);
            Actor_SetScale(thisx, enBomScales[this->isPowderKeg]);
        }

        if ((thisx->xzDistToPlayer >= 20.0f) || (fabsf(thisx->yDistanceFromLink) >= 80.0f))
        {
            this->unk_1F8 = 1;
        }

        this->actionFunc(this, play);

        Actor_UpdateBgCheckInfo(play, thisx, 35.0f, 10.0f, 36.0f,
                                UPDBGCHECKINFO_FLAG_WALL | UPDBGCHECKINFO_FLAG_CEILING | UPDBGCHECKINFO_FLAG_FLOOR |
                                    UPDBGCHECKINFO_FLAG_MM_8 | UPDBGCHECKINFO_FLAG_MM_10);
        if (thisx->params == BOMB_TYPE_BODY)
        {
            static Vec3us D_80872ED4[] = {
                { 40, 20, 100 },
                { 300, 60, 600 },
            };
            Vec3us* sp60 = &D_80872ED4[this->isPowderKeg];

            // spawn spark effect on even frames
            dustAccel.y = 0.2f;
            Math_Vec3f_Copy(&effPos, &thisx->home.pos);
            if ((play->gameplayFrames % 2) == 0)
            {
                EffectSsGSpk_SpawnFuse(play, thisx, &effPos, &effVelocity, &effAccel);
            }
            if (this->isPowderKeg)
            {
                /* TODO NA_SE_IT_BIG_BOMB_IGNIT */
                Audio_PlaySfx_AtPosWithChannelIO(&thisx->projectedPos, NA_SE_IT_BOMB_IGNIT - SFX_FLAG,
                                                 (this->flashSpeedScale == 7)   ? 0
                                                 : (this->flashSpeedScale == 3) ? 1
                                                                                : 2);
            }
            else
            {
                Actor_PlaySfx(thisx, NA_SE_IT_BOMB_IGNIT - SFX_FLAG);
            }

            effPos.y += 3.0f;
            func_8002829C(play, &effPos, &effVelocity, &dustAccel, &dustColor, &dustColor, 50, 5);

            if ((this->collider1.base.acFlags & AC_HIT) ||
                ((this->collider1.base.ocFlags1 & OC1_HIT) && ((this->collider1.base.oc->type == ACTORCAT_ENEMY) ||
                                                               (this->collider1.base.oc->type == ACTORCAT_BOSS))))
            {
                this->timer = 0;
                thisx->shape.rot.z = 0;
            }
            else
            {
                // if a lit stick touches the bomb, set timer to 100
                // these bombs never have a timer over 70, so this isn't used
                if ((this->timer > 100) && Player_IsBurningStickInRange(play, &thisx->world.pos, 30.0f, 50.0f))
                {
                    this->timer = 100;
                }
            }

            dustAccel.y = 0.2f;
            effPos = thisx->world.pos;
            effPos.y += 10.0f;

            // double bomb flash speed and adjust red color at certain times during the countdown
            if ((this->timer == sp60->x) || (this->timer == sp60->y) || (this->timer == 3))
            {
                thisx->shape.rot.z = 0;
                this->flashSpeedScale >>= 1;
            }

            if ((this->timer < sp60->z) && (this->timer & (this->flashSpeedScale + 1)))
            {
                Math_ApproachF(&this->unk_1F4, 140.0f, 1.0f, 140.0f / this->flashSpeedScale);
            }
            else
            {
                Math_ApproachZeroF(&this->unk_1F4, 1.0f, 140.0f / this->flashSpeedScale);
            }

            if (this->timer < 3)
            {
                Actor_SetScale(thisx, thisx->scale.x + 0.002f);
            }

            if (this->timer == 0)
            {
                effPos = thisx->world.pos;
                effPos.y += 10.0f;
                if (Actor_HasParent(thisx, play))
                {
                    effPos.y += 30.0f;
                }
                //! @note Assumes `isPowderKeg` values aligns with clearTag params.
                //! Here, 0/1 are small/large explosions respectively.
                Actor_Spawn(&play->actorCtx, play, ACTOR_EN_CLEAR_TAG, effPos.x, effPos.y - 10.0f, effPos.z, 0, 0, 0,
                            CLEAR_TAG_PARAMS(this->isPowderKeg));
                Actor_RequestQuakeAndRumble(thisx, play, sQuakeY[this->isPowderKeg],
                                            sQuakeDurations[this->isPowderKeg]);
                play->envCtx.adjLight1Color[0] = play->envCtx.adjLight1Color[1] =
                    play->envCtx.adjLight1Color[2] = 250;
                play->envCtx.adjAmbientColor[0] = play->envCtx.adjAmbientColor[1] =
                    play->envCtx.adjAmbientColor[2] = 250;
                Camera_RequestQuake(&play->mainCamera, 2, 11, 8);
                thisx->params = BOMB_TYPE_EXPLOSION;
                this->timer = 10;
                thisx->flags |= (ACTOR_FLAG_DRAW_CULLING_DISABLED | ACTOR_FLAG_UPDATE_DURING_OCARINA);
                this->actionFunc = EnBomMM_Explode;
                if (this->isPowderKeg)
                {
                    gCustomSave.powderKegTimer = 0;
                    Actor_PlaySfx(thisx, NA_SE_IT_BOMB_EXPLOSION); /* TODO NA_SE_IT_BIG_BOMB_EXPLOSION */
                }
                else
                {
                    Actor_PlaySfx(thisx, NA_SE_IT_BOMB_EXPLOSION);
                }
            }
        }

        Actor_SetFocus(thisx, 20.0f);

        if (thisx->params <= BOMB_TYPE_BODY)
        {
            Collider_UpdateCylinder(thisx, &this->collider1);

            // if link is not holding the bomb anymore and bump conditions are met, subscribe to OC
            if (!Actor_HasParent(thisx, play) && (this->unk_1F8 != 0))
            {
                CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider1.base);
            }
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider1.base);
        }

        if ((enBomScales[this->isPowderKeg] <= thisx->scale.x) && (thisx->params != BOMB_TYPE_EXPLOSION))
        {
            if (thisx->depthInWater >= 20.0f)
            {
                Vec3f effPos;

                effPos.x = thisx->world.pos.x;
                effPos.y = thisx->world.pos.y + thisx->depthInWater;
                effPos.z = thisx->world.pos.z;
                EffectSsGRipple_Spawn(play, &effPos, 70, 500, 0);
                EffectSsGRipple_Spawn(play, &effPos, 70, 500, 10);
                effPos.y += 10.0f;
                EffectSsGSplash_Spawn(play, &effPos, NULL, NULL, 1, 500);
                Actor_Spawn(&play->actorCtx, play, ACTOR_EN_CLEAR_TAG, effPos.x, effPos.y, effPos.z, 0, 0, 1,
                            CLEAR_TAG_PARAMS(CLEAR_TAG_SMOKE));
                SfxSource_PlaySfxAtFixedWorldPos(play, &thisx->world.pos, 30, NA_SE_IT_BOMB_UNEXPLOSION);
                this->unk_1F4 = 0.0f;
                thisx->velocity.y = (KREG(83) * 0.1f) + -2.0f;
                thisx->gravity = (KREG(84) * 0.1f) + -0.5f;
                this->unk_1FC = KREG(81) + 10;
            }
            else if (thisx->bgCheckFlags & BGCHECKFLAG_WATER_TOUCH)
            {
                thisx->bgCheckFlags &= ~BGCHECKFLAG_WATER_TOUCH;
                Actor_PlaySfx(thisx, NA_SE_EV_BOMB_DROP_WATER);
            }
        }
    }
}

static Vec3f D_80872EE0 = { 0.0f, 800.0f, 0.0f };
static Vec3f D_80872EEC = { -750.0f, 0.0f, 0.0f };
static Vec3f D_80872EF8 = { -800.0f, 0.0f, 0.0f };
static Vec3f D_80872F04 = { 0.0f, 0.0f, 0.0f };

void EnBomMM_Draw(Actor* thisx, PlayState* play)
{
    EnBomMM* this = THIS;

    OPEN_DISPS(play->state.gfxCtx);

    if (this->actor.params == BOMB_TYPE_BODY)
    {
        Gfx_SetupDL25_Opa(play->state.gfxCtx);

        Collider_UpdateSpheres(0, &this->collider2);

        if (!this->isPowderKeg)
        {
            PreDraw1(&this->actor, play, 0);
            Matrix_MultVec3f(&D_80872EE0, &this->actor.home.pos);

            gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_OPA_DISP++, 0x04007A50); /* gBombCapDL */

            Matrix_ReplaceRotation(&play->billboardMtxF);
            Matrix_RotateXS(0x4000, MTXMODE_APPLY);

            gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gDPPipeSync(POLY_OPA_DISP++);
            gDPSetEnvColor(POLY_OPA_DISP++, (s8)this->unk_1F4, 0, 40, 255);
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, (s8)this->unk_1F4, 0, 40, 255);
            gSPDisplayList(POLY_OPA_DISP++, 0x04007860); /* gBombBodyDL */
        }
        else
        {
            Vec3f sp58;
            Vec3f sp4C;

            if (this->unk_1FA != 0)
            {
                s16 sp4A = this->actor.world.rot.y - this->actor.shape.rot.y;
                f32 sp44 = (1000.0f / Math_CosS(ABS_ALT((s16)(this->unk_1FA % 10922)) - 0x1555)) + -1000.0f;

                Matrix_RotateYS(sp4A, MTXMODE_APPLY);
                Matrix_Translate(0.0f, sp44, 0.0f, MTXMODE_APPLY);
                Matrix_RotateXS(this->unk_1FA, MTXMODE_APPLY);
                Matrix_RotateYS(-sp4A, MTXMODE_APPLY);
            }

            Matrix_MultVec3f(&D_80872EEC, &this->actor.home.pos);
            Matrix_MultVec3f(&D_80872EF8, &sp58);
            Matrix_MultVec3f(&D_80872F04, &sp4C);

            gDPSetEnvColor(POLY_OPA_DISP++, 255, 255, 255, 255);
            gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_POWDER_KEG_3);
            gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_POWDER_KEG_4);

            EnBomMM_UpdateKegFuseSegments(play, &this->actor.home.pos, &sp58, &sp4C, this->timer);
            EnBomMM_DrawKegFuseSegments(play, this->timer);
        }
    }

    CLOSE_DISPS();
}

void EnBomMM_InitKegFuseSegments(PlayState* play, Vec3f* bombPos)
{
    PowderKegFuseSegment* fuseSegmentPtr = &sPowderKegFuseSegments[0];
    s32 i;

    for (i = 0; i < ARRAY_COUNT(sPowderKegFuseSegments); i++, fuseSegmentPtr++)
    {
        Math_Vec3f_Copy(&fuseSegmentPtr->pos, bombPos);
        Math_Vec3f_Copy(&fuseSegmentPtr->velocity, &gZeroVec3f);
        fuseSegmentPtr->rotY = 0;
        fuseSegmentPtr->rotX = 0x4000;
    }
}

void EnBomMM_UpdateKegFuseSegments(PlayState* play, Vec3f* fuseRootPos, Vec3f* arg2, Vec3f* arg3, s32 timer)
{
    s32 i;
    f32 temp_f20;
    Vec3f spCC;
    Vec3f spC0;
    PowderKegFuseSegment* fuseSegmentPtr = &sPowderKegFuseSegments[0];
    PowderKegFuseSegment* fuseSegmentPtr2 = &sPowderKegFuseSegments[1];
    f32 temp_f26 = Math_Vec3f_DistXYZ(arg3, fuseRootPos);
    s32 spB0;
    f32 temp_f2;
    f32 distXZ;

    Math_Vec3f_Copy(&fuseSegmentPtr->pos, fuseRootPos);
    Math_Vec3f_Diff(arg2, fuseRootPos, &spCC);

    fuseSegmentPtr->rotY = Math_Atan2S(spCC.z, spCC.x);
    distXZ = sqrtf(SQXZ(spCC));
    fuseSegmentPtr->rotX = Math_Atan2S(distXZ, spCC.y);

    spB0 = (timer / 240) + 1;

    for (i = 0; i < spB0; i++, fuseSegmentPtr++, fuseSegmentPtr2++, timer -= 240)
    {
        f32 phi_f22;
        CollisionPoly* spA0;
        s32 sp9C;
        Vec3f sp90;

        if (timer >= 240)
        {
            phi_f22 = 8.0f;
        }
        else
        {
            phi_f22 = (timer % 240) * (1.0f / 240) * 8.0f;
        }

        Math_Vec3f_Sum(&fuseSegmentPtr2->pos, &fuseSegmentPtr2->velocity, &fuseSegmentPtr2->pos);
        temp_f20 = Math_Vec3f_DistXYZAndStoreDiff(arg3, &fuseSegmentPtr2->pos, &spCC);
        if (temp_f20 < temp_f26)
        {
            if (temp_f20 == 0.0f)
            {
                spCC.x = 0.0f;
                spCC.y = temp_f26;
                spCC.z = 0.0f;
            }
            else
            {
                temp_f20 = temp_f26 / temp_f20;
                spCC.x *= temp_f20;
                spCC.y *= temp_f20;
                spCC.z *= temp_f20;
            }
            Math_Vec3f_Sum(arg3, &spCC, &fuseSegmentPtr2->pos);
        }

        if (Math_Vec3f_DistXYZAndStoreDiff(&fuseSegmentPtr->pos, &fuseSegmentPtr2->pos, &spCC) == 0.0f)
        {
            spCC.x = 0.0f;
            spCC.y = phi_f22;
            spCC.z = 0.0f;
        }

        fuseSegmentPtr2->rotY = Math_Atan2S(spCC.z, spCC.x);
        distXZ = sqrtf(SQXZ(spCC));
        fuseSegmentPtr2->rotX = Math_Atan2S(distXZ, spCC.y);

        fuseSegmentPtr2->rotY =
            (s16)CLAMP(BINANG_SUB(fuseSegmentPtr2->rotY, fuseSegmentPtr->rotY), -8000, 8000) + fuseSegmentPtr->rotY;
        fuseSegmentPtr2->rotX =
            (s16)CLAMP(BINANG_SUB(fuseSegmentPtr2->rotX, fuseSegmentPtr->rotX), -8000, 8000) + fuseSegmentPtr->rotX;

        temp_f20 = Math_CosS(fuseSegmentPtr2->rotX) * phi_f22;
        spC0.x = Math_SinS(fuseSegmentPtr2->rotY) * temp_f20;
        spC0.z = Math_CosS(fuseSegmentPtr2->rotY) * temp_f20;
        spC0.y = Math_SinS(fuseSegmentPtr2->rotX) * phi_f22;

        Math_Vec3f_Sum(&fuseSegmentPtr->pos, &spC0, &fuseSegmentPtr2->pos);
        Math_Vec3f_Copy(&sp90, &fuseSegmentPtr2->pos);

        sp90.y += 50.0f;

        temp_f2 = BgCheck_EntityRaycastDown3(&play->colCtx, &spA0, &sp9C, &sp90) - fuseSegmentPtr2->pos.y;
        if (temp_f2 >= 0.0f)
        {
            spC0.y += temp_f2;
            if (phi_f22 < spC0.y)
            {
                spC0.y = phi_f22;
                temp_f2 = 0.0f;
            }
            else
            {
                temp_f2 = sqrtf(SQ(phi_f22) - SQ(spC0.y));
            }

            if (temp_f20 == 0.0f)
            {
                spC0.x = temp_f2;
            }
            else
            {
                temp_f2 /= temp_f20;
                spC0.x *= temp_f2;
                spC0.z *= temp_f2;
            }

            Math_Vec3f_Sum(&fuseSegmentPtr->pos, &spC0, &fuseSegmentPtr2->pos);
            Math_Vec3f_Copy(&fuseSegmentPtr2->velocity, &gZeroVec3f);
        }

        fuseSegmentPtr2->velocity.y += -1.0f;
        if (fuseSegmentPtr2->velocity.y < -10.0f)
        {
            fuseSegmentPtr2->velocity.y = -10.0f;
        }
    }

    Math_Vec3f_Copy(fuseRootPos, &fuseSegmentPtr->pos);
}

void EnBomMM_DrawKegFuseSegments(PlayState* play, s32 fuseTimer)
{
    s32 temp_s5;
    s32 i;
    PowderKegFuseSegment* fuseSegmentPtr = &sPowderKegFuseSegments[0];
    PowderKegFuseSegment* fuseSegmentPtr2;

    OPEN_DISPS(play->state.gfxCtx);

    Matrix_Translate(fuseSegmentPtr->pos.x, fuseSegmentPtr->pos.y, fuseSegmentPtr->pos.z, MTXMODE_NEW);
    Matrix_RotateZYX(fuseSegmentPtr->rotX, fuseSegmentPtr->rotY, 0, MTXMODE_APPLY);
    Matrix_Scale(0.01f, 0.01f, 0.01f, MTXMODE_APPLY);

    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_POWDER_KEG_0);

    temp_s5 = (fuseTimer / 240) + 1;
    fuseSegmentPtr2 = &sPowderKegFuseSegments[1];

    for (i = 1; i < temp_s5; i++, fuseSegmentPtr2++)
    {
        Matrix_Translate(fuseSegmentPtr2->pos.x, fuseSegmentPtr2->pos.y, fuseSegmentPtr2->pos.z, MTXMODE_NEW);
        Matrix_RotateZYX(fuseSegmentPtr2->rotX, fuseSegmentPtr2->rotY, 0, MTXMODE_APPLY);
        Matrix_Scale(0.01f, 0.01f, 0.01f, MTXMODE_APPLY);

        gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        if ((i % 2) == 0)
        {
            gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_POWDER_KEG_1);
        }
        else
        {
            gSPDisplayList(POLY_OPA_DISP++, CUSTOM_OBJECT_POWDER_KEG_2);
        }
    }

    CLOSE_DISPS();
}

ActorProfile En_BomMM_InitVars = {
    /**/ ACTOR_CUSTOM_EN_BOM,
    /**/ ACTORCAT_EXPLOSIVE,
    /**/ FLAGS,
    /**/ CUSTOM_OBJECT_ID_POWDER_KEG,
    /**/ sizeof(EnBomMM),
    /**/ EnBomMM_Init,
    /**/ EnBomMM_Destroy,
    /**/ EnBomMM_Update,
    /**/ EnBomMM_Draw,
};

OVL_INFO_ACTOR(ACTOR_CUSTOM_EN_BOM, En_BomMM_InitVars);
