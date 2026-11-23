#include "mm_native_compat.h"
#include <combo/effect.h>
extern u8 FOREIGN_ENEMY_API_KNOCKBACK_LARGE_NO_DAMAGE[];
#undef ACTOR_EN_CLEAR_TAG
#define ACTOR_EN_CLEAR_TAG 0xa2
#define Matrix_MultZero ((void (*)(Vec3f*))(void*)FOREIGN_ENEMY_API_MATRIX_MULT_ZERO)
#define CollisionCheck_BlueBlood ((void (*)(PlayState*,Collider*,Vec3f*))0x8004c430)
#define CollisionCheck_SpawnShieldParticlesMetal ((void (*)(PlayState*,Vec3f*))0x800511b8)
#include <combo/enemy_ground.h>
#include <combo/enemy_flight.h>

static s32 FighterReactToMelee(PlayState* play, Actor* actor, f32 range, s16 facing, s16 playerFacing, s16 yaw)
{
    Player* player = GET_PLAYER(play);
    s16 playerAngle = (s16)(actor->yawTowardsPlayer + 0x8000) - player->actor.shape.rot.y;
    s16 angle = actor->yawTowardsPlayer - yaw;
    return actor->xzDistToPlayer <= range && player->meleeWeaponState && playerFacing >= ABS(playerAngle) && facing >= ABS(angle);
}
static f32 FighterDistance(Actor* a, Actor* b)
{
    Vec3f d = { a->world.pos.x-b->world.pos.x, a->world.pos.y-b->world.pos.y, a->world.pos.z-b->world.pos.z };
    return sqrtf(d.x*d.x+d.y*d.y+d.z*d.z);
}

static int FighterCanAttack(Actor* actor, f32 range)
{
    if (actor->xzDistToPlayer > range || fabsf(actor->yDistanceFromLink) >= 50.f)
        return 0;
    Vec3f from = actor->world.pos;
    Vec3f to = GET_PLAYER(gPlay)->actor.world.pos;
    from.y += 40.f;
    to.y += 40.f;
    return enemyFlightClear(gPlay, &from, &to);
}
#define Actor_WorldDistXYZToActor FighterDistance
#undef NA_SE_EN_BOSU_SWORD
#define NA_SE_EN_BOSU_SWORD 0x3a4d
#undef NA_SE_EN_COMMON_FREEZE
#define NA_SE_EN_COMMON_FREEZE 0x389e
#undef NA_SE_EN_GERUDOFT_DOWN
#define NA_SE_EN_GERUDOFT_DOWN 0x387a
#undef NA_SE_EN_GERUDOFT_WALK
#define NA_SE_EN_GERUDOFT_WALK 0x39a0
#undef NA_SE_EN_LAST2_SHOUT
#define NA_SE_EN_LAST2_SHOUT 0x39b9
#undef NA_SE_EN_PIRATE_ATTACK
#define NA_SE_EN_PIRATE_ATTACK 0x3998
#undef NA_SE_EN_PIRATE_BREATH
#define NA_SE_EN_PIRATE_BREATH 0x39b7
#undef NA_SE_EN_PIRATE_DAMAGE
#define NA_SE_EN_PIRATE_DAMAGE 0x3999
#undef NA_SE_EN_PIRATE_DAMM_BREATH
#define NA_SE_EN_PIRATE_DAMM_BREATH 0x3a73
#undef NA_SE_EN_PIRATE_DEAD
#define NA_SE_EN_PIRATE_DEAD 0x399a
#undef NA_SE_EN_PIRATE_ONGND
#define NA_SE_EN_PIRATE_ONGND 0x3a66
#undef NA_SE_EN_PIRATE_SHOUT
#define NA_SE_EN_PIRATE_SHOUT 0x3a74
#undef NA_SE_EN_TEKU_JUMP
#define NA_SE_EN_TEKU_JUMP 0x386c
#undef NA_SE_IT_SHIELD_BOUND
#define NA_SE_IT_SHIELD_BOUND 0x1806

#define ACTOR_EN_KAIZOKU 0x21d
#define ACTOR_FLAG_FREEZE_EXCEPTION (1u << 20)
#define TATL_HINT_ID_PIRATE 0xff
#define SWITCH_FLAG_NONE -1
#define ACTORCAT_EXPLOSIVES ACTORCAT_EXPLOSIVE
#define PLAYER_MELEE_WEAPON_STATE_0 0
#define ARROW_TYPE_DEKU_NUT 9
#define EFFECT_HITMARK_METAL 1
#define ActorShadow_DrawFeet ((void*)0x8001f280)
#define Actor_SetFeetPos ((void (*)(Actor*,s32,s32,Vec3f*,s32,Vec3f*))0x8001f74c)
#define Effect_Destroy Effect_Delete
#define func_800BE184 FighterReactToMelee
#define EffectSsKirakira_SpawnSmall EffectSsKiraKira_SpawnSmall
#define EffectSsHitmark_SpawnFixedScale EffectSsHitMark_SpawnFixedScale
#define SEGMENTED_TO_K0 SEGMENTED_TO_VIRTUAL
#define playerHeightRel yDistanceFromLink
#define Actor_TestFloorInDirection ((s16 (*)(Actor*,PlayState*,f32,s16))0x80026850)
#define Actor_IsFacingPlayer(actor,angle) (ABS((s16)((actor)->yawTowardsPlayer-(actor)->shape.rot.y)) <= (angle))
#define Actor_IsTargeted(play,actor) (((GET_PLAYER(play)->stateFlags1 & (1u<<4)) != 0) && (actor)->isTargeted)
#define Actor_OtherIsTargeted(play,actor) (((GET_PLAYER(play)->stateFlags1 & (1u<<4)) != 0) && !(actor)->isTargeted)
#define SkelAnime_DrawTransformFlexOpa ((void (*)(PlayState*,void**,Vec3s*,s32,GeneratedMmOverrideLimbDraw,GeneratedMmPostLimbDraw,TransformLimbDrawOpa,Actor*))(void*)FOREIGN_ENEMY_API_TRANSFORM_FLEX_OPA)

#undef OBJECT_KZ
#define OBJECT_KZ 0x1204
#include <combo/sfx.h>
static void FighterBlureVertex(void* effect, Vec3f* tip, Vec3f* hilt)
{
    if (effect)
        EffectBlure_AddVertex(effect, tip, hilt);
}
static void FighterBlureSpace(void* effect)
{
    if (effect)
        EffectBlure_AddSpace(effect);
}
#define gFighterPirateSlashAnim (*(AnimationHeader*)0x060003cc)
#define gFighterPirateSpinAttackAnim (*(AnimationHeader*)0x06000f5c)
#define gFighterPirateFlipAnim (*(AnimationHeader*)0x06001390)
#define gFighterPirateBlockAnim (*(AnimationHeader*)0x06001578)
#define gFighterPirateThrowFlashAnim (*(AnimationHeader*)0x06001e9c)
#define gFighterPirateUnusedDefeatAnim (*(AnimationHeader*)0x06002730)
#define gFighterPirateDefeatIdleAnim (*(AnimationHeader*)0x06002ba0)
#define gFighterPirateDefeatAnim (*(AnimationHeader*)0x06003a3c)
#define gFighterPirateDemonstrativeSwordSwingAnim (*(AnimationHeader*)0x060043e4)
#define gFighterPirateDamageAnim (*(AnimationHeader*)0x06004860)
#define gFighterPirateUnsheatheAnim (*(AnimationHeader*)0x06005644)
#define gFighterPirateWalkAnim (*(AnimationHeader*)0x060058b8)
#define gFighterPirate1TLUT ((u8*)0x060058d0)
#define gFighterPirateSkin1Tex ((u8*)0x06005ad0)
#define gFighterPirateShirtTopRedGradientTex ((u8*)0x06005b10)
#define gFighterPirateArmor1Tex ((u8*)0x06005b50)
#define gFighterPirateShirtBottomPatternAndVeilTex ((u8*)0x06005c50)
#define gFighterPirateBellyButtonTex ((u8*)0x06005d50)
#define gFighterPiratePantsTex ((u8*)0x06005e50)
#define gFighterPirateZipper1Tex ((u8*)0x06005f50)
#define gFighterPirateTorsoDL ((Gfx*)0x06007cb0)
#define gFighterPirateRightUpperArmDL ((Gfx*)0x06008130)
#define gFighterPirateRightForearmDL ((Gfx*)0x06008268)
#define gFighterPirateLeftUpperArmDL ((Gfx*)0x06008490)
#define gFighterPirateLeftForearmDL ((Gfx*)0x060085c8)
#define gFighterPirateWaistDL ((Gfx*)0x060087f0)
#define gFighterPirateRightThighDL ((Gfx*)0x06008a10)
#define gFighterPirateRightShinDL ((Gfx*)0x06008b10)
#define gFighterPirateLeftThighDL ((Gfx*)0x06008d80)
#define gFighterPirateLeftShinDL ((Gfx*)0x06008e80)
#define gFighterPirate2TLUT ((u8*)0x06009100)
#define gFighterPirateSkin2Tex ((u8*)0x060092f8)
#define gFighterPirateEarTex ((u8*)0x06009338)
#define gFighterPirateEyeOpenTex ((u8*)0x060093b8)
#define gFighterPirateShirtBottomPatternAndVeil2Tex ((u8*)0x060097b8)
#define gFighterPirateShoesTex ((u8*)0x060098b8)
#define gFighterPirateShoeBottomTex ((u8*)0x06009938)
#define gFighterPirateEyeHalfTex ((u8*)0x06009978)
#define gFighterPirateZipper2Tex ((u8*)0x06009d78)
#define gFighterPirateHairTex ((u8*)0x06009df8)
#define gFighterPirateFingersTex ((u8*)0x06009ef8)
#define gFighterPirateYellowSwordGuardAndEmblemTex ((u8*)0x06009ff8)
#define gFighterPirateEyeClosedTex ((u8*)0x0600a0f8)
#define gFighterPirateLeftFootDL ((Gfx*)0x0600c318)
#define gFighterPirateRightFootDL ((Gfx*)0x0600c4d8)
#define gFighterPirateLeftHandDL ((Gfx*)0x0600c698)
#define gFighterPirateLeftSwordDL ((Gfx*)0x0600c830)
#define gFighterPirateRightHandDL ((Gfx*)0x0600ca08)
#define gFighterPirateRightSwordDL ((Gfx*)0x0600cba0)
#define gFighterPirateVeilDL ((Gfx*)0x0600cd78)
#define gFighterPiratePonytailDL ((Gfx*)0x0600ce30)
#define gFighterPirateHeadDL ((Gfx*)0x0600cf20)
#define gFighterPirateSkel (*(FlexSkeletonHeader*)0x0600d828)
#define gFighterPirateSidestepAnim (*(AnimationHeader*)0x0600dbe4)
#define gFighterPirateUnusedJumpAnim (*(AnimationHeader*)0x0600e1c8)
#define gFighterPirateLandAnim (*(AnimationHeader*)0x0600e8bc)
#define gFighterPirateLandTalkAnim (*(AnimationHeader*)0x0600ed1c)
#define gFighterPirateUnusedTalkAnim (*(AnimationHeader*)0x0600ef9c)
#define gFighterPirateLowerWeaponsAnim (*(AnimationHeader*)0x0600f288)
#define gFighterPirateFightingIdleAnim (*(AnimationHeader*)0x0600f8e4)
enum { KAIZOKU_LIMB_NONE, KAIZOKU_LIMB_ROOT, KAIZOKU_LIMB_TORSO, KAIZOKU_LIMB_NECK, KAIZOKU_LIMB_PONYTAIL, KAIZOKU_LIMB_VEIL, KAIZOKU_LIMB_HEAD, KAIZOKU_LIMB_RIGHT_UPPER_ARM, KAIZOKU_LIMB_RIGHT_FOREARM, KAIZOKU_LIMB_RIGHT_WRIST, KAIZOKU_LIMB_RIGHT_HAND, KAIZOKU_LIMB_RIGHT_SWORD, KAIZOKU_LIMB_LEFT_UPPER_ARM, KAIZOKU_LIMB_LEFT_FOREARM, KAIZOKU_LIMB_LEFT_WRIST, KAIZOKU_LIMB_LEFT_HAND, KAIZOKU_LIMB_LEFT_SWORD, KAIZOKU_LIMB_LEFT_THIGH, KAIZOKU_LIMB_LEFT_SHIN, KAIZOKU_LIMB_LEFT_FOOT, KAIZOKU_LIMB_RIGHT_THIGH, KAIZOKU_LIMB_RIGHT_SHIN, KAIZOKU_LIMB_RIGHT_FOOT, KAIZOKU_LIMB_WAIST, KAIZOKU_LIMB_MAX };
typedef struct { Actor actor; void* validationFunc; } FighterPictoActor;

struct EnKaizoku;

typedef void (*EnKaizokuActionFunc)(struct EnKaizoku*, PlayState*);

#define KAIZOKU_GET_EXIT_INDEX(thisx) ((thisx)->params & 0x3F)
#define KAIZOKU_GET_SWITCH_FLAG(thisx) (((thisx)->params >> 6) & 0x7F)

#define KAIZOKU_GET_TYPE(this) ((this)->picto.actor.world.rot.z)

#define KAIZOKU_SWITCH_FLAG_NONE 0x7F

typedef enum EnKaizokuAnimation {
     KAIZOKU_ANIM_FIGHTING_IDLE,
     KAIZOKU_ANIM_UNUSED_CONVERSATION,
     KAIZOKU_ANIM_UNUSED_JUMP,
     KAIZOKU_ANIM_SIDESTEP,
     KAIZOKU_ANIM_WALK,
     KAIZOKU_ANIM_DAMAGE,
     KAIZOKU_ANIM_UNUSED_DEFEAT,
     KAIZOKU_ANIM_BLOCK,
     KAIZOKU_ANIM_FLIP,
     KAIZOKU_ANIM_SLASH_ATTCK,
     KAIZOKU_ANIM_SPIN_ATTACK,
     KAIZOKU_ANIM_LAND,
     KAIZOKU_ANIM_CHALLENGE,
     KAIZOKU_ANIM_UNSHEATHE,
     KAIZOKU_ANIM_LOWER_WEAPONS,
     KAIZOKU_ANIM_DEMONSTRATIVE_SWORD_SWING,
     KAIZOKU_ANIM_DEFEAT,
     KAIZOKU_ANIM_DEFEAT_IDLE,
     KAIZOKU_ANIM_THROW_FLASH,
     KAIZOKU_ANIM_MAX
} EnKaizokuAnimation;

typedef enum EnKaizokuBodyPart {
     KAIZOKU_BODYPART_0,
     KAIZOKU_BODYPART_1,
     KAIZOKU_BODYPART_2,
     KAIZOKU_BODYPART_3,
     KAIZOKU_BODYPART_4,
     KAIZOKU_BODYPART_5,
     KAIZOKU_BODYPART_6,
     KAIZOKU_BODYPART_7,
     KAIZOKU_BODYPART_8,
     KAIZOKU_BODYPART_9,
     KAIZOKU_BODYPART_10,
     KAIZOKU_BODYPART_11,
     KAIZOKU_BODYPART_12,
     KAIZOKU_BODYPART_13,
     KAIZOKU_BODYPART_14,
     KAIZOKU_BODYPART_MAX
} EnKaizokuBodyPart;

typedef enum KaizokuTextOffset {
     KAIZOKU_TALK_SHOUT_FROM_ABOVE,
     KAIZOKU_TALK_INTRO,
     KAIZOKU_TALK_PLAYER_VICTORY,
     KAIZOKU_TALK_PLAYER_DEFEAT
} KaizokuTextOffset;

typedef struct EnKaizoku {
     FighterPictoActor picto;
     SkelAnime skelAnime;
     Vec3s jointTable[KAIZOKU_LIMB_MAX];
     Vec3s morphTable[KAIZOKU_LIMB_MAX];
     EnKaizokuActionFunc actionFunc;
     s16 action;
     s16 combatTimer;
     s16 lookTimer;
     s16 iceTimer;
     s16 drawDmgEffTimer;
     s16 drawDmgEffType;
     s16 switchFlag;
     f32 drawDmgEffScale;
     f32 drawDmgEffFrozenSteamScale;
     s16 textIdOffset;
     s16 textType;
     s16 eyeIndex;
     s16 blinkTimer;
     s16 spinAttackState;
     s16 swordState;
     s16 exitIndex;
     s16 csId;
     u8 animationsDisabled;
     u8 defeatBreathingStarted;
     f32 animEndFrame;
     f32 circlingRate;
     EnKaizokuAnimation animIndex;
     s32 bodyPartIndex;
     s32 colorType;
     f32 boyoBounceVelocity;
     s16 boyoBounceAngle;
     Vec3f swordScaleRight;
     Vec3f swordScaleLeft;
     Vec3f bodyPartsPos[KAIZOKU_BODYPART_MAX];
     Vec3f velocity;
     s32 blureIndex;
     ColliderCylinder bodyCollider;
     ColliderQuad swordCollider;
     ColliderTris blockCollider;
     ColliderTrisElement blockElements[2];
     Vec3s headRot;
     Vec3f rightFootPos;
     Vec3f leftFootPos;
     s32 cutsceneTimer;
     s16 cutsceneState;
     s16 subCamId;
     s16 flashTimer;
     Vec3f subCamEye;
     Vec3f subCamAt;
     Vec3f subCamUp;
     Vec3f subCamEyeTarget;
     Vec3f subCamAtTarget;
     f32 subCamVelocity;
     f32 unk_5E4;
     f32 flashScreenAlpha;
     f32 flashScreenAlphaTarget;
} EnKaizoku;

#define FLAGS                                                                                 \
    (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED | \
     ACTOR_FLAG_FREEZE_EXCEPTION)

void EnKaizoku_Init(Actor* thisx, PlayState* play);
void EnKaizoku_Destroy(Actor* thisx, PlayState* play);
void EnKaizoku_Update(Actor* thisx, PlayState* play2);
void EnKaizoku_Draw(Actor* thisx, PlayState* play);

s32 EnKaizoku_ValidatePictograph(PlayState* play, Actor* actor);
void EnKaizoku_SetupWaitForApproach(EnKaizoku* this);
void EnKaizoku_WaitForApproach(EnKaizoku* this, PlayState* play);
void EnKaizoku_PlayerLoss(EnKaizoku* this, PlayState* play);
void EnKaizoku_SetupPlayerWinCutscene(EnKaizoku* this);
void EnKaizoku_PlayerWinCutscene(EnKaizoku* this, PlayState* play);
void EnKaizoku_SetupReady(EnKaizoku* this);
void EnKaizoku_Ready(EnKaizoku* this, PlayState* play);
void EnKaizoku_SetupSpinDodge(EnKaizoku* this, PlayState* play);
void EnKaizoku_SpinDodge(EnKaizoku* this, PlayState* play);
void EnKaizoku_SetupBlock(EnKaizoku* this);
void EnKaizoku_Block(EnKaizoku* this, PlayState* play);
void EnKaizoku_SetupJump(EnKaizoku* this);
void EnKaizoku_SpawnVerticalFootDust(PlayState* play, Vec3f* pos);
void EnKaizoku_Jump(EnKaizoku* this, PlayState* play);
void EnKaizoku_SetupRollBack(EnKaizoku* this);
void EnKaizoku_RollBack(EnKaizoku* this, PlayState* play);
void EnKaizoku_SetupSlash(EnKaizoku* this);
void EnKaizoku_Slash(EnKaizoku* this, PlayState* play2);
void EnKaizoku_SetupRollForward(EnKaizoku* this);
void EnKaizoku_RollForward(EnKaizoku* this, PlayState* play);
void EnKaizoku_SetupAdvance(EnKaizoku* this);
void EnKaizoku_Advance(EnKaizoku* this, PlayState* play);
void EnKaizoku_SetupPivot(EnKaizoku* this);
void EnKaizoku_Pivot(EnKaizoku* this, PlayState* play);
void EnKaizoku_SetupSpinAttack(EnKaizoku* this);
void EnKaizoku_SpinAttack(EnKaizoku* this, PlayState* play);
void EnKaizoku_SetupCircle(EnKaizoku* this);
void EnKaizoku_Circle(EnKaizoku* this, PlayState* play);
void EnKaizoku_Stunned(EnKaizoku* this, PlayState* play);
void EnKaizoku_Damaged(EnKaizoku* this, PlayState* play);
void EnKaizoku_DefeatKnockdown(EnKaizoku* this, PlayState* play);

typedef enum KaizokuAction {
     KAIZOKU_ACTION_HIDDEN,
     KAIZOKU_ACTION_READY,
     KAIZOKU_ACTION_SPIN_DODGE,
     KAIZOKU_ACTION_CIRCLE,
     KAIZOKU_ACTION_ADVANCE,
     KAIZOKU_ACTION_ROLL_FORWARD,
     KAIZOKU_ACTION_JUMP,
     KAIZOKU_ACTION_ROLL_BACK,
     KAIZOKU_ACTION_UNUSED_8,
     KAIZOKU_ACTION_SLASH,
     KAIZOKU_ACTION_PIVOT,
     KAIZOKU_ACTION_SPIN_ATTACK,
     KAIZOKU_ACTION_BLOCK,
     KAIZOKU_ACTION_STUNNED,
     KAIZOKU_ACTION_DAMAGED,
     KAIZOKU_ACTION_KNOCK_DOWN,
     KAIZOKU_ACTION_SCENE_FADE
} KaizokuAction;

typedef enum KaizokuLipstickColors {
     KAIZOKU_LIPSTICK_YELLOW,
     KAIZOKU_LIPSTICK_PURPLE,
     KAIZOKU_LIPSTICK_SEAGREEN,
} KaizokuLipstickColors;

Color_RGBA8 sKaizokuLipstickColors[] = {
    { 255, 255, 90, 255 },
    { 55, 25, 80, 255 },
    { 5, 195, 110, 255 },
};

typedef enum KaizokuOutfitColors {
     KAIZOKU_OUTFIT_ORANGE,
     KAIZOKU_OUTFIT_LAVENDER,
     KAIZOKU_OUTFIT_GREEN,
} KaizokuOutfitColors;

static Color_RGBA8 sKaizokuOutfitColors[] = {
    { 255, 130, 10, 255 },
    { 185, 130, 210, 255 },
    { 135, 195, 80, 255 },
};

typedef enum KaizokuDamageEffect {
     KAIZOKU_DMGEFF_NONE,
     KAIZOKU_DMGEFF_STUN,
     KAIZOKU_DMGEFF_FIRE_ARROW,
     KAIZOKU_DMGEFF_FREEZE,
     KAIZOKU_DMGEFF_LIGHT_ARROW,
     KAIZOKU_DMGEFF_ZORA_SHIELD,
     KAIZOKU_DMGEFF_STUNNED_ONLY = 0xD,
     KAIZOKU_DMGEFF_ALWAYS_HIT,
     KAIZOKU_DMGEFF_IFRAME_PROTECTED
} KaizokuDamageEffect;

static DamageTable sDamageTable = { {
     DMG_ENTRY(0, KAIZOKU_DMGEFF_STUN),
     DMG_ENTRY(1, KAIZOKU_DMGEFF_IFRAME_PROTECTED),
     DMG_ENTRY(0, KAIZOKU_DMGEFF_NONE),
     DMG_ENTRY(1, KAIZOKU_DMGEFF_IFRAME_PROTECTED),
     DMG_ENTRY(0, KAIZOKU_DMGEFF_STUN),
     DMG_ENTRY(1, KAIZOKU_DMGEFF_IFRAME_PROTECTED),
     DMG_ENTRY(0, KAIZOKU_DMGEFF_NONE),
     DMG_ENTRY(0, KAIZOKU_DMGEFF_STUN),
     DMG_ENTRY(1, KAIZOKU_DMGEFF_ALWAYS_HIT),
     DMG_ENTRY(1, KAIZOKU_DMGEFF_ALWAYS_HIT),
     DMG_ENTRY(0, KAIZOKU_DMGEFF_STUNNED_ONLY),
     DMG_ENTRY(2, KAIZOKU_DMGEFF_FIRE_ARROW),
     DMG_ENTRY(2, KAIZOKU_DMGEFF_FREEZE),
     DMG_ENTRY(2, KAIZOKU_DMGEFF_LIGHT_ARROW),
     DMG_ENTRY(1, KAIZOKU_DMGEFF_ALWAYS_HIT),
     DMG_ENTRY(0, KAIZOKU_DMGEFF_STUN),
     DMG_ENTRY(1, KAIZOKU_DMGEFF_ALWAYS_HIT),
     DMG_ENTRY(2, KAIZOKU_DMGEFF_ALWAYS_HIT),
     DMG_ENTRY(0, KAIZOKU_DMGEFF_STUN),
     DMG_ENTRY(0, KAIZOKU_DMGEFF_ZORA_SHIELD),
     DMG_ENTRY(0, KAIZOKU_DMGEFF_NONE),
     DMG_ENTRY(0, KAIZOKU_DMGEFF_NONE),
     DMG_ENTRY(1, KAIZOKU_DMGEFF_ALWAYS_HIT),
     DMG_ENTRY(1, KAIZOKU_DMGEFF_ALWAYS_HIT),
     DMG_ENTRY(1, KAIZOKU_DMGEFF_STUNNED_ONLY),
     DMG_ENTRY(0, KAIZOKU_DMGEFF_NONE),
     DMG_ENTRY(0, KAIZOKU_DMGEFF_NONE),
     DMG_ENTRY(0, KAIZOKU_DMGEFF_NONE),
     DMG_ENTRY(0, KAIZOKU_DMGEFF_NONE),
     DMG_ENTRY(0, KAIZOKU_DMGEFF_NONE),
     DMG_ENTRY(0, KAIZOKU_DMGEFF_NONE),
     DMG_ENTRY(1, KAIZOKU_DMGEFF_ALWAYS_HIT),
} };

ActorProfile En_Kaizoku_Profile = {
     0xe1d,
     ACTORCAT_ENEMY,
     FLAGS,
     OBJECT_KZ,
     sizeof(EnKaizoku),
     EnKaizoku_Init,
     EnKaizoku_Destroy,
     EnKaizoku_Update,
     EnKaizoku_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COL_MATERIAL_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK4,
        { 0x00000000, 0x00, 0x00 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        ATELEM_NONE | ATELEM_SFX_NORMAL,
        ACELEM_ON | ACELEM_HOOKABLE,
        OCELEM_ON,
    },
    { 20, 50, 0, { 0, 0, 0 } },
};

static ColliderQuadInit sQuadInit = {
    {
        COL_MATERIAL_NONE,
        AT_ON | AT_TYPE_PLAYER | AT_TYPE_ENEMY,
        AC_NONE,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_QUAD,
    },
    {
        ELEM_MATERIAL_UNK4,
        { 0xF7CFFFFF, 0x00, 0x00 },
        { 0x00000000, 0x00, 0x00 },
        ATELEM_ON | ATELEM_SFX_NORMAL | ATELEM_UNK7,
        ACELEM_NONE,
        OCELEM_NONE,
    },
    { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
};

static AnimationHeader* sAnimations[KAIZOKU_ANIM_MAX] = {
    &gFighterPirateFightingIdleAnim,
    &gFighterPirateUnusedTalkAnim,
    &gFighterPirateUnusedJumpAnim,
    &gFighterPirateSidestepAnim,
    &gFighterPirateWalkAnim,
    &gFighterPirateDamageAnim,
    &gFighterPirateUnusedDefeatAnim,
    &gFighterPirateBlockAnim,
    &gFighterPirateFlipAnim,
    &gFighterPirateSlashAnim,
    &gFighterPirateSpinAttackAnim,
    &gFighterPirateLandAnim,
    &gFighterPirateLandTalkAnim,
    &gFighterPirateUnsheatheAnim,
    &gFighterPirateLowerWeaponsAnim,
    &gFighterPirateDemonstrativeSwordSwingAnim,
    &gFighterPirateDefeatAnim,
    &gFighterPirateDefeatIdleAnim,
    &gFighterPirateThrowFlashAnim,
};

static u8 sAnimationModes[KAIZOKU_ANIM_MAX] = {
    ANIMMODE_LOOP,
    ANIMMODE_LOOP,
    ANIMMODE_ONCE,
    ANIMMODE_LOOP,
    ANIMMODE_LOOP,
    ANIMMODE_ONCE,
    ANIMMODE_ONCE,
    ANIMMODE_ONCE,
    ANIMMODE_ONCE,
    ANIMMODE_ONCE,
    ANIMMODE_ONCE,
    ANIMMODE_ONCE,
    ANIMMODE_LOOP,
    ANIMMODE_ONCE,
    ANIMMODE_ONCE,
    ANIMMODE_ONCE,
    ANIMMODE_ONCE,
    ANIMMODE_LOOP,
    ANIMMODE_ONCE,
};

void EnKaizoku_Init(Actor* thisx, PlayState* play) {

    EnKaizoku* this = (EnKaizoku*)thisx;
    EffectBlureInit1 blureInit;

    this->picto.actor.hintId = TATL_HINT_ID_PIRATE;
    this->picto.actor.attentionRangeType = ATTENTION_RANGE_3;
    this->picto.actor.colChkInfo.mass = 80;
    ActorShape_Init(&this->picto.actor.shape, 0.0f, ActorShadow_DrawFeet, 0.0f);
    this->textType = KAIZOKU_GET_TYPE(this);
    this->picto.actor.colChkInfo.health = 8;
    this->exitIndex = KAIZOKU_GET_EXIT_INDEX(&this->picto.actor);
    this->switchFlag = KAIZOKU_GET_SWITCH_FLAG(&this->picto.actor);

    if (this->textType >= 2) {
        this->textType = 0;
    } else if (this->textType == 2) {
        this->textType = 0;
    }

    this->colorType = KAIZOKU_GET_TYPE(this);
    this->picto.actor.world.rot.z = 0;
    this->picto.actor.colChkInfo.damageTable = &sDamageTable;
    SkelAnime_InitFlex(play, &this->skelAnime, &gFighterPirateSkel, &gFighterPirateWalkAnim, this->jointTable,
                       this->morphTable, KAIZOKU_LIMB_MAX);
    Collider_InitAndSetCylinder(play, &this->bodyCollider, &this->picto.actor, &sCylinderInit);
    Collider_InitAndSetQuad(play, &this->swordCollider, &this->picto.actor, &sQuadInit);
    blureInit.p1StartColor[0] = blureInit.p1StartColor[1] = blureInit.p1StartColor[2] = blureInit.p1StartColor[3] =
        blureInit.p2StartColor[0] = blureInit.p2StartColor[1] = blureInit.p2StartColor[2] = blureInit.p1EndColor[0] =
            blureInit.p1EndColor[1] = blureInit.p1EndColor[2] = blureInit.p2EndColor[0] = blureInit.p2EndColor[1] =
                blureInit.p2EndColor[2] = 255;
    blureInit.p2StartColor[3] = 64;
    blureInit.p1EndColor[3] = blureInit.p2EndColor[3] = 0;
    blureInit.elemDuration = 8;
    blureInit.unkFlag = 0;
    blureInit.calcMode = 2;
    Effect_Add(play, &this->blureIndex, EFFECT_BLURE1, 0, 0, &blureInit);
    Actor_SetScale(&this->picto.actor, 0.0125f);
    this->picto.actor.flags |= ACTOR_FLAG_LOCK_ON_DISABLED;
    this->picto.actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    if (this->switchFlag == KAIZOKU_SWITCH_FLAG_NONE) {
        this->switchFlag = SWITCH_FLAG_NONE;
    }

    if ((this->switchFlag > SWITCH_FLAG_NONE) && Flags_GetSwitch(play, this->switchFlag)) {
        Actor_Kill(&this->picto.actor);
        return;
    }

    this->bodyCollider.dim.radius = 20;
    this->bodyCollider.dim.height = 65;
    this->bodyCollider.dim.yShift = 0;
    this->csId = -1;
    this->picto.actor.world.pos = this->picto.actor.home.pos;
    this->picto.actor.prevPos = this->picto.actor.world.pos;
    this->picto.actor.gravity = -2.f;
    this->picto.actor.minVelocityY = -20.f;
    this->picto.validationFunc = EnKaizoku_ValidatePictograph;
    this->picto.actor.flags |= ACTOR_FLAG_HOOKSHOT_PULLS_PLAYER;
    this->colorType = (s32)(Rand_ZeroOne() * ARRAY_COUNT(sKaizokuOutfitColors));
    this->animationsDisabled = false;
    this->swordScaleRight = (Vec3f){ 1.f, 1.f, 1.f };
    this->swordScaleLeft = this->swordScaleRight;
    this->picto.actor.flags &= ~(ACTOR_FLAG_LOCK_ON_DISABLED | ACTOR_FLAG_FREEZE_EXCEPTION);
    this->picto.actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED;
    this->picto.actor.shape.shadowScale = 90.f;
    EnKaizoku_SetupReady(this);
}

void EnKaizoku_Destroy(Actor* thisx, PlayState* play) {
    EnKaizoku* this = (EnKaizoku*)thisx;

    Effect_Destroy(play, this->blureIndex);
    Collider_DestroyCylinder(play, &this->bodyCollider);
    Collider_DestroyQuad(play, &this->swordCollider);

}

s32 EnKaizoku_DodgeRanged(EnKaizoku* this, PlayState* play) {
    Actor* itemAction;
    Player* player = GET_PLAYER(play);
    s16 prevAction = this->action;

    for (itemAction = play->actorCtx.actors[ACTORCAT_ITEMACTION].first; itemAction != NULL;
         itemAction = itemAction->next) {
        if (((itemAction->id != ACTOR_ARMS_HOOK) && (itemAction->id != ACTOR_EN_ARROW)) || (player->unk_a73 == 0)) {
            continue;
        }

        if (((itemAction->id != ACTOR_ARMS_HOOK) || (this->action != KAIZOKU_ACTION_STUNNED)) &&
            (this->action != KAIZOKU_ACTION_JUMP)) {
            EnKaizoku_SetupJump(this);
        }
        break;
    }

    if (prevAction != this->action) {
        return true;
    } else {
        return false;
    }
}

void EnKaizoku_TurnHead(EnKaizoku* this) {
    if (this->action == KAIZOKU_ACTION_READY) {
        this->headRot.y = Math_SinS(this->lookTimer * 4200) * 8920;
    } else if (this->action != KAIZOKU_ACTION_STUNNED) {
        if ((this->action == KAIZOKU_ACTION_SLASH) || (this->action == KAIZOKU_ACTION_SPIN_ATTACK)) {
            Math_SmoothStepToS(&this->headRot.y, this->picto.actor.yawTowardsPlayer - this->picto.actor.shape.rot.y, 1,
                               500, 0);
            this->headRot.y = CLAMP(this->headRot.y, -0x256F, 0x256F);
        } else {
            this->headRot.y = 0;
        }
    }
}

s32 EnKaizoku_ReactToPlayer(EnKaizoku* this, PlayState* play, s16 arg2) {
    Player* player = GET_PLAYER(play);
    s16 angleToWall = ABS_ALT(this->picto.actor.wallYaw - this->picto.actor.shape.rot.y);
    s16 angleToPlayer = ABS_ALT(this->picto.actor.yawTowardsPlayer - this->picto.actor.shape.rot.y);
    Actor* explosiveActor;

    if (func_800BE184(play, &this->picto.actor, 100.0f, 0x2710, 0x4000, this->picto.actor.shape.rot.y)) {
        if (player->meleeWeaponAnimation == PLAYER_MWA_JUMPSLASH_START) {
            if (this->action != KAIZOKU_ACTION_SPIN_DODGE) {
                EnKaizoku_SetupSpinDodge(this, play);
            }
            return true;
        } else {
            EnKaizoku_SetupBlock(this);
            return true;
        }
    } else if (func_800BE184(play, &this->picto.actor, 100.0f, 0x5DC0, 0x2AA8, this->picto.actor.shape.rot.y)) {
        this->picto.actor.shape.rot.y = this->picto.actor.world.rot.y = this->picto.actor.yawTowardsPlayer;
        if ((this->picto.actor.bgCheckFlags & BGCHECKFLAG_WALL) && (ABS_ALT(angleToWall) < 0x2EE0) &&
            (this->picto.actor.xzDistToPlayer < 90.0f)) {
            if (this->action != KAIZOKU_ACTION_JUMP) {
                EnKaizoku_SetupJump(this);
            }
            return true;
        } else if (player->meleeWeaponAnimation == PLAYER_MWA_JUMPSLASH_START) {
            if (this->action != KAIZOKU_ACTION_SPIN_DODGE) {
                EnKaizoku_SetupSpinDodge(this, play);
            }
        } else if (this->picto.actor.xzDistToPlayer < BREG(11) + 180.0f) {
            EnKaizoku_SetupBlock(this);
        } else if (this->action != KAIZOKU_ACTION_ROLL_BACK) {
            EnKaizoku_SetupRollBack(this);
        }
        return true;
    }

    explosiveActor = Actor_FindNearby(play, &this->picto.actor, -1, ACTORCAT_EXPLOSIVES, 80.0f);
    if (explosiveActor != NULL) {
        this->picto.actor.shape.rot.y = this->picto.actor.world.rot.y = this->picto.actor.yawTowardsPlayer;

        if (((this->picto.actor.bgCheckFlags & BGCHECKFLAG_WALL) && (angleToWall < 0x2EE0)) ||
            (explosiveActor->id == ACTOR_EN_BOM_CHU)) {
            if ((explosiveActor->id == ACTOR_EN_BOM_CHU) &&
                (Actor_WorldDistXYZToActor(&this->picto.actor, explosiveActor) < 80.0f) &&
                (BINANG_ADD(this->picto.actor.shape.rot.y - explosiveActor->world.rot.y, 0x8000) < 0x4000)) {
                if (this->action != KAIZOKU_ACTION_JUMP) {
                    EnKaizoku_SetupJump(this);
                }
            } else {
                EnKaizoku_SetupBlock(this);
            }

            return true;
        }

        if (this->action != KAIZOKU_ACTION_ROLL_BACK) {
            EnKaizoku_SetupRollBack(this);
        }
        return true;
    }

    if (arg2) {
        s16 yawDiff;

        if (angleToPlayer >= 0x2710) {

            EnKaizoku_SetupBlock(this);
        } else {
            yawDiff = player->actor.shape.rot.y - this->picto.actor.shape.rot.y;
            if ((this->picto.actor.xzDistToPlayer <= 65.0f) && !Actor_OtherIsTargeted(play, &this->picto.actor) &&
                (ABS_ALT(yawDiff) < 0x5000)) {
                if (this->action != KAIZOKU_ACTION_SLASH) {
                    EnKaizoku_SetupSlash(this);
                    return true;
                }
            } else if (this->action != KAIZOKU_ACTION_CIRCLE) {
                EnKaizoku_SetupCircle(this);
            }
        }
        return true;
    }

    return false;
}

void EnKaizoku_ChangeAnim(EnKaizoku* this, EnKaizokuAnimation animIndex) {
    this->animIndex = animIndex;
    this->animEndFrame = Animation_GetLastFrame(sAnimations[this->animIndex]);
    Animation_Change(&this->skelAnime, sAnimations[this->animIndex], 1.0f, 0.0f, this->animEndFrame,
                     sAnimationModes[this->animIndex], 0.0f);
}

s32 EnKaizoku_ValidatePictograph(PlayState* play, Actor* actor) {
    return 0;
}

void EnKaizoku_SetupWaitForApproach(EnKaizoku* this) {
    Math_Vec3f_Copy(&this->swordScaleRight, &gZeroVec3f);
    Math_Vec3f_Copy(&this->swordScaleLeft, &gZeroVec3f);
    this->animationsDisabled = true;
    this->action = KAIZOKU_ACTION_HIDDEN;
    this->actionFunc = EnKaizoku_WaitForApproach;
}

void EnKaizoku_WaitForApproach(EnKaizoku* this, PlayState* play) {
    EnKaizoku_SetupReady(this);
}

void EnKaizoku_SetupPlayerLoss(EnKaizoku* this, PlayState* play) {
    EnKaizoku_SetupReady(this);
}

void EnKaizoku_PlayerLoss(EnKaizoku* this, PlayState* play) {
    EnKaizoku_SetupReady(this);
}

void EnKaizoku_SetupPlayerWinCutscene(EnKaizoku* this) {
    EnKaizoku_SetupReady(this);
}

void EnKaizoku_PlayerWinCutscene(EnKaizoku* this, PlayState* play) {
    EnKaizoku_SetupReady(this);
}

void EnKaizoku_SetupReady(EnKaizoku* this) {
    this->picto.actor.speed = 0.0f;
    EnKaizoku_ChangeAnim(this, KAIZOKU_ANIM_FIGHTING_IDLE);
    this->action = KAIZOKU_ACTION_READY;
    this->actionFunc = EnKaizoku_Ready;
    this->picto.actor.shape.shadowScale = 90.0f;
}

void EnKaizoku_Ready(EnKaizoku* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if ((this->lookTimer == 0) && !EnKaizoku_DodgeRanged(this, play) && !EnKaizoku_ReactToPlayer(this, play, false)) {
        s16 angleToPlayer = this->picto.actor.yawTowardsPlayer - this->picto.actor.shape.rot.y;

        if ((this->picto.actor.xzDistToPlayer < 100.0f) && (player->meleeWeaponState != PLAYER_MELEE_WEAPON_STATE_0) &&
            (angleToPlayer >= 0x1F40)) {
            this->picto.actor.shape.rot.y = this->picto.actor.world.rot.y = this->picto.actor.yawTowardsPlayer;
            EnKaizoku_SetupCircle(this);
        } else if (Actor_IsFacingPlayer(&this->picto.actor, 0xBB8)) {
            if ((this->picto.actor.xzDistToPlayer < 400.0f) && (this->picto.actor.xzDistToPlayer > 150.0f) &&
                (Rand_ZeroOne() < 0.7f)) {
                if ((Rand_ZeroOne() > 0.5f) || (ABS_ALT(angleToPlayer) < 0x3000)) {
                    EnKaizoku_SetupRollForward(this);
                } else {
                    EnKaizoku_SetupSpinAttack(this);
                }
            } else if (Rand_ZeroOne() > 0.1f) {
                EnKaizoku_SetupAdvance(this);
            } else {
                EnKaizoku_SetupCircle(this);
            }
        } else {
            EnKaizoku_SetupPivot(this);
        }
    }
}

void EnKaizoku_SetupSpinDodge(EnKaizoku* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    EnKaizoku_ChangeAnim(this, KAIZOKU_ANIM_SIDESTEP);
    if (Math_SinS(player->actor.shape.rot.y - this->picto.actor.shape.rot.y) > 0.0f) {
        this->picto.actor.speed = -10.0f;
    } else if (Math_SinS(player->actor.shape.rot.y - this->picto.actor.shape.rot.y) < 0.0f) {
        this->picto.actor.speed = 10.0f;
    } else if (Rand_ZeroOne() > 0.5f) {
        this->picto.actor.speed = 10.0f;
    } else {
        this->picto.actor.speed = -10.0f;
    }

    this->skelAnime.playSpeed = 1.0f;
    this->picto.actor.world.rot.y = this->picto.actor.shape.rot.y;
    this->combatTimer = 6;
    this->action = KAIZOKU_ACTION_SPIN_DODGE;
    this->circlingRate = 0.0f;
    this->actionFunc = EnKaizoku_SpinDodge;
}

void EnKaizoku_SpinDodge(EnKaizoku* this, PlayState* play) {
    s16 wallAngle;
    s32 beforeCurFrame;
    s32 afterCurFrame;

    this->picto.actor.world.rot.y = this->picto.actor.yawTowardsPlayer + 0x3A98;
    if ((this->picto.actor.bgCheckFlags & BGCHECKFLAG_WALL) ||
        !Actor_TestFloorInDirection(&this->picto.actor, play, this->picto.actor.speed,
                                    this->picto.actor.shape.rot.y + 0x4000)) {
        if (this->picto.actor.bgCheckFlags & BGCHECKFLAG_WALL) {
            if (this->picto.actor.speed >= 0.0f) {
                wallAngle = this->picto.actor.shape.rot.y + 0x4000;
            } else {
                wallAngle = this->picto.actor.shape.rot.y - 0x4000;
            }
            wallAngle = this->picto.actor.wallYaw - wallAngle;
        } else {
            this->picto.actor.speed *= -0.8f;
            wallAngle = 0;
        }

        if (ABS_ALT(wallAngle) > 0x4000) {
            EnKaizoku_SetupJump(this);
            return;
        }
    }

    if (this->picto.actor.xzDistToPlayer <= 65.0f) {
        Math_ApproachF(&this->circlingRate, -4.0f, 1.0f, 1.5f);
    } else if (this->picto.actor.xzDistToPlayer > 40.0f) {
        Math_ApproachF(&this->circlingRate, 4.0f, 1.0f, 1.5f);
    } else {
        Math_ApproachZeroF(&this->circlingRate, 1.0f, 6.65f);
    }

    if (this->circlingRate != 0.0f) {
        this->picto.actor.world.pos.x += Math_SinS(this->picto.actor.yawTowardsPlayer) * this->circlingRate;
        this->picto.actor.world.pos.z += Math_CosS(this->picto.actor.yawTowardsPlayer) * this->circlingRate;
    }

    this->skelAnime.playSpeed = 1.0f;
    beforeCurFrame = this->skelAnime.curFrame - this->skelAnime.playSpeed;
    afterCurFrame = this->skelAnime.curFrame + this->skelAnime.playSpeed;
    if ((this->skelAnime.curFrame != 0.0f) &&
        (((beforeCurFrame < 0) && (afterCurFrame > 0)) || ((beforeCurFrame < 5) && (afterCurFrame >= 6)))) {
        Actor_PlaySfx(&this->picto.actor, NA_SE_EN_GERUDOFT_WALK);
    }

    if (!(play->gameplayFrames & 0x5F)) {
        Actor_PlaySfx(&this->picto.actor, NA_SE_EN_PIRATE_BREATH);
    }

    if (this->combatTimer == 0) {
        this->picto.actor.shape.rot.y = this->picto.actor.yawTowardsPlayer;
        if (!EnKaizoku_DodgeRanged(this, play)) {
            if (this->picto.actor.xzDistToPlayer <= 70.0f) {
                EnKaizoku_SetupSlash(this);
            } else {
                EnKaizoku_SetupRollBack(this);
            }
        }
    } else if (this->picto.actor.speed >= 0.0f) {
        this->picto.actor.shape.rot.y += 0x4000;
    } else {
        this->picto.actor.shape.rot.y -= 0x4000;
    }
}

void EnKaizoku_SetupBlock(EnKaizoku* this) {
    EnKaizoku_ChangeAnim(this, KAIZOKU_ANIM_BLOCK);

    if (this->swordState != 0) {
        this->swordState = -1;
    }
    this->animationsDisabled = false;
    this->picto.actor.speed = 0.0f;
    this->combatTimer = Rand_S16Offset(10, 10);
    this->bodyCollider.base.acFlags |= AC_HARD;
    this->lookTimer = 12;
    this->action = KAIZOKU_ACTION_BLOCK;
    this->actionFunc = EnKaizoku_Block;
}

void EnKaizoku_Block(EnKaizoku* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 angleToPlayer;

    this->animationsDisabled = true;
    if (this->combatTimer == 0) {
        this->skelAnime.playSpeed = 1.0f;
    }
    if (this->lookTimer == 0) {
        this->animationsDisabled = false;
        angleToPlayer = this->picto.actor.yawTowardsPlayer - this->picto.actor.shape.rot.y;
        if ((ABS_ALT(angleToPlayer) <= 0x4000) && (this->picto.actor.xzDistToPlayer < 40.0f) &&
            (fabsf(this->picto.actor.playerHeightRel) < 50.0f)) {
            if (func_800BE184(play, &this->picto.actor, 100.0f, 10000, 0x4000, this->picto.actor.shape.rot.y)) {
                if (player->meleeWeaponAnimation == PLAYER_MWA_JUMPSLASH_START) {
                    this->bodyCollider.base.acFlags &= ~AC_HARD;
                    EnKaizoku_SetupSpinDodge(this, play);
                } else if ((play->gameplayFrames % 2) == 0) {
                    EnKaizoku_SetupBlock(this);
                } else {
                    this->bodyCollider.base.acFlags &= ~AC_HARD;
                    EnKaizoku_SetupRollBack(this);
                }
            } else {
                angleToPlayer = player->actor.shape.rot.y - this->picto.actor.shape.rot.y;
                if (ABS_ALT(angleToPlayer) < 0x4000) {
                    this->bodyCollider.base.acFlags &= ~AC_HARD;
                    EnKaizoku_SetupSlash(this);
                } else {
                    this->bodyCollider.base.acFlags &= ~AC_HARD;
                    EnKaizoku_SetupCircle(this);
                }
            }
        } else {
            this->bodyCollider.base.acFlags &= ~AC_HARD;
            EnKaizoku_SetupCircle(this);
        }
    } else if ((this->combatTimer == 0) &&
               func_800BE184(play, &this->picto.actor, 100.0f, 10000, 0x4000, this->picto.actor.shape.rot.y)) {
        if (player->meleeWeaponAnimation == PLAYER_MWA_JUMPSLASH_START) {
            this->bodyCollider.base.acFlags &= ~AC_HARD;
            EnKaizoku_SetupSpinDodge(this, play);
        } else if (!EnKaizoku_DodgeRanged(this, play)) {
            if (!(play->gameplayFrames & 1)) {
                if ((this->picto.actor.xzDistToPlayer < 100.0f) && (Rand_ZeroOne() > 0.7f)) {
                    this->bodyCollider.base.acFlags &= ~AC_HARD;
                    EnKaizoku_SetupJump(this);
                } else {
                    this->bodyCollider.base.acFlags &= ~AC_HARD;
                    EnKaizoku_SetupRollBack(this);
                }
            } else {
                EnKaizoku_SetupBlock(this);
            }
        }
    }
}

void EnKaizoku_SetupJump(EnKaizoku* this) {
    EnKaizoku_ChangeAnim(this, KAIZOKU_ANIM_FLIP);
    this->picto.actor.speed = 6.5f;
    this->picto.actor.velocity.y = 15.0f;
    Actor_PlaySfx(&this->picto.actor, NA_SE_EN_TEKU_JUMP);
    this->picto.actor.world.rot.y = this->picto.actor.shape.rot.y;
    this->bodyCollider.elem.elemMaterial = ELEM_MATERIAL_UNK4;
    this->bodyCollider.base.colMaterial = COL_MATERIAL_NONE;
    this->swordCollider.elem.elemMaterial = ELEM_MATERIAL_UNK4;
    this->action = KAIZOKU_ACTION_JUMP;
    this->actionFunc = EnKaizoku_Jump;
}

static Color_RGBA8 sKaizokuDustPrimColor = { 200, 160, 120, 255 };
static Color_RGBA8 sKaizokuDustEnvColor = { 130, 90, 50, 255 };

static Vec3f sKaizokuDustVelocity = { 0.0f, -1.5f, 0.0f };
static Vec3f sKaizokuDustAccel = { 0.0f, -0.2f, 0.0f };

void EnKaizoku_SpawnVerticalFootDust(PlayState* play, Vec3f* pos) {
    EffectSsKirakira_SpawnSmall(play, pos, &sKaizokuDustVelocity, &sKaizokuDustAccel, &sKaizokuDustPrimColor,
                                &sKaizokuDustEnvColor);
}

void EnKaizoku_Jump(EnKaizoku* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;

    Math_SmoothStepToS(&this->picto.actor.shape.rot.y, this->picto.actor.yawTowardsPlayer, 1, 0xFA0, 1);
    if (this->picto.actor.velocity.y >= 5.0f) {
        EnKaizoku_SpawnVerticalFootDust(play, &this->leftFootPos);
        EnKaizoku_SpawnVerticalFootDust(play, &this->rightFootPos);
    }

    this->animationsDisabled = false;
    if ((curFrame >= this->animEndFrame) &&
        (this->picto.actor.bgCheckFlags & (BGCHECKFLAG_GROUND | BGCHECKFLAG_GROUND_TOUCH))) {
        this->bodyCollider.elem.elemMaterial = ELEM_MATERIAL_UNK1;
        this->bodyCollider.base.colMaterial = COL_MATERIAL_HIT3;
        this->swordCollider.elem.elemMaterial = ELEM_MATERIAL_UNK2;
        this->picto.actor.shape.rot.x = 0;
        this->picto.actor.world.rot.y = this->picto.actor.shape.rot.y = this->picto.actor.yawTowardsPlayer;
        this->picto.actor.speed = 0.0f;
        this->picto.actor.velocity.y = 0.0f;
        this->picto.actor.world.pos.y = this->picto.actor.floorHeight;
        EnKaizoku_SetupSlash(this);
    }
}

void EnKaizoku_SetupRollBack(EnKaizoku* this) {
    EnKaizoku_ChangeAnim(this, KAIZOKU_ANIM_FLIP);
    this->picto.actor.speed = -8.0f;
    Actor_PlaySfx(&this->picto.actor, NA_SE_EN_TEKU_JUMP);
    this->bodyCollider.elem.elemMaterial = ELEM_MATERIAL_UNK4;
    this->bodyCollider.base.colMaterial = COL_MATERIAL_NONE;
    this->swordCollider.elem.elemMaterial = ELEM_MATERIAL_UNK4;
    this->action = KAIZOKU_ACTION_ROLL_BACK;
    this->actionFunc = EnKaizoku_RollBack;
    this->picto.actor.shape.rot.y = this->picto.actor.world.rot.y = this->picto.actor.yawTowardsPlayer;
}

void EnKaizoku_RollBack(EnKaizoku* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;

    this->animationsDisabled = false;
    if (curFrame >= this->animEndFrame) {
        if ((this->picto.actor.xzDistToPlayer < 170.0f) && (this->picto.actor.xzDistToPlayer > 140.0f) &&
            (Rand_ZeroOne() < 0.2f)) {
            EnKaizoku_SetupSpinAttack(this);
        } else {
            EnKaizoku_SetupBlock(this);
        }
    }
    if (!(play->gameplayFrames & 0x5F)) {
        Actor_PlaySfx(&this->picto.actor, NA_SE_EN_PIRATE_BREATH);
    }
}

void EnKaizoku_SetupSlash(EnKaizoku* this) {
    if (!FighterCanAttack(&this->picto.actor, 120.f))
    {
        if (this->action != KAIZOKU_ACTION_READY)
            EnKaizoku_SetupReady(this);
        this->swordState = -1;
        this->swordCollider.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
        return;
    }

    EnKaizoku_ChangeAnim(this, KAIZOKU_ANIM_SLASH_ATTCK);
    this->spinAttackState = 0;
    this->swordCollider.base.atFlags &= ~AT_BOUNCED;
    this->picto.actor.speed = 0.0f;
    AudioSfx_StopByPosAndId(&this->picto.actor.projectedPos, NA_SE_EN_PIRATE_BREATH);
    this->action = KAIZOKU_ACTION_SLASH;
    this->actionFunc = EnKaizoku_Slash;
}

void EnKaizoku_Slash(EnKaizoku* this, PlayState* play2) {
    PlayState* play = play2;
    f32 curFrame;
    s16 rotYDelta;
    s16 rotYDelta2;
    Player* player = GET_PLAYER(play2);

    curFrame = this->skelAnime.curFrame;

    rotYDelta = ABS_ALT(player->actor.shape.rot.y - this->picto.actor.shape.rot.y);
    rotYDelta2 = ABS_ALT(this->picto.actor.yawTowardsPlayer - this->picto.actor.shape.rot.y);
    this->picto.actor.speed = 0.0f;

    if (Animation_OnFrame(&this->skelAnime, 1.0f)) {
        Actor_PlaySfx(&this->picto.actor, NA_SE_EN_PIRATE_ATTACK);
        this->swordState = 1;
    } else if (Animation_OnFrame(&this->skelAnime, 6.0f)) {
        this->swordState = -1;
    }

    if (this->swordCollider.base.atFlags & AT_BOUNCED) {
        this->swordState = -1;
        this->swordCollider.base.atFlags &= ~(AT_BOUNCED | AT_HIT);
        EnKaizoku_SetupRollBack(this);
        return;
    }

    this->animationsDisabled = false;
    if (curFrame >= this->animEndFrame) {
        if (!Actor_IsFacingPlayer(&this->picto.actor, 0x1554)) {
            EnKaizoku_SetupReady(this);

            if (rotYDelta2 > 0x4000) {
                this->lookTimer = 20;
            }
        } else {
            if ((Rand_ZeroOne() > 0.7f) || (this->picto.actor.xzDistToPlayer >= 120.0f)) {
                EnKaizoku_SetupReady(this);
                return;
            }

            this->picto.actor.world.rot.y = this->picto.actor.yawTowardsPlayer;

            if (BREG(12) * 0.1f + 0.01f * 40.0f < Rand_ZeroOne()) {
                EnKaizoku_SetupBlock(this);
            } else if (rotYDelta <= 0x2710) {
                if (rotYDelta2 > 0x4000) {
                    this->picto.actor.world.rot.y = this->picto.actor.yawTowardsPlayer;
                    EnKaizoku_SetupCircle(this);
                } else {
                    EnKaizoku_ReactToPlayer(this, play, true);
                }
            } else {
                EnKaizoku_SetupCircle(this);
            }
        }
    }
}

void EnKaizoku_SetupRollForward(EnKaizoku* this) {
    EnKaizoku_ChangeAnim(this, KAIZOKU_ANIM_FLIP);
    this->combatTimer = 0;
    this->picto.actor.speed = 10.0f;
    this->picto.actor.world.rot.y = this->picto.actor.shape.rot.y = this->picto.actor.yawTowardsPlayer;
    Actor_PlaySfx(&this->picto.actor, NA_SE_EN_TEKU_JUMP);
    this->action = KAIZOKU_ACTION_ROLL_FORWARD;
    this->actionFunc = EnKaizoku_RollForward;
}

void EnKaizoku_RollForward(EnKaizoku* this, PlayState* play) {
    f32 curFrame = this->skelAnime.curFrame;

    this->animationsDisabled = false;
    if (curFrame >= this->animEndFrame) {
        this->picto.actor.speed = 0.0f;
        if (!Actor_IsFacingPlayer(&this->picto.actor, 0x1554)) {
            EnKaizoku_SetupReady(this);
            this->combatTimer = Rand_ZeroOne() * 5.0f + 5.0f;
        } else {
            EnKaizoku_SetupSlash(this);
        }
    }
    if (!(play->gameplayFrames & 0x5F)) {
        Actor_PlaySfx(&this->picto.actor, NA_SE_EN_PIRATE_BREATH);
    }
}

void EnKaizoku_SetupAdvance(EnKaizoku* this) {
    EnKaizoku_ChangeAnim(this, KAIZOKU_ANIM_WALK);
    this->action = KAIZOKU_ACTION_ADVANCE;
    this->actionFunc = EnKaizoku_Advance;
}

void EnKaizoku_Advance(EnKaizoku* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 beforeCurFrame;
    s32 afterCurFrame;
    s16 yawDiff;

    if (!EnKaizoku_DodgeRanged(this, play)) {
        Math_SmoothStepToS(&this->picto.actor.shape.rot.y, this->picto.actor.yawTowardsPlayer, 1, 0x2EE, 0);
        this->picto.actor.world.rot.y = this->picto.actor.shape.rot.y;
        if (this->picto.actor.xzDistToPlayer <= 40.0f) {
            Math_ApproachF(&this->picto.actor.speed, -8.0f, 1.0f, 1.5f);
        } else if (this->picto.actor.xzDistToPlayer > 55.0f) {
            Math_ApproachF(&this->picto.actor.speed, 8.0f, 1.0f, 1.5f);
        } else {
            Math_ApproachZeroF(&this->picto.actor.speed, 2.0f, 6.65f);
        }

        this->skelAnime.playSpeed = 1.0f;
        yawDiff = ABS_ALT(player->actor.shape.rot.y - this->picto.actor.shape.rot.y);
        if ((this->picto.actor.xzDistToPlayer < 150.0f) && (player->meleeWeaponState != PLAYER_MELEE_WEAPON_STATE_0) &&
            (yawDiff >= 0x2000)) {
            this->picto.actor.shape.rot.y = this->picto.actor.world.rot.y = this->picto.actor.yawTowardsPlayer;
            if (Rand_ZeroOne() > 0.7f) {
                EnKaizoku_SetupCircle(this);
                return;
            }
        }

        beforeCurFrame = this->skelAnime.curFrame - this->skelAnime.playSpeed;
        afterCurFrame = this->skelAnime.curFrame + this->skelAnime.playSpeed;
        if (!Actor_IsFacingPlayer(&this->picto.actor, 0x11C7)) {
            if (Rand_ZeroOne() > 0.5f) {
                EnKaizoku_SetupCircle(this);
            } else {
                EnKaizoku_SetupReady(this);
            }
        } else if (this->picto.actor.xzDistToPlayer < 90.0f) {
            if ((Rand_ZeroOne() > 0.03f) || ((this->picto.actor.xzDistToPlayer <= 65.0f) && (yawDiff < 0x4000))) {
                EnKaizoku_SetupSlash(this);
            } else if (Actor_OtherIsTargeted(play, &this->picto.actor) && (Rand_ZeroOne() > 0.5f)) {
                EnKaizoku_SetupRollBack(this);
            } else {
                EnKaizoku_SetupCircle(this);
            }
        }

        if (!EnKaizoku_ReactToPlayer(this, play, false)) {
            if ((this->picto.actor.xzDistToPlayer < 210.0f) && (this->picto.actor.xzDistToPlayer > 150.0f) &&
                Actor_IsFacingPlayer(&this->picto.actor, 0x1388)) {
                if (Actor_IsTargeted(play, &this->picto.actor)) {
                    if (Rand_ZeroOne() > 0.5f) {
                        EnKaizoku_SetupRollForward(this);
                    } else {
                        EnKaizoku_SetupSpinAttack(this);
                    }
                } else {
                    EnKaizoku_SetupCircle(this);
                    return;
                }
            }

            if (!(play->gameplayFrames & 0x5F)) {
                Actor_PlaySfx(&this->picto.actor, NA_SE_EN_PIRATE_BREATH);
            }

            if ((this->skelAnime.curFrame != 0.0f) &&
                (((beforeCurFrame < 0) && (afterCurFrame > 0)) || ((beforeCurFrame < 4) && (afterCurFrame >= 5)))) {
                Actor_PlaySfx(&this->picto.actor, NA_SE_EN_GERUDOFT_WALK);
            }
        }
    }
}

void EnKaizoku_SetupPivot(EnKaizoku* this) {
    EnKaizoku_ChangeAnim(this, KAIZOKU_ANIM_SIDESTEP);
    this->action = KAIZOKU_ACTION_PIVOT;
    this->actionFunc = EnKaizoku_Pivot;
}

void EnKaizoku_Pivot(EnKaizoku* this, PlayState* play) {
    s16 angleToPlayer;
    s16 turnRate;

    if (!EnKaizoku_DodgeRanged(this, play) && !EnKaizoku_ReactToPlayer(this, play, false)) {
        angleToPlayer = this->picto.actor.yawTowardsPlayer - this->picto.actor.shape.rot.y;
        if (angleToPlayer > 0) {
            turnRate = TRUNCF_BINANG(angleToPlayer * 0.25f) + 0x7D0;
        } else {
            turnRate = TRUNCF_BINANG(angleToPlayer * 0.25f) - 0x7D0;
        }
        this->picto.actor.shape.rot.y += turnRate;
        this->picto.actor.world.rot.y = this->picto.actor.shape.rot.y;
        this->skelAnime.playSpeed = 1.0f;

        if (Actor_IsFacingPlayer(&this->picto.actor, 0x1388)) {
            if (Rand_ZeroOne() > 0.8f) {
                EnKaizoku_SetupCircle(this);
            } else {
                EnKaizoku_SetupAdvance(this);
            }
        }
        if (!(play->gameplayFrames & 0x5F)) {
            Actor_PlaySfx(&this->picto.actor, NA_SE_EN_PIRATE_BREATH);
        }
    }
}

void EnKaizoku_SetupSpinAttack(EnKaizoku* this) {
    if (!FighterCanAttack(&this->picto.actor, 400.f))
    {
        if (this->action != KAIZOKU_ACTION_READY)
            EnKaizoku_SetupReady(this);
        this->swordState = -1;
        this->swordCollider.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
        return;
    }

    EnKaizoku_ChangeAnim(this, KAIZOKU_ANIM_SPIN_ATTACK);
    this->swordCollider.base.atFlags &= ~(AT_BOUNCED | AT_HIT);
    this->spinAttackState = 0;
    this->action = KAIZOKU_ACTION_SPIN_ATTACK;
    this->actionFunc = EnKaizoku_SpinAttack;
    this->picto.actor.speed = 0.0f;
}

void EnKaizoku_SpinAttack(EnKaizoku* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    f32 curFrame = this->skelAnime.curFrame;
    s16 yawDiff;
    s16 angleToPlayer;

    if (this->spinAttackState < 2) {
        if (this->swordCollider.base.atFlags & AT_BOUNCED) {
            this->swordCollider.base.atFlags &= ~(AT_BOUNCED | AT_HIT);
            this->spinAttackState = 1;
            this->skelAnime.playSpeed = 1.0f;
        }

    }

    if (this->skelAnime.curFrame <= 8.0f) {
        this->picto.actor.shape.rot.y = this->picto.actor.world.rot.y = this->picto.actor.yawTowardsPlayer;
    } else if (Animation_OnFrame(&this->skelAnime, 13.0f)) {
        Actor_SpawnFloorDustRing(play, &this->picto.actor, &this->leftFootPos, 3.0f, 2, 2.0f, 0, 0, false);
        Actor_SpawnFloorDustRing(play, &this->picto.actor, &this->rightFootPos, 3.0f, 2, 2.0f, 0, 0, false);
        this->swordState = 1;
        this->picto.actor.speed = 10.0f;
        Actor_PlaySfx(&this->picto.actor, NA_SE_EN_PIRATE_ATTACK);
    } else if (Animation_OnFrame(&this->skelAnime, 21.0f)) {
        this->picto.actor.speed = 0.0f;
    } else if (Animation_OnFrame(&this->skelAnime, 24.0f)) {
        this->swordState = -1;
    }

    this->animationsDisabled = false;
    if ((curFrame >= this->animEndFrame) && (this->spinAttackState < 2)) {
        if (!Actor_IsFacingPlayer(&this->picto.actor, 0x1554)) {
            EnKaizoku_SetupReady(this);
            this->combatTimer = Rand_ZeroOne() * 5.0f + 5.0f;
            this->lookTimer = 46;
            return;
        }

        if (this->spinAttackState != 0) {
            EnKaizoku_SetupRollBack(this);
            return;
        }

        if ((Rand_ZeroOne() > 0.7f) || (this->picto.actor.xzDistToPlayer >= 120.0f)) {
            EnKaizoku_SetupReady(this);
            this->combatTimer = Rand_ZeroOne() * 5.0f + 5.0f;
            return;
        }

        this->picto.actor.world.rot.y = this->picto.actor.yawTowardsPlayer;
        if (BREG(12) * 0.1f + 0.01f * 40.0f < Rand_ZeroOne()) {
            EnKaizoku_SetupBlock(this);
            return;
        }

        angleToPlayer = ABS_ALT(player->actor.shape.rot.y - this->picto.actor.shape.rot.y);
        if (angleToPlayer <= 0x2710) {
            yawDiff = ABS_ALT(this->picto.actor.yawTowardsPlayer - this->picto.actor.shape.rot.y);
            if (yawDiff > 0x4000) {
                this->picto.actor.world.rot.y = this->picto.actor.yawTowardsPlayer;
                EnKaizoku_SetupCircle(this);
            } else {
                EnKaizoku_ReactToPlayer(this, play, true);
            }
        } else {
            EnKaizoku_SetupCircle(this);
        }
    }
}

void EnKaizoku_SetupCircle(EnKaizoku* this) {
    EnKaizoku_ChangeAnim(this, KAIZOKU_ANIM_SIDESTEP);
    this->picto.actor.speed = Rand_CenteredFloat(12.0f);
    this->skelAnime.playSpeed = 1.0f;
    this->picto.actor.world.rot.y = this->picto.actor.shape.rot.y;
    this->combatTimer = Rand_ZeroOne() * 30.0f + 30.0f;
    this->action = KAIZOKU_ACTION_CIRCLE;
    this->actionFunc = EnKaizoku_Circle;
    this->circlingRate = 0.0f;
}

void EnKaizoku_Circle(EnKaizoku* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 angleBehindPlayer;
    s32 beforeCurFrame;
    s32 afterCurFrame;
    s16 yaw;

    Math_SmoothStepToS(&this->picto.actor.shape.rot.y, this->picto.actor.yawTowardsPlayer, 1, 0xFA0, 1);
    if (!EnKaizoku_DodgeRanged(this, play) && !EnKaizoku_ReactToPlayer(this, play, false)) {
        this->picto.actor.world.rot.y = this->picto.actor.shape.rot.y + 0x4000;
        angleBehindPlayer = player->actor.shape.rot.y + 0x8000;
        if (Math_SinS(angleBehindPlayer - this->picto.actor.shape.rot.y) >= 0.0f) {
            this->picto.actor.speed -= 0.25f;
            if (this->picto.actor.speed < -8.0f) {
                this->picto.actor.speed = -8.0f;
            }
        } else if (Math_SinS(angleBehindPlayer - this->picto.actor.shape.rot.y) < 0.0f) {
            this->picto.actor.speed += 0.25f;
            if (this->picto.actor.speed > 8.0f) {
                this->picto.actor.speed = 8.0f;
            }
        }

        if ((this->picto.actor.bgCheckFlags & BGCHECKFLAG_WALL) ||
            !Actor_TestFloorInDirection(&this->picto.actor, play, this->picto.actor.speed,
                                        this->picto.actor.shape.rot.y + 0x4000)) {

            if (this->picto.actor.bgCheckFlags & BGCHECKFLAG_WALL) {
                if (this->picto.actor.speed >= 0.0f) {
                    yaw = this->picto.actor.shape.rot.y + 0x4000;
                } else {
                    yaw = this->picto.actor.shape.rot.y - 0x4000;
                }
                yaw = this->picto.actor.wallYaw - yaw;
            } else {
                this->picto.actor.speed *= -0.8f;
                yaw = 0;
            }

            if (ABS_ALT(yaw) > 0x4000) {
                this->picto.actor.speed *= -0.8f;
                if (this->picto.actor.speed < 0.0f) {
                    this->picto.actor.speed -= 0.5f;
                } else {
                    this->picto.actor.speed += 0.5f;
                }
            }
        }

        if (this->picto.actor.xzDistToPlayer <= 65.0f) {
            Math_ApproachF(&this->circlingRate, -4.0f, 1.0f, 1.5f);
        } else if (this->picto.actor.xzDistToPlayer > 40.0f) {
            Math_ApproachF(&this->circlingRate, 4.0f, 1.0f, 1.5f);
        } else {
            Math_ApproachZeroF(&this->circlingRate, 1.0f, 6.65f);
        }

        if (this->circlingRate != 0.0f) {
            this->picto.actor.world.pos.x += Math_SinS(this->picto.actor.shape.rot.y) * this->circlingRate;
            this->picto.actor.world.pos.z += Math_CosS(this->picto.actor.shape.rot.y) * this->circlingRate;
        }

        beforeCurFrame = this->skelAnime.curFrame - this->skelAnime.playSpeed;
        afterCurFrame = this->skelAnime.curFrame + this->skelAnime.playSpeed;
        if (this->skelAnime.curFrame != 0.0f) {
            if ((((beforeCurFrame < 0) && (afterCurFrame > 0)) || ((beforeCurFrame < 5) && (afterCurFrame > 5)))) {
                Actor_PlaySfx(&this->picto.actor, NA_SE_EN_GERUDOFT_WALK);
            }
        }

        if (!(play->gameplayFrames & 0x5F)) {
            Actor_PlaySfx(&this->picto.actor, NA_SE_EN_PIRATE_BREATH);
        }

        if (this->picto.actor.xzDistToPlayer <= 65.0f) {
            EnKaizoku_SetupSlash(this);
        } else if (this->combatTimer == 0) {
            if (Actor_OtherIsTargeted(play, &this->picto.actor) && (Rand_ZeroOne() > 0.5f)) {
                EnKaizoku_SetupRollBack(this);
            } else {
                EnKaizoku_SetupReady(this);
            }
        }
    }
}

void EnKaizoku_SetupStunned(EnKaizoku* this) {
    if (this->picto.actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
        this->picto.actor.speed = 0.0f;
    }

    if (this->action == KAIZOKU_ACTION_SPIN_ATTACK) {
        EnKaizoku_ChangeAnim(this, KAIZOKU_ANIM_DAMAGE);
    }

    if (((this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_SFX) ||
         (this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX)) &&
        (this->drawDmgEffTimer == 0)) {
        this->drawDmgEffTimer = 0;
        this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FIRE;
    }

    Actor_PlaySfx(&this->picto.actor, NA_SE_EN_COMMON_FREEZE);
    this->animationsDisabled = true;

    if ((this->drawDmgEffType != ACTOR_DRAW_DMGEFF_FROZEN_SFX) &&
        (this->drawDmgEffType != ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX)) {
        this->iceTimer = 40;
    }

    this->action = KAIZOKU_ACTION_STUNNED;
    this->actionFunc = EnKaizoku_Stunned;
}

void EnKaizoku_Stunned(EnKaizoku* this, PlayState* play) {
    if (this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_SFX) {
        if ((this->drawDmgEffTimer != 0) && (this->drawDmgEffTimer < 60)) {
            this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX;
        }
    }

    if (this->picto.actor.bgCheckFlags & BGCHECKFLAG_GROUND_TOUCH) {
        this->picto.actor.speed = 0.0f;
    }

    if (this->picto.actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
        if (this->picto.actor.speed < 0.0f) {
            this->picto.actor.speed += 0.05f;
        }
    }

    if ((this->iceTimer == 0) && (this->drawDmgEffTimer == 0) &&
        !CHECK_FLAG_ALL(this->picto.actor.flags, ACTOR_FLAG_HOOKSHOT_ATTACHED) &&
        (this->picto.actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
        this->animationsDisabled = false;
        EnKaizoku_ReactToPlayer(this, play, true);

        if ((this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_SFX) ||
            (this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX)) {
            Actor_SpawnIceEffects(play, &this->picto.actor, this->bodyPartsPos, KAIZOKU_BODYPART_MAX, 2, 0.7f, 0.4f);
            this->drawDmgEffTimer = 0;
            this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FIRE;
            this->picto.actor.flags |= ACTOR_FLAG_HOOKSHOT_PULLS_PLAYER;
        }
    }
}

void EnKaizoku_SetupDamaged(EnKaizoku* this, PlayState* play) {
    Vec3f velocity;

    Matrix_RotateYS(this->picto.actor.yawTowardsPlayer, MTXMODE_NEW);
    Matrix_MultVecZ(-10.0f, &velocity);
    Math_Vec3f_Copy(&this->velocity, &velocity);
    this->lookTimer = 0;
    this->animationsDisabled = false;
    this->picto.actor.speed = 0.0f;
    EnKaizoku_ChangeAnim(this, KAIZOKU_ANIM_DAMAGE);

    if (((this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_SFX) ||
         (this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX)) &&
        (this->drawDmgEffTimer != 0)) {
        Actor_SpawnIceEffects(play, &this->picto.actor, this->bodyPartsPos, KAIZOKU_BODYPART_MAX, 2, 0.7f, 0.4f);
        this->drawDmgEffTimer = 0;
        this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FIRE;
        this->picto.actor.flags |= ACTOR_FLAG_HOOKSHOT_PULLS_PLAYER;
    }

    Actor_PlaySfx(&this->picto.actor, NA_SE_EN_PIRATE_DAMAGE);
    this->action = KAIZOKU_ACTION_DAMAGED;
    this->actionFunc = EnKaizoku_Damaged;
}

void EnKaizoku_Damaged(EnKaizoku* this, PlayState* play) {
    s16 angleToWall;

    Math_SmoothStepToS(&this->picto.actor.shape.rot.y, this->picto.actor.yawTowardsPlayer, 1, 0x1194, 0);
    if (!EnKaizoku_DodgeRanged(this, play) && !EnKaizoku_ReactToPlayer(this, play, false) &&
        (this->velocity.x < 1.0f) && (this->velocity.z < 1.0f)) {
        angleToWall = this->picto.actor.wallYaw - this->picto.actor.shape.rot.y;
        if (this->picto.actor.bgCheckFlags & BGCHECKFLAG_WALL) {
            if (ABS_ALT(angleToWall) < 0x3000 && (this->picto.actor.xzDistToPlayer < 90.0f)) {
                EnKaizoku_SetupJump(this);
                return;
            }
        }

        if ((this->picto.actor.xzDistToPlayer <= 65.0f) && ((play->gameplayFrames % 8) != 0)) {
            this->swordCollider.elem.elemMaterial = ELEM_MATERIAL_UNK2;
            EnKaizoku_SetupSlash(this);
        } else {
            EnKaizoku_SetupRollBack(this);
        }
    }
}

void EnKaizoku_SetupDefeatKnockdown(EnKaizoku* this, PlayState* play) {
    Vec3f sp24;

    this->picto.actor.flags &= ~ACTOR_FLAG_FREEZE_EXCEPTION;
    Matrix_RotateYS(this->picto.actor.yawTowardsPlayer, MTXMODE_NEW);
    Matrix_MultVecZ(-10.0f, &sp24);
    Math_Vec3f_Copy(&this->velocity, &sp24);
    EnKaizoku_ChangeAnim(this, KAIZOKU_ANIM_DEFEAT);

    if (((this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_SFX) ||
         (this->drawDmgEffType == ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX)) &&
        (this->drawDmgEffTimer == 0)) {
        this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FIRE;
    }
    this->picto.actor.speed = 0.0f;
    this->animationsDisabled = false;
    this->combatTimer = 60;

    Enemy_StartFinishingBlow(play, &this->picto.actor);
    Actor_PlaySfx(&this->picto.actor, NA_SE_EN_PIRATE_DEAD);
    this->picto.actor.flags |= ACTOR_FLAG_LOCK_ON_DISABLED;
    this->picto.actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    this->picto.actor.flags &= ~ACTOR_FLAG_HOOKSHOT_PULLS_PLAYER;
    this->cutsceneTimer = 0;
    this->cutsceneState = 0;
    this->action = KAIZOKU_ACTION_KNOCK_DOWN;
    this->actionFunc = EnKaizoku_DefeatKnockdown;
}

void EnKaizoku_DefeatKnockdown(EnKaizoku* this, PlayState* play) {
    if (!this->combatTimer)
        Actor_Kill(&this->picto.actor);
}

void EnKaizoku_UpdateDamage(EnKaizoku* this, PlayState* play) {
    s32 wasHit = false;

    this->swordCollider.elem.atDmgInfo.damage = 4;

    if (!(this->swordCollider.base.atFlags & AT_BOUNCED) && (this->swordCollider.base.atFlags & AT_HIT))
    {
        this->swordCollider.base.atFlags &= ~AT_HIT;
        if (this->action == KAIZOKU_ACTION_SPIN_ATTACK && this->swordCollider.base.at == &GET_PLAYER(play)->actor)
        {
            ((void (*)(PlayState*,Actor*,f32,s16,f32))(void*)FOREIGN_ENEMY_API_KNOCKBACK_LARGE_NO_DAMAGE)(
                play, &this->picto.actor, 3.f, this->picto.actor.yawTowardsPlayer, 1.f);
            Health_ChangeBy(play, -0xc);
            this->spinAttackState = 1;
        }
    }

    if ((this->picto.actor.bgCheckFlags & BGCHECKFLAG_WALL) && (this->picto.actor.wallBgId != BG_ACTOR_MAX) &&
        ((this->action == KAIZOKU_ACTION_SPIN_DODGE) || (this->action == KAIZOKU_ACTION_CIRCLE) ||
         (this->action == KAIZOKU_ACTION_ADVANCE) || (this->action == KAIZOKU_ACTION_READY))) {
        EnKaizoku_SetupSlash(this);
    }

    if ((this->bodyCollider.base.acFlags & AC_HIT) && (this->action > KAIZOKU_ACTION_HIDDEN) &&
        (this->spinAttackState < 2) && (this->action != KAIZOKU_ACTION_JUMP) &&
        (this->action != KAIZOKU_ACTION_BLOCK) && (this->action != KAIZOKU_ACTION_DAMAGED) &&
        (this->action != KAIZOKU_ACTION_KNOCK_DOWN)) {
        Actor_SetDropFlag(&this->picto.actor, &this->bodyCollider.elem);
        AudioSfx_StopByPosAndId(&this->picto.actor.projectedPos, NA_SE_EN_PIRATE_BREATH);

        switch (this->picto.actor.colChkInfo.damageEffect) {
            case KAIZOKU_DMGEFF_ZORA_SHIELD:
                if (((this->drawDmgEffType != ACTOR_DRAW_DMGEFF_FROZEN_SFX) &&
                     (this->drawDmgEffType != ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX)) ||
                    (this->drawDmgEffTimer == 0)) {
                    this->drawDmgEffTimer = 40;
                    this->drawDmgEffType = ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_MEDIUM;
                }
                FALLTHROUGH;
            case KAIZOKU_DMGEFF_STUN:
                if (((this->drawDmgEffType != ACTOR_DRAW_DMGEFF_FROZEN_SFX) &&
                     (this->drawDmgEffType != ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX)) ||
                    this->drawDmgEffTimer == 0) {
                    Actor_SetColorFilter(&this->picto.actor, COLORFILTER_COLORFLAG_BLUE, 120, COLORFILTER_BUFFLAG_OPA,
                                         40);
                    this->bodyCollider.elem.elemMaterial = ELEM_MATERIAL_UNK1;
                    this->bodyCollider.base.colMaterial = COL_MATERIAL_HIT3;
                    this->swordCollider.elem.elemMaterial = ELEM_MATERIAL_UNK2;
                    EnKaizoku_SetupStunned(this);
                }
                break;

            case KAIZOKU_DMGEFF_IFRAME_PROTECTED:
                if ((this->action != KAIZOKU_ACTION_ROLL_FORWARD) && (this->action != KAIZOKU_ACTION_ROLL_BACK) &&
                    (this->action < KAIZOKU_ACTION_DAMAGED)) {
                    wasHit = true;
                }
                break;

            case KAIZOKU_DMGEFF_ALWAYS_HIT:
                wasHit = true;
                break;

            case KAIZOKU_DMGEFF_STUNNED_ONLY:
                if (this->action == KAIZOKU_ACTION_STUNNED) {
                    wasHit = true;
                } else if (this->action != KAIZOKU_ACTION_JUMP) {
                    EnKaizoku_SetupJump(this);
                }
                break;

            case KAIZOKU_DMGEFF_FIRE_ARROW:
                if ((this->action != KAIZOKU_ACTION_ROLL_FORWARD) && (this->action != KAIZOKU_ACTION_ROLL_BACK) &&
                    (this->action < KAIZOKU_ACTION_DAMAGED)) {
                    this->drawDmgEffTimer = 40;
                    this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FIRE;
                    wasHit = true;
                }
                break;

            case KAIZOKU_DMGEFF_FREEZE:
                if (((this->drawDmgEffType != ACTOR_DRAW_DMGEFF_FROZEN_SFX) &&
                     (this->drawDmgEffType != ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX)) ||
                    (this->drawDmgEffTimer == 0)) {
                    Actor_ApplyDamage(&this->picto.actor);
                    this->bodyCollider.elem.elemMaterial = ELEM_MATERIAL_UNK1;
                    this->bodyCollider.base.colMaterial = COL_MATERIAL_HIT3;
                    this->swordCollider.elem.elemMaterial = ELEM_MATERIAL_UNK4;
                    this->drawDmgEffTimer = 80;
                    this->drawDmgEffType = ACTOR_DRAW_DMGEFF_FROZEN_SFX;
                    this->drawDmgEffScale = 0.0f;
                    this->drawDmgEffFrozenSteamScale = 1.5f;
                    this->picto.actor.flags &= ~ACTOR_FLAG_HOOKSHOT_PULLS_PLAYER;
                    if (this->picto.actor.colChkInfo.health <= 0) {
                        EnKaizoku_SetupDefeatKnockdown(this, play);
                    } else {
                        EnKaizoku_SetupStunned(this);
                    }
                }
                break;

            case KAIZOKU_DMGEFF_LIGHT_ARROW:
                if (((this->drawDmgEffType != ACTOR_DRAW_DMGEFF_FROZEN_SFX) &&
                     (this->drawDmgEffType != ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX)) ||
                    (this->drawDmgEffTimer == 0)) {
                    this->drawDmgEffTimer = 20;
                    this->drawDmgEffType = ACTOR_DRAW_DMGEFF_LIGHT_ORBS;
                    Actor_Spawn(&play->actorCtx, play, ACTOR_EN_CLEAR_TAG, this->picto.actor.focus.pos.x,
                                this->picto.actor.focus.pos.y, this->picto.actor.focus.pos.z, 0, 0, 0,
                                CLEAR_TAG_PARAMS(CLEAR_TAG_LARGE_LIGHT_RAYS));
                    wasHit = true;
                }
                break;

            default:
                break;
        }

        if (wasHit) {
            Vec3f bloodPos;
            s32 i;

            Actor_SetColorFilter(&this->picto.actor, COLORFILTER_COLORFLAG_RED, 255, COLORFILTER_BUFFLAG_OPA, 8);
            Actor_ApplyDamage(&this->picto.actor);
            if (this->picto.actor.colChkInfo.health <= 0) {
                EnKaizoku_SetupDefeatKnockdown(this, play);
                return;
            }

            this->bodyCollider.elem.elemMaterial = ELEM_MATERIAL_UNK1;
            this->bodyCollider.base.colMaterial = COL_MATERIAL_HIT3;
            this->swordCollider.elem.elemMaterial = ELEM_MATERIAL_UNK4;
            Math_Vec3f_Copy(&bloodPos, &this->picto.actor.focus.pos);
            for (i = 0; i < 3; i++) {
                bloodPos.y += Rand_ZeroFloat(20.0f);
                CollisionCheck_BlueBlood(play, NULL, &bloodPos);
            }

            EnKaizoku_SetupDamaged(this, play);
        }
    } else if (this->bodyCollider.base.acFlags & AC_HIT) {
        if ((this->action != KAIZOKU_ACTION_JUMP) && (this->action != KAIZOKU_ACTION_DAMAGED) &&
            (this->action != KAIZOKU_ACTION_KNOCK_DOWN)) {
            Vec3f pos;

            pos.x = this->bodyCollider.elem.acDmgInfo.hitPos.x;
            pos.y = this->bodyCollider.elem.acDmgInfo.hitPos.y;
            pos.z = this->bodyCollider.elem.acDmgInfo.hitPos.z;

            this->bodyCollider.base.acFlags &= ~AC_HIT;
            Actor_PlaySfx(&this->picto.actor, NA_SE_IT_SHIELD_BOUND);
            EffectSsHitmark_SpawnFixedScale(play, EFFECT_HITMARK_METAL, &pos);
            CollisionCheck_SpawnShieldParticlesMetal(play, &pos);
        }
    }
}

static Vec3f sFootOffset = { 300.0f, 0.0f, 0.0f };

static Vec3f sSwordTipOffset = { 0.0f, -3000.0f, 0.0f };
static Vec3f sSwordHiltOffset = { 400.0f, 0.0f, 0.0f };

static Vec3f sSwordQuadOffset1 = { 1600.0f, -4000.0f, 0.0f };
static Vec3f sSwordQuadOffset0 = { -3000.0f, -2000.0f, 1300.0f };
static Vec3f sSwordQuadOffset3 = { -3000.0f, -2000.0f, -1300.0f };
static Vec3f sSwordQuadOffset2 = { 1000.0f, 1000.0f, 0.0f };

static TexturePtr sEyeTextures[] = {
    gFighterPirateEyeOpenTex,
    gFighterPirateEyeHalfTex,
    gFighterPirateEyeClosedTex,
    gFighterPirateEyeHalfTex,
};

void EnKaizoku_Update(Actor* thisx, PlayState* play2) {
    EnKaizoku* this = (EnKaizoku*)thisx;
    PlayState* play = play2;
    EnemyGroundGuard ground;
    comboEnemyGroundGuardBeginNative(thisx, &ground);
    if (this->action == KAIZOKU_ACTION_READY &&
        (thisx->xzDistToPlayer > 600.f || fabsf(thisx->yDistanceFromLink) >= 50.f))
        this->lookTimer = 2;

    if (!this->animationsDisabled) {
        SkelAnime_Update(&this->skelAnime);
    }

    if (this->action != KAIZOKU_ACTION_HIDDEN) {
        EnKaizoku_TurnHead(this);
    }

    DECR(this->blinkTimer);
    DECR(this->combatTimer);
    DECR(this->lookTimer);
    DECR(this->iceTimer);
    DECR(this->cutsceneTimer);
    DECR(this->drawDmgEffTimer);

    this->actionFunc(this, play);
    Actor_MoveWithGravity(&this->picto.actor);
    if (this->action != KAIZOKU_ACTION_HIDDEN) {
        EnKaizoku_UpdateDamage(this, play);
    }
    if (this->action != KAIZOKU_ACTION_HIDDEN) {
        Actor_SetFocus(&this->picto.actor, 60.0f);
    }

    Actor_SetScale(&this->picto.actor, 0.0125f);
    if (this->blinkTimer == 0) {
        this->eyeIndex++;
        if (this->eyeIndex >= ARRAY_COUNT(sEyeTextures)) {
            this->eyeIndex = 0;
            this->blinkTimer = Rand_S16Offset(20, 60);
        }
    }

    if (this->picto.actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
        Vec3f bounceVelocity;

        Matrix_RotateYS(this->picto.actor.shape.rot.y + this->boyoBounceAngle, MTXMODE_NEW);
        Matrix_MultVecZ(this->boyoBounceVelocity, &bounceVelocity);
        this->picto.actor.world.pos.x += this->velocity.x + bounceVelocity.x;
        this->picto.actor.world.pos.z += this->velocity.z + bounceVelocity.z;
        Math_ApproachZeroF(&this->velocity.x, 1.0f, 2.0f);
        Math_ApproachZeroF(&this->velocity.z, 1.0f, 2.0f);
        Math_ApproachZeroF(&this->boyoBounceVelocity, 1.0f, 5.0f);
    }

    Actor_UpdateBgCheckInfo(play, &this->picto.actor, 35.0f, 40.0f, 35.0f,
                            UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_2 | UPDBGCHECKINFO_FLAG_4 |
                                UPDBGCHECKINFO_FLAG_8 | UPDBGCHECKINFO_FLAG_10);
    Collider_UpdateCylinder(&this->picto.actor, &this->bodyCollider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->bodyCollider.base);
    if ((this->spinAttackState < 2) && (this->action != KAIZOKU_ACTION_HIDDEN)) {
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->bodyCollider.base);
    }
    if (this->swordState > 0) {
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->swordCollider.base);
    }
    comboEnemyGroundGuardEnd(play, &ground);
}

s32 EnKaizoku_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx) {
    EnKaizoku* this = (EnKaizoku*)thisx;

    OPEN_DISPS(play->state.gfxCtx);

    if (limbIndex == KAIZOKU_LIMB_VEIL) {
        *dList = NULL;
    }

    if (limbIndex == KAIZOKU_LIMB_NECK) {
        rot->z += this->headRot.x;
        rot->x += this->headRot.y;
        rot->y += this->headRot.z;
    } else if (limbIndex == KAIZOKU_LIMB_HEAD) {
        gDPPipeSync(POLY_OPA_DISP++);
        gDPSetEnvColor(POLY_OPA_DISP++, sKaizokuLipstickColors[this->colorType].r,
                       sKaizokuLipstickColors[this->colorType].g, sKaizokuLipstickColors[this->colorType].b, 255);
    } else {
        gDPPipeSync(POLY_OPA_DISP++);
        gDPSetEnvColor(POLY_OPA_DISP++, sKaizokuOutfitColors[this->colorType].r,
                       sKaizokuOutfitColors[this->colorType].g, sKaizokuOutfitColors[this->colorType].b, 255);
    }

    CLOSE_DISPS();

    return false;
}

void EnKaizoku_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx) {
    Vec3f swordTip;
    Vec3f swordHilt;
    EnKaizoku* this = (EnKaizoku*)thisx;

    if (limbIndex == KAIZOKU_LIMB_RIGHT_SWORD) {
        Matrix_MultVec3f(&sSwordQuadOffset1, &this->swordCollider.dim.quad[1]);
        Matrix_MultVec3f(&sSwordQuadOffset0, &this->swordCollider.dim.quad[0]);
        Matrix_MultVec3f(&sSwordQuadOffset3, &this->swordCollider.dim.quad[3]);
        Matrix_MultVec3f(&sSwordQuadOffset2, &this->swordCollider.dim.quad[2]);
        Collider_SetQuadVertices(&this->swordCollider, &this->swordCollider.dim.quad[0],
                                 &this->swordCollider.dim.quad[1], &this->swordCollider.dim.quad[2],
                                 &this->swordCollider.dim.quad[3]);
        Matrix_MultVec3f(&sSwordTipOffset, &swordTip);
        Matrix_MultVec3f(&sSwordHiltOffset, &swordHilt);

        if ((this->swordState == 1) &&
            ((this->action == KAIZOKU_ACTION_SLASH) || (this->action == KAIZOKU_ACTION_SPIN_ATTACK))) {
            FighterBlureVertex(Effect_GetByIndex(this->blureIndex), &swordTip, &swordHilt);
        } else if (this->swordState != 0) {
            FighterBlureSpace(Effect_GetByIndex(this->blureIndex));
            this->swordState = 0;
        }
    } else {
        Actor_SetFeetPos(&this->picto.actor, limbIndex, KAIZOKU_LIMB_LEFT_FOOT, &sFootOffset, KAIZOKU_LIMB_RIGHT_FOOT,
                         &sFootOffset);
    }

    if (limbIndex == KAIZOKU_LIMB_LEFT_FOOT) {
        Matrix_MultVec3f(&sFootOffset, &this->leftFootPos);
    } else if (limbIndex == KAIZOKU_LIMB_RIGHT_FOOT) {
        Matrix_MultVec3f(&sFootOffset, &this->rightFootPos);
    }

    if ((limbIndex == KAIZOKU_LIMB_TORSO) || (limbIndex == KAIZOKU_LIMB_HEAD) ||
        (limbIndex == KAIZOKU_LIMB_RIGHT_UPPER_ARM) || (limbIndex == KAIZOKU_LIMB_RIGHT_FOREARM) ||
        (limbIndex == KAIZOKU_LIMB_RIGHT_HAND) || (limbIndex == KAIZOKU_LIMB_LEFT_UPPER_ARM) ||
        (limbIndex == KAIZOKU_LIMB_LEFT_FOREARM) || (limbIndex == KAIZOKU_LIMB_LEFT_HAND) ||
        (limbIndex == KAIZOKU_LIMB_LEFT_THIGH) || (limbIndex == KAIZOKU_LIMB_LEFT_SHIN) ||
        (limbIndex == KAIZOKU_LIMB_LEFT_FOOT) || (limbIndex == KAIZOKU_LIMB_RIGHT_THIGH) ||
        (limbIndex == KAIZOKU_LIMB_RIGHT_SHIN) || (limbIndex == KAIZOKU_LIMB_RIGHT_FOOT) ||
        (limbIndex == KAIZOKU_LIMB_WAIST)) {

        Matrix_MultZero(&this->bodyPartsPos[this->bodyPartIndex]);
        this->bodyPartIndex++;
        if (this->bodyPartIndex >= KAIZOKU_BODYPART_MAX) {
            this->bodyPartIndex = 0;
        }
    }
}

void EnKaizoku_TransformLimbDraw(PlayState* play, s32 limbIndex, Actor* thisx) {
    EnKaizoku* this = (EnKaizoku*)thisx;

    if (limbIndex == KAIZOKU_LIMB_RIGHT_SWORD) {
        Matrix_Scale(this->swordScaleRight.x, this->swordScaleRight.y, this->swordScaleRight.z, MTXMODE_APPLY);
    }
    if (limbIndex == KAIZOKU_LIMB_LEFT_SWORD) {
        Matrix_Scale(this->swordScaleLeft.x, this->swordScaleLeft.y, this->swordScaleLeft.z, MTXMODE_APPLY);
    }
}

void EnKaizoku_Draw(Actor* thisx, PlayState* play) {

    f32 drawDmgEffAlpha;
    EnKaizoku* this = (EnKaizoku*)thisx;

    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    Gfx_SetupDL25_Opa(play->state.gfxCtx);

    OPEN_DISPS(play->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_K0(sEyeTextures[this->eyeIndex]));
    SkelAnime_DrawTransformFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable,
                                   this->skelAnime.dListCount, EnKaizoku_OverrideLimbDraw, EnKaizoku_PostLimbDraw,
                                   EnKaizoku_TransformLimbDraw, &this->picto.actor);

    if (this->drawDmgEffTimer != 0) {
        drawDmgEffAlpha = this->drawDmgEffTimer * 0.05f;

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

        Actor_DrawDamageEffects(play, &this->picto.actor, this->bodyPartsPos, KAIZOKU_BODYPART_MAX,
                                this->drawDmgEffScale, this->drawDmgEffFrozenSteamScale, drawDmgEffAlpha,
                                this->drawDmgEffType);
    }

    CLOSE_DISPS();
}

OVL_INFO_NATIVE_ENEMY(0x21d, En_Kaizoku_Profile);
EXPORT_ENEMY_PROFILE(FOREIGN_ENEMY_PORT_MM_EN_KAIZOKU, En_Kaizoku_Profile, EnKaizoku_Init, EnKaizoku_Destroy, EnKaizoku_Update, EnKaizoku_Draw);

static const u32 FighterNativeOrigin[] = { 0x4e505431, 1, 0x21d, 0x204, 0xee195826u };
EXPORT_SYMBOL(FOREIGN_ENEMY_PORT_MM_EN_KAIZOKU_ORIGIN, FighterNativeOrigin);
