#include "oot_compat.h"
#include "types/en_fd.h"
#undef ACTOR_EN_BOM
#define ACTOR_EN_BOM 0x0010
#undef ACTOR_EN_FD
#define ACTOR_EN_FD 0x0099
#undef ACTOR_EN_FD_FIRE
#define ACTOR_EN_FD_FIRE 0x00a3
#undef ACTOR_EN_FW
#define ACTOR_EN_FW 0x00ab
#undef OBJECT_FW
#define OBJECT_FW 0x109e
#undef NA_SE_EN_FLAME_DAMAGE
#define NA_SE_EN_FLAME_DAMAGE 0x397a
#undef NA_SE_EN_FLAME_KICK
#define NA_SE_EN_FLAME_KICK 0x397c
#undef NA_SE_EN_FLAME_RUN
#define NA_SE_EN_FLAME_RUN 0x397d
#undef NA_SE_EN_FLAME_ROLL
#define NA_SE_EN_FLAME_ROLL 0x397e
#undef NA_SE_EN_FLAME_LAUGH
#define NA_SE_EN_FLAME_LAUGH 0x3981
#undef NA_SE_EN_FLAME_FIRE_ATTACK
#define NA_SE_EN_FLAME_FIRE_ATTACK 0x3983
#undef NA_SE_PL_BODY_HIT
#define NA_SE_PL_BODY_HIT 0x083e
#undef DMG_HOOKSHOT
#define DMG_HOOKSHOT (1 << 7)
#define gDust1Tex (*((u64 (*)[1])0x040572e0))
#define gDust2Tex (*((u64 (*)[1])0x040576e0))
#define gDust3Tex (*((u64 (*)[1])0x04057ae0))
#define gDust4Tex (*((u64 (*)[1])0x04057ee0))
#define gDust5Tex (*((u64 (*)[1])0x040582e0))
#define gDust6Tex (*((u64 (*)[1])0x040586e0))
#define gDust7Tex (*((u64 (*)[1])0x04058ae0))
#define gDust8Tex (*((u64 (*)[1])0x04058ee0))
#define gFlareDancerSkel (*(FlexSkeletonHeader*)0x06005810)
#define gFlareDancerDL_7928 (*((Gfx (*)[1])0x06007928))
#define gFlareDancerSquareParticleDL (*((Gfx (*)[1])0x06007938))
#define gFlareDancerDL_79F8 (*((Gfx (*)[1])0x060079f8))
#define gFlareDancerTriangleParticleDL (*((Gfx (*)[1])0x06007a78))
#define gFlareDancerCastingFireAnim (*(AnimationHeader*)0x060010b4)
#define gFlareDancerBackflipAnim (*(AnimationHeader*)0x06005c64)
#define gFlareDancerGettingUpAnim (*(AnimationHeader*)0x06006044)
#define gFlareDancerChasingAnim (*(AnimationHeader*)0x06006a18)
#define gFlareDancerTwirlAnim (*(AnimationHeader*)0x06006b64)

#undef ActorShape_Init
#define ActorShape_Init ((void (*)(ActorShape* shape, f32 yOffset, ActorShadowFunc shadowDraw, f32 shadowScale))0x800b3ba4)
#undef Actor_Kill
#define Actor_Kill FOREIGN_ENEMY_API_KILL
#undef Actor_MoveXZGravity
#define Actor_MoveXZGravity ((void (*)(Actor* actor))0x800b6a88)
#undef Actor_PlaySfx
#define Actor_PlaySfx FOREIGN_ENEMY_API_SFX_ACTOR
#undef Actor_PlaySfx_Flagged
#define Actor_PlaySfx_Flagged FOREIGN_ENEMY_API_SFX_FLAGGED
#undef Actor_SetPlayerKnockbackLargeNoDamage
#define Actor_SetPlayerKnockbackLargeNoDamage FOREIGN_ENEMY_API_KNOCKBACK_LARGE_NO_DAMAGE
#undef Actor_SetScale
#define Actor_SetScale ((void (*)(Actor* actor, f32 scale))0x800b67e0)
#undef Actor_SpawnAsChild
extern Actor* FOREIGN_ENEMY_API_SPAWN_CHILD(ActorContext* actorCtx, Actor* parent, PlayState* play, s16 actorId,
                                            f32 posX, f32 posY, f32 posZ, s16 rotX, s16 rotY, s16 rotZ, s16 params);
#define Actor_SpawnAsChild FOREIGN_ENEMY_API_SPAWN_CHILD
#undef Actor_SwapHookshotAttachment
extern void FOREIGN_ENEMY_API_TRANSFER_HOOK(PlayState* play, Actor* srcActor, Actor* destActor);
#define Actor_SwapHookshotAttachment FOREIGN_ENEMY_API_TRANSFER_HOOK
#undef Actor_UpdateBgCheckInfo
#define Actor_UpdateBgCheckInfo ((void (*)(PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight, s32 flags))0x800b78b8)
#undef Animation_ChangeByInfo
#define Animation_ChangeByInfo ((void (*)(SkelAnime* skelAnime, AnimationInfo* animationInfo, s32 index))0x800bdc5c)
#undef Animation_OnFrame
#define Animation_OnFrame ((s32 (*)(SkelAnime* skelAnime, f32 frame))0x801378b8)
#undef BgCheck_EntityLineTest1
#define BgCheck_EntityLineTest1 ((s32 (*)(CollisionContext* colCtx, Vec3f* posA, Vec3f* posB, Vec3f* posResult, CollisionPoly** outPoly, s32 chkWall, s32 chkFloor, s32 chkCeil, s32 chkOneFace, s32* bgId))0x800c55c4)
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
#undef CollisionCheck_SetInfo2
#define CollisionCheck_SetInfo2 ((void (*)(CollisionCheckInfo* info, DamageTable* damageTable, CollisionCheckInfoInit2* init))0x800e755c)
#undef CollisionCheck_SetOC
#define CollisionCheck_SetOC FOREIGN_ENEMY_API_COLLISION_OC
#undef DamageTable_Get
#define DamageTable_Get FOREIGN_ENEMY_API_DAMAGE_TABLE
#undef Enemy_StartFinishingBlow
#define Enemy_StartFinishingBlow ((void (*)(PlayState* play, Actor* actor))0x800bba88)
#undef Gfx_SetupDL
#define Gfx_SetupDL ((Gfx* (*)(Gfx* gfx, u32 i))0x8012bf78)
#undef Gfx_SetupDL_25Xlu
#define Gfx_SetupDL_25Xlu ((void (*)(GraphicsContext* gfxCtx))0x8012c2dc)
#undef Gfx_TwoTexScroll
#define Gfx_TwoTexScroll ((Gfx* (*)(GraphicsContext* gfxCtx, s32 tile1, u32 x1, u32 y1, s32 width1, s32 height1, s32 tile2, u32 x2, u32 y2, s32 width2, s32 height2))0x8012cbd0)
#undef Math_ApproachF
#define Math_ApproachF ((void (*)(f32* pValue, f32 target, f32 fraction, f32 step))0x800ffdf8)
#undef Math_CosS
#define Math_CosS ((f32 (*)(s16 angle))0x800fed44)
#undef Math_FAtan2F
#define Math_FAtan2F ((f32 (*)(f32 y, f32 x))0x80086b30)
#undef Math_SinS
#define Math_SinS ((f32 (*)(s16 angle))0x800fed84)
#undef Math_SmoothStepToF
#define Math_SmoothStepToF ((f32 (*)(f32* pValue, f32 target, f32 fraction, f32 step, f32 minStep))0x800ffcd8)
#undef Math_SmoothStepToS
#define Math_SmoothStepToS ((s16 (*)(s16* pValue, s16 target, s16 scale, s16 step, s16 minStep))0x800ffebc)
#undef Math_Vec3f_DistXYZ
#define Math_Vec3f_DistXYZ ((f32 (*)(Vec3f* a, Vec3f* b))0x800ff884)
#undef Math_Vec3f_Yaw
#define Math_Vec3f_Yaw ((s16 (*)(Vec3f* origin, Vec3f* point))0x800ffa60)
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
#undef Matrix_Scale
#define Matrix_Scale FOREIGN_ENEMY_API_MATRIX_SCALE
#undef Matrix_Translate
#define Matrix_Translate FOREIGN_ENEMY_API_MATRIX_TRANSLATE
#undef Rand_S16Offset
#define Rand_S16Offset ((s16 (*)(s16 base, s16 range))0x800ff450)
#undef Rand_ZeroOne
#define Rand_ZeroOne ((f32 (*)(void))0x80086fdc)
#undef SkelAnime_DrawFlex
#define SkelAnime_DrawFlex ((Gfx* (*)(PlayState* play, void** skeleton, Vec3s* jointTable, s32 dListCount, void* overrideLimbDraw, void* postLimbDraw, void* arg, Gfx* gfx))0x80134dbc)
#undef SkelAnime_InitFlex
#define SkelAnime_InitFlex ((void (*)(PlayState* play, SkelAnime* skelAnime, FlexSkeletonHeader* skeletonHeaderSeg, AnimationHeader* animation, Vec3s* jointTable, Vec3s* morphTable, s32 limbCount))0x80136b30)
#undef SkelAnime_Update
#define SkelAnime_Update ((s32 (*)(SkelAnime* skelAnime))0x80136cd0)
#undef func_800F5ACC
#define func_800F5ACC FOREIGN_ENEMY_API_OOT_BGM_STORE

#define FLAGS                                                                                 \
    (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED | \
     ACTOR_FLAG_HOOKSHOT_PULLS_ACTOR)

void EnFd_Init(Actor* thisx, PlayState* play);
void EnFd_Destroy(Actor* thisx, PlayState* play);
void EnFd_Update(Actor* thisx, PlayState* play);
void EnFd_Draw(Actor* thisx, PlayState* play);
void EnFd_Run(EnFd* this, PlayState* play);
void EnFd_SpinAndSpawnFire(EnFd* this, PlayState* play);
void EnFd_Reappear(EnFd* this, PlayState* play);
void EnFd_SpinAndGrow(EnFd* this, PlayState* play);
void EnFd_JumpToGround(EnFd* this, PlayState* play);
void EnFd_WaitForCore(EnFd* this, PlayState* play);
void EnFd_UpdateEffectsFlames(EnFd* this);
void EnFd_UpdateEffectsDots(EnFd* this);
void EnFd_SpawnEffect(EnFd*, u8, Vec3f*, Vec3f*, Vec3f*, u8, f32, f32);
void EnFd_DrawEffectsDots(EnFd* this, PlayState* play);
void EnFd_DrawEffectsFlames(EnFd* this, PlayState* play);
void EnFd_Land(EnFd* this, PlayState* play);

ActorProfile En_Fd_Profile = {
    0xc99, ACTORCAT_ENEMY, FLAGS, OBJECT_FW, sizeof(EnFd), EnFd_Init, EnFd_Destroy, EnFd_Update, EnFd_Draw,
};

#if OOT_VERSION < NTSC_1_1
#define FLARE_DANCER_BODY_DMG 0x00
#else
#define FLARE_DANCER_BODY_DMG 0x04
#endif

static ColliderJntSphElementInit sJntSphElementsInit[] = {
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0xFFCFFFFF, HIT_SPECIAL_EFFECT_FIRE, FLARE_DANCER_BODY_DMG },
            { 0x00040088, HIT_BACKLASH_NONE, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON | ACELEM_HOOKABLE,
            OCELEM_ON,
        },
        { 21, { { 1600, 0, 0 }, 5 }, 300 },
    },
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0xFFCFFFFF, HIT_SPECIAL_EFFECT_FIRE, FLARE_DANCER_BODY_DMG },
            { 0x00040008, HIT_BACKLASH_NONE, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_ON,
        },
        { 12, { { 1600, 0, 0 }, 5 }, 400 },
    },
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0xFFCFFFFF, HIT_SPECIAL_EFFECT_FIRE, FLARE_DANCER_BODY_DMG },
            { 0x00040008, HIT_BACKLASH_NONE, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_ON,
        },
        { 14, { { 800, 0, 0 }, 4 }, 300 },
    },
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0xFFCFFFFF, HIT_SPECIAL_EFFECT_FIRE, FLARE_DANCER_BODY_DMG },
            { 0x00040008, HIT_BACKLASH_NONE, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_ON,
        },
        { 15, { { 1600, 0, 0 }, 4 }, 300 },
    },
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0xFFCFFFFF, HIT_SPECIAL_EFFECT_FIRE, FLARE_DANCER_BODY_DMG },
            { 0x00040008, HIT_BACKLASH_NONE, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_ON,
        },
        { 16, { { 2000, 0, 0 }, 4 }, 300 },
    },
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0xFFCFFFFF, HIT_SPECIAL_EFFECT_FIRE, FLARE_DANCER_BODY_DMG },
            { 0x00040008, HIT_BACKLASH_NONE, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_ON,
        },
        { 17, { { 800, 0, 0 }, 4 }, 300 },
    },
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0xFFCFFFFF, HIT_SPECIAL_EFFECT_FIRE, FLARE_DANCER_BODY_DMG },
            { 0x00040008, HIT_BACKLASH_NONE, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_ON,
        },
        { 18, { { 1600, 0, 0 }, 4 }, 300 },
    },
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0xFFCFFFFF, HIT_SPECIAL_EFFECT_FIRE, FLARE_DANCER_BODY_DMG },
            { 0x00040008, HIT_BACKLASH_NONE, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_ON,
        },
        { 19, { { 2000, 0, 0 }, 4 }, 300 },
    },
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0xFFCFFFFF, HIT_SPECIAL_EFFECT_FIRE, FLARE_DANCER_BODY_DMG },
            { 0x00040008, HIT_BACKLASH_NONE, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_ON,
        },
        { 4, { { 2200, 0, 0 }, 4 }, 400 },
    },
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0xFFCFFFFF, HIT_SPECIAL_EFFECT_FIRE, FLARE_DANCER_BODY_DMG },
            { 0x00040008, HIT_BACKLASH_NONE, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_ON,
        },
        { 4, { { 5000, 0, 0 }, 4 }, 300 },
    },
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0xFFCFFFFF, HIT_SPECIAL_EFFECT_FIRE, FLARE_DANCER_BODY_DMG },
            { 0x00040008, HIT_BACKLASH_NONE, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_ON,
        },
        { 8, { { 2200, 0, 0 }, 4 }, 400 },
    },
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0xFFCFFFFF, HIT_SPECIAL_EFFECT_FIRE, FLARE_DANCER_BODY_DMG },
            { 0x00040008, HIT_BACKLASH_NONE, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_ON,
        },
        { 8, { { 5000, 0, 0 }, 4 }, 300 },
    },
};

static ColliderJntSphInit sJntSphInit = {
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

static CollisionCheckInfoInit2 sColChkInit = { 24, 2, 25, 25, MASS_IMMOVABLE };

typedef enum EnFdAnimation { ENFD_ANIM_0, ENFD_ANIM_1, ENFD_ANIM_2, ENFD_ANIM_3, ENFD_ANIM_4 } EnFdAnimation;

static AnimationInfo sAnimationInfo[] = {
    { &gFlareDancerCastingFireAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE_INTERP, 0.0f },
    { &gFlareDancerBackflipAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE_INTERP, -10.0f },
    { &gFlareDancerGettingUpAnim, 0.0f, 0.0f, -1.0f, ANIMMODE_ONCE_INTERP, -10.0f },
    { &gFlareDancerChasingAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP_INTERP, -10.0f },
    { &gFlareDancerTwirlAnim, 0.0f, 0.0f, -1.0f, ANIMMODE_ONCE_INTERP, -10.0f },
};

s32 EnFd_SpawnCore(EnFd* this, PlayState* play) {
    if (this->invincibilityTimer != 0) {
        return false;
    }

    if (Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, ACTOR_EN_FW, this->corePos.x, this->corePos.y,
                           this->corePos.z, 0, this->actor.shape.rot.y, 0, this->runDir) == NULL) {
        return false;
    }

    this->actor.child->colChkInfo.health = this->actor.colChkInfo.health % 8;

    if (this->actor.child->colChkInfo.health == 0) {
        this->actor.child->colChkInfo.health = 8;
    }

    if (ACTOR_FLAGS_CHECK_ALL(&this->actor, ACTOR_FLAG_HOOKSHOT_ATTACHED)) {
        Actor_SwapHookshotAttachment(play, &this->actor, this->actor.child);
    }

    this->coreActive = true;

    return true;
}

void EnFd_SpawnChildFire(EnFd* this, PlayState* play, s16 fireCnt, s16 color) {
    s32 i;

    for (i = 0; i < fireCnt; i++) {
        s16 angle = DEG_TO_BINANG((i * 360.0f) / fireCnt) + this->actor.yawTowardsPlayer;
        Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, ACTOR_EN_FD_FIRE, this->actor.world.pos.x,
                           this->actor.world.pos.y, this->actor.world.pos.z, 0, angle, 0, (color << 0xF) | i);
    }
}

void EnFd_SpawnDot(EnFd* this, PlayState* play) {
    Vec3f pos = { 0.0f, 0.0f, 0.0f };
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    Vec3f accel = { 0.0f, 0.0f, 0.0f };

    if (this->actionFunc == EnFd_Run || this->actionFunc == EnFd_SpinAndSpawnFire) {
        pos.x = this->actor.world.pos.x;
        pos.y = this->actor.floorHeight + 4.0f;
        pos.z = this->actor.world.pos.z;
        accel.x = (Rand_ZeroOne() - 0.5f) * 2.0f;
        accel.y = ((Rand_ZeroOne() - 0.5f) * 0.2f) + 0.3f;
        accel.z = (Rand_ZeroOne() - 0.5f) * 2.0f;
        EnFd_SpawnEffect(this, FD_EFFECT_FLAME, &pos, &velocity, &accel, 8, 0.6f, 0.2f);
    }
}

s32 EnFd_CheckHammer(EnFd* this, PlayState* play) {
    if (this->actionFunc == EnFd_Reappear || this->actionFunc == EnFd_SpinAndGrow ||
        this->actionFunc == EnFd_JumpToGround || this->actionFunc == EnFd_WaitForCore) {
        return false;
    } else if (play->actorCtx.unk2 != 0 && this->actor.xzDistToPlayer < 300.0f &&
               this->actor.yDistanceFromLink < 60.0f) {
        return true;
    } else {
        return false;
    }
}

s32 EnFd_ColliderCheck(EnFd* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    ColliderElement* elem;

    if (this->collider.base.acFlags & AC_HIT || EnFd_CheckHammer(this, play)) {
        this->collider.base.acFlags &= ~AC_HIT;
        if (this->invincibilityTimer != 0) {
            return false;
        }
        elem = &this->collider.elements[0].base;
        if (elem->acHitElem != NULL && (elem->acHitElem->atDmgInfo.dmgFlags & DMG_HOOKSHOT)) {
            return false;
        }

        if (!EnFd_SpawnCore(this, play)) {
            return false;
        }
        this->invincibilityTimer = 30;
        this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
        Actor_PlaySfx(&this->actor, NA_SE_EN_FLAME_DAMAGE);
        Enemy_StartFinishingBlow(play, &this->actor);
        return true;
    } else if (DECR(this->attackTimer) == 0 && this->collider.base.atFlags & AT_HIT) {
        this->collider.base.atFlags &= ~AT_HIT;
        if (this->invincibilityTimer != 0) {
            return false;
        }

        if (this->collider.base.atFlags & AT_BOUNCED) {
            return false;
        }
        this->attackTimer = 30;
        Actor_PlaySfx(&player->actor, NA_SE_PL_BODY_HIT);
        Actor_SetPlayerKnockbackLargeNoDamage(play, &this->actor, this->actor.speed + 2.0f,
                                              this->actor.yawTowardsPlayer, 6.0f);
    }
    return false;
}

s32 EnFd_CanSeeActor(EnFd* this, Actor* actor, PlayState* play) {
    CollisionPoly* colPoly;
    s32 bgId;
    Vec3f colPoint;
    s16 angle;

    if (Math_Vec3f_DistXYZ(&this->actor.world.pos, &actor->world.pos) > 400.0f) {
        return false;
    }

    angle = (f32)Math_Vec3f_Yaw(&this->actor.world.pos, &actor->world.pos) - this->actor.shape.rot.y;
    if (ABS(angle) > 0x1C70) {
        return false;
    }

    if (BgCheck_EntityLineTest1(&play->colCtx, &this->actor.world.pos, &actor->world.pos, &colPoint, &colPoly, true,
                                false, false, true, &bgId)) {
        return false;
    }

    return true;
}

Actor* EnFd_FindBomb(EnFd* this, PlayState* play) {
    Actor* actor = play->actorCtx.actors[ACTORCAT_EXPLOSIVE].first;

    while (actor != NULL) {
        if (actor->params != 0 || actor->parent != NULL) {
            actor = actor->next;
            continue;
        }

        if (actor->id != 9) {
            actor = actor->next;
            continue;
        }

        if (EnFd_CanSeeActor(this, actor, play) != 1) {
            actor = actor->next;
            continue;
        }

        return actor;
    }
    return NULL;
}

Actor* EnFd_FindPotentialTheat(EnFd* this, PlayState* play) {
    Player* player;
    Actor* bomb = EnFd_FindBomb(this, play);

    if (bomb != NULL) {
        return bomb;
    }

    if (this->attackTimer != 0) {
        return NULL;
    }

    player = GET_PLAYER(play);
    if (!EnFd_CanSeeActor(this, &player->actor, play)) {
        return NULL;
    }

    return &player->actor;
}

Vec3f* EnFd_GetPosAdjAroundCircle(Vec3f* dst, EnFd* this, f32 radius, s16 dir) {
    s16 angle;
    Vec3f newPos;

    angle = Math_Vec3f_Yaw(&this->actor.home.pos, &this->actor.world.pos) + (dir * 0x1554);
    newPos.x = (Math_SinS(angle) * radius) + this->actor.home.pos.x;
    newPos.z = (Math_CosS(angle) * radius) + this->actor.home.pos.z;
    newPos.x -= this->actor.world.pos.x;
    newPos.z -= this->actor.world.pos.z;
    *dst = newPos;
    return dst;
}

s32 EnFd_ShouldStopRunning(EnFd* this, PlayState* play, f32 radius, s16* runDir) {
    CollisionPoly* poly;
    s32 bgId;
    Vec3f colPoint;
    Vec3f pos;

    EnFd_GetPosAdjAroundCircle(&pos, this, radius, *runDir);

    pos.x += this->actor.world.pos.x;
    pos.y = this->actor.world.pos.y;
    pos.z += this->actor.world.pos.z;

    if (BgCheck_EntityLineTest1(&play->colCtx, &this->actor.world.pos, &pos, &colPoint, &poly, true, false, false, true,
                                &bgId)) {
        *runDir = -*runDir;
        return true;
    }

    if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
        Vec3f probe = this->actor.world.pos;
        s16 yaw = Math_Vec3f_Yaw(&probe, &pos);
        f32 step = MAX(0.f, this->actor.speed) + 10.f;
        probe.x += step * Math_SinS(yaw);
        probe.z += step * Math_CosS(yaw);
        probe.y += 30.f;
        poly = NULL;
        f32 floor = BgCheck_EntityRaycastDown4(&play->colCtx, &poly, &bgId, &this->actor, &probe);
        if (!poly || floor < this->actor.floorHeight - 20.f) {
            *runDir = -*runDir;
            return true;
        }
    }

    if (this->circlesToComplete != 0 || DECR(this->spinTimer) != 0) {
        return false;
    }

    if (Rand_ZeroOne() > 0.5f) {
        *runDir = -*runDir;
    }
    return true;
}

void EnFd_Fade(EnFd* this, PlayState* play) {
    if (this->invincibilityTimer != 0) {
        Math_SmoothStepToF(&this->fadeAlpha, 0.0f, 0.3f, 10.0f, 0.0f);
        this->actor.shape.shadowAlpha = this->fadeAlpha;
        if (!(this->fadeAlpha >= 0.9f)) {
            this->invincibilityTimer = 0;
            this->spinTimer = 0;
            this->actionFunc = EnFd_WaitForCore;
            this->actor.speed = 0.0f;
        }
    }
}

void EnFd_Init(Actor* thisx, PlayState* play) {
    EnFd* this = (EnFd*)thisx;

    SkelAnime_InitFlex(play, &this->skelAnime, &gFlareDancerSkel, NULL, this->jointTable, this->morphTable, 27);
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 32.0f);
    Collider_InitJntSph(play, &this->collider);
    Collider_SetJntSph(play, &this->collider, &this->actor, &sJntSphInit, this->colliderElements);
    CollisionCheck_SetInfo2(&this->actor.colChkInfo, DamageTable_Get(0xF), &sColChkInit);
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    this->actor.flags |= ACTOR_FLAG_SFX_FOR_PLAYER_BODY_HIT;
    Actor_SetScale(&this->actor, 0.01f);
    this->firstUpdateFlag = true;
    this->actor.gravity = -1.0f;
    this->runDir = Rand_ZeroOne() < 0.5f ? -1 : 1;
    this->actor.hintId = 0xff;
    this->actionFunc = EnFd_Reappear;
}

void EnFd_Destroy(Actor* thisx, PlayState* play) {
    EnFd* this = (EnFd*)thisx;

    Collider_DestroyJntSph(play, &this->collider);
}

void EnFd_Reappear(EnFd* this, PlayState* play) {
    this->actor.world.pos = this->actor.home.pos;
    this->actor.params = 0;
    this->actor.shape.shadowAlpha = 0xFF;
    this->coreActive = false;
    this->actor.scale.y = 0.0f;
    this->fadeAlpha = 255.0f;
    Animation_ChangeByInfo(&this->skelAnime, sAnimationInfo, ENFD_ANIM_0);
    Actor_PlaySfx(&this->actor, NA_SE_EN_FLAME_LAUGH);
    this->actionFunc = EnFd_SpinAndGrow;
}

void EnFd_SpinAndGrow(EnFd* this, PlayState* play) {
    if (Animation_OnFrame(&this->skelAnime, this->skelAnime.endFrame)) {
        this->actor.velocity.y = 6.0f;
        this->actor.scale.y = 0.01f;
        this->actor.world.rot.y ^= 0x8000;
        this->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED;
        this->actor.speed = 8.0f;
        Animation_ChangeByInfo(&this->skelAnime, sAnimationInfo, ENFD_ANIM_1);
        this->actionFunc = EnFd_JumpToGround;
    } else {
        this->actor.scale.y = this->skelAnime.curFrame * (0.01f / this->skelAnime.animLength);
        this->actor.shape.rot.y += 0x2000;
        this->actor.world.rot.y = this->actor.shape.rot.y;
    }
}

void EnFd_JumpToGround(EnFd* this, PlayState* play) {
    if ((this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) && !(this->actor.velocity.y > 0.0f)) {
        this->actor.velocity.y = 0.0f;
        this->actor.speed = 0.0f;
        this->actor.world.rot.y = this->actor.shape.rot.y;
        Animation_ChangeByInfo(&this->skelAnime, sAnimationInfo, ENFD_ANIM_2);
        this->actionFunc = EnFd_Land;
    }
}

void EnFd_Land(EnFd* this, PlayState* play) {
    Vec3f adjPos;

    Math_SmoothStepToF(&this->skelAnime.playSpeed, 1.0f, 0.1f, 1.0f, 0.0f);
    if (Animation_OnFrame(&this->skelAnime, this->skelAnime.endFrame)) {
        this->spinTimer = Rand_S16Offset(60, 90);
        this->runRadius = Math_Vec3f_DistXYZ(&this->actor.world.pos, &this->actor.home.pos);
        EnFd_GetPosAdjAroundCircle(&adjPos, this, this->runRadius, this->runDir);
        this->actor.world.rot.y = RAD_TO_BINANG(Math_FAtan2F(adjPos.x, adjPos.z));
        Animation_ChangeByInfo(&this->skelAnime, sAnimationInfo, ENFD_ANIM_4);
        this->actionFunc = EnFd_SpinAndSpawnFire;
    }
}

void EnFd_SpinAndSpawnFire(EnFd* this, PlayState* play) {
    f32 deceleration;
    f32 tgtSpeed;
    f32 rotSpeed;

    if ((this->spinTimer <= 30) && (this->invincibilityTimer == 0)) {
        Actor_PlaySfx_Flagged(&this->actor, NA_SE_EN_FLAME_FIRE_ATTACK - SFX_FLAG);
    } else {
        Actor_PlaySfx_Flagged(&this->actor, NA_SE_EN_FLAME_ROLL - SFX_FLAG);
    }

    if (DECR(this->spinTimer) != 0) {
        this->actor.shape.rot.y += (this->runDir * 0x2000);
        if (this->spinTimer == 30 && this->invincibilityTimer == 0) {
            if (this->actor.xzDistToPlayer > 160.0f) {
                EnFd_SpawnChildFire(this, play, 8, 0);
            } else {
                EnFd_SpawnChildFire(this, play, 8, 1);
            }
        }
    } else {
        deceleration = this->actor.world.rot.y;
        deceleration -= this->actor.shape.rot.y;
        rotSpeed = 0.0f;
        tgtSpeed = fabsf(deceleration);
        deceleration /= tgtSpeed;
        Math_ApproachF(&rotSpeed, tgtSpeed, 0.6f, 0x2000);
        rotSpeed *= deceleration;
        this->actor.shape.rot.y += (s16)rotSpeed;
        rotSpeed = fabsf(rotSpeed);
        if ((s32)rotSpeed <= 300) {
            this->actor.shape.rot.y = this->actor.world.rot.y;
        }

        if (this->actor.shape.rot.y == this->actor.world.rot.y) {
            this->initYawToInitPos = Math_Vec3f_Yaw(&this->actor.home.pos, &this->actor.world.pos);
            this->curYawToInitPos = this->runDir < 0 ? 0xFFFF : 0;
            this->circlesToComplete = (play->state.frameCount & 7) + 2;
            this->spinTimer = Rand_S16Offset(30, 120);
            Animation_ChangeByInfo(&this->skelAnime, sAnimationInfo, ENFD_ANIM_3);
            this->actionFunc = EnFd_Run;
        }
    }
}

void EnFd_Run(EnFd* this, PlayState* play) {
    Actor* potentialThreat;
    s16 yawToYawTarget;
    f32 runRadiusTarget;
    Vec3f adjPos;

    if (EnFd_ShouldStopRunning(this, play, this->runRadius, &this->runDir)) {
        if (this->invincibilityTimer == 0) {
            this->actor.world.rot.y ^= 0x8000;
            this->actor.velocity.y = 6.0f;
            this->actor.speed = 0.0f;
            Animation_ChangeByInfo(&this->skelAnime, sAnimationInfo, ENFD_ANIM_1);
            this->actionFunc = EnFd_JumpToGround;
            return;
        }
    }

    yawToYawTarget = Math_Vec3f_Yaw(&this->actor.home.pos, &this->actor.world.pos) - this->initYawToInitPos;
    if (this->runDir > 0) {
        if ((u16)this->curYawToInitPos > (u16)(yawToYawTarget)) {
            this->circlesToComplete--;
        }
    } else if ((u16)this->curYawToInitPos < (u16)(yawToYawTarget)) {
        this->circlesToComplete--;
    }

    if (this->circlesToComplete < 0) {
        this->circlesToComplete = 0;
    }
    this->curYawToInitPos = yawToYawTarget;

    potentialThreat = EnFd_FindPotentialTheat(this, play);
    if ((potentialThreat != NULL) && (this->invincibilityTimer == 0)) {
        runRadiusTarget = Math_Vec3f_DistXYZ(&this->actor.home.pos, &potentialThreat->world.pos);
    } else {
        runRadiusTarget = 200.0f;
    }
    Math_SmoothStepToF(&this->runRadius, runRadiusTarget, 0.3f, 100.0f, 0.0f);
    EnFd_GetPosAdjAroundCircle(&adjPos, this, this->runRadius, this->runDir);
    Math_SmoothStepToS(&this->actor.shape.rot.y, RAD_TO_BINANG(Math_FAtan2F(adjPos.x, adjPos.z)), 4, 0xFA0, 1);
    this->actor.world.rot = this->actor.shape.rot;
    Actor_PlaySfx_Flagged(&this->actor, NA_SE_EN_FLAME_RUN - SFX_FLAG);
    if (this->skelAnime.curFrame == 6.0f || this->skelAnime.curFrame == 13.0f || this->skelAnime.curFrame == 28.0f) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_FLAME_KICK);
    }
    Math_SmoothStepToF(&this->actor.speed, 8.0f, 0.1f, 1.0f, 0.0f);
}

void EnFd_WaitForCore(EnFd* this, PlayState* play) {
    if (this->spinTimer != 0) {
        this->spinTimer--;
        if (this->spinTimer == 0) {
            Actor_Kill(&this->actor);
        }
    } else if (PARAMS_GET_NOSHIFT(this->actor.params, 15, 1)) {
        this->actionFunc = EnFd_Reappear;
    } else if (PARAMS_GET_NOSHIFT(this->actor.params, 14, 1)) {
        this->actor.params = 0;
        this->spinTimer = 30;
    }
}

void EnFd_Update(Actor* thisx, PlayState* play) {
    EnFd* this = (EnFd*)thisx;

    if (this->firstUpdateFlag) {
        func_800F5ACC(NA_BGM_MINI_BOSS);
        this->firstUpdateFlag = false;
    }

    if (this->actionFunc != EnFd_Reappear) {
        SkelAnime_Update(&this->skelAnime);
        EnFd_SpawnDot(this, play);
    }

    if (ACTOR_FLAGS_CHECK_ALL(&this->actor, ACTOR_FLAG_HOOKSHOT_ATTACHED)) {
        if (EnFd_SpawnCore(this, play)) {
            this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
            this->invincibilityTimer = 30;
            Actor_PlaySfx(&this->actor, NA_SE_EN_FLAME_DAMAGE);
            Enemy_StartFinishingBlow(play, &this->actor);
        } else {
            this->actor.flags &= ~ACTOR_FLAG_HOOKSHOT_ATTACHED;
        }
    } else if (this->actionFunc != EnFd_WaitForCore) {
        EnFd_ColliderCheck(this, play);
    }
    Actor_MoveXZGravity(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 0.0f, 0.0f, 0.0f, UPDBGCHECKINFO_FLAG_2);
    EnFd_Fade(this, play);
    this->actionFunc(this, play);
    EnFd_UpdateEffectsDots(this);
    EnFd_UpdateEffectsFlames(this);
    if (this->actionFunc != EnFd_Reappear && this->actionFunc != EnFd_SpinAndGrow &&
        this->actionFunc != EnFd_WaitForCore) {
        if (this->attackTimer == 0 && this->invincibilityTimer == 0) {
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
        }

        if ((this->actionFunc == EnFd_Run) || (this->actionFunc == EnFd_SpinAndSpawnFire)) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
        }
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
    }
}

s32 EnFd_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx,
                          Gfx** gfxP) {
    EnFd* this = (EnFd*)thisx;

    if (this->invincibilityTimer != 0) {
        switch (limbIndex) {
            case 13:
            case 21:
                *dList = NULL;
                break;
        }
    }

    return false;
}

void EnFd_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx, Gfx** gfxP) {
    EnFd* this = (EnFd*)thisx;
    Vec3f initialPos = { 0.0f, 0.0f, 0.0f };
    Vec3f pos = { 0.0f, 0.0f, 0.0f };
    Vec3f accel = { 0.0f, 0.0f, 0.0f };
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    s32 i;

    if (limbIndex == 21) {
        Matrix_MultVec3f(&initialPos, &this->corePos);
    }

    if (limbIndex == 13) {
        Matrix_MultVec3f(&initialPos, &this->actor.focus.pos);
    }

    if (limbIndex == 3 || limbIndex == 6 || limbIndex == 7 || limbIndex == 10 || limbIndex == 14 || limbIndex == 15 ||
        limbIndex == 17 || limbIndex == 18 || limbIndex == 20 || limbIndex == 22 || limbIndex == 23 ||
        limbIndex == 24 || limbIndex == 25 || limbIndex == 26) {
        if ((play->state.frameCount % 2) != 0) {
            for (i = 0; i < 1; i++) {
                Matrix_MultVec3f(&initialPos, &pos);
                pos.x += (Rand_ZeroOne() - 0.5f) * 20.0f;
                pos.y += (Rand_ZeroOne() - 0.5f) * 40.0f;
                pos.z += (Rand_ZeroOne() - 0.5f) * 20.0f;
                accel.x = (Rand_ZeroOne() - 0.5f) * 0.4f;
                accel.y = ((Rand_ZeroOne() - 0.5f) * 0.2f) + 0.6f;
                accel.z = (Rand_ZeroOne() - 0.5f) * 0.4f;
                EnFd_SpawnEffect(this, FD_EFFECT_DOT, &pos, &velocity, &accel, 0, 0.006f, 0.0f);
            }
        }
    }

    Collider_UpdateSpheres(limbIndex, &this->collider);
}

static Color_RGB8 sFlamePrimColors[] = {
    { 255, 255, 200 },
    { 200, 200, 200 },
    { 255, 255, 0 },
};
static Color_RGB8 sFlameEnvColors[] = {
    { 0, 255, 0 },
    { 0, 0, 255 },
    { 255, 0, 0 },
};

void EnFd_Draw(Actor* thisx, PlayState* play) {
    EnFd* this = (EnFd*)thisx;
    s32 clampedHealth;
    u32 frames;

    frames = play->state.frameCount;

    OPEN_DISPS(play->state.gfxCtx);

    Matrix_Push();
    EnFd_DrawEffectsDots(this, play);
    EnFd_DrawEffectsFlames(this, play);
    Matrix_Pop();
    if (this->actionFunc != EnFd_Reappear && !(this->fadeAlpha < 0.9f)) {
        Gfx_SetupDL_25Xlu(play->state.gfxCtx);
        clampedHealth = CLAMP(thisx->colChkInfo.health - 1, 0, 23) / 8;
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 128, sFlamePrimColors[clampedHealth].r, sFlamePrimColors[clampedHealth].g,
                        sFlamePrimColors[clampedHealth].b, (u8)this->fadeAlpha);
        gDPSetEnvColor(POLY_XLU_DISP++, sFlameEnvColors[clampedHealth].r, sFlameEnvColors[clampedHealth].g,
                       sFlameEnvColors[clampedHealth].b, (u8)this->fadeAlpha);
        gSPSegment(POLY_XLU_DISP++, 0x8,
                   Gfx_TwoTexScroll(play->state.gfxCtx, G_TX_RENDERTILE, 0, 0, 0x20, 0x40, 1, 0,
                                    0xFF - (u8)(frames * 6), 8, 0x40));
        gDPPipeSync(POLY_XLU_DISP++);
        gSPSegment(POLY_XLU_DISP++, 0x09, gActorSetupXluDL);

        POLY_XLU_DISP =
            SkelAnime_DrawFlex(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount,
                               EnFd_OverrideLimbDraw, EnFd_PostLimbDraw, this, POLY_XLU_DISP);
    }

    if (this->fadeAlpha) {
    }

    CLOSE_DISPS();
}

void EnFd_SpawnEffect(EnFd* this, u8 type, Vec3f* pos, Vec3f* velocity, Vec3f* accel, u8 timer, f32 scale,
                      f32 scaleStep) {
    EnFdEffect* eff = this->effects;
    s16 i;

    for (i = 0; i < EN_FD_EFFECT_COUNT; i++, eff++) {
        if (eff->type != FD_EFFECT_NONE) {
            continue;
        }
        eff->scale = scale;
        eff->scaleStep = scaleStep;
        eff->initialTimer = eff->timer = timer;
        eff->type = type;
        eff->pos = *pos;
        eff->accel = *accel;
        eff->velocity = *velocity;
        if (eff->type == FD_EFFECT_DOT) {
            eff->color.a = 255;
            eff->timer = (s16)(Rand_ZeroOne() * 10.0f);
        }
        return;
    }
}

void EnFd_UpdateEffectsFlames(EnFd* this) {
    s16 i;
    EnFdEffect* eff = this->effects;

    for (i = 0; i < EN_FD_EFFECT_COUNT; i++, eff++) {
        if (eff->type == FD_EFFECT_FLAME) {
            eff->timer--;
            if (eff->timer == 0) {
                eff->type = FD_EFFECT_NONE;
            }
            eff->accel.x = (Rand_ZeroOne() * 0.4f) - 0.2f;
            eff->accel.z = (Rand_ZeroOne() * 0.4f) - 0.2f;
            eff->pos.x += eff->velocity.x;
            eff->pos.y += eff->velocity.y;
            eff->pos.z += eff->velocity.z;
            eff->velocity.x += eff->accel.x;
            eff->velocity.y += eff->accel.y;
            eff->velocity.z += eff->accel.z;
            eff->scale += eff->scaleStep;
        }
    }
}

void EnFd_UpdateEffectsDots(EnFd* this) {
    EnFdEffect* eff = this->effects;
    s16 i;
    Color_RGBA8 dotColors[] = {
        { 255, 128, 0, 0 },
        { 255, 0, 0, 0 },
        { 255, 255, 0, 0 },
        { 255, 0, 0, 0 },
    };

    for (i = 0; i < EN_FD_EFFECT_COUNT; i++, eff++) {
        if (eff->type == FD_EFFECT_DOT) {
            eff->pos.x += eff->velocity.x;
            eff->pos.y += eff->velocity.y;
            eff->pos.z += eff->velocity.z;
            eff->timer++;
            eff->velocity.x += eff->accel.x;
            eff->velocity.y += eff->accel.y;
            eff->velocity.z += eff->accel.z;

            eff->color.r = dotColors[eff->timer % 4].r;
            eff->color.g = dotColors[eff->timer % 4].g;
            eff->color.b = dotColors[eff->timer % 4].b;
            if (eff->color.a > 30) {
                eff->color.a -= 30;
            } else {
                eff->color.a = 0;
                eff->type = FD_EFFECT_NONE;
            }
        }
    }
}

void EnFd_DrawEffectsFlames(EnFd* this, PlayState* play) {
    static void* dustTextures[] = {
        gDust8Tex, gDust7Tex, gDust6Tex, gDust5Tex, gDust4Tex, gDust3Tex, gDust2Tex, gDust1Tex,
    };
    s16 i;
    s16 idx;
    s16 materialFlag;
    s32 color = CLAMP(this->actor.colChkInfo.health - 1, 0, 23) / 8;
    EnFdEffect* eff = this->effects;

    OPEN_DISPS(play->state.gfxCtx);

    materialFlag = false;
    Gfx_SetupDL_25Xlu(play->state.gfxCtx);

    for (i = 0; i < EN_FD_EFFECT_COUNT; i++, eff++) {
        if (eff->type != FD_EFFECT_FLAME) {
            continue;
        }

        if (!materialFlag) {
            POLY_XLU_DISP = Gfx_SetupDL(POLY_XLU_DISP, SETUPDL_0);
            gSPDisplayList(POLY_XLU_DISP++, gFlareDancerDL_7928);
            gDPSetEnvColor(POLY_XLU_DISP++, sFlameEnvColors[color].r, sFlameEnvColors[color].g,
                           sFlameEnvColors[color].b, (u8)this->fadeAlpha);
            materialFlag = true;
        }

        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, sFlamePrimColors[color].r, sFlamePrimColors[color].g,
                        sFlamePrimColors[color].b, (u8)this->fadeAlpha);
        gDPPipeSync(POLY_XLU_DISP++);
        Matrix_Translate(eff->pos.x, eff->pos.y, eff->pos.z, MTXMODE_NEW);
        Matrix_ReplaceRotation(&play->billboardMtxF);
        Matrix_Scale(eff->scale, eff->scale, 1.0f, MTXMODE_APPLY);
        MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
        idx = eff->timer * (8.0f / eff->initialTimer);
        gSPSegment(POLY_XLU_DISP++, 0x8, SEGMENTED_TO_VIRTUAL(dustTextures[idx]));
        gSPDisplayList(POLY_XLU_DISP++, gFlareDancerSquareParticleDL);
    }

    CLOSE_DISPS();
}

void EnFd_DrawEffectsDots(EnFd* this, PlayState* play) {
    s16 i;
    s16 materialFlag;
    EnFdEffect* eff = this->effects;

    OPEN_DISPS(play->state.gfxCtx);

    materialFlag = false;
    Gfx_SetupDL_25Xlu(play->state.gfxCtx);

    for (i = 0; i < EN_FD_EFFECT_COUNT; i++, eff++) {
        if (eff->type != FD_EFFECT_DOT) {
            continue;
        }

        if (!materialFlag) {
            Gfx_SetupDL_25Xlu(play->state.gfxCtx);
            gSPDisplayList(POLY_XLU_DISP++, gFlareDancerDL_79F8);
            materialFlag = true;
        }

        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, eff->color.r, eff->color.g, eff->color.b,
                        (u8)(eff->color.a * (this->fadeAlpha / 255.0f)));
        gDPPipeSync(POLY_XLU_DISP++);
        Matrix_Translate(eff->pos.x, eff->pos.y, eff->pos.z, MTXMODE_NEW);
        Matrix_ReplaceRotation(&play->billboardMtxF);
        Matrix_Scale(eff->scale, eff->scale, 1.0f, MTXMODE_APPLY);
        MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
        gSPDisplayList(POLY_XLU_DISP++, gFlareDancerTriangleParticleDL);
    }

    CLOSE_DISPS();
}

OVL_INFO_NATIVE_ENEMY(0x99, En_Fd_Profile);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_FD_INIT, EnFd_Init);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_FD_DESTROY, EnFd_Destroy);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_FD_UPDATE, EnFd_Update);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_FD_DRAW, EnFd_Draw);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_FD_PROFILE, En_Fd_Profile);
static const u32 kNativePortOrigin[] = { 0x4e505431, 0, 0x99, 0x9e, 0xd5a64ddc };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_FD_ORIGIN, kNativePortOrigin);
