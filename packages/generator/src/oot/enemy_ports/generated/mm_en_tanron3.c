#if defined(GAME_MM)
#include <combo.h>
#include <combo/actor_ovl.h>
#include <combo/global.h>
#include <combo/sfx.h>
#include "types/en_tanron3.h"
#undef KREG
#define KREG(index) 0
#define PLAYER_STATE2_80 PLAYER_STATE2_MM_80
#define UPDBGCHECKINFO_FLAG_1 UPDBGCHECKINFO_FLAG_MM_1
#define UPDBGCHECKINFO_FLAG_4 UPDBGCHECKINFO_FLAG_MM_4
#define UPDBGCHECKINFO_FLAG_8 UPDBGCHECKINFO_FLAG_MM_8
#define UPDBGCHECKINFO_FLAG_10 UPDBGCHECKINFO_FLAG_MM_10
#define gGyorgSmallFishSkel (*(FlexSkeletonHeader*)0x0600da20)
#define gGyorgSmallFishSwimAnim (*(AnimationHeader*)0x0600daac)

#undef Actor_Kill
extern u8 FOREIGN_ENEMY_API_KILL[];
#define Actor_Kill ((void (*)(Actor* actor))0x800b670c)
#undef Actor_MoveWithGravity
#define Actor_MoveWithGravity ((void (*)(Actor* actor))0x800b6a88)
#undef Actor_MoveWithoutGravityReverse
#define Actor_MoveWithoutGravityReverse ((void (*)(Actor* actor))0x800b6bd8)
#undef Actor_PlaySfx
#define Actor_PlaySfx ((void (*)(Actor* actor, u16 sfxId))0x800b8ec8)
#undef Actor_SetScale
#define Actor_SetScale ((void (*)(Actor* actor, f32 scale))0x800b67e0)
#undef Actor_UpdateBgCheckInfo
#define Actor_UpdateBgCheckInfo ((void (*)(PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight, u32 updBgCheckInfoFlags))0x800b78b8)
#undef Animation_MorphToLoop
#define Animation_MorphToLoop ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 morphFrames))0x80137594)
#undef Collider_InitAndSetCylinder
#define Collider_InitAndSetCylinder ((s32 (*)(PlayState* play, ColliderCylinder* dest, Actor* actor, ColliderCylinderInit* src))0x800e1374)
#undef Collider_UpdateCylinder
#define Collider_UpdateCylinder ((void (*)(Actor* actor, ColliderCylinder* cyl))0x800e7df8)
#undef CollisionCheck_SetAC
#define CollisionCheck_SetAC ((s32 (*)(PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))0x800e2740)
#undef CollisionCheck_SetAT
#define CollisionCheck_SetAT ((s32 (*)(PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))0x800e2558)
#undef EffectSsDtBubble_SpawnCustomColor
#define EffectSsDtBubble_SpawnCustomColor ((void (*)(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 scale, s16 life, s16 randXZ))0x800b210c)
#undef EffectSsGSplash_Spawn
#define EffectSsGSplash_Spawn ((void (*)(PlayState* play, Vec3f* pos, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 type, s16 scale))0x800b1f4c)
#undef Gfx_SetFog
#define Gfx_SetFog ((Gfx* (*)(Gfx* gfx, s32 r, s32 g, s32 b, s32 a, s32 n, s32 f))0x8012bc50)
#undef Gfx_SetupDL25_Opa
#define Gfx_SetupDL25_Opa ((void (*)(GraphicsContext* gfxCtx))0x8012c28c)
#undef Item_DropCollectibleRandom
#define Item_DropCollectibleRandom ((void (*)(PlayState* play, Actor* fromActor, Vec3f* spawnPos, s16 params))0x800a7d28)
#undef Math_ApproachF
#define Math_ApproachF ((void (*)(f32* pValue, f32 target, f32 scale, f32 maxStep))0x800ffdf8)
#undef Math_ApproachS
#define Math_ApproachS ((void (*)(s16* pValue, s16 target, s16 scale, s16 maxStep))0x800fffd8)
#undef Math_Atan2S_XY
#define Math_Atan2S_XY ((s16 (*)(f32 x, f32 y))0x80180100)
#undef Math_SinS
#define Math_SinS ((f32 (*)(s16 angle))0x800fed84)
#undef Math_SmoothStepToS
#define Math_SmoothStepToS ((s16 (*)(s16* pValue, s16 target, s16 scale, s16 step, s16 minStep))0x800ffebc)
#undef Math_Vec3f_Copy
#define Math_Vec3f_Copy ((void (*)(Vec3f* dest, Vec3f* src))0x800ff50c)
#undef Matrix_MultVecZ
#define Matrix_MultVecZ ((void (*)(f32 z, Vec3f* dest))0x80181c10)
#undef Matrix_RotateXFApply
#define Matrix_RotateXFApply ((void (*)(f32 x))0x801807b8)
#undef Matrix_RotateYF
#define Matrix_RotateYF ((void (*)(f32 y, MatrixMode mode))0x80180b48)
#undef Matrix_RotateYS
#define Matrix_RotateYS ((void (*)(s16 y, MatrixMode mode))0x801809ac)
#undef Play_SetFog
#define Play_SetFog ((Gfx* (*)(PlayState* this, Gfx* gfx))0x801660b8)
#undef Rand_CenteredFloat
#define Rand_CenteredFloat ((f32 (*)(f32 scale))0x801795c0)
#undef Rand_ZeroFloat
#define Rand_ZeroFloat ((f32 (*)(f32 scale))0x80179594)
#undef Rand_ZeroOne
#define Rand_ZeroOne ((f32 (*)(void))0x80086fdc)
#undef SkelAnime_DrawFlexOpa
#define SkelAnime_DrawFlexOpa ((void (*)(PlayState* play, void** skeleton, Vec3s* jointTable, s32 dListCount, void* overrideLimbDraw, void* postLimbDraw, Actor* actor))0x80133f28)
#undef SkelAnime_InitFlex
#define SkelAnime_InitFlex ((void (*)(PlayState* play, SkelAnime* skelAnime, FlexSkeletonHeader* skeletonHeaderSeg, AnimationHeader* animation, Vec3s* jointTable, Vec3s* morphTable, s32 limbCount))0x80136b30)
#undef func_800B8D50
#define func_800B8D50 ((void (*)(PlayState* play, Actor* actor, f32 arg2, s16 yaw, f32 arg4, u32 arg5))0x800b8d50)
#else
#include "mm_native_compat.h"
#define gGyorgSmallFishSkel (*(FlexSkeletonHeader*)0x0600da20)
#define gGyorgSmallFishSwimAnim (*(AnimationHeader*)0x0600daac)

#include "types/en_tanron3.h"
#undef ACTOR_EN_TANRON3
#define ACTOR_EN_TANRON3 0x23f
#undef OBJECT_BOSS03
#define OBJECT_BOSS03 0x115c
#undef NA_SE_EV_OUT_OF_WATER
#define NA_SE_EV_OUT_OF_WATER 0x2808
#undef NA_SE_EN_PIRANHA_ATTACK
#define NA_SE_EN_PIRANHA_ATTACK 0x39f4
#undef NA_SE_EN_KONB_MINI_DEAD
#define NA_SE_EN_KONB_MINI_DEAD 0x3ab6
#undef Collider_InitAndSetCylinder
#define Collider_InitAndSetCylinder ((s32 (*)(struct PlayState* play, ColliderCylinder* dest, struct Actor* actor, ColliderCylinderInit* src))(void*)FOREIGN_ENEMY_API_CYLINDER_INIT_SET)
#undef Matrix_RotateXFApply
#define Matrix_RotateXFApply ((void (*)(f32 x))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_XF_APPLY)
#undef Matrix_MultVecZ
#define Matrix_MultVecZ ((void (*)(f32 z, Vec3f* dest))(void*)FOREIGN_ENEMY_API_MATRIX_MULT_Z)
#undef Actor_PlaySfx
#define Actor_PlaySfx ((void (*)(Actor* actor, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_ACTOR)
#undef Math_Atan2S_XY
#define Math_Atan2S_XY ((s16 (*)(f32 x, f32 y))(void*)FOREIGN_ENEMY_API_ATAN2S_XY)
#undef Matrix_RotateYS
#define Matrix_RotateYS ((void (*)(s16 y, MatrixMode mode))(void*)FOREIGN_ENEMY_API_MATRIX_ROTATE_YS)
#undef Actor_Kill
extern u8 FOREIGN_ENEMY_API_KILL[];
#define Actor_Kill ((void (*)(Actor* actor))(void*)FOREIGN_ENEMY_API_KILL)
#undef Item_DropCollectibleRandom
#define Item_DropCollectibleRandom ((void (*)(struct PlayState* play, Actor* fromActor, Vec3f* spawnPos, s16 params))(void*)FOREIGN_ENEMY_API_RANDOM_DROP)
#undef func_800B8D50
#define func_800B8D50 ((void (*)(struct PlayState* play, Actor* actor, f32 arg2, s16 yaw, f32 arg4, u32 arg5))(void*)FOREIGN_ENEMY_API_KNOCKBACK_LARGE)
#undef CollisionCheck_SetAT
#define CollisionCheck_SetAT ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AT)
#undef CollisionCheck_SetAC
#define CollisionCheck_SetAC ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AC)
#undef Rand_ZeroFloat
#define Rand_ZeroFloat ((f32 (*)(f32 scale))0x80026d64u)
#undef Rand_CenteredFloat
#define Rand_CenteredFloat ((f32 (*)(f32 scale))0x80026d90u)
#undef SkelAnime_InitFlex
#define SkelAnime_InitFlex ((void (*)(struct PlayState* play, SkelAnime* skelAnime, FlexSkeletonHeader* skeletonHeaderSeg, AnimationHeader* animation, Vec3s* jointTable, Vec3s* morphTable, s32 limbCount))0x8008c788u)
#undef Actor_SetScale
#define Actor_SetScale ((void (*)(Actor* actor, f32 scale))0x80020f88u)
#undef EffectSsDtBubble_SpawnCustomColor
#define EffectSsDtBubble_SpawnCustomColor ((void (*)(struct PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 scale, s16 life, s16 randXZ))0x8001d18cu)
#undef Animation_MorphToLoop
#define Animation_MorphToLoop ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 morphFrames))0x8008d328u)
#undef Math_Vec3f_Copy
#define Math_Vec3f_Copy ((void (*)(Vec3f* dest, Vec3f* src))0x80063cacu)
#undef Rand_ZeroOne
#define Rand_ZeroOne ((f32 (*)(void))0x800cdcccu)
#undef Math_ApproachS
#define Math_ApproachS ((void (*)(s16* pValue, s16 target, s16 scale, s16 maxStep))0x80064624u)
#undef Math_SmoothStepToS
#define Math_SmoothStepToS ((s16 (*)(s16* pValue, s16 target, s16 scale, s16 step, s16 minStep))0x80064508u)
#undef Math_ApproachF
#define Math_ApproachF ((void (*)(f32* pValue, f32 target, f32 scale, f32 maxStep))0x80064280u)
#undef Actor_MoveWithGravity
#define Actor_MoveWithGravity ((void (*)(Actor* actor))0x8002121cu)
#undef Math_SinS
#define Math_SinS ((f32 (*)(s16 angle))0x800636c4u)
#undef Actor_UpdateBgCheckInfo
#define Actor_UpdateBgCheckInfo ((void (*)(struct PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight, u32 updBgCheckInfoFlags))0x80021e6cu)
#undef EffectSsGSplash_Spawn
#define EffectSsGSplash_Spawn ((void (*)(struct PlayState* play, Vec3f* pos, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 type, s16 scale))0x8001cf94u)
#undef Collider_UpdateCylinder
#define Collider_UpdateCylinder ((void (*)(struct Actor* actor, ColliderCylinder* cyl))0x80050b00u)
#undef Gfx_SetupDL25_Opa
#define Gfx_SetupDL25_Opa ((void (*)(GraphicsContext* gfxCtx))0x8007e298u)
#undef Gfx_SetFog
#define Gfx_SetFog ((Gfx* (*)(Gfx* gfx, s32 r, s32 g, s32 b, s32 a, s32 n, s32 f))0x8007dc40u)
#undef SkelAnime_DrawFlexOpa
#define SkelAnime_DrawFlexOpa ((void (*)(struct PlayState* play, void** skeleton, Vec3s* jointTable, s32 dListCount, GeneratedMmOverrideLimbDraw overrideLimbDraw, GeneratedMmPostLimbDraw postLimbDraw, struct Actor* actor))0x80089d8cu)
#undef Play_SetFog
#define Play_SetFog ((Gfx* (*)(PlayState* this, Gfx* gfx))0x8009a574u)
#endif

#define FLAGS                                                                                 \
    (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED | \
     ACTOR_FLAG_DRAW_CULLING_DISABLED)

#define WORK_TIMER_PICK_NEW_DEVIATION 0
#define WORK_TIMER_DIE 0
#define WORK_TIMER_OUT_OF_WATER 1
#define WORK_TIMER_ATTACK 2
#define WORK_TIMER_WAIT 2

void EnTanron3_Init(Actor* thisx, PlayState* play);
void EnTanron3_Destroy(Actor* thisx, PlayState* play);
void EnTanron3_Update(Actor* thisx, PlayState* play);
void EnTanron3_Draw(Actor* thisx, PlayState* play);

void EnTanron3_SetupLive(EnTanron3* this, PlayState* play);
void EnTanron3_Live(EnTanron3* this, PlayState* play);
void EnTanron3_Die(EnTanron3* this, PlayState* play);

static const Vec3f sZeroVec = { 0.0f, 0.0f, 0.0f };

static Actor* enemyGarahnaBoss(EnTanron3* this) {
#if defined(GAME_MM)
    Actor* parent = this->actor.parent;
    return parent && parent->id == 0x12b ? parent : NULL;
#else
    return NULL;
#endif
}

static s16 enemyGarahnaAlert(EnTanron3* this) {
    Actor* boss = enemyGarahnaBoss(this);
    return boss ? *(s16*)((u8*)boss + 0x324) : 0;
}

static s32 enemyGarahnaWater(PlayState* play, f32 x, f32 z, f32* surface, WaterBox** water) {
#if defined(GAME_MM)
    return ((s32(*)(PlayState*, CollisionContext*, f32, f32, f32*, WaterBox**))0x800ca1ac)(play, &play->colCtx, x, z,
                                                                                           surface, water);
#else
    return ((s32(*)(PlayState*, CollisionContext*, f32, f32, f32*, WaterBox**))0x80034908)(play, &play->colCtx, x, z,
                                                                                           surface, water);
#endif
}

static void enemyGarahnaMove(Actor* actor) {
#if defined(GAME_MM)
    ((void (*)(Actor*))0x800b6bd8)(actor);
#else
    s16 pitch = actor->world.rot.x;
    actor->world.rot.x = -pitch;
    ((void (*)(Actor*))0x800212b8)(actor);
    actor->world.rot.x = pitch;
#endif
}

typedef struct {
    u8 type;
    s16 unk_02;
    Vec3f pos, velocity, accel;
    u8 pad28[4];
    s16 alpha, alphaDelta;
    u8 pad30[4];
    Vec3f unk_34;
    f32 unk_40;
} EnemyGarahnaBossEffect;
_Static_assert(sizeof(EnemyGarahnaBossEffect) == 0x44, "Native Gyorg effect record");

ActorProfile En_Tanron3_Profile = {
#if defined(GAME_MM)
      0x23f,
#else
      0xe3f,
#endif
    #if defined(GAME_MM)
    ACTORCAT_BOSS,
#else
    ACTORCAT_ENEMY,
#endif
      FLAGS,
#if defined(GAME_MM)
      0x15c,
#else
      0x115c,
#endif
      sizeof(EnTanron3),
    EnTanron3_Init,
    EnTanron3_Destroy,
    EnTanron3_Update,
    EnTanron3_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COL_MATERIAL_HIT3,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK3,
        { 0xF7CFFFFF, 0x00, 0x02 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        ATELEM_ON | ATELEM_SFX_NORMAL,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 7, 10, -5, { 0, 0, 0 } },
};

void EnTanron3_CreateEffect(EnTanron3* this, PlayState* play, Vec3f* effectPos) {
    if (enemyGarahnaBoss(this) && play->specialEffects) {

        EnemyGarahnaBossEffect* effectPtr = play->specialEffects;
        s16 i;

        for (i = 0; i < 150; i++, effectPtr++) {
            if ((effectPtr->type == 0) || (effectPtr->type == 1)) {
                effectPtr->type = 2;
                effectPtr->pos = *effectPos;
                effectPtr->velocity = sZeroVec;
                effectPtr->accel = sZeroVec;
                effectPtr->accel.y = -2.0f;
                effectPtr->unk_34.x = 0.1f;
                effectPtr->unk_34.y = 0.0f;
                effectPtr->unk_34.z = Rand_ZeroFloat(2 * M_PIf);
                effectPtr->unk_02 = Rand_ZeroFloat(100.0f);
                effectPtr->velocity.x = Rand_CenteredFloat(25.0f);
                effectPtr->velocity.z = Rand_CenteredFloat(25.0f);
                break;
            }
        }

        return;
    }
    EffectSsGSplash_Spawn(play, effectPos, NULL, NULL, 1, 150);
}

void EnTanron3_Init(Actor* thisx, PlayState* play) {
    EnTanron3* this = (EnTanron3*)thisx;

    this->actor.gravity = -1.0f;
    Collider_InitAndSetCylinder(play, &this->atCollider, &this->actor, &sCylinderInit);
    Collider_InitAndSetCylinder(play, &this->acCollider, &this->actor, &sCylinderInit);
    SkelAnime_InitFlex(play, &this->skelAnime, &gGyorgSmallFishSkel, &gGyorgSmallFishSwimAnim, this->jointTable,
                       this->morphTable, GYORG_SMALL_FISH_LIMB_MAX);
    Actor_SetScale(&this->actor, 0.02f);
    EnTanron3_SetupLive(this, play);
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    this->currentRotationAngle = Rand_ZeroFloat(500000.0f);
    this->waterSurfaceYPos = 430.0f;
    this->standaloneRadius = 700.0f;
    this->bossOwned = enemyGarahnaBoss(this) != NULL;

    if (enemyGarahnaBoss(this) == NULL) {
        WaterBox* water;
        f32 radius, edge;
        EnTanron3* owner = thisx->parent && thisx->parent->id == thisx->id ? (EnTanron3*)thisx->parent : NULL;
        if (thisx->params != 0 ||
            (!owner && !enemyGarahnaWater(play, this->actor.home.pos.x, this->actor.home.pos.z, &this->waterSurfaceYPos, &water))) {
            Actor_Kill(thisx);
            return;
        }
        if (owner) {
            this->waterSurfaceYPos = owner->waterSurfaceYPos;
            radius = owner->standaloneRadius;
        } else {
            radius = this->actor.home.pos.x - water->xMin;
            edge = water->xMin + water->xLength - this->actor.home.pos.x;
            if (edge < radius)
                radius = edge;
            edge = this->actor.home.pos.z - water->zMin;
            if (edge < radius)
                radius = edge;
            edge = water->zMin + water->zLength - this->actor.home.pos.z;
            if (edge < radius)
                radius = edge;
            radius -= 20.0f;
            if (radius < 20.0f)
                radius = 20.0f;
            if (radius > 700.0f)
                radius = 700.0f;
        }
        this->standaloneRadius = radius;
        this->targetPos = this->actor.home.pos;
        if (this->actor.world.pos.y >= this->waterSurfaceYPos)
            this->actor.world.pos.y = this->waterSurfaceYPos - 30.0f;
        this->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED;
        if (!owner) {
            this->swarmOwner = 1;
            this->swarm.flags = ENEMY_FAMILY_NO_CLEAR;
            this->actor.colChkInfo.health = 1;
            this->actor.draw = NULL;
            this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
            this->actor.flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
        }
#if defined(GAME_MM)
        ((void (*)(PlayState*, ActorContext*, Actor*, u8))0x800bc154)(play, &play->actorCtx, thisx, ACTORCAT_ENEMY);
#endif
    }
}

void EnTanron3_Destroy(Actor* thisx, PlayState* play) {
    if (((EnTanron3*)thisx)->swarmOwner) {
        for (s32 category = 0; category < ACTORCAT_MAX; ++category)
            for (Actor* child = play->actorCtx.actors[category].first; child; child = child->next)
                if (child->parent == thisx && child->id == thisx->id)
                    child->parent = NULL;
        thisx->child = NULL;
        comboEnemyFamilyClear(&((EnTanron3*)thisx)->swarm);
    }
    Actor* boss = enemyGarahnaBoss((EnTanron3*)thisx);
    if (boss != NULL)
        (*(s8*)((u8*)boss + 0x252))--;
    Collider_DestroyCylinder(play, &((EnTanron3*)thisx)->atCollider);
    Collider_DestroyCylinder(play, &((EnTanron3*)thisx)->acCollider);
}

void EnTanron3_SpawnBubbles(EnTanron3* this, PlayState* play) {
    static Color_RGBA8 sPrimColor = { 100, 55, 55, 255 };
    static Color_RGBA8 sEnvColor = { 50, 10, 10, 255 };
    s32 i;
    Vec3f velocity;
    Vec3f accel;

    for (i = 0; i < 20; i++) {
        Matrix_RotateYF(Rand_ZeroFloat(2 * M_PIf), MTXMODE_NEW);
        Matrix_RotateXFApply(Rand_ZeroFloat(2 * M_PIf));
        Matrix_MultVecZ(Rand_ZeroFloat(3.0f) + 2.0f, &velocity);
        accel.x = velocity.x * -0.05f;
        accel.y = velocity.y * -0.05f;
        accel.z = velocity.z * -0.05f;
        EffectSsDtBubble_SpawnCustomColor(play, &this->actor.world.pos, &velocity, &accel, &sPrimColor, &sEnvColor,
                                          Rand_ZeroFloat(30.0f) + 70.0f, Rand_ZeroFloat(5.0f) + 15.0f, false);
    }
}

void EnTanron3_SetupLive(EnTanron3* this, PlayState* play) {
    this->actionFunc = EnTanron3_Live;
    Animation_MorphToLoop(&this->skelAnime, &gGyorgSmallFishSwimAnim, -10.0f);
    this->rotationStep = 0;
    this->rotationScale = 5;
    this->workTimer[WORK_TIMER_PICK_NEW_DEVIATION] = 50;
    this->actor.speed = 5.0f;
    this->speedMaxStep = 0.5f;
    this->deviation.x = Rand_CenteredFloat(500.0f);
    this->deviation.y = Rand_CenteredFloat(100.0f);
    this->deviation.z = Rand_CenteredFloat(500.0f);
    Math_Vec3f_Copy(&this->targetPos, &this->actor.world.pos);
    this->timer = Rand_ZeroFloat(100.0f);
}

void EnTanron3_Live(EnTanron3* this, PlayState* play) {
    s32 atanTemp;
    f32 xDistance;
    f32 yDistance;
    f32 zDistance;
    f32 xzDistance;
    f32 extraScaleY = 0.0f;
    Player* player = GET_PLAYER(play);

    this->skelAnime.curFrame = 4.0f;
    if ((player->actor.bgCheckFlags & BGCHECKFLAG_GROUND) &&
        (player->actor.shape.feetPos[0].y >= (enemyGarahnaBoss(this) ? 438.0f : this->waterSurfaceYPos + 8.0f))) {
        this->isNonHostile = true;
    } else if (this->isNonHostile && (this->workTimer[WORK_TIMER_WAIT] == 0) && !(this->timer & 0x1F)) {
        xDistance = this->targetPos.x - player->actor.world.pos.x;
        zDistance = this->targetPos.z - player->actor.world.pos.z;
        if (sqrtf(SQ(xDistance) + SQ(zDistance)) < 500.0f) {
            this->isNonHostile = false;
            this->workTimer[WORK_TIMER_ATTACK] = 150;
        }
    }

    if (this->actor.world.pos.y < this->waterSurfaceYPos) {
        this->isBeached = false;
        switch (this->isNonHostile) {
            case false:
                this->targetSpeedXZ = 5.0f;
                this->targetRotationStep = 0x1000;
                this->nextRotationAngle = 0x3A98;

                Math_Vec3f_Copy(&this->targetPos, &player->actor.world.pos);
                if (!(this->timer & 0xF)) {
                    if ((Rand_ZeroOne() < 0.5f) && (this->actor.xzDistToPlayer <= 200.0f)) {
                        Actor_PlaySfx(&this->actor, NA_SE_EN_PIRANHA_ATTACK);
                    }
                }

                if ((this->workTimer[WORK_TIMER_ATTACK] == 0) || (player->stateFlags2 & PLAYER_STATE2_80)) {
                    this->workTimer[WORK_TIMER_WAIT] = 150;
                    this->isNonHostile = true;
                }
                break;

            case true:
                if ((enemyGarahnaAlert(this) != 0) && !(this->timer & 0x7)) {
                    this->nextRotationAngle = 0x4E20;
                    this->actor.speed = 6.0f;
                } else {
                    this->nextRotationAngle = 0x1F40;
                }
                this->targetRotationStep = 0x200;
                this->targetSpeedXZ = 2.0f;

                atanTemp = Math_Atan2S_XY(this->targetPos.z - (enemyGarahnaBoss(this) ? 0.0f : this->actor.home.pos.z),
                                          this->targetPos.x - (enemyGarahnaBoss(this) ? 0.0f : this->actor.home.pos.x));
                Matrix_RotateYS(atanTemp, MTXMODE_NEW);
                Matrix_MultVecZ(this->standaloneRadius, &this->targetPos);
                if (enemyGarahnaBoss(this))
                    this->targetPos.y = 250.0f;
                else {
                    this->targetPos.x += this->actor.home.pos.x;
                    this->targetPos.z += this->actor.home.pos.z;
                    this->targetPos.y = this->actor.home.pos.y - 50.0f;
                }

                extraScaleY = 150.0f;
                break;

            default:
                break;
        }

        if (this->workTimer[WORK_TIMER_OUT_OF_WATER] == 0) {
            if ((this->workTimer[WORK_TIMER_PICK_NEW_DEVIATION] == 0) && (this->actor.speed > 1.0f)) {
                this->workTimer[WORK_TIMER_PICK_NEW_DEVIATION] = Rand_ZeroFloat(20.0f);
                this->deviation.x = Rand_CenteredFloat(100.0f);
                this->deviation.y = Rand_CenteredFloat(50.0f + extraScaleY);
                this->deviation.z = Rand_CenteredFloat(100.0f);
            }
            this->targetPosWithDeviation.y = this->targetPos.y + this->deviation.y + 50.0f;
        }

        this->targetPosWithDeviation.x = this->targetPos.x + this->deviation.x;
        this->targetPosWithDeviation.z = this->targetPos.z + this->deviation.z;
        xDistance = this->targetPosWithDeviation.x - this->actor.world.pos.x;
        yDistance = this->targetPosWithDeviation.y - this->actor.world.pos.y;
        zDistance = this->targetPosWithDeviation.z - this->actor.world.pos.z;

        xzDistance = sqrtf(SQ(xDistance) + SQ(zDistance));
        atanTemp = Math_Atan2S_XY(xzDistance, -yDistance);
        Math_ApproachS(&this->actor.world.rot.x, atanTemp, this->rotationScale, this->rotationStep);
        atanTemp = Math_Atan2S_XY(zDistance, xDistance);
        Math_SmoothStepToS(&this->actor.world.rot.y, atanTemp, this->rotationScale, this->rotationStep, 0);
        Math_ApproachS(&this->rotationStep, this->targetRotationStep, 1, 0x100);

        Math_ApproachF(&this->actor.speed, this->targetSpeedXZ, 1.0f, this->speedMaxStep);
        enemyGarahnaMove(&this->actor);
    } else {
        switch (this->isBeached) {
            case false:
                this->actor.gravity = -1.0f;
                this->targetPosWithDeviation.y = this->waterSurfaceYPos - 50.0f;
                this->workTimer[WORK_TIMER_OUT_OF_WATER] = 25;
                Math_ApproachS(&this->actor.world.rot.x, 0x3000, 5, 0xBD0);
                if (this->actor.bgCheckFlags & BGCHECKFLAG_WALL) {
                    this->actor.speed = 0.0f;
                    if (this->actor.velocity.y > 0.0f) {
                        this->actor.velocity.y = -1.0f;
                    }
                }
                if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
                    this->isBeached = true;
                }
                break;

            case true:
                this->nextRotationAngle = 0x3A98;
                this->actor.gravity = -1.5f;
                if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
                    this->actor.velocity.y = Rand_ZeroFloat(5.0f) + 5.0f;
                    this->actor.speed = Rand_ZeroFloat(2.0f) + 2.0f;
                    if (Rand_ZeroOne() < 0.5f) {
                        this->targetShapeRotation.x =
                            TRUNCF_BINANG(Rand_CenteredFloat(0x1F4)) + this->targetShapeRotation.x + 0x8000;
                    }
                    if (Rand_ZeroOne() < 0.5f) {
                        this->targetShapeRotation.z =
                            TRUNCF_BINANG(Rand_CenteredFloat(0x1F4)) + this->targetShapeRotation.z + 0x8000;
                    }
                    if (Rand_ZeroOne() < 0.5f) {
                        this->targetShapeRotation.y = TRUNCF_BINANG(Rand_ZeroFloat(0x10000));
                    }
                    this->actor.world.rot.y = Math_Atan2S_XY(this->actor.world.pos.z, this->actor.world.pos.x) +
                                              TRUNCF_BINANG(Rand_CenteredFloat(0xCE20));
                }

                Math_ApproachS(&this->actor.shape.rot.y, this->targetShapeRotation.y, 3, 0x500);
                Math_ApproachS(&this->actor.shape.rot.x, this->targetShapeRotation.x, 3, 0xC00);
                Math_ApproachS(&this->actor.shape.rot.z, this->targetShapeRotation.z, 3, 0xC00);
                if ((Rand_ZeroOne() < 0.5f) & !(this->timer & 0x3)) {
                    Vec3f effectPos;

                    effectPos.x = Rand_CenteredFloat(30.0f) + this->actor.world.pos.x;
                    effectPos.y = this->actor.world.pos.y;
                    effectPos.z = Rand_CenteredFloat(30.0f) + this->actor.world.pos.z;
                    EnTanron3_CreateEffect(this, play, &effectPos);
                }
                break;

            default:
                break;
        }
        Actor_MoveWithGravity(&this->actor);
    }

    this->currentRotationAngle += this->nextRotationAngle;
    this->tailRotation = Math_SinS(this->currentRotationAngle) * 5000.0f;
    this->bodyRotation = Math_SinS(this->currentRotationAngle + 0x6978) * 5000.0f;
    this->trunkRotation = Math_SinS(this->currentRotationAngle) * 5000.0f;
    if (!this->isBeached) {
        this->actor.shape.rot = this->actor.world.rot;
    }
}

void EnTanron3_SetupDie(EnTanron3* this, PlayState* play) {
    f32 xDistance;
    f32 yDistance;
    f32 zDistance;
    Player* player = GET_PLAYER(play);

    this->actionFunc = EnTanron3_Die;
    xDistance = this->actor.world.pos.x - player->actor.world.pos.x;
    yDistance = this->actor.world.pos.y - player->actor.world.pos.y + 30.0f;
    zDistance = this->actor.world.pos.z - player->actor.world.pos.z;
    this->actor.world.rot.x = Math_Atan2S_XY(sqrtf(SQ(xDistance) + SQ(zDistance)), -yDistance);
    this->actor.world.rot.y = Math_Atan2S_XY(zDistance, xDistance);
    this->workTimer[WORK_TIMER_DIE] = 6;
    this->actor.speed = 10.0f;
    Actor_PlaySfx(&this->actor, NA_SE_EN_KONB_MINI_DEAD);
}

void EnTanron3_Die(EnTanron3* this, PlayState* play) {
    enemyGarahnaMove(&this->actor);
    if (this->workTimer[WORK_TIMER_DIE] == 0) {
        EnTanron3_SpawnBubbles(this, play);
        Actor_Kill(&this->actor);
        if (Rand_ZeroOne() < 0.3f) {
            Item_DropCollectibleRandom(play, NULL, &this->actor.world.pos, 0x60);
        }
    }
}

void EnTanron3_CheckCollisions(EnTanron3* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (player->actor.world.pos.y > (enemyGarahnaBoss(this) ? 350.0f : this->waterSurfaceYPos - 80.0f)) {
        if (this->atCollider.base.atFlags & AT_HIT) {
            this->atCollider.base.atFlags &= ~AT_HIT;
            func_800B8D50(
                play, NULL, 3.0f,
                Math_Atan2S_XY((enemyGarahnaBoss(this) ? 0.0f : this->actor.home.pos.z) - player->actor.world.pos.z,
                               (enemyGarahnaBoss(this) ? 0.0f : this->actor.home.pos.x) - player->actor.world.pos.x),
                5.0f, 0);
        }
    }
    if (this->acCollider.base.acFlags & AC_HIT) {
        this->acCollider.base.acFlags &= ~AC_HIT;
        if (this->deathTimer == 0) {
            this->deathTimer = 15;
            this->fogTimer = 15;
            EnTanron3_SetupDie(this, play);
            Actor* boss = enemyGarahnaBoss(this);
            if (boss)
                *(s16*)((u8*)boss + 0x324) = 20;
        }
    }
}

void EnTanron3_Update(Actor* thisx, PlayState* play) {
    EnTanron3* this = (EnTanron3*)thisx;
    s16 i;
    Vec3f splashPos;

    if (this->swarmOwner) {
        if (this->swarmRetry) {
            --this->swarmRetry;
            return;
        }
        while (this->swarmSpawned < 10) {
            s16 yaw = this->swarmSpawned * (0x10000 / 10);
            f32 radius = MIN(this->standaloneRadius * .25f, 30.f);
            Vec3f pos = this->actor.home.pos;
            pos.x += radius * Math_SinS(yaw);
            pos.z += radius * Math_CosS(yaw);
            pos.y = MIN(pos.y, this->waterSurfaceYPos - 30.f);
            if (!comboEnemyFamilySpawnVanillaChild(&this->swarm, &play->actorCtx, thisx, play,
                    thisx->id, pos.x, pos.y, pos.z, 0, yaw, 0, 0)) {
                this->swarm.blocked = 0;
                this->swarmRetry = 15;
                return;
            }
            ++this->swarmSpawned;
        }
        if (comboEnemyFamilyDone(&this->swarm)) {
            this->actor.colChkInfo.health = 0;
            Actor_Kill(thisx);
        }
        return;
    }

    if (KREG(63) == 0) {
        this->timer++;

        for (i = 0; i < TANRON3_WORK_TIMER_MAX; i++) {
            if (this->workTimer[i] != 0) {
                this->workTimer[i]--;
            }
        }
        if (this->deathTimer != 0) {
            this->deathTimer--;
        }
        if (this->fogTimer != 0) {
            this->fogTimer--;
        }

        this->actionFunc(this, play);
        Actor_UpdateBgCheckInfo(play, &this->actor, 10.0f, 10.0f, 20.0f, UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_4);

        if (((this->actor.prevPos.y < this->waterSurfaceYPos) && (this->waterSurfaceYPos <= this->actor.world.pos.y)) ||
            ((this->actor.prevPos.y > this->waterSurfaceYPos) && (this->waterSurfaceYPos >= this->actor.world.pos.y))) {
            splashPos.x = this->actor.world.pos.x;
            splashPos.y = this->waterSurfaceYPos + 10.0f;
            splashPos.z = this->actor.world.pos.z;
            EffectSsGSplash_Spawn(play, &splashPos, NULL, NULL, 1, 500);
            Actor_PlaySfx(&this->actor, NA_SE_EV_OUT_OF_WATER);
        }
    }

    EnTanron3_CheckCollisions(this, play);
    Actor_SetFocus(&this->actor, 0.f);
    Collider_UpdateCylinder(&this->actor, &this->atCollider);
    Collider_UpdateCylinder(&this->actor, &this->acCollider);
    CollisionCheck_SetAT(play, &play->colChkCtx, &this->atCollider.base);
    CollisionCheck_SetAC(play, &play->colChkCtx, &this->acCollider.base);

    if (this->bossOwned && (!enemyGarahnaBoss(this) || ((s8)enemyGarahnaBoss(this)->colChkInfo.health <= 0)) &&
        (this->actionFunc != EnTanron3_Die)) {
        EnTanron3_SetupDie(this, play);
        this->workTimer[WORK_TIMER_DIE] = 0;
    }
}

s32 EnTanron3_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx) {
    EnTanron3* this = (EnTanron3*)thisx;

    if (limbIndex == GYORG_SMALL_FISH_LIMB_ROOT) {
        rot->y += this->bodyRotation;
    }

    if (limbIndex == GYORG_SMALL_FISH_LIMB_TRUNK_ROOT) {
        rot->y += this->trunkRotation;
    }

    if (limbIndex == GYORG_SMALL_FISH_LIMB_TAIL_FIN) {
        rot->y += this->tailRotation;
    }

    return false;
}

void EnTanron3_Draw(Actor* thisx, PlayState* play) {
    EnTanron3* this = (EnTanron3*)thisx;

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    if ((this->fogTimer % 2) != 0) {
        POLY_OPA_DISP = Gfx_SetFog(POLY_OPA_DISP, 255, 0, 0, 255, 900, 1099);
    }
    SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount,
                          EnTanron3_OverrideLimbDraw, NULL, &this->actor);
    POLY_OPA_DISP = Play_SetFog(play, POLY_OPA_DISP);

    CLOSE_DISPS();
}

#if defined(GAME_MM)
OVL_INFO_ACTOR(0x23f, En_Tanron3_Profile);
EXPORT_SYMBOL(ENEMY_NATIVE_GARAHNA_MM_V2, EnTanron3_Init);
#else
OVL_INFO_NATIVE_ENEMY(0x23f, En_Tanron3_Profile);
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_MM_EN_TANRON3, En_Tanron3_Profile, EnTanron3_Init, EnTanron3_Destroy,
                     EnTanron3_Update, EnTanron3_Draw);
static const u32 generatedNativeOrigin[] = { 0x4e505431u, 0x1u, 0x23fu, 0x15cu, 0xff71e607u };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_TANRON3_ORIGIN, generatedNativeOrigin);
#endif
