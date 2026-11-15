#include "oot_compat.h"
#include "types/en_skjneedle.h"
#undef ACTOR_EN_SKJNEEDLE
#define ACTOR_EN_SKJNEEDLE 0x0116
#undef OBJECT_SKJ
#define OBJECT_SKJ 0x110a
#define gSkullKidNeedleDL (*((Gfx (*)[1])0x06000eb0))

#undef ActorShape_Init
#define ActorShape_Init ((void (*)(ActorShape* shape, f32 yOffset, ActorShadowFunc shadowDraw, f32 shadowScale))0x800b3ba4)
#undef Actor_Kill
#define Actor_Kill FOREIGN_ENEMY_API_KILL
#undef Actor_MoveXZGravity
#define Actor_MoveXZGravity ((void (*)(Actor* actor))0x800b6a88)
#undef Actor_ProcessInitChain
#define Actor_ProcessInitChain ((void (*)(Actor* actor, InitChainEntry* ichain))0x800ffadc)
#undef Actor_SetScale
#define Actor_SetScale ((void (*)(Actor* actor, f32 scale))0x800b67e0)
#undef Actor_UpdateBgCheckInfo
#define Actor_UpdateBgCheckInfo ((void (*)(PlayState* play, Actor* actor, f32 wallCheckHeight, f32 wallCheckRadius, f32 ceilingCheckHeight, s32 flags))0x800b78b8)
#undef Collider_DestroyCylinder
#define Collider_DestroyCylinder FOREIGN_ENEMY_API_CYLINDER_DESTROY
#undef Collider_InitCylinder
#define Collider_InitCylinder ((s32 (*)(PlayState* play, ColliderCylinder* cyl))0x800e119c)
#undef Collider_SetCylinderType1
#define Collider_SetCylinderType1 ((s32 (*)(PlayState* play, ColliderCylinder* dest, Actor* actor, ColliderCylinderInitType1* src))0x800e12a4)
#undef Collider_UpdateCylinder
#define Collider_UpdateCylinder ((void (*)(Actor* actor, ColliderCylinder* cyl))0x800e7df8)
#undef CollisionCheck_SetAT
#define CollisionCheck_SetAT FOREIGN_ENEMY_API_COLLISION_AT
#undef CollisionCheck_SetOC
#define CollisionCheck_SetOC FOREIGN_ENEMY_API_COLLISION_OC
#undef Gfx_SetupDL_25Opa
#define Gfx_SetupDL_25Opa ((void (*)(GraphicsContext* gfxCtx))0x8012c28c)
#undef Matrix_Finalize
#define Matrix_Finalize(gfx,...) (((Mtx* (*)(GraphicsContext*))0x80181a40)(gfx))

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_HOOKSHOT_PULLS_ACTOR)

void EnSkjneedle_Init(Actor* thisx, PlayState* play);
void EnSkjneedle_Destroy(Actor* thisx, PlayState* play);
void EnSkjneedle_Update(Actor* thisx, PlayState* play2);
void EnSkjneedle_Draw(Actor* thisx, PlayState* play);

s32 EnSkjNeedle_CollisionCheck(EnSkjneedle* this);

ActorProfile En_Skjneedle_Profile = {
    0xd16,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_SKJ,
    sizeof(EnSkjneedle),
    EnSkjneedle_Init,
    EnSkjneedle_Destroy,
    EnSkjneedle_Update,
    EnSkjneedle_Draw,
};

static ColliderCylinderInitType1 sCylinderInit = {
    {
        COL_MATERIAL_HIT1,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0xFFCFFFFF, HIT_SPECIAL_EFFECT_NONE, 0x08 },
        { 0xFFCFFFFF, HIT_BACKLASH_NONE, 0x00 },
        ATELEM_ON | ATELEM_SFX_NORMAL,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 10, 4, -2, { 0, 0, 0 } },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_U8(attentionRangeType, ATTENTION_RANGE_2, ICHAIN_CONTINUE),
    ICHAIN_F32(lockOnArrowOffset, 30, ICHAIN_STOP),
};

void EnSkjneedle_Init(Actor* thisx, PlayState* play) {
    EnSkjneedle* this = (EnSkjneedle*)thisx;

    Actor_ProcessInitChain(&this->actor, sInitChain);
    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinderType1(play, &this->collider, &this->actor, &sCylinderInit);
    ActorShape_Init(&this->actor.shape, 0, NULL, 0.0f);
    thisx->flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    Actor_SetScale(&this->actor, 0.01f);
}

void EnSkjneedle_Destroy(Actor* thisx, PlayState* play) {
    EnSkjneedle* this = (EnSkjneedle*)thisx;

    Collider_DestroyCylinder(play, &this->collider);
}

s32 EnSkjNeedle_CollisionCheck(EnSkjneedle* this) {
    if (this->collider.base.atFlags & AT_HIT) {
        this->collider.base.acFlags &= ~AC_HIT;
        return 1;
    }
    return 0;
}

void EnSkjneedle_Update(Actor* thisx, PlayState* play2) {
    EnSkjneedle* this = (EnSkjneedle*)thisx;
    PlayState* play = play2;

    this->unusedTimer1++;
    if (this->killTimer != 0) {
        this->killTimer--;
    }
    if (EnSkjNeedle_CollisionCheck(this) || this->killTimer == 0) {
        Actor_Kill(&this->actor);
    } else {
        Actor_SetScale(&this->actor, 0.01f);
        Actor_MoveXZGravity(&this->actor);
        Actor_UpdateBgCheckInfo(play, &this->actor, 20.0f, 20.0f, 20.0f,
                                UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_2);
        if (this->actor.bgCheckFlags & (BGCHECKFLAG_GROUND | BGCHECKFLAG_WALL | BGCHECKFLAG_CEILING)) {
            Actor_Kill(&this->actor);
            return;
        }
        Collider_UpdateCylinder(&this->actor, &this->collider);
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
    }
}

void EnSkjneedle_Draw(Actor* thisx, PlayState* play) {

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL_25Opa(play->state.gfxCtx);
    MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);
    gSPDisplayList(POLY_OPA_DISP++, gSkullKidNeedleDL);

    CLOSE_DISPS();
}

OVL_INFO_NATIVE_ENEMY(0x116, En_Skjneedle_Profile);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_SKJNEEDLE_INIT, EnSkjneedle_Init);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_SKJNEEDLE_DESTROY, EnSkjneedle_Destroy);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_SKJNEEDLE_UPDATE, EnSkjneedle_Update);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_SKJNEEDLE_DRAW, EnSkjneedle_Draw);
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_SKJNEEDLE_PROFILE, En_Skjneedle_Profile);
static const u32 kNativePortOrigin[] = { 0x4e505431, 0, 0x116, 0x10a, 0x5a7c9433 };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_OOT_EN_SKJNEEDLE_ORIGIN, kNativePortOrigin);
