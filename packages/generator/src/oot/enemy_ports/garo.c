#include <combo/enemy_garo.h>
#include <combo/enemy_port.h>

enum {
    ACTOR_DRAW_DMGEFF_FIRE = 0,
    ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX = 10,
    ACTOR_DRAW_DMGEFF_FROZEN_SFX = 11,
    ACTOR_DRAW_DMGEFF_LIGHT_ORBS = 20,
    ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_SMALL = 30
};
#define CLEAR_TAG_PARAMS(x) (x)
#define CLEAR_TAG_LARGE_LIGHT_RAYS 4
#define Effect_Destroy Effect_Delete
#undef AudioSfx_SetChannelIO
#define AudioSfx_SetChannelIO comboEnemySfxChannelIo
#define Actor_DrawDamageEffects comboEnemyDrawDamageEffects
#define Actor_SpawnIceEffects comboEnemyIceShatter
void Effect_Delete(PlayState*, s32);
void comboEnemySfxChannelIo(Vec3f*, u16, u8);
void comboEnemyDrawDamageEffects(PlayState*, Actor*, Vec3f*, s16, f32, f32, f32, u8);
void comboEnemyIceShatter(PlayState*, Actor*, Vec3f*, s32, s32, f32, f32);

static void GaroInitCylinder(PlayState* p, ColliderCylinder* c, Actor* a, ColliderCylinderInit* i) {
    Collider_InitCylinder(p, c);
    Collider_SetCylinder(p, c, a, i);
}

static void GaroInitQuad(PlayState* p, ColliderQuad* c, Actor* a, ColliderQuadInit* i) {
    Collider_InitQuad(p, c);
    Collider_SetQuad(p, c, a, i);
}

#define Collider_InitAndSetCylinder GaroInitCylinder
#define Collider_InitAndSetQuad GaroInitQuad
#define UPDBGCHECKINFO_FLAG_1 UPDBGCHECKINFO_FLAG_MM_1
#define UPDBGCHECKINFO_FLAG_2 UPDBGCHECKINFO_FLAG_MM_2
#define UPDBGCHECKINFO_FLAG_4 UPDBGCHECKINFO_FLAG_MM_4
#define UPDBGCHECKINFO_FLAG_8 UPDBGCHECKINFO_FLAG_MM_8
#define UPDBGCHECKINFO_FLAG_10 UPDBGCHECKINFO_FLAG_MM_10

#define gGaroDashAttackAnim (*(AnimationHeader*)0x06003530)
#define gGaroSlashStartAnim (*(AnimationHeader*)0x0600378c)
#define gGaroSlashLoopAnim (*(AnimationHeader*)0x060038ac)
#define gGaroJumpBackAnim (*(AnimationHeader*)0x06003d5c)
#define gGaroDamagedAnim (*(AnimationHeader*)0x060044f8)
#define gGaroGuardAnim (*(AnimationHeader*)0x06004858)
#define gGaroFallDownAnim (*(AnimationHeader*)0x06005d5c)
#define gGaroKnockedBackAnim (*(AnimationHeader*)0x0600603c)
#define gGaroCowerAnim (*(AnimationHeader*)0x06007b04)
#define gGaroAppearAnim (*(AnimationHeader*)0x060081f4)
#define gGaroSkel (*(FlexSkeletonHeader*)0x0600aa00)
#define gGaroIdleAnim (*(AnimationHeader*)0x0600ae80)
#define gGaroBounceAnim (*(AnimationHeader*)0x0600b5f0)
#undef NA_SE_EN_ANSATSUSYA_ENTRY
#define NA_SE_EN_ANSATSUSYA_ENTRY 0x3812
#undef NA_SE_EN_COMMON_FREEZE
#define NA_SE_EN_COMMON_FREEZE 0x389E
#undef NA_SE_EN_ANSATSUSYA_MOVING
#define NA_SE_EN_ANSATSUSYA_MOVING 0x39BD
#undef NA_SE_EN_ANSATSUSYA_DASH_2
#define NA_SE_EN_ANSATSUSYA_DASH_2 0x39BE
#undef NA_SE_EN_ANSATSUSYA_CRYING
#define NA_SE_EN_ANSATSUSYA_CRYING 0x39C0
#undef NA_SE_EN_ANSATSUSYA_SKIP
#define NA_SE_EN_ANSATSUSYA_SKIP 0x39C1
#undef NA_SE_EN_ANSATSUSYA_DAMAGE
#define NA_SE_EN_ANSATSUSYA_DAMAGE 0x39C5
#undef NA_SE_EN_ANSATSUSYA_DEAD
#define NA_SE_EN_ANSATSUSYA_DEAD 0x39C6
#undef NA_SE_EN_ANSATSUSYA_SWORD
#define NA_SE_EN_ANSATSUSYA_SWORD 0x39C7
#undef NA_SE_IT_SHIELD_BOUND
#define NA_SE_IT_SHIELD_BOUND 0x1806
#undef NA_SE_IT_SWORD_SWING_HARD
#define NA_SE_IT_SWORD_SWING_HARD 0x1818
void EnJso_Init(Actor* thisx, PlayState* play);
void EnJso_Destroy(Actor* thisx, PlayState* play);
void EnJso_ChangeAnim(EnJso* this, s32 animIndex);
void EnJso_SetupReappear(EnJso* this, PlayState* play);
void EnJso_Reappear(EnJso* this, PlayState* play);
void EnJso_SetupCirclePlayer(EnJso* this, PlayState* play);
void EnJso_CirclePlayer(EnJso* this, PlayState* play);
void EnJso_SetupGuard(EnJso* this);
void EnJso_Guard(EnJso* this, PlayState* play);
void EnJso_SetupSpinBeforeAttack(EnJso* this);
void EnJso_SpinBeforeAttack(EnJso* this, PlayState* play);
void EnJso_SetupDashAttack(EnJso* this);
void EnJso_DashAttack(EnJso* this, PlayState* play);
void EnJso_SetupSlash(EnJso* this, PlayState* play);
void EnJso_Slash(EnJso* this, PlayState* play);
void EnJso_SetupWaitAfterSlash(EnJso* this);
void EnJso_WaitAfterSlash(EnJso* this, PlayState* play);
void EnJso_SetupKnockedBack(EnJso* this);
void EnJso_KnockedBack(EnJso* this, PlayState* play);
void EnJso_SetupCower(EnJso* this);
void EnJso_Cower(EnJso* this, PlayState* play);
void EnJso_SetupStunned(EnJso* this);
void EnJso_Stunned(EnJso* this, PlayState* play);
void EnJso_SetupDamaged(EnJso* this, PlayState* play);
void EnJso_Damaged(EnJso* this, PlayState* play);
void EnJso_SetupJumpBack(EnJso* this);
void EnJso_JumpBack(EnJso* this, PlayState* play);
void EnJso_SetupDead(EnJso* this, PlayState* play);
void EnJso_Dead(EnJso* this, PlayState* play);
static void GaroKneelBeforeBurst(EnJso*, PlayState*);
void comboEnemyDeathFlamePreset(PlayState*, Vec3f*, Vec3f*, Vec3f*, s16, s16, s32);
void EnJso_UpdateDamage(EnJso* this, PlayState* play);
void EnJso_Update(Actor* thisx, PlayState* play);
s32 EnJso_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx);
void EnJso_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx);
Gfx* EnJso_SetAfterimageRenderMode(GraphicsContext* gfxCtx);
void EnJso_Draw(Actor* thisx, PlayState* play);
typedef enum EnJsoAction {
    EN_JSO_ACTION_INTRO_CUTSCENE,
    EN_JSO_ACTION_REAPPEAR,
    EN_JSO_ACTION_CIRCLE_PLAYER,
    EN_JSO_ACTION_GUARD,
    EN_JSO_ACTION_SPIN_BEFORE_ATTACK,
    EN_JSO_ACTION_DASH_ATTACK,
    EN_JSO_ACTION_SLASH,
    EN_JSO_ACTION_WAIT_AFTER_SLASH,
    EN_JSO_ACTION_STUNNED,
    EN_JSO_ACTION_KNOCKED_BACK,
    EN_JSO_ACTION_COWER,
    EN_JSO_ACTION_DAMAGED,
    EN_JSO_ACTION_JUMP_BACK,
    EN_JSO_ACTION_DEAD,
    EN_JSO_ACTION_FALL_DOWN_AND_TALK,
    EN_JSO_ACTION_UNK_15
} EnJsoAction;

typedef enum EnJsoIntroType {
    EN_JSO_INTRO_SPIN_UP_FROM_GROUND,
    EN_JSO_INTRO_JUMP_OUT_FROM_GROUND,
    EN_JSO_INTRO_LAND_FROM_ABOVE,
    EN_JSO_INTRO_SCALE_UP
} EnJsoIntroType;

typedef enum EnJsoIntroCsState {
    EN_JSO_INTRO_CS_STATE_DONE_OR_STARTED,

    EN_JSO_INTRO_CS_STATE_WAITING_FOR_TEXTBOX_TO_APPEAR,

    EN_JSO_INTRO_CS_STATE_WAITING_FOR_TEXTBOX_TO_CONTINUE,

    EN_JSO_INTRO_CS_STATE_RAISE_ARM_AND_DRAW_RIGHT_SWORD,

    EN_JSO_INTRO_CS_STATE_DRAW_LEFT_SWORD,

    EN_JSO_INTRO_CS_STATE_ENDING
} EnJsoIntroCsState;

typedef enum EnJsoSwordState {
    EN_JSO_SWORD_STATE_BOTH_DRAWN,
    EN_JSO_SWORD_STATE_KNOCKED_OUT_OF_HANDS,
    EN_JSO_SWORD_STATE_RIGHT_DRAWN,
    EN_JSO_SWORD_STATE_LEFT_DRAWN,
    EN_JSO_SWORD_STATE_NONE_DRAWN = 5
} EnJsoSwordState;

static s32 sIsAttacking = false;

static s32 sIsPlayerLockedOn = false;

typedef enum EnJsoDamageEffect {
    EN_JSO_DMGEFF_IMMUNE,
    EN_JSO_DMGEFF_STUN,
    EN_JSO_DMGEFF_FIRE,
    EN_JSO_DMGEFF_FREEZE,
    EN_JSO_DMGEFF_LIGHT_ORB,
    EN_JSO_DMGEFF_ELECTRIC_STUN,
    EN_JSO_DMGEFF_NONE = 0xF
} EnJsoDamageEffect;

static void GaroMatrixOrigin(Vec3f* destination) {
    Vec3f origin = { 0, 0, 0 };
    Matrix_MultVec3f(&origin, destination);
}
#define Matrix_MultZero GaroMatrixOrigin
static Gfx gActorSetupOpaDL[] = { gsSPEndDisplayList() };
static void GaroSwordFragment(EnJso* this, s32 limb, Gfx* display) {
    int index = limb == GARO_LIMB_LEFT_SWORD ? 0 : 1;
    Vec3f origin = { 0, 0, 0 };
    Matrix_MultVec3f(&origin, &this->swords[index].pos);
    this->swords[index].velocity = (Vec3f){ index ? 3.f : -3.f, 8.f, 2.f };
    this->swords[index].rot = this->actor.shape.rot;
    this->swords[index].display = display;
    this->swords[index].life = 40;
}

static void GaroUpdateSwords(EnJso* this) {
    for (int i = 0; i < 2; i++)
        if (this->swords[i].life > 0) {
            this->swords[i].life--;
            this->swords[i].pos.x += this->swords[i].velocity.x;
            this->swords[i].pos.y += this->swords[i].velocity.y;
            this->swords[i].pos.z += this->swords[i].velocity.z;
            this->swords[i].velocity.y -= .7f;
            this->swords[i].rot.x += 0x900;
            this->swords[i].rot.z += 0x700;
            if (this->swords[i].pos.y < this->actor.floorHeight)
                this->swords[i].life = 0;
        }
}

static void GaroDrawSwords(EnJso* this, PlayState* play) {
    OPEN_DISPS(play->state.gfxCtx);
    for (int i = 0; i < 2; i++)
        if (this->swords[i].life > 0 && this->swords[i].display) {
            Matrix_Translate(this->swords[i].pos.x, this->swords[i].pos.y, this->swords[i].pos.z, MTXMODE_NEW);
            Matrix_RotateYS(this->swords[i].rot.y, MTXMODE_APPLY);
            Matrix_RotateXS(this->swords[i].rot.x, MTXMODE_APPLY);
            Matrix_RotateZS(this->swords[i].rot.z, MTXMODE_APPLY);
            Matrix_Scale(this->scale, this->scale, this->scale, MTXMODE_APPLY);
            gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_OPA_DISP++, this->swords[i].display);
        }
    CLOSE_DISPS();
}

static DamageTable sDamageTable = { {
    DMG_ENTRY(0, EN_JSO_DMGEFF_STUN),   DMG_ENTRY(1, EN_JSO_DMGEFF_NONE),          DMG_ENTRY(0, EN_JSO_DMGEFF_IMMUNE),
    DMG_ENTRY(1, EN_JSO_DMGEFF_NONE),   DMG_ENTRY(1, EN_JSO_DMGEFF_NONE),          DMG_ENTRY(1, EN_JSO_DMGEFF_NONE),
    DMG_ENTRY(0, EN_JSO_DMGEFF_IMMUNE), DMG_ENTRY(0, EN_JSO_DMGEFF_STUN),          DMG_ENTRY(1, EN_JSO_DMGEFF_NONE),
    DMG_ENTRY(1, EN_JSO_DMGEFF_NONE),   DMG_ENTRY(1, EN_JSO_DMGEFF_NONE),          DMG_ENTRY(2, EN_JSO_DMGEFF_FIRE),
    DMG_ENTRY(2, EN_JSO_DMGEFF_FREEZE), DMG_ENTRY(2, EN_JSO_DMGEFF_LIGHT_ORB),     DMG_ENTRY(1, EN_JSO_DMGEFF_NONE),
    DMG_ENTRY(0, EN_JSO_DMGEFF_STUN),   DMG_ENTRY(1, EN_JSO_DMGEFF_NONE),          DMG_ENTRY(2, EN_JSO_DMGEFF_NONE),
    DMG_ENTRY(0, EN_JSO_DMGEFF_STUN),   DMG_ENTRY(0, EN_JSO_DMGEFF_ELECTRIC_STUN), DMG_ENTRY(0, EN_JSO_DMGEFF_IMMUNE),
    DMG_ENTRY(0, EN_JSO_DMGEFF_IMMUNE), DMG_ENTRY(1, EN_JSO_DMGEFF_NONE),          DMG_ENTRY(1, EN_JSO_DMGEFF_NONE),
    DMG_ENTRY(1, EN_JSO_DMGEFF_NONE),   DMG_ENTRY(0, EN_JSO_DMGEFF_IMMUNE),        DMG_ENTRY(0, EN_JSO_DMGEFF_IMMUNE),
    DMG_ENTRY(0, EN_JSO_DMGEFF_IMMUNE), DMG_ENTRY(0, EN_JSO_DMGEFF_IMMUNE),        DMG_ENTRY(0, EN_JSO_DMGEFF_IMMUNE),
    DMG_ENTRY(0, EN_JSO_DMGEFF_IMMUNE), DMG_ENTRY(1, EN_JSO_DMGEFF_NONE),
} };

static ColliderCylinderInit sCylinderInit = {
    {
        COL_MATERIAL_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0xF7CFFFFF, 0x08, 0x04 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        ATELEM_ON | ATELEM_SFX_NORMAL,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 22, 55, 0, { 0, 0, 0 } },
};

static ColliderQuadInit sQuadInit = {
    {
        COL_MATERIAL_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_NONE,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_QUAD,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0xF7CFFFFF, 0x04, 0x08 },
        { 0x00000000, 0x00, 0x00 },
        ATELEM_ON | ATELEM_SFX_NORMAL | ATELEM_UNK7,
        ACELEM_NONE,
        OCELEM_NONE,
    },
    { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
};

typedef enum EnJsoAnimation {
    EN_JSO_ANIM_APPEAR,
    EN_JSO_ANIM_IDLE,
    EN_JSO_ANIM_BOUNCE,
    EN_JSO_ANIM_GUARD,
    EN_JSO_ANIM_DASH_ATTACK,
    EN_JSO_ANIM_SLASH_START,
    EN_JSO_ANIM_SLASH_LOOP,
    EN_JSO_ANIM_KNOCKED_BACK,
    EN_JSO_ANIM_COWER,
    EN_JSO_ANIM_DAMAGED,
    EN_JSO_ANIM_JUMP_BACK,
    EN_JSO_ANIM_FALL_DOWN,
    EN_JSO_ANIM_MAX
} EnJsoAnimation;

static AnimationHeader* sAnimations[EN_JSO_ANIM_MAX] = {
    &gGaroAppearAnim,     &gGaroIdleAnim,       &gGaroBounceAnim,    &gGaroGuardAnim,
    &gGaroDashAttackAnim, &gGaroSlashStartAnim, &gGaroSlashLoopAnim, &gGaroKnockedBackAnim,
    &gGaroCowerAnim,      &gGaroDamagedAnim,    &gGaroJumpBackAnim,  &gGaroFallDownAnim,
};

static u8 sAnimationModes[EN_JSO_ANIM_MAX] = {
    ANIMMODE_ONCE, ANIMMODE_LOOP, ANIMMODE_LOOP, ANIMMODE_ONCE, ANIMMODE_ONCE, ANIMMODE_ONCE,
    ANIMMODE_LOOP, ANIMMODE_ONCE, ANIMMODE_LOOP, ANIMMODE_LOOP, ANIMMODE_ONCE, ANIMMODE_ONCE,
};

void EnJso_Init(Actor* thisx, PlayState* play) {
    EnJso* this = (EnJso*)thisx;
    EffectBlureInit1 rightSwordBlureInit;
    EffectBlureInit1 leftSwordBlureInit;

    this->actor.hintId = 0xff;
    this->actor.attentionRangeType = ATTENTION_RANGE_5;
    this->actor.colChkInfo.mass = 80;
    this->actor.colChkInfo.health = 3;
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 0.0f);
    this->actor.colChkInfo.damageTable = &sDamageTable;
    this->actor.shape.shadowScale = 0.0f;
    SkelAnime_InitFlex(play, &this->skelAnime, &gGaroSkel, &gGaroAppearAnim, this->jointTable, this->morphTable,
                       GARO_LIMB_MAX);
    this->actor.focus.pos = this->actor.world.pos;
    Collider_InitAndSetCylinder(play, &this->bodyCollider, &this->actor, &sCylinderInit);
    Collider_InitAndSetQuad(play, &this->rightSwordCollider, &this->actor, &sQuadInit);
    Collider_InitAndSetQuad(play, &this->leftSwordCollider, &this->actor, &sQuadInit);

    rightSwordBlureInit.p1StartColor[0] = leftSwordBlureInit.p1StartColor[0] = rightSwordBlureInit.p1StartColor[1] =
        leftSwordBlureInit.p1StartColor[1] = rightSwordBlureInit.p1StartColor[2] = leftSwordBlureInit.p1StartColor[2] =
            rightSwordBlureInit.p1StartColor[3] = leftSwordBlureInit.p1StartColor[3] =
                rightSwordBlureInit.p2StartColor[0] = leftSwordBlureInit.p2StartColor[0] =
                    rightSwordBlureInit.p2StartColor[1] = leftSwordBlureInit.p2StartColor[1] =
                        rightSwordBlureInit.p2StartColor[2] = leftSwordBlureInit.p2StartColor[2] =
                            rightSwordBlureInit.p1EndColor[0] = leftSwordBlureInit.p1EndColor[0] =
                                rightSwordBlureInit.p1EndColor[1] = leftSwordBlureInit.p1EndColor[1] =
                                    rightSwordBlureInit.p1EndColor[2] = leftSwordBlureInit.p1EndColor[2] =
                                        rightSwordBlureInit.p2EndColor[0] = leftSwordBlureInit.p2EndColor[0] =
                                            rightSwordBlureInit.p2EndColor[1] = leftSwordBlureInit.p2EndColor[1] =
                                                rightSwordBlureInit.p2EndColor[2] = leftSwordBlureInit.p2EndColor[2] =
                                                    255;
    leftSwordBlureInit.p2StartColor[3] = rightSwordBlureInit.p2StartColor[3] = 64;
    rightSwordBlureInit.p1EndColor[3] = leftSwordBlureInit.p1EndColor[3] = rightSwordBlureInit.p2EndColor[3] =
        leftSwordBlureInit.p2EndColor[3] = 0;
    rightSwordBlureInit.elemDuration = leftSwordBlureInit.elemDuration = 8;
    rightSwordBlureInit.unkFlag = leftSwordBlureInit.unkFlag = 0;
    rightSwordBlureInit.calcMode = leftSwordBlureInit.calcMode = 2;

    Effect_Add(play, &this->rightSwordBlureIndex, EFFECT_BLURE1, 0, 0, &rightSwordBlureInit);
    Effect_Add(play, &this->leftSwordBlureIndex, EFFECT_BLURE1, 0, 0, &leftSwordBlureInit);

    this->actor.gravity = -3.0f;
    this->scale = 0.035f;
    this->actor.flags &= ~ACTOR_FLAG_LOCK_ON_DISABLED;
    this->actor.draw = EnJso_Draw;
    this->swordState = EN_JSO_SWORD_STATE_BOTH_DRAWN;
    this->actor.shape.shadowScale = 16.f;
    this->actor.shape.yOffset = 970.0f;
    this->attackTimer = 10;
    Actor_SetScale(&this->actor, this->scale);
    EnJso_SetupCirclePlayer(this, play);
}

void EnJso_Destroy(Actor* thisx, PlayState* play) {
    EnJso* this = (EnJso*)thisx;

    Collider_DestroyCylinder(play, &this->bodyCollider);
    Collider_DestroyQuad(play, &this->rightSwordCollider);
    Collider_DestroyQuad(play, &this->leftSwordCollider);

    if (this->isAttacking)
        sIsAttacking = false;
    if (this->isPlayerLockedOn)
        sIsPlayerLockedOn = false;
    Effect_Destroy(play, this->rightSwordBlureIndex);
    Effect_Destroy(play, this->leftSwordBlureIndex);
}

void EnJso_ChangeAnim(EnJso* this, s32 animIndex) {
    f32 morphFrames = -4.0f;

    this->animEndFrame = Animation_GetLastFrame(sAnimations[animIndex]);
    if ((animIndex == EN_JSO_ANIM_GUARD) || (animIndex == EN_JSO_ANIM_DAMAGED)) {
        morphFrames = 0.0f;
    }

    Animation_Change(&this->skelAnime, sAnimations[animIndex], 1.0f, 0.0f, this->animEndFrame,
                     sAnimationModes[animIndex], morphFrames);
}

void EnJso_SetupReappear(EnJso* this, PlayState* play) {
    EnJso_ChangeAnim(this, EN_JSO_ANIM_APPEAR);
    this->actor.speed = 0.0f;
    this->actor.velocity.y = 0.0f;

    if (this->isPlayerLockedOn) {
        this->isPlayerLockedOn = false;
        sIsPlayerLockedOn = false;
    }

    if (this->isAttacking) {
        this->isAttacking = false;
        sIsAttacking = false;
    }

    if (this->swordState != EN_JSO_SWORD_STATE_BOTH_DRAWN) {
        this->swordState = EN_JSO_SWORD_STATE_BOTH_DRAWN;
    }

    this->actor.shape.rot.x = this->actor.world.rot.x = 0;
    this->actor.shape.rot.y = this->actor.world.rot.y = this->actor.yawTowardsPlayer;
    Math_Vec3f_Copy(&this->actor.world.pos, &this->actor.home.pos);
    this->actor.floorHeight = this->actor.home.pos.y;
    Actor_SpawnFloorDustRing(play, &this->actor, &this->actor.world.pos, this->actor.shape.shadowScale, 1, 8.0f, 500,
                             10, true);
    this->actor.flags |= ACTOR_FLAG_LOCK_ON_DISABLED;
    Actor_PlaySfx(&this->actor, NA_SE_EN_ANSATSUSYA_ENTRY);
    this->afterimageCount = 0;
    this->action = EN_JSO_ACTION_REAPPEAR;
    this->actor.gravity = 0.0f;
    this->actionFunc = EnJso_Reappear;
}

void EnJso_Reappear(EnJso* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;

    if (this->actor.colChkInfo.health <= 0) {
        Player* player = GET_PLAYER(play);

        player->actor.freezeTimer = 3;
    }

    Math_ApproachF(&this->actor.shape.shadowScale, 16.0f, 0.4f, 4.0f);
    Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 0xA, 0xBB8, 0x14);

    if ((play->gameplayFrames % 8) == 0) {
        Actor_SpawnFloorDustRing(play, &this->actor, &this->actor.world.pos, this->actor.shape.shadowScale, 1, 8.0f,
                                 500, 10, true);
    }

    if (curFrame >= this->animEndFrame) {
        this->actor.gravity = -3.0f;
        if (this->actor.colChkInfo.health > 0) {
            this->actor.flags &= ~ACTOR_FLAG_LOCK_ON_DISABLED;
            EnJso_SetupCirclePlayer(this, play);
        } else {
            EnJso_SetupDead(this, play);
        }
    }
}

void EnJso_SetupCirclePlayer(EnJso* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    EnJso_ChangeAnim(this, EN_JSO_ANIM_BOUNCE);
    this->circlingAngularVelocity = 0x258;
    this->action = EN_JSO_ACTION_CIRCLE_PLAYER;
    this->circlingAngle = player->actor.shape.rot.y;
    this->actionFunc = EnJso_CirclePlayer;
}

void EnJso_CirclePlayer(EnJso* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;
    Player* player = GET_PLAYER(play);
    Vec3f targetPos;

    Actor_PlaySfx(&this->actor, NA_SE_EN_ANSATSUSYA_MOVING - SFX_FLAG);

    if ((this->action != EN_JSO_ACTION_REAPPEAR) && (this->action != EN_JSO_ACTION_INTRO_CUTSCENE) &&
        (fabsf(player->actor.world.pos.y - this->actor.world.pos.y) > 60.0f)) {
        EnJso_SetupReappear(this, play);
        return;
    }

    if (curFrame < this->animEndFrame) {
        SkelAnime_Update(&this->skelAnime);
    } else if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
        SkelAnime_Update(&this->skelAnime);
    }

    if (Animation_OnFrame(&this->skelAnime, 6.0f)) {
        this->actor.velocity.y = 10.0f;
        if ((play->gameplayFrames % 2) == 0) {
            Actor_PlaySfx(&this->actor, NA_SE_EN_ANSATSUSYA_CRYING);
        }
    }

    if (Animation_OnFrame(&this->skelAnime, 12.0f)) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_ANSATSUSYA_SKIP);
        this->actor.speed = 0.0f;
        if (Rand_ZeroFloat(1.0f) < 0.5f) {
            this->circlingAngularVelocity = -this->circlingAngularVelocity;
        }
    }

    if ((this->attackTimer == 0) && !sIsAttacking && (!sIsPlayerLockedOn || this->isPlayerLockedOn)) {
        sIsAttacking = true;
        this->isAttacking = true;
        this->action = EN_JSO_ACTION_SPIN_BEFORE_ATTACK;
        this->actor.speed = 0.0f;
        EnJso_SetupSpinBeforeAttack(this);
        return;
    }

    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 0xA, 0xFA0, 0x14);
    Math_ApproachF(&this->actor.speed, 5.0f, 0.3f, 2.0f);
    this->circlingAngle += this->circlingAngularVelocity;
    targetPos.x = (Math_SinS(this->circlingAngle) * 200.0f) + player->actor.world.pos.x;
    targetPos.y = this->actor.world.pos.y;
    targetPos.z = (Math_CosS(this->circlingAngle) * 200.0f) + player->actor.world.pos.z;
    Math_SmoothStepToS(&this->actor.world.rot.y, Math_Vec3f_Yaw(&this->actor.world.pos, &targetPos), 0xA, 0xFA0, 0x14);
}

void EnJso_SetupGuard(EnJso* this) {
    EnJso_ChangeAnim(this, EN_JSO_ANIM_GUARD);
    this->action = EN_JSO_ACTION_GUARD;
    this->actor.world.rot.y = this->actor.yawTowardsPlayer;
    Actor_PlaySfx(&this->actor, NA_SE_IT_SHIELD_BOUND);
    this->bodyCollider.base.acFlags |= AC_HARD;
    this->actionFunc = EnJso_Guard;
}

void EnJso_Guard(EnJso* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;

    if (curFrame >= this->animEndFrame) {
        this->isGuarding = false;

        if (this->isAttacking) {
            this->isAttacking = false;
            sIsAttacking = false;
        }

        EnJso_SetupCirclePlayer(this, play);
    }
}

void EnJso_SetupSpinBeforeAttack(EnJso* this) {
    EnJso_ChangeAnim(this, EN_JSO_ANIM_DASH_ATTACK);
    this->actor.world.rot.y = -this->actor.yawTowardsPlayer;
    this->actor.speed = 10.0f;
    this->actor.velocity.y = 20.0f;
    this->actionFunc = EnJso_SpinBeforeAttack;
}

void EnJso_SpinBeforeAttack(EnJso* this, PlayState* play) {
    this->actor.world.rot.x += 0x1770;
    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 0xA, 0xFA0, 0x14);

    if (this->actor.velocity.y > 0.0f) {
        return;
    }

    if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
        this->actor.world.rot.x = 0;
        this->actor.velocity.y = 0.0f;
        this->actor.speed = 0.0f;
        this->actor.world.rot.y = this->actor.shape.rot.y = this->actor.yawTowardsPlayer;
        EnJso_SetupDashAttack(this);
    }
}

void EnJso_SetupDashAttack(EnJso* this) {
    this->action = EN_JSO_ACTION_DASH_ATTACK;
    this->attackMovementTimer = 40;
    this->bodyCollider.base.colMaterial = COL_MATERIAL_HIT2;
    this->bodyCollider.base.acFlags &= ~AC_HARD;
    this->actor.speed = 15.0f;
    this->actor.velocity.y = 13.0f;
    Actor_PlaySfx(&this->actor, NA_SE_EN_ANSATSUSYA_ENTRY);
    Actor_PlaySfx(&this->actor, NA_SE_EN_ANSATSUSYA_DASH_2);
    this->actionFunc = EnJso_DashAttack;
}

void EnJso_DashAttack(EnJso* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;
    s16 yawDiff;
    s16 absYawDiff;
    Vec3f knockbackVelocity;

    if ((this->rightSwordCollider.base.atFlags & AT_BOUNCED) || (this->leftSwordCollider.base.atFlags & AT_BOUNCED)) {
        this->rightSwordCollider.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
        this->leftSwordCollider.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
        if (this->swordState == EN_JSO_SWORD_STATE_BOTH_DRAWN) {
            Matrix_RotateYS(this->actor.yawTowardsPlayer, MTXMODE_NEW);
            knockbackVelocity.x = 0.0f;
            knockbackVelocity.y = 0.0f;
            knockbackVelocity.z = -10.0f;
            Matrix_MultVec3f(&knockbackVelocity, &this->knockbackVelocity);
            this->swordState = EN_JSO_SWORD_STATE_KNOCKED_OUT_OF_HANDS;
            this->attackMovementTimer = 0;
            this->disableBlure = true;
            AudioSfx_SetChannelIO(&this->actor.projectedPos, NA_SE_EN_ANSATSUSYA_DASH_2, 0);
            EnJso_SetupKnockedBack(this);
            return;
        }
    }

    if ((this->actor.velocity.y < 0.0f) && (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
        this->actor.velocity.y = 13.0f;
    }

    if (curFrame < this->animEndFrame) {
        return;
    }

    yawDiff = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;
    absYawDiff = ABS_ALT(yawDiff);

    if ((this->attackMovementTimer == 0) || (this->actor.xzDistToPlayer < 100.0f) || (absYawDiff > 0x4300)) {
        AudioSfx_SetChannelIO(&this->actor.projectedPos, NA_SE_EN_ANSATSUSYA_DASH_2, 0);
        Math_ApproachZeroF(&this->actor.speed, 0.3f, 3.0f);
        EnJso_SetupSlash(this, play);
    }
}

void EnJso_SetupSlash(EnJso* this, PlayState* play) {
    EnJso_ChangeAnim(this, EN_JSO_ANIM_SLASH_START);
    this->action = EN_JSO_ACTION_SLASH;
    Actor_SpawnFloorDustRing(play, &this->actor, &this->actor.world.pos, this->actor.shape.shadowScale, 1, 8.0f, 500,
                             10, true);
    Math_ApproachZeroF(&this->actor.speed, 0.3f, 3.0f);
    this->slashHitSomething = false;
    Actor_PlaySfx(&this->actor, NA_SE_IT_SWORD_SWING_HARD);
    this->actionFunc = EnJso_Slash;
}

void EnJso_Slash(EnJso* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;

    Math_ApproachZeroF(&this->actor.speed, 0.5f, 5.0f);

    if ((play->gameplayFrames % 8) == 0) {
        Actor_SpawnFloorDustRing(play, &this->actor, &this->actor.world.pos, this->actor.shape.shadowScale, 1, 8.0f,
                                 500, 10, true);
    }

    if ((this->rightSwordCollider.base.atFlags & AT_HIT) || (this->leftSwordCollider.base.atFlags & AT_HIT)) {
        this->slashHitSomething = true;
        this->rightSwordCollider.base.atFlags &= ~AT_HIT;
        this->leftSwordCollider.base.atFlags &= ~AT_HIT;
    }

    if ((this->rightSwordCollider.base.atFlags & AT_BOUNCED) || (this->leftSwordCollider.base.atFlags & AT_BOUNCED)) {
        this->rightSwordCollider.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
        this->leftSwordCollider.base.atFlags &= ~(AT_HIT | AT_BOUNCED);

        if (this->swordState == EN_JSO_SWORD_STATE_BOTH_DRAWN) {
            Vec3f knockbackVelocity;

            Matrix_RotateYS(this->actor.yawTowardsPlayer, MTXMODE_NEW);
            knockbackVelocity.x = 0.0f;
            knockbackVelocity.y = 0.0f;
            knockbackVelocity.z = -10.0f;
            Matrix_MultVec3f(&knockbackVelocity, &this->knockbackVelocity);
            this->swordState = EN_JSO_SWORD_STATE_KNOCKED_OUT_OF_HANDS;
            this->disableBlure = true;
            EnJso_SetupKnockedBack(this);
            return;
        }
    }

    if (curFrame >= this->animEndFrame) {
        this->actor.speed = 0.0f;
        this->disableBlure = true;
        EnJso_SetupWaitAfterSlash(this);
    }
}

void EnJso_SetupWaitAfterSlash(EnJso* this) {
    if (this->slashHitSomething) {
        EnJso_ChangeAnim(this, EN_JSO_ANIM_SLASH_LOOP);
        this->timer = 20;
    } else {
        EnJso_ChangeAnim(this, EN_JSO_ANIM_SLASH_LOOP);
        this->timer = 40;
    }

    this->action = EN_JSO_ACTION_WAIT_AFTER_SLASH;
    this->actionFunc = EnJso_WaitAfterSlash;
}

void EnJso_WaitAfterSlash(EnJso* this, PlayState* play) {
    if (this->timer == 0) {
        this->attackTimer = Rand_S16Offset(30, 30);
        this->bodyCollider.base.colMaterial = COL_MATERIAL_NONE;
        this->bodyCollider.base.acFlags |= AC_HARD;
        this->slashHitSomething = false;
        sIsAttacking = false;
        this->isAttacking = false;
        EnJso_SetupCirclePlayer(this, play);
    }
}

void EnJso_SetupKnockedBack(EnJso* this) {
    EnJso_ChangeAnim(this, EN_JSO_ANIM_KNOCKED_BACK);
    this->bodyCollider.base.acFlags &= ~AC_HARD;
    this->timer = 30;
    this->action = EN_JSO_ACTION_KNOCKED_BACK;
    this->actor.speed = 0.0f;
    this->actionFunc = EnJso_KnockedBack;
}

void EnJso_KnockedBack(EnJso* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;

    if (curFrame >= this->animEndFrame) {
        EnJso_SetupCower(this);
    }
}

void EnJso_SetupCower(EnJso* this) {
    EnJso_ChangeAnim(this, EN_JSO_ANIM_COWER);
    this->action = EN_JSO_ACTION_COWER;
    this->timer = 30;
    this->actionFunc = EnJso_Cower;
}

void EnJso_Cower(EnJso* this, PlayState* play) {
    if (this->timer == 0) {
        EnJso_SetupJumpBack(this);
    }
}

void EnJso_SetupStunned(EnJso* this) {
    Vec3f knockbackVelocity;

    AudioSfx_SetChannelIO(&this->actor.projectedPos, NA_SE_EN_ANSATSUSYA_DASH_2, 0);
    EnJso_ChangeAnim(this, EN_JSO_ANIM_DAMAGED);

    this->timer = 30;
    this->actor.speed = 0.0f;

    if (((this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_SFX) ||
         (this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX)) &&
        (this->drawDmgEffAlpha == 0)) {
        this->drawDmgEffAlpha = 0;
        this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FIRE;
    }

    this->timer = 40;
    Matrix_RotateYS(this->actor.yawTowardsPlayer, MTXMODE_NEW);
    Matrix_MultVecZ(-10.0f, &knockbackVelocity);
    Math_Vec3f_Copy(&this->knockbackVelocity, &knockbackVelocity);
    this->action = EN_JSO_ACTION_STUNNED;
    this->bodyCollider.base.acFlags &= ~AC_HARD;
    this->actionFunc = EnJso_Stunned;
}

void EnJso_Stunned(EnJso* this, PlayState* play) {
    if (this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_SFX) {
        if ((this->drawDmgEffAlpha != 0) && (this->drawDmgEffAlpha < 60)) {
            this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX;
        }
    }

    if ((this->timer == 0) && (this->drawDmgEffAlpha == 0)) {
        if ((this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_SFX) ||
            (this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX)) {
            Actor_SpawnIceEffects(play, &this->actor, this->bodyPartsPos, EN_JSO_BODYPART_MAX, 2, 0.3f, 0.2f);
            this->drawDmgEffAlpha = 0;
            this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FIRE;
        }

        EnJso_SetupJumpBack(this);
    }
}

void EnJso_SetupDamaged(EnJso* this, PlayState* play) {
    Vec3f knockbackVelocity;

    AudioSfx_SetChannelIO(&this->actor.projectedPos, NA_SE_EN_ANSATSUSYA_DASH_2, 0);
    EnJso_ChangeAnim(this, EN_JSO_ANIM_DAMAGED);
    this->slashHitSomething = false;
    this->actor.velocity.y = 10.0f;
    this->actor.speed = 0.0f;
    Matrix_RotateYS(this->actor.yawTowardsPlayer, MTXMODE_NEW);
    Matrix_MultVecZ(-20.0f, &knockbackVelocity);
    Math_Vec3f_Copy(&this->knockbackVelocity, &knockbackVelocity);

    if (((this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_SFX) ||
         (this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX)) &&
        (this->drawDmgEffAlpha != 0)) {
        Actor_SpawnIceEffects(play, &this->actor, this->bodyPartsPos, EN_JSO_BODYPART_MAX, 2, 0.3f, 0.2f);
        this->drawDmgEffAlpha = 0;
        this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FIRE;
    }

    Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 8);
    Actor_PlaySfx(&this->actor, NA_SE_EN_ANSATSUSYA_DAMAGE);
    this->action = EN_JSO_ACTION_DAMAGED;
    this->actionFunc = EnJso_Damaged;
}

void EnJso_Damaged(EnJso* this, PlayState* play) {
    if (this->actor.velocity.y > 0.0f) {
        return;
    }

    if ((this->actor.colorFilterTimer == 0) && (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
        EnJso_SetupJumpBack(this);
    }
}

void EnJso_SetupJumpBack(EnJso* this) {
    EnJso_ChangeAnim(this, EN_JSO_ANIM_JUMP_BACK);
    this->action = EN_JSO_ACTION_JUMP_BACK;
    this->timer = 30;
    this->actor.speed = 7.0f;
    this->actor.velocity.y = 20.0f;

    if (this->swordState != EN_JSO_SWORD_STATE_BOTH_DRAWN) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_ANSATSUSYA_SWORD);
        this->swordState = EN_JSO_SWORD_STATE_BOTH_DRAWN;
    }

    this->actor.world.rot.y += 0x8000;
    this->actionFunc = EnJso_JumpBack;
}

void EnJso_JumpBack(EnJso* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;

    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 0xA, 0xBB8, 0x14);

    if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
        this->actor.speed = 0.0f;
    }

    if ((this->timer == 0) || ((curFrame >= this->animEndFrame) && (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND))) {
        this->actor.world.rot.x = 0;
        this->actor.velocity.y = 0.0f;
        this->actor.speed = 0.0f;
        this->timer = 0;
        sIsAttacking = false;
        this->isAttacking = false;
        this->actor.world.rot.y = this->actor.shape.rot.y;
        this->attackTimer = Rand_S16Offset(10, 10);
        EnJso_SetupCirclePlayer(this, play);
    }
}

void EnJso_SetupDead(EnJso* this, PlayState* play) {
    AudioSfx_SetChannelIO(&this->actor.projectedPos, NA_SE_EN_ANSATSUSYA_DASH_2, 0);
    EnJso_ChangeAnim(this, EN_JSO_ANIM_DAMAGED);

    if (((this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_SFX) ||
         (this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX)) &&
        (this->drawDmgEffAlpha == 0)) {
        this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FIRE;
    }

    this->actor.flags &= ~(ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE);
    this->actor.flags |= ACTOR_FLAG_LOCK_ON_DISABLED;
    this->actor.speed = 0.0f;
    this->disableBlure = true;
    this->timer = 30;
    Enemy_StartFinishingBlow(play, &this->actor);
    Actor_PlaySfx(&this->actor, NA_SE_EN_ANSATSUSYA_DEAD);
    this->action = EN_JSO_ACTION_DEAD;
    this->actionFunc = EnJso_Dead;
}

void EnJso_Dead(EnJso* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;

    if ((this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_SFX) ||
        (this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX)) {
        if (this->drawDmgEffAlpha != 0) {
            Actor_SpawnIceEffects(play, &this->actor, this->bodyPartsPos, EN_JSO_BODYPART_MAX, 2, 0.3f, 0.2f);
            this->drawDmgEffAlpha = 0;
            this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FIRE;
        } else {
            return;
        }
    }

    if ((curFrame >= this->animEndFrame) && (this->timer == 0)) {
        EnJso_ChangeAnim(this, EN_JSO_ANIM_FALL_DOWN);
        this->action = EN_JSO_ACTION_FALL_DOWN_AND_TALK;
        this->timer = 20;
        this->actor.speed = 0.f;
        this->actor.world.rot.x = this->actor.shape.rot.x = 0;
        this->knockbackVelocity.x = this->knockbackVelocity.z = 0.f;
        this->actionFunc = GaroKneelBeforeBurst;
    }
}

static void GaroKneelBeforeBurst(EnJso* this, PlayState* play) {
    if (this->skelAnime.curFrame < this->animEndFrame || this->timer)
        return;
    for (s32 i = 0; i < 9; ++i) {
        s16 angle = i * 0x2000;
        Vec3f velocity = { Math_SinS(angle), 0.f, Math_CosS(angle) };
        Vec3f position = this->actor.world.pos;
        position.x += velocity.x * 12.f;
        position.z += velocity.z * 12.f;
        if (this->actor.floorHeight > -30000.f)
            position.y = this->actor.floorHeight;
        comboEnemyDeathFlamePreset(play, &position, &velocity, &velocity, 150, 20, 1);
    }
    Actor_PlaySfx(&this->actor, 0x3a1f);
    if (this->isAttacking)
        sIsAttacking = false;
    if (this->isPlayerLockedOn)
        sIsPlayerLockedOn = false;
    this->isAttacking = this->isPlayerLockedOn = false;
    Actor_Kill(&this->actor);
}

void EnJso_UpdateDamage(EnJso* this, PlayState* play) {
    s32 attackDealsDamage = false;

    if (this->bodyCollider.base.acFlags & AC_HIT) {
        this->bodyCollider.base.acFlags &= ~AC_HIT;
        if ((this->actor.colChkInfo.damageEffect == EN_JSO_DMGEFF_STUN) ||
            (this->actor.colChkInfo.damageEffect == EN_JSO_DMGEFF_ELECTRIC_STUN)) {
            if (((this->drawDmgEffType != ACTOR_DRAW_DMGEFF_FROZEN_SFX) &&
                 (this->drawDmgEffType != ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX)) ||
                (this->drawDmgEffAlpha == 0)) {
                Actor_PlaySfx(&this->actor, NA_SE_EN_COMMON_FREEZE);
                Actor_SetColorFilter(&this->actor, COLORFILTER_COLORFLAG_BLUE, 255, COLORFILTER_BUFFLAG_OPA, 40);
                if (this->actor.colChkInfo.damageEffect == EN_JSO_DMGEFF_ELECTRIC_STUN) {
                    this->drawDmgEffAlpha = 40;
                    this->drawDmgEffType = ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_SMALL;
                }

                EnJso_SetupStunned(this);
            }
        } else {
            switch (this->action) {
                case EN_JSO_ACTION_CIRCLE_PLAYER:
                case EN_JSO_ACTION_GUARD:
                    this->actor.speed = 0.0f;
                    EnJso_SetupGuard(this);
                    attackDealsDamage = false;
                    break;

                case EN_JSO_ACTION_DASH_ATTACK:
                case EN_JSO_ACTION_SLASH:
                case EN_JSO_ACTION_WAIT_AFTER_SLASH:
                case EN_JSO_ACTION_STUNNED:
                case EN_JSO_ACTION_COWER:
                    switch (this->actor.colChkInfo.damageEffect) {
                        case EN_JSO_DMGEFF_NONE:
                            attackDealsDamage = true;
                            break;

                        case EN_JSO_DMGEFF_FIRE:
                            this->drawDmgEffAlpha = 40;
                            this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FIRE;
                            attackDealsDamage = true;
                            break;

                        case EN_JSO_DMGEFF_LIGHT_ORB:
                            if (((this->drawDmgEffType != ACTOR_DRAW_DMGEFF_FROZEN_SFX) &&
                                 (this->drawDmgEffType != ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX)) ||
                                (this->drawDmgEffAlpha == 0)) {
                                comboEnemyActorSpawn(&play->actorCtx, play, 0xa2, this->actor.focus.pos.x,
                                                     this->actor.focus.pos.y, this->actor.focus.pos.z, 0, 0, 0,
                                                     CLEAR_TAG_PARAMS(CLEAR_TAG_LARGE_LIGHT_RAYS));
                                this->drawDmgEffAlpha = 20;
                                this->drawDmgEffType = ACTOR_DRAW_DMGEFF_LIGHT_ORBS;
                                attackDealsDamage = true;
                            }
                            break;

                        case EN_JSO_DMGEFF_FREEZE:
                            if (((this->drawDmgEffType != ACTOR_DRAW_DMGEFF_FROZEN_SFX) &&
                                 (this->drawDmgEffType != ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX)) ||
                                (this->drawDmgEffAlpha == 0)) {
                                attackDealsDamage = false;
                                Actor_ApplyDamage(&this->actor);
                                this->drawDmgEffAlpha = 80;
                                this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FROZEN_SFX;
                                this->drawDmgEffScale = 0.0f;
                                this->drawDmgEffFrozenSteamScale = 1.5f;
                            }

                            if (this->actor.colChkInfo.health <= 0) {
                                EnJso_SetupDead(this, play);
                                attackDealsDamage = false;
                            } else {
                                EnJso_SetupStunned(this);
                                attackDealsDamage = false;
                            }
                            break;

                        default:
                            break;
                    }
                    break;

                default:
                    break;
            }

            if (attackDealsDamage) {
                Actor_ApplyDamage(&this->actor);
                this->bodyCollider.base.colMaterial = COL_MATERIAL_NONE;
                this->bodyCollider.base.acFlags |= AC_HARD;
                if (this->actor.colChkInfo.health > 0) {
                    EnJso_SetupDamaged(this, play);
                } else {
                    EnJso_SetupDead(this, play);
                }
            }
        }
    }
}

static void GaroNeutralIdle(EnJso* garo, PlayState* play) {
    garo->actor.speed = 0.f;
    if (comboEnemyGaroInRange(&garo->actor, play, 450.f, 50.f)) {
        EnJso_SetupCirclePlayer(garo, play);
        garo->attackTimer = 20;
    } else if (!garo->disableAnimations) {
        SkelAnime_Update(&garo->skelAnime);
    }
}

void EnJso_Update(Actor* thisx, PlayState* play) {
    EnJso* this = (EnJso*)thisx;
    GaroUpdateSwords(this);
    if (this->action == EN_JSO_ACTION_CIRCLE_PLAYER && this->actionFunc != GaroNeutralIdle &&
        comboEnemyGaroCanRest(this, play)) {
        EnJso_ChangeAnim(this, EN_JSO_ANIM_IDLE);
        this->actor.speed = 0.f;
        this->actionFunc = GaroNeutralIdle;
    }

    if ((this->action != EN_JSO_ACTION_CIRCLE_PLAYER) && !this->disableAnimations) {
        SkelAnime_Update(&this->skelAnime);
    }

    EnJso_UpdateDamage(this, play);
    if (this->actor.isTargeted) {
        sIsPlayerLockedOn = true;
        this->isPlayerLockedOn = true;
    } else if (this->isPlayerLockedOn) {
        this->isPlayerLockedOn = false;
        sIsPlayerLockedOn = false;
    }

    this->actionFunc(this, play);
    DECR(this->attackMovementTimer);
    DECR(this->attackTimer);
    DECR(this->timer);
    DECR(this->drawDmgEffAlpha);

    if (this->action != EN_JSO_ACTION_FALL_DOWN_AND_TALK) {
        Actor_SetFocus(&this->actor, 50.0f);
    } else {
        Actor_SetFocus(&this->actor, 30.0f);
    }

    Actor_SetScale(&this->actor, this->scale);
    Actor_MoveWithGravity(&this->actor);
    this->actor.world.pos.x += this->knockbackVelocity.x;
    this->actor.world.pos.z += this->knockbackVelocity.z;

    if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
        Math_ApproachZeroF(&this->knockbackVelocity.x, 1.0f, 2.0f);
        Math_ApproachZeroF(&this->knockbackVelocity.z, 1.0f, 2.0f);
    }

    if (this->action != EN_JSO_ACTION_REAPPEAR) {
        Actor_UpdateBgCheckInfo(play, &this->actor, 35.0f, 40.0f, 40.0f,
                                UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_2 | UPDBGCHECKINFO_FLAG_4 |
                                    UPDBGCHECKINFO_FLAG_8 | UPDBGCHECKINFO_FLAG_10);
    }

    if ((this->action == EN_JSO_ACTION_SPIN_BEFORE_ATTACK) || (this->action == EN_JSO_ACTION_DASH_ATTACK)) {
        this->afterimageIndex++;
        if (this->afterimageIndex >= EN_JSO_AFTERIMAGE_COUNT) {
            this->afterimageIndex = 0;
        }

        if (this->afterimageCount < EN_JSO_AFTERIMAGE_COUNT - 1) {
            this->afterimageCount++;
        }

        this->afterimagePos[this->afterimageIndex] = this->actor.world.pos;
        this->afterimagePos[this->afterimageIndex].y += 40.0f;
        this->afterimageRot[this->afterimageIndex] = this->actor.world.rot;

        {
            s32 i;

            for (i = 0; i < EN_JSO_AFTERIMAGE_COUNT; i++) {
                this->afterimageJointTable[this->afterimageIndex][i] = this->jointTable[i];
            }
        }
    } else {
        this->afterimageCount = 0;
    }

    if ((this->action != EN_JSO_ACTION_INTRO_CUTSCENE) && (this->action != EN_JSO_ACTION_CIRCLE_PLAYER) &&
        (this->action != EN_JSO_ACTION_SPIN_BEFORE_ATTACK) && (this->action != EN_JSO_ACTION_DAMAGED) &&
        (this->action != EN_JSO_ACTION_JUMP_BACK)) {
        this->actor.shape.rot.y = this->actor.world.rot.y;
    }

    this->actor.shape.rot.x = this->actor.world.rot.x;
    Collider_UpdateCylinder(&this->actor, &this->bodyCollider);

    if ((this->action != EN_JSO_ACTION_INTRO_CUTSCENE) && (this->action != EN_JSO_ACTION_REAPPEAR) &&
        (this->action != EN_JSO_ACTION_DAMAGED) && (this->action != EN_JSO_ACTION_SPIN_BEFORE_ATTACK) &&
        (this->action != EN_JSO_ACTION_DEAD) && (this->action != EN_JSO_ACTION_FALL_DOWN_AND_TALK) &&
        (this->action != EN_JSO_ACTION_UNK_15)) {
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->bodyCollider.base);
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->bodyCollider.base);

        if ((this->action == EN_JSO_ACTION_SLASH) && !this->slashHitSomething &&
            (this->swordState == EN_JSO_SWORD_STATE_BOTH_DRAWN)) {
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->rightSwordCollider.base);
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->leftSwordCollider.base);
        }
    }
}

s32 EnJso_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx) {
    EnJso* this = (EnJso*)thisx;

    if (limbIndex == GARO_LIMB_RIGHT_ARM) {
        rot->x += this->rightArmRot.x;
        rot->y += this->rightArmRot.y;
        rot->z += this->rightArmRot.z;
    }

    if (limbIndex == GARO_LIMB_ROBE_RIGHT) {
        rot->x += this->robeRightRot.x;
        rot->y += this->robeRightRot.y;
        rot->z += this->robeRightRot.z;
    }

    if ((limbIndex == GARO_LIMB_LEFT_SWORD) || (limbIndex == GARO_LIMB_RIGHT_SWORD)) {
        if (this->swordState == EN_JSO_SWORD_STATE_NONE_DRAWN) {
            *dList = NULL;
        } else if (limbIndex == GARO_LIMB_LEFT_SWORD) {
            if (this->swordState == EN_JSO_SWORD_STATE_RIGHT_DRAWN) {
                *dList = NULL;
            }
        } else if ((limbIndex == GARO_LIMB_RIGHT_SWORD) && (this->swordState == EN_JSO_SWORD_STATE_LEFT_DRAWN)) {
            *dList = NULL;
        }
    }

    return false;
}

void EnJso_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx) {
    static Vec3f sSwordTipOffset = { 1600.0f, 0.0f, 0.0f };
    static Vec3f sSwordBaseOffset = { 0.0f, 0.0f, 0.0f };
    static Vec3f sSwordTipQuadOffset = { 1700.0f, 0.0f, 0.0f };
    static Vec3f sSwordBaseQuadOffset = { 0.0f, 0.0f, 0.0f };
    EnJso* this = (EnJso*)thisx;
    Vec3f swordTipPos;
    Vec3f swordBasePos;

    Matrix_Push();

    if (limbIndex == GARO_LIMB_LEFT_SWORD) {
        if (this->swordState == EN_JSO_SWORD_STATE_KNOCKED_OUT_OF_HANDS) {
            GaroSwordFragment(this, limbIndex, *dList);
        }

        Matrix_Translate(0.0f, 0.0f, 0.0f, MTXMODE_APPLY);
        Math_Vec3f_Copy(&this->leftSwordCollider.dim.quad[3], &this->leftSwordCollider.dim.quad[1]);
        Math_Vec3f_Copy(&this->leftSwordCollider.dim.quad[2], &this->leftSwordCollider.dim.quad[0]);
        Matrix_MultVec3f(&sSwordTipQuadOffset, &this->leftSwordCollider.dim.quad[1]);
        Matrix_MultVec3f(&sSwordBaseQuadOffset, &this->leftSwordCollider.dim.quad[0]);
        Collider_SetQuadVertices(&this->leftSwordCollider, &this->leftSwordCollider.dim.quad[0],
                                 &this->leftSwordCollider.dim.quad[1], &this->leftSwordCollider.dim.quad[2],
                                 &this->leftSwordCollider.dim.quad[3]);
        Matrix_MultVec3f(&sSwordTipOffset, &swordTipPos);
        Matrix_MultVec3f(&sSwordBaseOffset, &swordBasePos);

        if (((this->action == EN_JSO_ACTION_SLASH) || (this->action == EN_JSO_ACTION_DASH_ATTACK)) &&
            !this->disableBlure) {
            EffectBlure_AddVertex(Effect_GetByIndex(this->leftSwordBlureIndex), &swordTipPos, &swordBasePos);
        } else if (this->disableBlure == true) {
            EffectBlure_AddSpace(Effect_GetByIndex(this->leftSwordBlureIndex));
        }
    }

    if (limbIndex == GARO_LIMB_RIGHT_SWORD) {
        if (this->swordState == EN_JSO_SWORD_STATE_KNOCKED_OUT_OF_HANDS) {
            GaroSwordFragment(this, limbIndex, *dList);
            this->swordState = EN_JSO_SWORD_STATE_NONE_DRAWN;
        }

        Matrix_Translate(0.0f, 0.0f, 0.0f, MTXMODE_APPLY);
        Math_Vec3f_Copy(&this->rightSwordCollider.dim.quad[3], &this->rightSwordCollider.dim.quad[1]);
        Math_Vec3f_Copy(&this->rightSwordCollider.dim.quad[2], &this->rightSwordCollider.dim.quad[0]);
        Matrix_MultVec3f(&sSwordTipQuadOffset, &this->rightSwordCollider.dim.quad[1]);
        Matrix_MultVec3f(&sSwordBaseQuadOffset, &this->rightSwordCollider.dim.quad[0]);
        Collider_SetQuadVertices(&this->rightSwordCollider, &this->rightSwordCollider.dim.quad[0],
                                 &this->rightSwordCollider.dim.quad[1], &this->rightSwordCollider.dim.quad[2],
                                 &this->rightSwordCollider.dim.quad[3]);
        Matrix_MultVec3f(&sSwordTipOffset, &swordTipPos);
        Matrix_MultVec3f(&sSwordBaseOffset, &swordBasePos);

        if (((this->action == EN_JSO_ACTION_SLASH) || (this->action == EN_JSO_ACTION_DASH_ATTACK)) &&
            !this->disableBlure) {
            EffectBlure_AddVertex(Effect_GetByIndex(this->rightSwordBlureIndex), &swordTipPos, &swordBasePos);
        } else if (this->disableBlure == true) {
            EffectBlure_AddSpace(Effect_GetByIndex(this->rightSwordBlureIndex));
            this->disableBlure = false;
        }
    }

    if ((limbIndex == GARO_LIMB_LEFT_SWORD) || (limbIndex == GARO_LIMB_RIGHT_SWORD) ||
        (limbIndex == GARO_LIMB_ROBE_TOP) || (limbIndex == GARO_LIMB_ROBE_BACK) || (limbIndex == GARO_LIMB_ROBE_LEFT) ||
        (limbIndex == GARO_LIMB_ROBE_RIGHT) || (limbIndex == GARO_LIMB_ROBE_FRONT) || (limbIndex == GARO_LIMB_HEAD) ||
        (limbIndex == GARO_LIMB_RIGHT_THIGH) || (limbIndex == GARO_LIMB_RIGHT_FOOT) ||
        (limbIndex == GARO_LIMB_LEFT_THIGH) || (limbIndex == GARO_LIMB_LEFT_FOOT)) {
        Matrix_MultZero(&this->bodyPartsPos[this->bodyPartIndex]);
        this->bodyPartIndex++;
        if (this->bodyPartIndex >= EN_JSO_BODYPART_MAX) {
            this->bodyPartIndex = 0;
        }
    }

    Matrix_Pop();
}

Gfx* EnJso_SetAfterimageRenderMode(GraphicsContext* gfxCtx) {
    Gfx* gfxHead = GRAPH_ALLOC(gfxCtx, 2 * sizeof(Gfx));
    Gfx* gfx = gfxHead;

    gDPSetRenderMode(
        gfx++, AA_EN | Z_CMP | Z_UPD | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | ZMODE_XLU | FORCE_BL | G_RM_FOG_SHADE_A,
        AA_EN | Z_CMP | Z_UPD | IM_RD | CLR_ON_CVG | CVG_DST_WRAP | ZMODE_XLU | FORCE_BL |
            GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA));
    gSPEndDisplayList(gfx++);

    return gfxHead;
}

void EnJso_Draw(Actor* thisx, PlayState* play) {
    static s16 sAfterimageAlpha[EN_JSO_AFTERIMAGE_COUNT] = {
        128, 0, 0, 0, 0, 128, 0, 0, 0, 0, 128, 0, 0, 0, 0, 128, 0, 0, 0, 0,
    };
    EnJso* this = (EnJso*)thisx;

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    Gfx_SetupDL25_Opa(play->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x0C, gActorSetupOpaDL);
    SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount,
                          EnJso_OverrideLimbDraw, EnJso_PostLimbDraw, &this->actor);

    if (this->afterimageCount > 0) {
        s32 i;
        s32 index = this->afterimageIndex;

        for (i = 0; i < this->afterimageCount; i++) {
            if (sAfterimageAlpha[i] == 0) {
                continue;
            }

            Matrix_Translate(this->afterimagePos[index].x, this->afterimagePos[index].y, this->afterimagePos[index].z,
                             MTXMODE_NEW);
            Matrix_Scale(this->scale, this->scale, this->scale, MTXMODE_APPLY);
            Matrix_RotateYS(BINANG_TO_RAD(this->afterimageRot[index].y), MTXMODE_APPLY);
            Matrix_RotateXS(BINANG_TO_RAD(this->afterimageRot[index].x), MTXMODE_APPLY);
            Matrix_RotateZS(BINANG_TO_RAD(this->afterimageRot[index].z), MTXMODE_APPLY);

            gDPPipeSync(POLY_XLU_DISP++);
            gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, sAfterimageAlpha[i]);
            gSPSegment(POLY_XLU_DISP++, 0x0C, EnJso_SetAfterimageRenderMode(play->state.gfxCtx));
            POLY_XLU_DISP = SkelAnime_DrawFlex(play, this->skelAnime.skeleton, this->afterimageJointTable[index],
                                               this->skelAnime.dListCount, NULL, NULL, &this->actor, POLY_XLU_DISP);

            index--;
            if (index < 0) {
                index = EN_JSO_AFTERIMAGE_COUNT - 1;
            }
        }
    }

    if (this->drawDmgEffAlpha != 0) {
        f32 drawDmgEffAlpha = this->drawDmgEffAlpha * 0.05f;

        if ((this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_SFX) ||
            (this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX)) {
            this->drawDmgEffScale += 0.3f;
            if (this->drawDmgEffScale > 0.5f) {
                this->drawDmgEffScale = 0.5f;
            }

            Math_ApproachF(&this->drawDmgEffFrozenSteamScale, this->drawDmgEffScale, 0.1f, 0.04f);
        } else {
            this->drawDmgEffScale = 0.8f;
            this->drawDmgEffFrozenSteamScale = 0.8f;
        }

        Actor_DrawDamageEffects(play, &this->actor, this->bodyPartsPos, EN_JSO_BODYPART_MAX, this->drawDmgEffScale,
                                this->drawDmgEffFrozenSteamScale, drawDmgEffAlpha, this->drawDmgEffType);
    }

    CLOSE_DISPS();
    GaroDrawSwords(this, play);
}

ActorProfile Enemy_Garo_Profile = {
    0xd13,
    ACTORCAT_ENEMY,
    ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED,
    0x1155,
    sizeof(EnJso),
    EnJso_Init,
    EnJso_Destroy,
    EnJso_Update,
    EnJso_Draw
};
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_GARO, Enemy_Garo_Profile, EnJso_Init, EnJso_Destroy, EnJso_Update, EnJso_Draw);

EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_GARO_LIGHT_RAYS_V1, EnJso_UpdateDamage);
