#include "oot_variant_compat.h"
#include "types/en_st.h"
#undef ACTOR_EN_ST
#define ACTOR_EN_ST 0xc37
#undef OBJECT_ST
#define OBJECT_ST 0x1024
#undef ActorShape_Init
#define ActorShape_Init ((void (*)(ActorShape* shape, f32 yOffset, ActorShadowFunc shadowDraw, f32 shadowScale))0x800b3ba4)
#undef Actor_ApplyDamage
#define Actor_ApplyDamage ((u8 (*)(Actor* actor))0x800be22c)
#undef Actor_Kill
extern void FOREIGN_ENEMY_API_KILL(Actor* actor);
#define Actor_Kill FOREIGN_ENEMY_API_KILL
#undef Actor_PlaySfx
extern void FOREIGN_ENEMY_API_SFX_ACTOR(Actor* actor, u16 sfxId);
#define Actor_PlaySfx FOREIGN_ENEMY_API_SFX_ACTOR
#undef Actor_SetColorFilter
extern void FOREIGN_ENEMY_API_COLOR_FILTER(Actor* actor, s16 colorFlag, s16 colorIntensityMax, s16 bufFlag,
                                           s16 duration);
#define Actor_SetColorFilter FOREIGN_ENEMY_API_COLOR_FILTER
#undef Actor_SetFocus
#define Actor_SetFocus ((void (*)(Actor* actor, f32 yOffset))0x800b675c)
#undef Actor_SetPlayerKnockbackLargeNoDamage
extern void FOREIGN_ENEMY_API_KNOCKBACK_LARGE_NO_DAMAGE(PlayState* play, Actor* actor, f32 speed, s16 rot,
                                                        f32 yVelocity);
#define Actor_SetPlayerKnockbackLargeNoDamage FOREIGN_ENEMY_API_KNOCKBACK_LARGE_NO_DAMAGE
#undef Actor_SetScale
#define Actor_SetScale ((void (*)(Actor* actor, f32 scale))0x800b67e0)
#undef Actor_UpdateBgCheckInfo
#define Actor_UpdateBgCheckInfo ((void (*)(PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight, s32 flags))0x800b78b8)
#undef Actor_UpdatePos
#define Actor_UpdatePos ((void (*)(Actor* actor))0x800b69ac)
#undef Actor_UpdateVelocityXZGravity
#define Actor_UpdateVelocityXZGravity ((void (*)(Actor* actor))0x800b6a10)
#undef Animation_ChangeByInfo
#define Animation_ChangeByInfo ((void (*)(SkelAnime* skelAnime, AnimationInfo* animationInfo, s32 index))0x800bdc5c)
#undef BgCheck_EntityLineTest1
#define BgCheck_EntityLineTest1 ((s32 (*)(CollisionContext*, Vec3f*, Vec3f*, Vec3f*, CollisionPoly**, s32, s32, s32, s32, s32*))0x800c55c4)
#undef Collider_DestroyCylinder
extern s32 FOREIGN_ENEMY_API_CYLINDER_DESTROY(PlayState* play, ColliderCylinder* cyl);
#define Collider_DestroyCylinder FOREIGN_ENEMY_API_CYLINDER_DESTROY
#undef Collider_DestroyJntSph
extern s32 FOREIGN_ENEMY_API_JOINT_DESTROY(PlayState* play, ColliderJntSph* jntSph);
#define Collider_DestroyJntSph FOREIGN_ENEMY_API_JOINT_DESTROY
#undef Collider_InitCylinder
#define Collider_InitCylinder ((s32 (*)(PlayState* play, ColliderCylinder* cyl))0x800e119c)
#undef Collider_InitJntSph
#define Collider_InitJntSph ((s32 (*)(PlayState* play, ColliderJntSph* jntSph))0x800e0b4c)
#undef Collider_SetCylinder
#define Collider_SetCylinder ((s32 (*)(PlayState* play, ColliderCylinder* dest, Actor* actor, ColliderCylinderInit* src))0x800e130c)
#undef Collider_SetJntSph
#define Collider_SetJntSph ((s32 (*)(PlayState* play, ColliderJntSph* dest, Actor* actor, ColliderJntSphInit* src, ColliderJntSphElement* jntSphElements))0x800e0e60)
#undef Collider_UpdateCylinder
#define Collider_UpdateCylinder ((void (*)(Actor* actor, ColliderCylinder* cyl))0x800e7df8)
#undef Collider_UpdateSpheres
#define Collider_UpdateSpheres ((void (*)(s32 limb, ColliderJntSph* jntSph))0x800e7fdc)
#undef CollisionCheck_SetAC
extern s32 FOREIGN_ENEMY_API_COLLISION_AC(PlayState* play, CollisionCheckContext* colChkCtx, Collider* collider);
#define CollisionCheck_SetAC FOREIGN_ENEMY_API_COLLISION_AC
#undef CollisionCheck_SetInfo2
#define CollisionCheck_SetInfo2 ((void (*)(CollisionCheckInfo* info, DamageTable* damageTable, CollisionCheckInfoInit2* init))0x800e755c)
#undef CollisionCheck_SetOC
extern s32 FOREIGN_ENEMY_API_COLLISION_OC(PlayState* play, CollisionCheckContext* colChkCtx, Collider* collider);
#define CollisionCheck_SetOC FOREIGN_ENEMY_API_COLLISION_OC
#undef DMG_ARROW
#define DMG_ARROW (DMG_ARROW_NORMAL | DMG_ARROW_FIRE | DMG_ARROW_ICE | DMG_ARROW_LIGHT | DMG_ARROW_UNK1 | DMG_ARROW_UNK2 | DMG_ARROW_UNK3)
#undef DMG_BOOMERANG
#define DMG_BOOMERANG (1 << 4)
#undef DMG_DEFAULT
#define DMG_DEFAULT ~(DMG_SHIELD | DMG_MIR_RAY)
#undef DMG_DEKU_NUT
#define DMG_DEKU_NUT (1 << 0)
#undef DMG_DEKU_STICK
#define DMG_DEKU_STICK (1 << 1)
#undef DMG_EXPLOSIVE
#define DMG_EXPLOSIVE (1 << 3)
#undef DMG_HOOKSHOT
#define DMG_HOOKSHOT (1 << 7)
#undef DamageTable_Get
extern DamageTable* FOREIGN_ENEMY_API_DAMAGE_TABLE(s32 index);
#define DamageTable_Get FOREIGN_ENEMY_API_DAMAGE_TABLE
#undef EffectSsDeadDb_Spawn
extern void FOREIGN_ENEMY_API_DEATH_FLAME(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale,
                                          s16 scaleStep, s16 primR, s16 primG, s16 primB, s16 primA, s16 envR, s16 envG,
                                          s16 envB, s16 unused, s32 arg14, s16 playSfx);
#define EffectSsDeadDb_Spawn FOREIGN_ENEMY_API_DEATH_FLAME
#undef Enemy_StartFinishingBlow
#define Enemy_StartFinishingBlow ((void (*)(PlayState* play, Actor* actor))0x800bba88)
#undef Gfx_SetupDL_25Opa
#define Gfx_SetupDL_25Opa ((void (*)(GraphicsContext* gfxCtx))0x8012c28c)
#undef Item_DropCollectibleRandom
extern void FOREIGN_ENEMY_API_RANDOM_DROP(PlayState* play, Actor* fromActor, Vec3f* spawnPos, s16 params);
#define Item_DropCollectibleRandom FOREIGN_ENEMY_API_RANDOM_DROP
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
#undef Matrix_MultVec3f
#define Matrix_MultVec3f ((void (*)(Vec3f* src, Vec3f* dest))0x80181a98)
#undef Matrix_Pop
#define Matrix_Pop ((void (*)(void))0x801801cc)
#undef Matrix_Push
#define Matrix_Push ((void (*)(void))0x8018019c)
#undef Matrix_RotateY
extern void FOREIGN_ENEMY_API_MATRIX_ROTATE_YF(f32 y, u8 mode);
#define Matrix_RotateY FOREIGN_ENEMY_API_MATRIX_ROTATE_YF
#undef Matrix_Translate
extern void FOREIGN_ENEMY_API_MATRIX_TRANSLATE(f32 x, f32 y, f32 z, u8 mode);
#define Matrix_Translate FOREIGN_ENEMY_API_MATRIX_TRANSLATE
#undef Rand_ZeroOne
#define Rand_ZeroOne ((f32 (*)(void))0x80086fdc)
#undef SkelAnime_DrawOpa
#define SkelAnime_DrawOpa ((void (*)(PlayState* play, void** skeleton, Vec3s* jointTable, void* overrideLimbDraw, void* postLimbDraw, void* arg))0x80133b3c)
#undef SkelAnime_Init
#define SkelAnime_Init ((void (*)(PlayState* play, SkelAnime* skelAnime, SkeletonHeader* skeletonHeaderSeg, AnimationHeader* animation, Vec3s* jointTable, Vec3s* morphTable, s32 limbCount))0x80136a7c)
#undef SkelAnime_Update
#define SkelAnime_Update ((s32 (*)(SkelAnime* skelAnime))0x80136cd0)
#undef func_8002836C
#define func_8002836C ((void (*)(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 scale, s16 scaleStep, s16 life))0x800b0eb0)
#undef NA_SE_EN_STALTU_DOWN
#define NA_SE_EN_STALTU_DOWN 0x3868
#undef NA_SE_EN_STALTU_UP
#define NA_SE_EN_STALTU_UP 0x3869
#undef NA_SE_EN_STALTU_LAUGH
#define NA_SE_EN_STALTU_LAUGH 0x386A
#undef NA_SE_EN_STALTU_DAMAGE
#define NA_SE_EN_STALTU_DAMAGE 0x386B
#undef NA_SE_EN_DODO_M_GND
#define NA_SE_EN_DODO_M_GND 0x387B
#undef NA_SE_EN_STALTU_ROLL
#define NA_SE_EN_STALTU_ROLL 0x3884
#undef NA_SE_EN_STALWALL_DEAD
#define NA_SE_EN_STALWALL_DEAD 0x3885
#undef NA_SE_EN_GOMA_JR_FREEZE
#define NA_SE_EN_GOMA_JR_FREEZE 0x389E
#undef NA_SE_EN_STALTU_WAVE
#define NA_SE_EN_STALTU_WAVE 0x39E0
#undef NA_SE_EN_STALTU_DOWN_SET
#define NA_SE_EN_STALTU_DOWN_SET 0x39E1
#undef NA_SE_PL_BODY_HIT
#define NA_SE_PL_BODY_HIT 0x83E
#define object_st_Anim_000304 (*(AnimationHeader*)0x06000304)
#define object_st_Skel_005298 (*(SkeletonHeader*)0x06005298)
#define object_st_Anim_0055A8 (*(AnimationHeader*)0x060055a8)
#define object_st_Anim_005B98 (*(AnimationHeader*)0x06005b98)

#define FLAGS                                                                                 \
    (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED | \
     ACTOR_FLAG_DRAW_CULLING_DISABLED)

void EnSt_Init(Actor* thisx, PlayState* play);
void EnSt_Destroy(Actor* thisx, PlayState* play);
void EnSt_Update(Actor* thisx, PlayState* play);
void EnSt_Draw(Actor* thisx, PlayState* play);
void EnSt_ReturnToCeiling(EnSt* this, PlayState* play);
void EnSt_MoveToGround(EnSt* this, PlayState* play);
void EnSt_StartOnCeilingOrGround(EnSt* this, PlayState* play);
void EnSt_WaitOnGround(EnSt* this, PlayState* play);
void EnSt_Die(EnSt* this, PlayState* play);
void EnSt_BounceAround(EnSt* this, PlayState* play);
void EnSt_FinishBouncing(EnSt* this, PlayState* play);

static Vtx sSkulltulaUnusedVtx[] = {};

static Gfx sSkulltulaUnusedDL[10] = {};

ActorProfile En_St_Profile = {
    ACTOR_EN_ST, ACTORCAT_ENEMY, FLAGS, OBJECT_ST, sizeof(EnSt), EnSt_Init, EnSt_Destroy, EnSt_Update, EnSt_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COL_MATERIAL_HIT6,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0x00000000, HIT_SPECIAL_EFFECT_NONE, 0x00 },
        { 0x00000000, HIT_BACKLASH_NONE, 0x00 },
        ATELEM_ON | ATELEM_SFX_NORMAL,
        ACELEM_ON,
        OCELEM_NONE,
    },
    { 32, 50, -24, { 0, 0, 0 } },
};

static CollisionCheckInfoInit2 sColChkInit = { 2, 0, 0, 0, MASS_IMMOVABLE };

static ColliderCylinderInit sCylinderInit2 = {
    {
        COL_MATERIAL_HIT6,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0x00000000, HIT_SPECIAL_EFFECT_NONE, 0x00 },
        { 0x00000000, HIT_BACKLASH_NONE, 0x00 },
        ATELEM_NONE,
        ACELEM_NONE,
        OCELEM_ON,
    },
    { 20, 60, -30, { 0, 0, 0 } },
};

static ColliderJntSphElementInit sJntSphElementsInit[] = {
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0xFFCFFFFF, HIT_SPECIAL_EFFECT_NONE, 0x04 },
            { 0x00000000, HIT_BACKLASH_NONE, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_NONE,
            OCELEM_ON,
        },
        { 1, { { 0, -240, 0 }, 28 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COL_MATERIAL_HIT6,
        AT_ON | AT_TYPE_ENEMY,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sJntSphElementsInit),
    sJntSphElementsInit,
};

typedef enum EnStAnimation {
    ENST_ANIM_0,
    ENST_ANIM_1,
    ENST_ANIM_2,
    ENST_ANIM_3,
    ENST_ANIM_4,
    ENST_ANIM_5,
    ENST_ANIM_6,
    ENST_ANIM_7
} EnStAnimation;

static AnimationInfo sAnimationInfo[] = {
    { &object_st_Anim_000304, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP_INTERP, 0.0f },
    { &object_st_Anim_005B98, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE_INTERP, -8.0f },
    { &object_st_Anim_000304, 4.0f, 0.0f, -1.0f, ANIMMODE_ONCE_INTERP, -8.0f },
    { &object_st_Anim_000304, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP_INTERP, -8.0f },
    { &object_st_Anim_0055A8, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE_INTERP, -8.0f },
    { &object_st_Anim_000304, 8.0f, 0.0f, -1.0f, ANIMMODE_LOOP_INTERP, -8.0f },
    { &object_st_Anim_000304, 6.0f, 0.0f, -1.0f, ANIMMODE_LOOP_INTERP, -8.0f },
    { &object_st_Anim_005B98, 2.0f, 0.0f, -1.0f, ANIMMODE_LOOP_INTERP, -8.0f },
};

void EnSt_SetupAction(EnSt* this, EnStActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

void EnSt_SpawnDust(EnSt* this, PlayState* play, s32 dustCnt) {
    Color_RGBA8 primColor = { 170, 130, 90, 255 };
    Color_RGBA8 envColor = { 100, 60, 20, 0 };
    Vec3f dustVel = { 0.0f, 0.0f, 0.0f };
    Vec3f dustAccel = { 0.0f, 0.3f, 0.0f };
    Vec3f dustPos;
    s16 yAngle;
    s32 i;

    yAngle = (Rand_ZeroOne() - 0.5f) * 65536.0f;
    dustPos.y = this->actor.floorHeight;
    for (i = dustCnt; i >= 0; i--, yAngle += (s16)(0x10000 / dustCnt)) {
        dustAccel.x = (Rand_ZeroOne() - 0.5f) * 4.0f;
        dustAccel.z = (Rand_ZeroOne() - 0.5f) * 4.0f;
        dustPos.x = this->actor.world.pos.x + (Math_SinS(yAngle) * 22.0f);
        dustPos.z = this->actor.world.pos.z + (Math_CosS(yAngle) * 22.0f);
        func_8002836C(play, &dustPos, &dustVel, &dustAccel, &primColor, &envColor, 120, 40, 10);
    }
}

void EnSt_SpawnBlastEffect(EnSt* this, PlayState* play) {
    Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };
    Vec3f blastPos;

    blastPos.x = this->actor.world.pos.x;
    blastPos.y = this->actor.floorHeight;
    blastPos.z = this->actor.world.pos.z;

    EffectSsBlast_SpawnWhiteShockwaveSetScale(play, &blastPos, &zeroVec, &zeroVec, 100, 220, 8);
}

void EnSt_SpawnDeadEffect(EnSt* this, PlayState* play) {
    Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };
    Vec3f firePos;

    firePos.x = this->actor.world.pos.x + ((Rand_ZeroOne() - 0.5f) * 60.0f);
    firePos.y = (this->actor.world.pos.y + 10.0f) + ((Rand_ZeroOne() - 0.5f) * 45.0f);
    firePos.z = this->actor.world.pos.z + ((Rand_ZeroOne() - 0.5f) * 60.0f);
    EffectSsDeadDb_Spawn(play, &firePos, &zeroVec, &zeroVec, 100, 0, 255, 255, 255, 255, 255, 0, 0, 1, 9, true);
}

s32 EnSt_CreateBlureEffect(PlayState* play) {
    EffectBlureInit1 blureInit;
    u8 p1StartColor[] = { 255, 255, 255, 75 };
    u8 p2StartColor[] = { 255, 255, 255, 75 };
    u8 p1EndColor[] = { 255, 255, 255, 0 };
    u8 p2EndColor[] = { 255, 255, 255, 0 };
    s32 i;
    s32 blureIdx;

    for (i = 0; i < 4; i++) {
        blureInit.p1StartColor[i] = p1StartColor[i];
        blureInit.p2StartColor[i] = p2StartColor[i];
        blureInit.p1EndColor[i] = p1EndColor[i];
        blureInit.p2EndColor[i] = p2EndColor[i];
    }

    blureInit.elemDuration = 6;
    blureInit.unkFlag = 0;
    blureInit.calcMode = 3;

    Effect_Add(play, &blureIdx, EFFECT_BLURE1, 0, 0, &blureInit);
    return blureIdx;
}

s32 EnSt_CheckCeilingPos(EnSt* this, PlayState* play) {
    CollisionPoly* poly;
    s32 bgId;
    Vec3f checkPos;

    checkPos.x = this->actor.world.pos.x;
    checkPos.y = this->actor.world.pos.y + 1000.0f;
    checkPos.z = this->actor.world.pos.z;
    if (!BgCheck_EntityLineTest1(&play->colCtx, &this->actor.world.pos, &checkPos, &this->ceilingPos, &poly, false,
                                 false, true, true, &bgId)) {
        return false;
    }
    this->unusedPos = this->actor.world.pos;
    this->unusedPos.y -= 100.0f;
    return true;
}

void EnSt_AddBlurVertex(EnSt* this) {
    Vec3f v1 = { 834.0f, 834.0f, 0.0f };
    Vec3f v2 = { 834.0f, -584.0f, 0.0f };
    Vec3f v1Pos;
    Vec3f v2Pos;

    v1.x *= this->colliderScale;
    v1.y *= this->colliderScale;
    v1.z *= this->colliderScale;

    v2.x *= this->colliderScale;
    v2.y *= this->colliderScale;
    v2.z *= this->colliderScale;

    Matrix_Push();
    Matrix_MultVec3f(&v1, &v1Pos);
    Matrix_MultVec3f(&v2, &v2Pos);
    Matrix_Pop();
    EffectBlure_AddVertex(Effect_GetByIndex(this->blureIdx), &v1Pos, &v2Pos);
}

void EnSt_AddBlurSpace(EnSt* this) {
    EffectBlure_AddSpace(Effect_GetByIndex(this->blureIdx));
}

void EnSt_SetWaitingAnimation(EnSt* this) {
    Animation_ChangeByInfo(&this->skelAnime, sAnimationInfo, ENST_ANIM_3);
}

void EnSt_SetReturnToCeilingAnimation(EnSt* this) {
    Actor_PlaySfx(&this->actor, NA_SE_EN_STALTU_UP);
    Animation_ChangeByInfo(&this->skelAnime, sAnimationInfo, ENST_ANIM_2);
}

void EnSt_SetLandAnimation(EnSt* this) {
    this->actor.world.pos.y = this->actor.floorHeight + this->floorHeightOffset;
    Animation_ChangeByInfo(&this->skelAnime, sAnimationInfo, ENST_ANIM_4);
    this->sfxTimer = 0;
    this->animFrames = this->skelAnime.animLength;
}

void EnSt_SetDropAnimAndVel(EnSt* this) {
    if (this->takeDamageSpinTimer == 0) {
        Animation_ChangeByInfo(&this->skelAnime, sAnimationInfo, ENST_ANIM_4);
        this->animFrames = this->skelAnime.animLength;
    }
    this->sfxTimer = 0;
    this->actor.velocity.y = -10.0f;
}

void EnSt_InitColliders(EnSt* this, PlayState* play) {
    ColliderCylinderInit* cylinders[6] = {
        &sCylinderInit, &sCylinderInit, &sCylinderInit, &sCylinderInit2, &sCylinderInit2, &sCylinderInit2,
    };

    s32 i;
    s32 pad;

    for (i = 0; i < ARRAY_COUNT(cylinders); i++) {
        Collider_InitCylinder(play, &this->colliderCylinders[i]);
        Collider_SetCylinder(play, &this->colliderCylinders[i], &this->actor, cylinders[i]);
    }

    this->colliderCylinders[0].elem.acDmgInfo.dmgFlags =
        DMG_MAGIC_FIRE | DMG_ARROW | DMG_HOOKSHOT | DMG_HAMMER_SWING | DMG_BOOMERANG | DMG_EXPLOSIVE | DMG_DEKU_NUT;
    this->colliderCylinders[1].elem.acDmgInfo.dmgFlags =
        DMG_DEFAULT &
        ~(DMG_MAGIC_FIRE | DMG_ARROW | DMG_HOOKSHOT | DMG_HAMMER_SWING | DMG_BOOMERANG | DMG_EXPLOSIVE | DMG_DEKU_NUT) &
        ~(DMG_MAGIC_LIGHT | DMG_MAGIC_ICE);
    this->colliderCylinders[2].base.colMaterial = COL_MATERIAL_METAL;
    this->colliderCylinders[2].elem.acElemFlags = ACELEM_ON | ACELEM_HOOKABLE | ACELEM_NO_AT_INFO;
    this->colliderCylinders[2].elem.elemMaterial = ELEM_MATERIAL_UNK2;
    this->colliderCylinders[2].elem.acDmgInfo.dmgFlags =
        DMG_DEFAULT &
        ~(DMG_MAGIC_FIRE | DMG_ARROW | DMG_HOOKSHOT | DMG_HAMMER_SWING | DMG_BOOMERANG | DMG_EXPLOSIVE | DMG_DEKU_NUT);

    CollisionCheck_SetInfo2(&this->actor.colChkInfo, DamageTable_Get(2), &sColChkInit);

    Collider_InitJntSph(play, &this->colliderJntSph);
    Collider_SetJntSph(play, &this->colliderJntSph, &this->actor, &sJntSphInit, this->colliderJntSphElements);
}

void EnSt_CheckBodyStickHit(EnSt* this, PlayState* play) {
    ColliderElement* bodyElem = &this->colliderCylinders[0].elem;
    Player* player = GET_PLAYER(play);

    if (player->unk_B28 != 0) {
        bodyElem->acDmgInfo.dmgFlags |= DMG_DEKU_STICK;
        this->colliderCylinders[1].elem.acDmgInfo.dmgFlags &= ~DMG_DEKU_STICK;
        this->colliderCylinders[2].elem.acDmgInfo.dmgFlags &= ~DMG_DEKU_STICK;
    } else {
        bodyElem->acDmgInfo.dmgFlags &= ~DMG_DEKU_STICK;
        this->colliderCylinders[1].elem.acDmgInfo.dmgFlags |= DMG_DEKU_STICK;
        this->colliderCylinders[2].elem.acDmgInfo.dmgFlags |= DMG_DEKU_STICK;
    }
}

void EnSt_SetBodyCylinderAC(EnSt* this, PlayState* play) {
    Collider_UpdateCylinder(&this->actor, &this->colliderCylinders[0]);
    CollisionCheck_SetAC(play, &play->colChkCtx, &this->colliderCylinders[0].base);
}

void EnSt_SetLegsCylinderAC(EnSt* this, PlayState* play) {
    s16 angleTowardsLink = ABS((s16)(this->actor.yawTowardsPlayer - this->actor.shape.rot.y));

    if (angleTowardsLink < 0x3FFC) {
        Collider_UpdateCylinder(&this->actor, &this->colliderCylinders[2]);
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->colliderCylinders[2].base);
    } else {
        Collider_UpdateCylinder(&this->actor, &this->colliderCylinders[1]);
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->colliderCylinders[1].base);
    }
}

s32 EnSt_SetCylinderOC(EnSt* this, PlayState* play) {
    Vec3f cyloffsets[] = {
        { 40.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f },
        { -40.0f, 0.0f, 0.0f },
    };
    Vec3f cylPos;
    s32 i;

    for (i = 0; i < 3; i++) {
        cylPos = this->actor.world.pos;
        cyloffsets[i].x *= this->colliderScale;
        cyloffsets[i].y *= this->colliderScale;
        cyloffsets[i].z *= this->colliderScale;
        Matrix_Push();
        Matrix_Translate(cylPos.x, cylPos.y, cylPos.z, MTXMODE_NEW);
        Matrix_RotateY(BINANG_TO_RAD_ALT(this->initialYaw), MTXMODE_APPLY);
        Matrix_MultVec3f(&cyloffsets[i], &cylPos);
        Matrix_Pop();
        this->colliderCylinders[i + 3].dim.pos.x = cylPos.x;
        this->colliderCylinders[i + 3].dim.pos.y = cylPos.y;
        this->colliderCylinders[i + 3].dim.pos.z = cylPos.z;
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->colliderCylinders[i + 3].base);
    }

    return true;
}

void EnSt_UpdateCylinders(EnSt* this, PlayState* play) {
    if ((this->actor.colChkInfo.health != 0) || (this->actionFunc == EnSt_FinishBouncing)) {
        if (DECR(this->gaveDamageSpinTimer) == 0) {
            EnSt_SetCylinderOC(this, play);
        }

        DECR(this->invulnerableTimer);
        DECR(this->takeDamageSpinTimer);

        if (this->invulnerableTimer == 0 && this->takeDamageSpinTimer == 0) {
            EnSt_SetBodyCylinderAC(this, play);
            EnSt_SetLegsCylinderAC(this, play);
        }
    }
}

s32 EnSt_CheckHitPlayer(EnSt* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 hit;
    s32 i;

    for (i = 0, hit = 0; i < 3; i++) {
        if (((this->colliderCylinders[i + 3].base.ocFlags2 & OC2_HIT_PLAYER) != 0) == 0) {
            continue;
        }
        this->colliderCylinders[i + 3].base.ocFlags2 &= ~OC2_HIT_PLAYER;
        hit = true;
    }

    if (!hit) {
        return false;
    }

    if (this->swayTimer == 0) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_STALTU_ROLL);
    }

    this->gaveDamageSpinTimer = 30;
    play->damagePlayer(play, -8);
    Actor_PlaySfx(&player->actor, NA_SE_PL_BODY_HIT);
    Actor_SetPlayerKnockbackLargeNoDamage(play, &this->actor, 4.0f, this->actor.yawTowardsPlayer, 6.0f);
    return true;
}

s32 EnSt_CheckHitFrontside(EnSt* this) {
    u8 acFlags = this->colliderCylinders[2].base.acFlags;

    if (!!(acFlags & AC_HIT) == 0) {

        return false;
    } else {
        this->colliderCylinders[2].base.acFlags &= ~AC_HIT;
        this->invulnerableTimer = 8;
        this->playSwayFlag = 0;
        this->swayTimer = 60;
        return true;
    }
}

s32 EnSt_CheckHitBackside(EnSt* this, PlayState* play) {
    ColliderCylinder* collider = &this->colliderCylinders[0];
    s32 flags = 0;
    s32 hit = false;

    if (collider->base.acFlags & AC_HIT) {
        collider->base.acFlags &= ~AC_HIT;
        hit = true;
        flags |= collider->elem.acHitElem->atDmgInfo.dmgFlags;
    }

    collider = &this->colliderCylinders[1];
    if (collider->base.acFlags & AC_HIT) {
        collider->base.acFlags &= ~AC_HIT;
        hit = true;
        flags |= collider->elem.acHitElem->atDmgInfo.dmgFlags;
    }

    if (!hit) {
        return false;
    }

    this->invulnerableTimer = 8;
    if (this->actor.colChkInfo.damageEffect == 1) {
        if (this->stunTimer == 0) {
            Actor_PlaySfx(&this->actor, NA_SE_EN_GOMA_JR_FREEZE);
            this->stunTimer = 120;
            Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_BLUE, 200, COLORFILTER_BUFFLAG_OPA,
                                 this->stunTimer);
        }
        return false;
    }

    this->swayTimer = this->stunTimer = 0;
    this->gaveDamageSpinTimer = 1;
    Animation_ChangeByInfo(&this->skelAnime, sAnimationInfo, ENST_ANIM_3);
    this->takeDamageSpinTimer = this->skelAnime.animLength;
    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 200, COLORFILTER_BUFFLAG_OPA,
                         this->takeDamageSpinTimer);
    if (Actor_ApplyDamage(&this->actor)) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_STALTU_DAMAGE);
        return false;
    }
    Enemy_StartFinishingBlow(play, &this->actor);
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    this->groundBounces = 3;
    this->deathTimer = 20;
    this->actor.gravity = -1.0f;
    Actor_PlaySfx(&this->actor, NA_SE_EN_STALWALL_DEAD);

    if (flags & DMG_ARROW) {
        EnSt_SetupAction(this, EnSt_Die);
        this->finishDeathTimer = 8;
    } else {
        EnSt_SetupAction(this, EnSt_BounceAround);
    }

    return true;
}

s32 EnSt_CheckColliders(EnSt* this, PlayState* play) {
    if (EnSt_CheckHitFrontside(this)) {

        return false;
    }

    if (play->actorCtx.unk2 != 0) {
        return true;
    }

    if (EnSt_CheckHitBackside(this, play)) {

        return true;
    }

    if (this->stunTimer == 0 && this->takeDamageSpinTimer == 0) {

        EnSt_CheckHitPlayer(this, play);
    }
    return false;
}

void EnSt_SetColliderScale(EnSt* this) {
    f32 scaleAmount = 1.0f;
    f32 radius;
    f32 height;
    f32 yShift;
    s32 i;

    if (this->actor.params == 1) {
        scaleAmount = 1.4f;
    }

    radius = this->colliderJntSph.elements[0].dim.modelSphere.radius;
    radius *= scaleAmount;
    this->colliderJntSph.elements[0].dim.modelSphere.radius = radius;

    for (i = 0; i < 6; i++) {
        yShift = this->colliderCylinders[i].dim.yShift;
        radius = this->colliderCylinders[i].dim.radius;
        height = this->colliderCylinders[i].dim.height;
        yShift *= scaleAmount;
        radius *= scaleAmount;
        height *= scaleAmount;

        this->colliderCylinders[i].dim.yShift = yShift;
        this->colliderCylinders[i].dim.radius = radius;
        this->colliderCylinders[i].dim.height = height;
    }
    Actor_SetScale(&this->actor, 0.04f * scaleAmount);
    this->colliderScale = scaleAmount;
    this->floorHeightOffset = 32.0f * scaleAmount;
}

s32 EnSt_SetTeethColor(EnSt* this, s16 redTarget, s16 greenTarget, s16 blueTarget, s16 minMaxStep) {
    s16 red = this->teethR;
    s16 green = this->teethG;
    s16 blue = this->teethB;

    minMaxStep = 255 / (s16)(0.6f * minMaxStep);
    if (minMaxStep <= 0) {
        minMaxStep = 1;
    }

    Math_SmoothStepToS(&red, redTarget, 1, minMaxStep, minMaxStep);
    Math_SmoothStepToS(&green, greenTarget, 1, minMaxStep, minMaxStep);
    Math_SmoothStepToS(&blue, blueTarget, 1, minMaxStep, minMaxStep);
    this->teethR = red;
    this->teethG = green;
    this->teethB = blue;
    return 1;
}

s32 EnSt_DecrStunTimer(EnSt* this) {
    if (this->stunTimer == 0) {
        return 0;
    }
    this->stunTimer--;
    return this->stunTimer + 1;
}

void EnSt_UpdateYaw(EnSt* this, PlayState* play) {
    u16 yawDir = 0;
    Vec3s rot;
    s16 yawDiff;
    s16 timer;
    s16 yawTarget;

    if (this->stunTimer != 0) {
        if (this->stunTimer < 30) {
            if ((this->stunTimer % 2) != 0) {
                this->actor.shape.rot.y += 0x800;
            } else {
                this->actor.shape.rot.y -= 0x800;
            }
        }
        return;
    }

    if (this->swayTimer == 0 && this->deathTimer == 0 && this->finishDeathTimer == 0) {

        if (this->takeDamageSpinTimer != 0 || this->gaveDamageSpinTimer != 0) {

            this->actor.shape.rot.y += 0x2000;
            return;
        }

        if (this->actionFunc != EnSt_WaitOnGround) {

            this->rotAwayTimer = 30;
            this->rotTowardsTimer = 0;
        }

        if (this->rotAwayTimer != 0) {

            this->rotAwayTimer--;
            if (this->rotAwayTimer == 0) {
                Actor_PlaySfx(&this->actor, NA_SE_EN_STALTU_ROLL);
                this->rotTowardsTimer = 30;
            }
        } else if (this->rotTowardsTimer != 0) {

            this->rotTowardsTimer--;
            if (this->rotTowardsTimer == 0) {
                Actor_PlaySfx(&this->actor, NA_SE_EN_STALTU_ROLL);
                this->rotAwayTimer = 30;
            }
            yawDir = 0x8000;
        }

        rot = this->actor.shape.rot;
        yawTarget = (this->actionFunc == EnSt_WaitOnGround ? this->actor.yawTowardsPlayer : this->initialYaw);
        yawDiff = rot.y - (yawTarget ^ yawDir);
        if (ABS(yawDiff) <= 0x4000) {
            Math_SmoothStepToS(&rot.y, yawTarget ^ yawDir, 4, 0x2000, 1);
        } else {
            rot.y += 0x2000;
        }

        this->actor.shape.rot = this->actor.world.rot = rot;

        if (yawDir == 0 && this->rotAwayTimer < 0xA) {
            timer = this->rotAwayTimer;
        } else if (yawDir == 0x8000 && this->rotTowardsTimer < 0xA) {
            timer = this->rotTowardsTimer;
        } else {
            return;
        }

        if ((timer % 2) != 0) {
            this->actor.shape.rot.y += 0x800;
        } else {
            this->actor.shape.rot.y -= 0x800;
        }
    }
}

s32 EnSt_IsDoneBouncing(EnSt* this, PlayState* play) {
    if (this->actor.velocity.y > 0.0f || this->groundBounces == 0) {

        return false;
    }

    if (!(this->actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {

        return false;
    }

    Actor_PlaySfx(&this->actor, NA_SE_EN_DODO_M_GND);
    EnSt_SpawnDust(this, play, 10);

    this->actor.velocity.y = 6.0f / (4 - this->groundBounces);
    this->groundBounces--;
    if (this->groundBounces != 0) {
        return false;
    } else {

        this->actor.velocity.y = 0.0f;
    }
    return true;
}

void EnSt_Bob(EnSt* this, PlayState* play) {
    f32 ySpeedTarget = 0.5f;

    if ((play->gameplayFrames & 8) != 0) {
        ySpeedTarget *= -1.0f;
    }
    Math_SmoothStepToF(&this->actor.velocity.y, ySpeedTarget, 0.4f, 1000.0f, 0.0f);
}

s32 EnSt_IsCloseToPlayer(EnSt* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    f32 yDist;

    if (this->takeDamageSpinTimer != 0) {

        return false;
    } else if (this->actor.xzDistToPlayer > 160.0f) {

        return false;
    }

    yDist = this->actor.world.pos.y - player->actor.world.pos.y;
    if (yDist < 0.0f || yDist > 400.0f) {

        return false;
    }

    if (player->actor.world.pos.y < this->actor.floorHeight) {

        return false;
    }
    return true;
}

s32 EnSt_IsCloseToInitialPos(EnSt* this) {
    f32 velY = this->actor.velocity.y;
    f32 checkY = this->actor.world.pos.y + (velY * 2.0f);

    if (checkY >= this->actor.home.pos.y) {
        return true;
    }
    return false;
}

s32 EnSt_IsCloseToGround(EnSt* this) {
    f32 velY = this->actor.velocity.y;
    f32 checkY = this->actor.world.pos.y + (velY * 2.0f);

    if (checkY - this->actor.floorHeight <= this->floorHeightOffset) {
        return true;
    }
    return false;
}

void EnSt_Sway(EnSt* this) {
    Vec3f amtToTranslate;
    Vec3f translatedPos;
    f32 swayAmt;
    s16 rotAngle;

    if (this->swayTimer != 0) {

        this->swayAngle += 0xA28;
        this->swayTimer--;

        if (this->swayTimer == 0) {
            this->swayAngle = 0;
        }

        swayAmt = this->swayTimer * (7.0f / 15.0f);
        rotAngle = Math_SinS(this->swayAngle) * (swayAmt * (65536.0f / 360.0f));

        if (this->absPrevSwayAngle >= ABS(rotAngle) && this->playSwayFlag == 0) {
            Actor_PlaySfx(&this->actor, NA_SE_EN_STALTU_WAVE);
            this->playSwayFlag = 1;
        }

        if (this->absPrevSwayAngle < ABS(rotAngle)) {
            this->playSwayFlag = 0;
        }

        this->absPrevSwayAngle = ABS(rotAngle);
        amtToTranslate.x = Math_SinS(rotAngle) * -200.0f;
        amtToTranslate.y = Math_CosS(rotAngle) * -200.0f;
        amtToTranslate.z = 0.0f;
        Matrix_Push();
        Matrix_Translate(this->ceilingPos.x, this->ceilingPos.y, this->ceilingPos.z, MTXMODE_NEW);
        Matrix_RotateY(BINANG_TO_RAD(this->actor.world.rot.y), MTXMODE_APPLY);
        Matrix_MultVec3f(&amtToTranslate, &translatedPos);
        Matrix_Pop();
        this->actor.shape.rot.z = -(rotAngle * 2);
        this->actor.world.pos.x = translatedPos.x;
        this->actor.world.pos.z = translatedPos.z;
    }
}

void EnSt_Init(Actor* thisx, PlayState* play) {
    EnSt* this = (EnSt*)thisx;
    s32 pad;

    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 14.0f);
    SkelAnime_Init(play, &this->skelAnime, &object_st_Skel_005298, NULL, this->jointTable, this->morphTable, 30);
    Animation_ChangeByInfo(&this->skelAnime, sAnimationInfo, ENST_ANIM_0);
    this->blureIdx = EnSt_CreateBlureEffect(play);
    EnSt_InitColliders(this, play);
    if (thisx->params == 2) {
        this->actor.flags |= ACTOR_FLAG_REACT_TO_LENS;
    }
    if (this->actor.params == 1) {
        this->actor.naviEnemyId = TATL_HINT_ID_SKULLTULA;
    } else {
        this->actor.naviEnemyId = TATL_HINT_ID_SKULLTULA;
    }
    EnSt_CheckCeilingPos(this, play);
    this->actor.flags |= ACTOR_FLAG_CAN_ATTACH_TO_ARROW;
    this->actor.flags |= ACTOR_FLAG_SFX_FOR_PLAYER_BODY_HIT;
    EnSt_SetColliderScale(this);
    this->actor.gravity = 0.0f;
    this->initialYaw = this->actor.world.rot.y;
    EnSt_SetupAction(this, EnSt_StartOnCeilingOrGround);
}

void EnSt_Destroy(Actor* thisx, PlayState* play) {
    EnSt* this = (EnSt*)thisx;
    s32 i;

    Effect_Delete(play, this->blureIdx);
    for (i = 0; i < 6; i++) {
        Collider_DestroyCylinder(play, &this->colliderCylinders[i]);
    }
    Collider_DestroyJntSph(play, &this->colliderJntSph);
}

void EnSt_WaitOnCeiling(EnSt* this, PlayState* play) {
    if (EnSt_IsCloseToPlayer(this, play)) {
        EnSt_SetDropAnimAndVel(this);
        EnSt_SetupAction(this, EnSt_MoveToGround);
    } else {
        EnSt_Bob(this, play);
    }
}

void EnSt_WaitOnGround(EnSt* this, PlayState* play) {
    if (this->takeDamageSpinTimer != 0) {
        this->takeDamageSpinTimer--;
        if (this->takeDamageSpinTimer == 0) {
            Animation_ChangeByInfo(&this->skelAnime, sAnimationInfo, ENST_ANIM_3);
        }
    }

    if (this->animFrames != 0) {
        this->animFrames--;
        if (this->animFrames == 0) {
            Animation_ChangeByInfo(&this->skelAnime, sAnimationInfo, ENST_ANIM_3);
        }
    }

    if (!EnSt_IsCloseToPlayer(this, play)) {

        EnSt_SetReturnToCeilingAnimation(this);
        EnSt_SetupAction(this, EnSt_ReturnToCeiling);
        return;
    }

    if (DECR(this->sfxTimer) == 0) {

        Actor_PlaySfx(&this->actor, NA_SE_EN_STALTU_LAUGH);
        this->sfxTimer = 64;
    }

    EnSt_Bob(this, play);
}

void EnSt_LandOnGround(EnSt* this, PlayState* play) {
    if (this->animFrames != 0) {
        this->animFrames--;
        if (this->animFrames == 0) {
            Animation_ChangeByInfo(&this->skelAnime, sAnimationInfo, ENST_ANIM_3);
        }
    }

    if (this->takeDamageSpinTimer != 0) {
        this->takeDamageSpinTimer--;
        if (this->takeDamageSpinTimer == 0) {
            Animation_ChangeByInfo(&this->skelAnime, sAnimationInfo, ENST_ANIM_3);
        }
    }

    this->sfxTimer++;
    if (this->sfxTimer == 14) {

        Actor_PlaySfx(&this->actor, NA_SE_EN_STALTU_DOWN_SET);
    }

    if ((this->actor.floorHeight + this->floorHeightOffset) < this->actor.world.pos.y) {

        this->sfxTimer = 0;
        EnSt_SetupAction(this, EnSt_WaitOnGround);
    } else {
        Math_SmoothStepToF(&this->actor.velocity.y, 2.0f, 0.3f, 1.0f, 0.0f);
    }
}

void EnSt_MoveToGround(EnSt* this, PlayState* play) {
    if (this->takeDamageSpinTimer != 0) {
        this->takeDamageSpinTimer--;
        if (this->takeDamageSpinTimer == 0) {
            Animation_ChangeByInfo(&this->skelAnime, sAnimationInfo, ENST_ANIM_5);
        }
    }

    if (!EnSt_IsCloseToPlayer(this, play)) {

        EnSt_SetReturnToCeilingAnimation(this);
        EnSt_SetupAction(this, EnSt_ReturnToCeiling);
    } else if (EnSt_IsCloseToGround(this)) {

        EnSt_SpawnBlastEffect(this, play);
        EnSt_SetLandAnimation(this);
        EnSt_SetupAction(this, EnSt_LandOnGround);
    } else if (DECR(this->sfxTimer) == 0) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_STALTU_DOWN);
        this->sfxTimer = 3;
    }
}

void EnSt_ReturnToCeiling(EnSt* this, PlayState* play) {
    f32 animPctDone = this->skelAnime.curFrame / (this->skelAnime.animLength - 1.0f);

    if (animPctDone == 1.0f) {
        EnSt_SetReturnToCeilingAnimation(this);
    }

    if (EnSt_IsCloseToPlayer(this, play)) {

        EnSt_SetDropAnimAndVel(this);
        EnSt_SetupAction(this, EnSt_MoveToGround);
    } else if (EnSt_IsCloseToInitialPos(this)) {

        EnSt_SetWaitingAnimation(this);
        EnSt_SetupAction(this, EnSt_WaitOnCeiling);
    } else {

        this->actor.velocity.y = 4.0f * animPctDone;
    }
}

void EnSt_BounceAround(EnSt* this, PlayState* play) {
    this->actor.colorFilterTimer = this->deathTimer;
    Actor_UpdateVelocityXZGravity(&this->actor);
    this->actor.world.rot.x += 0x800;
    this->actor.world.rot.z -= 0x800;
    this->actor.shape.rot = this->actor.world.rot;
    if (EnSt_IsDoneBouncing(this, play)) {
        this->actor.shape.yOffset = 400.0f;
        this->actor.speed = 1.0f;
        this->actor.gravity = -2.0f;
        EnSt_SetupAction(this, EnSt_FinishBouncing);
    } else {
        Math_SmoothStepToF(&this->actor.shape.yOffset, 400.0f, 0.4f, 10000.0f, 0.0f);
    }
}

void EnSt_FinishBouncing(EnSt* this, PlayState* play) {
    Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };

    if (DECR(this->deathTimer) == 0) {
        this->actor.velocity = zeroVec;
        this->finishDeathTimer = 8;
        EnSt_SetupAction(this, EnSt_Die);
        return;
    }

    if (DECR(this->setTargetYawTimer) == 0) {
        this->deathYawTarget = Math_Vec3f_Yaw(&this->actor.world.pos, &this->actor.home.pos);
        this->setTargetYawTimer = 8;
    }

    Math_SmoothStepToS(&this->actor.world.rot.x, 0x3FFC, 4, 0x2710, 1);
    Math_SmoothStepToS(&this->actor.world.rot.z, 0, 4, 0x2710, 1);
    Math_SmoothStepToS(&this->actor.world.rot.y, this->deathYawTarget, 0xA, 0x2710, 1);

    this->actor.shape.rot = this->actor.world.rot;

    Actor_UpdateVelocityXZGravity(&this->actor);
    this->groundBounces = 2;
    EnSt_IsDoneBouncing(this, play);
}

void EnSt_Die(EnSt* this, PlayState* play) {
    if (DECR(this->finishDeathTimer) != 0) {
        EnSt_SpawnDeadEffect(this, play);
    } else {
        Item_DropCollectibleRandom(play, NULL, &this->actor.world.pos,
                                   COLLECTIBLE_DROP_RANDOM_PARAMS(COLLECTIBLE_DROP_TABLE_14, false));
        Actor_Kill(&this->actor);
    }
}

void EnSt_StartOnCeilingOrGround(EnSt* this, PlayState* play) {
    if (!EnSt_IsCloseToGround(this)) {
        this->rotAwayTimer = 60;
        EnSt_SetupAction(this, EnSt_WaitOnCeiling);
        EnSt_WaitOnCeiling(this, play);
    } else {
        EnSt_SetLandAnimation(this);
        EnSt_SetupAction(this, EnSt_LandOnGround);
        EnSt_LandOnGround(this, play);
    }
}

void EnSt_Update(Actor* thisx, PlayState* play) {
    EnSt* this = (EnSt*)thisx;
    s32 pad;
    Color_RGBA8 color = { 0, 0, 0, 0 };

    if (this->actor.flags & ACTOR_FLAG_ATTACHED_TO_ARROW) {
        SkelAnime_Update(&this->skelAnime);
    } else if (!EnSt_CheckColliders(this, play)) {

        if (this->stunTimer == 0) {
            SkelAnime_Update(&this->skelAnime);
        }

        if (this->swayTimer == 0 && this->stunTimer == 0) {
            Actor_UpdatePos(&this->actor);
        }

        Actor_UpdateBgCheckInfo(play, &this->actor, 0.0f, 0.0f, 0.0f, UPDBGCHECKINFO_FLAG_2);

        if ((this->stunTimer == 0) && (this->swayTimer == 0)) {

            this->actionFunc(this, play);
        } else if (this->stunTimer != 0) {

            EnSt_DecrStunTimer(this);
        } else {

            EnSt_Sway(this);
        }

        EnSt_UpdateYaw(this, play);

        if (this->actionFunc == EnSt_WaitOnGround) {
            if ((play->gameplayFrames & 0x10) != 0) {
                color.r = 255;
            }
        }

        EnSt_SetTeethColor(this, color.r, color.g, color.b, 8);
        EnSt_UpdateCylinders(this, play);
        Actor_SetFocus(&this->actor, 0.0f);
    }
}

s32 EnSt_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dListP, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnSt* this = (EnSt*)thisx;

    OPEN_DISPS(play->state.gfxCtx);
    switch (limbIndex) {
        case 1:
            if (this->gaveDamageSpinTimer != 0 && this->swayTimer == 0) {
                if (this->gaveDamageSpinTimer >= 2) {
                    EnSt_AddBlurVertex(this);
                } else {
                    EnSt_AddBlurSpace(this);
                }
            }
            break;
        case 4:

            gDPPipeSync(POLY_OPA_DISP++);
            gDPSetEnvColor(POLY_OPA_DISP++, this->teethR, this->teethG, this->teethB, 0);
            break;
    }
    CLOSE_DISPS();
    return false;
}

void EnSt_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dListP, Vec3s* rot, void* thisx) {
    EnSt* this = (EnSt*)thisx;

    Collider_UpdateSpheres(limbIndex, &this->colliderJntSph);
}

void EnSt_Draw(Actor* thisx, PlayState* play) {
    EnSt* this = (EnSt*)thisx;

    EnSt_CheckBodyStickHit(this, play);
    Gfx_SetupDL_25Opa(play->state.gfxCtx);
    SkelAnime_DrawOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, EnSt_OverrideLimbDraw,
                      EnSt_PostLimbDraw, this);
}

OVL_INFO_NATIVE_ENEMY(0x37, En_St_Profile);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_ST_INIT, EnSt_Init);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_ST_DESTROY, EnSt_Destroy);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_ST_UPDATE, EnSt_Update);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_ST_DRAW, EnSt_Draw);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_ST_PROFILE, En_St_Profile);
static const u32 kNativePortOrigin[] = { 0x4e505431, 0, 0x37, 0x24, 0x4e7d5782 };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_ST_ORIGIN, kNativePortOrigin);
