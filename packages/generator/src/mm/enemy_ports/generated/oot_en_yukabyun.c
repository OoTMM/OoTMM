#include "oot_compat.h"
#include <combo/enemy_placement.h>
#include "types/en_yukabyun.h"
#undef ACTOR_EN_YUKABYUN
#define ACTOR_EN_YUKABYUN 0x0c6b
#undef OBJECT_YUKABYUN
#define OBJECT_YUKABYUN 0x105f
#undef NA_SE_EN_YUKABYUN_FLY
#define NA_SE_EN_YUKABYUN_FLY 0x3979
#undef NA_SE_EN_OCTAROCK_ROCK
#define NA_SE_EN_OCTAROCK_ROCK 0x38c0
#define gFloorTileEnemyBottomTex ((void*)0x06000000)
#define gFloorTileEnemyTopTex ((void*)0x06000af0)
#define gFloorTileEnemyFragmentDL ((Gfx*)0x06000a60)
#define gFloorTileEnemyDL ((Gfx*)0x06000970)
#undef EffectSsHahen_SpawnBurst
extern void FOREIGN_ENEMY_API_TILE_FRAGMENTS(PlayState*, Vec3f*, f32, s16, s16, s16, s16, s16, s16, Gfx*);
#define EffectSsHahen_SpawnBurst FOREIGN_ENEMY_API_TILE_FRAGMENTS
#undef ActorShape_Init
#define ActorShape_Init ((void (*)(ActorShape* shape, f32 yOffset, ActorShadowFunc shadowDraw, f32 shadowScale))0x800b3ba4)
#undef Actor_Kill
#define Actor_Kill FOREIGN_ENEMY_API_KILL
#undef Actor_PlaySfx_Flagged
#define Actor_PlaySfx_Flagged FOREIGN_ENEMY_API_SFX_FLAGGED
#undef Actor_ProcessInitChain
#define Actor_ProcessInitChain ((void (*)(Actor* actor, InitChainEntry* ichain))0x800ffadc)
#undef Actor_SetFocus
#define Actor_SetFocus ((void (*)(Actor* actor, f32 yOffset))0x800b675c)
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
#undef Math_StepToF
#define Math_StepToF ((s32 (*)(f32* pValue, f32 target, f32 step))0x800ff03c)
#undef SfxSource_PlaySfxAtFixedWorldPos
#define SfxSource_PlaySfxAtFixedWorldPos FOREIGN_ENEMY_API_SFX_WORLD
#undef Actor_MoveXZGravity
#define Actor_MoveXZGravity ((void (*)(Actor* actor))0x800b6a88)
#undef Actor_UpdateBgCheckInfo
#define Actor_UpdateBgCheckInfo ((void (*)(PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight, s32 flags))0x800b78b8)
#undef Gfx_SetupDL_25Opa
#define Gfx_SetupDL_25Opa ((void (*)(GraphicsContext* gfxCtx))0x8012c28c)

#define FLAGS ACTOR_FLAG_UPDATE_CULLING_DISABLED

void EnYukabyun_Init(Actor* thisx, PlayState* play);
void EnYukabyun_Destroy(Actor* thisx, PlayState* play);
void EnYukabyun_Update(Actor* thisx, PlayState* play);
void EnYukabyun_Draw(Actor* thisx, PlayState* play);

void func_80B43A94(EnYukabyun* this, PlayState* play);
void func_80B43AD4(EnYukabyun* this, PlayState* play);
void func_80B43B6C(EnYukabyun* this, PlayState* play);

ActorProfile En_Yukabyun_Profile = {
    ACTOR_EN_YUKABYUN,  ACTORCAT_ENEMY,    FLAGS,           OBJECT_YUKABYUN, sizeof(EnYukabyun), EnYukabyun_Init,
    EnYukabyun_Destroy, EnYukabyun_Update, EnYukabyun_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COL_MATERIAL_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_NO_PUSH | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0xFFCFFFFF, HIT_SPECIAL_EFFECT_NONE, 0x04 },
        { 0xFFCFFFFF, HIT_BACKLASH_NONE, 0x00 },
        ATELEM_ON | ATELEM_SFX_HARD,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 28, 8, 0, { 0, 0, 0 } },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F(scale, 1, ICHAIN_CONTINUE),
    ICHAIN_F32(lockOnArrowOffset, 16, ICHAIN_STOP),
};

static void* D_80B43F64[] = { gFloorTileEnemyTopTex, gFloorTileEnemyBottomTex };

void EnYukabyun_Init(Actor* thisx, PlayState* play) {
    EnYukabyun* this = (EnYukabyun*)thisx;

    Actor_ProcessInitChain(&this->actor, sInitChain);
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 0.4f);
    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    this->actor.params++;
    this->unk_152 = 0;
    this->unk_150 = (u8)(this->actor.params) * 0xA + 0x14;
    this->actionfunc = func_80B43A94;
}

void EnYukabyun_Destroy(Actor* thisx, PlayState* play) {
    EnYukabyun* this = (EnYukabyun*)thisx;
    Collider_DestroyCylinder(play, &this->collider);
}

void func_80B43A94(EnYukabyun* this, PlayState* play) {
    if (!comboEnemyFlyingAttackInRange(&this->actor, play, 400.f))
        return;
    if (this->unk_150 != 0) {
        this->unk_150--;
    }
    if (this->unk_150 == 0) {
        this->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_IGNORE_QUAKE;
        this->actionfunc = func_80B43AD4;
    }
}

void func_80B43AD4(EnYukabyun* this, PlayState* play) {
    this->unk_150 += 0xA0;
    this->actor.shape.rot.y += this->unk_150;
    if (this->unk_150 >= 0x2000) {
        this->actor.world.rot.y = this->actor.yawTowardsPlayer;
        this->actor.speed = 10.0f;
        comboEnemyFlyingAttackAim(&this->actor, play, 1);
        this->actionfunc = func_80B43B6C;
    }
    Math_StepToF(&this->actor.world.pos.y, this->actor.home.pos.y + 30.0f, 1.0f);
    Actor_PlaySfx_Flagged(&this->actor, NA_SE_EN_YUKABYUN_FLY - SFX_FLAG);
}

void func_80B43B6C(EnYukabyun* this, PlayState* play) {
    comboEnemyFlyingAttackAim(&this->actor, play, 0);
    this->actor.shape.rot.y += this->unk_150;
    if (this->actor.xzDistToPlayer > 5000.0f) {
        Actor_Kill(&this->actor);
        return;
    }
    Actor_PlaySfx_Flagged(&this->actor, NA_SE_EN_YUKABYUN_FLY - SFX_FLAG);
}

void EnYukabyun_Break(EnYukabyun* this, PlayState* play) {
    EffectSsHahen_SpawnBurst(play, &this->actor.world.pos, 8.0f, 0, 1300, 300, 15, 0x5f, 10, gFloorTileEnemyFragmentDL);
    Actor_Kill(&this->actor);
}

void EnYukabyun_Update(Actor* thisx, PlayState* play) {
    EnYukabyun* this = (EnYukabyun*)thisx;
    s32 pad;

    if (((this->collider.base.atFlags & AT_HIT) || (this->collider.base.acFlags & AC_HIT) ||
         ((this->collider.base.ocFlags1 & OC1_HIT) && !(this->collider.base.oc->id == ACTOR_EN_YUKABYUN))) ||
        ((this->actionfunc == func_80B43B6C) && (this->actor.bgCheckFlags & (BGCHECKFLAG_WALL | BGCHECKFLAG_GROUND | BGCHECKFLAG_CEILING)))) {
        this->collider.base.atFlags &= ~AT_HIT;
        this->collider.base.acFlags &= ~AC_HIT;
        this->collider.base.ocFlags1 &= ~OC1_HIT;
        this->actor.flags &= ~(ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE);
        SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 30, NA_SE_EN_OCTAROCK_ROCK);
        this->actionfunc = EnYukabyun_Break;
    }

    this->actionfunc(this, play);
    Actor_MoveXZGravity(&this->actor);

    if (!(this->actionfunc == func_80B43A94 || this->actionfunc == EnYukabyun_Break)) {
        Actor_UpdateBgCheckInfo(play, &this->actor, 5.0f, 20.0f, 8.0f, UPDBGCHECKINFO_FLAG_WALL | UPDBGCHECKINFO_FLAG_FLOOR | UPDBGCHECKINFO_FLAG_CEILING);
        if (this->actionfunc == func_80B43B6C &&
            (this->actor.bgCheckFlags & (BGCHECKFLAG_WALL | BGCHECKFLAG_GROUND | BGCHECKFLAG_CEILING))) {
            this->actor.flags &= ~(ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE);
            SfxSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 30, NA_SE_EN_OCTAROCK_ROCK);
            EnYukabyun_Break(this, play);
            return;
        }
        Collider_UpdateCylinder(&this->actor, &this->collider);

        this->actor.flags |= ACTOR_FLAG_SFX_FOR_PLAYER_BODY_HIT;

        CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
    }
    Actor_SetFocus(&this->actor, 4.0f);
}

void EnYukabyun_Draw(Actor* thisx, PlayState* play) {
    EnYukabyun* this = (EnYukabyun*)thisx;

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL_25Opa(play->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_80B43F64[this->unk_152]));
    MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);
    gSPDisplayList(POLY_OPA_DISP++, gFloorTileEnemyDL);

    CLOSE_DISPS();
}

OVL_INFO_NATIVE_ENEMY(0x6b, En_Yukabyun_Profile);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_YUKABYUN_INIT, EnYukabyun_Init);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_YUKABYUN_DESTROY, EnYukabyun_Destroy);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_YUKABYUN_UPDATE, EnYukabyun_Update);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_YUKABYUN_DRAW, EnYukabyun_Draw);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_YUKABYUN_PROFILE, En_Yukabyun_Profile);
static const u32 kNativePortOrigin[] = { 0x4e505431, 0, 0x6b, 0x5f, 0x160a633a };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_YUKABYUN_ORIGIN, kNativePortOrigin);
