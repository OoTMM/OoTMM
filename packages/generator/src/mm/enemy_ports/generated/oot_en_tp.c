#include "oot_compat.h"
#include "types/en_tp.h"
#undef ACTOR_EN_TP
#define ACTOR_EN_TP 0x0035
#undef OBJECT_TP
#define OBJECT_TP 0x1022
#undef NA_SE_EN_TAIL_FLY
#define NA_SE_EN_TAIL_FLY 0x3864
#undef NA_SE_EN_TAIL_CRY
#define NA_SE_EN_TAIL_CRY 0x3865
#undef NA_SE_EN_TAIL_DEAD
#define NA_SE_EN_TAIL_DEAD 0x3866
#undef NA_SE_EN_GOMA_JR_FREEZE
#define NA_SE_EN_GOMA_JR_FREEZE 0x389e
#define gTailpasaranBodyElemDL (*((Gfx (*)[1])0x06000000))
#define gTailpasaranHeadDL (*((Gfx (*)[1])0x060008d0))
#define gTailpasaranBodyElemTex (*((u64 (*)[1])0x06000c68))

#undef ActorShape_Init
#define ActorShape_Init ((void (*)(ActorShape* shape, f32 yOffset, ActorShadowFunc shadowDraw, f32 shadowScale))0x800b3ba4)
#undef Actor_ApplyDamage
#define Actor_ApplyDamage ((u8 (*)(Actor* actor))0x800be22c)
#undef Actor_Kill
#define Actor_Kill FOREIGN_ENEMY_API_KILL
#undef Actor_MoveXZGravity
#define Actor_MoveXZGravity ((void (*)(Actor* actor))0x800b6a88)
#undef Actor_PlaySfx
#define Actor_PlaySfx FOREIGN_ENEMY_API_SFX_ACTOR
#undef Actor_ProcessInitChain
#define Actor_ProcessInitChain ((void (*)(Actor* actor, InitChainEntry* ichain))0x800ffadc)
#undef Actor_SetColorFilter
#define Actor_SetColorFilter FOREIGN_ENEMY_API_COLOR_FILTER
#undef Actor_SetDropFlagJntSph
extern void FOREIGN_ENEMY_API_DROP_FLAG_JOINT(Actor* actor, ColliderJntSph* jntSph, s32 freezeFlag);
#define Actor_SetDropFlagJntSph FOREIGN_ENEMY_API_DROP_FLAG_JOINT
#undef Actor_SetScale
#define Actor_SetScale ((void (*)(Actor* actor, f32 scale))0x800b67e0)
#undef Actor_Spawn
extern Actor* FOREIGN_ENEMY_API_TAIL_MEMBER(ActorContext* actorCtx, PlayState* play, s16 actorId, f32 posX, f32 posY,
                                            f32 posZ, s16 rotX, s16 rotY, s16 rotZ, s16 params);
#define Actor_Spawn FOREIGN_ENEMY_API_TAIL_MEMBER
#undef Actor_UpdateBgCheckInfo
#define Actor_UpdateBgCheckInfo ((void (*)(PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight, s32 flags))0x800b78b8)
#undef Actor_UpdatePos
#define Actor_UpdatePos ((void (*)(Actor* actor))0x800b69ac)
#undef Audio_PlaySfxGeneral
#define Audio_PlaySfxGeneral FOREIGN_ENEMY_API_SFX_GENERAL
#undef Collider_DestroyJntSph
#define Collider_DestroyJntSph FOREIGN_ENEMY_API_JOINT_DESTROY
#undef Collider_InitJntSph
#define Collider_InitJntSph ((s32 (*)(PlayState* play, ColliderJntSph* jntSph))0x800e0b4c)
#undef Collider_SetJntSph
#define Collider_SetJntSph ((s32 (*)(PlayState* play, ColliderJntSph* dest, Actor* actor, ColliderJntSphInit* src, ColliderJntSphElement* jntSphElements))0x800e0e60)
#undef Collider_UpdateSpheres
#define Collider_UpdateSpheres ((void (*)(s32 limb, ColliderJntSph* jntSph))0x800e7fdc)
#undef CollisionCheck_SetAC
#define CollisionCheck_SetAC FOREIGN_ENEMY_API_COLLISION_AC
#undef CollisionCheck_SetAT
#define CollisionCheck_SetAT FOREIGN_ENEMY_API_COLLISION_AT
#undef EffectSsDeadDb_Spawn
extern void FOREIGN_ENEMY_API_DEATH_FLAME(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale,
                                          s16 scaleStep, s16 primR, s16 primG, s16 primB, s16 primA, s16 envR, s16 envG,
                                          s16 envB, s16 unused, s32 arg14, s16 playSfx);
#define EffectSsDeadDb_Spawn FOREIGN_ENEMY_API_DEATH_FLAME
#undef EffectSsDtBubble_SpawnCustomColor
#define EffectSsDtBubble_SpawnCustomColor ((void (*)(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 scale, s16 life, s16 randXZ))0x800b210c)
#undef EffectSsKiraKira_SpawnSmall
extern void FOREIGN_ENEMY_API_SPARKLE_SMALL(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel,
                                            Color_RGBA8* primColor, Color_RGBA8* envColor);
#define EffectSsKiraKira_SpawnSmall FOREIGN_ENEMY_API_SPARKLE_SMALL
#undef Gfx_SetupDL_25Opa
#define Gfx_SetupDL_25Opa ((void (*)(GraphicsContext* gfxCtx))0x8012c28c)
#undef Gfx_SetupDL_25Xlu
#define Gfx_SetupDL_25Xlu ((void (*)(GraphicsContext* gfxCtx))0x8012c2dc)
#undef Item_DropCollectibleRandom
#define Item_DropCollectibleRandom FOREIGN_ENEMY_API_RANDOM_DROP
#undef Math_CosF
#define Math_CosF ((f32 (*)(f32 angle))0x80179540)
#undef Math_CosS
#define Math_CosS ((f32 (*)(s16 angle))0x800fed44)
#undef Math_SinS
#define Math_SinS ((f32 (*)(s16 angle))0x800fed84)
#undef Math_SmoothStepToF
#define Math_SmoothStepToF ((f32 (*)(f32* pValue, f32 target, f32 fraction, f32 step, f32 minStep))0x800ffcd8)
#undef Math_SmoothStepToS
#define Math_SmoothStepToS ((s16 (*)(s16* pValue, s16 target, s16 scale, s16 step, s16 minStep))0x800ffebc)
#undef Math_Vec3f_Yaw
#define Math_Vec3f_Yaw ((s16 (*)(Vec3f* origin, Vec3f* point))0x800ffa60)
#undef Matrix_Finalize
#define Matrix_Finalize(gfx,...) (((Mtx* (*)(GraphicsContext*))0x80181a40)(gfx))
#undef Matrix_ReplaceRotation
#define Matrix_ReplaceRotation ((void (*)(MtxF* mf))0x801820a0)
#undef Matrix_Translate
#define Matrix_Translate FOREIGN_ENEMY_API_MATRIX_TRANSLATE
#undef Rand_CenteredFloat
#define Rand_CenteredFloat ((f32 (*)(f32 f))0x801795c0)
#undef Rand_S16Offset
#define Rand_S16Offset ((s16 (*)(s16 base, s16 range))0x800ff450)
#undef Rand_ZeroOne
#define Rand_ZeroOne ((f32 (*)(void))0x80086fdc)

#define FLAGS 0

void EnTp_Init(Actor* thisx, PlayState* play2);
void EnTp_Destroy(Actor* thisx, PlayState* play);
void EnTp_Update(Actor* thisx, PlayState* play);
void EnTp_Draw(Actor* thisx, PlayState* play);

void EnTp_SetupBodyElem(EnTp* this);
void EnTp_BodyElem(EnTp* this, PlayState* play);
void EnTp_Head_Attack(EnTp* this, PlayState* play);
void EnTp_SetupDying(EnTp* this);
void EnTp_Dying(EnTp* this, PlayState* play);
void EnTp_SetupBodyElemRemnant(EnTp* this);
void EnTp_BodyElemRemnant(EnTp* this, PlayState* play);
void EnTp_Head_Hover(EnTp* this, PlayState* play);
void EnTp_Head_SetupWaitPlayerAndRise(EnTp* this);
void EnTp_Head_WaitPlayerAndRise(EnTp* this, PlayState* play);
void EnTp_Head_SetupSinkIntoGroundAndReappear(EnTp* this);
void EnTp_Head_SinkIntoGroundAndReappear(EnTp* this, PlayState* play);

typedef enum EnTpAction {
    ENTP_ACTION_BODYELEMREMNANT,
    ENTP_ACTION_DYING,
    ENTP_ACTION_BODYELEM,
    ENTP_ACTION_HEAD_WAIT_PLAYER_AND_RISE = 4,
    ENTP_ACTION_HEAD_ATTACK = 7,
    ENTP_ACTION_HEAD_8,
    ENTP_ACTION_HEAD_SINK_INTO_GROUND_AND_REAPPEAR
} EnTpAction;

ActorProfile En_Tp_Profile = {
    0xc35, ACTORCAT_ENEMY, FLAGS, OBJECT_TP, sizeof(EnTp), EnTp_Init, EnTp_Destroy, EnTp_Update, EnTp_Draw,
};

static ColliderJntSphElementInit sJntSphElementsInit[] = {
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0xFFCFFFFF, HIT_SPECIAL_EFFECT_ELECTRIC, 0x08 },
            { 0xFFCFFFFF, HIT_BACKLASH_ELECTRIC, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_NONE,
        },
        { 0, { { 0, 0, 0 }, 4 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COL_MATERIAL_HIT1,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sJntSphElementsInit),
    sJntSphElementsInit,
};

typedef enum EnTpDamageReaction {
    ENTP_DMG_REACT_NO_EFFECT,
    ENTP_DMG_REACT_STUN,
    ENTP_DMG_REACT_MELEE_BACKLASH = 0xE,
    ENTP_DMG_REACT_VULNERABLE
} EnTpDamageReaction;

static DamageTable sDamageTable = { {
    DMG_ENTRY(0, ENTP_DMG_REACT_STUN),           DMG_ENTRY(2, ENTP_DMG_REACT_VULNERABLE),
    DMG_ENTRY(0, ENTP_DMG_REACT_NO_EFFECT),      DMG_ENTRY(0, ENTP_DMG_REACT_NO_EFFECT),
    DMG_ENTRY(1, ENTP_DMG_REACT_VULNERABLE),     DMG_ENTRY(0, ENTP_DMG_REACT_NO_EFFECT),
    DMG_ENTRY(2, ENTP_DMG_REACT_MELEE_BACKLASH), DMG_ENTRY(0, ENTP_DMG_REACT_NO_EFFECT),
    DMG_ENTRY(1, ENTP_DMG_REACT_MELEE_BACKLASH), DMG_ENTRY(2, ENTP_DMG_REACT_MELEE_BACKLASH),
    DMG_ENTRY(4, ENTP_DMG_REACT_MELEE_BACKLASH), DMG_ENTRY(0, ENTP_DMG_REACT_NO_EFFECT),
    DMG_ENTRY(0, ENTP_DMG_REACT_NO_EFFECT),      DMG_ENTRY(0, ENTP_DMG_REACT_NO_EFFECT),
    DMG_ENTRY(0, ENTP_DMG_REACT_NO_EFFECT),      DMG_ENTRY(0, ENTP_DMG_REACT_NO_EFFECT),
    DMG_ENTRY(0, ENTP_DMG_REACT_NO_EFFECT),      DMG_ENTRY(0, ENTP_DMG_REACT_NO_EFFECT),
    DMG_ENTRY(0, ENTP_DMG_REACT_NO_EFFECT),      DMG_ENTRY(0, ENTP_DMG_REACT_NO_EFFECT),
    DMG_ENTRY(0, ENTP_DMG_REACT_NO_EFFECT),      DMG_ENTRY(0, ENTP_DMG_REACT_NO_EFFECT),
    DMG_ENTRY(1, ENTP_DMG_REACT_MELEE_BACKLASH), DMG_ENTRY(4, ENTP_DMG_REACT_MELEE_BACKLASH),
    DMG_ENTRY(2, ENTP_DMG_REACT_MELEE_BACKLASH), DMG_ENTRY(2, ENTP_DMG_REACT_MELEE_BACKLASH),
    DMG_ENTRY(8, ENTP_DMG_REACT_MELEE_BACKLASH), DMG_ENTRY(4, ENTP_DMG_REACT_MELEE_BACKLASH),
    DMG_ENTRY(0, ENTP_DMG_REACT_NO_EFFECT),      DMG_ENTRY(0, ENTP_DMG_REACT_NO_EFFECT),
    DMG_ENTRY(4, ENTP_DMG_REACT_MELEE_BACKLASH), DMG_ENTRY(0, ENTP_DMG_REACT_NO_EFFECT),
} };
static InitChainEntry sInitChain[] = {
    ICHAIN_F32(lockOnArrowOffset, 10, ICHAIN_STOP),
};

void EnTp_SetupAction(EnTp* this, void (*actionFunc)(EnTp*, PlayState*)) {
    this->actionFunc = actionFunc;
}

void EnTp_Init(Actor* thisx, PlayState* play2) {
    EnTp* newChild;
    EnTp* prevChild;
    s32 i;
    EnTp* this = (EnTp*)thisx;
    PlayState* play = play2;

    Actor_ProcessInitChain(&this->actor, sInitChain);
    this->actor.attentionRangeType = ATTENTION_RANGE_3;
    this->actor.colChkInfo.damageTable = &sDamageTable;
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 0.14f);
    this->modeVar = 0;
    this->actor.colChkInfo.health = 1;
    prevChild = this;
    this->bodyElemAlpha = 255;
    Collider_InitJntSph(play, &this->collider);
    Collider_SetJntSph(play, &this->collider, &this->actor, &sJntSphInit, this->colliderElements);
    if (this->actor.params < 0) {
        this->actor.hintId = 0xff;
        this->timer = 0;
        this->collider.base.acFlags |= AC_HARD;
        this->collider.elements[0].dim.worldSphere.radius = 8;
        this->collider.elements[0].dim.modelSphere.radius = this->collider.elements[0].dim.worldSphere.radius;
        EnTp_Head_SetupWaitPlayerAndRise(this);
        this->actor.focus.pos = this->actor.world.pos;
        this->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED;
        Actor_SetScale(&this->actor, 1.5f);
        for (i = 0; i < 7; i++) {
            if (((0 * i) != 0) && ((0 * i) != 0)) {
            }
            newChild = (EnTp*)Actor_Spawn(&play->actorCtx, play, ACTOR_EN_TP, this->actor.world.pos.x,
                                          this->actor.world.pos.y, this->actor.world.pos.z, 0, 0, 0, 0 * i);
            if (newChild != NULL) {
                prevChild->actor.child = &newChild->actor;
                newChild->actor.parent = &prevChild->actor;
                newChild->sparkTimer = i + 1;
                newChild->headInstance = this;
                Actor_SetScale(&newChild->actor, 0.3f);
                if (i == 2) {
                    newChild->actor.flags |=
                        ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED;
                    newChild->modeVar = 1;
                }
                newChild->timer2 = i * -5;
                newChild->timer = i * -5;
                prevChild = newChild;
                newChild->bodyElemDist = 6.0f - ((f32)i * 0.75f);
            }
        }
    } else if (this->actor.params == ENTP_TYPE_BODYELEM) {
        EnTp_SetupBodyElem(this);
    } else {
        EnTp_SetupBodyElemRemnant(this);
    }
}

void EnTp_Destroy(Actor* thisx, PlayState* play) {
    EnTp* this = (EnTp*)thisx;

    Collider_DestroyJntSph(play, &this->collider);
}

void EnTp_SetupBodyElem(EnTp* this) {
    this->action = ENTP_ACTION_BODYELEM;
    EnTp_SetupAction(this, EnTp_BodyElem);
}

void EnTp_BodyElem(EnTp* this, PlayState* play) {
    s16 sp36;
    s16 temp_a0_sp34;

    if (this->actor.params == ENTP_TYPE_BODYELEM_DYING) {
        this->action = ENTP_ACTION_DYING;
        if (this->actor.parent == NULL) {
            EnTp_SetupDying(this);
        }
    } else {
        if (this->modeVar != 0) {
            this->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED;
        }
        if (this->headInstance->modeVar != 0) {
            this->actor.speed = this->redShineAmount = this->actor.velocity.y = this->unk_168 = 0.0f;
            if (this->actor.world.pos.y < this->headInstance->actor.home.pos.y) {
                this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
            }
            this->actor.world.pos = this->actor.parent->prevPos;
        } else {
            Math_SmoothStepToF(&this->actor.world.pos.y, this->actor.parent->world.pos.y - 4.0f, 1.0f, 1.0f, 0.0f);
            sp36 = this->headInstance->actor.shape.rot.y + 0x4000;
            temp_a0_sp34 = (this->headInstance->timer2 + this->timer) * 0x7D0;
            this->actor.world.pos.x =
                (Math_SinS(temp_a0_sp34) * (Math_SinS(sp36) * this->bodyElemDist)) + this->actor.home.pos.x;
            this->actor.world.pos.z =
                (Math_SinS(temp_a0_sp34) * (Math_CosS(sp36) * this->bodyElemDist)) + this->actor.home.pos.z;
        }
    }
}

void EnTp_Head_SetupAttack(EnTp* this) {
    this->action = ENTP_ACTION_HEAD_ATTACK;
    this->timer = 200;
    EnTp_SetupAction(this, EnTp_Head_Attack);
}

void EnTp_Head_Attack(EnTp* this, PlayState* play) {
    Player* player;

    player = GET_PLAYER(play);
    Math_SmoothStepToF(&this->actor.world.pos.y, player->actor.world.pos.y + 30.0f, 1.0f, 0.5f, 0.0f);
    SFX_PLAY_AT_POS(&this->actor.projectedPos, NA_SE_EN_TAIL_FLY - SFX_FLAG);
    if (this->collider.base.atFlags & AT_HIT) {
        this->collider.base.atFlags &= ~AT_HIT;
        if (&player->actor == this->collider.base.at) {
            this->timer = 1;
        }
    }
    if (this->redShineAmount < 255) {
        this->redShineAmount += 15;
    }
    if (Math_CosF(this->unk_168) == 0.0f) {
        this->unk_170 = 2.0f * Rand_ZeroOne();
    }
    this->actor.world.pos.y += Math_CosF(this->unk_168) * (2.0f + this->unk_170);
    this->unk_168 += 0.2f;
    Math_SmoothStepToF(&this->actor.speed, 2.5f, 0.1f, 0.2f, 0.0f);
    this->timer--;
    if (this->timer != 0) {
        Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 1, 0x2EE, 0);
        this->actor.shape.rot.y = this->actor.world.rot.y;
    } else {
        EnTp_Head_SetupSinkIntoGroundAndReappear(this);
    }
}

void EnTp_SetupDying(EnTp* this) {
    Actor* child;

    this->timer = 2;
    if (this->actor.params < 0) {
        child = this->actor.child;
        while (child != NULL) {
            child->params = ENTP_TYPE_BODYELEM_DYING;
            child->colChkInfo.health = 0;
            child = child->child;
        }
        this->timer = 13;
        Actor_PlaySfx(&this->actor, NA_SE_EN_TAIL_DEAD);
    }
    this->action = ENTP_ACTION_DYING;
    EnTp_SetupAction(this, EnTp_Dying);
}

void EnTp_Dying(EnTp* this, PlayState* play) {
    s16 i;
    EnTp* actor;
    Vec3f sp78 = { 0.0f, 0.5f, 0.0f };
    Vec3f sp6C = { 0.0f, 0.0f, 0.0f };

    this->timer--;
    if (this->timer <= 0) {
        if (this->actor.params == ENTP_TYPE_HEAD_DYING) {
            sp6C.x = ((Rand_ZeroOne() - 0.5f) * 15.0f) + this->actor.world.pos.x;
            sp6C.z = ((Rand_ZeroOne() - 0.5f) * 15.0f) + this->actor.world.pos.z;
            sp6C.y = ((Rand_ZeroOne() - 0.5f) * 5.0f) + this->actor.world.pos.y;
            EffectSsDeadDb_Spawn(play, &sp6C, &sp78, &sp78, 100, 0, 255, 255, 255, 255, 0, 0, 255, 1, 9, true);
            sp6C.x = ((Rand_ZeroOne() - 0.5f) * 15.0f) + this->actor.world.pos.x;
            sp6C.z = ((Rand_ZeroOne() - 0.5f) * 15.0f) + this->actor.world.pos.z;
            sp6C.y = ((Rand_ZeroOne() - 0.5f) * 5.0f) + this->actor.world.pos.y;
            EffectSsDeadDb_Spawn(play, &sp6C, &sp78, &sp78, 100, 0, 255, 255, 255, 255, 0, 0, 255, 1, 9, true);
            Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos,
                                       COLLECTIBLE_DROP_RANDOM_PARAMS(COLLECTIBLE_DROP_TABLE_5, false));
        } else {
#if OOT_VERSION < NTSC_1_1
            for (i = 0; i < 2; i++)
#else
            for (i = 0; i < 1; i++)
#endif
            {
                actor = (EnTp*)Actor_Spawn(&play->actorCtx, play, ACTOR_EN_TP, this->actor.world.pos.x,
                                           this->actor.world.pos.y, this->actor.world.pos.z, 0, 0, 0,
                                           ENTP_TYPE_BODYELEMREMNANT);
                if (actor != NULL) {
                    Actor_SetScale(&actor->actor, this->actor.scale.z * 0.5f);
                    actor->redShineAmount = this->redShineAmount;
                }
            }
        }
        if (this->actor.child != NULL) {
            this->actor.child->parent = NULL;
            this->actor.child->params = ENTP_TYPE_BODYELEM_DYING;
            this->actor.child->colChkInfo.health = 0;
        }
        this->modeVar = 2;
        Actor_Kill(&this->actor);
    }
}

void EnTp_SetupBodyElemRemnant(EnTp* this) {
    this->action = ENTP_ACTION_BODYELEMREMNANT;
    this->actor.world.pos.x += (Rand_ZeroOne() - 0.5f) * 5.0f;
    this->actor.world.pos.y += (Rand_ZeroOne() - 0.5f) * 5.0f;
    this->actor.world.pos.z += (Rand_ZeroOne() - 0.5f) * 5.0f;
    this->actor.velocity.x = (Rand_ZeroOne() - 0.5f) * 1.5f;
    this->actor.velocity.y = (Rand_ZeroOne() - 0.5f) * 1.5f;
    this->actor.velocity.z = (Rand_ZeroOne() - 0.5f) * 1.5f;
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    EnTp_SetupAction(this, EnTp_BodyElemRemnant);
}

void EnTp_BodyElemRemnant(EnTp* this, PlayState* play) {
    Actor_UpdatePos(&this->actor);
    this->bodyElemAlpha -= 20;
    if (this->bodyElemAlpha < 20) {
        this->bodyElemAlpha = 0;
        Actor_Kill(&this->actor);
    }
}

void EnTp_Head_SetupHover(EnTp* this) {
    this->timer = (Rand_ZeroOne() * 15.0f) + 40.0f;
    this->action = ENTP_ACTION_HEAD_8;
    EnTp_SetupAction(this, EnTp_Head_Hover);
}

void EnTp_Head_Hover(EnTp* this, PlayState* play) {
    Player* player;

    player = GET_PLAYER(play);
    Math_SmoothStepToF(&this->actor.speed, 2.5f, 0.1f, 0.2f, 0.0f);
    Math_SmoothStepToF(&this->actor.world.pos.y, player->actor.world.pos.y + 85.0f + this->bodyElemDist, 1.0f,
                       this->actor.speed * 0.25f, 0.0f);
    SFX_PLAY_AT_POS(&this->actor.projectedPos, NA_SE_EN_TAIL_FLY - SFX_FLAG);
    if (this->collider.base.atFlags & AT_HIT) {
        this->collider.base.atFlags &= ~AT_HIT;
        if (&player->actor == this->collider.base.at) {
            this->timer2 = 1;
        }
    }
    if (this->redShineAmount != 0) {
        this->redShineAmount -= 15;
    }
    if (Math_CosF(this->unk_168) == 0.0f) {
        this->unk_170 = Rand_ZeroOne() * 4.0f;
    }
    this->actor.world.pos.y += Math_CosF(this->unk_168) * ((this->actor.speed * 0.25f) + this->unk_170);
    this->actor.world.rot.y += this->unk_164;
    this->unk_168 += 0.2f;
    if (this->timer != 0) {
        this->timer--;
    }
    Math_SmoothStepToS(&this->actor.world.rot.y, Math_Vec3f_Yaw(&this->actor.world.pos, &this->actor.home.pos), 1,
                       0x2EE, 0);
    if (this->timer == 0) {
        EnTp_Head_SetupAttack(this);
    }
    this->actor.shape.rot.y = this->actor.world.rot.y;
}

void EnTp_Head_SetupWaitPlayerAndRise(EnTp* this) {
    this->action = ENTP_ACTION_HEAD_WAIT_PLAYER_AND_RISE;
    this->modeVar = 0;
    this->actor.shape.rot.x = -0x4000;
    this->timer = 60;
    this->timer2 = 0;
    this->actor.speed = 0.0f;
    EnTp_SetupAction(this, EnTp_Head_WaitPlayerAndRise);
}

void EnTp_Head_WaitPlayerAndRise(EnTp* this, PlayState* play) {
    Player* player;
    s16 sp32;

    player = GET_PLAYER(play);
    this->timer2--;
    if (this->actor.xzDistToPlayer < 200.0f) {
        if (this->collider.base.atFlags & AT_HIT) {
            this->collider.base.atFlags &= ~AT_HIT;
            if (&player->actor == this->collider.base.at) {
                this->timer = 0;
            }
        }
        if (this->timer != 0) {
            this->timer--;
            Math_SmoothStepToS(&this->actor.shape.rot.x, 0, 1, 0x1F4, 0);
            Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 1, 0x5DC, 0);
            sp32 = Math_Vec3f_Yaw(&this->actor.home.pos, &player->actor.world.pos) + 0x4000;
            Math_SmoothStepToF(&this->actor.world.pos.y, this->actor.home.pos.y + 30.0f, 0.3f, 1.0f, 0.3f);
            this->actor.world.pos.x =
                (Math_SinS(this->timer2 * 0x7D0) * (Math_SinS(sp32) * this->bodyElemDist)) + this->actor.home.pos.x;
            this->actor.world.pos.z =
                (Math_SinS(this->timer2 * 0x7D0) * (Math_CosS(sp32) * this->bodyElemDist)) + this->actor.home.pos.z;
        } else {
            this->actor.shape.rot.x = 0;
            this->modeVar = 1;
            EnTp_Head_SetupHover(this);
        }
    } else {
        Math_SmoothStepToS(&this->actor.shape.rot.x, -0x4000, 1, 0x1F4, 0);
        if (Math_SmoothStepToF(&this->actor.world.pos.y, this->actor.home.pos.y, 0.3f, 1.5f, 0.3f) == 0.0f) {
            this->timer = 60;
        } else {
            sp32 = Math_Vec3f_Yaw(&this->actor.home.pos, &player->actor.world.pos);
            this->actor.world.pos.x =
                (Math_SinS(this->timer2 * 0x7D0) * (Math_SinS(sp32) * 6.0f)) + this->actor.home.pos.x;
            this->actor.world.pos.z =
                (Math_SinS(this->timer2 * 0x7D0) * (Math_CosS(sp32) * 6.0f)) + this->actor.home.pos.z;
        }
    }
    this->actor.shape.rot.y = this->actor.world.rot.y;
    if (this->actor.world.pos.y != this->actor.home.pos.y) {
        SFX_PLAY_AT_POS(&this->actor.projectedPos, NA_SE_EN_TAIL_FLY - SFX_FLAG);
    }
}

void EnTp_Head_SetupSinkIntoGroundAndReappear(EnTp* this) {
    this->action = ENTP_ACTION_HEAD_SINK_INTO_GROUND_AND_REAPPEAR;
    this->timer = 0;
    EnTp_SetupAction(this, EnTp_Head_SinkIntoGroundAndReappear);
}

void EnTp_Head_SinkIntoGroundAndReappear(EnTp* this, PlayState* play) {
    static Vec3f sBubbleAccel = { 0.0f, -0.5f, 0.0f };
    static Color_RGBA8 sBubblePrimColor = { 255, 255, 255, 255 };
    static Color_RGBA8 sBubbleEnvColor = { 150, 150, 150, 0 };
    Vec3f bubbleVel;
    Vec3f bubblePos;
    s32 isInsideGround;
    EnTp* child;
    s16 new_var;

    isInsideGround = false;
    this->timer2--;
    if ((this->timer != 0) || ((this->actor.home.pos.y - this->actor.world.pos.y) > 60.0f)) {
        if (1) {
            this->timer--;
        }
        if ((new_var = this->timer) == 0) {
            EnTp_Head_SetupWaitPlayerAndRise(this);
            child = (EnTp*)this->actor.child;
            while (child != NULL) {
                child->timer2 = child->timer;
                child = (EnTp*)child->actor.child;
            }
        } else {
            if (this->actor.shape.rot.x != -0x4000) {
                this->timer = 80;
                this->actor.velocity.y = 0.0f;
                this->actor.speed = 0.0f;
                this->actor.world.pos = this->actor.home.pos;
                this->actor.shape.rot.x = -0x4000;
                child = (EnTp*)this->actor.child;
                while (child != NULL) {
                    child->actor.velocity.y = 0.0f;
                    child->actor.speed = 0.0f;
                    child->actor.world.pos = this->actor.home.pos;
                    child->actor.world.pos.y = this->actor.home.pos.y - 80.0f;
                    child = (EnTp*)child->actor.child;
                }
            }
            this->actor.world.pos.y = this->actor.home.pos.y - (f32)this->timer;
        }
    } else {
        if (this->actor.shape.rot.x != 0x4000) {
            this->actor.shape.rot.x -= 0x400;
        }
        if (this->redShineAmount != 0) {
            this->redShineAmount -= 15;
        }
        this->actor.speed = 2.0f * Math_CosS(this->actor.shape.rot.x);
        this->actor.velocity.y = Math_SinS(this->actor.shape.rot.x) * -2.0f;
        if ((this->actor.world.pos.y - this->actor.floorHeight) < 20.0f) {
            isInsideGround = true;
        }
        if (this->actor.world.pos.y != this->actor.home.pos.y) {
            SFX_PLAY_AT_POS(&this->actor.projectedPos, NA_SE_EN_TAIL_FLY - SFX_FLAG);
        }
        if (isInsideGround && (play->gameplayFrames & 1)) {
            bubblePos = this->actor.world.pos;
            bubblePos.y = this->actor.floorHeight;
            bubbleVel.x = Rand_CenteredFloat(5.0f);
            bubbleVel.y = (Rand_ZeroOne() * 3.5f) + 1.5f;
            bubbleVel.z = Rand_CenteredFloat(5.0f);
            EffectSsDtBubble_SpawnCustomColor(play, &bubblePos, &bubbleVel, &sBubbleAccel, &sBubblePrimColor,
                                              &sBubbleEnvColor, Rand_S16Offset(100, 50), 20, 0);
        }
    }
}

void EnTp_CheckCollide(EnTp* this, PlayState* play) {
    EnTp* parent;
    EnTp* child;
    s32 isHeadInstance;
    s32 applyStun;
    EnTp* headInstance;

    if ((this->collider.base.acFlags & AC_HIT) && (this->action >= ENTP_ACTION_BODYELEM)) {
        isHeadInstance = false;
        applyStun = false;
        if (this->actor.params < 0) {
            isHeadInstance = true;
        }
        this->collider.base.acFlags &= ~AC_HIT;
        Actor_SetDropFlagJntSph(&this->actor, &this->collider, true);
        this->unk_158 = this->actor.colChkInfo.damageEffect;
        if (this->actor.colChkInfo.damageEffect != ENTP_DMG_REACT_NO_EFFECT) {
            if (this->actor.colChkInfo.damageEffect == ENTP_DMG_REACT_STUN) {
                applyStun = true;
            }
            if (!isHeadInstance) {
                Actor_ApplyDamage(&this->actor);
            }
            if (this->actor.colChkInfo.health == 0) {
                this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
                headInstance = this->headInstance;
                if (headInstance->actor.params < 0) {
                    EnTp_SetupDying(headInstance);
                    headInstance->unk_158 = this->actor.colChkInfo.damageEffect;
                    headInstance->actor.params = ENTP_TYPE_HEAD_DYING;
                }
            } else {
                if (applyStun) {
                    this->actor.freezeTimer = 80;
                    Actor_PlaySfx(&this->actor, NA_SE_EN_GOMA_JR_FREEZE);
                    if (isHeadInstance) {
                        Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_BLUE, 255, COLORFILTER_BUFFLAG_OPA,
                                             80);
                    } else {
                        Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_BLUE, 255, COLORFILTER_BUFFLAG_XLU,
                                             80);
                    }
                }
                parent = (EnTp*)this->actor.parent;
                while (parent != NULL) {
                    parent->collider.base.acFlags &= ~AC_HIT;
                    if (applyStun) {
                        parent->actor.freezeTimer = 80;
                        Actor_PlaySfx(&this->actor, NA_SE_EN_GOMA_JR_FREEZE);
                        if (isHeadInstance) {
                            Actor_SetColorFilter(&parent->actor, COLORFILTER_COLORFLAG_BLUE, 255,
                                                 COLORFILTER_BUFFLAG_OPA, 80);
                        } else {
                            Actor_SetColorFilter(&parent->actor, COLORFILTER_COLORFLAG_BLUE, 255,
                                                 COLORFILTER_BUFFLAG_XLU, 80);
                        }
                    }
                    parent = (EnTp*)parent->actor.parent;
                }
                child = (EnTp*)this->actor.child;
                while (child != NULL) {
                    child->collider.base.acFlags &= ~AC_HIT;
                    if (applyStun) {
                        child->actor.freezeTimer = 80;
                        if (isHeadInstance) {
                            Actor_SetColorFilter(&child->actor, COLORFILTER_COLORFLAG_BLUE, 255,
                                                 COLORFILTER_BUFFLAG_OPA, 80);
                        } else {
                            Actor_SetColorFilter(&child->actor, COLORFILTER_COLORFLAG_BLUE, 255,
                                                 COLORFILTER_BUFFLAG_XLU, 80);
                        }
                    }
                    child = (EnTp*)child->actor.child;
                }
            }
        }
    }
}

void EnTp_Update(Actor* thisx, PlayState* play) {
    Player* player;
    Vec3f sparkVel = { 0.0f, 0.0f, 0.0f };
    Vec3f sparkAccel = { 0.0f, -0.6f, 0.0f };
    Vec3f sparkPos;
    Color_RGBA8 sparkPrimColor = { 0, 0, 255, 255 };
    Color_RGBA8 sparkEnvColor = { 0, 0, 0, 0 };
    EnTp* this = (EnTp*)thisx;
    s16 v;

    player = GET_PLAYER(play);

    if (player->stateFlags1 & PLAYER_STATE1_26) {
        this->unk_158 = ENTP_DMG_REACT_NO_EFFECT;
    }
    if (this->actor.colChkInfo.health != 0) {
        EnTp_CheckCollide(this, play);
    }
    this->actionFunc(this, play);
    if (this->actor.params < 0) {
        Actor_MoveXZGravity(&this->actor);
        if (this->action != ENTP_ACTION_HEAD_SINK_INTO_GROUND_AND_REAPPEAR) {
            Actor_UpdateBgCheckInfo(play, &this->actor, 0.0f, 15.0f, 10.0f,
                                    UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2);
        }
        if ((this->actor.speed != 0.0f) && (this->actor.bgCheckFlags & BGCHECKFLAG_WALL)) {
            v = this->actor.wallYaw - this->actor.world.rot.y;
            if (ABS(v) > 0x4000) {
                if (v >= 0) {
                    this->actor.world.rot.y -= 0x1F4;
                } else {
                    this->actor.world.rot.y += 0x1F4;
                }
                this->actor.shape.rot.y = this->actor.world.rot.y;
            }
        }
        this->actor.shape.rot.z += 0x800;
        if (this->actor.shape.rot.z == 0) {
            SFX_PLAY_AT_POS(&this->actor.projectedPos, NA_SE_EN_TAIL_CRY);
        }
        if (this->action >= ENTP_ACTION_BODYELEM) {
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
        }
    }
    if (this->actor.params != ENTP_TYPE_BODYELEM_DYING) {
        this->sparkTimer--;
        this->sparkTimer &= 7;
    }
    this->actor.focus.pos = this->actor.world.pos;
    if (this->unk_158 == ENTP_DMG_REACT_MELEE_BACKLASH) {
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
    }
    if ((this->sparkTimer & 7) == 0) {
        sparkPrimColor.r = this->redShineAmount;
        sparkAccel.x = -this->actor.velocity.x * 0.25f;
        sparkAccel.y = -this->actor.velocity.y * 0.25f;
        sparkAccel.z = -this->actor.velocity.z * 0.25f;
        sparkPos.x = ((Rand_ZeroOne() - 0.5f) * 25.0f) + this->actor.world.pos.x;
        sparkPos.y = ((Rand_ZeroOne() - 0.5f) * 20.0f) + this->actor.world.pos.y;
        sparkPos.z = ((Rand_ZeroOne() - 0.5f) * 25.0f) + this->actor.world.pos.z;
        EffectSsKiraKira_SpawnSmall(play, &sparkPos, &sparkVel, &sparkAccel, &sparkPrimColor, &sparkEnvColor);
    }
    if ((this->action >= ENTP_ACTION_BODYELEM) && (this->actor.colChkInfo.health != 0)) {
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    }
}

void EnTp_Draw(Actor* thisx, PlayState* play) {
    s16 type;
    EnTp* this = (EnTp*)thisx;

    OPEN_DISPS(play->state.gfxCtx);
    if (this->modeVar != 2) {
        type = this->actor.params;
        if ((type < 0) || (type == ENTP_TYPE_HEAD_DYING)) {
            Gfx_SetupDL_25Opa(play->state.gfxCtx);
            MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);
            gSPDisplayList(POLY_OPA_DISP++, gTailpasaranHeadDL);
            Matrix_Translate(0.0f, 0.0f, 8.0f, MTXMODE_APPLY);
        } else {
            Gfx_SetupDL_25Xlu(play->state.gfxCtx);
            Matrix_ReplaceRotation(&play->billboardMtxF);
            gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x00, this->redShineAmount, 0, 255, this->bodyElemAlpha);
            gDPPipeSync(POLY_XLU_DISP++);
            gDPSetCombineLERP(POLY_XLU_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, PRIMITIVE, ENVIRONMENT,
                              TEXEL0, ENVIRONMENT, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, PRIMITIVE, ENVIRONMENT,
                              TEXEL0, ENVIRONMENT);
            gDPPipeSync(POLY_XLU_DISP++);
            gSPSegment(POLY_XLU_DISP++, 8, SEGMENTED_TO_VIRTUAL(gTailpasaranBodyElemTex));
            gDPPipeSync(POLY_XLU_DISP++);
            MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
            gSPDisplayList(POLY_XLU_DISP++, gTailpasaranBodyElemDL);
        }
    }
    CLOSE_DISPS();
    if ((this->actor.params <= 0) || (this->actor.params == ENTP_TYPE_BODYELEM_DYING)) {
        Collider_UpdateSpheres(0, &this->collider);
    }
}

OVL_INFO_NATIVE_ENEMY(0x35, En_Tp_Profile);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_TP_INIT, EnTp_Init);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_TP_DESTROY, EnTp_Destroy);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_TP_UPDATE, EnTp_Update);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_TP_DRAW, EnTp_Draw);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_TP_PROFILE, En_Tp_Profile);
static const u32 kNativePortOrigin[] = { 0x4e505431, 0, 0x35, 0x22, 0x140be849 };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_TP_ORIGIN, kNativePortOrigin);
