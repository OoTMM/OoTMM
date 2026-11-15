#include "mm_native_compat.h"
#define gSkullFishSkel (*(FlexSkeletonHeader*)0x06004188)
#define gSkullFishAttackAnim (*(AnimationHeader*)0x06004274)
#define gSkullFishSwimAnim (*(AnimationHeader*)0x06004340)

#include "types/en_pr.h"
#include "types/en_prz.h"
#define TATL_HINT_ID_SKULLFISH 0xff
#undef ACTOR_EN_PRZ
#define ACTOR_EN_PRZ 0x181
#undef OBJECT_PR
#define OBJECT_PR 0x116b
#undef NA_SE_EN_BUBLEWALK_DEAD
#define NA_SE_EN_BUBLEWALK_DEAD 0x3978
#undef Collider_InitAndSetCylinder
#define Collider_InitAndSetCylinder ((s32 (*)(struct PlayState* play, ColliderCylinder* dest, struct Actor* actor, ColliderCylinderInit* src))(void*)FOREIGN_ENEMY_API_CYLINDER_INIT_SET)
#undef Actor_PlaySfx
#define Actor_PlaySfx ((void (*)(Actor* actor, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_ACTOR)
#undef Actor_SetColorFilter
#define Actor_SetColorFilter ((void (*)(Actor* actor, u16 colorFlag, u16 colorIntensityMax, u16 bufFlag, u16 duration))(void*)FOREIGN_ENEMY_API_COLOR_FILTER)
#undef Actor_Kill
extern u8 FOREIGN_ENEMY_API_KILL[];
#define Actor_Kill ((void (*)(Actor* actor))(void*)FOREIGN_ENEMY_API_KILL)
#undef Math_Vec3s_Copy
#define Math_Vec3s_Copy ((void (*)(Vec3s* dest, Vec3s* src))(void*)FOREIGN_ENEMY_API_VEC3S_COPY)
#undef CollisionCheck_SetAT
#define CollisionCheck_SetAT ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AT)
#undef CollisionCheck_SetAC
#define CollisionCheck_SetAC ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AC)
#undef Matrix_Translate
#define Matrix_Translate ((void (*)(f32 x, f32 y, f32 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_TRANSLATE)
#undef Scene_SetRenderModeXlu
#define Scene_SetRenderModeXlu ((void (*)(struct PlayState* play, s32 index, u32 flags))(void*)FOREIGN_ENEMY_API_RENDER_XLU)
#undef Actor_SetScale
#define Actor_SetScale ((void (*)(Actor* actor, f32 scale))0x80020f88u)
#undef SkelAnime_InitFlex
#define SkelAnime_InitFlex ((void (*)(struct PlayState* play, SkelAnime* skelAnime, FlexSkeletonHeader* skeletonHeaderSeg, AnimationHeader* animation, Vec3s* jointTable, Vec3s* morphTable, s32 limbCount))0x8008c788u)
#undef Math_Vec3f_Copy
#define Math_Vec3f_Copy ((void (*)(Vec3f* dest, Vec3f* src))0x80063cacu)
#undef Rand_CenteredFloat
#define Rand_CenteredFloat ((f32 (*)(f32 scale))0x80026d90u)
#undef Animation_GetLastFrame
#define Animation_GetLastFrame ((s16 (*)(void* animation))0x8008a194u)
#undef Animation_Change
#define Animation_Change ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 playSpeed, f32 startFrame, f32 endFrame, u8 mode, f32 morphFrames))0x8008d17cu)
#undef BgCheck_GetWaterSurfaceNoBgIdAlt
#define BgCheck_GetWaterSurfaceNoBgIdAlt ((s32 (*)(struct PlayState* play, CollisionContext* colCtx, f32 x, f32 z, f32* outWaterSurface, WaterBox** outWaterBox))0x80034908u)
#undef Math_ApproachZeroF
#define Math_ApproachZeroF ((void (*)(f32* pValue, f32 scale, f32 maxStep))0x800642f0u)
#undef Math_ApproachF
#define Math_ApproachF ((void (*)(f32* pValue, f32 target, f32 scale, f32 maxStep))0x80064280u)
#undef Math_SmoothStepToS
#define Math_SmoothStepToS ((s16 (*)(s16* pValue, s16 target, s16 scale, s16 step, s16 minStep))0x80064508u)
#undef Math_Vec3f_Pitch
#define Math_Vec3f_Pitch ((s16 (*)(Vec3f* a, Vec3f* b))0x80063f34u)
#undef Math_SinS
#define Math_SinS ((f32 (*)(s16 angle))0x800636c4u)
#undef Math_CosS
#define Math_CosS ((f32 (*)(s16 angle))0x80063684u)
#undef BgCheck_EntityLineTest1
#define BgCheck_EntityLineTest1 ((s32 (*)(CollisionContext* colCtx, Vec3f* posA, Vec3f* posB, Vec3f* posResult, CollisionPoly** outPoly, s32 checkWall, s32 checkFloor, s32 checkCeil, s32 checkOneFace, s32* bgId))0x800308b4u)
#undef Rand_S16Offset
#define Rand_S16Offset ((s16 (*)(s16 base, s16 range))0x80063bf0u)
#undef Rand_ZeroOne
#define Rand_ZeroOne ((f32 (*)(void))0x800cdcccu)
#undef Math_Vec3f_Yaw
#define Math_Vec3f_Yaw ((s16 (*)(Vec3f* a, Vec3f* b))0x80063f00u)
#undef Rand_ZeroFloat
#define Rand_ZeroFloat ((f32 (*)(f32 scale))0x80026d64u)
#undef EffectSsBubble_Spawn
#define EffectSsBubble_Spawn ((void (*)(struct PlayState* play, Vec3f* pos, f32 yPosOffset, f32 yPosRandScale, f32 xzPosRandScale, f32 scale))0x8001cedcu)
#undef SkelAnime_Update
#define SkelAnime_Update ((s32 (*)(SkelAnime* skelAnime))0x8008c9c0u)
#undef Actor_MoveWithGravity
#define Actor_MoveWithGravity ((void (*)(Actor* actor))0x8002121cu)
#undef Actor_UpdateBgCheckInfo
#define Actor_UpdateBgCheckInfo ((void (*)(struct PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight, u32 updBgCheckInfoFlags))0x80021e6cu)
#undef Actor_ApplyDamage
#define Actor_ApplyDamage ((u8 (*)(Actor* actor))0x80028390u)
#undef Collider_UpdateCylinder
#define Collider_UpdateCylinder ((void (*)(struct Actor* actor, ColliderCylinder* cyl))0x80050b00u)
#undef Matrix_MultVec3f
#define Matrix_MultVec3f ((void (*)(Vec3f* src, Vec3f* dest))0x800ab958u)
#undef Gfx_SetupDL25_Opa
#define Gfx_SetupDL25_Opa ((void (*)(GraphicsContext* gfxCtx))0x8007e298u)
#undef SkelAnime_DrawFlexOpa
#define SkelAnime_DrawFlexOpa ((void (*)(struct PlayState* play, void** skeleton, Vec3s* jointTable, s32 dListCount, GeneratedMmOverrideLimbDraw overrideLimbDraw, GeneratedMmPostLimbDraw postLimbDraw, struct Actor* actor))0x80089d8cu)
#undef SkelAnime_DrawFlex
#define SkelAnime_DrawFlex ((Gfx* (*)(struct PlayState* play, void** skeleton, Vec3s* jointTable, s32 dListCount, OverrideLimbDraw overrideLimbDraw, PostLimbDraw postLimbDraw, struct Actor* actor, Gfx* gfx))0x8008a88cu)

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED)

void EnPrz_Init(Actor* thisx, PlayState* play);
void EnPrz_Destroy(Actor* thisx, PlayState* play);
void EnPrz_Update(Actor* thisx, PlayState* play);
void EnPrz_Draw(Actor* thisx, PlayState* play);

void func_80A76388(EnPrz* this);
void func_80A763E8(EnPrz* this, PlayState* play);
void func_80A76604(EnPrz* this, PlayState* play);
void func_80A76634(EnPrz* this, PlayState* play);
void func_80A76748(EnPrz* this);
void func_80A767A8(EnPrz* this, PlayState* play);
void func_80A76B14(EnPrz* this, PlayState* play);

static DamageTable sDamageTable = { {
    DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(1, 0xF), DMG_ENTRY(1, 0xF), DMG_ENTRY(1, 0xF),
    DMG_ENTRY(0, 0x0), DMG_ENTRY(1, 0xF), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(1, 0xF),
    DMG_ENTRY(1, 0xF), DMG_ENTRY(2, 0xF), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(1, 0xF), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(0, 0x0), DMG_ENTRY(1, 0xF), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(1, 0xF),
    DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(0, 0x0), DMG_ENTRY(1, 0xF),
} };

f32 D_80A771E0[PLAYER_FORM_MAX] = {
    40.0f, 40.0f, 40.0f, 30.0f, 30.0f,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COL_MATERIAL_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK4,
        { 0x20000000, 0x00, 0x04 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        ATELEM_ON | ATELEM_SFX_NORMAL,
        ACELEM_ON,
        OCELEM_NONE,
    },
    { 10, 10, 0, { 0, 0, 0 } },
};

ActorProfile En_Prz_Profile = {
    0xd81, ACTORCAT_ENEMY, FLAGS, 0x116b, sizeof(EnPrz), EnPrz_Init, EnPrz_Destroy, EnPrz_Update, EnPrz_Draw,
};

void EnPrz_Init(Actor* thisx, PlayState* play) {
    EnPrz* this = (EnPrz*)thisx;

    this->unk_20C = 0.01f;
    this->unk_1E4 = this->actor.world.rot.y;

    Actor_SetScale(&this->actor, 0.01f);

    this->actor.attentionRangeType = ATTENTION_RANGE_3;
    this->unk_1E8 = 255;
    this->actor.hintId = TATL_HINT_ID_SKULLFISH;
    this->actor.colChkInfo.damageTable = &sDamageTable;
    this->actor.colChkInfo.health = 1;

    SkelAnime_InitFlex(play, &this->skelAnime, &gSkullFishSkel, &gSkullFishSwimAnim, this->jointTable, this->morphTable,
                       PR_2_LIMB_MAX);

    this->unk_1E6 = ENPRZ_GET(&this->actor);
    this->actor.shape.yOffset = 500.0f;
    this->actor.flags |= ACTOR_FLAG_LOCK_ON_DISABLED;
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;

    Collider_InitAndSetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    Math_Vec3f_Copy(&this->unk_1D8, &this->actor.world.pos);

    this->unk_220 = &this->actor;
    this->unk_1EC = 255;
    this->unk_202 = Rand_CenteredFloat((this->unk_1E6 * 100.0f) + 1000.0f);

    func_80A76388(this);
}

void EnPrz_Destroy(Actor* thisx, PlayState* play) {}

typedef enum EnPrzAnimation { ENPRZ_ANIM_0, ENPRZ_ANIM_1, ENPRZ_ANIM_MAX } EnPrzAnimation;

static AnimationHeader* sAnimations[ENPRZ_ANIM_MAX] = {
    &gSkullFishSwimAnim,
    &gSkullFishAttackAnim,
};

static u8 sAnimationModes[ENPRZ_ANIM_MAX] = {
    ANIMMODE_LOOP,
    ANIMMODE_LOOP,
};

void EnPrz_ChangeAnim(EnPrz* this, s32 animIndex) {
    this->animIndex = animIndex;
    Animation_Change(&this->skelAnime, sAnimations[animIndex], 1.0f, 0.0f,
                     Animation_GetLastFrame(sAnimations[animIndex]), sAnimationModes[animIndex], -2.0f);
}

s32 func_80A75FA4(EnPrz* this, PlayState* play) {
    WaterBox* waterBox;
    f32 temp_f0;

    if (BgCheck_GetWaterSurfaceNoBgIdAlt(play, &play->colCtx, this->actor.world.pos.x, this->actor.world.pos.z,
                                         &this->unk_210, &waterBox)) {
        temp_f0 = BREG(10) + (this->unk_210 - 10.0f);
        if (temp_f0 < this->actor.world.pos.y) {
            this->unk_1D8.y = temp_f0;
            return 1;
        }
    } else {
        temp_f0 = this->actor.floorHeight + 35.0f;
        if (this->actor.world.pos.y < temp_f0) {
            this->unk_1D8.y = temp_f0;
            return 2;
        }
    }

    return 0;
}

void func_80A76070(EnPrz* this, s16 arg1, PlayState* play) {
    s16 temp_s0 = arg1 - this->actor.world.rot.y;

    temp_s0 *= 0.01f;

    if (temp_s0 > 100) {
        temp_s0 = 100;
    }

    if (temp_s0 < -100) {
        temp_s0 = -100;
    }

    if (fabsf(this->actor.world.rot.y - (f32)arg1) < (Rand_CenteredFloat(20.0f) + 100.0f)) {
        Math_ApproachZeroF(&this->unk_218, 0.5f, 20.0f);
    } else {
        Math_ApproachF(&this->unk_218, temp_s0, 0.5f, 5.0f);
    }

    temp_s0 = fabsf(this->unk_218) * 0.1f;
    if (temp_s0 < 2) {
        temp_s0 = 2;
    }

    Math_SmoothStepToS(&this->actor.world.rot.y, arg1, temp_s0, this->unk_202 + 0x1388, 0);
    func_80A75FA4(this, play);
    Math_ApproachF(&this->actor.world.pos.y, this->unk_1D8.y, 0.5f, 1.0f);
    temp_s0 = Math_Vec3f_Pitch(&this->actor.world.pos, &this->unk_1D8) * 0.5f;

    if (fabsf(this->actor.world.pos.y - this->unk_208) > 4.0f) {
        Math_SmoothStepToS(&this->actor.world.rot.x, temp_s0, 1, BREG(48) + 0x1F4, 0);
    } else {
        Math_SmoothStepToS(&this->actor.world.rot.x, 0, 1, BREG(52) + 0xBB8, 0);
    }
}

s32 func_80A762C0(EnPrz* this, PlayState* play) {
    CollisionPoly* sp54;
    Vec3f sp48;
    s32 bgId;
    Vec3f sp38;

    Math_Vec3f_Copy(&sp38, &this->actor.world.pos);

    sp38.x += Math_SinS(this->actor.world.rot.y) * 40.0f;
    sp38.z += Math_CosS(this->actor.world.rot.y) * 40.0f;

    if (BgCheck_EntityLineTest1(&play->colCtx, &this->actor.world.pos, &sp38, &sp48, &sp54, true, false, false, true,
                                &bgId)) {
        return true;
    }
    return false;
}

void func_80A76388(EnPrz* this) {
    this->actor.speed = Rand_CenteredFloat(1.0f) + 4.0f;
    EnPrz_ChangeAnim(this, ENPRZ_ANIM_0);
    this->unk_1EA = 1;
    this->actionFunc = func_80A763E8;
}

void func_80A763E8(EnPrz* this, PlayState* play) {
    Actor* sp3C = this->actor.parent;
    Player* player = GET_PLAYER(play);
    s16 yaw;

    if (&this->actor == this->unk_220) {
        return;
    }

    if ((player->stateFlags1 & PLAYER_STATE1_8000000) && (this->unk_1F2 == 0)) {
        func_80A76748(this);
        return;
    }

    if (func_80A762C0(this, play)) {
        func_80A76604(this, play);
        return;
    }

    if ((this->unk_220 != NULL) && (this->unk_220->update == NULL)) {
        this->unk_220 = this->actor.parent;
    }

    if ((this->unk_1EE == 0) && (this->unk_1F2 == 0)) {
        s16 rot;

        this->unk_214 = Rand_CenteredFloat(40.0f);

        rot = BINANG_ROT180(this->unk_220->world.rot.y);
        this->unk_1D8.x += Math_SinS(rot) * this->unk_214;
        this->unk_1D8.y = Rand_CenteredFloat(40.0f) + (this->unk_220->world.pos.y + 40.0f);
        rot = BINANG_ROT180(this->unk_220->world.rot.y);
        this->unk_1D8.z += Math_CosS(rot) * this->unk_214;

        this->unk_1EE = Rand_S16Offset(20, 30);
    } else if (sqrtf(SQ(this->actor.world.pos.x - sp3C->world.pos.x) +
                     SQ(this->actor.world.pos.z - sp3C->world.pos.z)) > 100.0f) {
        Math_Vec3f_Copy(&this->unk_1D8, &sp3C->world.pos);
    }

    if (1) {
    }

    if ((Rand_ZeroOne() < 0.1f) && (this->unk_1C8 == 0)) {
        this->unk_1C8 = 1;
    }

    yaw = Math_Vec3f_Yaw(&this->actor.world.pos, &this->unk_1D8);
    func_80A76070(this, yaw, play);
}

void func_80A76604(EnPrz* this, PlayState* play) {
    this->unk_1EE = 0;
    this->unk_1E4 = BINANG_ADD(this->actor.world.rot.y, 0x4000);
    this->unk_1EA = 2;
    this->actionFunc = func_80A76634;
}

void func_80A76634(EnPrz* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    func_80A76070(this, this->unk_1E4, play);

    if (ABS_ALT(BINANG_SUB(this->actor.world.rot.y, this->unk_1E4)) < 0x100) {
        if (func_80A762C0(this, play) != 0) {
            this->unk_1E4 += 0x1500;
            this->unk_1E4 += TRUNCF_BINANG(Rand_ZeroFloat(5000.0f));
        } else if ((player->stateFlags1 & PLAYER_STATE1_8000000) && (player->actor.floorHeight < 30.0f)) {
            this->actionFunc = func_80A763E8;
        } else {
            this->unk_1EE = 10;
            this->unk_1EA = 1;
            this->actionFunc = func_80A763E8;
        }
    }
}

void func_80A76748(EnPrz* this) {
    this->actor.speed = Rand_CenteredFloat(1.0f) + 3.0f;
    this->unk_1EE = 0;
    this->unk_1EA = 3;
    this->skelAnime.playSpeed = 2.0f;
    this->actionFunc = func_80A767A8;
}

void func_80A767A8(EnPrz* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    EnPr* pr = (EnPr*)this->actor.parent;
    f32 distXZ;

    if (func_80A762C0(this, play)) {
        EnPrz_ChangeAnim(this, ENPRZ_ANIM_0);
        this->actor.speed = Rand_CenteredFloat(1.0f) + 4.0f;
        func_80A76604(this, play);
        return;
    }

    distXZ = sqrtf(SQ(player->actor.world.pos.x - this->actor.parent->home.pos.x) +
                   SQ(player->actor.world.pos.z - this->actor.parent->home.pos.z));

    if (!(player->stateFlags1 & PLAYER_STATE1_8000000) || (pr->unk_2C8 < distXZ)) {
        this->unk_1F2 = 100;
        this->skelAnime.playSpeed = 1.0f;
        func_80A76388(this);
        return;
    }

    if ((this->actor.xzDistToPlayer < 200.0f) && (fabsf(player->actor.world.pos.y - this->actor.world.pos.y) < 80.0f)) {
        if (this->animIndex != ENPRZ_ANIM_1) {
            EnPrz_ChangeAnim(this, ENPRZ_ANIM_1);
        }
        this->skelAnime.playSpeed = 1.0f;
    } else {
        if (this->animIndex != ENPRZ_ANIM_0) {
            EnPrz_ChangeAnim(this, ENPRZ_ANIM_0);
        }
        this->skelAnime.playSpeed = 2.0f;
    }

    if (this->unk_1EE == 0) {
        this->unk_1EE = Rand_S16Offset(2, 3);
        Math_Vec3f_Copy(&this->unk_1D8, &player->actor.world.pos);
    }

    if ((Rand_ZeroOne() < 0.1f) && (this->unk_1C8 == 0)) {
        this->unk_1C8 = 1;
    }

    this->unk_1D8.y =
        (player->actor.world.pos.y + D_80A771E0[GET_PLAYER_FORM]) + Rand_CenteredFloat((2.0f * this->unk_1E6) + 1.0f);
    func_80A76070(this, Math_Vec3f_Yaw(&this->actor.world.pos, &this->unk_1D8), play);
}

void func_80A76A1C(EnPrz* this) {
    this->unk_1E8 = 0;
    this->actor.flags |= ACTOR_FLAG_LOCK_ON_DISABLED;
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;

    Actor_PlaySfx(&this->actor, NA_SE_EN_BUBLEWALK_DEAD);

    this->unk_1EE = Rand_S16Offset(100, 30);
    this->unk_1FC = 0x4000;

    if (Rand_ZeroOne() < 0.5f) {
        this->unk_1FC = -0x4000;
    }

    this->unk_1FE = this->actor.world.rot.y;
    this->actor.speed = Rand_ZeroFloat(0.5f);
    this->actor.world.rot.y = Rand_CenteredFloat(0x8000);

    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 30);
    this->unk_1EE = 50;
    EnPrz_ChangeAnim(this, ENPRZ_ANIM_0);
    this->unk_1EA = 7;
    this->actionFunc = func_80A76B14;
}

void func_80A76B14(EnPrz* this, PlayState* play) {
    WaterBox* waterBox;
    s32 i;
    Vec3f sp6C;
    s32 phi_s0 = false;

    Math_SmoothStepToS(&this->actor.shape.rot.x, 0, 5, 0x2710, 0x3E8);
    Math_SmoothStepToS(&this->actor.shape.rot.z, this->unk_1FC, 5, 0x2710, 0x3E8);
    Math_SmoothStepToS(&this->actor.shape.rot.y, this->unk_1FE, 5, 0x2710, 0x3E8);

    if (BgCheck_GetWaterSurfaceNoBgIdAlt(play, &play->colCtx, this->actor.world.pos.x, this->actor.world.pos.z,
                                         &this->unk_210, &waterBox)) {
        if ((this->unk_210 - 15.0f) <= this->actor.world.pos.y) {
            phi_s0 = true;
        } else {
            Math_ApproachF(&this->actor.world.pos.y, this->unk_210 - 14.0f, 0.3f, 1.0f);
        }
    }

    if ((this->unk_1EE == 0) || phi_s0) {
        Math_SmoothStepToS(&this->unk_1EC, 0, 1, 15, 50);

        if (this->unk_1EC < 2) {
            for (i = 0; i < 10; i++) {
                Math_Vec3f_Copy(&sp6C, &this->actor.world.pos);

                sp6C.x += Rand_CenteredFloat(20.0f);
                sp6C.y += Rand_CenteredFloat(5.0f);
                sp6C.z += Rand_CenteredFloat(20.0f);

                EffectSsBubble_Spawn(play, &sp6C, 0.0f, 5.0f, 5.0f, Rand_ZeroFloat(0.03f) + 0.07f);
            }
            Actor_Kill(&this->actor);
        }
    }
}

void EnPrz_Update(Actor* thisx, PlayState* play) {
    EnPrz* this = (EnPrz*)thisx;
    s32 sp44 = false;
    Vec3f sp38;

    if (this->unk_1EA != 7) {
        SkelAnime_Update(&this->skelAnime);
    }

    this->actionFunc(this, play);

    if (this->unk_1EE != 0) {
        this->unk_1EE--;
    }

    if (this->unk_1F2 != 0) {
        this->unk_1F2--;
    }

    Actor_MoveWithGravity(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 0.0f, 10.0f, 10.0f,
                            UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_4 | UPDBGCHECKINFO_FLAG_8 |
                                UPDBGCHECKINFO_FLAG_10);

    if (this->unk_1EA != 7) {
        Math_Vec3s_Copy(&this->actor.shape.rot, &this->actor.world.rot);
    }

    if (this->unk_1EA != 7) {
        if (this->collider.base.acFlags & AC_HIT) {
            Actor_ApplyDamage(&this->actor);
            if (this->actor.colChkInfo.health <= 0) {
                sp44 = true;
            }
        }

        if (sp44 || (this->actor.parent == NULL) || (this->actor.parent->colChkInfo.health <= 0)) {
            this->collider.base.acFlags &= ~AC_HIT;
            if (this->unk_1EA != 7) {
                func_80A76A1C(this);
                return;
            }
        }

        Collider_UpdateCylinder(&this->actor, &this->collider);
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);

        if (this->unk_1C8 != 0) {
            Math_Vec3f_Copy(&sp38, &this->unk_1CC);
            this->unk_1C8 = 0;

            sp38.x += Rand_CenteredFloat(20.0f);
            sp38.y += Rand_CenteredFloat(5.0f);
            sp38.z += Rand_CenteredFloat(20.0f);

            EffectSsBubble_Spawn(play, &sp38, 0.0f, 5.0f, 5.0f, Rand_ZeroFloat(0.03f) + 0.07f);
        }
    }
}

s32 EnPrz_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx) {
    EnPrz* this = (EnPrz*)thisx;

    if (limbIndex == PR_2_LIMB_SKULL) {
        rot->y += TRUNCF_BINANG(this->unk_218) * -100;
    }
    return false;
}

void EnPrz_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx) {
    Vec3f sZeroVec = { 0.0f, 0.0f, 0.0f };
    EnPrz* this = (EnPrz*)thisx;

    if (limbIndex == PR_2_LIMB_SKULL) {
        Matrix_Translate(0.0f, 0.0f, 0.0f, MTXMODE_APPLY);
        Matrix_MultVec3f(&sZeroVec, &this->unk_1CC);
    }
}

void EnPrz_Draw(Actor* thisx, PlayState* play) {
    EnPrz* this = (EnPrz*)thisx;

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);

    if (this->unk_1EC == 255) {
        gDPPipeSync(POLY_OPA_DISP++);
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, this->unk_1E8, this->unk_1E8, this->unk_1E8, this->unk_1EC);
        gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, this->unk_1EC);

        Scene_SetRenderModeXlu(play, 0, 1);
        SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount,
                              EnPrz_OverrideLimbDraw, EnPrz_PostLimbDraw, &this->actor);
    } else {
        gDPPipeSync(POLY_XLU_DISP++);
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, this->unk_1EC);

        Scene_SetRenderModeXlu(play, 1, 2);
        POLY_XLU_DISP = SkelAnime_DrawFlex(play, this->skelAnime.skeleton, this->skelAnime.jointTable,
                                           this->skelAnime.dListCount, NULL, NULL, &this->actor, POLY_XLU_DISP);
    }

    CLOSE_DISPS();
}

OVL_INFO_NATIVE_ENEMY(0x181, En_Prz_Profile);
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_MM_EN_PRZ, En_Prz_Profile, EnPrz_Init, EnPrz_Destroy, EnPrz_Update, EnPrz_Draw);
static const u32 generatedNativeOrigin[] = { 0x4e505431u, 0x1u, 0x181u, 0x16bu, 0xad532661u };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_PRZ_ORIGIN, generatedNativeOrigin);
