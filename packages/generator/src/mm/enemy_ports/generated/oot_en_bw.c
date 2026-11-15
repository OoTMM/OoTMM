#include "oot_compat.h"
#include "types/en_bw.h"
#undef ACTOR_EN_BW
#define ACTOR_EN_BW 0x0038
#undef OBJECT_BW
#define OBJECT_BW 0x1025
#undef NA_SE_EN_STAL_JUMP
#define NA_SE_EN_STAL_JUMP 0x386c
#undef NA_SE_EN_DODO_M_GND
#define NA_SE_EN_DODO_M_GND 0x387b
#undef NA_SE_EN_GOMA_JR_FREEZE
#define NA_SE_EN_GOMA_JR_FREEZE 0x389e
#undef NA_SE_EN_BUBLEWALK_WALK
#define NA_SE_EN_BUBLEWALK_WALK 0x3974
#undef NA_SE_EN_BUBLEWALK_AIM
#define NA_SE_EN_BUBLEWALK_AIM 0x3975
#undef NA_SE_EN_BUBLEWALK_REVERSE
#define NA_SE_EN_BUBLEWALK_REVERSE 0x3976
#undef NA_SE_EN_BUBLEWALK_DAMAGE
#define NA_SE_EN_BUBLEWALK_DAMAGE 0x3977
#undef NA_SE_EN_BUBLEWALK_DEAD
#define NA_SE_EN_BUBLEWALK_DEAD 0x3978
#undef NA_SE_PL_BODY_HIT
#define NA_SE_PL_BODY_HIT 0x083e
#define gEffFire1DL (*((Gfx (*)[1])0x04052a10))
#define gTorchSlugEyestalkWaveAnim (*(AnimationHeader*)0x06000228)
#define gTorchSlugSkel (*(SkeletonHeader*)0x060020f0)
#define gTorchSlugEyestalkRaiseAnim (*(AnimationHeader*)0x060021a0)
#define gTorchSlugEyestalkFlailAnim (*(AnimationHeader*)0x06002250)

#undef ActorShape_Init
#define ActorShape_Init ((void (*)(ActorShape* shape, f32 yOffset, ActorShadowFunc shadowDraw, f32 shadowScale))0x800b3ba4)
#undef Actor_ApplyDamage
#define Actor_ApplyDamage ((u8 (*)(Actor* actor))0x800be22c)
#undef Actor_IsFacingPlayer
#define Actor_IsFacingPlayer ((s32 (*)(Actor* actor, s16 maxAngle))0x800b73e0)
#undef Actor_Kill
#define Actor_Kill FOREIGN_ENEMY_API_KILL
#undef Actor_MoveXZGravity
#define Actor_MoveXZGravity ((void (*)(Actor* actor))0x800b6a88)
#undef Actor_PlaySfx
#define Actor_PlaySfx FOREIGN_ENEMY_API_SFX_ACTOR
#undef Actor_SetColorFilter
#define Actor_SetColorFilter FOREIGN_ENEMY_API_COLOR_FILTER
#undef Actor_SetDropFlag
#define Actor_SetDropFlag FOREIGN_ENEMY_API_DROP_FLAG
#undef Actor_SetScale
#define Actor_SetScale ((void (*)(Actor* actor, f32 scale))0x800b67e0)
#undef Actor_SpawnFloorDustRing
#define Actor_SpawnFloorDustRing ((void (*)(PlayState* play, Actor* actor, Vec3f* posXZ, f32 radius, s32 amountMinusOne, f32 randAccelWeight, s16 scale, s16 scaleStep, u8 useLighting))0x800bbdac)
#undef Actor_TestFloorInDirection
#define Actor_TestFloorInDirection ((s16 (*)(Actor* actor, PlayState* play, f32 distance, s16 angle))0x800bc4ec)
#undef Actor_UpdateBgCheckInfo
#define Actor_UpdateBgCheckInfo ((void (*)(PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight, s32 flags))0x800b78b8)
#undef Animation_MorphToLoop
#define Animation_MorphToLoop ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 morphFrames))0x80137594)
#undef Animation_MorphToPlayOnce
#define Animation_MorphToPlayOnce ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 morphFrames))0x80137488)
#undef BgCheck_AnyLineTest2
#define BgCheck_AnyLineTest2 ((s32 (*)(CollisionContext* colCtx, Vec3f* posA, Vec3f* posB, Vec3f* posResult, CollisionPoly** outPoly, s32 chkWall, s32 chkFloor, s32 chkCeil, s32 chkOneFace))0x800c583c)
#undef Collider_DestroyCylinder
#define Collider_DestroyCylinder FOREIGN_ENEMY_API_CYLINDER_DESTROY
#undef Collider_InitCylinder
#define Collider_InitCylinder ((s32 (*)(PlayState* play, ColliderCylinder* cyl))0x800e119c)
#undef Collider_SetCylinder
#define Collider_SetCylinder ((s32 (*)(PlayState* play, ColliderCylinder* dest, Actor* actor, ColliderCylinderInit* src))0x800e130c)
#undef Collider_UpdateCylinder
#define Collider_UpdateCylinder ((void (*)(Actor* actor, ColliderCylinder* cyl))0x800e7df8)
#undef CollisionCheck_SetAC
#define CollisionCheck_SetAC FOREIGN_ENEMY_API_COLLISION_AC
#undef CollisionCheck_SetAT
#define CollisionCheck_SetAT FOREIGN_ENEMY_API_COLLISION_AT
#undef CollisionCheck_SetOC
#define CollisionCheck_SetOC FOREIGN_ENEMY_API_COLLISION_OC
#undef EffectSsEnIce_SpawnFlyingVec3f
#define EffectSsEnIce_SpawnFlyingVec3f FOREIGN_ENEMY_API_FLYING_ICE_F
#undef Gfx_SetupDL_25Opa
#define Gfx_SetupDL_25Opa ((void (*)(GraphicsContext* gfxCtx))0x8012c28c)
#undef Gfx_SetupDL_25Xlu
#define Gfx_SetupDL_25Xlu ((void (*)(GraphicsContext* gfxCtx))0x8012c2dc)
#undef Gfx_TwoTexScroll
#define Gfx_TwoTexScroll ((Gfx* (*)(GraphicsContext* gfxCtx, s32 tile1, u32 x1, u32 y1, s32 width1, s32 height1, s32 tile2, u32 x2, u32 y2, s32 width2, s32 height2))0x8012cbd0)
#undef Item_DropCollectibleRandom
#define Item_DropCollectibleRandom FOREIGN_ENEMY_API_RANDOM_DROP
#undef Math_CosF
#define Math_CosF ((f32 (*)(f32 angle))0x80179540)
#undef Math_FAtan2F
#define Math_FAtan2F ((f32 (*)(f32 y, f32 x))0x80086b30)
#undef Math_SinF
#define Math_SinF ((f32 (*)(f32 angle))0x801794ec)
#undef Math_SinS
#define Math_SinS ((f32 (*)(s16 angle))0x800fed84)
#undef Math_SmoothStepToF
#define Math_SmoothStepToF ((f32 (*)(f32* pValue, f32 target, f32 fraction, f32 step, f32 minStep))0x800ffcd8)
#undef Math_SmoothStepToS
#define Math_SmoothStepToS ((s16 (*)(s16* pValue, s16 target, s16 scale, s16 step, s16 minStep))0x800ffebc)
#undef Matrix_Finalize
#define Matrix_Finalize(gfx,...) (((Mtx* (*)(GraphicsContext*))0x80181a40)(gfx))
#undef Matrix_MultVec3f
#define Matrix_MultVec3f ((void (*)(Vec3f* src, Vec3f* dest))0x80181a98)
#undef Matrix_Pop
#define Matrix_Pop ((void (*)(void))0x801801cc)
#undef Matrix_Push
#define Matrix_Push ((void (*)(void))0x8018019c)
#undef Matrix_ReplaceRotation
#define Matrix_ReplaceRotation ((void (*)(MtxF* mf))0x801820a0)
#undef Matrix_RotateX
#define Matrix_RotateX FOREIGN_ENEMY_API_MATRIX_ROTATE_XF
#undef Matrix_RotateY
#define Matrix_RotateY FOREIGN_ENEMY_API_MATRIX_ROTATE_YF
#undef Matrix_RotateZ
#define Matrix_RotateZ FOREIGN_ENEMY_API_MATRIX_ROTATE_ZF
#undef Matrix_Scale
#define Matrix_Scale FOREIGN_ENEMY_API_MATRIX_SCALE
#undef Matrix_Translate
#define Matrix_Translate FOREIGN_ENEMY_API_MATRIX_TRANSLATE
#undef Rand_CenteredFloat
#define Rand_CenteredFloat ((f32 (*)(f32 f))0x801795c0)
#undef Rand_ZeroOne
#define Rand_ZeroOne ((f32 (*)(void))0x80086fdc)
#undef SkelAnime_Draw
#define SkelAnime_Draw ((Gfx* (*)(PlayState* play, void** skeleton, Vec3s* jointTable, void* overrideLimbDraw, void* postLimbDraw, void* arg, Gfx* gfx))0x80134990)
#undef SkelAnime_Init
#define SkelAnime_Init ((void (*)(PlayState* play, SkelAnime* skelAnime, SkeletonHeader* skeletonHeaderSeg, AnimationHeader* animation, Vec3s* jointTable, Vec3s* morphTable, s32 limbCount))0x80136a7c)
#undef SkelAnime_Update
#define SkelAnime_Update ((s32 (*)(SkelAnime* skelAnime))0x80136cd0)
#undef SurfaceType_GetFloorType
#define SurfaceType_GetFloorType ((u32 (*)(CollisionContext* colCtx, CollisionPoly* poly, s32 bgId))0x800c99d4)
#undef func_8002836C
#define func_8002836C ((void (*)(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 scale, s16 scaleStep, s16 life))0x800b0eb0)
#undef func_800355E4
extern u8 FOREIGN_ENEMY_API_FORWARD_HAMMER(PlayState* play, Collider* collider);
#define func_800355E4 FOREIGN_ENEMY_API_FORWARD_HAMMER

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED)

void EnBw_Init(Actor* thisx, PlayState* play);
void EnBw_Destroy(Actor* thisx, PlayState* play);
void EnBw_Update(Actor* thisx, PlayState* play2);
void EnBw_Draw(Actor* thisx, PlayState* play2);

void func_809CE884(EnBw* this, PlayState* play);
void func_809CE9A8(EnBw* this);
void func_809CEA24(EnBw* this, PlayState* play);
void func_809CF72C(EnBw* this);
void func_809CF7AC(EnBw* this, PlayState* play);
void func_809CF8F0(EnBw* this);
void func_809CF984(EnBw* this, PlayState* play);
void func_809CFBA8(EnBw* this);
void func_809CFC4C(EnBw* this, PlayState* play);
void func_809CFF10(EnBw* this);
void func_809CFF98(EnBw* this, PlayState* play);
void func_809D00F4(EnBw* this);
void func_809D014C(EnBw* this, PlayState* play);
void func_809D01CC(EnBw* this);
void func_809D0268(EnBw* this, PlayState* play);
void func_809D03CC(EnBw* this);
void func_809D0424(EnBw* this, PlayState* play);

ActorProfile En_Bw_Profile = {
    0xc38, ACTORCAT_ENEMY, FLAGS, OBJECT_BW, sizeof(EnBw), EnBw_Init, EnBw_Destroy, EnBw_Update, EnBw_Draw,
};

static ColliderCylinderInit sCylinderInit1 = {
    {
        COL_MATERIAL_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_NONE,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0xFFCFFFFF, HIT_SPECIAL_EFFECT_FIRE, 0x08 },
        { 0x00000000, HIT_BACKLASH_NONE, 0x00 },
        ATELEM_ON | ATELEM_SFX_NORMAL,
        ACELEM_NONE,
        OCELEM_NONE,
    },
    { 30, 65, 0, { 0, 0, 0 } },
};

static ColliderCylinderInit sCylinderInit2 = {
    {
        COL_MATERIAL_HIT0,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER,
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
    { 30, 35, 0, { 0, 0, 0 } },
};

static DamageTable sDamageTable = { {
    DMG_ENTRY(0, 0x1), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(2, 0xF), DMG_ENTRY(0, 0x0), DMG_ENTRY(2, 0xF),
    DMG_ENTRY(2, 0xF), DMG_ENTRY(0, 0x1), DMG_ENTRY(0, 0x0), DMG_ENTRY(2, 0xF), DMG_ENTRY(4, 0xF), DMG_ENTRY(2, 0xF),
    DMG_ENTRY(4, 0xE), DMG_ENTRY(2, 0xF), DMG_ENTRY(2, 0xF), DMG_ENTRY(2, 0xF), DMG_ENTRY(2, 0xF), DMG_ENTRY(0, 0x6),
    DMG_ENTRY(3, 0xE), DMG_ENTRY(0, 0x6), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(1, 0xF), DMG_ENTRY(4, 0xF),
    DMG_ENTRY(2, 0xF), DMG_ENTRY(2, 0xF), DMG_ENTRY(8, 0xF), DMG_ENTRY(4, 0xF), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(4, 0xF), DMG_ENTRY(0, 0x0),
} };

static s32 sSlugGroup = 0;

void EnBw_SetupAction(EnBw* this, EnBwActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

void EnBw_Init(Actor* thisx, PlayState* play) {
    EnBw* this = (EnBw*)thisx;

    Actor_SetScale(&this->actor, 0.012999999f);
    this->actor.hintId = 0xff;
    this->actor.gravity = -2.0f;
    SkelAnime_Init(play, &this->skelAnime, &gTorchSlugSkel, &gTorchSlugEyestalkWaveAnim, this->jointTable,
                   this->morphTable, TORCH_SLUG_LIMB_MAX);
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 40.0f);
    this->actor.colChkInfo.damageTable = &sDamageTable;
    this->actor.colChkInfo.health = 6;
#if OOT_VERSION >= PAL_1_0
    this->actor.colChkInfo.mass = MASS_HEAVY;
#endif
    this->actor.focus.pos = this->actor.world.pos;
    func_809CE9A8(this);
    this->color1.a = this->color1.r = 255;
    this->color1.g = this->color1.b = 0;
    this->unk_248 = 0.6f;
    this->unk_221 = 3;
    Collider_InitCylinder(play, &this->collider1);
    Collider_SetCylinder(play, &this->collider1, &this->actor, &sCylinderInit1);
    Collider_SetCylinder(play, &this->collider2, &this->actor, &sCylinderInit2);
    this->unk_236 = this->actor.world.rot.y;
    this->actor.params = sSlugGroup;
    sSlugGroup = (sSlugGroup + 1) & 3;
}

void EnBw_Destroy(Actor* thisx, PlayState* play) {
    EnBw* this = (EnBw*)thisx;

    Collider_DestroyCylinder(play, &this->collider1);
    Collider_DestroyCylinder(play, &this->collider2);
}

void func_809CE884(EnBw* this, PlayState* play) {
    Math_SmoothStepToF(&this->actor.speed, 0.0f, 1.0f, 0.5f, 0.0f);
    this->unk_222 -= 250;
    this->actor.scale.x = 0.013f + Math_SinF(this->unk_222 * 0.001f) * 0.0069999998f;
    this->actor.scale.y = 0.013f - Math_SinF(this->unk_222 * 0.001f) * 0.0069999998f;
    this->actor.scale.z = 0.013f + Math_SinF(this->unk_222 * 0.001f) * 0.0069999998f;
    if (this->unk_222 == 0) {
        this->actor.world.rot.y = this->actor.shape.rot.y;
        func_809CE9A8(this);
    }
}

void func_809CE9A8(EnBw* this) {
    Animation_MorphToLoop(&this->skelAnime, &gTorchSlugEyestalkWaveAnim, -2.0f);
    this->unk_220 = 2;
    this->unk_222 = Rand_ZeroOne() * 200.0f + 200.0f;
    this->unk_232 = 0;
    this->actor.speed = 0.0f;
    EnBw_SetupAction(this, func_809CEA24);
}

void func_809CEA24(EnBw* this, PlayState* play) {
    CollisionPoly* sp74 = NULL;
    Vec3f sp68;
    u32 sp64 = 0;
    s16 sp62;
    s16 sp60;
    f32 sp5C;
    f32 sp58;
    Player* player = GET_PLAYER(play);
    Player* player2 = GET_PLAYER(play);

    SkelAnime_Update(&this->skelAnime);
    this->unk_244 = this->unk_250 + 0.1f;
    sp58 = Math_CosF(this->unk_240);
    this->unk_240 += this->unk_244;
    if (this->unk_24C < 0.8f) {
        this->unk_24C += 0.1f;
    }
    this->actor.scale.x = 0.013f - Math_SinF(this->unk_240) * (this->unk_24C * 0.004f);
    this->actor.scale.y = 0.013f - Math_SinF(this->unk_240) * (this->unk_24C * 0.004f);
    this->actor.scale.z = 0.013f + Math_SinF(this->unk_240) * (this->unk_24C * 0.004f);
    sp5C = Math_CosF(this->unk_240);
    if (this->unk_232 == 0) {
        if (ABS(sp58) < ABS(sp5C)) {
            this->unk_232++;
        }
    } else {
        if (ABS(sp58) > ABS(sp5C)) {
            Actor_PlaySfx(&this->actor, NA_SE_EN_BUBLEWALK_WALK);
            this->unk_232 = 0;
        }
    }
    sp5C *= this->unk_24C * (10.0f * this->unk_244);
    this->actor.speed = ABS(sp5C);
    if (this->unk_221 != 1) {
        sp58 = Math_SinF(this->unk_240);
        sp60 = ABS(sp58) * 85.0f;
        this->color1.g = sp60;
    }
    if ((((play->gameplayFrames % 4) == (u32)this->actor.params) && (this->actor.speed != 0.0f) &&
         (sp64 =
              BgCheck_AnyLineTest2(&play->colCtx, &this->actor.world.pos, &this->unk_264, &sp68, &sp74, 1, 0, 0, 1))) ||
        (this->unk_222 == 0)) {
        if (sp74 != NULL) {
            sp74 = SEGMENTED_TO_VIRTUAL(sp74);
            sp62 = RAD_TO_BINANG(Math_FAtan2F(sp74->normal.x, sp74->normal.z));
        } else {
            sp62 = this->actor.world.rot.y + 0x8000;
        }
        if ((this->unk_236 != sp62) || (sp64 == 0)) {
            if (BgCheck_AnyLineTest2(&play->colCtx, &this->unk_270, &this->unk_288, &sp68, &sp74, 1, 0, 0, 1)) {
                sp64 |= 2;
            }
            if (BgCheck_AnyLineTest2(&play->colCtx, &this->unk_270, &this->unk_27C, &sp68, &sp74, 1, 0, 0, 1)) {
                sp64 |= 4;
            }
            switch (sp64) {
                case 0:
                    this->unk_236 += this->unk_238;
                    FALLTHROUGH;
                case 1:
                    if (this->unk_221 == 3) {
                        if (play->gameplayFrames & 2) {
                            this->unk_238 = 0x4000;
                        } else {
                            this->unk_238 = -0x4000;
                        }
                    } else {
                        if ((s16)(this->actor.yawTowardsPlayer - this->actor.shape.rot.y) >= 0.0f) {
                            this->unk_238 = 0x4000;
                        } else {
                            this->unk_238 = -0x4000;
                        }
                        if (this->unk_221 == 1) {
                            this->unk_238 = -this->unk_238;
                        }
                    }
                    break;
                case 2:
                    this->unk_236 += this->unk_238;
                    FALLTHROUGH;
                case 3:
                    this->unk_238 = 0x4000;
                    break;
                case 4:
                    this->unk_236 += this->unk_238;
                    FALLTHROUGH;
                case 5:
                    this->unk_238 = -0x4000;
                    break;
                case 7:
                    this->unk_238 = 0;
                    break;
            }
            if (sp64 != 6) {
                this->unk_236 = sp62;
            }
            this->unk_222 = (Rand_ZeroOne() * 200.0f) + 200.0f;
        }
    } else if ((this->actor.speed != 0.0f) && (this->actor.bgCheckFlags & BGCHECKFLAG_WALL)) {
        if (this->unk_236 != this->actor.wallYaw) {
            sp64 = 1;
            this->unk_236 = this->actor.wallYaw;
            if (this->unk_221 == 3) {
                if (play->gameplayFrames & 0x20) {
                    this->unk_238 = 0x4000;
                } else {
                    this->unk_238 = -0x4000;
                }
                this->actor.bgCheckFlags &= ~BGCHECKFLAG_WALL;
                this->unk_222 = (Rand_ZeroOne() * 20.0f) + 160.0f;
            } else {
                if ((s16)(this->actor.yawTowardsPlayer - this->unk_236) >= 0) {
                    this->unk_238 = 0x4000;
                } else {
                    this->unk_238 = -0x4000;
                }
                if (this->unk_221 == 1) {
                    this->unk_238 = -this->unk_238;
                }
            }
        } else if (this->unk_221 == 0) {
            sp64 = BgCheck_AnyLineTest2(&play->colCtx, &this->actor.world.pos, &player->actor.world.pos, &sp68, &sp74,
                                        1, 0, 0, 1);
            if (sp64 != 0) {
                sp74 = SEGMENTED_TO_VIRTUAL(sp74);
                sp60 = RAD_TO_BINANG(Math_FAtan2F(sp74->normal.x, sp74->normal.z));
                if (this->unk_236 != sp60) {
                    if ((s16)(this->actor.yawTowardsPlayer - sp60) >= 0) {
                        this->unk_238 = 0x4000;
                    } else {
                        this->unk_238 = -0x4000;
                    }
                    this->unk_236 = sp60;
                }
            }
        }
    }
    this->unk_222--;
    if (this->unk_224 != 0) {
        this->unk_224--;
    }
    if ((this->unk_234 == 0) && !Actor_TestFloorInDirection(&this->actor, play, 50.0f, this->unk_236 + this->unk_238)) {
        if (this->unk_238 != 0x4000) {
            this->unk_238 = 0x4000;
        } else {
            this->unk_238 = -0x4000;
        }
    }
    switch (this->unk_221) {
        case 3:
            Math_SmoothStepToF(&this->unk_248, 0.6f, 1.0f, 0.05f, 0.0f);
            if ((this->unk_224 == 0) && (this->actor.xzDistToPlayer < 200.0f) &&
                (ABS(this->actor.yDistanceFromLink) < 50.0f) && Actor_IsFacingPlayer(&this->actor, 0x1C70)) {
                func_809CF72C(this);
            } else {
                Math_SmoothStepToS(&this->actor.world.rot.y, this->unk_236 + this->unk_238, 1,
                                   this->actor.speed * 1000.0f, 0);
            }
            break;
        case 0:
            Math_SmoothStepToF(&this->unk_248, 0.6f, 1.0f, 0.05f, 0.0f);
            if (sp64 == 0) {
                Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 1,
                                   this->actor.speed * 1000.0f, 0);
                if ((this->actor.xzDistToPlayer < 90.0f) && (this->actor.yDistanceFromLink < 50.0f) &&
                    Actor_IsFacingPlayer(&this->actor, 0x1554) &&
                    Actor_TestFloorInDirection(&this->actor, play, 71.24802f, this->actor.yawTowardsPlayer)) {
                    func_809CF8F0(this);
                }
            } else {
                Math_SmoothStepToS(&this->actor.world.rot.y, this->unk_236 + this->unk_238, 1,
                                   this->actor.speed * 1000.0f, 0);
            }
            if ((this->unk_224 == 0) || (ABS(this->actor.yDistanceFromLink) > 60.0f) ||
                (player2->stateFlags1 & (PLAYER_STATE1_13 | PLAYER_STATE1_14))) {
                this->unk_221 = 3;
                this->unk_224 = 150;
                this->unk_250 = 0.0f;
            }
            break;
        case 1:
            if (((sp64 == 0) && !(this->actor.bgCheckFlags & BGCHECKFLAG_WALL)) ||
                Actor_IsFacingPlayer(&this->actor, 0x1C70)) {
                if (Actor_IsFacingPlayer(&this->actor, 0x1C70)) {
                    this->unk_238 = -this->unk_238;
                }
                Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer - 0x8000, 1,
                                   this->actor.speed * 1000.0f, 0);
            } else {
                Math_SmoothStepToS(&this->actor.world.rot.y, this->unk_236 + this->unk_238, 1,
                                   this->actor.speed * 1000.0f, 0);
            }
            if (this->unk_224 <= 200) {
                sp60 = Math_SinS(this->unk_224 * (0x960 - this->unk_224)) * 55.0f;
                this->color1.r = 255 - ABS(sp60);
                sp60 = Math_SinS(this->unk_224 * (0x960 - this->unk_224)) * 115.0f;
                this->color1.g = ABS(sp60) + 85;
                sp60 = Math_SinS(0x960 - this->unk_224) * 255.0f;
                this->color1.b = ABS(sp60);
                if (this->unk_224 == 0) {
                    this->unk_221 = 3;
                    this->unk_250 = 0.0f;
                }
            }
            break;
    }
    this->actor.shape.rot.y = this->actor.world.rot.y;
}

void func_809CF72C(EnBw* this) {
    Animation_MorphToPlayOnce(&this->skelAnime, &gTorchSlugEyestalkRaiseAnim, -2.0f);
    this->unk_220 = 3;
    this->unk_221 = 0;
    this->unk_250 = 0.6f;
    this->unk_222 = 20;
    this->unk_224 = 0xBB8;
    this->actor.speed = 0.0f;
    Actor_PlaySfx(&this->actor, NA_SE_EN_BUBLEWALK_AIM);
    EnBw_SetupAction(this, func_809CF7AC);
}

void func_809CF7AC(EnBw* this, PlayState* play) {
    if (SkelAnime_Update(&this->skelAnime)) {
        if (this->unk_222 > 0) {
            this->unk_222--;
        } else {
            this->unk_222 = 100;
        }
    }
    if (this->unk_222 >= 20) {
        this->unk_224 -= 250;
    }
    this->actor.scale.x = 0.013f - Math_SinF(this->unk_224 * 0.001f) * 0.0034999999f;
    this->actor.scale.y = 0.013f + Math_SinF(this->unk_224 * 0.001f) * 0.0245f;
    this->actor.scale.z = 0.013f - Math_SinF(this->unk_224 * 0.001f) * 0.0034999999f;
    if (this->unk_224 == 0) {
        func_809CE9A8(this);
        this->unk_224 = 200;
    }
}

void func_809CF8F0(EnBw* this) {
    Animation_MorphToPlayOnce(&this->skelAnime, &gTorchSlugEyestalkFlailAnim, -1.0f);
    this->actor.speed = 7.0f;
    this->actor.world.rot.y = this->actor.shape.rot.y = this->actor.yawTowardsPlayer;
    this->unk_220 = 4;
    this->unk_222 = 1000;
    this->actor.velocity.y = 11.0f;
    Actor_PlaySfx(&this->actor, NA_SE_EN_STAL_JUMP);
    this->actor.flags |= ACTOR_FLAG_SFX_FOR_PLAYER_BODY_HIT;
    EnBw_SetupAction(this, func_809CF984);
}

void func_809CF984(EnBw* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 floorPolyType;

    Math_SmoothStepToF(&this->actor.speed, 0.0f, 1.0f, 0.5f, 0.0f);
    this->unk_222 += 250;
    this->actor.scale.x = 0.013f - Math_SinF(this->unk_222 * 0.001f) * 0.0034999999f;
    this->actor.scale.y = 0.013f + Math_SinF(this->unk_222 * 0.001f) * 0.0245f;
    this->actor.scale.z = 0.013f - Math_SinF(this->unk_222 * 0.001f) * 0.0034999999f;
    if (this->collider1.base.atFlags & AT_HIT) {
        this->collider1.base.atFlags &= ~AT_HIT;
        this->actor.speed = -6.0f;
        this->actor.world.rot.y = this->actor.yawTowardsPlayer;
        if ((&player->actor == this->collider1.base.at) && !(this->collider1.base.atFlags & AT_BOUNCED)) {
            Actor_PlaySfx(&player->actor, NA_SE_PL_BODY_HIT);
        }
    }
    SkelAnime_Update(&this->skelAnime);
    if (this->actor.bgCheckFlags & (BGCHECKFLAG_GROUND | BGCHECKFLAG_GROUND_TOUCH)) {
        floorPolyType = SurfaceType_GetFloorType(&play->colCtx, this->actor.floorPoly, this->actor.floorBgId);
        if ((floorPolyType == FLOOR_TYPE_2) || (floorPolyType == FLOOR_TYPE_3) || (floorPolyType == FLOOR_TYPE_9)) {
            Actor_Kill(&this->actor);
            return;
        }
        Actor_SpawnFloorDustRing(play, &this->actor, &this->actor.world.pos, 30.0f, 11, 4.0f, 0, 0, false);
        this->unk_222 = 3000;
        this->actor.flags &= ~ACTOR_FLAG_SFX_FOR_PLAYER_BODY_HIT;
        this->actor.speed = 0.0f;
        Actor_PlaySfx(&this->actor, NA_SE_EN_DODO_M_GND);
        EnBw_SetupAction(this, func_809CE884);
    }
}

void func_809CFBA8(EnBw* this) {
    Animation_MorphToLoop(&this->skelAnime, &gTorchSlugEyestalkFlailAnim, -1.0f);
    this->unk_220 = 5;
    this->unk_222 = 1000;
    this->unk_260 = 0.0f;
    this->actor.speed = 0.0f;
    this->actor.velocity.y = 11.0f;
    this->unk_25C = Rand_ZeroOne() * 0.25f + 1.0f;
    this->unk_224 = 0xBB8;
    Actor_PlaySfx(&this->actor, NA_SE_EN_BUBLEWALK_REVERSE);
    EnBw_SetupAction(this, func_809CFC4C);
}

void func_809CFC4C(EnBw* this, PlayState* play) {
    Math_SmoothStepToS(&this->actor.shape.rot.z, 0x7FFF, 1, 0xFA0, 0);
    Math_SmoothStepToF(&this->unk_248, 0.0f, 1.0f, 0.05f, 0.0f);
    SkelAnime_Update(&this->skelAnime);
    if (this->actor.bgCheckFlags & (BGCHECKFLAG_GROUND | BGCHECKFLAG_GROUND_TOUCH)) {
        if ((play->gameplayFrames % 0x80) == 0) {
            this->unk_25C = (Rand_ZeroOne() * 0.25f) + 0.7f;
        }
        this->unk_221 = 4;
        this->unk_258 += this->unk_25C;
        Math_SmoothStepToF(&this->unk_260, 0.075f, 1.0f, 0.005f, 0.0f);
        if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND_TOUCH) {
            Actor_SpawnFloorDustRing(play, &this->actor, &this->actor.world.pos, 30.0f, 11, 4.0f, 0, 0, false);
            Actor_PlaySfx(&this->actor, NA_SE_EN_DODO_M_GND);
        }
        if (this->unk_224 != 0) {
            this->unk_224 -= 250;
            this->actor.scale.x = 0.013f + Math_SinF(this->unk_224 * 0.001f) * 0.0069999998f;
            this->actor.scale.y = 0.013f - Math_SinF(this->unk_224 * 0.001f) * 0.0069999998f;
            this->actor.scale.z = 0.013f + Math_SinF(this->unk_224 * 0.001f) * 0.0069999998f;
        }
        this->unk_222--;
        if (this->unk_222 == 0) {
            func_809CFF10(this);
        }
    } else {
        this->color1.r -= 8;
        this->color1.g += 32;
        if (this->color1.r < 200) {
            this->color1.r = 200;
        }
        if (this->color1.g > 200) {
            this->color1.g = 200;
        }
        if (this->color1.b > 235) {
            this->color1.b = 255;
        } else {
            this->color1.b += 40;
        }
        if (this->actor.shape.yOffset < 1000.0f) {
            this->actor.shape.yOffset += 200.0f;
        }
    }
}

void func_809CFF10(EnBw* this) {
    Animation_MorphToLoop(&this->skelAnime, &gTorchSlugEyestalkFlailAnim, -1.0f);
    this->unk_220 = 6;
    this->unk_222 = 1000;
    this->unk_221 = 3;
    this->actor.speed = 0.0f;
    this->actor.velocity.y = 11.0f;
    Actor_PlaySfx(&this->actor, NA_SE_EN_BUBLEWALK_REVERSE);
    this->actor.bgCheckFlags &= ~BGCHECKFLAG_GROUND;
    EnBw_SetupAction(this, func_809CFF98);
}

void func_809CFF98(EnBw* this, PlayState* play) {
    Math_SmoothStepToS(&this->actor.shape.rot.z, 0, 1, 0xFA0, 0);
    Math_SmoothStepToF(&this->unk_248, 0.6f, 1.0f, 0.05f, 0.0f);
    SkelAnime_Update(&this->skelAnime);
    if (this->actor.bgCheckFlags & (BGCHECKFLAG_GROUND | BGCHECKFLAG_GROUND_TOUCH)) {
        Actor_SpawnFloorDustRing(play, &this->actor, &this->actor.world.pos, 30.0f, 11, 4.0f, 0, 0, false);
        this->unk_222 = 0xBB8;
        this->unk_250 = 0.0f;
        Actor_PlaySfx(&this->actor, NA_SE_EN_DODO_M_GND);
        EnBw_SetupAction(this, func_809CE884);
    }
    if (this->color1.r < 247) {
        this->color1.r += 8;
    } else {
        this->color1.r = 255;
    }
    if (this->color1.g < 32) {
        this->color1.g = 0;
    } else {
        this->color1.g -= 32;
    }
    if (this->color1.b < 40) {
        this->color1.b = 0;
    } else {
        this->color1.b -= 40;
    }
    if (this->actor.shape.yOffset > 0.0f) {
        this->actor.shape.yOffset -= 200.0f;
    }
}

void func_809D00F4(EnBw* this) {
    this->unk_220 = 0;
    this->unk_222 = 40;
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    this->actor.speed = 0.0f;
    Actor_PlaySfx(&this->actor, NA_SE_EN_BUBLEWALK_DEAD);
    EnBw_SetupAction(this, func_809D014C);
}

void func_809D014C(EnBw* this, PlayState* play) {
    if (this->unk_230) {
        this->actor.scale.x += 0.0002f;
        this->actor.scale.y -= 0.0002f;
        this->actor.scale.z += 0.0002f;
    }
    this->actor.shape.shadowAlpha = this->color1.a -= 6;
    this->unk_222--;
    if (this->unk_222 <= 0) {
        Actor_Kill(&this->actor);
    }
}

void func_809D01CC(EnBw* this) {
    this->unk_220 = 1;
    this->actor.speed = 0.0f;
    this->unk_25C = (Rand_ZeroOne() * 0.25f) + 1.0f;
    this->unk_260 = 0.0f;
    if (this->damageReaction == 0xE) {
        this->iceTimer = 0x50;
    }
    this->unk_222 = (this->actor.colorFilterParams & 0x4000) ? 25 : 80;
    EnBw_SetupAction(this, func_809D0268);
}

void func_809D0268(EnBw* this, PlayState* play) {
    this->color1.r -= 1;
    this->color1.g += 4;
    this->color1.b += 5;
    if (this->color1.r < 200) {
        this->color1.r = 200;
    }
    if (this->color1.g > 200) {
        this->color1.g = 200;
    }
    if (this->color1.b > 230) {
        this->color1.b = 230;
    }
    if (this->actor.colorFilterParams & 0x4000) {
        if ((play->gameplayFrames % 0x80) == 0) {
            this->unk_25C = 0.5f + Rand_ZeroOne() * 0.25f;
        }
        this->unk_258 += this->unk_25C;
        Math_SmoothStepToF(&this->unk_260, 0.075f, 1.0f, 0.005f, 0.0f);
    }
    this->unk_222--;
    if (this->unk_222 == 0) {
        func_809CE9A8(this);
        this->color1.r = this->color1.g = 200;
        this->color1.b = 255;
        this->unk_224 = 0x258;
        this->unk_221 = 1;
        this->unk_250 = 0.7f;
        this->unk_236++;
    }
}

void func_809D03CC(EnBw* this) {
    this->actor.speed = 0.0f;
    if (this->damageReaction == 0xE) {
        this->iceTimer = 32;
    }
    this->unk_23C = this->actor.colorFilterTimer;
    Actor_PlaySfx(&this->actor, NA_SE_EN_GOMA_JR_FREEZE);
    EnBw_SetupAction(this, func_809D0424);
}

void func_809D0424(EnBw* this, PlayState* play) {
    if (this->actor.colorFilterTimer == 0) {
        this->unk_23C = 0;
        if (this->actor.colChkInfo.health != 0) {
            if ((this->unk_220 != 5) && (this->unk_220 != 6)) {
                func_809CE9A8(this);
                this->color1.r = this->color1.g = 200;
                this->color1.b = 255;
                this->unk_224 = 0x258;
                this->unk_221 = 1;
                this->unk_250 = 0.7f;
                this->unk_236++;
            } else if (this->unk_220 != 5) {
                EnBw_SetupAction(this, func_809CFF98);
            } else {
                func_809CFF10(this);
            }
        } else {
            if (func_800355E4(play, &this->collider2.base)) {
                this->unk_230 = 0;
                this->actor.scale.y -= 0.009f;
                Actor_SpawnFloorDustRing(play, &this->actor, &this->actor.world.pos, 30.0f, 11, 4.0f, 0, 0, false);
            } else {
                this->unk_230 = 1;
            }
            Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos,
                                       COLLECTIBLE_DROP_RANDOM_PARAMS(COLLECTIBLE_DROP_TABLE_9, false));
            func_809D00F4(this);
        }
    }
}

void func_809D0584(EnBw* this, PlayState* play) {
    if ((this->actor.bgCheckFlags & BGCHECKFLAG_CEILING) && (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
        this->unk_230 = 0;
        this->actor.scale.y -= 0.009f;
        Actor_SpawnFloorDustRing(play, &this->actor, &this->actor.world.pos, 30.0f, 11, 4.0f, 0, 0, false);
        func_809D00F4(this);
    } else {
        if (this->collider2.base.acFlags & AC_HIT) {
            this->collider2.base.acFlags &= ~AC_HIT;
            if ((this->actor.colChkInfo.damageEffect == 0) || (this->unk_220 == 6)) {
                return;
            }
            this->damageReaction = this->actor.colChkInfo.damageEffect;
            Actor_SetDropFlag(&this->actor, &this->collider2.elem, false);
            if ((this->damageReaction == 1) || (this->damageReaction == 0xE)) {
                if (this->unk_23C == 0) {
                    Actor_ApplyDamage(&this->actor);
                    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_BLUE, 120, COLORFILTER_BUFFLAG_OPA, 80);
                    func_809D03CC(this);
                    this->unk_248 = 0.0f;
                }
                return;
            }
            if (this->unk_248 == 0.0f) {
                Actor_ApplyDamage(&this->actor);
            }
            if (((this->unk_221 == 1) || (this->unk_221 == 4)) && (this->actor.colChkInfo.health == 0)) {
                if (this->unk_220 != 0) {
                    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 8);
                    if (func_800355E4(play, &this->collider2.base)) {
                        this->unk_230 = 0;
                        this->actor.scale.y -= 0.009f;
                        Actor_SpawnFloorDustRing(play, &this->actor, &this->actor.world.pos, 30.0f, 11, 4.0f, 0, 0,
                                                 false);
                    } else {
                        this->unk_230 = 1;
                    }
                    Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos,
                                               COLLECTIBLE_DROP_RANDOM_PARAMS(COLLECTIBLE_DROP_TABLE_9, false));
                    func_809D00F4(this);
                }
            } else if ((this->unk_220 != 1) && (this->unk_220 != 6)) {
                Actor_PlaySfx(&this->actor, NA_SE_EN_BUBLEWALK_DAMAGE);
                Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 8);
                if (this->unk_220 != 5) {
                    func_809D01CC(this);
                }
                this->unk_248 = 0.0f;
            }
        }
        if ((play->actorCtx.unk2 != 0) && (this->actor.xzDistToPlayer <= 400.0f) &&
            (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
            if (this->unk_220 == 5) {
                this->unk_23C = 0;
                func_809CFF10(this);
            } else if (this->unk_220 != 0) {
                this->unk_23C = 0;
                func_809CFBA8(this);
            }
        }
    }
}

void EnBw_Update(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    EnBw* this = (EnBw*)thisx;
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    Vec3f accel = { 0.0f, 0.0f, 0.0f };
    Color_RGBA8 sp50 = { 255, 200, 0, 255 };
    Color_RGBA8 sp4C = { 255, 80, 0, 255 };
    Color_RGBA8 sp48 = { 0, 0, 0, 255 };
    Color_RGBA8 sp44 = { 0, 0, 0, 220 };

    func_809D0584(this, play);
    if (thisx->colChkInfo.damageEffect != 6) {
        this->actionFunc(this, play);
        if (this->unk_23C == 0) {
            this->unk_23A = (this->unk_23A + 4) & 0x7F;
        }
        if ((play->gameplayFrames & this->unk_221) == 0) {
            accel.y = -3.0f;
            velocity.x = Rand_CenteredFloat(this->unk_248 * 24.0f);
            velocity.y = this->unk_248 * 30.0f;
            velocity.z = Rand_CenteredFloat(this->unk_248 * 24.0f);
            accel.x = velocity.x * -0.075f;
            accel.z = velocity.z * -0.075f;
            func_8002836C(play, &thisx->world.pos, &velocity, &accel, &sp50, &sp4C, 0x3C, 0, 0x14);
        }
        if (this->unk_248 <= 0.4f) {
            this->collider1.elem.atDmgInfo.effect = HIT_SPECIAL_EFFECT_NONE;
            if (((play->gameplayFrames & 1) == 0) && (this->unk_220 < 5) && (this->unk_23C == 0)) {
                accel.y = -0.1f;
                velocity.x = Rand_CenteredFloat(4.0f);
                velocity.y = Rand_CenteredFloat(2.0f) + 6.0f;
                velocity.z = Rand_CenteredFloat(4.0f);
                accel.x = velocity.x * -0.1f;
                accel.z = velocity.z * -0.1f;
                Rand_CenteredFloat(4.0f);
                Rand_CenteredFloat(4.0f);
                sp48.a = this->color1.a;
                if (sp48.a >= 30) {
                    sp44.a = sp48.a - 30;
                } else {
                    sp44.a = 0;
                }
                func_8002836C(play, &thisx->world.pos, &velocity, &accel, &sp48, &sp44, 0xB4, 0x28,
                              20.0f - (this->unk_248 * 40.0f));
            }
        } else {
            this->collider1.elem.atDmgInfo.effect = HIT_SPECIAL_EFFECT_FIRE;
        }

        this->unk_234 = Actor_TestFloorInDirection(thisx, play, 50.0f, thisx->world.rot.y);
        if ((this->unk_220 == 4) || (this->unk_220 == 6) || (this->unk_220 == 5) || (this->unk_220 == 1) ||
            (this->unk_234 != 0)) {
            Actor_MoveXZGravity(thisx);
        }
        Actor_UpdateBgCheckInfo(play, thisx, 20.0f, 30.0f, 21.0f,
                                UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_2 |
                                    UPDBGCHECKINFO_FLAG_3 | UPDBGCHECKINFO_FLAG_4);
    }
    Collider_UpdateCylinder(thisx, &this->collider2);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider2.base);
    if ((this->unk_220 != 0) && ((thisx->colorFilterTimer == 0) || !(thisx->colorFilterParams & 0x4000))) {
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider2.base);
    }
    if ((this->unk_221 != 1) && (this->unk_220 < 5) && (this->unk_248 > 0.4f)) {
        Collider_UpdateCylinder(thisx, &this->collider1);
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider1.base);
    }
    thisx->focus.pos = thisx->world.pos;
    thisx->focus.pos.y += 5.0f;
}

s32 EnBw_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx, Gfx** gfx) {
    EnBw* this = (EnBw*)thisx;

    if (limbIndex == TORCH_SLUG_LIMB_BODY) {
        gSPSegment(
            (*gfx)++, 0x09,
            Gfx_TwoTexScroll(play->state.gfxCtx, G_TX_RENDERTILE, 0, 0, 0x20, 0x20, 1, 0, this->unk_23A, 0x20, 0x20));
        if ((this->unk_220 == 1) || (this->unk_220 == 5)) {
            Matrix_Push();
            Matrix_Scale(1.0f, 1.0f, 1.0f, MTXMODE_APPLY);
            Matrix_RotateX(this->unk_258 * 0.115f, MTXMODE_APPLY);
            Matrix_RotateY(this->unk_258 * 0.13f, MTXMODE_APPLY);
            Matrix_RotateZ(this->unk_258 * 0.1f, MTXMODE_APPLY);
            Matrix_Scale(1.0f - this->unk_260, 1.0f + this->unk_260, 1.0f - this->unk_260, MTXMODE_APPLY);
            Matrix_RotateZ(-(this->unk_258 * 0.1f), MTXMODE_APPLY);
            Matrix_RotateY(-(this->unk_258 * 0.13f), MTXMODE_APPLY);
            Matrix_RotateX(-(this->unk_258 * 0.115f), MTXMODE_APPLY);
            MATRIX_FINALIZE_AND_LOAD((*gfx)++, play->state.gfxCtx);
            gSPDisplayList((*gfx)++, *dList);
            Matrix_Pop();
            return 1;
        }
    }
    return 0;
}

static Vec3f sIceOffsets[] = {
    { 20.0f, 10.0f, 0.0f },  { -20.0f, 10.0f, 0.0f }, { 0.0f, 10.0f, -25.0f }, { 10.0f, 0.0f, 15.0f },
    { 10.0f, 0.0f, -15.0f }, { 0.0f, 10.0f, 25.0f },  { -10.0f, 0.0f, 15.0f }, { -10.0f, 0.0f, -15.0f },
};

void EnBw_Draw(Actor* thisx, PlayState* play2) {
    Vec3f spAC = { 0.0f, 0.0f, 0.0f };
    PlayState* play = play2;
    EnBw* this = (EnBw*)thisx;
    Vec3f icePos;
    s32 iceIndex;

    OPEN_DISPS(play->state.gfxCtx);

    if (this->color1.a == 0xFF) {
        Gfx_SetupDL_25Opa(play->state.gfxCtx);
        gDPSetEnvColor(POLY_OPA_DISP++, this->color1.r, this->color1.g, this->color1.b, this->color1.a);
        gSPSegment(POLY_OPA_DISP++, 0x08, ACTOR_SETUP_OPA_DL);
        POLY_OPA_DISP = SkelAnime_Draw(play, this->skelAnime.skeleton, this->skelAnime.jointTable,
                                       EnBw_OverrideLimbDraw, NULL, this, POLY_OPA_DISP);
    } else {
        Gfx_SetupDL_25Xlu(play2->state.gfxCtx);
        gDPPipeSync(POLY_XLU_DISP++);
        gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 0, 0, 0, this->color1.a);
        gDPSetEnvColor(POLY_XLU_DISP++, this->color1.r, this->color1.g, this->color1.b, this->color1.a);
        gSPSegment(POLY_XLU_DISP++, 0x08, gActorSetupXluDL);
        POLY_XLU_DISP = SkelAnime_Draw(play, this->skelAnime.skeleton, this->skelAnime.jointTable,
                                       EnBw_OverrideLimbDraw, NULL, this, POLY_XLU_DISP);
    }

    if (((play->gameplayFrames + 1) % 4) == (u32)thisx->params) {
        spAC.z = thisx->scale.z * 375000.0f;
        Matrix_MultVec3f(&spAC, &this->unk_264);
        spAC.z = thisx->scale.z * 150000.0f;
        Matrix_MultVec3f(&spAC, &this->unk_270);
        spAC.x = thisx->scale.x * 550000.0f;
        Matrix_MultVec3f(&spAC, &this->unk_288);
        spAC.x = -spAC.x;
        Matrix_MultVec3f(&spAC, &this->unk_27C);
    }

    Matrix_Translate(thisx->world.pos.x, thisx->world.pos.y + ((thisx->scale.y - 0.013f) * 1000.0f), thisx->world.pos.z,
                     MTXMODE_NEW);
    Gfx_SetupDL_25Xlu(play->state.gfxCtx);
    gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 0, 0);

    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(play->state.gfxCtx, G_TX_RENDERTILE, 0, 0, 0x20, 0x40, 1, 0,
                                (play->gameplayFrames * -20) % 0x200, 0x20, 0x80));
    gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 255, 255, 0, 255);
    Matrix_Scale(this->unk_248 * 0.01f, this->unk_248 * 0.01f, this->unk_248 * 0.01f, MTXMODE_APPLY);
    Matrix_ReplaceRotation(&play->billboardMtxF);
    MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
    gSPDisplayList(POLY_XLU_DISP++, gEffFire1DL);

    if (this->iceTimer != 0) {
        thisx->colorFilterTimer++;
        this->iceTimer--;
        if ((this->iceTimer & 3) == 0) {
            iceIndex = this->iceTimer >> 2;

            icePos.x = sIceOffsets[iceIndex].x + thisx->world.pos.x;
            icePos.y = sIceOffsets[iceIndex].y + thisx->world.pos.y;
            icePos.z = sIceOffsets[iceIndex].z + thisx->world.pos.z;
            EffectSsEnIce_SpawnFlyingVec3f(play, thisx, &icePos, 0x96, 0x96, 0x96, 0xFA, 0xEB, 0xF5, 0xFF, 1.3f);
        }
    }

    CLOSE_DISPS();
}

OVL_INFO_NATIVE_ENEMY(0x38, En_Bw_Profile);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_BW_INIT, EnBw_Init);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_BW_DESTROY, EnBw_Destroy);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_BW_UPDATE, EnBw_Update);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_BW_DRAW, EnBw_Draw);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_BW_PROFILE, En_Bw_Profile);
static const u32 kNativePortOrigin[] = { 0x4e505431, 0, 0x38, 0x25, 0x2491925a };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_BW_ORIGIN, kNativePortOrigin);
