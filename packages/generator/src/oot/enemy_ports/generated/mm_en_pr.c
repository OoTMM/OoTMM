#include "mm_native_compat.h"
#define object_pr_Anim_000268 (*(AnimationHeader*)0x06000268)
#define object_pr_Anim_000740 (*(AnimationHeader*)0x06000740)
#define object_pr_Anim_001E10 (*(AnimationHeader*)0x06001e10)
#define object_pr_Anim_0021E8 (*(AnimationHeader*)0x060021e8)
#define object_pr_Skel_0038B8 (*(FlexSkeletonHeader*)0x060038b8)

#include "types/en_prz.h"
#include "types/en_pr.h"
#define TATL_HINT_ID_DESBREKO 0xff
#undef ACTOR_EN_CLEAR_TAG
#define ACTOR_EN_CLEAR_TAG 0xa2
#undef ACTOR_EN_PR
#define ACTOR_EN_PR 0x14b
#undef ACTOR_EN_PRZ
#define ACTOR_EN_PRZ 0x181
#undef OBJECT_PR
#define OBJECT_PR 0x116b
#undef NA_SE_EN_BUBLEWALK_DAMAGE
#define NA_SE_EN_BUBLEWALK_DAMAGE 0x3977
#undef NA_SE_EN_BUBLEWALK_DEAD
#define NA_SE_EN_BUBLEWALK_DEAD 0x3978
#undef Collider_InitAndSetCylinder
#define Collider_InitAndSetCylinder ((s32 (*)(struct PlayState* play, ColliderCylinder* dest, struct Actor* actor, ColliderCylinderInit* src))(void*)FOREIGN_ENEMY_API_CYLINDER_INIT_SET)
#undef Actor_SpawnAsChild
#define Actor_SpawnAsChild ((Actor* (*)(ActorContext* actorCtx, Actor* parent, struct PlayState* play, s16 actorId, f32 posX, f32 posY, f32 posZ, s16 rotX, s16 rotY, s16 rotZ, s32 params))(void*)FOREIGN_ENEMY_API_SPAWN_CHILD)
#undef Collider_DestroyCylinder
#define Collider_DestroyCylinder ((s32 (*)(struct PlayState* play, ColliderCylinder* cyl))(void*)FOREIGN_ENEMY_API_CYLINDER_DESTROY)
#undef Actor_PlaySfx
#define Actor_PlaySfx ((void (*)(Actor* actor, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_ACTOR)
#undef Actor_Kill
extern u8 FOREIGN_ENEMY_API_KILL[];
#define Actor_Kill ((void (*)(Actor* actor))(void*)FOREIGN_ENEMY_API_KILL)
#undef Actor_Spawn
#define Actor_Spawn ((Actor* (*)(ActorContext* actorCtx, struct PlayState* play, s16 actorId, f32 posX, f32 posY, f32 posZ, s16 rotX, s16 rotY, s16 rotZ, s32 params))(void*)FOREIGN_ENEMY_API_SPAWN)
#undef Actor_SetColorFilter
#define Actor_SetColorFilter ((void (*)(Actor* actor, u16 colorFlag, u16 colorIntensityMax, u16 bufFlag, u16 duration))(void*)FOREIGN_ENEMY_API_COLOR_FILTER)
#undef Math_Vec3s_Copy
#define Math_Vec3s_Copy ((void (*)(Vec3s* dest, Vec3s* src))(void*)FOREIGN_ENEMY_API_VEC3S_COPY)
#undef CollisionCheck_SetAC
#define CollisionCheck_SetAC ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AC)
#undef CollisionCheck_SetAT
#define CollisionCheck_SetAT ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AT)
#undef Matrix_Translate
#define Matrix_Translate ((void (*)(f32 x, f32 y, f32 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_TRANSLATE)
#undef Matrix_MultZero
#define Matrix_MultZero ((void (*)(Vec3f* dest))(void*)FOREIGN_ENEMY_API_MATRIX_MULT_ZERO)
#undef Scene_SetRenderModeXlu
#define Scene_SetRenderModeXlu ((void (*)(struct PlayState* play, s32 index, u32 flags))(void*)FOREIGN_ENEMY_API_RENDER_XLU)
#undef Actor_DrawDamageEffects
#define Actor_DrawDamageEffects ((void (*)(struct PlayState* play, Actor* actor, Vec3f bodyPartsPos[], s16 bodyPartsCount, f32 effectScale, f32 frozenSteamScale, f32 effectAlpha, u8 type))(void*)FOREIGN_ENEMY_API_DAMAGE_EFFECTS)
#undef Actor_SetScale
#define Actor_SetScale ((void (*)(Actor* actor, f32 scale))0x80020f88u)
#undef SkelAnime_InitFlex
#define SkelAnime_InitFlex ((void (*)(struct PlayState* play, SkelAnime* skelAnime, FlexSkeletonHeader* skeletonHeaderSeg, AnimationHeader* animation, Vec3s* jointTable, Vec3s* morphTable, s32 limbCount))0x8008c788u)
#undef Animation_GetLastFrame
#define Animation_GetLastFrame ((s16 (*)(void* animation))0x8008a194u)
#undef Animation_Change
#define Animation_Change ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 playSpeed, f32 startFrame, f32 endFrame, u8 mode, f32 morphFrames))0x8008d17cu)
#undef BgCheck_EntityLineTest1
#define BgCheck_EntityLineTest1 ((s32 (*)(CollisionContext* colCtx, Vec3f* posA, Vec3f* posB, Vec3f* posResult, CollisionPoly** outPoly, s32 checkWall, s32 checkFloor, s32 checkCeil, s32 checkOneFace, s32* bgId))0x800308b4u)
#undef BgCheck_GetWaterSurfaceNoBgIdAlt
#define BgCheck_GetWaterSurfaceNoBgIdAlt ((s32 (*)(struct PlayState* play, CollisionContext* colCtx, f32 x, f32 z, f32* outWaterSurface, WaterBox** outWaterBox))0x80034908u)
#undef Math_SmoothStepToS
#define Math_SmoothStepToS ((s16 (*)(s16* pValue, s16 target, s16 scale, s16 step, s16 minStep))0x80064508u)
#undef Math_Vec3f_Yaw
#define Math_Vec3f_Yaw ((s16 (*)(Vec3f* a, Vec3f* b))0x80063f00u)
#undef Math_Vec3f_Copy
#define Math_Vec3f_Copy ((void (*)(Vec3f* dest, Vec3f* src))0x80063cacu)
#undef Rand_CenteredFloat
#define Rand_CenteredFloat ((f32 (*)(f32 scale))0x80026d90u)
#undef Enemy_StartFinishingBlow
#define Enemy_StartFinishingBlow ((void (*)(struct PlayState* play, Actor* actor))0x80025b4cu)
#undef Math_SinS
#define Math_SinS ((f32 (*)(s16 angle))0x800636c4u)
#undef Math_ApproachF
#define Math_ApproachF ((void (*)(f32* pValue, f32 target, f32 scale, f32 maxStep))0x80064280u)
#undef Actor_ApplyDamage
#define Actor_ApplyDamage ((u8 (*)(Actor* actor))0x80028390u)
#undef SkelAnime_Update
#define SkelAnime_Update ((s32 (*)(SkelAnime* skelAnime))0x8008c9c0u)
#undef Actor_SetFocus
#define Actor_SetFocus ((void (*)(Actor* actor, f32 height))0x80020f04u)
#undef Math_CosS
#define Math_CosS ((f32 (*)(s16 angle))0x80063684u)
#undef Rand_S16Offset
#define Rand_S16Offset ((s16 (*)(s16 base, s16 range))0x80063bf0u)
#undef Actor_MoveWithGravity
#define Actor_MoveWithGravity ((void (*)(Actor* actor))0x8002121cu)
#undef Math_Vec3f_Pitch
#define Math_Vec3f_Pitch ((s16 (*)(Vec3f* a, Vec3f* b))0x80063f34u)
#undef Actor_UpdateBgCheckInfo
#define Actor_UpdateBgCheckInfo ((void (*)(struct PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight, u32 updBgCheckInfoFlags))0x80021e6cu)
#undef Collider_UpdateCylinder
#define Collider_UpdateCylinder ((void (*)(struct Actor* actor, ColliderCylinder* cyl))0x80050b00u)
#undef Rand_ZeroFloat
#define Rand_ZeroFloat ((f32 (*)(f32 scale))0x80026d64u)
#undef EffectSsBubble_Spawn
#define EffectSsBubble_Spawn ((void (*)(struct PlayState* play, Vec3f* pos, f32 yPosOffset, f32 yPosRandScale, f32 xzPosRandScale, f32 scale))0x8001cedcu)
#undef Matrix_MultVec3f
#define Matrix_MultVec3f ((void (*)(Vec3f* src, Vec3f* dest))0x800ab958u)
#undef Gfx_SetupDL25_Opa
#define Gfx_SetupDL25_Opa ((void (*)(GraphicsContext* gfxCtx))0x8007e298u)
#undef Gfx_SetupDL25_Xlu
#define Gfx_SetupDL25_Xlu ((void (*)(GraphicsContext* gfxCtx))0x8007e2c0u)
#undef SkelAnime_DrawFlexOpa
#define SkelAnime_DrawFlexOpa ((void (*)(struct PlayState* play, void** skeleton, Vec3s* jointTable, s32 dListCount, GeneratedMmOverrideLimbDraw overrideLimbDraw, GeneratedMmPostLimbDraw postLimbDraw, struct Actor* actor))0x80089d8cu)
#undef SkelAnime_DrawFlex
#define SkelAnime_DrawFlex ((Gfx* (*)(struct PlayState* play, void** skeleton, Vec3s* jointTable, s32 dListCount, OverrideLimbDraw overrideLimbDraw, PostLimbDraw postLimbDraw, struct Actor* actor, Gfx* gfx))0x8008a88cu)

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED)

void EnPr_Init(Actor* thisx, PlayState* play2);
void EnPr_Destroy(Actor* thisx, PlayState* play);
void EnPr_Update(Actor* thisx, PlayState* play);
void EnPr_Draw(Actor* thisx, PlayState* play);

void func_80A326F0(EnPr* this);
void func_80A32740(EnPr* this, PlayState* play);
void func_80A32854(EnPr* this);
void func_80A3289C(EnPr* this, PlayState* play);
void func_80A3295C(EnPr* this);
void func_80A32984(EnPr* this, PlayState* play);
void func_80A32A40(EnPr* this, PlayState* play);
void func_80A32AF8(EnPr* this);
void func_80A32B20(EnPr* this, PlayState* play);
void func_80A32CDC(EnPr* this);
void func_80A32D28(EnPr* this, PlayState* play);
void func_80A32EA4(EnPr* this, PlayState* play);
void func_80A32F48(EnPr* this, PlayState* play);

static DamageTable sDamageTable = { {
    DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(1, 0xF), DMG_ENTRY(1, 0xF), DMG_ENTRY(1, 0xF),
    DMG_ENTRY(0, 0x0), DMG_ENTRY(1, 0xF), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(0, 0x0), DMG_ENTRY(2, 0x4), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(1, 0xF), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(0, 0x0), DMG_ENTRY(1, 0x5), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(1, 0xF), DMG_ENTRY(1, 0xF),
    DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(0, 0x0), DMG_ENTRY(1, 0xF),
} };

f32 D_80A338C0[PLAYER_FORM_MAX] = {
    30.0f, 30.0f, 30.0f, 15.0f, 15.0f,
};

ActorProfile En_Pr_Profile = {
    0xd4b, ACTORCAT_ENEMY, FLAGS, 0x116b, sizeof(EnPr), EnPr_Init, EnPr_Destroy, EnPr_Update, EnPr_Draw,
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
    { 18, 20, 5, { 0, 0, 0 } },
};

typedef enum EnPrAnimation {
    ENPR_ANIM_0,
    ENPR_ANIM_1,
    ENPR_ANIM_2,
    ENPR_ANIM_3,
    ENPR_ANIM_4,
    ENPR_ANIM_MAX
} EnPrAnimation;

static AnimationHeader* sAnimations[ENPR_ANIM_MAX] = {
    &object_pr_Anim_0021E8, &object_pr_Anim_001E10, &object_pr_Anim_0021E8,
    &object_pr_Anim_000740, &object_pr_Anim_000268,
};

static u8 sAnimationModes[ENPR_ANIM_MAX] = {
    ANIMMODE_LOOP, ANIMMODE_ONCE, ANIMMODE_LOOP, ANIMMODE_LOOP, ANIMMODE_ONCE,
};

void EnPr_Init(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    EnPr* this = (EnPr*)thisx;
    EnPrz* prz;
    s32 i;
    s32 temp_s4;

    this->actor.colChkInfo.health = 3;
    this->actor.colChkInfo.mass = 50;
    this->actor.hintId = TATL_HINT_ID_DESBREKO;

    Collider_InitAndSetCylinder(play, &this->collider, &this->actor, &sCylinderInit);

    this->unk_208 = 255;
    this->unk_2CC = 0.01f;

    Actor_SetScale(&this->actor, 0.01f);

    this->actor.attentionRangeType = ATTENTION_RANGE_3;
    this->unk_2B8 = this->actor.world.pos.y;
    this->actor.shape.yOffset = 1500.0f;
    this->actor.colChkInfo.damageTable = &sDamageTable;

    SkelAnime_InitFlex(play, &this->skelAnime, &object_pr_Skel_0038B8, &object_pr_Anim_0021E8, this->jointTable,
                       this->morphTable, OBJECT_PR_1_LIMB_MAX);
    this->unk_2C8 = this->actor.world.rot.z * 20.0f;

    if (this->unk_2C8 < 80.0f) {
        this->unk_2C8 = 80.0f;
    }
    this->actor.world.rot.z = 0;

    temp_s4 = ENPR_GET_FF00(&this->actor);
    if (temp_s4 > ENPR_FF00_MAX) {
        temp_s4 = ENPR_FF00_MAX;
    } else if (temp_s4 < ENPR_FF00_MIN) {
        temp_s4 = ENPR_FF00_MIN;
    }

    this->unk_2F8 = &this->actor;

    for (i = 0; i < temp_s4; i++) {
        prz = (EnPrz*)Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, ACTOR_EN_PRZ, this->actor.world.pos.x,
                                         this->actor.world.pos.y, this->actor.world.pos.z, 0, this->actor.world.rot.y,
                                         0, i + 1);
        if (prz != NULL) {
            prz->unk_220 = this->unk_2F8;
            this->unk_2F8 = &prz->actor;
        }
    }

    this->unk_2D0 = 255;
    this->unk_20E = 30;
    this->unk_216 = this->actor.world.rot.y;
    this->unk_214 = this->actor.world.rot.y;
    this->unk_2B4 = this->actor.world.pos.y;

    func_80A326F0(this);
}

void EnPr_Destroy(Actor* thisx, PlayState* play) {
    EnPr* this = (EnPr*)thisx;

    Collider_DestroyCylinder(play, &this->collider);
}

void EnPr_ChangeAnim(EnPr* this, s32 animIndex) {
    f32 playSpeed;

    this->animIndex = animIndex;
    playSpeed = 1.0f;
    this->animEndFrame = Animation_GetLastFrame(sAnimations[animIndex]);

    if (this->animIndex == ENPR_ANIM_2) {
        playSpeed = 2.0f;
    }

    Animation_Change(&this->skelAnime, sAnimations[this->animIndex], playSpeed, 0.0f, this->animEndFrame,
                     sAnimationModes[this->animIndex], -2.0f);
}

s32 func_80A324E0(EnPr* this, PlayState* play) {
    CollisionPoly* sp54;
    Vec3f sp48;
    s32 bgId;
    WaterBox* waterBox;

    if (BgCheck_EntityLineTest1(&play->colCtx, &this->actor.world.pos, &this->unk_2E0, &sp48, &sp54, 1, 0, 0, 1,
                                &bgId)) {
        return 1;
    }

    if (BgCheck_GetWaterSurfaceNoBgIdAlt(play, &play->colCtx, this->actor.world.pos.x, this->actor.world.pos.z,
                                         &this->unk_2B4, &waterBox)) {
        if ((this->unk_2B4 - 30.0f) < this->actor.world.pos.y) {
            this->unk_2B8 = this->unk_2B4 - 30.0f;
            return 2;
        }
    }

    if (this->actor.world.pos.y < (this->actor.floorHeight + 20.0f)) {
        this->unk_2B8 = this->actor.floorHeight + 20.0f;
        return 2;
    }
    return 0;
}

s32 func_80A325E4(EnPr* this) {
    s16 temp = this->unk_214 * 0.2f;
    s32 abs = ABS_ALT(temp) & 0xFFFF;

    Math_SmoothStepToS(&this->actor.world.rot.y, this->unk_22C, 1, abs + 1, 0);
    this->unk_216 = this->actor.world.rot.y - this->unk_22C;

    if (this->unk_216 > 10000) {
        this->unk_216 = 10000;
    } else if (this->unk_216 < -10000) {
        this->unk_216 = -10000;
    }

    if (ABS_ALT(BINANG_SUB(this->actor.world.rot.y, this->unk_22C)) > 0x100) {
        return true;
    }
    this->unk_216 = 0;
    return false;
}

void func_80A326F0(EnPr* this) {
    if (this->animIndex != ENPR_ANIM_0) {
        EnPr_ChangeAnim(this, ENPR_ANIM_0);
    }
    this->unk_206 = 0;
    this->actor.speed = 1.0f;
    this->actionFunc = func_80A32740;
}

void func_80A32740(EnPr* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    switch (func_80A324E0(this, play)) {
        case 1:
            func_80A3295C(this);
            return;

        case 2:
            this->unk_206 = 3;
            this->actionFunc = func_80A32A40;
            break;
    }

    if (this->unk_2C8 < sqrtf(SQ(this->actor.world.pos.x - this->actor.home.pos.x) +
                              SQ(this->actor.world.pos.z - this->actor.home.pos.z))) {
        func_80A32854(this);
    } else if ((this->unk_20E == 0) && (player->stateFlags1 & PLAYER_STATE1_8000000)) {
        if (sqrtf(SQ(player->actor.world.pos.x - this->actor.home.pos.x) +
                  SQ(player->actor.world.pos.z - this->actor.home.pos.z)) < this->unk_2C8) {
            func_80A32AF8(this);
        }
    }
}

void func_80A32854(EnPr* this) {
    if (this->animIndex != ENPR_ANIM_0) {
        EnPr_ChangeAnim(this, ENPR_ANIM_0);
    }
    this->unk_206 = 1;
    this->actionFunc = func_80A3289C;
}

void func_80A3289C(EnPr* this, PlayState* play) {
    this->unk_22C = Math_Vec3f_Yaw(&this->actor.world.pos, &this->actor.home.pos);
    func_80A325E4(this);

    if (sqrtf(SQ(this->actor.world.pos.x - this->actor.home.pos.x) +
              SQ(this->actor.world.pos.z - this->actor.home.pos.z)) < (((BREG(53) * 0.1f) + 0.5f) * this->unk_2C8)) {
        this->unk_206 = 3;
        this->actionFunc = func_80A32A40;
    }
}

void func_80A3295C(EnPr* this) {
    this->unk_22C = BINANG_ADD(this->actor.world.rot.y, 0x4000);
    this->unk_206 = 2;
    this->actionFunc = func_80A32984;
}

void func_80A32984(EnPr* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if ((player->stateFlags1 & PLAYER_STATE1_8000000) && (this->unk_20E == 0)) {
        this->unk_22C = this->actor.world.rot.y;
        func_80A32AF8(this);
    } else if (!func_80A325E4(this)) {
        if (func_80A324E0(this, play)) {
            this->unk_22C += 0x1000;
        } else {
            if (this->animIndex != ENPR_ANIM_0) {
                EnPr_ChangeAnim(this, ENPR_ANIM_0);
            }
            this->unk_206 = 3;
            this->actionFunc = func_80A32A40;
        }
    }
}

void func_80A32A40(EnPr* this, PlayState* play) {
    Vec3f sp34;
    WaterBox* waterBox;

    Math_Vec3f_Copy(&sp34, &this->actor.world.pos);
    sp34.y = Rand_CenteredFloat(50.0f) + this->actor.home.pos.y;

    if (BgCheck_GetWaterSurfaceNoBgIdAlt(play, &play->colCtx, this->actor.world.pos.x, this->actor.world.pos.z,
                                         &this->unk_2B4, &waterBox)) {
        if (sp34.y < (this->unk_2B4 - 30.0f)) {
            this->unk_2B8 = sp34.y;
        } else {
            this->unk_2B8 = this->unk_2B4 - 30.0f;
        }
    }

    this->unk_206 = 0;
    this->actionFunc = func_80A32740;
}

void func_80A32AF8(EnPr* this) {
    this->unk_206 = 4;
    this->skelAnime.playSpeed = 2.0f;
    this->actionFunc = func_80A32B20;
}

void func_80A32B20(EnPr* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    this->actor.speed = BREG(57) + 3.0f;
    Math_SmoothStepToS(&this->unk_22C, this->actor.yawTowardsPlayer, 1 + BREG(49), 0x3E8 + BREG(50), BREG(51));
    this->unk_2B8 = D_80A338C0[GET_PLAYER_FORM] + player->actor.world.pos.y;
    func_80A324E0(this, play);

    if (!(player->stateFlags1 & PLAYER_STATE1_8000000)) {
        this->skelAnime.playSpeed = 1.0f;
        this->actor.speed = 1.0f;
        func_80A32854(this);
    } else if (this->unk_2C8 < sqrtf(SQ(player->actor.world.pos.x - this->actor.home.pos.x) +
                                     SQ(player->actor.world.pos.z - this->actor.home.pos.z))) {
        this->skelAnime.playSpeed = 1.0f;
        this->actor.speed = 1.0f;
        func_80A32854(this);
    } else if (!func_80A325E4(this) && (this->actor.xzDistToPlayer < 200.0f) &&
               (fabsf(player->actor.world.pos.y - this->actor.world.pos.y) < 80.0f)) {
        this->skelAnime.playSpeed = 1.0f;
        func_80A32CDC(this);
    }
}

void func_80A32CDC(EnPr* this) {
    EnPr_ChangeAnim(this, ENPR_ANIM_3);
    this->unk_206 = 5;
    this->unk_20A = 400;
    this->unk_2C0 = 0.0f;
    this->actionFunc = func_80A32D28;
}

void func_80A32D28(EnPr* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (!(player->stateFlags1 & PLAYER_STATE1_8000000)) {
        this->skelAnime.playSpeed = 1.0f;
        this->actor.speed = 1.0f;
        func_80A32854(this);
    } else {
        Math_SmoothStepToS(&this->unk_22C, this->actor.yawTowardsPlayer, 1 + BREG(49), 0x3E8 + BREG(50), BREG(51));
        func_80A325E4(this);
        this->unk_2B8 = D_80A338C0[GET_PLAYER_FORM] + player->actor.world.pos.y;
        func_80A324E0(this, play);
        if (this->unk_2C8 < sqrtf(SQ(player->actor.world.pos.x - this->actor.home.pos.x) +
                                  SQ(player->actor.world.pos.z - this->actor.home.pos.z))) {
            this->skelAnime.playSpeed = 1.0f;
            this->actor.speed = 1.0f;
            func_80A32854(this);
        }
    }
}

void func_80A32E60(EnPr* this) {
    EnPr_ChangeAnim(this, ENPR_ANIM_4);
    this->unk_206 = 6;
    this->actor.speed = 0.0f;
    this->actionFunc = func_80A32EA4;
}

void func_80A32EA4(EnPr* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;

    if (curFrame >= this->animEndFrame) {
        if (this->actor.colChkInfo.health <= 0) {
            this->unk_206 = 7;
            this->unk_20A = 50;
            this->actor.flags |= ACTOR_FLAG_LOCK_ON_DISABLED;
            this->unk_2C4 = 0.0f;
            Enemy_StartFinishingBlow(play, &this->actor);
            Actor_PlaySfx(&this->actor, NA_SE_EN_BUBLEWALK_DEAD);
            this->unk_216 = 0;
            this->actionFunc = func_80A32F48;
        } else {
            func_80A326F0(this);
        }
    }
}

void func_80A32F48(EnPr* this, PlayState* play) {
    WaterBox* waterBox;

    if (this->unk_208 > 0) {
        this->unk_208 -= 2;
    } else {
        this->unk_208 = 0;
    }

    if (BgCheck_GetWaterSurfaceNoBgIdAlt(play, &play->colCtx, this->actor.world.pos.x, this->actor.world.pos.z,
                                         &this->unk_2B4, &waterBox)) {
        if ((this->unk_2B4 - 100.0f) < this->actor.world.pos.y) {
            this->unk_212 += 0xBB8;
            this->unk_2C4 = 2.0f * Math_SinS(this->unk_212);
        }
        Math_ApproachF(&this->actor.world.pos.y, (this->unk_2B4 - 16.0f) + this->unk_2C4, 0.5f, 2.0f);
        Math_SmoothStepToS(&this->actor.world.rot.z, 0x7700, 0x12C, 0x3E8, 0x3E8);
    }

    if ((this->unk_20A == 0) && (this->unk_208 == 0)) {
        this->unk_2D2 = 1;
    }

    if (this->unk_2D2 != 0) {
        Math_SmoothStepToS(&this->unk_2D0, 0, 1, 15, 50);
        if (this->unk_2D0 < 2) {
            Actor_Kill(&this->actor);
        }
    }
}

void func_80A33098(EnPr* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if ((this->unk_206 != 7) && (this->unk_206 != 6)) {
        if (this->collider.base.acFlags & AC_HIT) {
            this->collider.base.acFlags &= ~AC_HIT;
            if (this->actor.colChkInfo.damageEffect == 4) {
                this->drawDmgEffAlpha = 40;
                this->drawDmgEffType = ACTOR_DRAW_DMGEFF_LIGHT_ORBS;
                Actor_Spawn(&play->actorCtx, play, ACTOR_EN_CLEAR_TAG, this->actor.focus.pos.x, this->actor.focus.pos.y,
                            this->actor.focus.pos.z, 0, 0, 0, CLEAR_TAG_PARAMS(CLEAR_TAG_LARGE_LIGHT_RAYS));
            }

            if ((player->stateFlags1 & PLAYER_STATE1_8000000) && (this->actor.colChkInfo.damageEffect == 5)) {
                this->drawDmgEffAlpha = 40;
                this->drawDmgEffType = ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_MEDIUM;
            }

            if ((this->unk_206 != 6) && (this->unk_206 != 7)) {
                Actor_ApplyDamage(&this->actor);
                Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 8);
                Actor_PlaySfx(&this->actor, NA_SE_EN_BUBLEWALK_DAMAGE);
                func_80A32E60(this);
            }
        }
    }
}

void EnPr_Update(Actor* thisx, PlayState* play) {
    EnPr* this = (EnPr*)thisx;
    s32 i;

    SkelAnime_Update(&this->skelAnime);

    if (this->unk_20A != 0) {
        this->unk_20A--;
    }

    if (this->unk_20E != 0) {
        this->unk_20E--;
    }

    if (this->unk_20C != 0) {
        this->unk_20C--;
    }

    if (this->unk_210 != 0) {
        this->unk_210--;
    }

    this->actionFunc(this, play);

    Actor_SetFocus(&this->actor, 20.0f);
    func_80A33098(this, play);
    Math_Vec3f_Copy(&this->unk_2E0, &this->actor.world.pos);

    this->unk_2E0.x += (Math_SinS(this->actor.world.rot.y) * 70.0f);
    this->unk_2E0.y = this->unk_2D4.y - 10.0f;
    this->unk_2E0.z += Math_CosS(this->actor.world.rot.y) * 70.0f;

    Math_SmoothStepToS(&this->unk_214, this->unk_216, 1, 0x7D0, 0);

    if (this->unk_210 == 0) {
        this->unk_210 = Rand_S16Offset(10, 30);
        this->unk_204 = 1;
    }

    Actor_MoveWithGravity(&this->actor);

    if (this->unk_206 != 7) {
        Vec3f sp50;
        s16 pitch;

        Math_ApproachF(&this->actor.world.pos.y, this->unk_2B8, 0.3f, BREG(11) + 1.0f);
        Math_Vec3f_Copy(&sp50, &this->actor.world.pos);
        sp50.y = this->unk_2B8;
        pitch = Math_Vec3f_Pitch(&this->actor.world.pos, &sp50) * 0.3f;

        if (fabsf(this->actor.world.pos.y - this->unk_2B8) > 8.0f) {
            Math_SmoothStepToS(&this->actor.world.rot.x, pitch, 1, 0x1F4 + BREG(48), 0);
        } else {
            Math_SmoothStepToS(&this->actor.world.rot.x, 0, 1, 0x1F4 + BREG(52), 0);
        }
    }

    Actor_UpdateBgCheckInfo(play, &this->actor, 0.0f, 30.0f, 20.0f,
                            UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_4 | UPDBGCHECKINFO_FLAG_8 |
                                UPDBGCHECKINFO_FLAG_10);
    Math_Vec3s_Copy(&this->actor.shape.rot, &this->actor.world.rot);

    if (this->unk_206 != 7) {
        Collider_UpdateCylinder(&this->actor, &this->collider);
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);

        if (this->unk_204 != 0) {
            Vec3f sp40;

            Math_Vec3f_Copy(&sp40, &this->unk_2D4);
            this->unk_204 = 0;
            sp40.x += Rand_CenteredFloat(20.0f);
            sp40.y += Rand_CenteredFloat(5.0f);
            sp40.z += Rand_CenteredFloat(20.0f);

            for (i = 0; i < (s32)Rand_ZeroFloat(5.0f) + 5; i++) {
                EffectSsBubble_Spawn(play, &sp40, 0.0f, 5.0f, 5.0f, Rand_ZeroFloat(0.03f) + 0.07f);
            }
        }
    }
}

s32 EnPr_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx) {
    EnPr* this = (EnPr*)thisx;

    if (limbIndex == OBJECT_PR_1_LIMB_02) {
        rot->y += this->unk_214;
    }
    return false;
}

void EnPr_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx) {
    Vec3f sp24 = { 0.0f, 0.0f, 0.0f };
    EnPr* this = (EnPr*)thisx;

    if (limbIndex == OBJECT_PR_1_LIMB_02) {
        Matrix_Translate(0.0f, 0.0f, 0.0f, MTXMODE_APPLY);
        Matrix_MultVec3f(&sp24, &this->unk_2D4);
    }

    if ((limbIndex == OBJECT_PR_1_LIMB_NONE) || (limbIndex == OBJECT_PR_1_LIMB_01) ||
        (limbIndex == OBJECT_PR_1_LIMB_02) || (limbIndex == OBJECT_PR_1_LIMB_03) ||
        (limbIndex == OBJECT_PR_1_LIMB_04) || (limbIndex == OBJECT_PR_1_LIMB_05) ||
        (limbIndex == OBJECT_PR_1_LIMB_06) || (limbIndex == OBJECT_PR_1_LIMB_07) ||
        (limbIndex == OBJECT_PR_1_LIMB_08) || (limbIndex == OBJECT_PR_1_LIMB_09)) {
        Matrix_MultZero(&this->bodyPartsPos[this->bodyPartsCount]);
        this->bodyPartsCount++;
        if (this->bodyPartsCount >= ENPR_BODYPART_MAX) {
            this->bodyPartsCount = 0;
        }
    }
}

void EnPr_Draw(Actor* thisx, PlayState* play) {
    EnPr* this = (EnPr*)thisx;

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    Gfx_SetupDL25_Xlu(play->state.gfxCtx);

    if (this->unk_2D2 == 0) {
        gDPPipeSync(POLY_OPA_DISP++);
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, this->unk_208, this->unk_208, this->unk_208, this->unk_2D0);
        gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, this->unk_2D0);

        Scene_SetRenderModeXlu(play, 0, 1);
        SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount,
                              EnPr_OverrideLimbDraw, EnPr_PostLimbDraw, &this->actor);
    } else {
        gDPPipeSync(POLY_XLU_DISP++);
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, this->unk_2D0);

        Scene_SetRenderModeXlu(play, 1, 2);
        POLY_XLU_DISP = SkelAnime_DrawFlex(play, this->skelAnime.skeleton, this->skelAnime.jointTable,
                                           this->skelAnime.dListCount, NULL, NULL, &this->actor, POLY_XLU_DISP);
    }

    if (this->drawDmgEffAlpha != 0) {
        f32 drawDmgEffAlpha = this->drawDmgEffAlpha * 0.05f;

        this->unk_238 = 0.8f;
        this->unk_234 = 0.8f;
        Actor_DrawDamageEffects(play, &this->actor, this->bodyPartsPos, ENPR_BODYPART_MAX, 0.8f, 0.8f, drawDmgEffAlpha,
                                this->drawDmgEffType);
    }

    CLOSE_DISPS();
}

OVL_INFO_NATIVE_ENEMY(0x14b, En_Pr_Profile);
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_MM_EN_PR, En_Pr_Profile, EnPr_Init, EnPr_Destroy, EnPr_Update, EnPr_Draw);
static const u32 generatedNativeOrigin[] = { 0x4e505431u, 0x1u, 0x14bu, 0x16bu, 0x770f3d95u };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_PR_ORIGIN, generatedNativeOrigin);
