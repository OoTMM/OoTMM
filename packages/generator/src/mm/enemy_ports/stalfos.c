

#include "stalfos.h"
#include <combo/enemy_port.h>
#include <combo/effect.h>
#include <combo/matrix.h>
#include <combo/enemy_native.h>
#include <combo/common/animation.h>

#define StalfosNativeBodyParts ((void (*)(Actor*,PlayState*,s32,Gfx**))0x800bbcec)
#define ActorShadow_DrawFeet ((void*)0x800b42f8)
#define Actor_FindNearby ((Actor* (*)(PlayState*,Actor*,s16,u8,float))0x800be0b8)
#define Actor_TestFloorInDirection ((s16 (*)(Actor*,PlayState*,float,s16))0x800bc4ec)
#define Effect_Delete ((void (*)(PlayState*,s32))0x800afdcc)
static void StalfosBlureSpace(void* effect) {
    if (effect)
        ((void (*)(void*))0x800a8514)(effect);
}

static void StalfosBlureVertex(void* effect, Vec3f* first, Vec3f* second) {
    if (effect)
        EffectBlure_AddVertex(effect, first, second);
}
#define EffectBlure_AddSpace StalfosBlureSpace
#define SurfaceType_GetFloorProperty ((s32 (*)(CollisionContext*,CollisionPoly*,s32))0x800c9b40)
#define SkelAnime_InterpFrameTable ((void (*)(s32,Vec3s*,Vec3s*,Vec3s*,float))0x80135448)
#define SkelAnime_CopyFrameTableTrue ((void (*)(SkelAnime*,Vec3s*,Vec3s*,u8*))0x80137674)
#define AnimTaskQueue_AddCopyUsingMap ((void (*)(PlayState*,s32,Vec3s*,Vec3s*,u8*))0x80135b00)
#define Actor_SetFeetPos ((void (*)(Actor*,s32,s32,Vec3f*,s32,Vec3f*))0x800b4a98)
#define func_8002EBCC ((void (*)(Actor*,PlayState*,s32))0x800b8050)
#undef Actor_GetProjectileActor
#define Actor_GetProjectileActor(play,actor,radius) func_800BC270((play),(actor),(radius),~(1u<<4))
#define ACTOR_FLAGS_CHECK_ALL(actor,mask) (((actor)->flags & (mask)) == (mask))
static s32 Actor_IsLockedOn(PlayState* play, Actor* actor) {
    return (GET_PLAYER(play)->stateFlags3 & PLAYER_STATE3_MM_HOSTILE_LOCK_ON) && actor->isTargeted;
}

static s32 Actor_OtherIsLockedOn(PlayState* play, Actor* actor) {
    return (GET_PLAYER(play)->stateFlags3 & PLAYER_STATE3_MM_HOSTILE_LOCK_ON) && !actor->isTargeted;
}

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED)

static void BodyBreak_Alloc(StalfosBodyBreak*, s32, PlayState*);
static void BodyBreak_SetInfo(StalfosBodyBreak*, s32, s32, s32, u32, Gfx**, s16);
static s32 BodyBreak_SpawnParts(Actor*, StalfosBodyBreak*, PlayState*, s16);
void StalfosPort_EnTest_Init(Actor* thisx, PlayState* play);
void StalfosPort_EnTest_Destroy(Actor* thisx, PlayState* play);
void StalfosPort_EnTest_Update(Actor* thisx, PlayState* play);
void StalfosPort_EnTest_Draw(Actor* thisx, PlayState* play);

void StalfosPort_EnTest_SetupWaitGround(EnTest* this);
void StalfosPort_EnTest_SetupWaitAbove(EnTest* this);
void StalfosPort_EnTest_SetupJumpBack(EnTest* this);
void StalfosPort_EnTest_SetupSlashDownEnd(EnTest* this);
void StalfosPort_EnTest_SetupSlashUp(EnTest* this);
void StalfosPort_EnTest_SetupJumpslash(EnTest* this);
void StalfosPort_EnTest_SetupWalkAndBlock(EnTest* this);
void StalfosPort_func_80860EC0(EnTest* this);
void StalfosPort_EnTest_SetupSlashDown(EnTest* this);
void StalfosPort_func_80860BDC(EnTest* this);
void StalfosPort_EnTest_SetupIdleFromBlock(EnTest* this);
void StalfosPort_EnTest_SetupRecoil(EnTest* this);
void StalfosPort_func_80862398(EnTest* this);
void StalfosPort_func_80862154(EnTest* this);
void StalfosPort_EnTest_SetupStopAndBlock(EnTest* this);
void StalfosPort_func_808627C4(EnTest* this, PlayState* play);

void StalfosPort_EnTest_WaitGround(EnTest* this, PlayState* play);
void StalfosPort_EnTest_WaitAbove(EnTest* this, PlayState* play);
void StalfosPort_EnTest_Fall(EnTest* this, PlayState* play);
void StalfosPort_EnTest_Land(EnTest* this, PlayState* play);
void StalfosPort_EnTest_Rise(EnTest* this, PlayState* play);
void StalfosPort_EnTest_Idle(EnTest* this, PlayState* play);
void StalfosPort_EnTest_WalkAndBlock(EnTest* this, PlayState* play);
void StalfosPort_func_80860C24(EnTest* this, PlayState* play);
void StalfosPort_func_80860F84(EnTest* this, PlayState* play);
void StalfosPort_EnTest_SlashDown(EnTest* this, PlayState* play);
void StalfosPort_EnTest_SlashDownEnd(EnTest* this, PlayState* play);
void StalfosPort_EnTest_SlashUp(EnTest* this, PlayState* play);
void StalfosPort_EnTest_JumpBack(EnTest* this, PlayState* play);
void StalfosPort_EnTest_Jumpslash(EnTest* this, PlayState* play);
void StalfosPort_EnTest_JumpUp(EnTest* this, PlayState* play);
void StalfosPort_EnTest_StopAndBlock(EnTest* this, PlayState* play);
void StalfosPort_EnTest_IdleFromBlock(EnTest* this, PlayState* play);
void StalfosPort_func_808621D4(EnTest* this, PlayState* play);
void StalfosPort_func_80862418(EnTest* this, PlayState* play);
void StalfosPort_EnTest_Stunned(EnTest* this, PlayState* play);
void StalfosPort_func_808628C8(EnTest* this, PlayState* play);
void StalfosPort_func_80863044(EnTest* this, PlayState* play);
void StalfosPort_func_8086318C(EnTest* this, PlayState* play);
void StalfosPort_EnTest_Recoil(EnTest* this, PlayState* play);
void StalfosPort_func_808633E8(EnTest* this, PlayState* play);
void StalfosPort_func_80862FA8(EnTest* this, PlayState* play);

s32 StalfosPort_EnTest_ReactToProjectile(PlayState* play, EnTest* this);

static u8 sUpperBodyLimbCopyMap[] = {
    false, false, false, false, true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
    true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
    true,  true,  true,  true,  true,  true,  true,  false, false, false, false, false, false, false, false, false,
    false, false, false, false, false, false, false, false, false, false, false, false, false,
};

ActorProfile StalfosPort_Profile = {
    0xc02,
    ACTORCAT_ENEMY,
    FLAGS,
    0x1032,
    sizeof(EnTest),
    StalfosPort_EnTest_Init,
    StalfosPort_EnTest_Destroy,
    StalfosPort_EnTest_Update,
    StalfosPort_EnTest_Draw,
};

static ColliderCylinderInit sBodyColliderInit = {
    {
        COL_MATERIAL_HIT5,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0x00000000, HIT_SPECIAL_EFFECT_NONE, 0x00 },
        { 0xFFCFFFFF, HIT_BACKLASH_NONE, 0x00 },
        ATELEM_NONE,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 25, 65, 0, { 0, 0, 0 } },
};

static ColliderCylinderInit sShieldColliderInit = {
    {
        COL_MATERIAL_METAL,
        AT_NONE,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0x00000000, HIT_SPECIAL_EFFECT_NONE, 0x00 },
        { 0xFFC1FFFF, HIT_BACKLASH_NONE, 0x00 },
        ATELEM_NONE,
        ACELEM_ON,
        OCELEM_NONE,
    },
    { 20, 70, -50, { 0, 0, 0 } },
};

static ColliderQuadInit sSwordColliderQuadInit = {
    {
        COL_MATERIAL_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_NONE,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_QUAD,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0xFFCFFFFF, HIT_SPECIAL_EFFECT_NONE, 0x10 },
        { 0x00000000, HIT_BACKLASH_NONE, 0x00 },
        ATELEM_ON | ATELEM_SFX_NORMAL | ATELEM_UNK7,
        ACELEM_NONE,
        OCELEM_NONE,
    },
    { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
};

typedef enum StalfosDamageReaction {
    STALFOS_DMG_REACT_NORMAL,
    STALFOS_DMG_REACT_STUN,
    STALFOS_DMG_REACT_FIREMAGIC = 6,
    STALFOS_DMG_REACT_SLING = 0xD,
    STALFOS_DMG_REACT_LIGHT,
    STALFOS_DMG_REACT_FREEZE
} StalfosDamageReaction;

static DamageTable sDamageTable = { {
    DMG_ENTRY(0, STALFOS_DMG_REACT_STUN),   DMG_ENTRY(2, STALFOS_DMG_REACT_NORMAL),
    DMG_ENTRY(1, STALFOS_DMG_REACT_SLING),  DMG_ENTRY(2, STALFOS_DMG_REACT_NORMAL),
    DMG_ENTRY(0, STALFOS_DMG_REACT_STUN),   DMG_ENTRY(2, STALFOS_DMG_REACT_NORMAL),
    DMG_ENTRY(2, STALFOS_DMG_REACT_NORMAL), DMG_ENTRY(0, STALFOS_DMG_REACT_STUN),
    DMG_ENTRY(1, STALFOS_DMG_REACT_NORMAL), DMG_ENTRY(2, STALFOS_DMG_REACT_NORMAL),
    DMG_ENTRY(4, STALFOS_DMG_REACT_NORMAL), DMG_ENTRY(2, STALFOS_DMG_REACT_NORMAL),
    DMG_ENTRY(4, STALFOS_DMG_REACT_FREEZE), DMG_ENTRY(2, STALFOS_DMG_REACT_LIGHT),
    DMG_ENTRY(2, STALFOS_DMG_REACT_NORMAL), DMG_ENTRY(2, STALFOS_DMG_REACT_NORMAL),
    DMG_ENTRY(2, STALFOS_DMG_REACT_NORMAL), DMG_ENTRY(0, STALFOS_DMG_REACT_FIREMAGIC),
    DMG_ENTRY(3, STALFOS_DMG_REACT_FREEZE), DMG_ENTRY(0, STALFOS_DMG_REACT_LIGHT),
    DMG_ENTRY(0, STALFOS_DMG_REACT_NORMAL), DMG_ENTRY(0, STALFOS_DMG_REACT_NORMAL),
    DMG_ENTRY(1, STALFOS_DMG_REACT_NORMAL), DMG_ENTRY(4, STALFOS_DMG_REACT_NORMAL),
    DMG_ENTRY(2, STALFOS_DMG_REACT_NORMAL), DMG_ENTRY(2, STALFOS_DMG_REACT_NORMAL),
    DMG_ENTRY(8, STALFOS_DMG_REACT_NORMAL), DMG_ENTRY(4, STALFOS_DMG_REACT_NORMAL),
    DMG_ENTRY(0, STALFOS_DMG_REACT_NORMAL), DMG_ENTRY(0, STALFOS_DMG_REACT_NORMAL),
    DMG_ENTRY(4, STALFOS_DMG_REACT_NORMAL), DMG_ENTRY(0, STALFOS_DMG_REACT_NORMAL),
} };

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(hintId, 0xff, ICHAIN_CONTINUE),         ICHAIN_F32(targetArrowOffset, 500, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 15, ICHAIN_CONTINUE), ICHAIN_F32(scale.y, 0, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, -1500, ICHAIN_STOP),
};

void StalfosPort_EnTest_SetupAction(EnTest* this, EnTestActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

void StalfosPort_EnTest_Init(Actor* thisx, PlayState* play) {
    EffectBlureInit1 slashBlure;
    EnTest* this = (EnTest*)thisx;

    if (thisx->params != STALFOS_TYPE_CEILING && thisx->params != STALFOS_TYPE_INVISIBLE) {
        Actor_Kill(thisx);
        return;
    }
    Actor_ProcessInitChain(&this->actor, sInitChain);

    SkelAnime_Init(play, &this->skelAnime, (SkeletonHeader*)0x06007c28, (AnimationHeader*)0x0600316c, this->jointTable,
                   this->morphTable, STALFOS_LIMB_MAX);
    SkelAnime_Init(play, &this->upperSkelanime, (SkeletonHeader*)0x06007c28, (AnimationHeader*)0x0600316c,
                   this->upperJointTable, this->upperMorphTable, STALFOS_LIMB_MAX);

    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawFeet, 90.0f);

    this->actor.colChkInfo.cylRadius = 40;
    this->actor.colChkInfo.cylHeight = 100;
    this->actor.focus.pos = this->actor.world.pos;
    this->actor.focus.pos.y += 45.0f;
    this->actor.colChkInfo.damageTable = &sDamageTable;

    Collider_InitCylinder(play, &this->bodyCollider);
    Collider_SetCylinder(play, &this->bodyCollider, &this->actor, &sBodyColliderInit);

    Collider_InitCylinder(play, &this->shieldCollider);
    Collider_SetCylinder(play, &this->shieldCollider, &this->actor, &sShieldColliderInit);

    Collider_InitQuad(play, &this->swordCollider);
    Collider_SetQuad(play, &this->swordCollider, &this->actor, &sSwordColliderQuadInit);
    this->initialized = 1;

    this->actor.colChkInfo.mass = MASS_HEAVY;
    this->actor.colChkInfo.health = 10;

    slashBlure.p1StartColor[0] = slashBlure.p1StartColor[1] = slashBlure.p1StartColor[2] = slashBlure.p1StartColor[3] =
        slashBlure.p2StartColor[0] = slashBlure.p2StartColor[1] = slashBlure.p2StartColor[2] =
            slashBlure.p1EndColor[0] = slashBlure.p1EndColor[1] = slashBlure.p1EndColor[2] = slashBlure.p2EndColor[0] =
                slashBlure.p2EndColor[1] = slashBlure.p2EndColor[2] = 255;

    slashBlure.p1EndColor[3] = 0;
    slashBlure.p2EndColor[3] = 0;
    slashBlure.p2StartColor[3] = 64;

    slashBlure.elemDuration = 4;
    slashBlure.unkFlag = 0;
    slashBlure.calcMode = 2;

    Effect_Add(play, &this->effectIndex, EFFECT_BLURE1, 0, 0, &slashBlure);

    if (this->actor.params != STALFOS_TYPE_CEILING) {
        StalfosPort_EnTest_SetupWaitGround(this);
    } else {
        StalfosPort_EnTest_SetupWaitAbove(this);
    }

    if (this->actor.params == STALFOS_TYPE_INVISIBLE) {
        this->actor.flags |= ACTOR_FLAG_REACT_TO_LENS;
    }
}

void StalfosPort_EnTest_Destroy(Actor* thisx, PlayState* play) {
    EnTest* this = (EnTest*)thisx;

    if (!this->initialized)
        return;

    if (this->bodyBreak.matrices) {
        free(this->bodyBreak.matrices);
        this->bodyBreak.matrices = NULL;
    }
    Effect_Delete(play, this->effectIndex);
    Collider_DestroyCylinder(play, &this->shieldCollider);
    Collider_DestroyCylinder(play, &this->bodyCollider);
    Collider_DestroyQuad(play, &this->swordCollider);
}

void StalfosPort_EnTest_ChooseRandomAction(EnTest* this, PlayState* play) {
    switch ((u32)(Rand_ZeroOne() * 10.0f)) {
        case 0:
        case 1:
        case 5:
        case 6:
            if ((this->actor.xzDistToPlayer < 220.0f) && (this->actor.xzDistToPlayer > 170.0f) &&
                Actor_IsFacingPlayer(&this->actor, 0x71C) && Actor_IsLockedOn(play, &this->actor)) {
                StalfosPort_EnTest_SetupJumpslash(this);
                break;
            }
            __attribute__((fallthrough));
        case 8:
            StalfosPort_EnTest_SetupWalkAndBlock(this);
            break;

        case 3:
        case 4:
        case 7:
            StalfosPort_func_808627C4(this, play);
            break;

        case 2:
        case 9:
        case 10:
            StalfosPort_EnTest_SetupStopAndBlock(this);
            break;
    }
}

void StalfosPort_EnTest_ChooseAction(EnTest* this, PlayState* play) {

    Player* player = GET_PLAYER(play);
    s16 yawDiff = player->actor.shape.rot.y - this->actor.shape.rot.y;

    yawDiff = ABS(yawDiff);

    if (yawDiff >= 0x61A8) {
        switch ((u32)(Rand_ZeroOne() * 10.0f)) {
            case 0:
            case 3:
            case 7:
                StalfosPort_EnTest_SetupStopAndBlock(this);
                break;

            case 1:
            case 5:
            case 6:
            case 8:
                StalfosPort_func_808627C4(this, play);
                break;

            case 2:
            case 4:
            case 9:
                if (this->actor.params != STALFOS_TYPE_CEILING) {
                    this->actor.world.rot.y = this->actor.yawTowardsPlayer;
                    StalfosPort_EnTest_SetupJumpBack(this);
                }
                break;
        }
    } else if (yawDiff <= 0x3E80) {
        if (ABS((s16)(this->actor.yawTowardsPlayer - this->actor.shape.rot.y)) > 0x3E80) {
            if (((play->gameplayFrames % 2) != 0) && (this->actor.params != STALFOS_TYPE_CEILING)) {
                this->actor.world.rot.y = this->actor.yawTowardsPlayer;
                StalfosPort_EnTest_SetupJumpBack(this);
            } else if ((this->actor.xzDistToPlayer < 220.0f) && (this->actor.xzDistToPlayer > 170.0f)) {
                if (Actor_IsFacingPlayer(&this->actor, 0x71C) && !Actor_IsLockedOn(play, &this->actor)) {
                    StalfosPort_EnTest_SetupJumpslash(this);
                }
            } else {
                StalfosPort_EnTest_SetupWalkAndBlock(this);
            }
        } else {
            if (this->actor.xzDistToPlayer < 110.0f) {
                if (Rand_ZeroOne() > 0.2f) {
                    if (player->stateFlags3 & PLAYER_STATE3_MM_HOSTILE_LOCK_ON) {
                        if (this->actor.isTargeted) {
                            StalfosPort_EnTest_SetupSlashDown(this);
                        } else {
                            StalfosPort_func_808627C4(this, play);
                        }
                    } else {
                        StalfosPort_EnTest_SetupSlashDown(this);
                    }
                }
            } else {
                StalfosPort_EnTest_ChooseRandomAction(this, play);
            }
        }
    } else {
        StalfosPort_EnTest_ChooseRandomAction(this, play);
    }
}

void StalfosPort_EnTest_SetupWaitGround(EnTest* this) {
    Animation_PlayLoop(&this->skelAnime, (AnimationHeader*)0x0600316c);
    this->unk_7C8 = 0;
    this->timer = 15;
    this->actor.scale.y = 0.0f;
    this->actor.world.pos.y = this->actor.home.pos.y - 3.5f;
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    StalfosPort_EnTest_SetupAction(this, StalfosPort_EnTest_WaitGround);
}

void StalfosPort_EnTest_WaitGround(EnTest* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);

    if ((this->timer == 0) && (ABS(this->actor.yDistanceFromLink) < 150.0f)) {
        this->unk_7C8 = 3;
        StalfosPort_EnTest_SetupAction(this, StalfosPort_EnTest_Rise);
        this->actor.world.rot.y = this->actor.yawTowardsPlayer;
        this->actor.shape.rot.y = this->actor.yawTowardsPlayer;

        if (this->actor.params != STALFOS_TYPE_2) {
        }
    } else {
        if (this->timer != 0) {
            this->timer--;
        }

        this->actor.world.pos.y = this->actor.home.pos.y - 3.5f;
    }
}

void StalfosPort_EnTest_SetupWaitAbove(EnTest* this) {
    Animation_PlayLoop(&this->skelAnime, (AnimationHeader*)0x0600316c);
    this->unk_7C8 = 0;
    this->actor.world.pos.y = this->actor.home.pos.y + 150.0f;
    Actor_SetScale(&this->actor, 0.0f);
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    StalfosPort_EnTest_SetupAction(this, StalfosPort_EnTest_WaitAbove);
}

void StalfosPort_EnTest_WaitAbove(EnTest* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);
    this->actor.world.pos.y = this->actor.home.pos.y + 150.0f;

    if ((this->actor.xzDistToPlayer < 200.0f) && (ABS(this->actor.yDistanceFromLink) < 450.0f)) {
        StalfosPort_EnTest_SetupAction(this, StalfosPort_EnTest_Fall);
        this->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED;
        this->actor.shape.rot.y = this->actor.world.rot.y = this->actor.yawTowardsPlayer;
        Actor_SetScale(&this->actor, 0.015f);
    }
}

void StalfosPort_EnTest_SetupIdle(EnTest* this) {
    Animation_PlayLoop(&this->skelAnime, (AnimationHeader*)0x0600316c);
    this->unk_7C8 = 0xA;
    this->timer = (Rand_ZeroOne() * 10.0f) + 5.0f;
    this->actor.speed = 0.0f;
    this->actor.world.rot.y = this->actor.shape.rot.y;
    StalfosPort_EnTest_SetupAction(this, StalfosPort_EnTest_Idle);
}

void StalfosPort_EnTest_Idle(EnTest* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 yawDiff;

    SkelAnime_Update(&this->skelAnime);

    if (!StalfosPort_EnTest_ReactToProjectile(play, this)) {
        yawDiff = player->actor.shape.rot.y - this->actor.shape.rot.y;

        if (this->actor.xzDistToPlayer < 100.0f) {
            if ((player->meleeWeaponState != 0) && (ABS(yawDiff) >= 0x1F40)) {
                this->actor.shape.rot.y = this->actor.world.rot.y = this->actor.yawTowardsPlayer;

                if (Rand_ZeroOne() > 0.7f && player->meleeWeaponAnimation != PLAYER_MWA_JUMPSLASH_START) {
                    StalfosPort_EnTest_SetupJumpBack(this);
                } else {
                    StalfosPort_func_808627C4(this, play);
                }
                return;
            }
        }

        if (this->timer != 0) {
            this->timer--;
        } else {
            if (Actor_IsFacingPlayer(&this->actor, 0x1555)) {
                if ((this->actor.xzDistToPlayer < 220.0f) && (this->actor.xzDistToPlayer > 160.0f) &&
                    (Rand_ZeroOne() < 0.3f)) {
                    if (Actor_IsLockedOn(play, &this->actor)) {
                        StalfosPort_EnTest_SetupJumpslash(this);
                    } else {
                        StalfosPort_func_808627C4(this, play);
                    }
                } else {
                    if (Rand_ZeroOne() > 0.3f) {
                        StalfosPort_EnTest_SetupWalkAndBlock(this);
                    } else {
                        StalfosPort_func_808627C4(this, play);
                    }
                }
            } else {
                if (Rand_ZeroOne() > 0.7f) {
                    StalfosPort_func_80860BDC(this);
                } else {
                    StalfosPort_EnTest_ChooseAction(this, play);
                }
            }
        }
    }
}

void StalfosPort_EnTest_Fall(EnTest* this, PlayState* play) {
    Animation_PlayOnceSetSpeed(&this->skelAnime, (AnimationHeader*)0x0600c438, 0.0f);
    SkelAnime_Update(&this->skelAnime);

    if (this->actor.world.pos.y <= this->actor.floorHeight) {
        this->skelAnime.playSpeed = 1.0f;
        this->unk_7C8 = 0xC;
        this->timer = this->unk_7E4 * 0.15f;
        Actor_PlaySfx(&this->actor, 0x387a);
        StalfosPort_EnTest_SetupAction(this, StalfosPort_EnTest_Land);
    }
}

void StalfosPort_EnTest_Land(EnTest* this, PlayState* play) {
    if (SkelAnime_Update(&this->skelAnime)) {
        StalfosPort_EnTest_SetupIdle(this);
        this->timer = (Rand_ZeroOne() * 10.0f) + 5.0f;
    }
}

void StalfosPort_EnTest_SetupWalkAndBlock(EnTest* this) {
    Animation_Change(&this->upperSkelanime, (AnimationHeader*)0x06001c20, 2.0f, 0.0f,
                     Animation_GetLastFrame((AnimationHeader*)0x06001c20), 2, 2.0f);
    Animation_PlayLoop(&this->skelAnime, (AnimationHeader*)0x060081b4);
    this->timer = (s16)(Rand_ZeroOne() * 5.0f);
    this->unk_7C8 = 0xD;
    this->actor.world.rot.y = this->actor.shape.rot.y;
    StalfosPort_EnTest_SetupAction(this, StalfosPort_EnTest_WalkAndBlock);
}

void StalfosPort_EnTest_WalkAndBlock(EnTest* this, PlayState* play) {

    f32 checkDist = 0.0f;
    s32 absPlaySpeed;
    s32 prevFrame;
    s32 beforeCurFrame;
    f32 playSpeed;
    Player* player = GET_PLAYER(play);
    s16 yawDiff;

    if (!StalfosPort_EnTest_ReactToProjectile(play, this)) {
        this->timer++;

        if (Actor_OtherIsLockedOn(play, &this->actor)) {
            checkDist = 150.0f;
        }

        if (this->actor.xzDistToPlayer <= (80.0f + checkDist)) {
            Math_SmoothStepToF(&this->actor.speed, -5.0f, 1.0f, 0.8f, 0.0f);
        } else if (this->actor.xzDistToPlayer > (110.0f + checkDist)) {
            Math_SmoothStepToF(&this->actor.speed, 5.0f, 1.0f, 0.8f, 0.0f);
        }

        if (this->actor.speed >= 5.0f) {
            this->actor.speed = 5.0f;
        } else if (this->actor.speed < -5.0f) {
            this->actor.speed = -5.0f;
        }

        if ((this->actor.params == STALFOS_TYPE_CEILING) &&
            !Actor_TestFloorInDirection(&this->actor, play, this->actor.speed, this->actor.world.rot.y)) {
            this->actor.speed *= -1.0f;
        }

        if (ABS(this->actor.speed) < 3.0f) {

            Animation_Change(&this->skelAnime, (AnimationHeader*)0x060081b4, 0.0f, this->skelAnime.curFrame,
                             Animation_GetLastFrame((AnimationHeader*)0x060081b4), 0, -6.0f);
            playSpeed = this->actor.speed * 10.0f;
        } else {
            Animation_Change(&this->skelAnime, (AnimationHeader*)0x060026d4, 0.0f, this->skelAnime.curFrame,
                             Animation_GetLastFrame((AnimationHeader*)0x060026d4), 0, -4.0f);
            playSpeed = this->actor.speed * 10.0f * 0.02f;
        }

        if (this->actor.speed >= 0.0f) {
            if (this->unk_7DE == 0) {
                this->unk_7DE++;
            }

            playSpeed = CLAMP_MAX(playSpeed, 2.5f);
            this->skelAnime.playSpeed = playSpeed;
        } else {
            playSpeed = CLAMP_MIN(playSpeed, -2.5f);
            this->skelAnime.playSpeed = playSpeed;
        }

        yawDiff = player->actor.shape.rot.y - this->actor.shape.rot.y;

        if ((this->actor.xzDistToPlayer < 100.0f) && (player->meleeWeaponState != 0)) {
            if (ABS(yawDiff) >= 0x1F40) {
                this->actor.shape.rot.y = this->actor.world.rot.y = this->actor.yawTowardsPlayer;

                if ((Rand_ZeroOne() > 0.7f) && (player->meleeWeaponAnimation != PLAYER_MWA_JUMPSLASH_START)) {
                    StalfosPort_EnTest_SetupJumpBack(this);
                } else {
                    StalfosPort_EnTest_SetupStopAndBlock(this);
                }

                return;
            }
        }

        prevFrame = (s32)this->skelAnime.curFrame;
        SkelAnime_Update(&this->skelAnime);
        beforeCurFrame = (s32)(this->skelAnime.curFrame - ABS(this->skelAnime.playSpeed));
        absPlaySpeed = (s32)(f32)ABS(this->skelAnime.playSpeed);

        if ((s32)this->skelAnime.curFrame != prevFrame) {
            s32 afterPrevFrame = absPlaySpeed + prevFrame;

            if (((afterPrevFrame > 1) && (beforeCurFrame < 1)) || ((beforeCurFrame < 7) && (afterPrevFrame > 7))) {
                Actor_PlaySfx(&this->actor, 0x383d);
            }
        }

        if ((this->timer % 32) == 0) {
            Actor_PlaySfx(&this->actor, 0x3838);
            this->timer += (s16)(Rand_ZeroOne() * 5.0f);
        }

        if ((this->actor.xzDistToPlayer < 220.0f) && (this->actor.xzDistToPlayer > 160.0f) &&
            (Actor_IsFacingPlayer(&this->actor, 0x71C))) {
            if (Actor_IsLockedOn(play, &this->actor)) {
                if (Rand_ZeroOne() < 0.1f) {
                    StalfosPort_EnTest_SetupJumpslash(this);
                    return;
                }
            } else if (player->heldItemAction != PLAYER_IA_NONE) {
                if (this->actor.isTargeted) {
                    if ((play->gameplayFrames % 2) != 0) {
                        StalfosPort_func_808627C4(this, play);
                        return;
                    }

                    StalfosPort_EnTest_ChooseAction(this, play);
                } else {
                    StalfosPort_func_80860EC0(this);
                }
            }
        }

        if (Rand_ZeroOne() < 0.4f) {
            this->actor.shape.rot.y = this->actor.world.rot.y = this->actor.yawTowardsPlayer;
        }

        if (!Actor_IsFacingPlayer(&this->actor, 0x11C7)) {
            StalfosPort_EnTest_SetupIdle(this);
            this->timer = (Rand_ZeroOne() * 10.0f) + 10.0f;
            return;
        }

        if (this->actor.xzDistToPlayer < 110.0f) {
            if (Rand_ZeroOne() > 0.2f) {
                if (player->stateFlags3 & PLAYER_STATE3_MM_HOSTILE_LOCK_ON) {
                    if (this->actor.isTargeted) {
                        StalfosPort_EnTest_SetupSlashDown(this);
                    } else {
                        StalfosPort_func_808627C4(this, play);
                    }
                } else {
                    StalfosPort_EnTest_SetupSlashDown(this);
                }
            } else {
                StalfosPort_EnTest_SetupStopAndBlock(this);
            }
        } else if (Rand_ZeroOne() < 0.1f) {
            this->actor.speed = 5.0f;
        }
    }
}

void StalfosPort_func_80860BDC(EnTest* this) {
    Animation_PlayLoop(&this->skelAnime, (AnimationHeader*)0x0600e2b0);
    this->unk_7C8 = 0xE;
    StalfosPort_EnTest_SetupAction(this, StalfosPort_func_80860C24);
}

void StalfosPort_func_80860C24(EnTest* this, PlayState* play) {
    s16 yawDiff;
    s16 yawChange;
    f32 playSpeed;
    s32 prevFrame;
    s32 beforeCurFrame;
    s32 afterPrevFrame;
    s32 absPlaySpeed;

    if (!StalfosPort_EnTest_ReactToProjectile(play, this)) {
        yawDiff = this->actor.yawTowardsPlayer;
        yawDiff -= this->actor.shape.rot.y;

        if (yawDiff > 0) {
            yawChange = (yawDiff / 42.0f) + 300.0f;
            this->actor.shape.rot.y += yawChange * 2;
        } else {
            yawChange = (yawDiff / 42.0f) - 300.0f;
            this->actor.shape.rot.y += yawChange * 2;
        }

        this->actor.world.rot.y = this->actor.shape.rot.y;

        if (yawDiff > 0) {
            playSpeed = yawChange * 0.02f;
            playSpeed = CLAMP_MAX(playSpeed, 1.0f);
            this->skelAnime.playSpeed = playSpeed;
        } else {
            playSpeed = yawChange * 0.02f;
            playSpeed = CLAMP_MIN(playSpeed, -1.0f);
            this->skelAnime.playSpeed = playSpeed;
        }

        prevFrame = (s32)this->skelAnime.curFrame;
        SkelAnime_Update(&this->skelAnime);
        beforeCurFrame = (s32)(this->skelAnime.curFrame - ABS(this->skelAnime.playSpeed));
        absPlaySpeed = (s32)(f32)ABS(this->skelAnime.playSpeed);

        if ((s32)this->skelAnime.curFrame != prevFrame) {
            afterPrevFrame = absPlaySpeed + prevFrame;

            if (((afterPrevFrame > 2) && (beforeCurFrame <= 0)) || ((beforeCurFrame < 7) && (afterPrevFrame >= 9))) {
                Actor_PlaySfx(&this->actor, 0x383d);
            }
        }

        if (Actor_IsFacingPlayer(&this->actor, 0x71C)) {
            if (Rand_ZeroOne() > 0.8f) {
                if ((Rand_ZeroOne() > 0.7f)) {
                    StalfosPort_func_80860EC0(this);
                } else {
                    StalfosPort_EnTest_ChooseAction(this, play);
                }
            } else {
                StalfosPort_EnTest_SetupWalkAndBlock(this);
            }
        }
    }
}

void StalfosPort_func_80860EC0(EnTest* this) {
    Animation_PlayLoop(&this->skelAnime, (AnimationHeader*)0x0600e2b0);
    this->unk_7C8 = 0xF;
    this->actor.speed = (Rand_ZeroOne() > 0.5f) ? -0.5f : 0.5f;
    this->timer = (s16)((Rand_ZeroOne() * 15.0f) + 25.0f);
    this->unk_7EC = 0.0f;
    this->actor.world.rot.y = this->actor.shape.rot.y;
    StalfosPort_EnTest_SetupAction(this, StalfosPort_func_80860F84);
}

void StalfosPort_func_80860F84(EnTest* this, PlayState* play) {
    s16 playerYaw180;

    s32 prevFrame;
    s32 beforeCurFrame;
    s16 yawDiff;
    Player* player = GET_PLAYER(play);
    f32 checkDist = 0.0f;
    s16 newYaw;
    s32 absPlaySpeed;

    if (!StalfosPort_EnTest_ReactToProjectile(play, this)) {
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0xFA0, 1);
        this->actor.world.rot.y = this->actor.shape.rot.y + 0x3E80;
        playerYaw180 = player->actor.shape.rot.y + 0x8000;

        if (this->actor.speed >= 0.0f) {
            if (this->actor.speed < 6.0f) {
                this->actor.speed += 0.5f;
            } else {
                this->actor.speed = 6.0f;
            }
        } else {
            if (this->actor.speed > -6.0f) {
                this->actor.speed -= 0.5f;
            } else {
                this->actor.speed = -6.0f;
            }
        }

        if ((this->actor.bgCheckFlags & BGCHECKFLAG_WALL) ||
            ((this->actor.params == STALFOS_TYPE_CEILING) &&
             !Actor_TestFloorInDirection(&this->actor, play, this->actor.speed, this->actor.world.rot.y))) {
            if (this->actor.bgCheckFlags & BGCHECKFLAG_WALL) {
                if (this->actor.speed >= 0.0f) {
                    newYaw = this->actor.shape.rot.y + 0x3FFF;
                } else {
                    newYaw = this->actor.shape.rot.y - 0x3FFF;
                }

                newYaw = this->actor.wallYaw - newYaw;
            } else {
                this->actor.speed *= -0.8f;
                newYaw = 0;
            }

            if (ABS(newYaw) > 0x4000) {
                this->actor.speed *= -0.8f;

                if (this->actor.speed < 0.0f) {
                    this->actor.speed -= 0.5f;
                } else {
                    this->actor.speed += 0.5f;
                }
            }
        }

        if (Actor_OtherIsLockedOn(play, &this->actor)) {
            checkDist = 200.0f;
        }

        if (this->actor.xzDistToPlayer <= (80.0f + checkDist)) {
            Math_SmoothStepToF(&this->unk_7EC, -2.5f, 1.0f, 0.8f, 0.0f);
        } else if (this->actor.xzDistToPlayer > (110.0f + checkDist)) {
            Math_SmoothStepToF(&this->unk_7EC, 2.5f, 1.0f, 0.8f, 0.0f);
        } else {
            Math_SmoothStepToF(&this->unk_7EC, 0.0f, 1.0f, 6.65f, 0.0f);
        }

        if (this->unk_7EC != 0.0f) {
            this->actor.world.pos.x += Math_SinS(this->actor.shape.rot.y) * this->unk_7EC;
            this->actor.world.pos.z += Math_CosS(this->actor.shape.rot.y) * this->unk_7EC;
        }

        this->skelAnime.playSpeed = this->actor.speed * 0.5f;

        prevFrame = (s32)this->skelAnime.curFrame;
        SkelAnime_Update(&this->skelAnime);
        beforeCurFrame = (s32)(this->skelAnime.curFrame - ABS(this->skelAnime.playSpeed));
        absPlaySpeed = (s32)(f32)ABS(this->skelAnime.playSpeed);

        if ((s32)this->skelAnime.curFrame != prevFrame) {
            s32 afterPrevFrame = absPlaySpeed + prevFrame;

            if (((afterPrevFrame > 1) && (beforeCurFrame < 1)) || ((beforeCurFrame < 7) && (afterPrevFrame > 7))) {
                Actor_PlaySfx(&this->actor, 0x383d);
            }
        }

        if ((play->gameplayFrames & 95) == 0) {
            Actor_PlaySfx(&this->actor, 0x3838);
        }

        yawDiff = playerYaw180 - this->actor.shape.rot.y;
        yawDiff = ABS(yawDiff);

        if ((yawDiff > 0x6800) || (this->timer == 0)) {
            StalfosPort_EnTest_ChooseAction(this, play);
        } else if (this->timer != 0) {
            this->timer--;
        }
    }
}

void StalfosPort_EnTest_SetupSlashDown(EnTest* this) {
    Animation_PlayOnce(&this->skelAnime, (AnimationHeader*)0x0600b00c);
    Audio_StopSfxByPosAndId(&this->actor.projectedPos, 0x3838);
    this->swordCollider.base.atFlags &= ~AT_BOUNCED;
    this->unk_7C8 = 0x10;
    this->actor.speed = 0.0f;
    StalfosPort_EnTest_SetupAction(this, StalfosPort_EnTest_SlashDown);
    this->swordCollider.elem.atDmgInfo.damage = 16;

    if (this->unk_7DE != 0) {
        this->unk_7DE = 3;
    }
}

void StalfosPort_EnTest_SlashDown(EnTest* this, PlayState* play) {
    this->actor.speed = 0.0f;

    if ((s32)this->skelAnime.curFrame < 4) {
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0xBB8, 0);
    }

    if ((s32)this->skelAnime.curFrame == 7) {
        Actor_PlaySfx(&this->actor, 0x3839);
    }

    if ((this->skelAnime.curFrame > 7.0f) && (this->skelAnime.curFrame < 11.0f)) {
        this->swordState = 1;
    } else {
        this->swordState = 0;
    }

    if (SkelAnime_Update(&this->skelAnime)) {
        if ((play->gameplayFrames % 2) != 0) {
            StalfosPort_EnTest_SetupSlashDownEnd(this);
        } else {
            StalfosPort_EnTest_SetupSlashUp(this);
        }
    }
}

void StalfosPort_EnTest_SetupSlashDownEnd(EnTest* this) {
    Animation_PlayOnce(&this->skelAnime, (AnimationHeader*)0x0600b4e4);
    this->unk_7C8 = 0x12;
    this->actor.speed = 0.0f;
    StalfosPort_EnTest_SetupAction(this, StalfosPort_EnTest_SlashDownEnd);
}

void StalfosPort_EnTest_SlashDownEnd(EnTest* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 yawDiff;

    if (SkelAnime_Update(&this->skelAnime)) {
        if (this->swordCollider.base.atFlags & AT_HIT) {
            this->swordCollider.base.atFlags &= ~AT_HIT;
            if (this->actor.params != STALFOS_TYPE_CEILING) {
                StalfosPort_EnTest_SetupJumpBack(this);
                return;
            }
        }

        if (Rand_ZeroOne() > 0.7f) {
            StalfosPort_EnTest_SetupIdle(this);
            this->timer = (Rand_ZeroOne() * 5.0f) + 5.0f;
            return;
        }

        this->actor.world.rot.y = Actor_WorldYawTowardActor(&this->actor, &player->actor);

        if (Rand_ZeroOne() > 0.7f) {
            if (this->actor.params != STALFOS_TYPE_CEILING) {
                StalfosPort_EnTest_SetupJumpBack(this);
                return;
            }
        }

        yawDiff = player->actor.shape.rot.y - this->actor.shape.rot.y;

        if (ABS(yawDiff) <= 0x2710) {
            yawDiff = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;

            if ((ABS(yawDiff) > 0x3E80) && (this->actor.params != STALFOS_TYPE_CEILING)) {
                this->actor.world.rot.y = this->actor.yawTowardsPlayer;
                StalfosPort_EnTest_SetupJumpBack(this);
            } else if (player->stateFlags3 & PLAYER_STATE3_MM_HOSTILE_LOCK_ON) {
                if (this->actor.isTargeted) {
                    StalfosPort_EnTest_SetupSlashDown(this);
                } else if ((play->gameplayFrames % 2) != 0) {
                    StalfosPort_func_808627C4(this, play);
                } else {
                    StalfosPort_EnTest_SetupJumpBack(this);
                }
            } else {
                StalfosPort_EnTest_SetupSlashDown(this);
            }
        } else {
            StalfosPort_func_808627C4(this, play);
        }
    }
}

void StalfosPort_EnTest_SetupSlashUp(EnTest* this) {
    Animation_PlayOnce(&this->skelAnime, (AnimationHeader*)0x0600be4c);
    this->swordCollider.base.atFlags &= ~AT_BOUNCED;
    this->unk_7C8 = 0x11;
    this->swordCollider.elem.atDmgInfo.damage = 16;
    this->actor.speed = 0.0f;
    StalfosPort_EnTest_SetupAction(this, StalfosPort_EnTest_SlashUp);

    if (this->unk_7DE != 0) {
        this->unk_7DE = 3;
    }
}

void StalfosPort_EnTest_SlashUp(EnTest* this, PlayState* play) {
    this->actor.speed = 0.0f;

    if ((s32)this->skelAnime.curFrame == 2) {
        Actor_PlaySfx(&this->actor, 0x3839);
    }

    if ((this->skelAnime.curFrame > 1.0f) && (this->skelAnime.curFrame < 8.0f)) {
        this->swordState = 1;
    } else {
        this->swordState = 0;
    }

    if (SkelAnime_Update(&this->skelAnime)) {
        StalfosPort_EnTest_SetupSlashDown(this);
    }
}

void StalfosPort_EnTest_SetupJumpBack(EnTest* this) {
    Animation_PlayOnce(&this->skelAnime, (AnimationHeader*)0x06001978);
    Actor_PlaySfx(&this->actor, 0x386c);
    this->unk_7C8 = 0x14;
    this->timer = 5;
    StalfosPort_EnTest_SetupAction(this, StalfosPort_EnTest_JumpBack);

    if (this->unk_7DE != 0) {
        this->unk_7DE = 3;
    }

    if (this->actor.params != STALFOS_TYPE_CEILING) {
        this->actor.speed = -11.0f;
    } else {
        this->actor.speed = -7.0f;
    }
}

void StalfosPort_EnTest_JumpBack(EnTest* this, PlayState* play) {
    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0xBB8, 1);

    if (this->timer == 0) {
        Actor_PlaySfx(&this->actor, 0x3838);
    } else {
        this->timer--;
    }

    if (SkelAnime_Update(&this->skelAnime)) {
        if (!StalfosPort_EnTest_ReactToProjectile(play, this)) {
            if (this->actor.xzDistToPlayer <= 100.0f) {
                if (Actor_IsFacingPlayer(&this->actor, 0x1555)) {
                    StalfosPort_EnTest_SetupSlashDown(this);
                } else {
                    StalfosPort_EnTest_SetupIdle(this);
                    this->timer = (Rand_ZeroOne() * 5.0f) + 5.0f;
                }
            } else {
                if ((this->actor.xzDistToPlayer <= 220.0f) && Actor_IsFacingPlayer(&this->actor, 0xE38)) {
                    StalfosPort_EnTest_SetupJumpslash(this);
                } else {
                    StalfosPort_EnTest_SetupIdle(this);
                    this->timer = (Rand_ZeroOne() * 5.0f) + 5.0f;
                }
            }
            this->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED;
        }
    } else if (this->skelAnime.curFrame == (this->skelAnime.endFrame - 4.0f)) {
        Actor_PlaySfx(&this->actor, 0x387b);
    }
}

void StalfosPort_EnTest_SetupJumpslash(EnTest* this) {
    Animation_PlayOnce(&this->skelAnime, (AnimationHeader*)0x0600a324);
    Audio_StopSfxByPosAndId(&this->actor.projectedPos, 0x3838);
    this->timer = 0;
    this->unk_7C8 = 0x17;
    this->actor.velocity.y = 10.0f;
    this->actor.speed = 8.0f;
    Actor_PlaySfx(&this->actor, 0x386c);
    this->actor.world.rot.y = this->actor.shape.rot.y;
    this->swordCollider.base.atFlags &= ~AT_BOUNCED;
    StalfosPort_EnTest_SetupAction(this, StalfosPort_EnTest_Jumpslash);
    this->swordCollider.elem.atDmgInfo.damage = 32;

    if (this->unk_7DE != 0) {
        this->unk_7DE = 3;
    }
}

void StalfosPort_EnTest_Jumpslash(EnTest* this, PlayState* play) {
    if (SkelAnime_Update(&this->skelAnime)) {
        if (this->timer == 0) {
            Animation_PlayOnce(&this->skelAnime, (AnimationHeader*)0x0600a99c);
            this->timer = 1;
            this->swordState = 1;
            Actor_PlaySfx(&this->actor, 0x3839);
            Actor_PlaySfx(&this->actor, 0x386c);
        } else {
            this->actor.speed = 0.0f;
            StalfosPort_EnTest_SetupIdle(this);
        }
    }

    if ((this->timer != 0) && (this->skelAnime.curFrame >= 5.0f)) {
        this->swordState = 0;
    }

    if (this->actor.world.pos.y <= this->actor.floorHeight) {
        if (this->actor.speed != 0.0f) {
            Actor_PlaySfx(&this->actor, 0x387b);
        }

        this->actor.world.pos.y = this->actor.floorHeight;
        this->actor.velocity.y = 0.0f;
        this->actor.speed = 0.0f;
    }
}

void StalfosPort_EnTest_SetupJumpUp(EnTest* this) {
    Animation_PlayOnce(&this->skelAnime, (AnimationHeader*)0x0600a324);
    this->timer = 0;
    this->unk_7C8 = 4;
    this->actor.velocity.y = 14.0f;
    this->actor.speed = 6.0f;
    Actor_PlaySfx(&this->actor, 0x386c);
    this->actor.world.rot.y = this->actor.shape.rot.y;
    StalfosPort_EnTest_SetupAction(this, StalfosPort_EnTest_JumpUp);
}

void StalfosPort_EnTest_JumpUp(EnTest* this, PlayState* play) {
    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0xFA0, 1);
    SkelAnime_Update(&this->skelAnime);

    if (this->actor.world.pos.y <= this->actor.floorHeight) {
        Actor_PlaySfx(&this->actor, 0x387b);
        this->actor.shape.rot.y = this->actor.yawTowardsPlayer;
        this->actor.world.pos.y = this->actor.floorHeight;
        this->unk_7E4 = -(s32)this->actor.velocity.y;

        if (this->unk_7E4 == 0) {
            this->unk_7E4 = 1;
        }

        this->actor.velocity.y = 0.0f;
        this->actor.speed = 0.0f;
        this->unk_7C8 = 0xC;
        this->timer = 4;
        Animation_Change(&this->skelAnime, (AnimationHeader*)0x0600c438, 0.0f, 0.0f, 0.0f, 2, 0.0f);
        StalfosPort_EnTest_SetupAction(this, StalfosPort_EnTest_Land);
    }
}

void StalfosPort_EnTest_SetupStopAndBlock(EnTest* this) {
    Animation_Change(&this->skelAnime, (AnimationHeader*)0x06001c20, 2.0f, 0.0f,
                     Animation_GetLastFrame((AnimationHeader*)0x06001c20), 2, 2.0f);
    this->unk_7C8 = 0x15;
    this->actor.speed = 0.0f;
    this->timer = (Rand_ZeroOne() * 10.0f) + 11.0f;
    this->actor.world.rot.y = this->actor.shape.rot.y;
    this->unk_7DE = 5;
    StalfosPort_EnTest_SetupAction(this, StalfosPort_EnTest_StopAndBlock);
}

void StalfosPort_EnTest_StopAndBlock(EnTest* this, PlayState* play) {
    Math_SmoothStepToF(&this->actor.speed, 0.0f, 1.0f, 0.5f, 0.0f);
    SkelAnime_Update(&this->skelAnime);

    if ((ABS((s16)(this->actor.yawTowardsPlayer - this->actor.shape.rot.y)) > 0x3E80) &&
        (this->actor.params != STALFOS_TYPE_CEILING) && ((play->gameplayFrames % 2) != 0)) {
        this->actor.world.rot.y = this->actor.yawTowardsPlayer;
        StalfosPort_EnTest_SetupJumpBack(this);
    }

    if (this->timer == 0) {
        StalfosPort_EnTest_SetupIdleFromBlock(this);
    } else {
        this->timer--;
    }
}

void StalfosPort_EnTest_SetupIdleFromBlock(EnTest* this) {
    Animation_MorphToLoop(&this->skelAnime, (AnimationHeader*)0x0600316c, -4.0f);
    this->unk_7C8 = 0x16;
    StalfosPort_EnTest_SetupAction(this, StalfosPort_EnTest_IdleFromBlock);
}

void StalfosPort_EnTest_IdleFromBlock(EnTest* this, PlayState* play) {
    Math_SmoothStepToF(&this->actor.speed, 0.0f, 1.0f, 1.5f, 0.0f);
    SkelAnime_Update(&this->skelAnime);

    if (this->skelAnime.morphWeight == 0.0f) {
        this->actor.speed = 0.0f;
        this->unk_7DE = 0;

        if (!StalfosPort_EnTest_ReactToProjectile(play, this)) {
            if (this->actor.xzDistToPlayer < 500.0f) {
                StalfosPort_EnTest_ChooseAction(this, play);
            } else {
                StalfosPort_func_808627C4(this, play);
            }
        }
    }
}

void StalfosPort_func_80862154(EnTest* this) {
    Animation_PlayOnce(&this->skelAnime, (AnimationHeader*)0x06008604);
    Actor_PlaySfx(&this->actor, 0x383a);
    this->unk_7C8 = 8;
    this->actor.speed = -2.0f;
    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 8);
    StalfosPort_EnTest_SetupAction(this, StalfosPort_func_808621D4);
}

void StalfosPort_func_808621D4(EnTest* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    Math_SmoothStepToF(&this->actor.speed, 0.0f, 1.0f, 0.1f, 0.0f);

    if (SkelAnime_Update(&this->skelAnime)) {
        this->actor.speed = 0.0f;

        if ((this->actor.bgCheckFlags & BGCHECKFLAG_WALL) &&
            ((ABS((s16)(this->actor.wallYaw - this->actor.shape.rot.y)) < 0x38A4) &&
             (this->actor.xzDistToPlayer < 80.0f))) {
            StalfosPort_EnTest_SetupJumpUp(this);
        } else if (!StalfosPort_EnTest_ReactToProjectile(play, this)) {
            StalfosPort_EnTest_ChooseAction(this, play);
        } else {
            return;
        }
    }

    if (player->meleeWeaponState != 0) {
        if ((this->actor.bgCheckFlags & BGCHECKFLAG_WALL) &&
            ((ABS((s16)(this->actor.wallYaw - this->actor.shape.rot.y)) < 0x38A4) &&
             (this->actor.xzDistToPlayer < 80.0f))) {
            StalfosPort_EnTest_SetupJumpUp(this);
        } else if ((Rand_ZeroOne() > 0.7f) && (this->actor.params != STALFOS_TYPE_CEILING) &&
                   (player->meleeWeaponAnimation != PLAYER_MWA_JUMPSLASH_START)) {
            StalfosPort_EnTest_SetupJumpBack(this);
        } else {
            StalfosPort_EnTest_SetupStopAndBlock(this);
        }

        this->unk_7C8 = 8;
    }
}

void StalfosPort_func_80862398(EnTest* this) {
    Animation_PlayOnce(&this->skelAnime, (AnimationHeader*)0x06000444);
    Actor_PlaySfx(&this->actor, 0x383a);
    this->unk_7C8 = 9;
    this->actor.speed = -2.0f;
    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 8);
    StalfosPort_EnTest_SetupAction(this, StalfosPort_func_80862418);
}

void StalfosPort_func_80862418(EnTest* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    Math_SmoothStepToF(&this->actor.speed, 0.0f, 1.0f, 0.1f, 0.0f);

    if (SkelAnime_Update(&this->skelAnime)) {
        this->actor.speed = 0.0f;

        if (!StalfosPort_EnTest_ReactToProjectile(play, this)) {
            StalfosPort_EnTest_ChooseAction(this, play);
        } else {
            return;
        }
    }

    if (player->meleeWeaponState != 0) {
        if ((this->actor.bgCheckFlags & BGCHECKFLAG_WALL) &&
            ((ABS((s16)(this->actor.wallYaw - this->actor.shape.rot.y)) < 0x38A4) &&
             (this->actor.xzDistToPlayer < 80.0f))) {
            StalfosPort_EnTest_SetupJumpUp(this);
        } else if ((Rand_ZeroOne() > 0.7f) && (this->actor.params != STALFOS_TYPE_CEILING) &&
                   (player->meleeWeaponAnimation != PLAYER_MWA_JUMPSLASH_START)) {
            StalfosPort_EnTest_SetupJumpBack(this);
        } else {
            StalfosPort_EnTest_SetupStopAndBlock(this);
        }

        this->unk_7C8 = 8;
    }
}

void StalfosPort_EnTest_SetupStunned(EnTest* this) {
    this->unk_7C8 = 0xB;
    this->unk_7DE = 0;
    this->swordState = 0;
    this->skelAnime.playSpeed = 0.0f;
    this->actor.speed = -4.0f;

    if (this->lastDamageReaction == STALFOS_DMG_REACT_LIGHT) {
        Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_GRAY, 120, COLORFILTER_BUFFLAG_OPA, 80);
    } else {
        Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_BLUE, 120, COLORFILTER_BUFFLAG_OPA, 80);

        if (this->lastDamageReaction == STALFOS_DMG_REACT_FREEZE) {
            this->iceTimer = 36;
        } else {
            Animation_PlayOnceSetSpeed(&this->skelAnime, (AnimationHeader*)0x06008604, 0.0f);
        }
    }

    Actor_PlaySfx(&this->actor, 0x389e);
    StalfosPort_EnTest_SetupAction(this, StalfosPort_EnTest_Stunned);
}

void StalfosPort_EnTest_Stunned(EnTest* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    Math_SmoothStepToF(&this->actor.speed, 0.0f, 1.0f, 1.0f, 0.0f);

    if (this->actor.colorFilterTimer == 0) {
        if (this->actor.colChkInfo.health == 0) {
            StalfosPort_func_80862FA8(this, play);
        } else if (player->meleeWeaponState != 0) {
            if ((this->actor.bgCheckFlags & BGCHECKFLAG_WALL) &&
                ((ABS((s16)(this->actor.wallYaw - this->actor.shape.rot.y)) < 0x38A4) &&
                 (this->actor.xzDistToPlayer < 80.0f))) {
                StalfosPort_EnTest_SetupJumpUp(this);
            } else if ((Rand_ZeroOne() > 0.7f) && (player->meleeWeaponAnimation != PLAYER_MWA_JUMPSLASH_START)) {
                StalfosPort_EnTest_SetupJumpBack(this);
            } else {
                StalfosPort_EnTest_SetupStopAndBlock(this);
            }

            this->unk_7C8 = 8;
        } else {
            this->actor.speed = 0.0f;
            if (!StalfosPort_EnTest_ReactToProjectile(play, this)) {
                StalfosPort_EnTest_ChooseAction(this, play);
            }
        }
    }
}

void StalfosPort_func_808627C4(EnTest* this, PlayState* play) {
    if (Actor_OtherIsLockedOn(play, &this->actor)) {
        StalfosPort_func_80860EC0(this);
        return;
    }

    Animation_MorphToLoop(&this->skelAnime, (AnimationHeader*)0x0600e2b0, -2.0f);
    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0xFA0, 1);
    this->actor.speed = ((play->gameplayFrames % 2) != 0) ? -4.0f : 4.0f;
    this->actor.world.rot.y = this->actor.shape.rot.y + 0x3FFF;
    this->timer = (Rand_ZeroOne() * 20.0f) + 20.0f;
    this->unk_7C8 = 0x18;
    StalfosPort_EnTest_SetupAction(this, StalfosPort_func_808628C8);
    this->unk_7EC = 0.0f;
}

void StalfosPort_func_808628C8(EnTest* this, PlayState* play) {

    Player* player = GET_PLAYER(play);

    s32 prevFrame;
    s32 beforeCurFrame;

    f32 checkDist = 0.0f;
    s16 newYaw;
    f32 absPlaySpeed;

    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0xFA0, 1);

    if (this->unk_7DE == 0) {
        this->unk_7DE++;
    }

    if (this->actor.speed >= 0.0f) {
        if (this->actor.speed < 6.0f) {
            this->actor.speed += 0.125f;
        } else {
            this->actor.speed = 6.0f;
        }
    } else {
        if (this->actor.speed > -6.0f) {
            this->actor.speed -= 0.125f;
        } else {
            this->actor.speed = -6.0f;
        }
    }

    if ((this->actor.bgCheckFlags & BGCHECKFLAG_WALL) ||
        ((this->actor.params == STALFOS_TYPE_CEILING) &&
         !Actor_TestFloorInDirection(&this->actor, play, this->actor.speed, this->actor.shape.rot.y + 0x3FFF))) {
        if (this->actor.bgCheckFlags & BGCHECKFLAG_WALL) {
            if (this->actor.speed >= 0.0f) {
                newYaw = (this->actor.shape.rot.y + 0x3FFF);
            } else {
                newYaw = (this->actor.shape.rot.y - 0x3FFF);
            }

            newYaw = this->actor.wallYaw - newYaw;
        } else {
            this->actor.speed *= -0.8f;
            newYaw = 0;
        }

        if (ABS(newYaw) > 0x4000) {
            this->actor.speed *= -0.8f;

            if (this->actor.speed < 0.0f) {
                this->actor.speed -= 0.5f;
            } else {
                this->actor.speed += 0.5f;
            }
        }
    }

    this->actor.world.rot.y = this->actor.shape.rot.y + 0x3FFF;

    if (Actor_OtherIsLockedOn(play, &this->actor)) {
        checkDist = 200.0f;
    }

    if (this->actor.xzDistToPlayer <= (80.0f + checkDist)) {
        Math_SmoothStepToF(&this->unk_7EC, -2.5f, 1.0f, 0.8f, 0.0f);
    } else if (this->actor.xzDistToPlayer > (110.0f + checkDist)) {
        Math_SmoothStepToF(&this->unk_7EC, 2.5f, 1.0f, 0.8f, 0.0f);
    } else {
        Math_SmoothStepToF(&this->unk_7EC, 0.0f, 1.0f, 6.65f, 0.0f);
    }

    if (this->unk_7EC != 0.0f) {
        this->actor.world.pos.x += (Math_SinS(this->actor.shape.rot.y) * this->unk_7EC);
        this->actor.world.pos.z += (Math_CosS(this->actor.shape.rot.y) * this->unk_7EC);
    }

    this->skelAnime.playSpeed = this->actor.speed * 0.5f;

    prevFrame = (s32)this->skelAnime.curFrame;
    SkelAnime_Update(&this->skelAnime);
    beforeCurFrame = (s32)(this->skelAnime.curFrame - ABS(this->skelAnime.playSpeed));
    absPlaySpeed = ABS(this->skelAnime.playSpeed);

    if ((this->timer % 32) == 0) {
        Actor_PlaySfx(&this->actor, 0x3838);
    }
    if ((s32)this->skelAnime.curFrame != prevFrame) {
        s32 afterPrevFrame = (s32)absPlaySpeed + prevFrame;

        if (((afterPrevFrame > 1) && (beforeCurFrame < 1)) || ((beforeCurFrame < 7) && (afterPrevFrame > 7))) {
            Actor_PlaySfx(&this->actor, 0x383d);
        }
    }

    if (this->timer == 0) {
        if (Actor_OtherIsLockedOn(play, &this->actor)) {
            StalfosPort_EnTest_SetupIdle(this);
        } else if (Actor_IsLockedOn(play, &this->actor)) {
            if (!StalfosPort_EnTest_ReactToProjectile(play, this)) {
                StalfosPort_EnTest_ChooseAction(this, play);
            }
        } else if (player->heldItemAction != PLAYER_IA_NONE) {
            if ((play->gameplayFrames % 2) != 0) {
                StalfosPort_EnTest_SetupIdle(this);
            } else {
                StalfosPort_EnTest_SetupWalkAndBlock(this);
            }
        } else {
            StalfosPort_EnTest_SetupWalkAndBlock(this);
        }

    } else {
        this->timer--;
    }
}

void StalfosPort_func_80862FA8(EnTest* this, PlayState* play) {
    Animation_PlayOnce(&this->skelAnime, (AnimationHeader*)0x06001420);
    Actor_PlaySfx(&this->actor, 0x383b);
    this->unk_7DE = 0;
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    this->actor.colorFilterTimer = 0;
    this->actor.speed = 0.0f;

    this->unk_7C8 = 5;
    StalfosPort_EnTest_SetupAction(this, StalfosPort_func_80863044);
}

void StalfosPort_func_80863044(EnTest* this, PlayState* play) {
    if (SkelAnime_Update(&this->skelAnime)) {
        this->timer = (Rand_ZeroOne() * 10.0f) + 10.0f;
        this->unk_7C8 = 7;
        StalfosPort_EnTest_SetupAction(this, StalfosPort_func_808633E8);
        BodyBreak_Alloc(&this->bodyBreak, 60, play);
    }

    if ((s32)this->skelAnime.curFrame == 15) {
        Actor_PlaySfx(&this->actor, 0x387a);
    }
}

void StalfosPort_func_808630F0(EnTest* this, PlayState* play) {
    Animation_PlayOnce(&this->skelAnime, (AnimationHeader*)0x06009a90);
    Actor_PlaySfx(&this->actor, 0x383b);
    this->unk_7C8 = 6;
    this->actor.colorFilterTimer = 0;
    this->unk_7DE = 0;
    this->actor.speed = 0.0f;

    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    StalfosPort_EnTest_SetupAction(this, StalfosPort_func_8086318C);
}

void StalfosPort_func_8086318C(EnTest* this, PlayState* play) {
    if (SkelAnime_Update(&this->skelAnime)) {
        this->timer = (Rand_ZeroOne() * 10.0f) + 10.0f;
        this->unk_7C8 = 7;
        StalfosPort_EnTest_SetupAction(this, StalfosPort_func_808633E8);
        BodyBreak_Alloc(&this->bodyBreak, 60, play);
    }

    if (((s32)this->skelAnime.curFrame == 10) || ((s32)this->skelAnime.curFrame == 25)) {
        Actor_PlaySfx(&this->actor, 0x387a);
    }
}

void StalfosPort_EnTest_SetupRecoil(EnTest* this) {
    this->swordState = 0;
    this->skelAnime.moveFlags = 2;
    this->unk_7C8 = 0x13;
    this->skelAnime.playSpeed = -1.0f;
    this->skelAnime.startFrame = this->skelAnime.curFrame;
    this->skelAnime.endFrame = 0.0f;
    StalfosPort_EnTest_SetupAction(this, StalfosPort_EnTest_Recoil);
}

void StalfosPort_EnTest_Recoil(EnTest* this, PlayState* play) {
    if (SkelAnime_Update(&this->skelAnime)) {
        if (Rand_ZeroOne() > 0.7f) {
            StalfosPort_EnTest_SetupIdle(this);
            this->timer = (Rand_ZeroOne() * 5.0f) + 5.0f;
        } else if (((play->gameplayFrames % 2) != 0) && (this->actor.params != STALFOS_TYPE_CEILING)) {
            StalfosPort_EnTest_SetupJumpBack(this);
        } else {
            StalfosPort_func_808627C4(this, play);
        }
    }
}

void StalfosPort_EnTest_Rise(EnTest* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);

    if (this->actor.scale.y < 0.015f) {
        this->actor.scale.y += 0.002f;
        this->actor.world.pos.y = this->actor.home.pos.y - 3.5f;
    } else {
        this->actor.world.pos.y = this->actor.home.pos.y;
        StalfosPort_EnTest_SetupJumpBack(this);
    }
}

void StalfosPort_func_808633E8(EnTest* this, PlayState* play) {
    this->actor.params = STALFOS_TYPE_1;

    if (BodyBreak_SpawnParts(&this->actor, &this->bodyBreak, play, this->actor.params)) {
        Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos, 0xd0);

        if (this->actor.parent != NULL) {
            this->actor.parent->home.rot.z--;
        }

        Actor_Kill(&this->actor);
    }
}

void StalfosPort_EnTest_UpdateHeadRot(EnTest* this, PlayState* play) {
    s16 lookAngle = this->actor.yawTowardsPlayer;

    lookAngle -= (s16)(this->headRot.y + this->actor.shape.rot.y);

    this->headRotOffset.y = CLAMP(lookAngle, -0x7D0, 0x7D0);
    this->headRot.y += this->headRotOffset.y;
    this->headRot.y = CLAMP(this->headRot.y, -0x382F, 0x382F);
}

void StalfosPort_EnTest_UpdateDamage(EnTest* this, PlayState* play) {
    if (this->shieldCollider.base.acFlags & AC_BOUNCED) {
        this->shieldCollider.base.acFlags &= ~AC_BOUNCED;
        this->bodyCollider.base.acFlags &= ~AC_HIT;

        if (this->unk_7C8 >= 0xA) {
            this->actor.speed = -4.0f;
        }
    } else if (this->bodyCollider.base.acFlags & AC_HIT) {
        this->bodyCollider.base.acFlags &= ~AC_HIT;

        if ((this->actor.colChkInfo.damageEffect != STALFOS_DMG_REACT_SLING) &&
            (this->actor.colChkInfo.damageEffect != STALFOS_DMG_REACT_FIREMAGIC)) {
            this->lastDamageReaction = this->actor.colChkInfo.damageEffect;
            if (this->swordState >= 1) {
                this->swordState = 0;
            }
            this->unk_7DC = 0;
            this->actor.world.rot.y = this->actor.yawTowardsPlayer;
            Actor_SetDropFlag(&this->actor, &this->bodyCollider.elem, false);
            Audio_StopSfxByPosAndId(&this->actor.projectedPos, 0x3838);

            if ((this->actor.colChkInfo.damageEffect == STALFOS_DMG_REACT_STUN) ||
                (this->actor.colChkInfo.damageEffect == STALFOS_DMG_REACT_FREEZE) ||
                (this->actor.colChkInfo.damageEffect == STALFOS_DMG_REACT_LIGHT)) {
                if (this->unk_7C8 != 0xB) {
                    Actor_ApplyDamage(&this->actor);
                    StalfosPort_EnTest_SetupStunned(this);
                }
            } else {
                if (Actor_IsFacingPlayer(&this->actor, 0x4000)) {
                    if (Actor_ApplyDamage(&this->actor) == 0) {
                        Enemy_StartFinishingBlow(play, &this->actor);
                        StalfosPort_func_80862FA8(this, play);
                    } else {
                        StalfosPort_func_80862154(this);
                    }
                } else if (Actor_ApplyDamage(&this->actor) == 0) {
                    StalfosPort_func_808630F0(this, play);
                    Enemy_StartFinishingBlow(play, &this->actor);
                } else {
                    StalfosPort_func_80862398(this);
                }
            }
        }
    }
}

void StalfosPort_EnTest_Update(Actor* thisx, PlayState* play) {
    EnTest* this = (EnTest*)thisx;
    f32 oldWeight;
    u32 floorProperty;

    StalfosPort_EnTest_UpdateDamage(this, play);

    if (this->actor.colChkInfo.damageEffect != STALFOS_DMG_REACT_FIREMAGIC) {
        Actor_MoveXZGravity(&this->actor);
        Actor_UpdateBgCheckInfo(play, &this->actor, 75.0f, 30.0f, 30.0f,
                                UPDBGCHECKINFO_FLAG_MM_1 | UPDBGCHECKINFO_FLAG_MM_4 | UPDBGCHECKINFO_FLAG_MM_8 |
                                    UPDBGCHECKINFO_FLAG_MM_10);

        if (this->actor.params == STALFOS_TYPE_1) {
            if (this->actor.world.pos.y <= this->actor.home.pos.y) {
                this->actor.world.pos.y = this->actor.home.pos.y;
                this->actor.velocity.y = 0.0f;
            }

            if (this->actor.floorHeight <= this->actor.home.pos.y) {
                this->actor.floorHeight = this->actor.home.pos.y;
            }
        } else if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND_TOUCH) {
            floorProperty = SurfaceType_GetFloorProperty(&play->colCtx, this->actor.floorPoly, this->actor.floorBgId);

            if ((floorProperty == 5) || (floorProperty == 12) ||
                SurfaceType_GetFloorType(&play->colCtx, this->actor.floorPoly, this->actor.floorBgId) == FLOOR_TYPE_9) {
                Actor_Kill(&this->actor);
                return;
            }
        }

        this->actionFunc(this, play);

        switch (this->unk_7DE) {
            case 0:
                break;

            case 1:
                Animation_Change(&this->upperSkelanime, (AnimationHeader*)0x06001c20, 2.0f, 0.0f,
                                 Animation_GetLastFrame((AnimationHeader*)0x06001c20), 2, 2.0f);
                AnimTaskQueue_AddCopyUsingMap(play, this->skelAnime.limbCount, this->skelAnime.jointTable,
                                              this->upperSkelanime.jointTable, sUpperBodyLimbCopyMap);
                this->unk_7DE++;
                break;

            case 2:
                SkelAnime_Update(&this->upperSkelanime);
                SkelAnime_CopyFrameTableTrue(&this->skelAnime, this->skelAnime.jointTable,
                                             this->upperSkelanime.jointTable, sUpperBodyLimbCopyMap);
                break;

            case 3:
                this->unk_7DE++;
                this->upperSkelanime.morphWeight = 4.0f;
                __attribute__((fallthrough));
            case 4:
                oldWeight = this->upperSkelanime.morphWeight;
                this->upperSkelanime.morphWeight -= 1.0f;

                if (this->upperSkelanime.morphWeight <= 0.0f) {
                    this->unk_7DE = 0;
                }

                SkelAnime_InterpFrameTable(this->skelAnime.limbCount, this->upperSkelanime.jointTable,
                                           this->upperSkelanime.jointTable, this->skelAnime.jointTable,
                                           1.0f - (this->upperSkelanime.morphWeight / oldWeight));
                SkelAnime_CopyFrameTableTrue(&this->skelAnime, this->skelAnime.jointTable,
                                             this->upperSkelanime.jointTable, sUpperBodyLimbCopyMap);
                break;
        }

        if ((this->actor.colorFilterTimer == 0) && (this->actor.colChkInfo.health != 0)) {
            if ((this->unk_7C8 != 0x10) && (this->unk_7C8 != 0x17)) {
                StalfosPort_EnTest_UpdateHeadRot(this, play);
            } else {
                Math_SmoothStepToS(&this->headRot.y, 0, 1, 0x3E8, 0);
            }
        }
    }

    Collider_UpdateCylinder(&this->actor, &this->bodyCollider);

    this->actor.focus.pos = this->actor.world.pos;
    this->actor.focus.pos.y += 45.0f;

    if ((this->actor.colChkInfo.health > 0) || (this->actor.colorFilterTimer != 0)) {
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->bodyCollider.base);

        if ((this->unk_7C8 >= 0xA) &&
            ((this->actor.colorFilterTimer == 0) || !(this->actor.colorFilterParams & 0x4000))) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->bodyCollider.base);
        }

        if (this->unk_7DE != 0) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->shieldCollider.base);
        }
    }

    if (this->swordState >= 1) {
        if (!(this->swordCollider.base.atFlags & AT_BOUNCED)) {
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->swordCollider.base);
        } else {
            this->swordCollider.base.atFlags &= ~AT_BOUNCED;
            StalfosPort_EnTest_SetupRecoil(this);
        }
    }

    if (this->actor.params == STALFOS_TYPE_INVISIBLE) {
        if (play->actorCtx.lensActive) {
            this->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_REACT_TO_LENS;
            this->actor.shape.shadowDraw = ActorShadow_DrawFeet;
        } else {
            this->actor.flags &= ~ACTOR_FLAG_REACT_TO_LENS;
            this->actor.shape.shadowDraw = NULL;
        }
    }
}

s32 StalfosPort_EnTest_OverrideLimbDraw(PlayState* play2, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                                        void* thisx) {
    EnTest* this = (EnTest*)thisx;
    PlayState* play = (PlayState*)play2;

    if (limbIndex == STALFOS_LIMB_HEAD_ROOT) {
        rot->x += this->headRot.y;
        rot->y -= this->headRot.x;
        rot->z += this->headRot.z;
    } else if (limbIndex == STALFOS_LIMB_HEAD) {
        OPEN_DISPS(play->state.gfxCtx);

        gDPPipeSync(POLY_OPA_DISP++);
        gDPSetEnvColor(POLY_OPA_DISP++, 80 + ABS((s16)(Math_SinS(play->gameplayFrames * 2000) * 175.0f)), 0, 0, 255);

        CLOSE_DISPS();
    }

    if ((this->actor.params == STALFOS_TYPE_INVISIBLE) &&
        !ACTOR_FLAGS_CHECK_ALL(&this->actor, ACTOR_FLAG_REACT_TO_LENS)) {
        *dList = NULL;
    }

    return false;
}

void StalfosPort_EnTest_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {

    static Vec3f D_80864658 = { 300.0f, 0.0f, 0.0f };
    static Vec3f D_80864664 = { 3400.0f, 0.0f, 0.0f };
    static Vec3f D_80864670 = { 0.0f, 0.0f, 0.0f };
    static Vec3f D_8086467C = { 7000.0f, 1000.0f, 0.0f };
    static Vec3f D_80864688 = { 3000.0f, -2000.0f, -1000.0f };
    static Vec3f D_80864694 = { 3000.0f, -2000.0f, 1000.0f };
    static Vec3f D_808646A0 = { -1300.0f, 1100.0f, 0.0f };

    s32 bodyPart = -1;
    Vec3f sp70;
    Vec3f sp64;
    EnTest* this = (EnTest*)thisx;

    Vec3f sp50;

    BodyBreak_SetInfo(&this->bodyBreak, limbIndex, 0, 60, 60, dList, BODYBREAK_OBJECT_SLOT_DEFAULT);

    if (limbIndex == STALFOS_LIMB_SWORD) {
        Matrix_MultVec3f(&D_8086467C, &this->swordCollider.dim.quad[1]);
        Matrix_MultVec3f(&D_80864688, &this->swordCollider.dim.quad[0]);
        Matrix_MultVec3f(&D_80864694, &this->swordCollider.dim.quad[3]);
        Matrix_MultVec3f(&D_808646A0, &this->swordCollider.dim.quad[2]);

        Collider_SetQuadVertices(&this->swordCollider, &this->swordCollider.dim.quad[0],
                                 &this->swordCollider.dim.quad[1], &this->swordCollider.dim.quad[2],
                                 &this->swordCollider.dim.quad[3]);

        Matrix_MultVec3f(&D_80864664, &sp70);
        Matrix_MultVec3f(&D_80864670, &sp64);

        if ((this->swordState >= 1) && ((this->actor.params != STALFOS_TYPE_INVISIBLE) || play->actorCtx.lensActive)) {
            StalfosBlureVertex(Effect_GetByIndex(this->effectIndex), &sp70, &sp64);
        } else if (this->swordState >= 0) {
            EffectBlure_AddSpace(Effect_GetByIndex(this->effectIndex));
            this->swordState = -1;
        }

    } else if ((limbIndex == STALFOS_LIMB_SHIELD) && (this->unk_7DE != 0)) {
        Matrix_MultVec3f(&D_80864670, &sp64);

        this->shieldCollider.dim.pos.x = sp64.x;
        this->shieldCollider.dim.pos.y = sp64.y;
        this->shieldCollider.dim.pos.z = sp64.z;
    } else {
        Actor_SetFeetPos(&this->actor, limbIndex, STALFOS_LIMB_FOOT_L, &D_80864658, STALFOS_LIMB_ANKLE_R, &D_80864658);

        if ((limbIndex == STALFOS_LIMB_FOOT_L) || (limbIndex == STALFOS_LIMB_ANKLE_R)) {
            if ((this->unk_7C8 == 0x15) || (this->unk_7C8 == 0x16)) {
                if (this->actor.speed != 0.0f) {
                    Matrix_MultVec3f(&D_80864658, &sp64);
                    Actor_SpawnFloorDustRing(play, &this->actor, &sp64, 10.0f, 1, 8.0f, 100, 15, false);
                }
            }
        }
    }

    if (this->iceTimer != 0) {
        switch (limbIndex) {
            case STALFOS_LIMB_HEAD:
                bodyPart = 0;
                break;
            case STALFOS_LIMB_CHEST:
                bodyPart = 1;
                break;
            case STALFOS_LIMB_SWORD:
                bodyPart = 2;
                break;
            case STALFOS_LIMB_SHIELD:
                bodyPart = 3;
                break;
            case STALFOS_LIMB_UPPERARM_R:
                bodyPart = 4;
                break;
            case STALFOS_LIMB_UPPERARM_L:
                bodyPart = 5;
                break;
            case STALFOS_LIMB_WAIST:
                bodyPart = 6;
                break;
            case STALFOS_LIMB_FOOT_L:
                bodyPart = 7;
                break;
            case STALFOS_LIMB_FOOT_R:
                bodyPart = 8;
                break;
        }

        if (bodyPart >= 0) {
            Matrix_MultVec3f(&D_80864670, &sp50);

            this->bodyPartsPos[bodyPart].x = sp50.x;
            this->bodyPartsPos[bodyPart].y = sp50.y;
            this->bodyPartsPos[bodyPart].z = sp50.z;
        }
    }
}

void StalfosPort_EnTest_Draw(Actor* thisx, PlayState* play) {
    EnTest* this = (EnTest*)thisx;

    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    func_8002EBCC(&this->actor, play, 1);

    if ((thisx->params <= STALFOS_TYPE_CEILING) || (thisx->child == NULL)) {
        SkelAnime_DrawOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable,
                          StalfosPort_EnTest_OverrideLimbDraw, StalfosPort_EnTest_PostLimbDraw, this);
    }

    if (this->iceTimer != 0) {
        thisx->colorFilterTimer++;
        this->iceTimer--;

        if ((this->iceTimer % 4) == 0) {
            s32 iceIndex = this->iceTimer >> 2;

            EffectSsEnIce_SpawnFlyingVec3s(play, thisx, &this->bodyPartsPos[iceIndex], 150, 150, 150, 250, 235, 245,
                                           255, 1.5f);
        }
    }
}

void StalfosPort_func_80864158(EnTest* this, f32 xzSpeed) {
    Animation_MorphToLoop(&this->skelAnime, (AnimationHeader*)0x0600e2b0, -2.0f);
    this->actor.speed = xzSpeed;
    this->actor.world.rot.y = this->actor.shape.rot.y + 0x3FFF;
    this->timer = (Rand_ZeroOne() * 20.0f) + 15.0f;
    this->unk_7C8 = 0x18;
    StalfosPort_EnTest_SetupAction(this, StalfosPort_func_808628C8);
}

s32 StalfosPort_EnTest_ReactToProjectile(PlayState* play, EnTest* this) {
    Actor* projectileActor;
    s16 yawToProjectile;
    s16 wallYawDiff;
    s16 touchingWall;
    s16 directionFlag;

    projectileActor = Actor_GetProjectileActor(play, &this->actor, 300.0f);

    if (projectileActor != NULL) {
        yawToProjectile = Actor_WorldYawTowardActor(&this->actor, projectileActor) - (u16)this->actor.shape.rot.y;

        if ((u8)(this->actor.bgCheckFlags & BGCHECKFLAG_WALL)) {
            wallYawDiff = ((u16)this->actor.wallYaw - (u16)this->actor.shape.rot.y);
            touchingWall = true;
        } else {
            touchingWall = false;
        }

        if (Math_Vec3f_DistXYZ(&this->actor.world.pos, &projectileActor->world.pos) < 200.0f) {
            if (Actor_IsLockedOn(play, &this->actor) && (projectileActor->id == ACTOR_ARMS_HOOK)) {
                StalfosPort_EnTest_SetupJumpUp(this);
            } else if (ABS(yawToProjectile) < 0x2000) {
                StalfosPort_EnTest_SetupStopAndBlock(this);
            } else if (ABS(yawToProjectile) < 0x6000) {
                StalfosPort_EnTest_SetupJumpBack(this);
            } else {
                StalfosPort_EnTest_SetupJumpUp(this);
            }

            return true;
        }

        if (Actor_IsLockedOn(play, &this->actor) && (projectileActor->id == ACTOR_ARMS_HOOK)) {
            StalfosPort_EnTest_SetupJumpUp(this);
            return true;
        }

        if ((ABS(yawToProjectile) < 0x2000) || (ABS(yawToProjectile) > 0x6000)) {
            directionFlag = play->gameplayFrames % 2;

            if (touchingWall && (wallYawDiff > 0x2000) && (wallYawDiff < 0x6000)) {
                directionFlag = false;
            } else if (touchingWall && (wallYawDiff < -0x2000) && (wallYawDiff > -0x6000)) {
                directionFlag = true;
            }

            if (directionFlag) {
                StalfosPort_func_80864158(this, 4.0f);
            } else {
                StalfosPort_func_80864158(this, -4.0f);
            }
        } else if (ABS(yawToProjectile) < 0x6000) {
            directionFlag = play->gameplayFrames % 2;

            if (touchingWall && (ABS(wallYawDiff) > 0x6000)) {
                directionFlag = false;
            } else if (touchingWall && (ABS(wallYawDiff) < 0x2000)) {
                directionFlag = true;
            }

            if (directionFlag) {
                StalfosPort_EnTest_SetupJumpBack(this);
            } else {
                StalfosPort_EnTest_SetupJumpUp(this);
            }
        }

        return true;
    }

    return false;
}

EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_STALFOS_INIT, StalfosPort_EnTest_Init);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_STALFOS_DESTROY, StalfosPort_EnTest_Destroy);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_STALFOS_UPDATE, StalfosPort_EnTest_Update);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_STALFOS_DRAW, StalfosPort_EnTest_Draw);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_STALFOS_PROFILE, StalfosPort_Profile);

static void BodyBreak_Alloc(StalfosBodyBreak* body, s32 count, PlayState* play) {
    if (body->matrices)
        free(body->matrices);
    memset(body, 0, sizeof(*body));
    body->play = play;
    body->prevLimbIndex = -1;
    if (count != 60)
        return;
    body->matrices = malloc(61 * (sizeof(MtxF) + sizeof(Gfx*) + sizeof(s16)));
    if (!body->matrices)
        return;
    body->dLists = (Gfx**)(body->matrices + 61);
    body->objectSlots = (s16*)(body->dLists + 61);
    body->capacity = count;
    body->val = 1;
}

static void BodyBreak_SetInfo(StalfosBodyBreak* body, s32 limb, s32 first, s32 last, u32 count, Gfx** dList,
                              s16 objectSlot) {
    if (body->val <= 0 || !body->play || body->play->actorCtx.freezeFlashTimer)
        return;
    if (limb >= first && limb <= last && *dList && body->val <= body->capacity) {
        body->dLists[body->val] = *dList;
        Matrix_Get(&body->matrices[body->val]);
        body->objectSlots[body->val++] = objectSlot;
    }
    if (body->prevLimbIndex != limb)
        body->count++;
    if ((u32)body->count >= count) {
        body->count = body->val - 1;
        body->val = -1;
    }
    body->prevLimbIndex = limb;
}

static s32 BodyBreak_SpawnParts(Actor* actor, StalfosBodyBreak* body, PlayState* play, s16 type) {
    if (!body->matrices)
        return 1;
    if (body->val != -1)
        return 0;
    if (type != 1)
        return 0;
    EnemyNativeSegments scope = { 0 };
    if (!comboEnemyNativeEnter(play, &scope, ENEMY_NATIVE_KEEP_MAIN))
        return 0;

    while (body->count > 0) {
        s32 index = body->count--;
        Matrix_Put(&body->matrices[index]);
        Matrix_Scale(1.0f / actor->scale.x, 1.0f / actor->scale.y, 1.0f / actor->scale.z, MTXMODE_APPLY);
        Actor* previous = actor->child;
        StalfosNativeBodyParts(actor, play, 1, &body->dLists[index]);
        Actor* part = actor->child;
        if (part && part != previous) {
            part->objectSlot = body->objectSlots[index] >= 0 ? body->objectSlots[index] : actor->objectSlot;
            Actor_SetObjectDependency(play, part);
        }
    }
    comboEnemyNativeLeave(&scope);
    free(body->matrices);
    body->matrices = NULL;
    body->val = -2;
    return 1;
}
