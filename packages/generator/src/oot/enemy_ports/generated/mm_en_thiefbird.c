#include "mm_native_compat.h"
#define gTakkuriDeathAnim (*(AnimationHeader*)0x06000088)
#define gTakkuriFlyWithItemAnim (*(AnimationHeader*)0x06000278)
#define gTakkuriFlyAnim (*(AnimationHeader*)0x06000604)
#define gTakkuriFeatherMaterialDL (*((Gfx (*)[1])0x06003060))
#define gTakkuriFeatherModelDL (*((Gfx (*)[1])0x060030d8))
#define gTakkuriStolenBottleDL (*((Gfx (*)[1])0x060033b0))
#define gTakkuriSkel (*(FlexSkeletonHeader*)0x060061a0)
#define gTakkuriAttackAnim (*(AnimationHeader*)0x060063c4)

#include "types/en_thiefbird.h"
#define TATL_HINT_ID_TAKKURI 0xff
#undef ACTOR_EN_ITEM00
#define ACTOR_EN_ITEM00 0x15
#undef ACTOR_EN_CLEAR_TAG
#define ACTOR_EN_CLEAR_TAG 0xa2
#undef ACTOR_EN_THIEFBIRD
#define ACTOR_EN_THIEFBIRD 0x291
#undef OBJECT_THIEFBIRD
#define OBJECT_THIEFBIRD 0x1022
#undef NA_SE_EV_ICE_FREEZE
#define NA_SE_EV_ICE_FREEZE 0x28b2
#undef NA_SE_EN_EXTINCT
#define NA_SE_EN_EXTINCT 0x3878
#undef NA_SE_EN_KAICHO_FLUTTER
#define NA_SE_EN_KAICHO_FLUTTER 0x3897
#undef NA_SE_EN_COMMON_FREEZE
#define NA_SE_EN_COMMON_FREEZE 0x389e
#undef NA_SE_EN_THIEFBIRD_VOICE
#define NA_SE_EN_THIEFBIRD_VOICE 0x3a97
#undef NA_SE_EN_THIEFBIRD_DAMAGE
#define NA_SE_EN_THIEFBIRD_DAMAGE 0x3a98
#undef NA_SE_EN_THIEFBIRD_DEAD
#define NA_SE_EN_THIEFBIRD_DEAD 0x3a99
#undef Collider_InitAndSetJntSph
#define Collider_InitAndSetJntSph ((s32 (*)(struct PlayState* play, ColliderJntSph* dest, struct Actor* actor, ColliderJntSphInit* src, ColliderJntSphElement* jntSphElements))(void*)FOREIGN_ENEMY_API_JOINT_INIT_SET)
#undef Actor_Kill
extern u8 FOREIGN_ENEMY_API_KILL[];
#define Actor_Kill ((void (*)(Actor* actor))(void*)FOREIGN_ENEMY_API_KILL)
#undef Collider_DestroyJntSph
#define Collider_DestroyJntSph ((s32 (*)(struct PlayState* play, ColliderJntSph* jntSph))(void*)FOREIGN_ENEMY_API_JOINT_DESTROY)
#undef Item_DropCollectible
#define Item_DropCollectible ((Actor* (*)(struct PlayState* play, Vec3f* spawnPos, u32 params))(void*)FOREIGN_ENEMY_API_DROP)
#undef Actor_SetColorFilter
#define Actor_SetColorFilter ((void (*)(Actor* actor, u16 colorFlag, u16 colorIntensityMax, u16 bufFlag, u16 duration))(void*)FOREIGN_ENEMY_API_COLOR_FILTER)
#undef Actor_SpawnIceEffects
#define Actor_SpawnIceEffects ((void (*)(struct PlayState* play, Actor* actor, Vec3f bodyPartsPos[], s32 bodyPartsCount, s32 effectsPerBodyPart, f32 scale, f32 scaleRange))(void*)FOREIGN_ENEMY_API_ICE_SHATTER)
#undef Actor_PlaySfx
#define Actor_PlaySfx ((void (*)(Actor* actor, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_ACTOR)
#undef Player_GetMask
#define Player_GetMask ((s32 (*)(struct PlayState* play))(void*)FOREIGN_ENEMY_API_PLAYER_MASK)
#undef func_800B3030
#define func_800B3030 ((void (*)(struct PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep, s32 colorIndex))(void*)FOREIGN_ENEMY_API_DEATH_FLAME_PRESET)
#undef SoundSource_PlaySfxAtFixedWorldPos
#define SoundSource_PlaySfxAtFixedWorldPos ((void (*)(struct PlayState* play, Vec3f* worldPos, u32 duration, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_WORLD)
#undef Actor_SetDropFlag
#define Actor_SetDropFlag ((void (*)(Actor* actor, ColliderElement* elem))(void*)FOREIGN_ENEMY_API_DROP_FLAG)
#undef Actor_Spawn
#define Actor_Spawn ((Actor* (*)(ActorContext* actorCtx, struct PlayState* play, s16 actorId, f32 posX, f32 posY, f32 posZ, s16 rotX, s16 rotY, s16 rotZ, s32 params))(void*)FOREIGN_ENEMY_API_SPAWN)
#undef CollisionCheck_SetAT
#define CollisionCheck_SetAT ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AT)
#undef CollisionCheck_SetAC
#define CollisionCheck_SetAC ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AC)
#undef CollisionCheck_SetOC
#define CollisionCheck_SetOC ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_OC)
#undef Actor_PlaySfx_Flagged
#define Actor_PlaySfx_Flagged ((void (*)(Actor* actor, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_FLAGGED)
#undef Matrix_MultVecX
#define Matrix_MultVecX ((void (*)(f32 x, Vec3f* dest))(void*)FOREIGN_ENEMY_API_MATRIX_MULT_X)
#undef Matrix_MultZero
#define Matrix_MultZero ((void (*)(Vec3f* dest))(void*)FOREIGN_ENEMY_API_MATRIX_MULT_ZERO)
#undef Matrix_Translate
#define Matrix_Translate ((void (*)(f32 x, f32 y, f32 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_TRANSLATE)
#undef Matrix_RotateYS
#define Matrix_RotateYS ((void (*)(s16 y, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_YS)
#undef Matrix_RotateZS
#define Matrix_RotateZS ((void (*)(s16 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_ZS)
#undef Matrix_Scale
#define Matrix_Scale ((void (*)(f32 x, f32 y, f32 z, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_SCALE)
#undef Actor_DrawDamageEffects
#define Actor_DrawDamageEffects ((void (*)(struct PlayState* play, Actor* actor, Vec3f bodyPartsPos[], s16 bodyPartsCount, f32 effectScale, f32 frozenSteamScale, f32 effectAlpha, u8 type))(void*)FOREIGN_ENEMY_API_DAMAGE_EFFECTS)
#undef Actor_ProcessInitChain
#define Actor_ProcessInitChain ((void (*)(struct Actor* actor, InitChainEntry* ichain))0x80063f7cu)
#undef SkelAnime_InitFlex
#define SkelAnime_InitFlex ((void (*)(struct PlayState* play, SkelAnime* skelAnime, FlexSkeletonHeader* skeletonHeaderSeg, AnimationHeader* animation, Vec3s* jointTable, Vec3s* morphTable, s32 limbCount))0x8008c788u)
#undef CollisionCheck_SetInfo
#define CollisionCheck_SetInfo ((void (*)(CollisionCheckInfo* info, DamageTable* damageTable, CollisionCheckInfoInit* init))0x80050344u)
#undef ActorShape_Init
#define ActorShape_Init ((void (*)(ActorShape* actorShape, f32 yOffset, ActorShadowFunc shadowDraw, f32 shadowScale))0x8001ec20u)
#undef Math_Vec3f_Copy
#define Math_Vec3f_Copy ((void (*)(Vec3f* dest, Vec3f* src))0x80063cacu)
#undef Rand_ZeroFloat
#define Rand_ZeroFloat ((f32 (*)(f32 scale))0x80026d64u)
#undef Rand_CenteredFloat
#define Rand_CenteredFloat ((f32 (*)(f32 scale))0x80026d90u)
#undef Rand_ZeroOne
#define Rand_ZeroOne ((f32 (*)(void))0x800cdcccu)
#undef Rand_Next
#define Rand_Next ((u32 (*)(void))0x800cdc90u)
#undef Math_SinS
#define Math_SinS ((f32 (*)(s16 angle))0x800636c4u)
#undef Math_CosS
#define Math_CosS ((f32 (*)(s16 angle))0x80063684u)
#undef Rupees_ChangeBy
#define Rupees_ChangeBy ((void (*)(s16 rupeeChange))0x800721ccu)
#undef Actor_WorldDistXZToActor
#define Actor_WorldDistXZToActor ((f32 (*)(Actor* actorA, Actor* actorB))0x800214fcu)
#undef Actor_WorldDistXYZToActor
#define Actor_WorldDistXYZToActor ((f32 (*)(Actor* actorA, Actor* actorB))0x800214acu)
#undef Animation_MorphToLoop
#define Animation_MorphToLoop ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 morphFrames))0x8008d328u)
#undef SkelAnime_Update
#define SkelAnime_Update ((s32 (*)(SkelAnime* skelAnime))0x8008c9c0u)
#undef Animation_OnFrame
#define Animation_OnFrame ((s32 (*)(SkelAnime* skelAnime, f32 frame))0x8008d6a8u)
#undef Actor_WorldDistXZToPoint
#define Actor_WorldDistXZToPoint ((f32 (*)(Actor* actor, Vec3f* refPoint))0x80021528u)
#undef Actor_WorldYawTowardPoint
#define Actor_WorldYawTowardPoint ((s16 (*)(Actor* actor, Vec3f* refPoint))0x8002140cu)
#undef Math_SmoothStepToS
#define Math_SmoothStepToS ((s16 (*)(s16* pValue, s16 target, s16 scale, s16 step, s16 minStep))0x80064508u)
#undef Rand_S16Offset
#define Rand_S16Offset ((s16 (*)(s16 base, s16 range))0x80063bf0u)
#undef Actor_WorldPitchTowardActor
#define Actor_WorldPitchTowardActor ((s16 (*)(Actor* actorA, Actor* actorB))0x80021430u)
#undef Actor_IsFacingPlayer
#define Actor_IsFacingPlayer ((s32 (*)(Actor* actor, s16 angle))0x80021a28u)
#undef Animation_PlayOnce
#define Animation_PlayOnce ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation))0x8008d1c4u)
#undef Math_ScaledStepToS
#define Math_ScaledStepToS ((s32 (*)(s16* pValue, s16 target, s16 step))0x80063704u)
#undef Actor_WorldPitchTowardPoint
#define Actor_WorldPitchTowardPoint ((s16 (*)(Actor* actor, Vec3f* refPoint))0x80021488u)
#undef Animation_Change
#define Animation_Change ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 playSpeed, f32 startFrame, f32 endFrame, u8 mode, f32 morphFrames))0x8008d17cu)
#undef Math_StepToF
#define Math_StepToF ((s32 (*)(f32* pValue, f32 target, f32 step))0x8006385cu)
#undef Actor_WorldDistXYZToPoint
#define Actor_WorldDistXYZToPoint ((f32 (*)(Actor* actor, Vec3f* refPoint))0x800214d8u)
#undef Math_ApproachS
#define Math_ApproachS ((void (*)(s16* pValue, s16 target, s16 scale, s16 maxStep))0x80064624u)
#undef Actor_WorldYawTowardActor
#define Actor_WorldYawTowardActor ((s16 (*)(Actor* actorA, Actor* actorB))0x800213b4u)
#undef Math_Vec3f_Pitch
#define Math_Vec3f_Pitch ((s16 (*)(Vec3f* a, Vec3f* b))0x80063f34u)
#undef Actor_ApplyDamage
#define Actor_ApplyDamage ((u8 (*)(Actor* actor))0x80028390u)
#undef Enemy_StartFinishingBlow
#define Enemy_StartFinishingBlow ((void (*)(struct PlayState* play, Actor* actor))0x80025b4cu)
#undef Math_Vec3f_Sum
#define Math_Vec3f_Sum ((void (*)(Vec3f* l, Vec3f* r, Vec3f* dest))0x80063d10u)
#undef Actor_MoveWithoutGravity
#define Actor_MoveWithoutGravity ((void (*)(Actor* actor))0x800212b8u)
#undef Actor_MoveWithGravity
#define Actor_MoveWithGravity ((void (*)(Actor* actor))0x8002121cu)
#undef Actor_UpdateBgCheckInfo
#define Actor_UpdateBgCheckInfo ((void (*)(struct PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight, u32 updBgCheckInfoFlags))0x80021e6cu)
#undef Collider_UpdateSpheres
#define Collider_UpdateSpheres ((void (*)(s32 limb, ColliderJntSph* jntSph))0x80050ce4u)
#undef Matrix_ReplaceRotation
#define Matrix_ReplaceRotation ((void (*)(MtxF* mf))0x800abe54u)
#undef Matrix_Finalize
#define Matrix_Finalize ((Mtx* (*)(struct GraphicsContext* gfxCtx))0x800ab900u)
#undef Gfx_SetupDL25_Opa
#define Gfx_SetupDL25_Opa ((void (*)(GraphicsContext* gfxCtx))0x8007e298u)
#undef SkelAnime_DrawFlexOpa
#define SkelAnime_DrawFlexOpa ((void (*)(struct PlayState* play, void** skeleton, Vec3s* jointTable, s32 dListCount, GeneratedMmOverrideLimbDraw overrideLimbDraw, GeneratedMmPostLimbDraw postLimbDraw, struct Actor* actor))0x80089d8cu)
#undef func_800AE5A0
#define func_800AE5A0 ((void (*)(struct PlayState* play))0x8001a468u)
#undef Math_Vec3s_ToVec3f
#define Math_Vec3s_ToVec3f ((void (*)(Vec3f* dest, Vec3s* src))0x80063cccu)
#define ACTOR_FLAG_MINIMAP_ICON_ENABLED 0
#define ITEM00_RUPEE_HUGE ITEM00_RUPEE_ORANGE
#define ITEM00_BOMBS_B ITEM00_BOMBS_5
#define ITEM00_NO_DROP (-1)
static Actor* enemyTakkuriDropNative(PlayState* play, Vec3f* pos, s16 params) {
    return ((Actor * (*)(PlayState*, Vec3f*, s16))0x80013678)(play, pos, params);
}

#define FLAGS                                                                                                        \
    (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_HOOKSHOT_PULLS_ACTOR | ACTOR_FLAG_IGNORE_QUAKE | \
     ACTOR_FLAG_MINIMAP_ICON_ENABLED)

void EnThiefbird_Init(Actor* thisx, PlayState* play);
void EnThiefbird_Destroy(Actor* thisx, PlayState* play);
void EnThiefbird_Update(Actor* thisx, PlayState* play2);
void EnThiefbird_Draw(Actor* thisx, PlayState* play);

void func_80C11538(EnThiefbird* this);
void func_80C11590(EnThiefbird* this, PlayState* play);
void func_80C118E4(EnThiefbird* this);
void func_80C1193C(EnThiefbird* this, PlayState* play);
void func_80C11D14(EnThiefbird* this, PlayState* play);
void func_80C11DC0(EnThiefbird* this);
void func_80C11DF0(EnThiefbird* this, PlayState* play);
void func_80C1215C(EnThiefbird* this, PlayState* play);
void func_80C12308(EnThiefbird* this);
void func_80C12378(EnThiefbird* this, PlayState* play);
void func_80C1242C(EnThiefbird* this);
void func_80C124B0(EnThiefbird* this, PlayState* play);
void func_80C126A8(EnThiefbird* this);
void func_80C126D8(EnThiefbird* this, PlayState* play);
void func_80C12744(EnThiefbird* this);
void func_80C127F4(EnThiefbird* this, PlayState* play);

ActorProfile En_Thiefbird_Profile = {
    0xe91,
    ACTORCAT_ENEMY,
    FLAGS,
    0x1022,
    sizeof(EnThiefbird),
    EnThiefbird_Init,
    EnThiefbird_Destroy,
    EnThiefbird_Update,
    EnThiefbird_Draw,
};

static ColliderJntSphElementInit sJntSphElementsInit[3] = {
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0xF7CFFFFF, 0x00, 0x00 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_NONE | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_ON,
        },
        { 1, { { 0, 0, 0 }, 24 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0xF7CFFFFF, 0x00, 0x00 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_NONE | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_ON,
        },
        { 9, { { 900, -600, 0 }, 20 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0xF7CFFFFF, 0x00, 0x04 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_HARD,
            ACELEM_ON,
            OCELEM_ON,
        },
        { 12, { { 1200, 0, 0 }, 9 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COL_MATERIAL_HIT3,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sJntSphElementsInit),
    sJntSphElementsInit,
};

static CollisionCheckInfoInit sColChkInfoInit = { 24, 15, 30, 30 };

static DamageTable sDamageTable = { {
    DMG_ENTRY(0, 0x1), DMG_ENTRY(1, 0x0), DMG_ENTRY(1, 0x0), DMG_ENTRY(1, 0x0), DMG_ENTRY(1, 0x0), DMG_ENTRY(1, 0x0),
    DMG_ENTRY(0, 0x0), DMG_ENTRY(1, 0x0), DMG_ENTRY(1, 0x0), DMG_ENTRY(1, 0x0), DMG_ENTRY(1, 0x0), DMG_ENTRY(2, 0x2),
    DMG_ENTRY(2, 0x3), DMG_ENTRY(2, 0x4), DMG_ENTRY(1, 0x0), DMG_ENTRY(1, 0x0), DMG_ENTRY(1, 0x0), DMG_ENTRY(2, 0x0),
    DMG_ENTRY(0, 0x1), DMG_ENTRY(0, 0x5), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(1, 0x0), DMG_ENTRY(1, 0x0),
    DMG_ENTRY(1, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(0, 0x0), DMG_ENTRY(1, 0x0),
} };

static s16 D_80C13664[] = {
    ITEM00_ARROWS_10, ITEM00_BOMBS_B, ITEM00_RUPEE_GREEN, ITEM00_RUPEE_BLUE, ITEM00_RUPEE_RED, ITEM00_RUPEE_PURPLE,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(cullingVolumeDistance, 3000, ICHAIN_CONTINUE),
    ICHAIN_F32(cullingVolumeScale, 1000, ICHAIN_CONTINUE),
    ICHAIN_S8(hintId, TATL_HINT_ID_TAKKURI, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 500, ICHAIN_STOP),
};

Vec3f D_80C13920;
s32 D_80C1392C;

void EnThiefbird_Init(Actor* thisx, PlayState* play) {
    if (thisx->params != 0) {
        Actor_Kill(thisx);
        return;
    }
    EnThiefbird* this = (EnThiefbird*)thisx;
    s32 i;
    ColliderJntSphElementDim* dim;

    Actor_ProcessInitChain(&this->actor, sInitChain);
    SkelAnime_InitFlex(play, &this->skelAnime, &gTakkuriSkel, &gTakkuriFlyAnim, this->jointTable, this->morphTable,
                       TAKKURI_LIMB_MAX);
    Collider_InitAndSetJntSph(play, &this->collider, &this->actor, &sJntSphInit, this->colliderElements);

    for (i = 0; i < ARRAY_COUNT(this->colliderElements); i++) {
        dim = &this->collider.elements[i].dim;
        dim->worldSphere.radius = dim->modelSphere.radius;
        dim->worldSphere.center.x = this->actor.world.pos.x;
        dim->worldSphere.center.y = this->actor.world.pos.y;
        dim->worldSphere.center.z = this->actor.world.pos.z;
    }

    CollisionCheck_SetInfo(&this->actor.colChkInfo, &sDamageTable, &sColChkInfoInit);
    ActorShape_Init(&this->actor.shape, 1500.0f, ActorShadow_DrawCircle, 35.0f);
    if (this->actor.params == 1) {
        D_80C1392C = true;
        Math_Vec3f_Copy(&D_80C13920, &this->actor.world.pos);
        Actor_Kill(&this->actor);
        return;
    }

    func_80C11538(this);
}

void EnThiefbird_Destroy(Actor* thisx, PlayState* play) {
    EnThiefbird* this = (EnThiefbird*)thisx;

    Collider_DestroyJntSph(play, &this->collider);
}

void func_80C10984(EnThiefbird* this, s32 arg1) {
    s32 i;
    EnThiefbirdUnkStruct* ptr = &this->unk_3F0[0];

    for (i = 0; i < ARRAY_COUNT(this->unk_3F0); i++, ptr++) {
        if (ptr->unk_22 == 0) {
            ptr->unk_22 = (s32)Rand_ZeroFloat(20.0f) + 40;
            ptr->unk_00.x = Rand_CenteredFloat(30.0f) + this->actor.focus.pos.x;
            ptr->unk_00.y = Rand_CenteredFloat(30.0f) + this->actor.focus.pos.y;
            ptr->unk_00.z = Rand_CenteredFloat(30.0f) + this->actor.focus.pos.z;
            ptr->unk_0C.x = Rand_CenteredFloat(5.0f);
            ptr->unk_0C.y = Rand_ZeroOne() + 2.0f;
            ptr->unk_0C.z = Rand_CenteredFloat(5.0f);
            ptr->unk_1C = Rand_ZeroFloat(1000.0f);
            ptr->unk_18 = (Rand_ZeroFloat(20.0f) + 40.0f) * 0.0001f;
            ptr->unk_1E = (s32)Rand_Next() >> 0x10;
            arg1--;
            if (arg1 == 0) {
                break;
            }
        }
    }
}

void func_80C10DE8(s32 items[], s32 count, s32 item) {
    s32 i;
    s32 idx = 0;

    for (i = 0; i < count; i++) {
        s32 j = 0;

        while (items[idx] != -1) {
            idx = (idx + 5) % 8;
            if (j == 8) {
                break;
            }
            j++;
        }

        items[idx] = item;
        idx = (idx + 5) % 8;
    }
}

s32 func_80C10E98(PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 phi_s0_2;
    s32 phi_s2;
    s32 spB0;
    s32 spAC;
    s32 spA8;
    s16 phi_s3 = 0;
    s32 spA0;
    s32 sp98;
    s32 i;
    s32 dropItem00Ids[8];
    s32 sp5C;

    for (i = 0; i < ARRAY_COUNT(dropItem00Ids); i++) {
        dropItem00Ids[i] = ITEM00_NO_DROP;
    }

    if (gSave.info.inventory.ammo[ITS_OOT_BOMBS] >= 5) {
        spB0 = 1;
        dropItem00Ids[1] = ITEM00_BOMBS_B;
        if (1) {
        }
    } else {
        spB0 = 0;
    }

    if (gSave.info.inventory.ammo[ITS_OOT_BOW] >= 10) {
        spAC = 1;
        dropItem00Ids[5] = ITEM00_ARROWS_10;
    } else {
        spAC = 0;
    }

    sp98 = (gSave.info.playerData.rupees / 4) * 3;
    phi_s0_2 = sp98 / 50;
    sp5C = (-spB0 - spAC);
    sp5C += 8;
    if (sp5C < phi_s0_2) {
        phi_s0_2 = sp5C;
    }
    phi_s2 = phi_s0_2;

    for (i = 0; i < phi_s2; i++) {
        if (Rand_ZeroOne() < 0.5f) {
            phi_s0_2--;
        }
    }

    i = sp5C - phi_s0_2;

    sp5C = phi_s0_2 * 50;
    sp98 -= sp5C;

    func_80C10DE8(dropItem00Ids, phi_s0_2, ITEM00_RUPEE_PURPLE);
    spA0 = sp98 / 20;
    if (i < spA0) {
        spA0 = i;
    }
    i -= spA0;
    sp98 -= spA0 * 20;

    func_80C10DE8(dropItem00Ids, spA0, ITEM00_RUPEE_RED);
    phi_s2 = sp98 / 5;
    if (i < phi_s2) {
        phi_s2 = i;
    }
    i -= phi_s2;
    sp98 -= phi_s2 * 5;

    func_80C10DE8(dropItem00Ids, phi_s2, ITEM00_RUPEE_BLUE);
    if (i < sp98) {
        spA8 = i;
    } else {
        spA8 = sp98;
    }

    func_80C10DE8(dropItem00Ids, spA8, ITEM00_RUPEE_GREEN);
    if ((spB0 + spAC + phi_s0_2 + spA0 + phi_s2 + spA8) == 0) {
        return false;
    } else {
        Vec3f sp64;

        for (i = 0; i < ARRAY_COUNT(dropItem00Ids); i++) {
            sp64.x = (Math_SinS(phi_s3) * 40.0f) + player->actor.world.pos.x;
            sp64.y = player->actor.world.pos.y + 20.0f;
            sp64.z = (Math_CosS(phi_s3) * 40.0f) + player->actor.world.pos.z;
            if (dropItem00Ids[i] != ITEM00_NO_DROP) {
                Actor* temp_s1_5 = enemyTakkuriDropNative(play, &sp64, dropItem00Ids[i]);

                if (temp_s1_5 != NULL) {
                    temp_s1_5->velocity.y = Rand_ZeroFloat(3.0f) + 6.0f;
                    temp_s1_5->speed = Rand_ZeroFloat(3.0f) + 3.0f;
                    temp_s1_5->world.rot.y = phi_s3;
                }
                phi_s3 += (s16)(0x10000 / (spB0 + spAC + phi_s0_2 + spA0 + phi_s2 + spA8));
            }
        }
    }

    gSave.info.inventory.ammo[ITS_OOT_BOMBS] -= spB0 * 5;
    gSave.info.inventory.ammo[ITS_OOT_BOW] -= spAC * 10;
    Rupees_ChangeBy(-((phi_s0_2 * 50) + (spA0 * 20) + (phi_s2 * 5) + spA8));
    return true;
}

void func_80C11338(EnThiefbird* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    f32 nearest = 5000.f;
    this->unk_3EC = NULL;

    for (Actor* actor = play->actorCtx.actors[ACTORCAT_MISC].first; actor; actor = actor->next) {
        if (actor->id != ACTOR_EN_ITEM00 || !actor->update)
            continue;
        EnItem00* item = (EnItem00*)actor;
        if (item->isExtended || item->timer <= 0 || Actor_WorldDistXZToActor(&player->actor, actor) <= 10.f)
            continue;
        for (int i = 0; i < ARRAY_COUNT(D_80C13664); ++i) {
            if (actor->params != D_80C13664[i])
                continue;
            f32 distance = Actor_WorldDistXYZToActor(&this->actor, actor);
            if (distance < nearest) {
                nearest = distance;
                this->unk_3EC = item;
            }
            break;
        }
    }
}

void func_80C11454(EnThiefbird* this) {
    this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX;
    this->drawDmgEffScale = 0.5f;
    this->drawDmgEffFrozenSteamScale = 0.75f;
    this->drawDmgEffAlpha = 1.0f;
    this->actor.flags &= ~ACTOR_FLAG_HOOKSHOT_PULLS_ACTOR;
    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 80);
}

void func_80C114C0(EnThiefbird* this, PlayState* play) {
    if (this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX) {
        this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FIRE;
        this->drawDmgEffAlpha = 0.0f;
        Actor_SpawnIceEffects(play, &this->actor, this->bodyPartsPos, EN_THIEFBIRD_BODYPART_MAX, 2, 0.2f, 0.2f);
        this->actor.flags |= ACTOR_FLAG_HOOKSHOT_PULLS_ACTOR;
    }
}

void func_80C11538(EnThiefbird* this) {
    Animation_MorphToLoop(&this->skelAnime, &gTakkuriFlyAnim, -4.0f);
    this->unk_18E = 60;
    this->collider.base.acFlags |= AC_ON;
    this->actionFunc = func_80C11590;
}

void func_80C11590(EnThiefbird* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 onAnimFirstFrame;

    SkelAnime_Update(&this->skelAnime);
    onAnimFirstFrame = Animation_OnFrame(&this->skelAnime, 0.0f);
    this->actor.speed = (Rand_ZeroOne() * 1.5f) + 3.0f;

    if (this->actor.bgCheckFlags & BGCHECKFLAG_WALL) {
        this->unk_192 = this->actor.wallYaw;
    } else if (Actor_WorldDistXZToPoint(&this->actor, &this->actor.home.pos) > 300.0f) {
        this->unk_192 = Actor_WorldYawTowardPoint(&this->actor, &this->actor.home.pos);
    }

    if (!Math_SmoothStepToS(&this->actor.shape.rot.y, this->unk_192, 5, 0x300, 0x10) && onAnimFirstFrame &&
        (Rand_ZeroOne() < 0.1f)) {
        s16 yaw = Actor_WorldYawTowardPoint(&this->actor, &this->actor.home.pos) - this->actor.shape.rot.y;

        if (yaw > 0) {
            this->unk_192 += Rand_S16Offset(4096, 4096);
        } else {
            this->unk_192 -= Rand_S16Offset(4096, 4096);
        }
        Actor_PlaySfx(&this->actor, NA_SE_EN_THIEFBIRD_VOICE);
    }

    if ((this->actor.depthInWater > -40.0f) || (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
        this->unk_190 = -4096;
    } else if (this->actor.world.pos.y < (this->actor.home.pos.y - 75.0f)) {
        this->unk_190 = -Rand_S16Offset(2048, 2048);
    } else if ((this->actor.home.pos.y + 75.0f) < this->actor.world.pos.y) {
        this->unk_190 = Rand_S16Offset(2048, 2048);
    }

    if (!Math_SmoothStepToS(&this->actor.shape.rot.x, this->unk_190, 10, 0x100, 8) && onAnimFirstFrame &&
        (Rand_ZeroOne() < 0.1f)) {
        if (this->actor.home.pos.y < this->actor.world.pos.y) {
            this->unk_190 -= Rand_S16Offset(0x400, 0x400);
        } else {
            this->unk_190 += Rand_S16Offset(0x400, 0x400);
        }

        this->unk_190 = CLAMP(this->unk_190, -4096, 4096);
    }

    if (this->unk_18E != 0) {
        this->unk_18E--;
    }

    if ((this->unk_18E == 0) && (this->actor.xzDistToPlayer < 300.0f) &&
        !(this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) && !(player->stateFlags1 & PLAYER_STATE1_800000) &&
        (Player_GetMask(play) != PLAYER_MASK_STONE) && (this->actor.depthInWater < -40.0f)) {
        func_80C118E4(this);
    }
}

void func_80C118E4(EnThiefbird* this) {
    Animation_MorphToLoop(&this->skelAnime, &gTakkuriAttackAnim, -10.0f);
    this->unk_18E = 300;
    this->collider.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
    this->actionFunc = func_80C1193C;
    this->actor.speed = 5.0f;
}

void func_80C1193C(EnThiefbird* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 pitch;

    SkelAnime_Update(&this->skelAnime);
    if (Animation_OnFrame(&this->skelAnime, 1.0f)) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_KAICHO_FLUTTER);
    }

    if (this->unk_18E != 0) {
        this->unk_18E--;
    }

    pitch = Actor_WorldPitchTowardPoint(&this->actor, &player->actor.focus.pos);
    pitch = CLAMP(pitch, -0x2800, 0x2800);
    Math_SmoothStepToS(&this->actor.shape.rot.x, pitch, 4, 0x800, 0x80);
    if (this->actor.bgCheckFlags & BGCHECKFLAG_WALL) {
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.wallYaw, 6, 0x1000, 0x100);
    } else if (Actor_IsFacingPlayer(&this->actor, 0x3C00) || (this->actor.xzDistToPlayer > 120.0f)) {
        s16 rot = BINANG_ROT180(this->actor.yawTowardsPlayer - player->actor.shape.rot.y);

        if (rot > 0x4000) {
            rot = this->actor.yawTowardsPlayer + 0x3000;
        } else if (rot < -0x4000) {
            rot = this->actor.yawTowardsPlayer - 0x3000;
        } else {
            rot = this->actor.yawTowardsPlayer;
        }
        Math_SmoothStepToS(&this->actor.shape.rot.y, rot, 4, 0x1000, 0x100);
    }

    if ((this->unk_18E == 0) || (player->stateFlags1 & PLAYER_STATE1_800000) ||
        (Player_GetMask(play) == PLAYER_MASK_STONE) || (this->collider.base.atFlags & AT_HIT) ||
        (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) || (this->actor.depthInWater > -40.0f)) {
        if (this->collider.base.atFlags & AT_HIT) {
            this->collider.base.atFlags &= ~AT_HIT;
            Actor_PlaySfx(&this->actor, NA_SE_EN_THIEFBIRD_VOICE);
            if (!(this->collider.base.atFlags & AT_BOUNCED)) {
                if (false) {
                    func_80C1242C(this);
                } else if (func_80C10E98(play)) {
                    func_80C11338(this, play);
                    func_80C12744(this);
                } else {
                    func_80C11538(this);
                }
            } else {
                func_80C12308(this);
            }
        } else {
            func_80C11538(this);
        }
    }
}

void func_80C11C60(EnThiefbird* this) {
    this->actor.speed = 0.0f;
    this->actor.velocity.y = 0.0f;
    Animation_PlayOnce(&this->skelAnime, &gTakkuriDeathAnim);
    this->actor.bgCheckFlags &= ~BGCHECKFLAG_GROUND;
    this->actor.shape.rot.x = 0;
    this->unk_18E = 40;
    this->actor.velocity.y = 0.0f;
    Actor_PlaySfx(&this->actor, NA_SE_EN_THIEFBIRD_DEAD);
    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 40);
    this->collider.base.acFlags &= ~AC_ON;
    this->actor.flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
    this->unk_192 = 0x1C00;
    this->actionFunc = func_80C11D14;
}

void func_80C11D14(EnThiefbird* this, PlayState* play) {
    this->actor.colorFilterTimer = 40;
    if (this->unk_18E != 0) {
        this->unk_18E--;
    }

    if (this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX) {
        if (this->unk_18E >= 38) {
            return;
        }
        func_80C114C0(this, play);
        this->actor.speed = 4.0f;
    }

    if (this->unk_18E < 20) {
        Math_ScaledStepToS(&this->unk_192, 0, 0x200);
    }

    this->actor.shape.rot.y += this->unk_192;
    if (this->unk_18E == 0) {
        func_80C11DC0(this);
    }
}

void func_80C11DC0(EnThiefbird* this) {
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    this->actionFunc = func_80C11DF0;
    this->actor.gravity = -0.5f;
}

void func_80C11DF0(EnThiefbird* this, PlayState* play) {
    s32 i;
    s32 j;

    if (Math_ScaledStepToS(&this->actor.shape.rot.x, -0x8000, 0x800)) {
        Math_ScaledStepToS(&this->unk_192, 0x1C00, 0x200);
        this->actor.shape.rot.y += this->unk_192;
    }

    if ((this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) || (this->actor.floorHeight == BGCHECK_Y_MIN)) {
        for (i = 0; i < EN_THIEFBIRD_BODYPART_MAX; i++) {
            func_800B3030(play, &this->bodyPartsPos[i], &gZeroVec3f, &gZeroVec3f, 0x8C, 0, 0);
        }

        SoundSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 11, NA_SE_EN_EXTINCT);
        enemyTakkuriDropNative(play, &this->actor.world.pos, ITEM00_RUPEE_HUGE);

        for (i = 0; i < ARRAY_COUNT(D_80C13664); i++) {
            for (j = 0; j < this->unk_196[i]; j++) {
                enemyTakkuriDropNative(play, &this->actor.world.pos, D_80C13664[i]);
            }
        }

        Actor_Kill(&this->actor);
    }
}

void func_80C11F6C(EnThiefbird* this, PlayState* play) {
    Animation_MorphToLoop(&this->skelAnime, &gTakkuriFlyWithItemAnim, -4.0f);
    func_80C10984(this, 15);
    if (this->actor.colChkInfo.damageEffect != 3) {
        this->actor.speed = 4.0f;
    } else {
        this->actor.speed = 0.0f;
    }

    if (this->actor.colChkInfo.damageEffect == 5) {
        Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_BLUE, 255, COLORFILTER_BUFFLAG_OPA, 40);
        Actor_PlaySfx(&this->actor, NA_SE_EN_COMMON_FREEZE);
    } else if (this->actor.colChkInfo.damageEffect == 1) {
        Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_BLUE, 255, COLORFILTER_BUFFLAG_OPA, 40);
        Actor_PlaySfx(&this->actor, NA_SE_EN_COMMON_FREEZE);
    } else {
        Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 40);
        Actor_PlaySfx(&this->actor, NA_SE_EN_THIEFBIRD_DAMAGE);
    }

    this->collider.base.acFlags &= ~AC_ON;
    if (this->unk_3E8 == 0) {
        this->unk_190 = -0x1000;
        this->unk_192 = BINANG_ROT180(this->actor.yawTowardsPlayer);
    } else {
        this->unk_190 = Actor_WorldPitchTowardPoint(&this->actor, &D_80C13920);
    }

    this->unk_18E = 40;

    if (Rand_ZeroOne() < 0.9f) {
        enemyTakkuriDropNative(play, &this->actor.focus.pos, ITEM00_RUPEE_GREEN);
    }

    if (Rand_ZeroOne() < 0.6f) {
        enemyTakkuriDropNative(play, &this->actor.focus.pos, ITEM00_RUPEE_GREEN);
    }

    if (Rand_ZeroOne() < 0.3f) {
        enemyTakkuriDropNative(play, &this->actor.focus.pos, ITEM00_RUPEE_GREEN);
    }
    this->actionFunc = func_80C1215C;
}

void func_80C1215C(EnThiefbird* this, PlayState* play) {
    f32 sp2C;

    if (this->unk_18E != 0) {
        this->unk_18E--;
    }

    if (this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX) {
        if (this->unk_18E >= 38) {
            return;
        }
        func_80C114C0(this, play);
        this->actor.speed = 4.0f;
    }

    SkelAnime_Update(&this->skelAnime);
    if (this->actor.bgCheckFlags & BGCHECKFLAG_WALL) {
        this->unk_192 = this->actor.wallYaw;
    } else if (this->unk_3E8 == 0) {
        this->unk_192 = BINANG_ROT180(this->actor.yawTowardsPlayer);
    } else {
        this->unk_192 = Actor_WorldYawTowardPoint(&this->actor, &D_80C13920);
        this->unk_190 = Actor_WorldPitchTowardPoint(&this->actor, &D_80C13920);
    }

    Math_SmoothStepToS(&this->actor.shape.rot.y, this->unk_192, 6, 0x1000, 0x100);
    Math_SmoothStepToS(&this->actor.shape.rot.x, this->unk_190, 5, 0x100, 0x10);
    sp2C = (this->unk_18E * 153.6f) + 0x800;
    this->actor.shape.rot.z = Math_SinS(this->unk_18E * 0x1999) * sp2C;
    if (this->unk_18E == 0) {
        if (this->unk_3E8 != 0) {
            func_80C1242C(this);
        } else if (this->unk_3EC != 0) {
            func_80C12744(this);
        } else {
            func_80C11538(this);
        }
    }
}

void func_80C12308(EnThiefbird* this) {
    Animation_MorphToLoop(&this->skelAnime, &gTakkuriFlyWithItemAnim, -4.0f);
    func_80C10984(this, 15);
    this->unk_190 = -0x1000;
    this->unk_192 = BINANG_ROT180(this->actor.yawTowardsPlayer);
    this->unk_18E = 40;
    this->actionFunc = func_80C12378;
}

void func_80C12378(EnThiefbird* this, PlayState* play) {
    if (this->unk_18E != 0) {
        this->unk_18E--;
    }

    SkelAnime_Update(&this->skelAnime);
    if (this->actor.bgCheckFlags & BGCHECKFLAG_WALL) {
        this->unk_192 = this->actor.wallYaw;
    } else {
        this->unk_192 = BINANG_ROT180(this->actor.yawTowardsPlayer);
    }

    Math_SmoothStepToS(&this->actor.shape.rot.y, this->unk_192, 6, 0x1000, 0x100);
    Math_SmoothStepToS(&this->actor.shape.rot.x, this->unk_190, 5, 0x100, 0x10);

    if (this->unk_18E == 0) {
        func_80C11538(this);
    }
}

void func_80C1242C(EnThiefbird* this) {
    func_80C11538(this);
}

void func_80C124B0(EnThiefbird* this, PlayState* play) {
    func_80C11590(this, play);
}

void func_80C126A8(EnThiefbird* this) {
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    this->collider.base.acFlags &= ~AC_ON;
    this->actionFunc = func_80C126D8;
}

void func_80C126D8(EnThiefbird* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);
    Math_ApproachS(&this->actor.shape.rot.x, 0x3000, 6, 0x1000);
    if (this->actor.yDistanceFromLink > 100.0f) {
        Actor_Kill(&this->actor);
    }
}

void func_80C12744(EnThiefbird* this) {
    Animation_MorphToLoop(&this->skelAnime, &gTakkuriFlyAnim, -4.0f);
    Animation_Change(&this->skelAnime, &gTakkuriFlyAnim, 1.0f, 0.0f, 0.0f, ANIMMODE_LOOP_INTERP, -4.0f);
    this->unk_190 = 0;
    this->collider.base.acFlags |= AC_ON;
    this->actor.flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
    this->actionFunc = func_80C127F4;
    this->actor.speed = 4.0f;
    this->skelAnime.playSpeed = 3.0f;
}

void func_80C127F4(EnThiefbird* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 temp_v0;
    f32 temp_f0;

    SkelAnime_Update(&this->skelAnime);
    if (this->unk_3EC != NULL) {
        Actor* item = play->actorCtx.actors[ACTORCAT_MISC].first;
        while (item != NULL && item != (Actor*)this->unk_3EC)
            item = item->next;
        if (!item || item->id != ACTOR_EN_ITEM00 || !item->update || this->unk_3EC->isExtended ||
            this->unk_3EC->timer <= 0 || Actor_WorldDistXZToActor(&player->actor, item) <= 10.f) {
            this->unk_3EC = NULL;
        }
    }

    if (this->unk_190 == -0x3800) {
        if (Math_ScaledStepToS(&this->unk_194, this->unk_190, 0xE00)) {
            this->unk_190 = 0;
        }
    } else {
        Math_ScaledStepToS(&this->unk_194, 0, 0x200);
    }

    if (this->unk_3EC == NULL) {
        func_80C11338(this, play);
    }

    if (this->unk_3EC != NULL) {
        if (this->actor.bgCheckFlags & BGCHECKFLAG_WALL) {
            Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.wallYaw, 3, 0x2000, 0x100);
        } else {
            Math_SmoothStepToS(&this->actor.shape.rot.y, Actor_WorldYawTowardActor(&this->actor, &this->unk_3EC->actor),
                               3, 0x2000, 0x100);
        }
        temp_v0 = Math_Vec3f_Pitch(&this->bodyPartsPos[EN_THIEFBIRD_BODYPART_9], &this->unk_3EC->actor.world.pos);
        temp_v0 = CLAMP(temp_v0, -0x3000, 0x3000);
        Math_SmoothStepToS(&this->actor.shape.rot.x, temp_v0, 4, 0x800, 0x80);
        temp_f0 = Actor_WorldDistXYZToPoint(&this->unk_3EC->actor, &this->bodyPartsPos[EN_THIEFBIRD_BODYPART_9]);
        this->actor.speed = (0.02f * temp_f0) + 2.0f;
        this->actor.speed = CLAMP_MAX(this->actor.speed, 4.0f);
        if ((this->unk_3EC->actor.speed <= 0.0f) && (temp_f0 < 40.0f)) {
            s32 i;

            this->unk_3EC->timer = 0;
            for (i = 0; i < ARRAY_COUNT(D_80C13664); i++) {
                if (this->unk_3EC->actor.params == D_80C13664[i]) {
                    this->unk_196[i]++;
                    break;
                }
            }
            this->unk_3EC = NULL;
            this->unk_194 = 0x100;
            this->unk_190 = -0x3800;
        }
    } else {
        this->actor.speed = 4.0f;
        if (this->actor.bgCheckFlags & BGCHECKFLAG_WALL) {
            Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.wallYaw, 6, 0x1000, 0x100);
        } else {
            Math_SmoothStepToS(&this->actor.shape.rot.y, BINANG_ROT180(this->actor.yawTowardsPlayer), 6, 0x1000, 0x100);
        }

        Math_SmoothStepToS(&this->actor.shape.rot.x, -0x800, 4, 0x800, 0x80);
        if (this->unk_194 == 0) {
            this->actor.flags &= ~ACTOR_FLAG_UPDATE_CULLING_DISABLED;
            func_80C11538(this);
        }
    }
}

void func_80C12B1C(EnThiefbird* this, PlayState* play) {
    ColliderJntSphElement* jntSphElem;
    s32 i;

    if (this->collider.base.acFlags & AC_HIT) {
        this->collider.base.acFlags &= ~AC_HIT;
        this->collider.base.atFlags &= ~AT_HIT;
        Actor_SetDropFlag(&this->actor, &this->collider.elements[0].base);
        func_80C114C0(this, play);
        this->unk_194 = 0;

        for (i = 0; i < ARRAY_COUNT(this->colliderElements); i++) {
            if (this->collider.elements[i].base.acElemFlags & ACELEM_HIT) {
                break;
            }
        }

        if (this->actor.colChkInfo.damageEffect == 3) {
            func_80C11454(this);
        } else if (this->actor.colChkInfo.damageEffect == 4) {
            this->drawDmgEffType = ACTOR_DRAW_DMGEFF_LIGHT_ORBS;
            this->drawDmgEffScale = 0.5f;
            this->drawDmgEffAlpha = 4.0f;
            if (i != ARRAY_COUNT(this->colliderElements)) {
                jntSphElem = &this->collider.elements[i];
                Actor_Spawn(&play->actorCtx, play, ACTOR_EN_CLEAR_TAG, jntSphElem->base.acDmgInfo.hitPos.x,
                            jntSphElem->base.acDmgInfo.hitPos.y, jntSphElem->base.acDmgInfo.hitPos.z, 0, 0, 0,
                            CLEAR_TAG_PARAMS(CLEAR_TAG_LARGE_LIGHT_RAYS));
            }
        } else if (this->actor.colChkInfo.damageEffect == 2) {
            this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FIRE;
            this->drawDmgEffScale = 0.5f;
            this->drawDmgEffAlpha = 4.0f;
        } else if (this->actor.colChkInfo.damageEffect == 5) {
            this->drawDmgEffType = ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_SMALL;
            this->drawDmgEffScale = 0.5f;
            this->drawDmgEffAlpha = 2.0f;
        }

        if (this->unk_3E8 != 0) {
            this->actor.colChkInfo.damage = 0;
        }

        if (!Actor_ApplyDamage(&this->actor)) {
            this->actor.colChkInfo.health = 0;
            Enemy_StartFinishingBlow(play, &this->actor);
            func_80C10984(this, 40);
            func_80C11C60(this);
        } else {
            func_80C11F6C(this, play);
        }
    }
}

void func_80C12D00(EnThiefbird* this) {
    static Vec3f D_80C1368C = { 0.0f, -0.15f, 0.0f };
    s32 i;
    f32 phi_f20;
    EnThiefbirdUnkStruct* ptr = &this->unk_3F0[0];

    for (i = 0; i < ARRAY_COUNT(this->unk_3F0); i++, ptr++) {
        if (ptr->unk_22 != 0) {
            ptr->unk_22--;
            Math_Vec3f_Sum(&ptr->unk_00, &ptr->unk_0C, &ptr->unk_00);
            Math_Vec3f_Sum(&ptr->unk_0C, &D_80C1368C, &ptr->unk_0C);
            ptr->unk_1C++;
            Math_StepToF(&ptr->unk_0C.x, 0.0f, 0.05f);
            Math_StepToF(&ptr->unk_0C.z, 0.0f, 0.05f);
            if (ptr->unk_0C.y < -0.5f) {
                ptr->unk_0C.y = -0.5f;
            }

            if ((i % 2) != 0) {
                phi_f20 = -1.0f;
            } else {
                phi_f20 = 1.0f;
            }

            ptr->unk_20 = Math_SinS(ptr->unk_1C * 0x7D0) * 0x2000;
            ptr->unk_1E += TRUNCF_BINANG(0x666 * fabsf(Math_SinS(ptr->unk_1C * 0xBB8)) * phi_f20);
        }
    }
}

void EnThiefbird_Update(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    EnThiefbird* this = (EnThiefbird*)thisx;

    func_80C12B1C(this, play);
    this->actionFunc(this, play);
    this->actor.world.rot.y = this->actor.shape.rot.y;
    this->actor.world.rot.x = -this->actor.shape.rot.x;
    if (this->actor.colChkInfo.health != 0) {
        Actor_MoveWithoutGravity(&this->actor);
    } else {
        Actor_MoveWithGravity(&this->actor);
    }

    Actor_UpdateBgCheckInfo(play, &this->actor, 25.0f, 25.0f, 50.0f,
                            UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_2 | UPDBGCHECKINFO_FLAG_4);
    if (this->actionFunc == func_80C1193C) {
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
    }

    if (this->collider.base.acFlags & AC_ON) {
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    }

    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
    if (this->drawDmgEffAlpha > 0.0f) {
        if (this->drawDmgEffType != ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX) {
            Math_StepToF(&this->drawDmgEffAlpha, 0.0f, 0.05f);
            this->drawDmgEffScale = (this->drawDmgEffAlpha + 1.0f) * 0.25f;
            this->drawDmgEffScale = CLAMP_MAX(this->drawDmgEffScale, 0.5f);
        } else if (!Math_StepToF(&this->drawDmgEffFrozenSteamScale, 0.5f, 0.0125f)) {
            Actor_PlaySfx_Flagged(&this->actor, NA_SE_EV_ICE_FREEZE - SFX_FLAG);
        }
    }

    func_80C12D00(this);

    if (((this->skelAnime.animation == &gTakkuriFlyAnim) && Animation_OnFrame(&this->skelAnime, 13.0f)) ||
        ((this->skelAnime.animation == &gTakkuriFlyWithItemAnim) && Animation_OnFrame(&this->skelAnime, 1.0f))) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_KAICHO_FLUTTER);
    }
}

s32 EnThiefbird_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx) {
    EnThiefbird* this = (EnThiefbird*)thisx;

    if ((limbIndex == TAKKURI_LIMB_RIGHT_EAR) || (limbIndex == TAKKURI_LIMB_LEFT_EAR)) {
        this->unk_3E4 = *dList;
        *dList = NULL;
    } else if (limbIndex == TAKKURI_LIMB_STOLEN_ITEM) {
        *dList = NULL;
    } else if (limbIndex == TAKKURI_LIMB_NECK) {
        rot->z += this->unk_194;
    }

    return false;
}

static s8 sLimbToBodyParts[TAKKURI_LIMB_MAX] = {
    BODYPART_NONE,           EN_THIEFBIRD_BODYPART_0, BODYPART_NONE,           EN_THIEFBIRD_BODYPART_1,
    EN_THIEFBIRD_BODYPART_3, BODYPART_NONE,           EN_THIEFBIRD_BODYPART_2, EN_THIEFBIRD_BODYPART_5,
    BODYPART_NONE,           BODYPART_NONE,           EN_THIEFBIRD_BODYPART_7, EN_THIEFBIRD_BODYPART_8,
    EN_THIEFBIRD_BODYPART_9, BODYPART_NONE,           BODYPART_NONE,           EN_THIEFBIRD_BODYPART_10,
    BODYPART_NONE,
};

void EnThiefbird_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx) {
    EnThiefbird* this = (EnThiefbird*)thisx;
    Gfx* gfx;
    s8 bodyPartIndex;

    Collider_UpdateSpheres(limbIndex, &this->collider);
    if ((limbIndex == TAKKURI_LIMB_RIGHT_EAR) || (limbIndex == TAKKURI_LIMB_LEFT_EAR)) {
        OPEN_DISPS(play->state.gfxCtx);

        gfx = POLY_OPA_DISP;
        Matrix_ReplaceRotation(&play->billboardMtxF);
        MATRIX_FINALIZE_AND_LOAD(&gfx[0], play->state.gfxCtx);
        gSPDisplayList(&gfx[1], this->unk_3E4);
        POLY_OPA_DISP = &gfx[2];

        CLOSE_DISPS();
    } else if (limbIndex == TAKKURI_LIMB_STOLEN_ITEM) {
        if (this->unk_3E8 != NULL) {
            OPEN_DISPS(play->state.gfxCtx);

            if (this->unk_3E8 == gTakkuriStolenBottleDL) {
                gfx = POLY_XLU_DISP;
            } else {
                gfx = POLY_OPA_DISP;
            }

            MATRIX_FINALIZE_AND_LOAD(&gfx[0], play->state.gfxCtx);
            gSPDisplayList(&gfx[1], this->unk_3E8);

            if (this->unk_3E8 == gTakkuriStolenBottleDL) {
                POLY_XLU_DISP = &gfx[2];
            } else {
                POLY_OPA_DISP = &gfx[2];
            }

            CLOSE_DISPS();
        }
    }

    bodyPartIndex = sLimbToBodyParts[limbIndex];
    if (bodyPartIndex != BODYPART_NONE) {
        if (bodyPartIndex == EN_THIEFBIRD_BODYPART_9) {
            Matrix_MultVecX(1000.0f, &this->bodyPartsPos[bodyPartIndex]);
        } else {
            Matrix_MultZero(&this->bodyPartsPos[bodyPartIndex]);
            if ((bodyPartIndex == EN_THIEFBIRD_BODYPART_3) || (bodyPartIndex == EN_THIEFBIRD_BODYPART_5)) {
                Matrix_MultVecX(2000.0f, &this->bodyPartsPos[bodyPartIndex + 1]);
            }
        }
    }
}

void func_80C13354(EnThiefbird* this, PlayState* play2) {
    PlayState* play = play2;
    s32 i;
    Gfx* gfx;
    EnThiefbirdUnkStruct* ptr = &this->unk_3F0[0];

    OPEN_DISPS(play->state.gfxCtx);

    gfx = POLY_OPA_DISP;
    Gfx_SetupDL(&gfx[0], 25);
    gSPDisplayList(&gfx[1], gTakkuriFeatherMaterialDL);
    gfx = &gfx[2];

    for (i = 0; i < ARRAY_COUNT(this->unk_3F0); i++, ptr++) {
        if (ptr->unk_22 != 0) {
            Matrix_Translate(ptr->unk_00.x, ptr->unk_00.y, ptr->unk_00.z, MTXMODE_NEW);
            Matrix_ReplaceRotation(&play->billboardMtxF);
            Matrix_RotateYS(ptr->unk_1E, MTXMODE_APPLY);
            Matrix_RotateZS(ptr->unk_20, MTXMODE_APPLY);
            Matrix_Translate(0.0f, -10.0f, 0.0f, MTXMODE_APPLY);
            Matrix_Scale(ptr->unk_18, ptr->unk_18, 1.0f, MTXMODE_APPLY);

            MATRIX_FINALIZE_AND_LOAD(&gfx[0], play->state.gfxCtx);
            gSPDisplayList(&gfx[1], gTakkuriFeatherModelDL);
            gfx = &gfx[2];
        }
    }
    POLY_OPA_DISP = gfx;

    CLOSE_DISPS();
}

void EnThiefbird_Draw(Actor* thisx, PlayState* play) {
    EnThiefbird* this = (EnThiefbird*)thisx;

    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount,
                          EnThiefbird_OverrideLimbDraw, EnThiefbird_PostLimbDraw, &this->actor);
    if (this->actor.colorFilterTimer > 0) {
        func_800AE5A0(play);
    }
    func_80C13354(this, play);
    Actor_DrawDamageEffects(play, &this->actor, this->bodyPartsPos, EN_THIEFBIRD_BODYPART_MAX, this->drawDmgEffScale,
                            this->drawDmgEffFrozenSteamScale, this->drawDmgEffAlpha, this->drawDmgEffType);
    Math_Vec3s_ToVec3f(&this->actor.focus.pos, &this->collider.elements[1].dim.worldSphere.center);
}

OVL_INFO_NATIVE_ENEMY(0x291, En_Thiefbird_Profile);
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_MM_EN_THIEFBIRD, En_Thiefbird_Profile, EnThiefbird_Init, EnThiefbird_Destroy,
                     EnThiefbird_Update, EnThiefbird_Draw);
static const u32 generatedNativeOrigin[] = { 0x4e505431u, 0x1u, 0x291u, 0x22u, 0x70a465b6u };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_THIEFBIRD_ORIGIN, generatedNativeOrigin);
