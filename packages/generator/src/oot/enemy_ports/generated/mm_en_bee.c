#include "mm_native_compat.h"
#define gBeeFlyingAnim (*(AnimationHeader*)0x0600005c)
#define gBeeSkel (*(SkeletonHeader*)0x06001398)

#include "types/en_bee.h"
#define TATL_HINT_ID_GIANT_BEE 0xff
#undef ACTOR_EN_BEE
#define ACTOR_EN_BEE 0x204
#undef OBJECT_BEE
#define OBJECT_BEE 0x11eb
#undef NA_SE_EN_BEE_FLY
#define NA_SE_EN_BEE_FLY 0x384a
#undef NA_SE_EN_EXTINCT
#define NA_SE_EN_EXTINCT 0x3878
#undef NA_SE_EN_CUTBODY
#define NA_SE_EN_CUTBODY 0x393b
#undef Collider_InitAndSetCylinder
#define Collider_InitAndSetCylinder ((s32 (*)(struct PlayState* play, ColliderCylinder* dest, struct Actor* actor, ColliderCylinderInit* src))(void*)FOREIGN_ENEMY_API_CYLINDER_INIT_SET)
#undef CutsceneManager_GetCurrentCsId
extern u8 FOREIGN_ENEMY_API_CUTSCENE_CURRENT[];
#define CutsceneManager_GetCurrentCsId ((s16 (*)(void))(void*)FOREIGN_ENEMY_API_CUTSCENE_CURRENT)
#undef Actor_ChangeCategory
#define Actor_ChangeCategory ((void (*)(struct PlayState* play, ActorContext* actorCtx, Actor* actor, u8 actorCategory))(void*)FOREIGN_ENEMY_API_CATEGORY)
#undef Collider_DestroyCylinder
#define Collider_DestroyCylinder ((s32 (*)(struct PlayState* play, ColliderCylinder* cyl))(void*)FOREIGN_ENEMY_API_CYLINDER_DESTROY)
#undef AudioSfx_StopByPosAndId
extern u8 FOREIGN_ENEMY_API_SFX_STOP_POSITION[];
#define AudioSfx_StopByPosAndId ((void (*)(Vec3f* pos, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_STOP_POSITION)
#undef SoundSource_PlaySfxAtFixedWorldPos
#define SoundSource_PlaySfxAtFixedWorldPos ((void (*)(struct PlayState* play, Vec3f* worldPos, u32 duration, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_WORLD)
#undef func_800B3030
#define func_800B3030 ((void (*)(struct PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep, s32 colorIndex))(void*)FOREIGN_ENEMY_API_DEATH_FLAME_PRESET)
#undef Actor_Kill
extern u8 FOREIGN_ENEMY_API_KILL[];
#define Actor_Kill ((void (*)(Actor* actor))(void*)FOREIGN_ENEMY_API_KILL)
#undef Actor_PlaySfx
#define Actor_PlaySfx ((void (*)(Actor* actor, u16 sfxId))(void*)FOREIGN_ENEMY_API_SFX_ACTOR)
#undef Math_Vec3s_Copy
#define Math_Vec3s_Copy ((void (*)(Vec3s* dest, Vec3s* src))(void*)FOREIGN_ENEMY_API_VEC3S_COPY)
#undef CollisionCheck_SetOC
#define CollisionCheck_SetOC ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_OC)
#undef CollisionCheck_SetAC
#define CollisionCheck_SetAC ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AC)
#undef CollisionCheck_SetAT
#define CollisionCheck_SetAT ((s32 (*)(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* col))(void*)FOREIGN_ENEMY_API_COLLISION_AT)
#undef ActorShape_Init
#define ActorShape_Init ((void (*)(ActorShape* actorShape, f32 yOffset, ActorShadowFunc shadowDraw, f32 shadowScale))0x8001ec20u)
#undef SkelAnime_Init
#define SkelAnime_Init ((void (*)(struct PlayState* play, SkelAnime* skelAnime, SkeletonHeader* skeletonHeaderSeg, AnimationHeader* animation, Vec3s* jointTable, Vec3s* morphTable, s32 limbCount))0x8008c684u)
#undef Animation_GetLastFrame
#define Animation_GetLastFrame ((s16 (*)(void* animation))0x8008a194u)
#undef Animation_Change
#define Animation_Change ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation, f32 playSpeed, f32 startFrame, f32 endFrame, u8 mode, f32 morphFrames))0x8008d17cu)
#undef Math_Vec3f_Copy
#define Math_Vec3f_Copy ((void (*)(Vec3f* dest, Vec3f* src))0x80063cacu)
#undef Math_SinS
#define Math_SinS ((f32 (*)(s16 angle))0x800636c4u)
#undef Rand_ZeroFloat
#define Rand_ZeroFloat ((f32 (*)(f32 scale))0x80026d64u)
#undef Math_CosS
#define Math_CosS ((f32 (*)(s16 angle))0x80063684u)
#undef Rand_S16Offset
#define Rand_S16Offset ((s16 (*)(s16 base, s16 range))0x80063bf0u)
#undef Rand_CenteredFloat
#define Rand_CenteredFloat ((f32 (*)(f32 scale))0x80026d90u)
#undef Math_Vec3f_Yaw
#define Math_Vec3f_Yaw ((s16 (*)(Vec3f* a, Vec3f* b))0x80063f00u)
#undef Math_SmoothStepToS
#define Math_SmoothStepToS ((s16 (*)(s16* pValue, s16 target, s16 scale, s16 step, s16 minStep))0x80064508u)
#undef Math_ApproachF
#define Math_ApproachF ((void (*)(f32* pValue, f32 target, f32 scale, f32 maxStep))0x80064280u)
#undef Enemy_StartFinishingBlow
#define Enemy_StartFinishingBlow ((void (*)(struct PlayState* play, Actor* actor))0x80025b4cu)
#undef SkelAnime_Update
#define SkelAnime_Update ((s32 (*)(SkelAnime* skelAnime))0x8008c9c0u)
#undef Actor_SetFocus
#define Actor_SetFocus ((void (*)(Actor* actor, f32 height))0x80020f04u)
#undef Actor_SetScale
#define Actor_SetScale ((void (*)(Actor* actor, f32 scale))0x80020f88u)
#undef Actor_MoveWithGravity
#define Actor_MoveWithGravity ((void (*)(Actor* actor))0x8002121cu)
#undef Actor_UpdateBgCheckInfo
#define Actor_UpdateBgCheckInfo ((void (*)(struct PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight, u32 updBgCheckInfoFlags))0x80021e6cu)
#undef Collider_UpdateCylinder
#define Collider_UpdateCylinder ((void (*)(struct Actor* actor, ColliderCylinder* cyl))0x80050b00u)
#undef Gfx_SetupDL25_Opa
#define Gfx_SetupDL25_Opa ((void (*)(GraphicsContext* gfxCtx))0x8007e298u)
#undef Gfx_SetupDL25_Xlu
#define Gfx_SetupDL25_Xlu ((void (*)(GraphicsContext* gfxCtx))0x8007e2c0u)
#undef SkelAnime_DrawOpa
#define SkelAnime_DrawOpa ((void (*)(struct PlayState* play, void** skeleton, Vec3s* jointTable, GeneratedMmOverrideLimbDraw overrideLimbDraw, GeneratedMmPostLimbDraw postLimbDraw, struct Actor* actor))0x8008993cu)

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE)

void EnBee_Init(Actor* thisx, PlayState* play);
void EnBee_Destroy(Actor* thisx, PlayState* play);
void EnBee_Update(Actor* thisx, PlayState* play);
void EnBee_Draw(Actor* thisx, PlayState* play);

void EnBee_SetupFlyIdle(EnBee* this);
void EnBee_FlyIdle(EnBee* this, PlayState* play);
void EnBee_SetupAttack(EnBee* this);
void EnBee_Attack(EnBee* this, PlayState* play);

s32 sNumLoadedBees = 0;

ActorProfile En_Bee_Profile = {
    0xe04, ACTORCAT_ENEMY, FLAGS, 0x11eb, sizeof(EnBee), EnBee_Init, EnBee_Destroy, EnBee_Update, EnBee_Draw,
};

static DamageTable sDamageTable = { {
    DMG_ENTRY(1, 0xF), DMG_ENTRY(1, 0xF), DMG_ENTRY(1, 0xF), DMG_ENTRY(1, 0xF), DMG_ENTRY(1, 0xF), DMG_ENTRY(1, 0xF),
    DMG_ENTRY(0, 0x0), DMG_ENTRY(1, 0xF), DMG_ENTRY(1, 0xF), DMG_ENTRY(1, 0xF), DMG_ENTRY(1, 0xF), DMG_ENTRY(2, 0xF),
    DMG_ENTRY(2, 0xF), DMG_ENTRY(2, 0xF), DMG_ENTRY(1, 0xF), DMG_ENTRY(1, 0xF), DMG_ENTRY(1, 0xF), DMG_ENTRY(1, 0xF),
    DMG_ENTRY(1, 0xF), DMG_ENTRY(1, 0xF), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(1, 0xF), DMG_ENTRY(1, 0xF),
    DMG_ENTRY(1, 0xF), DMG_ENTRY(1, 0xF), DMG_ENTRY(1, 0xF), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(0, 0x0), DMG_ENTRY(1, 0xF),
} };

static ColliderCylinderInit sCylinderInit = {
    {
        COL_MATERIAL_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_ON,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0xF7CFFFFF, 0x08, 0x02 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        ATELEM_ON | ATELEM_SFX_NORMAL,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 6, 13, -4, { 0, 0, 0 } },
};

void EnBee_Init(Actor* thisx, PlayState* play) {
    EnBee* this = (EnBee*)thisx;

    this->actor.colChkInfo.mass = 10;
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 19.0f);
    SkelAnime_Init(play, &this->skelAnime, &gBeeSkel, &gBeeFlyingAnim, this->jointTable, this->morphTable,
                   OBJECT_BEE_LIMB_MAX);
    this->actor.colChkInfo.health = 1;
    this->actor.colChkInfo.damageTable = &sDamageTable;
    this->actor.attentionRangeType = ATTENTION_RANGE_6;
    Collider_InitAndSetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    this->instanceId = sNumLoadedBees;
    sNumLoadedBees++;
    this->actor.shape.shadowScale = 12.0f;

    if (CutsceneManager_GetCurrentCsId() != CS_ID_NONE) {
        Actor_ChangeCategory(play, &play->actorCtx, &this->actor, ACTORCAT_ITEMACTION);
    }

    this->actor.hintId = TATL_HINT_ID_GIANT_BEE;
    EnBee_SetupFlyIdle(this);
}

void EnBee_Destroy(Actor* thisx, PlayState* play) {
    EnBee* this = (EnBee*)thisx;

    Collider_DestroyCylinder(play, &this->collider);
}

void EnBee_SetupFlyIdle(EnBee* this) {
    Vec3f tempPos;
    s16 yawOffset;

    Animation_Change(&this->skelAnime, &gBeeFlyingAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gBeeFlyingAnim),
                     ANIMMODE_LOOP, -10.0f);
    Math_Vec3f_Copy(&tempPos, &this->actor.home.pos);

    yawOffset = (this->instanceId * 0x700) + 0x2000;
    tempPos.x += Math_SinS(yawOffset) * 50.0f;
    tempPos.y = Rand_ZeroFloat(50.0f) + (this->actor.floorHeight + 30.0f);
    tempPos.z += Math_CosS(yawOffset) * 50.0f;

    Math_Vec3f_Copy(&this->targetPos[0], &tempPos);
    Math_Vec3f_Copy(&tempPos, &this->actor.home.pos);

    tempPos.x += Math_SinS(yawOffset - 0x4000) * 50.0f;
    tempPos.y = Rand_ZeroFloat(50.0f) + (this->actor.floorHeight + 30.0f);
    tempPos.z += Math_CosS(yawOffset - 0x4000) * 50.0f;

    Math_Vec3f_Copy(&this->targetPos[1], &tempPos);

    this->attackDelayTimer = Rand_S16Offset(20, 30);
    this->isHostile = false;
    this->actionFunc = EnBee_FlyIdle;
}

void EnBee_FlyIdle(EnBee* this, PlayState* play) {
    Vec3f nextPos;

    if ((this->actor.category != ACTORCAT_ENEMY) && (CutsceneManager_GetCurrentCsId() == CS_ID_NONE)) {
        Actor_ChangeCategory(play, &play->actorCtx, &this->actor, ACTORCAT_ENEMY);
    }

    Math_Vec3f_Copy(&nextPos, &this->targetPos[this->posIndex]);
    nextPos.x += Math_SinS(this->targetYaw) * 30.0f;
    nextPos.z += Math_CosS(this->targetYaw) * 30.0f;

    if ((this->instanceId % 2) == 0) {
        this->targetYaw += (s16)((s32)Rand_CenteredFloat(1000.0f) + 4000);
    } else {
        this->targetYaw -= (s16)((s32)Rand_CenteredFloat(1000.0f) + 4000);
    }

    this->flightHoverOffset += 1000;
    this->actor.velocity.y = Math_SinS(this->flightHoverOffset);

    if (this->targetYaw > 0x10000) {
        this->targetYaw = 0;
        this->posIndex++;
        this->posIndex &= 1;
    }

    Math_SmoothStepToS(&this->actor.world.rot.y, Math_Vec3f_Yaw(&this->actor.world.pos, &nextPos), 1, 0x7D0, 0);
    Math_ApproachF(&this->actor.speed, 3.0f, 0.3f, 1.0f);

#if defined(GAME_OOT)
    if (this->attackDelayTimer == 0 && this->actor.category == ACTORCAT_ENEMY) {
#else
    if (this->attackDelayTimer == 0 && this->actor.params != BEE_BEHAVIOR_IDLE) {
#endif
        EnBee_SetupAttack(this);
    }
}

void EnBee_SetupAttack(EnBee* this) {
    Animation_Change(&this->skelAnime, &gBeeFlyingAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gBeeFlyingAnim),
                     ANIMMODE_LOOP, -10.0f);
    this->isHostile = true;
    this->actionFunc = EnBee_Attack;
}

void EnBee_Attack(EnBee* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Vec3f nextPos;
    f32 rand;
    f32 yawOffset;
    s32 i;

    Math_Vec3f_Copy(&nextPos, &player->actor.world.pos);
    yawOffset = (this->instanceId * 0x700) + 0x2000;

    for (i = 0; i < 2; i++) {
        rand = Rand_CenteredFloat(20.0f);
        nextPos.x += Math_SinS((f32)this->actor.yawTowardsPlayer + this->targetYaw + yawOffset) * (rand + 30.0f);
        nextPos.y = (Math_SinS(this->flightHoverOffset) * 10.0f) + (player->actor.floorHeight + 40.0f);
        rand = Rand_CenteredFloat(20.0f);

        nextPos.z += Math_CosS((f32)this->actor.yawTowardsPlayer + this->targetYaw + yawOffset) * (rand + 30.0f);
        Math_Vec3f_Copy(&this->targetPos[i], &nextPos);
        yawOffset -= 0x4000;
    }

    Math_Vec3f_Copy(&nextPos, &this->targetPos[this->posIndex]);

    if ((this->instanceId % 2) == 0) {
        this->targetYaw += (this->instanceId * 0x700) + (s32)Rand_CenteredFloat((this->instanceId * 0x700) * 0.5f);
    } else {
        this->targetYaw -= (this->instanceId * 0x700) + (s32)Rand_CenteredFloat((this->instanceId * 0x700) * 0.5f);
    }

    this->flightHoverOffset += (s32)Rand_CenteredFloat(500.0f) + 1000;

    if (this->targetYaw > 0x10000) {
        this->targetYaw = 0;
        this->posIndex++;
        this->posIndex &= 1;
    }

    Math_SmoothStepToS(&this->actor.world.rot.y, Math_Vec3f_Yaw(&this->actor.world.pos, &nextPos), 1, 0x1388, 0);
    Math_ApproachF(&this->actor.world.pos.y, nextPos.y, 0.3f, 3.0f);
    Math_ApproachF(&this->actor.speed, 5.0f, 0.3f, 1.0f);
}

void EnBee_UpdateDamage(EnBee* this, PlayState* play) {
    if ((this->sfxHitTimer == 0) && (this->collider.base.atFlags & AT_HIT)) {
        AudioSfx_StopByPosAndId(&this->actor.projectedPos, NA_SE_EN_BEE_FLY - SFX_FLAG);
        this->sfxHitTimer = 5;
    }

    if (this->collider.base.acFlags & AC_HIT) {
        Enemy_StartFinishingBlow(play, &this->actor);
        this->actor.speed = 0.0f;
        SoundSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 10, NA_SE_EN_CUTBODY);
        this->actor.colChkInfo.health = 0;
        SoundSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 50, NA_SE_EN_EXTINCT);
        func_800B3030(play, &this->actor.world.pos, &gZeroVec3f, &gZeroVec3f, 100, 0, 2);
        Actor_Kill(&this->actor);
    }
}

void EnBee_Update(Actor* thisx, PlayState* play) {
    EnBee* this = (EnBee*)thisx;

    SkelAnime_Update(&this->skelAnime);

    if (this->actor.category == ACTORCAT_ENEMY) {
        if (this->attackDelayTimer != 0) {
            this->attackDelayTimer--;
        }
        if (this->sfxHitTimer != 0) {
            this->sfxHitTimer--;
        }
    }

    Actor_PlaySfx(&this->actor, NA_SE_EN_BEE_FLY - SFX_FLAG);
    EnBee_UpdateDamage(this, play);
    Math_Vec3s_Copy(&this->actor.shape.rot, &this->actor.world.rot);
    Actor_SetFocus(&this->actor, 0.0f);
    Actor_SetScale(&this->actor, 0.01f);

    this->actionFunc(this, play);

    Actor_MoveWithGravity(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 10.0f, 40.0f, 40.0f,
                            UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_4 | UPDBGCHECKINFO_FLAG_8 |
                                UPDBGCHECKINFO_FLAG_10);

    Collider_UpdateCylinder(&this->actor, &this->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
    CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
}

void EnBee_Draw(Actor* thisx, PlayState* play) {
    EnBee* this = (EnBee*)thisx;

    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    SkelAnime_DrawOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, NULL, NULL, &this->actor);
}

OVL_INFO_NATIVE_ENEMY(0x204, En_Bee_Profile);
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_MM_EN_BEE, En_Bee_Profile, EnBee_Init, EnBee_Destroy, EnBee_Update, EnBee_Draw);
static const u32 generatedNativeOrigin[] = { 0x4e505431u, 0x1u, 0x204u, 0x1ebu, 0xdf6b0424u };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_BEE_ORIGIN, generatedNativeOrigin);
