#include <combo/enemy_bodybreak_impl.h>
#include "oot_variant_compat.h"
#include "types/en_bb.h"
#undef ACTOR_EN_BB
#define ACTOR_EN_BB 0xc69
#undef OBJECT_BB
#define OBJECT_BB 0x105d
#undef ActorShape_Init
#define ActorShape_Init ((void (*)(ActorShape* shape, f32 yOffset, ActorShadowFunc shadowDraw, f32 shadowScale))0x800b3ba4)
#undef Actor_ApplyDamage
#define Actor_ApplyDamage ((u8 (*)(Actor* actor))0x800be22c)
#undef Actor_Kill
extern void FOREIGN_ENEMY_API_KILL(Actor* actor);
#define Actor_Kill FOREIGN_ENEMY_API_KILL
#undef Actor_MoveXZGravity
#define Actor_MoveXZGravity ((void (*)(Actor* actor))0x800b6a88)
#undef Actor_PlaySfx
extern void FOREIGN_ENEMY_API_SFX_ACTOR(Actor* actor, u16 sfxId);
#define Actor_PlaySfx FOREIGN_ENEMY_API_SFX_ACTOR
#undef Actor_ProcessInitChain
#define Actor_ProcessInitChain ((void (*)(Actor* actor, InitChainEntry* ichain))0x800ffadc)
#undef Actor_SetColorFilter
extern void FOREIGN_ENEMY_API_COLOR_FILTER(Actor* actor, s16 colorFlag, s16 colorIntensityMax, s16 bufFlag,
                                           s16 duration);
#define Actor_SetColorFilter FOREIGN_ENEMY_API_COLOR_FILTER
#undef Actor_SetDropFlag
extern void FOREIGN_ENEMY_API_DROP_FLAG(Actor* actor, ColliderElement* elem, s32 freezeFlag);
#define Actor_SetDropFlag FOREIGN_ENEMY_API_DROP_FLAG
#undef Actor_SetScale
#define Actor_SetScale ((void (*)(Actor* actor, f32 scale))0x800b67e0)
#undef Actor_Spawn
extern Actor* FOREIGN_ENEMY_API_SPAWN(ActorContext* actorCtx, PlayState* play, s16 actorId, f32 posX, f32 posY,
                                      f32 posZ, s16 rotX, s16 rotY, s16 rotZ, s16 params);
#define Actor_Spawn FOREIGN_ENEMY_API_SPAWN
#undef Actor_SpawnFloorDustRing
#define Actor_SpawnFloorDustRing ((void (*)(PlayState* play, Actor* actor, Vec3f* posXZ, f32 radius, s32 amountMinusOne, f32 randAccelWeight, s16 scale, s16 scaleStep, u8 useLighting))0x800bbdac)
#undef Actor_UpdateBgCheckInfo
#define Actor_UpdateBgCheckInfo ((void (*)(PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight, s32 flags))0x800b78b8)
#undef Actor_WorldYawTowardActor
#define Actor_WorldYawTowardActor ((s16 (*)(Actor* origin, Actor* target))0x800b6cd4)
#undef Animation_PlayLoop
#define Animation_PlayLoop ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation))0x80137540)
#undef Camera_GetCamDirYaw
#define Camera_GetCamDirYaw ((s16 (*)(Camera* camera))0x800dfcdc)
#undef Collider_DestroyJntSph
extern s32 FOREIGN_ENEMY_API_JOINT_DESTROY(PlayState* play, ColliderJntSph* jntSph);
#define Collider_DestroyJntSph FOREIGN_ENEMY_API_JOINT_DESTROY
#undef Collider_InitJntSph
#define Collider_InitJntSph ((s32 (*)(PlayState* play, ColliderJntSph* jntSph))0x800e0b4c)
#undef Collider_SetJntSph
#define Collider_SetJntSph ((s32 (*)(PlayState* play, ColliderJntSph* dest, Actor* actor, ColliderJntSphInit* src, ColliderJntSphElement* jntSphElements))0x800e0e60)
#undef CollisionCheck_SetAC
extern s32 FOREIGN_ENEMY_API_COLLISION_AC(PlayState* play, CollisionCheckContext* colChkCtx, Collider* collider);
#define CollisionCheck_SetAC FOREIGN_ENEMY_API_COLLISION_AC
#undef CollisionCheck_SetAT
extern s32 FOREIGN_ENEMY_API_COLLISION_AT(PlayState* play, CollisionCheckContext* colChkCtx, Collider* collider);
#define CollisionCheck_SetAT FOREIGN_ENEMY_API_COLLISION_AT
#undef CollisionCheck_SetOC
extern s32 FOREIGN_ENEMY_API_COLLISION_OC(PlayState* play, CollisionCheckContext* colChkCtx, Collider* collider);
#define CollisionCheck_SetOC FOREIGN_ENEMY_API_COLLISION_OC
#undef DMG_DEFAULT
#define DMG_DEFAULT ~(DMG_SHIELD | DMG_MIR_RAY)
#undef EffectSsEnFire_SpawnVec3f
#define EffectSsEnFire_SpawnVec3f ((void (*)(PlayState* play, Actor* actor, Vec3f* pos, s16 scale, s16 arg4, s16 flags, s16 bodyPart))0x800b2df4)
#undef EffectSsEnIce_SpawnFlyingVec3f
extern void FOREIGN_ENEMY_API_FLYING_ICE_F(PlayState* play, Actor* actor, Vec3f* pos, s16 primR, s16 primG, s16 primB,
                                           s16 primA, s16 envR, s16 envG, s16 envB, f32 scale);
#define EffectSsEnIce_SpawnFlyingVec3f FOREIGN_ENEMY_API_FLYING_ICE_F
#undef Gfx_SetupDL_25Opa
#define Gfx_SetupDL_25Opa ((void (*)(GraphicsContext* gfxCtx))0x8012c28c)
#undef Gfx_SetupDL_25Xlu
#define Gfx_SetupDL_25Xlu ((void (*)(GraphicsContext* gfxCtx))0x8012c2dc)
#undef Gfx_TwoTexScroll
#define Gfx_TwoTexScroll ((Gfx* (*)(GraphicsContext* gfxCtx, s32 tile1, u32 x1, u32 y1, s32 width1, s32 height1, s32 tile2, u32 x2, u32 y2, s32 width2, s32 height2))0x8012cbd0)
#undef Item_DropCollectibleRandom
extern void FOREIGN_ENEMY_API_RANDOM_DROP(PlayState* play, Actor* fromActor, Vec3f* spawnPos, s16 params);
#define Item_DropCollectibleRandom FOREIGN_ENEMY_API_RANDOM_DROP
#undef Math_CosF
#define Math_CosF ((f32 (*)(f32 angle))0x80179540)
#undef Math_CosS
#define Math_CosS ((f32 (*)(s16 angle))0x800fed44)
#undef Math_SinF
#define Math_SinF ((f32 (*)(f32 angle))0x801794ec)
#undef Math_SinS
#define Math_SinS ((f32 (*)(s16 angle))0x800fed84)
#undef Math_SmoothStepToF
#define Math_SmoothStepToF ((f32 (*)(f32* pValue, f32 target, f32 fraction, f32 step, f32 minStep))0x800ffcd8)
#undef Math_SmoothStepToS
#define Math_SmoothStepToS ((s16 (*)(s16* pValue, s16 target, s16 scale, s16 step, s16 minStep))0x800ffebc)
#undef Math_Vec3f_DistXZ
#define Math_Vec3f_DistXZ ((f32 (*)(Vec3f* a, Vec3f* b))0x800ff92c)
#undef Math_Vec3f_Pitch
#define Math_Vec3f_Pitch ((s16 (*)(Vec3f* a, Vec3f* b))0x800ffa94)
#undef Math_Vec3f_Yaw
#define Math_Vec3f_Yaw ((s16 (*)(Vec3f* origin, Vec3f* point))0x800ffa60)
#undef Matrix_MultVec3f
#define Matrix_MultVec3f ((void (*)(Vec3f* src, Vec3f* dest))0x80181a98)
#undef Matrix_RotateY
extern void FOREIGN_ENEMY_API_MATRIX_ROTATE_YF(f32 y, u8 mode);
#define Matrix_RotateY FOREIGN_ENEMY_API_MATRIX_ROTATE_YF
#undef Matrix_RotateZ
extern void FOREIGN_ENEMY_API_MATRIX_ROTATE_ZF(f32 z, u8 mode);
#define Matrix_RotateZ FOREIGN_ENEMY_API_MATRIX_ROTATE_ZF
#undef Matrix_RotateZYX
#define Matrix_RotateZYX ((void (*)(s16 x, s16 y, s16 z, u8 mode))0x8018103c)
#undef Matrix_Scale
extern void FOREIGN_ENEMY_API_MATRIX_SCALE(f32 x, f32 y, f32 z, u8 mode);
#define Matrix_Scale FOREIGN_ENEMY_API_MATRIX_SCALE
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
#undef SurfaceType_GetFloorType
#define SurfaceType_GetFloorType ((u32 (*)(CollisionContext* colCtx, CollisionPoly* poly, s32 bgId))0x800c99d4)
#undef NA_SE_EN_DODO_M_GND
#define NA_SE_EN_DODO_M_GND 0x387B
#undef NA_SE_EN_GOMA_JR_FREEZE
#define NA_SE_EN_GOMA_JR_FREEZE 0x389E
#undef NA_SE_EN_BUBLE_WING
#define NA_SE_EN_BUBLE_WING 0x38C8
#undef NA_SE_EN_BUBLE_MOUTH
#define NA_SE_EN_BUBLE_MOUTH 0x38C9
#undef NA_SE_EN_BUBLE_LAUGH
#define NA_SE_EN_BUBLE_LAUGH 0x38CA
#undef NA_SE_EN_BUBLE_BITE
#define NA_SE_EN_BUBLE_BITE 0x38CB
#undef NA_SE_EN_BUBLE_UP
#define NA_SE_EN_BUBLE_UP 0x38CC
#undef NA_SE_EN_BUBLE_DOWN
#define NA_SE_EN_BUBLE_DOWN 0x38CD
#undef NA_SE_EN_BUBLE_DEAD
#define NA_SE_EN_BUBLE_DEAD 0x38CE
#undef NA_SE_EN_BUBLEFALL_FIRE
#define NA_SE_EN_BUBLEFALL_FIRE 0x38CF
#undef NA_SE_EN_BUBLE_DAMAGE
#define NA_SE_EN_BUBLE_DAMAGE 0x38E3
#define object_Bb_Anim_000184 (*(AnimationHeader*)0x06000184)
#define object_Bb_Anim_000444 (*(AnimationHeader*)0x06000444)
#define object_Bb_Skel_001A30 (*(SkeletonHeader*)0x06001a30)

#define FLAGS                                                                                 \
    (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED | \
     ACTOR_FLAG_SFX_FOR_PLAYER_BODY_HIT)

#define vBombHopPhase actionVar1
#define vTrailIdx actionVar1
#define vTrailMaxAlpha actionVar2
#define vMoveAngleY actionVar2
#define vFlameTimer actionVar2

typedef enum EnBbAction {
    BB_DAMAGE,
    BB_KILL,
    BB_FLAME_TRAIL,
    BB_DOWN,
    BB_STUNNED,
    BB_UNUSED,
    BB_BLUE,
    BB_RED,
    BB_WHITE,
    BB_GREEN
} EnBbAction;

typedef enum EnBbMoveMode { BBMOVE_NORMAL, BBMOVE_NOCLIP, BBMOVE_HIDDEN } EnBbMoveMode;

typedef enum EnBbBlueActionState { BBBLUE_NORMAL, BBBLUE_AGGRO } EnBbBlueActionState;

typedef enum EnBbRedActionState { BBRED_WAIT, BBRED_ATTACK, BBRED_HIDE } EnBbRedActionState;

typedef enum EnBbGreenActionState { BBGREEN_FLAME_ON, BBGREEN_FLAME_OFF } EnBbGreenActionState;

void EnBb_Init(Actor* thisx, PlayState* play);
void EnBb_Destroy(Actor* thisx, PlayState* play);
void EnBb_Update(Actor* thisx, PlayState* play2);
void EnBb_Draw(Actor* thisx, PlayState* play);

void EnBb_FaceWaypoint(EnBb* this);
void EnBb_SetWaypoint(EnBb* this, PlayState* play);

void EnBb_SetupFlameTrail(EnBb* this);
void EnBb_FlameTrail(EnBb* this, PlayState* play);

void EnBb_SetupDeath(EnBb* this, PlayState* play);
void EnBb_Death(EnBb* this, PlayState* play);

void EnBb_Damage(EnBb* this, PlayState* play);

void EnBb_SetupBlue(EnBb* this);
void EnBb_Blue(EnBb* this, PlayState* play);

void EnBb_SetupDown(EnBb* this);
void EnBb_Down(EnBb* this, PlayState* play);

void EnBb_SetupRed(PlayState* play, EnBb* this);
void EnBb_Red(EnBb* this, PlayState* play);

void EnBb_SetupWhite(PlayState* play, EnBb* this);
void EnBb_White(EnBb* this, PlayState* play);

void EnBb_InitGreen(EnBb* this, PlayState* play);
void EnBb_Green(EnBb* this, PlayState* play);

void EnBb_Stunned(EnBb* this, PlayState* play);

static DamageTable sDamageTableBlueGreen = {
    DMG_ENTRY(0, 0xF), DMG_ENTRY(2, 0x0), DMG_ENTRY(1, 0x0), DMG_ENTRY(2, 0xA), DMG_ENTRY(0, 0xF), DMG_ENTRY(2, 0xE),
    DMG_ENTRY(2, 0xA), DMG_ENTRY(0, 0xF), DMG_ENTRY(1, 0x0), DMG_ENTRY(2, 0x0), DMG_ENTRY(4, 0x0), DMG_ENTRY(2, 0xE),
    DMG_ENTRY(4, 0xC), DMG_ENTRY(4, 0xB), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x6),
    DMG_ENTRY(3, 0x9), DMG_ENTRY(3, 0x8), DMG_ENTRY(0, 0xA), DMG_ENTRY(0, 0xA), DMG_ENTRY(1, 0x0), DMG_ENTRY(4, 0x0),
    DMG_ENTRY(2, 0x0), DMG_ENTRY(2, 0x0), DMG_ENTRY(8, 0x0), DMG_ENTRY(4, 0x0), DMG_ENTRY(0, 0x6), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(4, 0xA), DMG_ENTRY(0, 0x0),
};

static DamageTable sDamageTableRed = {
    DMG_ENTRY(0, 0xD), DMG_ENTRY(0, 0xD), DMG_ENTRY(0, 0xD), DMG_ENTRY(2, 0xA), DMG_ENTRY(0, 0xD), DMG_ENTRY(2, 0xE),
    DMG_ENTRY(2, 0xA), DMG_ENTRY(0, 0xD), DMG_ENTRY(0, 0xD), DMG_ENTRY(2, 0xE), DMG_ENTRY(4, 0xE), DMG_ENTRY(2, 0xE),
    DMG_ENTRY(4, 0x9), DMG_ENTRY(2, 0xE), DMG_ENTRY(4, 0xE), DMG_ENTRY(2, 0xE), DMG_ENTRY(2, 0xE), DMG_ENTRY(0, 0x6),
    DMG_ENTRY(3, 0x9), DMG_ENTRY(0, 0x6), DMG_ENTRY(0, 0xA), DMG_ENTRY(0, 0xA), DMG_ENTRY(1, 0x0), DMG_ENTRY(4, 0xE),
    DMG_ENTRY(2, 0xE), DMG_ENTRY(2, 0x0), DMG_ENTRY(8, 0xE), DMG_ENTRY(4, 0xE), DMG_ENTRY(0, 0x6), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(4, 0xA), DMG_ENTRY(0, 0x0),
};

static DamageTable sDamageTableWhite = {
    DMG_ENTRY(0, 0xF), DMG_ENTRY(2, 0xE), DMG_ENTRY(1, 0xE), DMG_ENTRY(2, 0xA), DMG_ENTRY(0, 0xF), DMG_ENTRY(2, 0xE),
    DMG_ENTRY(2, 0xA), DMG_ENTRY(0, 0xF), DMG_ENTRY(1, 0xE), DMG_ENTRY(2, 0xE), DMG_ENTRY(4, 0xE), DMG_ENTRY(4, 0x5),
    DMG_ENTRY(2, 0xE), DMG_ENTRY(2, 0xE), DMG_ENTRY(4, 0xE), DMG_ENTRY(2, 0xE), DMG_ENTRY(2, 0xE), DMG_ENTRY(4, 0x7),
    DMG_ENTRY(0, 0x6), DMG_ENTRY(0, 0x6), DMG_ENTRY(0, 0xA), DMG_ENTRY(0, 0xA), DMG_ENTRY(1, 0xE), DMG_ENTRY(4, 0xE),
    DMG_ENTRY(2, 0xE), DMG_ENTRY(2, 0xE), DMG_ENTRY(8, 0xE), DMG_ENTRY(4, 0xE), DMG_ENTRY(0, 0x6), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(4, 0xA), DMG_ENTRY(0, 0x0),
};

ActorProfile En_Bb_Profile = {
    ACTOR_EN_BB, ACTORCAT_ENEMY, FLAGS, OBJECT_BB, sizeof(EnBb), EnBb_Init, EnBb_Destroy, EnBb_Update, EnBb_Draw,
};

static ColliderJntSphElementInit sJntSphElementsInit[] = {
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0x00000000, HIT_SPECIAL_EFFECT_NONE, 0x00 },
            { 0xFFCFFFFF, HIT_BACKLASH_NONE, 0x00 },
            ATELEM_NONE,
            ACELEM_ON,
            OCELEM_ON,
        },
        { 0, { { 0, -120, 0 }, 4 }, 300 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COL_MATERIAL_HIT3,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sJntSphElementsInit),
    sJntSphElementsInit,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(lockOnArrowOffset, 10, ICHAIN_STOP),
};

void EnBb_SetupAction(EnBb* this, EnBbActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

Actor* EnBb_FindExplosive(PlayState* play, EnBb* this, f32 range) {
    Actor* explosive = play->actorCtx.actors[ACTORCAT_EXPLOSIVE].first;
    f32 dist;

    while (explosive != NULL) {
        if (explosive->params != 0) {
            explosive = explosive->next;
            continue;
        }
        dist = Actor_WorldDistXYZToActor(&this->actor, explosive);
        if ((explosive->params == 0) && (dist <= range)) {
            return explosive;
        }
        explosive = explosive->next;
    }
    return NULL;
}

void EnBb_SpawnFlameTrail(PlayState* play, EnBb* this, s16 startAtZero) {
    EnBb* now = this;
    EnBb* next;
    s32 i;

    for (i = 0; i < 5; i++) {
        next = (EnBb*)Actor_Spawn(&play->actorCtx, play, 0x69, this->actor.world.pos.x, this->actor.world.pos.y,
                                  this->actor.world.pos.z, 0, 0, 0, 0);
        if (next != NULL) {
            now->actor.child = &next->actor;
            next->actor.parent = &now->actor;
            next->targetActor = &this->actor;
            next->vTrailIdx = i + 1;
            next->actor.scale.x = 1.0f;
            next->vTrailMaxAlpha = next->flamePrimAlpha = 255 - (i * 40);
            next->flameScaleY = next->actor.scale.y = 0.8f - (i * 0.075f);
            next->flameScaleX = next->actor.scale.z = 1.0f - (i * 0.094f);
            if (startAtZero) {
                next->flamePrimAlpha = 0;
                next->flameScaleY = next->flameScaleX = 0.0f;
            }
            next->flameScrollMod = i + 1;
            next->timer = 2 * i + 2;
            next->flameEnvColor.r = 255;
            now = next;
        }
    }
}

void EnBb_KillFlameTrail(EnBb* this) {
    Actor* actor = &this->actor;
    Actor* nextActor;

    while (actor->child != NULL) {
        nextActor = actor->child;

        if (nextActor->id == ACTOR_EN_BB) {
            nextActor->parent = NULL;
            actor->child = NULL;
            nextActor->params = ENBB_KILL_TRAIL;
        }
        actor = nextActor;
    }
    this->actor.child = NULL;
}

void EnBb_Init(Actor* thisx, PlayState* play) {
    EffectBlureInit1 blureInit;
    s32 pad;
    EnBb* this = (EnBb*)thisx;

    Actor_ProcessInitChain(thisx, sInitChain);
    SkelAnime_Init(play, &this->skelAnime, &object_Bb_Skel_001A30, &object_Bb_Anim_000444, this->jointTable,
                   this->morphTable, 16);
    this->unk_254 = 0;
    this->blureIdx = -1;
    thisx->colChkInfo.health = 4;
    Collider_InitJntSph(play, &this->collider);
    Collider_SetJntSph(play, &this->collider, thisx, &sJntSphInit, this->colliderElements);

    this->actionState = PARAMS_GET_NOMASK(thisx->params, 8);

    if (PARAMS_GET_NOSHIFT(thisx->params, 7, 1)) {
        thisx->params |= 0xFF00;
    }
    if (thisx->params <= ENBB_BLUE) {
        ActorShape_Init(&thisx->shape, 200.0f, ActorShadow_DrawCircle, 35.0f);
    }
    if (PARAMS_GET_NOSHIFT(thisx->params, 8, 8)) {
        this->timer = 0;
        this->flameScaleY = 80.0f;
        this->flameScaleX = 100.0f;
        this->collider.elements[0].base.atElemFlags = ATELEM_ON | ATELEM_SFX_HARD;
        this->collider.elements[0].base.atDmgInfo.dmgFlags = DMG_DEFAULT;
        this->collider.elements[0].base.atDmgInfo.damage = 8;
        this->bobSize = this->actionState * 20.0f;
        this->flamePrimAlpha = 255;
        this->moveMode = BBMOVE_NORMAL;
        Actor_SetScale(thisx, 0.01f);
        switch (thisx->params) {
            case ENBB_BLUE:
                thisx->naviEnemyId = TATL_HINT_ID_BLUE_BUBBLE;
                thisx->colChkInfo.damageTable = &sDamageTableBlueGreen;
                this->flamePrimBlue = this->flameEnvColor.b = 255;
                thisx->world.pos.y += 50.0f;
                EnBb_SetupBlue(this);
                thisx->flags |= ACTOR_FLAG_CAN_ATTACH_TO_ARROW;
                break;
            case ENBB_RED:
                thisx->naviEnemyId = TATL_HINT_ID_BLUE_BUBBLE;
                thisx->colChkInfo.damageTable = &sDamageTableRed;
                this->flameEnvColor.r = 255;
                this->collider.elements[0].base.atDmgInfo.effect = HIT_SPECIAL_EFFECT_FIRE;
                EnBb_SetupRed(play, this);
                break;
            case ENBB_WHITE:
                thisx->naviEnemyId = TATL_HINT_ID_BLUE_BUBBLE;
                thisx->colChkInfo.damageTable = &sDamageTableWhite;
                this->path = this->actionState;
                blureInit.p1StartColor[0] = blureInit.p1StartColor[1] = blureInit.p1StartColor[2] =
                    blureInit.p1StartColor[3] = blureInit.p2StartColor[0] = blureInit.p2StartColor[1] =
                        blureInit.p2StartColor[2] = blureInit.p2StartColor[3] = blureInit.p1EndColor[0] =
                            blureInit.p1EndColor[1] = blureInit.p1EndColor[2] = blureInit.p2EndColor[0] =
                                blureInit.p2EndColor[1] = blureInit.p2EndColor[2] = 255;

                blureInit.p1EndColor[3] = 0;
                blureInit.p2EndColor[3] = 0;
                blureInit.elemDuration = 16;
                blureInit.unkFlag = 0;
                blureInit.calcMode = 2;

                Effect_Add(play, &this->blureIdx, EFFECT_BLURE1, 0, 0, &blureInit);
                EnBb_SetupWhite(play, this);
                EnBb_SetWaypoint(this, play);
                EnBb_FaceWaypoint(this);
                thisx->flags |= ACTOR_FLAG_CAN_ATTACH_TO_ARROW;
                break;
            case ENBB_GREEN_BIG:
                this->path = this->actionState >> 4;
                this->collider.elements[0].dim.modelSphere.radius = 0x16;
                Actor_SetScale(thisx, 0.03f);
                FALLTHROUGH;
            case ENBB_GREEN:
                thisx->naviEnemyId = TATL_HINT_ID_BLUE_BUBBLE;
                this->bobSize = (this->actionState & 0xF) * 20.0f;
                thisx->colChkInfo.damageTable = &sDamageTableBlueGreen;
                this->flameEnvColor.g = 255;
                thisx->colChkInfo.health = 1;

                EnBb_InitGreen(this, play);
                break;
        }
        thisx->focus.pos = thisx->world.pos;
    } else {
        EnBb_SetupFlameTrail(this);
    }
    this->collider.elements[0].dim.worldSphere.radius =
        this->collider.elements[0].dim.modelSphere.radius * this->collider.elements[0].dim.scale;
}

void EnBb_Destroy(Actor* thisx, PlayState* play) {
    s32 pad;
    EnBb* this = (EnBb*)thisx;
    if (this->bodyBreak.matrices) {
        free(this->bodyBreak.matrices);
        this->bodyBreak.matrices = NULL;
    }

    if (this->actor.params == ENBB_WHITE && this->blureIdx >= 0)
        Effect_Delete(play, this->blureIdx);
    Collider_DestroyJntSph(play, &this->collider);
}

void EnBb_SetupFlameTrail(EnBb* this) {
    this->action = BB_FLAME_TRAIL;
    this->moveMode = BBMOVE_NOCLIP;
    this->actor.velocity.y = 0.0f;
    this->actor.gravity = 0.0f;
    this->actor.speed = 0.0f;
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    EnBb_SetupAction(this, EnBb_FlameTrail);
}

void EnBb_FlameTrail(EnBb* this, PlayState* play) {
    if (this->actor.params == ENBB_KILL_TRAIL) {
        if (this->actor.parent == NULL) {
            EnBb_SetupDeath(this, play);
        }
    } else {
        if (this->timer == 0) {
            if (((EnBb*)this->targetActor)->flameScaleY != 0.0f) {
                Math_SmoothStepToF(&this->flameScaleY, this->actor.scale.y, 1.0f, this->actor.scale.y * 0.1f, 0.0f);
                Math_SmoothStepToF(&this->flameScaleX, this->actor.scale.z, 1.0f, this->actor.scale.z * 0.1f, 0.0f);
                if (this->flamePrimAlpha != this->vTrailMaxAlpha) {
                    this->flamePrimAlpha += 10;
                    if (this->vTrailMaxAlpha < this->flamePrimAlpha) {
                        this->flamePrimAlpha = this->vTrailMaxAlpha;
                    }
                }
            } else {
                if (!this->flamePrimAlpha) {
                    Actor_Kill(&this->actor);
                    return;
                } else if (this->flamePrimAlpha <= 20) {
                    this->flamePrimAlpha = 0;
                } else {
                    this->flamePrimAlpha -= 20;
                }
            }
            this->actor.world.pos = this->actor.parent->prevPos;
        } else {
            this->timer--;
            this->actor.shape.rot.y = this->actor.world.rot.y = this->actor.parent->world.rot.y;
        }
    }
    if (this->actor.parent != NULL) {
        this->actor.velocity.y = this->actor.parent->velocity.y;
    }
}

void EnBb_SetupDeath(EnBb* this, PlayState* play) {
    if (this->actor.params <= ENBB_BLUE) {
        this->actor.world.rot.y = this->actor.yawTowardsPlayer;
        this->actor.speed = -7.0f;
        this->timer = 5;
        this->actor.shape.rot.x += 0x4E20;
        EffectSsDeadSound_SpawnStationary(play, &this->actor.projectedPos, NA_SE_EN_BUBLE_DEAD, 1, 1, 0x28);
    }
    this->action = BB_KILL;
    EnBb_SetupAction(this, EnBb_Death);
}

void EnBb_Death(EnBb* this, PlayState* play) {
    s16 enpartType = 3;
    Vec3f sp40 = { 0.0f, 0.5f, 0.0f };
    Vec3f sp34 = { 0.0f, 0.0f, 0.0f };

    if (this->actor.params <= ENBB_BLUE) {
        Math_SmoothStepToF(&this->flameScaleY, 0.0f, 1.0f, 30.0f, 0.0f);
        Math_SmoothStepToF(&this->flameScaleX, 0.0f, 1.0f, 30.0f, 0.0f);
        if (this->timer != 0) {
            this->timer--;
            this->actor.shape.rot.x -= 0x4E20;
            return;
        }

        if (this->bodyBreak.val == BODYBREAK_STATUS_FINISHED) {
            BodyBreak_Alloc(&this->bodyBreak, 12, play);
        }

        if ((this->dmgEffect == 7) || (this->dmgEffect == 5)) {
            enpartType = 11;
        }

        if (!BodyBreak_SpawnParts(&this->actor, &this->bodyBreak, play, enpartType)) {
            return;
        }
        Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos,
                                   COLLECTIBLE_DROP_RANDOM_PARAMS(COLLECTIBLE_DROP_TABLE_13, false));
    } else {
        if (this->flamePrimAlpha) {
            if (this->flamePrimAlpha <= 20) {
                this->flamePrimAlpha = 0;
            } else {
                this->flamePrimAlpha -= 20;
            }
            return;
        }
    }
    Actor_Kill(&this->actor);
}

void EnBb_SetupDamage(EnBb* this) {
    this->action = BB_DAMAGE;
    Actor_PlaySfx(&this->actor, NA_SE_EN_BUBLE_DAMAGE);
    if (this->actor.params > ENBB_GREEN) {
        this->actor.world.rot.y = this->actor.yawTowardsPlayer;
        if (!(this->actor.bgCheckFlags & BGCHECKFLAG_WALL)) {
            this->actor.speed = -7.0f;
        }
        this->actor.shape.yOffset = 1500.0f;
    }
    if (this->actor.params == ENBB_RED) {
        EnBb_KillFlameTrail(this);
    }
    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 12);
    this->timer = 5;
    EnBb_SetupAction(this, EnBb_Damage);
}

void EnBb_Damage(EnBb* this, PlayState* play) {
    Math_SmoothStepToF(&this->actor.speed, 0.0f, 1.0f, 0.5f, 0.0f);
    if (this->actor.speed == 0.0f) {
        this->actor.shape.yOffset = 200.0f;
        EnBb_SetupDown(this);
    }
}

void EnBb_SetupBlue(EnBb* this) {
    Animation_PlayLoop(&this->skelAnime, &object_Bb_Anim_000444);
    this->actor.speed = (Rand_ZeroOne() * 0.5f) + 0.5f;
    this->timer = (Rand_ZeroOne() * 20.0f) + 40.0f;
    this->unk_264 = (Rand_ZeroOne() * 30.0f) + 180.0f;
    this->targetActor = NULL;
    this->action = BB_BLUE;
    EnBb_SetupAction(this, EnBb_Blue);
}

void EnBb_Blue(EnBb* this, PlayState* play) {
    Actor* explosive;
    s16 moveYawToWall;
    s16 thisYawToWall;
    s16 afterHitAngle;

    Math_SmoothStepToF(&this->flameScaleY, 80.0f, 1.0f, 10.0f, 0.0f);
    Math_SmoothStepToF(&this->flameScaleX, 100.0f, 1.0f, 10.0f, 0.0f);
    if (this->actor.floorHeight > BGCHECK_Y_MIN) {
        Math_SmoothStepToF(&this->actor.world.pos.y, this->actor.floorHeight + 50.0f + this->flyHeightMod, 1.0f, 0.5f,
                           0.0f);
    }
    SkelAnime_Update(&this->skelAnime);
    if (Math_CosF(this->bobPhase) == 0.0f) {
        if (this->charge) {
            this->bobSpeedMod = Rand_ZeroOne() * 2.0f;
        } else {
            this->bobSpeedMod = Rand_ZeroOne() * 4.0f;
        }
    }
    this->actor.world.pos.y += Math_CosF(this->bobPhase) * (1.0f + this->bobSpeedMod);
    this->bobPhase += 0.2f;
    Math_SmoothStepToF(&this->actor.speed, this->maxSpeedXZ, 1.0f, 0.5f, 0.0f);

    if (Math_Vec3f_DistXZ(&this->actor.world.pos, &this->actor.home.pos) > 300.0f) {
        this->vMoveAngleY = Math_Vec3f_Yaw(&this->actor.world.pos, &this->actor.home.pos);
        Math_SmoothStepToS(&this->actor.world.rot.y, this->vMoveAngleY, 1, 0x7D0, 0);
    } else {
        this->timer--;
        if (this->timer <= 0) {
            this->charge ^= true;
            this->flyHeightMod = (s16)(Math_CosF(this->bobPhase) * 10.0f);
            this->actor.speed = 0.0f;
            if (this->charge && (this->targetActor == NULL)) {
                this->vMoveAngleY = this->actor.world.rot.y;
                if (this->actor.xzDistToPlayer < 200.0f) {
                    Animation_PlayLoop(&this->skelAnime, &object_Bb_Anim_000184);
                    this->vMoveAngleY = this->actor.yawTowardsPlayer;
                }
                this->maxSpeedXZ = (Rand_ZeroOne() * 1.5f) + 6.0f;
                this->timer = (Rand_ZeroOne() * 5.0f) + 20.0f;
                this->actionState = BBBLUE_NORMAL;
            } else {
                Animation_PlayLoop(&this->skelAnime, &object_Bb_Anim_000444);
                this->maxSpeedXZ = (Rand_ZeroOne() * 1.5f) + 1.0f;
                this->timer = (Rand_ZeroOne() * 20.0f) + 40.0f;
                this->vMoveAngleY = Math_SinF(this->bobPhase) * 65535.0f;
            }
        }
        if ((this->actor.xzDistToPlayer < 150.0f) && (this->actionState != BBBLUE_NORMAL)) {
            if (!this->charge) {
                Animation_PlayLoop(&this->skelAnime, &object_Bb_Anim_000184);
                this->maxSpeedXZ = (Rand_ZeroOne() * 1.5f) + 6.0f;
                this->timer = (Rand_ZeroOne() * 5.0f) + 20.0f;
                this->vMoveAngleY = this->actor.yawTowardsPlayer;
                this->actionState = this->charge = true;
            }
        } else if (this->actor.xzDistToPlayer < 200.0f) {
            this->vMoveAngleY = this->actor.yawTowardsPlayer;
        }
        if (this->targetActor == NULL) {
            explosive = EnBb_FindExplosive(play, this, 300.0f);
        } else if (this->targetActor->params == 0) {
            explosive = this->targetActor;
        } else {
            explosive = NULL;
        }
        if (explosive != NULL) {
            this->vMoveAngleY = Actor_WorldYawTowardActor(&this->actor, explosive);
            if ((this->vBombHopPhase == 0) && (explosive != this->targetActor)) {
                this->vBombHopPhase = -0x8000;
                this->targetActor = explosive;
                this->actor.speed *= 0.5f;
            }
            Math_SmoothStepToS(&this->actor.world.rot.y, this->vMoveAngleY, 1, 0x1388, 0);
            Math_SmoothStepToF(&this->actor.world.pos.x, explosive->world.pos.x, 1.0f, 1.5f, 0.0f);
            Math_SmoothStepToF(&this->actor.world.pos.y, explosive->world.pos.y + 40.0f, 1.0f, 1.5f, 0.0f);
            Math_SmoothStepToF(&this->actor.world.pos.z, explosive->world.pos.z, 1.0f, 1.5f, 0.0f);
        } else {
            this->targetActor = NULL;
        }
        if (this->vBombHopPhase != 0) {
            this->actor.world.pos.y += -Math_CosS(this->vBombHopPhase) * 10.0f;
            this->vBombHopPhase += 0x1000;
            Math_SmoothStepToS(&this->actor.world.rot.y, this->vMoveAngleY, 1, 0x7D0, 0);
        }
        thisYawToWall = this->actor.wallYaw - this->actor.world.rot.y;
        moveYawToWall = this->actor.wallYaw - this->vMoveAngleY;
        if ((this->targetActor == NULL) && (this->actor.bgCheckFlags & BGCHECKFLAG_WALL) &&
            (ABS(thisYawToWall) > 0x4000 || ABS(moveYawToWall) > 0x4000)) {
            this->vMoveAngleY = this->actor.wallYaw + this->actor.wallYaw - this->actor.world.rot.y - 0x8000;
            Math_SmoothStepToS(&this->actor.world.rot.y, this->vMoveAngleY, 1, 0xBB8, 0);
        }
    }
    Math_SmoothStepToS(&this->actor.world.rot.y, this->vMoveAngleY, 1, 0x3E8, 0);
    if ((this->collider.base.acFlags & AC_HIT) || (this->collider.base.atFlags & AT_HIT)) {
        this->vMoveAngleY = this->actor.yawTowardsPlayer + 0x8000;
        if (this->collider.base.acFlags & AC_HIT) {
            afterHitAngle = -0x8000;
        } else {
            afterHitAngle = 0x4000;
            Actor_PlaySfx(&this->actor, NA_SE_EN_BUBLE_BITE);
            if (play->gameplayFrames & 1) {
                afterHitAngle = -0x4000;
            }
        }
        this->actor.world.rot.y = this->actor.yawTowardsPlayer + afterHitAngle;
        this->collider.base.acFlags &= ~AC_HIT;
        this->collider.base.atFlags &= ~AT_HIT;
    }

    if (this->maxSpeedXZ >= 6.0f) {
        if ((s32)this->skelAnime.curFrame == 0 || (s32)this->skelAnime.curFrame == 5) {
            Actor_PlaySfx(&this->actor, NA_SE_EN_BUBLE_MOUTH);
        } else if ((s32)this->skelAnime.curFrame == 2 || (s32)this->skelAnime.curFrame == 7) {
            Actor_PlaySfx(&this->actor, NA_SE_EN_BUBLE_WING);
        }
    } else {
        if ((s32)this->skelAnime.curFrame == 5) {
            Actor_PlaySfx(&this->actor, NA_SE_EN_BUBLE_WING);
        }
    }
    if (((s32)this->skelAnime.curFrame == 0) && (Rand_ZeroOne() < 0.1f)) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_BUBLE_LAUGH);
    }
    this->actor.shape.rot.y = this->actor.world.rot.y;
}

void EnBb_SetupDown(EnBb* this) {
    Animation_PlayLoop(&this->skelAnime, &object_Bb_Anim_000444);
    this->action = BB_DOWN;
    this->timer = 200;
    this->actor.colorFilterTimer = 0;
    this->actor.bgCheckFlags &= ~BGCHECKFLAG_GROUND;
    this->actor.speed = 3.0f;
    this->flameScaleX = 0.0f;
    this->flameScaleY = 0.0f;
    this->actor.gravity = -2.0f;
    Actor_PlaySfx(&this->actor, NA_SE_EN_BUBLE_DOWN);
    EnBb_SetupAction(this, EnBb_Down);
}

void EnBb_Down(EnBb* this, PlayState* play) {
    s16 yawDiff = this->actor.world.rot.y - this->actor.wallYaw;

    SkelAnime_Update(&this->skelAnime);
    if (this->actor.bgCheckFlags & BGCHECKFLAG_WALL) {
        if (ABS(yawDiff) > 0x4000) {
            this->actor.world.rot.y = this->actor.wallYaw + this->actor.wallYaw - this->actor.world.rot.y - 0x8000;
        }
        this->actor.bgCheckFlags &= ~BGCHECKFLAG_WALL;
    }
    if (this->actor.bgCheckFlags & (BGCHECKFLAG_GROUND | BGCHECKFLAG_GROUND_TOUCH)) {
        if (this->actor.params == ENBB_RED) {
            s32 floorType = SurfaceType_GetFloorType(&play->colCtx, this->actor.floorPoly, this->actor.floorBgId);

            if ((floorType == FLOOR_TYPE_2) || (floorType == FLOOR_TYPE_3) || (floorType == FLOOR_TYPE_9)) {
                this->moveMode = BBMOVE_HIDDEN;
                this->timer = 10;
                this->actionState++;
                this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
                this->action = BB_RED;
                EnBb_SetupAction(this, EnBb_Red);
                return;
            }
        }
        Actor_PlaySfx(&this->actor, NA_SE_EN_DODO_M_GND);
        if (this->actor.velocity.y < -14.0f) {
            this->actor.velocity.y *= -0.7f;
        } else {
            this->actor.velocity.y = 10.0f;
        }
        this->actor.bgCheckFlags &= ~BGCHECKFLAG_GROUND;
        Actor_SpawnFloorDustRing(play, &this->actor, &this->actor.world.pos, 7.0f, 2, 2.0f, 0, 0, false);
        Math_SmoothStepToS(&this->actor.world.rot.y, -this->actor.yawTowardsPlayer, 1, 0xBB8, 0);
    }
    this->actor.shape.rot.y = this->actor.world.rot.y;
    if ((s32)this->skelAnime.curFrame == 5) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_BUBLE_WING);
    }
    if (this->timer == 0) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_BUBLE_UP);
        switch (this->actor.params) {
            case ENBB_BLUE:
                this->actor.velocity.y = 0.0f;
                this->actor.gravity = 0.0f;
                EnBb_SetupBlue(this);
                break;
            case ENBB_RED:
                if (this->actor.velocity.y == 10.0f) {
                    EnBb_SetupRed(play, this);
                    EnBb_SpawnFlameTrail(play, this, true);
                }
                break;
            case ENBB_WHITE:
                this->actor.velocity.y = 0.0f;
                this->actor.gravity = 0.0f;
                EnBb_SetupWhite(play, this);
                this->actor.world.pos.y -= 60.0f;
                break;
        }
    } else {
        this->timer--;
    }
}

void EnBb_SetupRed(PlayState* play, EnBb* this) {
    Animation_PlayLoop(&this->skelAnime, &object_Bb_Anim_000184);
    if (this->action == BB_DOWN) {
        this->actor.speed = 5.0f;
        this->actor.gravity = -1.0f;
        this->actor.velocity.y = 16.0f;
        this->actionState = BBRED_ATTACK;
        this->timer = 0;
        this->moveMode = BBMOVE_NORMAL;
        this->actor.bgCheckFlags &= ~BGCHECKFLAG_GROUND;
    } else {
        this->actor.colChkInfo.health = 4;
        this->timer = 0;
        this->actionState = BBRED_WAIT;
        this->moveMode = BBMOVE_HIDDEN;
        this->actor.world.pos.y -= 80.0f;
        this->actor.home.pos = this->actor.world.pos;
        this->actor.velocity.y = this->actor.gravity = this->actor.speed = 0.0f;
        this->actor.bgCheckFlags &= ~BGCHECKFLAG_GROUND;
        this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    }
    this->action = BB_RED;
    EnBb_SetupAction(this, EnBb_Red);
}

void EnBb_Red(EnBb* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 floorType;
    s16 yawDiff;

    SkelAnime_Update(&this->skelAnime);
    if (this->timer != 0) {
        this->timer--;
    }

    yawDiff = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;
    switch (this->actionState) {
        case BBRED_WAIT:
            if ((Actor_WorldDistXYZToActor(&this->actor, &player->actor) <= 250.0f) && (ABS(yawDiff) <= 0x4000) &&
                (this->timer == 0)) {
                this->actor.speed = 5.0f;
                this->actor.gravity = -1.0f;
                this->actor.velocity.y = 18.0f;
                this->moveMode = BBMOVE_NOCLIP;
                this->timer = 7;
                this->actor.bgCheckFlags &= ~BGCHECKFLAG_GROUND;
                this->actionState++;
                EnBb_SpawnFlameTrail(play, this, false);
            }
            break;
        case BBRED_ATTACK:
            if (this->timer == 0) {
                this->moveMode = BBMOVE_NORMAL;
                this->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED;
            }
            this->bobPhase += Rand_ZeroOne();
            Math_SmoothStepToF(&this->flameScaleY, 80.0f, 1.0f, 10.0f, 0.0f);
            Math_SmoothStepToF(&this->flameScaleX, 100.0f, 1.0f, 10.0f, 0.0f);
            if (this->actor.bgCheckFlags & BGCHECKFLAG_WALL) {
                yawDiff = this->actor.world.rot.y - this->actor.wallYaw;
                if (ABS(yawDiff) > 0x4000) {
                    this->actor.world.rot.y =
                        this->actor.wallYaw + this->actor.wallYaw - this->actor.world.rot.y - 0x8000;
                }
                this->actor.bgCheckFlags &= ~BGCHECKFLAG_WALL;
            }
            if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
                floorType = SurfaceType_GetFloorType(&play->colCtx, this->actor.floorPoly, this->actor.floorBgId);
                if ((floorType == FLOOR_TYPE_2) || (floorType == FLOOR_TYPE_3) || (floorType == FLOOR_TYPE_9)) {
                    this->moveMode = BBMOVE_HIDDEN;
                    this->timer = 10;
                    this->actionState++;
                    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
                } else {
                    this->actor.velocity.y *= -1.06f;
                    if (this->actor.velocity.y > 13.0f) {
                        this->actor.velocity.y = 13.0f;
                    }
                    this->actor.world.rot.y = Math_SinF(this->bobPhase) * 65535.0f;
                }
                this->actor.bgCheckFlags &= ~BGCHECKFLAG_GROUND;
            }
            this->actor.shape.rot.y = this->actor.world.rot.y;
            if (Actor_GetCollidedExplosive(play, &this->collider.base) != NULL) {
                EnBb_SetupDown(this);
            }
            break;
        case BBRED_HIDE:
            if (this->timer == 0) {
                this->actor.speed = 0.0f;
                this->actor.gravity = 0.0f;
                this->actor.velocity.y = 0.0f;
                this->actionState = BBRED_WAIT;
                this->timer = 120;
                this->actor.world.pos = this->actor.home.pos;
                this->actor.shape.rot = this->actor.world.rot = this->actor.home.rot;
                EnBb_KillFlameTrail(this);
            }
            break;
    }
    if (this->actionState != BBRED_WAIT) {
        if (((s32)this->skelAnime.curFrame == 0) || ((s32)this->skelAnime.curFrame == 5)) {
            Actor_PlaySfx(&this->actor, NA_SE_EN_BUBLE_MOUTH);
        }
        Actor_PlaySfx(&this->actor, NA_SE_EN_BUBLEFALL_FIRE - SFX_FLAG);
    }
}

void EnBb_FaceWaypoint(EnBb* this) {
    this->actor.world.rot.y = this->actor.shape.rot.y = Math_Vec3f_Yaw(&this->actor.world.pos, &this->waypointPos);
}

void EnBb_SetWaypoint(EnBb* this, PlayState* play) {

    static const s16 offsets[4][2] = { { -40, 0 }, { 0, 40 }, { 40, 0 }, { 0, -40 } };
    this->waypoint = (this->waypoint + 1) & 3;
    this->waypointPos.x = this->actor.home.pos.x + offsets[this->waypoint][0];
    this->waypointPos.y = this->actor.home.pos.y + 60.0f;
    this->waypointPos.z = this->actor.home.pos.z + offsets[this->waypoint][1];
}

void EnBb_SetupWhite(PlayState* play, EnBb* this) {
    Animation_PlayLoop(&this->skelAnime, &object_Bb_Anim_000444);
    this->actor.speed = 0.0f;
    this->actor.world.pos.y += 60.0f;
    this->flameScaleX = 100.0f;
    this->action = BB_WHITE;
    this->waypoint = 0;
    this->timer = (Rand_ZeroOne() * 30.0f) + 40.0f;
    this->maxSpeedXZ = 7.0f;
    EnBb_SetupAction(this, EnBb_White);
}

void EnBb_White(EnBb* this, PlayState* play) {
    if (this->actor.speed == 0.0f) {
        f32 distL1;
        f32 vx;
        f32 vz;
        s16 pitch = Math_Vec3f_Pitch(&this->actor.world.pos, &this->waypointPos);
        f32 vy = Math_SinS(pitch) * this->maxSpeedXZ;
        f32 vxz = Math_CosS(pitch) * this->maxSpeedXZ;

        vx = Math_SinS(this->actor.shape.rot.y) * vxz;
        vz = Math_CosS(this->actor.shape.rot.y) * vxz;
        distL1 = Math_SmoothStepToF(&this->actor.world.pos.x, this->waypointPos.x, 1.0f, ABS(vx), 0.0f);
        distL1 += Math_SmoothStepToF(&this->actor.world.pos.y, this->waypointPos.y, 1.0f, ABS(vy), 0.0f);
        distL1 += Math_SmoothStepToF(&this->actor.world.pos.z, this->waypointPos.z, 1.0f, ABS(vz), 0.0f);
        this->bobPhase += (0.05f + (Rand_ZeroOne() * 0.01f));
        if (distL1 == 0.0f) {
            this->timer--;
            if (this->timer == 0) {
                EnBb_SetWaypoint(this, play);
                EnBb_FaceWaypoint(this);
                Animation_PlayLoop(&this->skelAnime, &object_Bb_Anim_000184);
                this->timer = Rand_ZeroOne() * 30.0f + 40.0f;
            } else {
                if (this->moveMode != BBMOVE_NORMAL) {
                    Animation_PlayLoop(&this->skelAnime, &object_Bb_Anim_000444);
                }
                this->actor.world.rot.y += 0x1F40;
            }
            this->moveMode = BBMOVE_NORMAL;
            this->maxSpeedXZ = 0.0f;
        } else {
            this->moveMode = BBMOVE_NOCLIP;
            this->maxSpeedXZ = 10.0f;
        }
        if (this->collider.base.atFlags & AT_HIT) {
            Actor_PlaySfx(&this->actor, NA_SE_EN_BUBLE_BITE);
            this->collider.base.atFlags &= ~AT_HIT;
        }
        this->actor.shape.rot.y = this->actor.world.rot.y;
    } else if (Math_SmoothStepToF(&this->actor.speed, 0.0f, 1.0f, 0.5f, 0.0f) == 0.0f) {
        EnBb_FaceWaypoint(this);
    }
    SkelAnime_Update(&this->skelAnime);
    if (((s32)this->skelAnime.curFrame == 0) && (Rand_ZeroOne() <= 0.1f)) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_BUBLE_LAUGH);
    }

    if ((this->maxSpeedXZ != 0.0f) && (((s32)this->skelAnime.curFrame == 0) || ((s32)this->skelAnime.curFrame == 5))) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_BUBLE_MOUTH);
    } else if (((s32)this->skelAnime.curFrame == 2) || ((s32)this->skelAnime.curFrame == 7)) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_BUBLE_WING);
    }
}

void EnBb_InitGreen(EnBb* this, PlayState* play) {
    Vec3f bobOffset = { 0.0f, 0.0f, 0.0f };

    Animation_PlayLoop(&this->skelAnime, &object_Bb_Anim_000444);
    this->moveMode = BBMOVE_NOCLIP;
    this->actionState = BBGREEN_FLAME_ON;
    this->bobPhase = Rand_ZeroOne();
    this->actor.shape.rot.x = this->actor.shape.rot.z = 0;
    this->actor.shape.rot.y = this->actor.yawTowardsPlayer;
    if (this->actor.params == ENBB_GREEN_BIG) {
        EnBb_SetWaypoint(this, play);
        EnBb_FaceWaypoint(this);
    }
    Matrix_Translate(this->actor.home.pos.x, this->actor.home.pos.y, this->actor.home.pos.z, MTXMODE_NEW);
    Matrix_RotateZYX(this->actor.world.rot.x, this->actor.world.rot.y, 0, MTXMODE_APPLY);
    Matrix_RotateZ(this->bobPhase, MTXMODE_APPLY);
    bobOffset.y = this->bobSize;
    Matrix_MultVec3f(&bobOffset, &this->actor.world.pos);
    this->targetActor = NULL;
    this->action = BB_GREEN;
    this->actor.speed = 0.0f;
    this->vFlameTimer = (Rand_ZeroOne() * 30.0f) + 180.0f;
    EnBb_SetupAction(this, EnBb_Green);
}

void EnBb_SetupGreen(EnBb* this) {
    Animation_PlayLoop(&this->skelAnime, &object_Bb_Anim_000444);
    this->moveMode = BBMOVE_NOCLIP;
    this->actionState = BBGREEN_FLAME_ON;
    this->targetActor = NULL;
    this->action = BB_GREEN;
    this->actor.speed = 0.0f;
    this->vFlameTimer = (Rand_ZeroOne() * 30.0f) + 180.0f;
    this->actor.shape.rot.z = 0;
    this->actor.shape.rot.y = this->actor.yawTowardsPlayer;
    EnBb_SetupAction(this, EnBb_Green);
}

void EnBb_Green(EnBb* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Vec3f bobOffset = { 0.0f, 0.0f, 0.0f };
    Vec3f nextPos = player->actor.world.pos;

    nextPos.y += 30.0f;
    if (this->actor.params == ENBB_GREEN_BIG) {
        if (this->actor.speed == 0.0f) {
            s16 pitch = Math_Vec3f_Pitch(&this->actor.home.pos, &this->waypointPos);
            s16 yaw = Math_Vec3f_Yaw(&this->actor.home.pos, &this->waypointPos);
            f32 vy = Math_SinS(pitch) * this->maxSpeedXZ;
            f32 vxz = Math_CosS(pitch) * this->maxSpeedXZ;
            f32 vz;
            f32 vx;
            f32 distL1;

            Math_SmoothStepToS(&this->actor.world.rot.y, yaw, 1, 0x3E8, 0);
            vx = Math_SinS(this->actor.world.rot.y) * vxz;
            distL1 = Math_CosS(this->actor.world.rot.y) * vxz;
            vz = Math_SmoothStepToF(&this->actor.home.pos.x, this->waypointPos.x, 1.0f, ABS(vx), 0.0f);
            vz += Math_SmoothStepToF(&this->actor.home.pos.y, this->waypointPos.y, 1.0f, ABS(vy), 0.0f);
            vz += Math_SmoothStepToF(&this->actor.home.pos.z, this->waypointPos.z, 1.0f, ABS(distL1), 0.0f);
            this->bobPhase += (0.05f + (Rand_ZeroOne() * 0.01f));
            if (vz == 0.0f) {
                EnBb_SetWaypoint(this, play);
            }
            this->moveMode = BBMOVE_NOCLIP;
            this->maxSpeedXZ = 10.0f;
            if (this->collider.base.atFlags & AT_HIT) {
                Actor_PlaySfx(&this->actor, NA_SE_EN_BUBLE_BITE);
                this->collider.base.atFlags &= ~AT_HIT;
            }
            if (Math_CosF(this->bobPhase) == 0.0f) {
                if (this->charge) {
                    this->bobSpeedMod = Rand_ZeroOne();
                } else {
                    this->bobSpeedMod = Rand_ZeroOne() * 3.0f;
                    Actor_PlaySfx(&this->actor, NA_SE_EN_BUBLE_LAUGH);
                }
            }
            this->actor.shape.rot.y = this->actor.world.rot.y;
        } else if (Math_SmoothStepToF(&this->actor.speed, 0.0f, 1.0f, 0.5f, 0.0f) == 0.0f) {
            EnBb_FaceWaypoint(this);
        }
    } else {
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0xFA0, 0);
        Math_SmoothStepToS(&this->actor.shape.rot.x, Math_Vec3f_Pitch(&this->actor.world.pos, &nextPos), 1, 0xFA0, 0);
    }
    SkelAnime_Update(&this->skelAnime);
    if (Math_CosF(this->bobPhase) <= 0.002f) {
        this->bobSpeedMod = Rand_ZeroOne() * 0.05f;
    }
    Matrix_Translate(this->actor.home.pos.x, this->actor.home.pos.y, this->actor.home.pos.z, MTXMODE_NEW);
    Matrix_RotateZYX(this->actor.world.rot.x, this->actor.world.rot.y, 0, MTXMODE_APPLY);
    Matrix_RotateZ(this->bobPhase, MTXMODE_APPLY);
    bobOffset.y = this->bobSize;
    Matrix_MultVec3f(&bobOffset, &nextPos);
    Math_SmoothStepToF(&this->actor.world.pos.x, nextPos.x, 1.0f, this->bobPhase * 0.75f, 0.0f);
    Math_SmoothStepToF(&this->actor.world.pos.y, nextPos.y, 1.0f, this->bobPhase * 0.75f, 0.0f);
    Math_SmoothStepToF(&this->actor.world.pos.z, nextPos.z, 1.0f, this->bobPhase * 0.75f, 0.0f);
    this->bobPhase += 0.1f + this->bobSpeedMod;
    if (Actor_GetCollidedExplosive(play, &this->collider.base) || (--this->vFlameTimer == 0)) {
        this->actionState++;
        this->timer = (Rand_ZeroOne() * 30.0f) + 60.0f;
        if (this->vFlameTimer != 0) {
            this->collider.base.acFlags &= ~AC_HIT;
        }
        Actor_PlaySfx(&this->actor, NA_SE_EN_BUBLE_DOWN);
    }
    if (this->actionState != BBGREEN_FLAME_ON) {
        this->timer--;
        if (this->timer == 0) {
            this->actionState = BBGREEN_FLAME_ON;
            this->vFlameTimer = (Rand_ZeroOne() * 30.0f) + 180.0f;
            Actor_PlaySfx(&this->actor, NA_SE_EN_BUBLE_UP);
        }
        Math_SmoothStepToF(&this->flameScaleY, 0.0f, 1.0f, 10.0f, 0.0f);
        Math_SmoothStepToF(&this->flameScaleX, 0.0f, 1.0f, 10.0f, 0.0f);
    } else {
        Math_SmoothStepToF(&this->flameScaleY, 80.0f, 1.0f, 10.0f, 0.0f);
        Math_SmoothStepToF(&this->flameScaleX, 100.0f, 1.0f, 10.0f, 0.0f);
    }
    if ((s32)this->skelAnime.curFrame == 5) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_BUBLE_WING);
    }
    if (((s32)this->skelAnime.curFrame == 0) && (Rand_ZeroOne() < 0.1f)) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_BUBLE_LAUGH);
    }
}

void EnBb_SetupStunned(EnBb* this) {
    this->action = BB_STUNNED;
    if (this->actor.params != ENBB_WHITE) {
        if (this->actor.params != ENBB_RED) {
            if (this->actor.params > ENBB_GREEN) {
                this->actor.gravity = -2.0f;
                this->actor.shape.yOffset = 1500.0f;
            }
            this->actor.speed = 0.0f;
            this->flameScaleX = 0.0f;
            this->flameScaleY = 0.0f;
        } else {
            EnBb_KillFlameTrail(this);
        }
    }
    switch (this->dmgEffect) {
        case 8:
            Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_GRAY, 200, COLORFILTER_BUFFLAG_OPA, 80);
            break;
        case 9:
            this->fireIceTimer = 0x30;
            FALLTHROUGH;
        case 15:
            Actor_PlaySfx(&this->actor, NA_SE_EN_GOMA_JR_FREEZE);
            Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_BLUE, 180, COLORFILTER_BUFFLAG_OPA, 80);
            break;
    }
    this->actor.bgCheckFlags &= ~BGCHECKFLAG_GROUND;
    EnBb_SetupAction(this, EnBb_Stunned);
}

void EnBb_Stunned(EnBb* this, PlayState* play) {
    s16 yawDiff = this->actor.world.rot.y - this->actor.wallYaw;

    if (this->actor.bgCheckFlags & BGCHECKFLAG_WALL) {
        if (ABS(yawDiff) > 0x4000) {
            this->actor.world.rot.y = this->actor.wallYaw + this->actor.wallYaw - this->actor.world.rot.y - 0x8000;
        }
        this->actor.bgCheckFlags &= ~BGCHECKFLAG_WALL;
    }
    if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND_TOUCH) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_DODO_M_GND);
        if (this->actor.velocity.y < -14.0f) {
            this->actor.velocity.y *= -0.4f;
        } else {
            this->actor.velocity.y = 0.0f;
        }
        Actor_SpawnFloorDustRing(play, &this->actor, &this->actor.world.pos, 7.0f, 2, 2.0f, 0, 0, false);
    }
    if (this->actor.colorFilterTimer == 0) {
        this->actor.shape.yOffset = 200.0f;
        if (this->actor.colChkInfo.health != 0) {
            if ((this->actor.params == ENBB_GREEN) || (this->actor.params == ENBB_GREEN_BIG)) {
                EnBb_SetupGreen(this);
            } else if (this->actor.params == ENBB_WHITE) {
                this->action = BB_WHITE;
                EnBb_SetupAction(this, EnBb_White);
            } else {
                EnBb_SetupDown(this);
            }
        } else {
            this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
            EnBb_SetupDeath(this, play);
        }
    }
}

void EnBb_CollisionCheck(EnBb* this, PlayState* play) {
    if (this->collider.base.atFlags & AT_BOUNCED) {
        this->collider.base.atFlags &= ~AT_BOUNCED;
        if (this->action != BB_DOWN) {
            if (this->actor.params >= ENBB_RED) {
                this->actor.world.rot.y = this->actor.shape.rot.y = this->actor.yawTowardsPlayer + 0x8000;
                if (this->actor.params == ENBB_RED) {
                    EnBb_KillFlameTrail(this);
                }
                EnBb_SetupDown(this);
                return;
            }
            this->actionVar2 = 1;
        }
    }
    if (this->collider.base.acFlags & AC_HIT) {
        this->collider.base.acFlags &= ~AC_HIT;
        this->dmgEffect = this->actor.colChkInfo.damageEffect;
        Actor_SetDropFlag(&this->actor, &this->collider.elements[0].base, false);
        switch (this->dmgEffect) {
            case 7:
                this->actor.freezeTimer = this->collider.elements[0].base.acHitElem->atDmgInfo.damage;
                FALLTHROUGH;
            case 5:
                this->fireIceTimer = 0x30;

                Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 48);
                goto block_15;
            case 6:
                this->actor.freezeTimer = this->collider.elements[0].base.acHitElem->atDmgInfo.damage;
                break;
            case 8:
            case 9:
            case 15:
                if (this->action != BB_STUNNED) {
                    Actor_ApplyDamage(&this->actor);
                    EnBb_SetupStunned(this);
                }
                break;
            default:
            block_15:
                if ((this->dmgEffect == 14) || (this->dmgEffect == 12) || (this->dmgEffect == 11) ||
                    (this->dmgEffect == 10) || (this->dmgEffect == 7) || (this->dmgEffect == 5)) {
                    if ((this->action != BB_DOWN) || (this->timer < 190)) {
                        Actor_ApplyDamage(&this->actor);
                    }
                    if ((this->action != BB_DOWN) && (this->actor.params != ENBB_WHITE)) {
                        EnBb_SetupDown(this);
                    }
                } else {
                    if (((this->action == BB_DOWN) && (this->timer < 190)) ||
                        ((this->actor.params != ENBB_WHITE) && (this->flameScaleX < 20.0f))) {
                        Actor_ApplyDamage(&this->actor);
                    } else {
                        this->collider.base.acFlags |= AC_HIT;
                    }
                }
                if (this->actor.colChkInfo.health == 0) {
                    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
                    if (this->actor.params == ENBB_RED) {
                        EnBb_KillFlameTrail(this);
                    }
                    EnBb_SetupDeath(this, play);
                } else if ((this->actor.params == ENBB_WHITE) &&
                           ((this->action == BB_WHITE) || (this->action == BB_STUNNED))) {
                    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 12);
                    this->actor.speed = -8.0f;
                    this->maxSpeedXZ = 0.0f;
                    this->actor.world.rot.y = this->actor.yawTowardsPlayer;
                    Actor_PlaySfx(&this->actor, NA_SE_EN_BUBLE_DAMAGE);
                } else if (((this->action == BB_DOWN) && (this->timer < 190)) ||
                           ((this->actor.params != ENBB_WHITE) && (this->flameScaleX < 20.0f))) {
                    EnBb_SetupDamage(this);
                }
                FALLTHROUGH;
            case 13:
                break;
        }
    }
}

void EnBb_Update(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    EnBb* this = (EnBb*)thisx;
    Vec3f sp4C = { 0.0f, 0.0f, 0.0f };
    Vec3f sp40 = { 0.0f, -0.6f, 0.0f };
    Color_RGBA8 sp3C = { 0, 0, 255, 255 };
    Color_RGBA8 sp38 = { 0, 0, 0, 0 };
    f32 sp34 = -15.0f;

    if (this->actor.params <= ENBB_BLUE) {
        EnBb_CollisionCheck(this, play);
    }
    if (this->actor.colChkInfo.damageEffect != 0xD) {
        this->actionFunc(this, play);
        if ((this->actor.params <= ENBB_BLUE) && (this->actor.speed >= -6.0f) &&
            !(this->actor.flags & ACTOR_FLAG_ATTACHED_TO_ARROW)) {
            Actor_MoveXZGravity(&this->actor);
        }
        if (this->moveMode == BBMOVE_NORMAL) {
            if ((this->actor.world.pos.y - 20.0f) <= this->actor.floorHeight) {
                sp34 = 20.0f;
            }
            Actor_UpdateBgCheckInfo(play, &this->actor, sp34, 25.0f, 20.0f,
                                    UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2);
        }
        if (this->actor.params == ENBB_WHITE && this->action == BB_WHITE && this->actor.colChkInfo.health &&
            !(this->actor.flags & ACTOR_FLAG_ATTACHED_TO_ARROW)) {
            Actor_UpdateBgCheckInfo(play, &this->actor, 0.f, 25.f, 0.f, UPDBGCHECKINFO_FLAG_0);
            if (this->actor.bgCheckFlags & BGCHECKFLAG_WALL) {
                EnBb_SetWaypoint(this, play);
                EnBb_FaceWaypoint(this);
            }
        }
        this->actor.focus.pos = this->actor.world.pos;
        this->collider.elements[0].dim.worldSphere.center.x = this->actor.world.pos.x;
        this->collider.elements[0].dim.worldSphere.center.y =
            this->actor.world.pos.y + (this->actor.shape.yOffset * this->actor.scale.y);
        this->collider.elements[0].dim.worldSphere.center.z = this->actor.world.pos.z;

        if ((this->action > BB_KILL) && ((this->actor.speed != 0.0f) || (this->action == BB_GREEN))) {
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
        }
        if ((this->action > BB_FLAME_TRAIL) &&
            ((this->actor.colorFilterTimer == 0) || !(this->actor.colorFilterParams & 0x4000)) &&
            (this->moveMode != BBMOVE_HIDDEN)) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
            CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
        }
    }
}

void EnBb_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    EnBb* this = (EnBb*)thisx;

    BodyBreak_SetInfo(&this->bodyBreak, limbIndex, 4, 15, 15, dList, BODYBREAK_OBJECT_SLOT_DEFAULT);
}

static Vec3f sFireIceOffsets[] = {
    { 13.0f, 10.0f, 0.0f }, { 5.0f, 25.0f, 5.0f },   { -5.0f, 25.0f, 5.0f },  { -13.0f, 10.0f, 0.0f },
    { 5.0f, 25.0f, -5.0f }, { -5.0f, 25.0f, -5.0f }, { 0.0f, 10.0f, -13.0f }, { 5.0f, 0.0f, 5.0f },
    { 5.0f, 0.0f, -5.0f },  { 0.0f, 10.0f, 13.0f },  { -5.0f, 0.0f, 5.0f },   { -5.0f, 0.0f, -5.0f },
};

static s32 EnBb_TickFireIce(EnBb* this) {
    if (this->fireIceTimer <= 0)
        return -1;
    if ((this->actor.colorFilterParams & 0xff) && this->actor.colorFilterTimer < 255)
        this->actor.colorFilterTimer++;
    this->fireIceTimer--;
    s32 index = this->fireIceTimer >> 2;
    return !(this->fireIceTimer % 4) && index < ARRAY_COUNT(sFireIceOffsets) ? index : -1;
}

void EnBb_Draw(Actor* thisx, PlayState* play) {
    s32 pad;
    EnBb* this = (EnBb*)thisx;
    Vec3f blureBase1 = { 0.0f, 5000.0f, 0.0f };
    Vec3f blureBase2 = { 0.0f, 2000.0f, 0.0f };
    Vec3f blureVtx1;
    Vec3f blureVtx2;

    OPEN_DISPS(play->state.gfxCtx);

    blureBase1.z = this->maxSpeedXZ * 80.0f;
    blureBase2.z = this->maxSpeedXZ * 80.0f;
    if (this->moveMode != BBMOVE_HIDDEN) {
        if (this->actor.params <= ENBB_BLUE) {
            Gfx_SetupDL_25Opa(play->state.gfxCtx);
            SkelAnime_DrawOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, NULL, EnBb_PostLimbDraw,
                              this);

            if (this->fireIceTimer != 0) {
                s32 index = EnBb_TickFireIce(this);
                if (index >= 0) {
                    Vec3f sp70;
                    sp70.x = this->actor.world.pos.x + sFireIceOffsets[index].x;
                    sp70.y = this->actor.world.pos.y + sFireIceOffsets[index].y;
                    sp70.z = this->actor.world.pos.z + sFireIceOffsets[index].z;

                    if ((this->dmgEffect != 7) && (this->dmgEffect != 5)) {
                        EffectSsEnIce_SpawnFlyingVec3f(play, &this->actor, &sp70, 0x96, 0x96, 0x96, 0xFA, 0xEB, 0xF5,
                                                       0xFF, 0.8f);
                    } else {
                        sp70.y -= 17.0f;
                        EffectSsEnFire_SpawnVec3f(play, &this->actor, &sp70, 0x28, 1, 0, -1);
                    }
                }
            }
            Matrix_Translate(0.0f, this->flameScaleX * -40.0f, 0.0f, MTXMODE_APPLY);
        } else {
            Matrix_Translate(0.0f, -40.0f, 0.0f, MTXMODE_APPLY);
        }
        if (this->actor.params != ENBB_WHITE) {
            Gfx_SetupDL_25Xlu(play->state.gfxCtx);
            gSPSegment(POLY_XLU_DISP++, 0x08,
                       Gfx_TwoTexScroll(play->state.gfxCtx, G_TX_RENDERTILE, 0, 0, 0x20, 0x40, 1, 0,
                                        ((play->gameplayFrames + (this->flameScrollMod * 10)) *
                                         (-20 - (this->flameScrollMod * -2))) %
                                            0x200,
                                        0x20, 0x80));
            gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 255, 255, this->flamePrimBlue, this->flamePrimAlpha);
            gDPSetEnvColor(POLY_XLU_DISP++, this->flameEnvColor.r, this->flameEnvColor.g, this->flameEnvColor.b, 0);
            Matrix_RotateY(
                BINANG_TO_RAD((s16)(Camera_GetCamDirYaw(GET_ACTIVE_CAM(play)) - this->actor.shape.rot.y + 0x8000)),
                MTXMODE_APPLY);
            Matrix_Scale(this->flameScaleX * 0.01f, this->flameScaleY * 0.01f, 1.0f, MTXMODE_APPLY);
            MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
            gSPDisplayList(POLY_XLU_DISP++, gEffFire1DL);
        } else {
            Matrix_MultVec3f(&blureBase1, &blureVtx1);
            Matrix_MultVec3f(&blureBase2, &blureVtx2);
            if ((this->maxSpeedXZ != 0.0f) && (this->action == BB_WHITE) && !(play->gameplayFrames & 1) &&
                (this->actor.colChkInfo.health != 0)) {
                void* blure = Effect_GetByIndex(this->blureIdx);
                if (blure)
                    EffectBlure_AddVertex(blure, &blureVtx1, &blureVtx2);
            } else if (this->action != BB_WHITE) {
                EffectBlure_AddSpace(Effect_GetByIndex(this->blureIdx));
            }
        }
    }
    CLOSE_DISPS();
}

OVL_INFO_NATIVE_ENEMY(0x69, En_Bb_Profile);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_BB_INIT, EnBb_Init);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_BB_DESTROY, EnBb_Destroy);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_BB_UPDATE, EnBb_Update);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_BB_DRAW, EnBb_Draw);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_BB_PROFILE, En_Bb_Profile);
static const u32 kNativePortOrigin[] = { 0x4e505431, 0, 0x69, 0x5d, 0xc2c8cb54 };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_BB_ORIGIN, kNativePortOrigin);
