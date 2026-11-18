#include "oot_compat.h"
#include "types/en_dha.h"
#undef ACTOR_EN_DH
#define ACTOR_EN_DH 0x00a4
#undef ACTOR_EN_DHA
#define ACTOR_EN_DHA 0x00a5
#undef OBJECT_DH
#define OBJECT_DH 0x10a6
#undef NA_SE_EN_DEADHAND_GRIP
#define NA_SE_EN_DEADHAND_GRIP 0x3902
#undef NA_SE_EN_DEADHAND_HAND_AT
#define NA_SE_EN_DEADHAND_HAND_AT 0x3903
#undef NA_SE_EN_DEADHAND_DAMAGE
#define NA_SE_EN_DEADHAND_DAMAGE 0x39a9
#undef NA_SE_EN_DEADHAND_HAND_DEAD
#define NA_SE_EN_DEADHAND_HAND_DEAD 0x39aa
#define object_dh_Skel_000BD8 (*(FlexSkeletonHeader*)0x06000bd8)
#define object_dh_Anim_0015B0 (*(AnimationHeader*)0x060015b0)

#undef ActorShape_Init
#define ActorShape_Init ((void (*)(ActorShape* shape, f32 yOffset, ActorShadowFunc shadowDraw, f32 shadowScale))0x800b3ba4)
#undef Actor_ApplyDamage
#define Actor_ApplyDamage ((u8 (*)(Actor* actor))0x800be22c)
#undef BgCheck_EntityLineTest1
#define BgCheck_EntityLineTest1 ((s32 (*)(CollisionContext*, Vec3f*, Vec3f*, Vec3f*, CollisionPoly**, s32, s32, s32, s32, s32*))0x800c55c4)
#undef Actor_Kill
#define Actor_Kill FOREIGN_ENEMY_API_KILL
#undef Actor_PlaySfx
#define Actor_PlaySfx FOREIGN_ENEMY_API_SFX_ACTOR
#undef Actor_ProcessInitChain
#define Actor_ProcessInitChain ((void (*)(Actor* actor, InitChainEntry* ichain))0x800ffadc)
#undef Actor_SetColorFilter
#define Actor_SetColorFilter FOREIGN_ENEMY_API_COLOR_FILTER
#undef Animation_PlayLoop
#define Animation_PlayLoop ((void (*)(SkelAnime* skelAnime, AnimationHeader* animation))0x80137540)
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
#undef CollisionCheck_SetOC
#define CollisionCheck_SetOC FOREIGN_ENEMY_API_COLLISION_OC
#undef Gfx_SetupDL_25Opa
#define Gfx_SetupDL_25Opa ((void (*)(GraphicsContext* gfxCtx))0x8012c28c)
#undef Item_DropCollectibleRandom
#define Item_DropCollectibleRandom FOREIGN_ENEMY_API_RANDOM_DROP
#undef Math_CosS
#define Math_CosS ((f32 (*)(s16 angle))0x800fed44)
#undef Math_SinS
#define Math_SinS ((f32 (*)(s16 angle))0x800fed84)
#undef Math_SmoothStepToF
#define Math_SmoothStepToF ((f32 (*)(f32* pValue, f32 target, f32 fraction, f32 step, f32 minStep))0x800ffcd8)
#undef Math_SmoothStepToS
#define Math_SmoothStepToS ((s16 (*)(s16* pValue, s16 target, s16 scale, s16 step, s16 minStep))0x800ffebc)
#undef Math_Vec3f_DistXYZ
#define Math_Vec3f_DistXYZ ((f32 (*)(Vec3f* a, Vec3f* b))0x800ff884)
#undef Math_Vec3f_Pitch
#define Math_Vec3f_Pitch ((s16 (*)(Vec3f* a, Vec3f* b))0x800ffa94)
#undef Math_Vec3f_Yaw
#define Math_Vec3f_Yaw ((s16 (*)(Vec3f* origin, Vec3f* point))0x800ffa60)
#undef Matrix_MultVec3f
#define Matrix_MultVec3f ((void (*)(Vec3f* src, Vec3f* dest))0x80181a98)
#undef Matrix_RotateZYX
#define Matrix_RotateZYX ((void (*)(s16 x, s16 y, s16 z, u8 mode))0x8018103c)
#undef Matrix_Translate
#define Matrix_Translate FOREIGN_ENEMY_API_MATRIX_TRANSLATE
#undef Rand_ZeroOne
#define Rand_ZeroOne ((f32 (*)(void))0x80086fdc)
#undef SkelAnime_DrawFlexOpa
#define SkelAnime_DrawFlexOpa ((void (*)(PlayState* play, void** skeleton, Vec3s* jointTable, s32 dListCount, void* overrideLimbDraw, void* postLimbDraw, void* arg))0x80133f28)
#undef SkelAnime_InitFlex
#define SkelAnime_InitFlex ((void (*)(PlayState* play, SkelAnime* skelAnime, FlexSkeletonHeader* skeletonHeaderSeg, AnimationHeader* animation, Vec3s* jointTable, Vec3s* morphTable, s32 limbCount))0x80136b30)
#undef SkelAnime_Update
#define SkelAnime_Update ((s32 (*)(SkelAnime* skelAnime))0x80136cd0)

#include "types/en_dh.h"
#define HIT_SPECIAL_EFFECT_NONE 0
#define HIT_BACKLASH_NONE 0
#define PLAYER_STATE2_7 (1u << 7)

static void armAngles(Vec3f* from, Vec3f* to, Vec3s* rotation) {
    rotation->x = Math_Vec3f_Pitch(from, to);
    rotation->y = Math_Vec3f_Yaw(from, to);
    rotation->z = 0;
}

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED)

void EnDha_Init(Actor* thisx, PlayState* play);
void EnDha_Destroy(Actor* thisx, PlayState* play);
void EnDha_Update(Actor* thisx, PlayState* play);
void EnDha_Draw(Actor* thisx, PlayState* play);

void EnDha_SetupWait(EnDha* this);
void EnDha_Wait(EnDha* this, PlayState* play);
void EnDha_SetupTakeDamage(EnDha* this);
void EnDha_TakeDamage(EnDha* this, PlayState* play);
void EnDha_SetupDeath(EnDha* this);
void EnDha_Die(EnDha* this, PlayState* play);
void EnDha_UpdateHealth(EnDha* this, PlayState* play);

ActorProfile En_Dha_Profile = {
    0xca5, ACTORCAT_ENEMY, FLAGS, OBJECT_DH, sizeof(EnDha), EnDha_Init, EnDha_Destroy, EnDha_Update, EnDha_Draw,
};

static DamageTable sDamageTable = { {
    DMG_ENTRY(0, 0x0), DMG_ENTRY(2, 0xF), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(2, 0xF), DMG_ENTRY(2, 0xF), DMG_ENTRY(4, 0xF), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0), DMG_ENTRY(2, 0xF), DMG_ENTRY(4, 0xF),
    DMG_ENTRY(2, 0xF), DMG_ENTRY(4, 0xF), DMG_ENTRY(8, 0xF), DMG_ENTRY(4, 0xF), DMG_ENTRY(0, 0x0), DMG_ENTRY(0, 0x0),
    DMG_ENTRY(4, 0xF), DMG_ENTRY(0, 0x0),
} };

static ColliderJntSphElementInit sJntSphElementsInit[] = {
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0x00000000, HIT_SPECIAL_EFFECT_NONE, 0x00 },
            { 0xFFCFFFFF, HIT_BACKLASH_NONE, 0x00 },
            ATELEM_NONE,
            ACELEM_ON,
            OCELEM_NONE,
        },
        { 1, { { 0, 0, 0 }, 12 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0x00000000, HIT_SPECIAL_EFFECT_NONE, 0x00 },
            { 0xFFCFFFFF, HIT_BACKLASH_NONE, 0x00 },
            ATELEM_NONE,
            ACELEM_ON,
            OCELEM_ON,
        },
        { 2, { { 3200, 0, 0 }, 10 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0x00000000, HIT_SPECIAL_EFFECT_NONE, 0x00 },
            { 0xFFCFFFFF, HIT_BACKLASH_NONE, 0x00 },
            ATELEM_NONE,
            ACELEM_ON,
            OCELEM_ON,
        },
        { 3, { { 1200, 0, 0 }, 10 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0x00000000, HIT_SPECIAL_EFFECT_NONE, 0x00 },
            { 0xFFCFFFFF, HIT_BACKLASH_NONE, 0x00 },
            ATELEM_NONE,
            ACELEM_ON,
            OCELEM_NONE,
        },
        { 4, { { 2700, 0, 0 }, 10 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0x00000000, HIT_SPECIAL_EFFECT_NONE, 0x00 },
            { 0xFFCFFFFF, HIT_BACKLASH_NONE, 0x00 },
            ATELEM_NONE,
            ACELEM_ON,
            OCELEM_ON,
        },
        { 5, { { 1200, 0, 0 }, 10 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COL_MATERIAL_HIT6,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER | OC1_TYPE_1,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sJntSphElementsInit),
    sJntSphElementsInit,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(naviEnemyId, 0xff, ICHAIN_CONTINUE),
    ICHAIN_F32(lockOnArrowOffset, 2000, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 10, ICHAIN_STOP),
};

void EnDha_SetupAction(EnDha* this, EnDhaActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

void EnDha_Init(Actor* thisx, PlayState* play) {
    EnDha* this = (EnDha*)thisx;

    Actor_ProcessInitChain(&this->actor, sInitChain);
    this->actor.colChkInfo.damageTable = &sDamageTable;
    SkelAnime_InitFlex(play, &this->skelAnime, &object_dh_Skel_000BD8, &object_dh_Anim_0015B0, this->jointTable,
                       this->morphTable, 4);
    ActorShape_Init(&this->actor.shape, 0, ActorShadow_DrawFeet, 90.0f);
    this->actor.focus.pos = this->actor.world.pos;
    this->actor.focus.pos.y += 50.0f;
    this->actor.colChkInfo.mass = MASS_HEAVY;
    this->actor.colChkInfo.health = 8;
    this->limbAngleX[0] = -0x4000;
    Collider_InitJntSph(play, &this->collider);
    Collider_SetJntSph(play, &this->collider, &this->actor, &sJntSphInit, this->colliderElements);
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;

    EnDha_SetupWait(this);
}

static void EnDha_ReleasePlayer(EnDha* hand, Player* player) {
    int ownedGrab = player->actor.parent == &hand->actor;
    if (ownedGrab) {
        player->actor.parent = NULL;
        if (player->stateFlags2 & PLAYER_STATE2_7)
            player->av2.actionVar2 = 100;
    }
    if ((ownedGrab || (hand->unk_1CC && !player->actor.parent)) && ABS(player->invincibilityTimer) < 30)
        player->invincibilityTimer = -30;
    hand->unk_1CC = 0;
}

void EnDha_Destroy(Actor* thisx, PlayState* play) {
    EnDha* this = (EnDha*)thisx;

    Player* player = GET_PLAYER(play);
    if (player)
        EnDha_ReleasePlayer(this, player);
    Collider_DestroyJntSph(play, &this->collider);
}

void EnDha_SetupWait(EnDha* this) {
    Animation_PlayLoop(&this->skelAnime, &object_dh_Anim_0015B0);
    this->unk_1C0 = 0;
    this->actionTimer = ((Rand_ZeroOne() * 10.0f) + 5.0f);
    this->actor.speed = 0.0f;
    this->actor.world.rot.y = this->actor.shape.rot.y;
    this->actor.home.rot.z = 1;
    EnDha_SetupAction(this, EnDha_Wait);
}

static s32 EnDha_CanReachPlayer(EnDha* this, PlayState* play, Vec3f* target) {
    Vec3f start = this->actor.world.pos;
    Vec3f hit;
    CollisionPoly* poly;
    s32 bgId;
    start.y += 12.f;
    return !BgCheck_EntityLineTest1(&play->colCtx, &start, target, &hit, &poly, true, true, true, false, &bgId);
}

void EnDha_Wait(EnDha* this, PlayState* play) {
    Vec3f armPosMultiplier1 = { 0.0f, 0.0f, 55.0f };
    Vec3f armPosMultiplier2 = { 0.0f, 0.0f, -54.0f };
    Player* player = GET_PLAYER(play);
    Vec3f playerPos = player->actor.world.pos;
    Vec3s angle;
    s16 yaw;

    playerPos.x += Math_SinS(player->actor.shape.rot.y) * -5.0f;
    playerPos.z += Math_CosS(player->actor.shape.rot.y) * -5.0f;

    playerPos.y += player->transformation == MM_PLAYER_FORM_HUMAN ? 38.f : 56.f;

    if (player->actor.parent == &this->actor || this->actor.xzDistToPlayer <= 100.0f) {
        this->handAngle.y = this->handAngle.x = this->limbAngleY = 0;

        if (player->actor.parent == &this->actor || Math_Vec3f_DistXYZ(&playerPos, &this->handPos[0]) <= 12.0f) {
            if (this->unk_1CC == 0) {
                if (!player->actor.parent && !player->invincibilityTimer &&
                    (player->actor.bgCheckFlags & BGCHECKFLAG_GROUND_STRICT) &&
                    EnDha_CanReachPlayer(this, play, &playerPos) && play->grabPlayer(play, player)) {
                    player->av2.actionVar2 = 0;
                    player->actor.parent = &this->actor;
                    this->timer = 0;
                    this->unk_1CC++;

                    if (this->actor.parent != NULL) {
                        this->actor.parent->params = ENDH_START_ATTACK_GRAB;
                    }

                    Actor_PlaySfx(&this->actor, NA_SE_EN_DEADHAND_GRIP);
                }
            } else {
                this->timer += 0x1194;
                this->limbAngleY = Math_SinS(this->timer) * 1820.0f;

                if (!(player->stateFlags2 & PLAYER_STATE2_7) || player->actor.parent != &this->actor) {
                    EnDha_ReleasePlayer(this, player);
                    EnDha_SetupTakeDamage(this);
                    return;
                }

                if (this->timer < -0x6E6B) {
                    Actor_PlaySfx(&this->actor, NA_SE_EN_DEADHAND_GRIP);
                }
            }

            armAngles(&this->handPos[1], &playerPos, &this->handAngle);
            this->handAngle.y -= this->actor.shape.rot.y + this->limbAngleY;
            this->handAngle.x -= this->actor.shape.rot.x + this->limbAngleX[0] + this->limbAngleX[1];
        } else {
            EnDha_ReleasePlayer(this, player);

            if (this->actor.home.rot.z != 0) {
                Actor_PlaySfx(&this->actor, NA_SE_EN_DEADHAND_HAND_AT);
                this->actor.home.rot.z = 0;
            }
        }

        this->actor.shape.rot.y = Math_Vec3f_Yaw(&this->actor.world.pos, &playerPos);

        Math_SmoothStepToF(&this->handPos[0].x, playerPos.x, 1.0f, 16.0f, 0.0f);
        Math_SmoothStepToF(&this->handPos[0].y, playerPos.y, 1.0f, 16.0f, 0.0f);
        Math_SmoothStepToF(&this->handPos[0].z, playerPos.z, 1.0f, 16.0f, 0.0f);

        armAngles(&this->armPos, &this->handPos[0], &angle);
        Matrix_Translate(this->handPos[0].x, this->handPos[0].y, this->handPos[0].z, MTXMODE_NEW);
        Matrix_RotateZYX(angle.x, angle.y, 0, MTXMODE_APPLY);
        Matrix_MultVec3f(&armPosMultiplier2, &this->armPos);
        Matrix_Translate(this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z, MTXMODE_NEW);
        armAngles(&this->actor.world.pos, &this->armPos, &angle);
        Matrix_RotateZYX(angle.x, angle.y, 0, MTXMODE_APPLY);
        Matrix_MultVec3f(&armPosMultiplier1, &this->armPos);
        this->limbAngleX[0] = Math_Vec3f_Pitch(&this->actor.world.pos, &this->armPos);
        yaw = Math_Vec3f_Yaw(&this->actor.world.pos, &this->armPos) - this->actor.shape.rot.y;

        if (ABS(yaw) >= 0x4000) {
            this->limbAngleX[0] = -0x8000 - this->limbAngleX[0];
        }

        this->limbAngleX[1] = (Math_Vec3f_Pitch(&this->armPos, &this->handPos[0]) - this->limbAngleX[0]);

        if (this->limbAngleX[1] < 0) {
            this->limbAngleX[0] += this->limbAngleX[1] * 2;
            this->limbAngleX[1] *= -2;
        }
    } else {
        EnDha_ReleasePlayer(this, player);

        this->actor.home.rot.z = 1;
        Math_SmoothStepToS(&this->limbAngleX[1], 0, 1, 0x3E8, 0);
        Math_SmoothStepToS(&this->limbAngleX[0], -0x4000, 1, 0x3E8, 0);
        SkelAnime_Update(&this->skelAnime);
    }
}

void EnDha_SetupTakeDamage(EnDha* this) {
    this->actionTimer = 15;
    EnDha_SetupAction(this, EnDha_TakeDamage);
}

void EnDha_TakeDamage(EnDha* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    EnDha_ReleasePlayer(this, player);

    Math_SmoothStepToS(&this->limbAngleX[1], 0, 1, 2000, 0);
    Math_SmoothStepToS(&this->limbAngleY, 0, 1, 600, 0);
    Math_SmoothStepToS(&this->limbAngleX[0], -0x4000, 1, 2000, 0);
    SkelAnime_Update(&this->skelAnime);
    this->actionTimer--;

    if (this->actionTimer == 0) {
        EnDha_SetupWait(this);
    }
}

void EnDha_SetupDeath(EnDha* this) {
    this->unk_1C0 = 8;
    this->actionTimer = 300;

    if (this->actor.parent != NULL) {
        if (this->actor.parent->params != ENDH_DEATH) {
            Actor_PlaySfx(&this->actor, NA_SE_EN_DEADHAND_HAND_DEAD);
        }
        if (this->actor.parent->params <= ENDH_WAIT_UNDERGROUND) {
            this->actor.parent->params--;
        }
    }

    EnDha_SetupAction(this, EnDha_Die);
}

void EnDha_Die(EnDha* this, PlayState* play) {
    s16 angle;
    Player* player = GET_PLAYER(play);

    EnDha_ReleasePlayer(this, player);

    Math_SmoothStepToS(&this->limbAngleX[1], 0, 1, 0x7D0, 0);
    angle = Math_SmoothStepToS(&this->limbAngleX[0], -0x4000, 1, 0x7D0, 0);
    SkelAnime_Update(&this->skelAnime);

    if (angle == 0) {

        if (this->actionTimer != 0) {
            if (-12000.0f < this->actor.shape.yOffset) {
                this->actor.shape.yOffset -= 1000.0f;

            } else {
                this->actionTimer--;

                if ((this->actor.parent != NULL) && (this->actor.parent->params == ENDH_DEATH)) {
                    Actor_Kill(&this->actor);
                }
            }
        } else {
            this->actor.shape.yOffset += 500.0f;

            if (this->actor.shape.yOffset == 0.0f) {
                EnDha_SetupWait(this);
            }
        }
    }
}

void EnDha_UpdateHealth(EnDha* this, PlayState* play) {
    if (!((this->unk_1C0 >= 8) || !(this->collider.base.acFlags & AC_HIT))) {
        this->collider.base.acFlags &= ~AC_HIT;

        if (this->actor.colChkInfo.damageEffect == 0 || this->actor.colChkInfo.damageEffect == 6) {
            return;
        } else {
            Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 8);
            if (Actor_ApplyDamage(&this->actor) == 0) {
                EnDha_SetupDeath(this);
                this->actor.colChkInfo.health = 8;
                Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos,
                                           COLLECTIBLE_DROP_RANDOM_PARAMS(COLLECTIBLE_DROP_TABLE_14, false));
            } else {
                Actor_PlaySfx(&this->actor, NA_SE_EN_DEADHAND_DAMAGE);
                this->unk_1C0 = 9;
                EnDha_SetupTakeDamage(this);
            }
        }
    }

    if (this->actionFunc != EnDha_Die && this->actor.parent->params == ENDH_DEATH) {
        EnDha_SetupDeath(this);
    }
}

void EnDha_Update(Actor* thisx, PlayState* play) {
    EnDha* this = (EnDha*)thisx;

    Actor* parent = play->actorCtx.actors[ACTORCAT_ENEMY].first;
    while (parent && parent != this->actor.parent)
        parent = parent->next;
    if (!parent || parent->id != 0xca4 || !parent->update) {
        this->actor.parent = NULL;
        Actor_Kill(thisx);
        return;
    }

    EnDha_UpdateHealth(this, play);
    this->actionFunc(this, play);
    CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
}

s32 EnDha_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnDha* this = (EnDha*)thisx;

    if (limbIndex == 1) {
        rot->y = -(s16)(this->limbAngleX[0] + 0x4000);
        rot->z += this->limbAngleY;
    } else if (limbIndex == 2) {
        rot->z = this->limbAngleX[1];
        rot->y -= this->limbAngleY;
    } else if (limbIndex == 3) {
        rot->y = -this->handAngle.y;
        rot->z = -this->handAngle.x;
    }

    return false;
}

void EnDha_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    Vec3f handVec = { 1100.0f, 0.0f, 0.0f };
    Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };
    EnDha* this = (EnDha*)thisx;

    switch (limbIndex) {
        case 1:
            Collider_UpdateSpheres(2, &this->collider);
            Collider_UpdateSpheres(3, &this->collider);
            break;
        case 2:
            Collider_UpdateSpheres(4, &this->collider);
            Collider_UpdateSpheres(5, &this->collider);
            Matrix_MultVec3f(&zeroVec, &this->armPos);
            break;
        case 3:
            Collider_UpdateSpheres(1, &this->collider);
            Matrix_MultVec3f(&handVec, &this->handPos[0]);
            Matrix_MultVec3f(&zeroVec, &this->handPos[1]);
            break;
    }
}

void EnDha_Draw(Actor* thisx, PlayState* play) {
    EnDha* this = (EnDha*)thisx;

    Gfx_SetupDL_25Opa(play->state.gfxCtx);
    SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount,
                          EnDha_OverrideLimbDraw, EnDha_PostLimbDraw, this);
}

OVL_INFO_NATIVE_ENEMY(0xa5, En_Dha_Profile);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_DHA_INIT, EnDha_Init);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_DHA_DESTROY, EnDha_Destroy);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_DHA_UPDATE, EnDha_Update);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_DHA_DRAW, EnDha_Draw);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_DHA_PROFILE, En_Dha_Profile);
static const u32 kNativePortOrigin[] = { 0x4e505431, 0, 0xa5, 0xa6, 0x2c8fd870 };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_DHA_ORIGIN, kNativePortOrigin);
