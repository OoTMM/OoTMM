#include "oot_compat.h"
#include "types/en_ny.h"
#undef ACTOR_EN_NY
#define ACTOR_EN_NY 0x00ec
#undef OBJECT_NY
#define OBJECT_NY 0x10e5
#undef NA_SE_EN_DODO_M_GND
#define NA_SE_EN_DODO_M_GND 0x387b
#undef NA_SE_EN_NYU_MOVE
#define NA_SE_EN_NYU_MOVE 0x38e9
#undef NA_SE_EN_NYU_HIT_STOP
#define NA_SE_EN_NYU_HIT_STOP 0x38ea
#undef NA_SE_EN_NYU_DEAD
#define NA_SE_EN_NYU_DEAD 0x394b
#define gEnNyRockBodyDL (*((Gfx (*)[1])0x06001dd0))
#define gEnNyMetalBodyDL (*((Gfx (*)[1])0x06001ea8))
#define gEnNySpikeDL (*((Gfx (*)[1])0x06001fc8))

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
#undef Actor_PlaySfx_Flagged
#define Actor_PlaySfx_Flagged FOREIGN_ENEMY_API_SFX_FLAGGED
#undef Actor_ProcessInitChain
#define Actor_ProcessInitChain ((void (*)(Actor* actor, InitChainEntry* ichain))0x800ffadc)
#undef Actor_SetColorFilter
#define Actor_SetColorFilter FOREIGN_ENEMY_API_COLOR_FILTER
#undef Actor_SetFocus
#define Actor_SetFocus ((void (*)(Actor* actor, f32 yOffset))0x800b675c)
#undef Actor_SetScale
#define Actor_SetScale ((void (*)(Actor* actor, f32 scale))0x800b67e0)
#undef Actor_UpdateBgCheckInfo
#define Actor_UpdateBgCheckInfo ((void (*)(PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight, s32 flags))0x800b78b8)
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
#undef CollisionCheck_SetOC
#define CollisionCheck_SetOC FOREIGN_ENEMY_API_COLLISION_OC
#undef EffectSsBubble_Spawn
#define EffectSsBubble_Spawn ((void (*)(PlayState* play, Vec3f* pos, f32 yPosOffset, f32 yPosRandScale, f32 xzPosRandScale, f32 scale))0x800b1e94)
#undef EffectSsDtBubble_SpawnColorProfile
#define EffectSsDtBubble_SpawnColorProfile ((void (*)(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 life, s16 colorProfile, s16 randXZ))0x800b2090)
#undef EffectSsEnFire_SpawnVec3f
#define EffectSsEnFire_SpawnVec3f ((void (*)(PlayState* play, Actor* actor, Vec3f* pos, s16 scale, s16 arg4, s16 flags, s16 bodyPart))0x800b2df4)
#undef EffectSsHitMark_SpawnFixedScale
extern void FOREIGN_ENEMY_API_FIXED_HIT_MARK(PlayState* play, s32 type, Vec3f* pos);
#define EffectSsHitMark_SpawnFixedScale FOREIGN_ENEMY_API_FIXED_HIT_MARK
#undef Enemy_StartFinishingBlow
#define Enemy_StartFinishingBlow ((void (*)(PlayState* play, Actor* actor))0x800bba88)
#undef Gfx_SetupDL_25Opa
#define Gfx_SetupDL_25Opa ((void (*)(GraphicsContext* gfxCtx))0x8012c28c)
#undef Gfx_SetupDL_25Xlu
#define Gfx_SetupDL_25Xlu ((void (*)(GraphicsContext* gfxCtx))0x8012c2dc)
#undef Item_DropCollectible
extern EnItem00* FOREIGN_ENEMY_API_DROP(PlayState* play, Vec3f* spawnPos, s16 params);
#define Item_DropCollectible FOREIGN_ENEMY_API_DROP
#undef Item_DropCollectibleRandom
#define Item_DropCollectibleRandom FOREIGN_ENEMY_API_RANDOM_DROP
#undef Math_ApproachF
#define Math_ApproachF ((void (*)(f32* pValue, f32 target, f32 fraction, f32 step))0x800ffdf8)
#undef Math_FAtan2F
#define Math_FAtan2F ((f32 (*)(f32 y, f32 x))0x80086b30)
#undef Math_SmoothStepToS
#define Math_SmoothStepToS ((s16 (*)(s16* pValue, s16 target, s16 scale, s16 step, s16 minStep))0x800ffebc)
#undef Math_StepToF
#define Math_StepToF ((s32 (*)(f32* pValue, f32 target, f32 step))0x800ff03c)
#undef Matrix_Finalize
#define Matrix_Finalize(gfx,...) (((Mtx* (*)(GraphicsContext*))0x80181a40)(gfx))
#undef Matrix_Scale
#define Matrix_Scale FOREIGN_ENEMY_API_MATRIX_SCALE
#undef Matrix_Translate
#define Matrix_Translate FOREIGN_ENEMY_API_MATRIX_TRANSLATE
#undef Rand_CenteredFloat
#define Rand_CenteredFloat ((f32 (*)(f32 f))0x801795c0)
#undef Rand_S16Offset
#define Rand_S16Offset ((s16 (*)(s16 base, s16 range))0x800ff450)
#undef Rand_ZeroFloat
#define Rand_ZeroFloat ((f32 (*)(f32 f))0x80179594)
#undef cosf
#define cosf ((f32 (*)(f32 x))0x80091f40)
#undef func_8002EBCC
#define func_8002EBCC ((void (*)(Actor* actor, PlayState* play, s32 flag))0x800b8050)
#undef func_8002ED80
#define func_8002ED80 ((void (*)(Actor* actor, PlayState* play, s32 flag))0x800b8118)
#undef sinf
#define sinf ((f32 (*)(f32 x))0x80088350)

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE)

void EnNy_Init(Actor* thisx, PlayState* play);
void EnNy_Destroy(Actor* thisx, PlayState* play);
void EnNy_Update(Actor* thisx, PlayState* play);
void EnNy_Draw(Actor* thisx, PlayState* play);

void EnNy_UpdateUnused(Actor* thisx, PlayState* play2);
void EnNy_Move(EnNy* this, PlayState* play);
void EnNy_Die(EnNy* this, PlayState* play);
void func_80ABCD40(EnNy* this);
void func_80ABCDBC(EnNy* this);
void EnNy_TurnToStone(EnNy* this, PlayState* play);
void func_80ABD11C(EnNy* this, PlayState* play);
void func_80ABCE50(EnNy* this, PlayState* play);
void func_80ABCE90(EnNy* this, PlayState* play);
void func_80ABCEEC(EnNy* this, PlayState* play);
void EnNy_UpdateDeath(Actor* thisx, PlayState* play);
void EnNy_SetupDie(EnNy* this, PlayState* play);
void EnNy_DrawDeathEffect(Actor* thisx, PlayState* play);
void func_80ABD3B8(EnNy* this, f32, f32);

ActorProfile En_Ny_Profile = {
    0xcec, ACTORCAT_ENEMY, FLAGS, OBJECT_NY, sizeof(EnNy), EnNy_Init, EnNy_Destroy, EnNy_Update, EnNy_Draw,
};

static ColliderJntSphElementInit sJntSphElementsInit[] = {
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0xFFCFFFFF, HIT_SPECIAL_EFFECT_KNOCKBACK, 0x08 },
            { 0xFFCFFFFF, HIT_BACKLASH_NONE, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_ON,
        },
        { 0, { { 0, 0, 0 }, 15 }, 100 },
    },
};

static ColliderJntSphInit sColliderJntSphInit = {
    {
        COL_MATERIAL_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sJntSphElementsInit),
    sJntSphElementsInit,
};

static DamageTable sDamageTable = { {
    DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(2, 0xF), DMG_ENTRY(0, 0x0), DMG_ENTRY(2, 0xF),
    DMG_ENTRY(2, 0xF), DMG_ENTRY(2, 0x1), DMG_ENTRY(0, 0x0), DMG_ENTRY(2, 0xF), DMG_ENTRY(4, 0xF), DMG_ENTRY(4, 0x2),
    DMG_ENTRY(2, 0xF), DMG_ENTRY(2, 0xF), DMG_ENTRY(4, 0xE), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(4, 0x2),
    DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(4, 0xF),
    DMG_ENTRY(2, 0xF), DMG_ENTRY(0, 0x0), DMG_ENTRY(8, 0xF), DMG_ENTRY(4, 0xF), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0),
} };

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(naviEnemyId, 0xff, ICHAIN_CONTINUE),
    ICHAIN_U8(attentionRangeType, ATTENTION_RANGE_2, ICHAIN_CONTINUE),
    ICHAIN_F32(lockOnArrowOffset, 30, ICHAIN_STOP),
};

void EnNy_Init(Actor* thisx, PlayState* play) {
    EnNy* this = (EnNy*)thisx;

    Actor_ProcessInitChain(&this->actor, sInitChain);
    this->actor.colChkInfo.damageTable = &sDamageTable;
    this->actor.colChkInfo.health = 2;
    Collider_InitJntSph(play, &this->collider);
    Collider_SetJntSph(play, &this->collider, &this->actor, &sColliderJntSphInit, this->colliderElements);
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 20.0f);
    this->unk_1CA = 0;
    this->unk_1D0 = 0;
    Actor_SetScale(&this->actor, 0.01f);
    this->actor.speed = 0.0f;
    this->actor.shape.rot.y = 0;
    this->actor.gravity = -0.4f;
    this->hitPlayer = 0;
    this->unk_1CE = 2;
    this->actor.velocity.y = 0.0f;
    this->unk_1D4 = 0xFF;
    this->unk_1D8 = 0;
    this->unk_1E8 = 0.0f;
    this->unk_1E0 = 0.25f;
    if (this->actor.params == 0) {
        this->actor.colChkInfo.mass = 0;
        this->unk_1D4 = 0;
        this->unk_1D8 = 0xFF;
        this->unk_1E0 = 1.0f;
        func_80ABCDBC(this);
    } else {
        this->actor.colChkInfo.mass = MASS_IMMOVABLE;
        this->actor.update = EnNy_UpdateUnused;
        this->collider.base.colMaterial = COL_MATERIAL_METAL;
    }
}

void EnNy_Destroy(Actor* thisx, PlayState* play) {
    EnNy* this = (EnNy*)thisx;
    Collider_DestroyJntSph(play, &this->collider);
}

void func_80ABCD40(EnNy* this) {
    f32 temp;

    temp = (this->actor.depthInWater > 0.0f) ? 0.7f : 1.0f;
    this->unk_1E8 = 2.8f * temp;
}

void func_80ABCD84(EnNy* this) {
    this->actionFunc = func_80ABCE50;
}

void func_80ABCD94(EnNy* this) {
    this->stoneTimer = 0x14;
    this->actionFunc = func_80ABCE90;
}

void func_80ABCDAC(EnNy* this) {
    this->actionFunc = func_80ABCEEC;
}

void func_80ABCDBC(EnNy* this) {
    this->unk_1F4 = 0.0f;
    func_80ABCD40(this);
    this->stoneTimer = 180;
    this->actionFunc = EnNy_Move;
}

void EnNy_SetupTurnToStone(EnNy* this) {
    Actor_PlaySfx(&this->actor, NA_SE_EN_NYU_HIT_STOP);
    this->actionFunc = EnNy_TurnToStone;
    this->unk_1E8 = 0.0f;
}

void func_80ABCE38(EnNy* this) {
    this->stoneTimer = 0x3C;
    this->actionFunc = func_80ABD11C;
}

void func_80ABCE50(EnNy* this, PlayState* play) {
    if (this->actor.xyzDistToPlayerSq <= SQ(160.0f)) {
        func_80ABCD94(this);
    }
}

void func_80ABCE90(EnNy* this, PlayState* play) {
    s32 phi_v1;
    s32 phi_v0;

    phi_v1 = this->unk_1D4 - 0x40;
    phi_v0 = this->unk_1D8 + 0x40;
    if (phi_v0 >= 0xFF) {
        phi_v1 = 0;
        phi_v0 = 0xFF;
        func_80ABCDAC(this);
    }
    this->unk_1D4 = phi_v1;
    this->unk_1D8 = phi_v0;
}

void func_80ABCEEC(EnNy* this, PlayState* play) {
    f32 phi_f0;

    phi_f0 = this->unk_1E0;
    phi_f0 += 2.0f;
    if (phi_f0 >= 1.0f) {
        phi_f0 = 1.0f;
        func_80ABCDBC(this);
    }
    this->unk_1E0 = phi_f0;
}

void EnNy_Move(EnNy* this, PlayState* play) {
    f32 yawDiff;
    s32 stoneTimer;

    Actor_PlaySfx_Flagged(&this->actor, NA_SE_EN_NYU_MOVE - SFX_FLAG);
    func_80ABCD40(this);
    stoneTimer = this->stoneTimer;
    this->stoneTimer--;
    if ((stoneTimer <= 0) || this->hitPlayer) {
        EnNy_SetupTurnToStone(this);
    } else {
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 0xA, this->unk_1F4, 0);
        Math_ApproachF(&this->unk_1F4, 2000.0f, 1.0f, 100.0f);
        this->actor.world.rot.y = this->actor.shape.rot.y;
        yawDiff = Math_FAtan2F(this->actor.yDistanceFromLink, this->actor.xzDistToPlayer);
        this->actor.speed = fabsf(cosf(yawDiff) * this->unk_1E8);
        if (this->unk_1F0 < this->actor.depthInWater) {
            this->unk_1EC = sinf(yawDiff) * this->unk_1E8;
        }
    }
}

void EnNy_TurnToStone(EnNy* this, PlayState* play) {
    f32 phi_f0;

    phi_f0 = this->unk_1E0;
    phi_f0 -= 2.0f;
    if (phi_f0 <= 0.25f) {
        phi_f0 = 0.25f;
        if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND_TOUCH) {
            if (!(this->unk_1F0 < this->actor.depthInWater)) {
                Actor_PlaySfx(&this->actor, NA_SE_EN_DODO_M_GND);
            }
            this->actor.bgCheckFlags &= ~BGCHECKFLAG_GROUND_TOUCH;
            this->actor.speed = 0.0f;
            this->actor.world.rot.y = this->actor.shape.rot.y;
            func_80ABCE38(this);
        }
    }
    this->unk_1E0 = phi_f0;
}

void func_80ABD11C(EnNy* this, PlayState* play) {
    s32 phi_v0;
    s32 phi_v1;

    phi_v0 = this->unk_1D4;
    phi_v0 += 0x40;
    phi_v1 = this->unk_1D8;
    phi_v1 -= 0x40;
    if (phi_v0 >= 0xFF) {
        phi_v0 = 0xFF;
        phi_v1 = 0;
        if (this->stoneTimer != 0) {
            this->stoneTimer--;
        } else {
            func_80ABCD84(this);
        }
    }
    this->unk_1D4 = phi_v0;
    this->unk_1D8 = phi_v1;
}

s32 EnNy_CollisionCheck(EnNy* this, PlayState* play) {
    u8 sp3F;
    Vec3f effectPos;

    sp3F = 0;
    this->hitPlayer = 0;
    if (this->collider.base.atFlags & AT_BOUNCED) {
        this->collider.base.atFlags &= ~AT_BOUNCED;
        this->hitPlayer = 1;
        this->actor.world.rot.y = this->actor.yawTowardsPlayer;
        this->actor.speed = -4.0f;
        return 0;
    }
    if (this->collider.base.atFlags & AT_HIT) {
        this->collider.base.atFlags &= ~AT_HIT;
        this->hitPlayer = 1;
        return 0;
    } else {
        if (this->collider.base.acFlags & AC_HIT) {
            this->collider.base.acFlags &= ~AC_HIT;
            effectPos.x = this->collider.elements[0].base.acDmgInfo.hitPos.x;
            effectPos.y = this->collider.elements[0].base.acDmgInfo.hitPos.y;
            effectPos.z = this->collider.elements[0].base.acDmgInfo.hitPos.z;
            if ((this->unk_1E0 == 0.25f) && (this->unk_1D4 == 0xFF)) {
                switch (this->actor.colChkInfo.damageEffect) {
                    case 0xE:
                        sp3F = 1;
                        FALLTHROUGH;
                    case 0xF:
                        Actor_ApplyDamage(&this->actor);
                        Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_XLU, 80);
                        break;
                    case 1:
                        Actor_ApplyDamage(&this->actor);
                        Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_XLU, 80);
                        break;
                    case 2:
                        this->unk_1CA = 4;
                        Actor_ApplyDamage(&this->actor);
                        Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_XLU, 80);
                        break;
                }
            }
            this->stoneTimer = 0;
            if (this->actor.colChkInfo.health == 0) {
                this->actor.shape.shadowAlpha = 0;
                this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
                this->unk_1D0 = sp3F;
                Enemy_StartFinishingBlow(play, &this->actor);
                return 1;
            }
            EffectSsHitMark_SpawnFixedScale(play, 0, &effectPos);
            return 0;
        }
    }
    return 0;
}

void func_80ABD3B8(EnNy* this, f32 arg1, f32 arg2) {
    if (this->unk_1E8 == 0.0f) {
        this->actor.gravity = -0.4f;
    } else if (!(arg1 < this->actor.depthInWater)) {
        this->actor.gravity = -0.4f;
    } else if (arg2 < this->actor.depthInWater) {
        this->actor.gravity = 0.0;
        if (this->unk_1EC < this->actor.velocity.y) {
            this->actor.velocity.y -= 0.4f;
            if (this->actor.velocity.y < this->unk_1EC) {
                this->actor.velocity.y = this->unk_1EC;
            }
        } else if (this->actor.velocity.y < this->unk_1EC) {
            this->actor.velocity.y += 0.4f;
            if (this->unk_1EC < this->actor.velocity.y) {
                this->actor.velocity.y = this->unk_1EC;
            }
        }
    }
}

void EnNy_Update(Actor* thisx, PlayState* play) {
    EnNy* this = (EnNy*)thisx;
    f32 temp_f20;
    f32 temp_f22;

    this->timer++;
    temp_f20 = this->unk_1E0 - 0.25f;
    if (this->unk_1CA != 0) {
        this->unk_1CA--;
    }
    Actor_SetFocus(&this->actor, 0.0f);
    Actor_SetScale(&this->actor, 0.01f);
    this->collider.elements[0].dim.scale = 1.33f * temp_f20 + 1.0f;
    temp_f22 = (24.0f * temp_f20) + 12.0f;
    this->actor.shape.rot.x += (s16)(this->unk_1E8 * 1000.0f);
    func_80ABD3B8(this, temp_f22 + 10.0f, temp_f22 - 10.0f);
    Actor_MoveXZGravity(&this->actor);
    Math_StepToF(&this->unk_1E4, this->unk_1E8, 0.1f);
    this->actionFunc(this, play);
    this->actor.prevPos.y -= temp_f22;
    this->actor.world.pos.y -= temp_f22;
    Actor_UpdateBgCheckInfo(play, &this->actor, 20.0f, 20.0f, 60.0f,
                            UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_2);
    this->unk_1F0 = temp_f22;
    this->actor.world.pos.y += temp_f22;
    if (EnNy_CollisionCheck(this, play) != 0) {
        s32 i;

        for (i = 0; i < 8; i++) {
            this->unk_1F8[i].x = (Rand_CenteredFloat(20.0f) + this->actor.world.pos.x);
            this->unk_1F8[i].y = (Rand_CenteredFloat(20.0f) + this->actor.world.pos.y);
            this->unk_1F8[i].z = (Rand_CenteredFloat(20.0f) + this->actor.world.pos.z);
        }
        this->timer = 0;
        this->actor.update = EnNy_UpdateDeath;
        this->actor.draw = EnNy_DrawDeathEffect;
        this->actionFunc = EnNy_SetupDie;
        return;
    }
    if (this->unk_1E0 > 0.25f) {
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
    }
    CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
}

void EnNy_SetupDie(EnNy* this, PlayState* play) {
    s32 effectScale;
    s32 i;
    Vec3f effectPos;
    Vec3f effectVelocity = { 0.0f, 0.0f, 0.0f };
    Vec3f effectAccel = { 0.0f, 0.1f, 0.0f };

    if (this->timer >= 2) {
        if (this->actor.depthInWater > 0.0f) {
            for (i = 0; i < 10; i++) {
                effectPos.x = Rand_CenteredFloat(30.0f) + this->actor.world.pos.x;
                effectPos.y = Rand_CenteredFloat(30.0f) + this->actor.world.pos.y;
                effectPos.z = Rand_CenteredFloat(30.0f) + this->actor.world.pos.z;
                effectScale = Rand_S16Offset(0x50, 0x64);
                EffectSsDtBubble_SpawnColorProfile(play, &effectPos, &effectVelocity, &effectAccel, effectScale, 25, 0,
                                                   1);
            }
            for (i = 0; i < 0x14; i++) {
                effectPos.x = Rand_CenteredFloat(30.0f) + this->actor.world.pos.x;
                effectPos.y = Rand_CenteredFloat(30.0f) + this->actor.world.pos.y;
                effectPos.z = Rand_CenteredFloat(30.0f) + this->actor.world.pos.z;
                EffectSsBubble_Spawn(play, &effectPos, 10.0f, 10.0f, 30.0f, 0.25f);
            }
        }
        for (i = 0; i < 8; i++) {
            this->unk_1F8[i + 8].x = Rand_CenteredFloat(10.0f);
            this->unk_1F8[i + 8].z = Rand_CenteredFloat(10.0f);
            this->unk_1F8[i + 8].y = Rand_ZeroFloat(4.0f) + 4.0f;
        }
        this->timer = 0;
        if (this->unk_1D0 == 0) {
            Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos,
                                       COLLECTIBLE_DROP_RANDOM_PARAMS(COLLECTIBLE_DROP_TABLE_10, false));
        } else {
            Item_DropCollectible(play, &this->actor.world.pos, ITEM00_ARROWS_SMALL);
        }
        Actor_PlaySfx(&this->actor, NA_SE_EN_NYU_DEAD);
        this->actionFunc = EnNy_Die;
    }
}

void EnNy_Die(EnNy* this, PlayState* play) {
    s32 i;

    if (this->actor.depthInWater > 0.0f) {
        for (i = 0; i < 8; i += 1) {
            this->unk_1F8[i].x += this->unk_1F8[i + 8].x;
            this->unk_1F8[i].y += this->unk_1F8[i + 8].y;
            this->unk_1F8[i].z += this->unk_1F8[i + 8].z;
            Math_StepToF(&this->unk_1F8[i + 8].x, 0.0f, 0.1f);
            Math_StepToF(&this->unk_1F8[i + 8].y, -1.0f, 0.4f);
            Math_StepToF(&this->unk_1F8[i + 8].z, 0.0f, 0.1f);
        }
        if (this->timer >= 0x1F) {
            Actor_Kill(&this->actor);
            return;
        }
    } else {
        for (i = 0; i < 8; i += 1) {
            this->unk_1F8[i].x += this->unk_1F8[i + 8].x;
            this->unk_1F8[i].y += this->unk_1F8[i + 8].y;
            this->unk_1F8[i].z += this->unk_1F8[i + 8].z;
            Math_StepToF(&this->unk_1F8[i + 8].x, 0.0f, 0.15f);
            Math_StepToF(&this->unk_1F8[i + 8].y, -1.0f, 0.6f);
            Math_StepToF(&this->unk_1F8[i + 8].z, 0.0f, 0.15f);
        }
        if (this->timer >= 0x10) {
            Actor_Kill(&this->actor);
            return;
        }
    }
}

void EnNy_UpdateDeath(Actor* thisx, PlayState* play) {
    EnNy* this = (EnNy*)thisx;

    this->timer++;
    if (this->unk_1CA != 0) {
        this->unk_1CA--;
    }
    this->actionFunc(this, play);
}

void EnNy_UpdateUnused(Actor* thisx, PlayState* play2) {
    EnNy* this = (EnNy*)thisx;
    PlayState* play = play2;
    f32 sp3C;
    f32 temp_f0;

    sp3C = this->unk_1E0 - 0.25f;
    this->timer++;
    Actor_SetFocus(&this->actor, 0.0f);
    Actor_SetScale(&this->actor, 0.01f);
    temp_f0 = (24.0f * sp3C) + 12.0f;
    this->actor.prevPos.y -= temp_f0;
    this->actor.world.pos.y -= temp_f0;

    Actor_UpdateBgCheckInfo(play, &this->actor, 20.0f, 20.0f, 60.0f,
                            UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_2);
    this->unk_1F0 = temp_f0;
    this->actor.world.pos.y += temp_f0;

    CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
    Actor_MoveXZGravity(&this->actor);
    Math_StepToF(&this->unk_1E4, this->unk_1E8, 0.1f);
}

static Vec3f sFireOffsets[] = {
    { 5.0f, 0.0f, 0.0f },
    { -5.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 5.0f },
    { 0.0f, 0.0f, -5.0f },
};

void EnNy_Draw(Actor* thisx, PlayState* play) {
    EnNy* this = (EnNy*)thisx;
    const void* nativeKeep = (const void*)(gSegments[4] + K0BASE);
    const void* highlightBase = (const void*)((u32)nativeKeep + 0x2cab0 - 0x16ef0);

    OPEN_DISPS(play->state.gfxCtx);
    Collider_UpdateSpheres(0, &this->collider);
    func_8002ED80(&this->actor, play, 1);
    Gfx_SetupDL_25Xlu(play->state.gfxCtx);
    MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetRenderMode(POLY_XLU_DISP++, G_RM_PASS, G_RM_AA_ZB_XLU_SURF2);
    gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, this->unk_1D8);
    gSPSegment(POLY_XLU_DISP++, 4, highlightBase);
    gSPDisplayList(POLY_XLU_DISP++, gEnNyMetalBodyDL);
    gSPSegment(POLY_XLU_DISP++, 4, nativeKeep);
    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetRenderMode(POLY_XLU_DISP++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_XLU_SURF2);
    gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, this->unk_1D4);
    gSPDisplayList(POLY_XLU_DISP++, gEnNyRockBodyDL);
    if (this->unk_1E0 > 0.25f) {
        Matrix_Scale(this->unk_1E0, this->unk_1E0, this->unk_1E0, MTXMODE_APPLY);
        func_8002EBCC(&this->actor, play, 1);
        Gfx_SetupDL_25Opa(play->state.gfxCtx);
        MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);
        gSPSegment(POLY_OPA_DISP++, 4, highlightBase);
        gSPDisplayList(POLY_OPA_DISP++, gEnNySpikeDL);
        gSPSegment(POLY_OPA_DISP++, 4, nativeKeep);
    }
    CLOSE_DISPS();
    if (this->unk_1CA != 0) {
        Vec3f tempVec;
        Vec3f* fireOffset;
        s16 temp;

        temp = this->unk_1CA - 1;
        this->actor.colorFilterTimer++;
        if (temp == 0) {
            fireOffset = &sFireOffsets[temp & 3];
            tempVec.x = Rand_CenteredFloat(5.0f) + (this->actor.world.pos.x + fireOffset->x);
            tempVec.y = Rand_CenteredFloat(5.0f) + (this->actor.world.pos.y + fireOffset->y);
            tempVec.z = Rand_CenteredFloat(5.0f) + (this->actor.world.pos.z + fireOffset->z);
            EffectSsEnFire_SpawnVec3f(play, &this->actor, &tempVec, 100, 0, 0, -1);
        }
    }
}

void EnNy_DrawDeathEffect(Actor* thisx, PlayState* play) {
    EnNy* this = (EnNy*)thisx;
    Vec3f* temp;
    f32 scale;
    s32 i;

    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL_25Opa(play->state.gfxCtx);
    gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 255);
    gDPSetRenderMode(POLY_OPA_DISP++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2);
    gDPPipeSync(POLY_OPA_DISP++);
    for (i = 0; i < 8; i++) {
        if (this->timer < (i + 22)) {
            temp = &this->unk_1F8[i];
            Matrix_Translate(temp->x, temp->y, temp->z, MTXMODE_NEW);
            scale = this->actor.scale.x * 0.4f * (1.0f + (i * 0.04f));
            Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
            MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);
            gSPDisplayList(POLY_OPA_DISP++, gEnNyRockBodyDL);
        }
    }
    CLOSE_DISPS();
    if (this->unk_1CA != 0) {
        Vec3f tempVec;
        Vec3f* fireOffset;
        s16 fireOffsetIndex;

        fireOffsetIndex = this->unk_1CA - 1;
        this->actor.colorFilterTimer++;
        if ((fireOffsetIndex & 1) == 0) {
            fireOffset = &sFireOffsets[fireOffsetIndex & 3];
            tempVec.x = Rand_CenteredFloat(5.0f) + (this->actor.world.pos.x + fireOffset->x);
            tempVec.y = Rand_CenteredFloat(5.0f) + (this->actor.world.pos.y + fireOffset->y);
            tempVec.z = Rand_CenteredFloat(5.0f) + (this->actor.world.pos.z + fireOffset->z);
            EffectSsEnFire_SpawnVec3f(play, &this->actor, &tempVec, 100, 0, 0, -1);
        }
    }
}

OVL_INFO_NATIVE_ENEMY(0xec, En_Ny_Profile);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_NY_INIT, EnNy_Init);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_NY_DESTROY, EnNy_Destroy);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_NY_UPDATE, EnNy_Update);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_NY_DRAW, EnNy_Draw);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_NY_PROFILE, En_Ny_Profile);
static const u32 kNativePortOrigin[] = { 0x4e505431, 0, 0xec, 0xe5, 0x2e892cc9 };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_NY_ORIGIN, kNativePortOrigin);
