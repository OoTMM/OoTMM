#include <combo/enemy_flight.h>
#include <combo/enemy_gomess_bat.h>
#include <combo.h>
#include <combo/matrix.h>
#include <combo/effect.h>
#include <combo/common/actors/En_Item00.h>
#include <combo/sfx_source.h>
#include <combo/quake.h>
#include <combo/enemy_keep.h>
#include <combo/enemy_snapper_mount.h>
#include <combo/enemy_minislime.h>
#include <combo/enemy_actor_layout.h>

void Actor_SpawnFloorDustRing(PlayState*, Actor*, Vec3f*, f32, s32, f32, s16, s16, u8);
#if defined(GAME_OOT)
void Scene_SetRenderModeXlu(PlayState*, s32, u32);
#endif

enum { DUEL_GARO, DUEL_GOMESS, DUEL_WART, DUEL_SNAPPER, DUEL_SLIME, DUEL_IGOS, DUEL_KEETA };
enum {
    DUEL_GARO_LIMBS = 20,
    DUEL_GOMESS_LIMBS = 22,
    DUEL_WART_LIMBS = 9,
    DUEL_SNAPPER_LIMBS = 24,
    DUEL_SLIME_LIMBS = 24,
    DUEL_IGOS_LIMBS = 29,
    DUEL_KEETA_LIMBS = 21,
    DUEL_LIMBS = DUEL_FIXED_FORM == DUEL_GARO      ? DUEL_GARO_LIMBS
                 : DUEL_FIXED_FORM == DUEL_GOMESS  ? DUEL_GOMESS_LIMBS
                 : DUEL_FIXED_FORM == DUEL_WART    ? DUEL_WART_LIMBS
                 : DUEL_FIXED_FORM == DUEL_SNAPPER ? DUEL_SNAPPER_LIMBS
                 : DUEL_FIXED_FORM == DUEL_SLIME   ? DUEL_SLIME_LIMBS
                 : DUEL_FIXED_FORM == DUEL_IGOS    ? DUEL_IGOS_LIMBS
                                                   : DUEL_KEETA_LIMBS,
};

typedef struct {
    Actor actor;
    SkelAnime skel;
    Vec3s joints[DUEL_LIMBS];
    Vec3s morph[DUEL_LIMBS];
    ColliderCylinder body;
    ColliderCylinder weapon;
    u8 shield;
    u8 phase;
    u8 timer;
    u8 exposed;
    u8 walking;
    u8 chaseHasPoint;
    u16 specialCooldown;
    u8 headCooldown;
    u8 breathAlpha;
    s16 headYaw;
    s16 headPitch;
    f32 headSpeed;
    Vec3f headPos;
    Vec3f chasePoint;
} StandaloneDuel;

typedef struct {
    StandaloneDuel duel;
    ColliderJntSph bodySpheres;
    ColliderJntSphElement bodyElements[7];
    u32 bodyPoseFrame;
    u8 bodyPoseMask;
    u8 footPoseMask;
    Vec3f footPos[2];
    u8 laughed;
    u8 defeatStage;
    s16 defeatSpin;
    Vec3f recoil;
    u8 damageEffect;
    u8 damageTimer;
} StandaloneKeeta;

enum {
    GEKKO_DAMAGE_STUN = 1,
    GEKKO_DAMAGE_FIRE,
    GEKKO_DAMAGE_ICE,
    GEKKO_DAMAGE_LIGHT,
    GEKKO_DAMAGE_ELECTRIC,
    GEKKO_DAMAGE_DEKU = 13,
    GEKKO_DAMAGE_HOOK,
    GEKKO_DAMAGE_BREAK
};

typedef struct {
    Vec3f points[12];
    u32 poseFrame;
    u16 pointMask;
    s16 footY;
    u8 timer;
    u8 stun;
    u8 type;
    f32 scale;
    f32 steam;
    f32 recoilSpeed;
    s16 recoilYaw;
} GekkoDamage;

typedef struct {
    StandaloneDuel duel;
    GekkoDamage damage;
    ColliderJntSph body;
    ColliderJntSphElement elements[2];
    Vec3f climbNormal;
    Vec3f climbForward;
    u8 mountPhase;
    u8 callMountAfterRecoil;
    u8 wallHit;
    s16 wallSpinYaw;
} StandaloneSnapper;

enum { IGOS_BREATH_PUFFS = 100 };
enum { IGOS_PUFF_NEW, IGOS_PUFF_RISING, IGOS_PUFF_SPREAD };

typedef struct {
    Vec3f pos;
    Vec3f velocity;
    f32 scale;
    u8 alpha;
    u8 age;
    u8 motion;
} IgosBreathPuff;

typedef struct {
    Vec3f pos;
    s16 yaw, pitch, jaw;
    u8 alpha;
} IgosHeadImage;

typedef struct {
    Vec3f pos;
    Vec3s rot;
    Vec3s joints[29];
    u8 alpha;
} IgosBodyImage;

typedef struct {
    Actor actor;
    SkelAnime headSkel;
    Vec3s headJoints[29], headMorph[29];
    ColliderCylinder headCollider;
    ColliderCylinder headAttackCollider;
    IgosHeadImage headTrail[7];
    EnemyFlightReturn returnRoute;
    Vec3f chasePoint;
    f32 headSpeed;
    f32 headDiveStep;
    s16 headYaw, headPitch, headJaw;
    s16 headTravelYaw, headTravelPitch;
    u8 headTrailNext;
    u8 headRetargetTimer;
    u8 headAction;
    u8 headTimer;
    u8 headBiteBlur;
    u8 headHitCooldown;
    u16 randTimer;
} IgosFlyingHead;

typedef struct {
    StandaloneDuel duel;
    IgosBreathPuff puffs[IGOS_BREATH_PUFFS];
    IgosBodyImage recoveryTrail[7];
    u8 recoveryTrailNext;
    ColliderJntSph sword, shield, bodySpheres;
    ColliderJntSphElement swordElement, shieldElement, bodyElements[2];
    u32 bodyPoseFrame;
    u8 bodyPoseMask;
    u32 swordPoseFrame, shieldPoseFrame;
    u8 hasSwordPose, hasShieldPose;
    Actor* headTarget;
    Vec3f breathOrigin;
    Vec3f headSocketOffset;
    u32 headSocketFrame;
    s16 neckYaw;
    u16 randTimerOffset;
    u8 hasHeadSocket;
    u8 hasBreathOrigin;
    u8 breathHitTimer;
    u8 fallenHitTimer;
    u8 shieldInvulnerability;
    u8 shieldFxTimer;
    u8 swordShieldContact;
    u8 damageInvulnerability;
    u8 fallenBackwards;
    Vec3f animDrift;
    u8 animDriftDelay;
    u8 swordBlurAlpha;
    u8 swordBlurFrame;
    u8 swordBlurLow;
    u8 bodyAlpha;
    u8 shadowAlphaFactor;
    u8 damageFlashTimer;
    u8 specialBackjump;
    s16 strafeTarget;
    s16 strafeMaxStep;
    s16 strafeNeckTarget;
    s16 waitNeckTarget;
    s16 breathNeckTarget;
    Vec3f retreatTarget;
    Vec3f shieldFlashPos;
    f32 shieldFlashScale;
    u8 retreatMode;
    u8 retreatShield;
    u8 blockFromRetreat;
    u8 lightRayTimer;
    u8 knightEncounter;
    u8 knightsSpawned;
    Vec3f footPos[2];
    u32 footPoseFrame;
    u8 footPoseMask;
    u32 fallenPoseFrame;
    u16 fallenPoseMask;
    Vec3f fallenParts[15];
    f32 elementalAlpha;
    f32 elementalScale;
    f32 iceSteamScale;
    u8 elementalType;
    u8 elementalTimer;
} StandaloneIgos;

_Static_assert(sizeof(IgosFlyingHead) <= sizeof(StandaloneIgos), "Igos actor profile must fit the flying head");

typedef struct {
    StandaloneDuel duel;
    ColliderQuad swords[2];
    s32 trails[2];
    u32 poseFrame[2];
    u8 poseMask;
    u8 trailMask;
    Vec3f damagePoints[12];
    f32 damageScale, steamScale;
    u8 damageEffect, damageTimer, damagePointCount;
    Vec3f bombPos;
    u32 bombPoseFrame;
    u8 bombPoseValid;
    u8 dashAudio;
    u8 flameScroll;
    u8 swordsDrawn;
    f32 flameScale[3];
    Vec3f knockback;
    s16 circleAngle, circleStep;
    u8 afterimageIndex, afterimageCount;
    struct {
        Vec3f pos;
        Vec3s rot;
        Vec3s joints[20];
    } afterimages[4];
} StandaloneGaroMaster;

enum {
    SLIME_FRAGMENT_LOOK = 97,
    SLIME_FRAGMENT_NOTICE,
    SLIME_FRAGMENT_THROW,
    SLIME_FORM,
    SLIME_RISE,
    SLIME_CEILING,
    SLIME_FALL,
    SLIME_GRAB,
    SLIME_FROZEN,
    SLIME_GROUND,
    SLIME_THROW
};
enum { SLIME_VERTEX_COUNT = 162 };
enum { SLIME_GRAB_STRUGGLE_PRESSES = 12 };
enum { SLIME_HOOK_PULLS_ACTOR = 1u << 9, SLIME_HOOK_ATTACHED = 1u << 13 };
typedef struct {
    Vec3f pos, velocity;
    Vec3s rot;
    f32 scale;
} SlimeIceShard;
enum { SLIME_ICE_MELT_SFX = 0x2944, SLIME_OPEN_AIR_HEIGHT = 600 };
typedef struct {
    StandaloneDuel duel;
    GekkoDamage damage;
    EnemyMinislime* fragments[MINISLIME_COUNT];
    f32 ceiling;
    f32 riseVelocity;
    u16 ceilingMoveTimer;
    u8 dropWarningActive;
    u8 cycle;
    u8 jumpRounds;
    s16 groundYaw;
    s16 shellGekkoPitch;
    u8 grabRounds;
    u8 grabSwings;
    u8 grabMove;
    u8 grabImpact;
    u8 grabStruggle;
    u8 grabReverb;
    s16 grabYaw;
    f32 grabOffset;
    f32 throwPerturbation[SLIME_VERTEX_COUNT];
    f32 riseStart, riseScaleX, riseScaleY, dropScaleZ;
    f32 shellScaleXZ, shellScaleY, shellOffsetY;
    u8 riseCounter;
    u8 throwRecovery;
    u8 waitingForShatterLanding;
    u8 squishRecovery;
    u8 waterRippleActive;
    u8 itemDropCooldown;
    Vec3f hookSafePos;
    u8 formFragments;
    u8 throwFragment;
    u8 frozenOnGround;
    u8 shellGrounded;
    u8 frozenReady;
    u8 frozenMelting;
    u8 frozenFallHit;
    s16 frozenMinY, frozenMaxY;
    u8 preFreezePhase;
    u8 preFreezeTimer;
    Vec3s throwMesh[SLIME_VERTEX_COUNT];
    u8 iceAlpha[SLIME_VERTEX_COUNT];
    Vtx shellMesh[SLIME_VERTEX_COUNT];
    f32 waveAmplitude[(SLIME_VERTEX_COUNT + 5) / 6];
    u8 waveTimer;
    u8 wavePhase;
    SlimeIceShard iceShards[SLIME_VERTEX_COUNT];
    Vec3f freezeAnchor;
    Vec3f shockwavePos;
    f32 shockwaveScale;
    u8 shockwaveAlpha;
#if defined(GAME_OOT)
    struct {
        Vec3f pos;
        u8 life;
    } meltSmoke[25];
#endif

    ColliderCylinder shell[12];
} StandaloneSlime;

static const u8 sSlimeTriangles[][3] = {
    { 0, 2, 1 },       { 1, 7, 6 },       { 1, 2, 7 },       { 2, 8, 7 },       { 6, 17, 16 },     { 6, 7, 17 },
    { 8, 19, 18 },     { 7, 8, 18 },      { 7, 18, 17 },     { 9, 21, 20 },     { 0, 3, 2 },       { 2, 9, 8 },
    { 2, 3, 9 },       { 3, 10, 9 },      { 8, 20, 19 },     { 8, 9, 20 },      { 10, 22, 21 },    { 9, 10, 21 },
    { 0, 4, 3 },       { 3, 11, 10 },     { 3, 4, 11 },      { 4, 12, 11 },     { 10, 23, 22 },    { 10, 11, 23 },
    { 12, 25, 24 },    { 11, 12, 24 },    { 11, 24, 23 },    { 13, 27, 26 },    { 0, 5, 4 },       { 4, 13, 12 },
    { 4, 5, 13 },      { 5, 14, 13 },     { 12, 26, 25 },    { 12, 13, 26 },    { 14, 28, 27 },    { 13, 14, 27 },
    { 0, 1, 5 },       { 5, 15, 14 },     { 5, 1, 15 },      { 1, 6, 15 },      { 14, 29, 28 },    { 14, 15, 29 },
    { 6, 16, 30 },     { 15, 6, 30 },     { 15, 30, 29 },    { 16, 32, 31 },    { 16, 17, 32 },    { 17, 33, 32 },
    { 17, 18, 33 },    { 18, 34, 33 },    { 18, 19, 34 },    { 19, 35, 34 },    { 19, 20, 36 },    { 20, 37, 36 },
    { 20, 21, 37 },    { 21, 38, 37 },    { 21, 22, 38 },    { 22, 39, 38 },    { 19, 36, 35 },    { 22, 40, 39 },
    { 22, 23, 40 },    { 23, 41, 40 },    { 23, 24, 41 },    { 24, 42, 41 },    { 24, 25, 42 },    { 25, 43, 42 },
    { 25, 26, 44 },    { 26, 45, 44 },    { 26, 27, 45 },    { 27, 46, 45 },    { 27, 28, 46 },    { 28, 47, 46 },
    { 25, 44, 43 },    { 28, 48, 47 },    { 28, 29, 48 },    { 29, 49, 48 },    { 29, 30, 49 },    { 30, 50, 49 },
    { 30, 16, 50 },    { 16, 31, 50 },    { 31, 32, 51 },    { 32, 52, 51 },    { 32, 33, 52 },    { 33, 53, 52 },
    { 33, 34, 53 },    { 34, 54, 53 },    { 34, 35, 54 },    { 35, 55, 54 },    { 35, 36, 55 },    { 36, 56, 55 },
    { 36, 37, 56 },    { 37, 57, 56 },    { 37, 38, 57 },    { 38, 58, 57 },    { 38, 39, 58 },    { 39, 59, 58 },
    { 39, 40, 59 },    { 40, 60, 59 },    { 40, 41, 60 },    { 41, 61, 60 },    { 41, 42, 61 },    { 42, 62, 61 },
    { 42, 43, 62 },    { 43, 63, 62 },    { 51, 72, 71 },    { 51, 52, 72 },    { 52, 73, 72 },    { 52, 53, 73 },
    { 53, 74, 73 },    { 53, 54, 74 },    { 54, 75, 74 },    { 54, 55, 75 },    { 55, 76, 75 },    { 55, 56, 76 },
    { 56, 77, 76 },    { 56, 57, 77 },    { 57, 78, 77 },    { 57, 58, 78 },    { 58, 79, 78 },    { 58, 59, 79 },
    { 59, 80, 79 },    { 59, 60, 80 },    { 60, 81, 80 },    { 60, 61, 81 },    { 61, 82, 81 },    { 61, 62, 82 },
    { 62, 83, 82 },    { 62, 63, 83 },    { 71, 72, 91 },    { 72, 92, 91 },    { 72, 73, 92 },    { 73, 93, 92 },
    { 73, 74, 93 },    { 74, 94, 93 },    { 74, 75, 94 },    { 75, 95, 94 },    { 75, 76, 95 },    { 76, 96, 95 },
    { 76, 77, 96 },    { 77, 97, 96 },    { 77, 78, 97 },    { 78, 98, 97 },    { 78, 79, 98 },    { 79, 99, 98 },
    { 79, 80, 99 },    { 80, 100, 99 },   { 80, 81, 100 },   { 81, 101, 100 },  { 81, 82, 101 },   { 82, 102, 101 },
    { 82, 83, 102 },   { 83, 103, 102 },  { 91, 112, 111 },  { 91, 92, 112 },   { 92, 113, 112 },  { 92, 93, 113 },
    { 93, 114, 113 },  { 93, 94, 114 },   { 94, 115, 114 },  { 94, 95, 115 },   { 95, 116, 115 },  { 95, 96, 116 },
    { 96, 117, 116 },  { 96, 97, 117 },   { 97, 118, 117 },  { 97, 98, 118 },   { 98, 119, 118 },  { 98, 99, 119 },
    { 99, 120, 119 },  { 99, 100, 120 },  { 100, 121, 120 }, { 100, 101, 121 }, { 101, 122, 121 }, { 101, 102, 122 },
    { 102, 123, 122 }, { 102, 103, 123 }, { 43, 44, 63 },    { 44, 64, 63 },    { 44, 45, 64 },    { 45, 65, 64 },
    { 45, 46, 65 },    { 46, 66, 65 },    { 46, 47, 66 },    { 47, 67, 66 },    { 47, 48, 67 },    { 48, 68, 67 },
    { 48, 49, 68 },    { 49, 69, 68 },    { 49, 50, 69 },    { 50, 70, 69 },    { 50, 31, 70 },    { 31, 51, 70 },
    { 63, 84, 83 },    { 63, 64, 84 },    { 64, 85, 84 },    { 64, 65, 85 },    { 65, 86, 85 },    { 65, 66, 86 },
    { 66, 87, 86 },    { 66, 67, 87 },    { 67, 88, 87 },    { 67, 68, 88 },    { 68, 89, 88 },    { 68, 69, 89 },
    { 69, 90, 89 },    { 69, 70, 90 },    { 70, 71, 90 },    { 70, 51, 71 },    { 83, 84, 103 },   { 84, 104, 103 },
    { 84, 85, 104 },   { 85, 105, 104 },  { 85, 86, 105 },   { 86, 106, 105 },  { 86, 87, 106 },   { 87, 107, 106 },
    { 87, 88, 107 },   { 88, 108, 107 },  { 88, 89, 108 },   { 89, 109, 108 },  { 89, 90, 109 },   { 90, 110, 109 },
    { 90, 71, 110 },   { 71, 91, 110 },   { 103, 124, 123 }, { 103, 104, 124 }, { 104, 125, 124 }, { 104, 105, 125 },
    { 105, 126, 125 }, { 105, 106, 126 }, { 106, 127, 126 }, { 106, 107, 127 }, { 107, 128, 127 }, { 107, 108, 128 },
    { 108, 129, 128 }, { 108, 109, 129 }, { 109, 130, 129 }, { 109, 110, 130 }, { 110, 111, 130 }, { 110, 91, 111 },
    { 161, 160, 156 }, { 155, 146, 160 }, { 146, 156, 160 }, { 146, 147, 156 }, { 144, 145, 155 }, { 145, 146, 155 },
    { 145, 131, 146 }, { 131, 147, 146 }, { 131, 132, 147 }, { 134, 135, 149 }, { 161, 156, 157 }, { 147, 148, 156 },
    { 148, 157, 156 }, { 148, 149, 157 }, { 132, 133, 147 }, { 133, 148, 147 }, { 133, 134, 148 }, { 134, 149, 148 },
    { 161, 157, 158 }, { 149, 150, 157 }, { 150, 158, 157 }, { 150, 151, 158 }, { 135, 136, 149 }, { 136, 150, 149 },
    { 136, 137, 150 }, { 137, 151, 150 }, { 137, 138, 151 }, { 140, 141, 153 }, { 161, 158, 159 }, { 151, 152, 158 },
    { 152, 159, 158 }, { 152, 153, 159 }, { 138, 139, 151 }, { 139, 152, 151 }, { 139, 140, 152 }, { 140, 153, 152 },
    { 161, 159, 160 }, { 153, 154, 159 }, { 154, 160, 159 }, { 154, 155, 160 }, { 141, 142, 153 }, { 142, 154, 153 },
    { 142, 143, 154 }, { 143, 155, 154 }, { 143, 144, 155 }, { 111, 131, 145 }, { 111, 112, 131 }, { 112, 132, 131 },
    { 112, 113, 132 }, { 113, 114, 132 }, { 114, 133, 132 }, { 114, 115, 133 }, { 115, 116, 134 }, { 116, 135, 134 },
    { 116, 117, 135 }, { 117, 118, 135 }, { 118, 136, 135 }, { 118, 119, 136 }, { 115, 134, 133 }, { 119, 137, 136 },
    { 119, 120, 137 }, { 120, 138, 137 }, { 120, 121, 138 }, { 121, 122, 138 }, { 122, 139, 138 }, { 122, 123, 139 },
    { 139, 123, 140 }, { 140, 123, 124 }, { 140, 124, 141 }, { 141, 124, 125 }, { 141, 125, 126 }, { 141, 126, 142 },
    { 142, 127, 143 }, { 143, 127, 128 }, { 143, 128, 144 }, { 144, 128, 129 }, { 144, 129, 130 }, { 144, 130, 145 },
    { 142, 126, 127 }, { 130, 111, 145 },
};

static const Vtx sGekkoSlimeVertices[162] = {
    { .n = { { 0, 1000, 0 }, 0, { 512, -512 }, { 0, 120, 0 }, 50 } },
    { .n = { { 0, 962, -273 }, 0, { 512, -473 }, { 0, 115, -33 }, 50 } },
    { .n = { { 260, 962, -84 }, 0, { 778, -473 }, { 31, 115, -10 }, 50 } },
    { .n = { { 161, 962, 221 }, 0, { 676, -473 }, { 19, 115, 27 }, 50 } },
    { .n = { { -161, 962, 221 }, 0, { 348, -473 }, { -19, 115, 27 }, 50 } },
    { .n = { { -260, 962, -84 }, 0, { 246, -473 }, { -31, 115, -10 }, 50 } },
    { .n = { { 0, 851, -526 }, 0, { 512, -359 }, { 0, 102, -63 }, 50 } },
    { .n = { { 263, 894, -362 }, 0, { 781, -404 }, { 32, 107, -43 }, 50 } },
    { .n = { { 500, 851, -162 }, 0, { 1024, -359 }, { 60, 102, -19 }, 50 } },
    { .n = { { 425, 894, 138 }, 0, { 948, -404 }, { 51, 107, 17 }, 50 } },
    { .n = { { 309, 851, 425 }, 0, { 828, -359 }, { 37, 102, 51 }, 50 } },
    { .n = { { 0, 894, 447 }, 0, { 512, -404 }, { 0, 107, 54 }, 50 } },
    { .n = { { -309, 851, 425 }, 0, { 196, -359 }, { -37, 102, 51 }, 50 } },
    { .n = { { -425, 894, 138 }, 0, { 76, -404 }, { -51, 107, 17 }, 50 } },
    { .n = { { -500, 851, -162 }, 0, { 0, -359 }, { -60, 102, -19 }, 50 } },
    { .n = { { -263, 894, -362 }, 0, { 243, -404 }, { -32, 107, -43 }, 50 } },
    { .n = { { 0, 675, -738 }, 0, { 512, -179 }, { 0, 81, -89 }, 50 } },
    { .n = { { 263, 724, -638 }, 0, { 781, -229 }, { 32, 87, -77 }, 50 } },
    { .n = { { 526, 724, -447 }, 0, { 1050, -229 }, { 63, 87, -54 }, 50 } },
    { .n = { { 702, 675, -228 }, 0, { 1231, -179 }, { 84, 81, -27 }, 50 } },
    { .n = { { 688, 724, 53 }, 0, { 1217, -229 }, { 83, 87, 6 }, 50 } },
    { .n = { { 588, 724, 362 }, 0, { 1114, -229 }, { 71, 87, 43 }, 50 } },
    { .n = { { 434, 675, 597 }, 0, { 956, -179 }, { 52, 81, 72 }, 50 } },
    { .n = { { 162, 724, 671 }, 0, { 678, -229 }, { 19, 87, 80 }, 50 } },
    { .n = { { -162, 724, 671 }, 0, { 346, -229 }, { -19, 87, 80 }, 50 } },
    { .n = { { -434, 675, 597 }, 0, { 68, -179 }, { -52, 81, 72 }, 50 } },
    { .n = { { -588, 724, 362 }, 0, { -90, -229 }, { -71, 87, 43 }, 50 } },
    { .n = { { -688, 724, 53 }, 0, { -193, -229 }, { -83, 87, 6 }, 50 } },
    { .n = { { -702, 675, -228 }, 0, { -207, -179 }, { -84, 81, -27 }, 50 } },
    { .n = { { -526, 724, -447 }, 0, { -26, -229 }, { -63, 87, -54 }, 50 } },
    { .n = { { -263, 724, -638 }, 0, { 243, -229 }, { -32, 87, -77 }, 50 } },
    { .n = { { 0, 447, -894 }, 0, { 512, 54 }, { 0, 54, -107 }, 50 } },
    { .n = { { 260, 506, -823 }, 0, { 778, -6 }, { 31, 61, -99 }, 50 } },
    { .n = { { 500, 526, -688 }, 0, { 1024, -26 }, { 60, 63, -83 }, 50 } },
    { .n = { { 702, 506, -501 }, 0, { 1231, -6 }, { 84, 61, -60 }, 50 } },
    { .n = { { 851, 447, -276 }, 0, { 1383, 54 }, { 102, 54, -33 }, 50 } },
    { .n = { { 863, 506, -7 }, 0, { 1395, -6 }, { 104, 61, -1 }, 50 } },
    { .n = { { 809, 526, 263 }, 0, { 1340, -26 }, { 97, 63, 32 }, 50 } },
    { .n = { { 694, 506, 513 }, 0, { 1222, -6 }, { 83, 61, 62 }, 50 } },
    { .n = { { 526, 447, 724 }, 0, { 1050, 54 }, { 63, 54, 87 }, 50 } },
    { .n = { { 273, 506, 818 }, 0, { 792, -6 }, { 33, 61, 98 }, 50 } },
    { .n = { { 0, 526, 851 }, 0, { 512, -26 }, { 0, 63, 102 }, 50 } },
    { .n = { { -273, 506, 818 }, 0, { 232, -6 }, { -33, 61, 98 }, 50 } },
    { .n = { { -526, 447, 724 }, 0, { -26, 54 }, { -63, 54, 87 }, 50 } },
    { .n = { { -694, 506, 513 }, 0, { -198, -6 }, { -83, 61, 62 }, 50 } },
    { .n = { { -809, 526, 263 }, 0, { -316, -26 }, { -97, 63, 32 }, 50 } },
    { .n = { { -863, 506, -7 }, 0, { -371, -6 }, { -104, 61, -1 }, 50 } },
    { .n = { { -851, 447, -276 }, 0, { -359, 54 }, { -102, 54, -33 }, 50 } },
    { .n = { { -702, 506, -501 }, 0, { -207, -6 }, { -84, 61, -60 }, 50 } },
    { .n = { { -500, 526, -688 }, 0, { 0, -26 }, { -60, 63, -83 }, 50 } },
    { .n = { { -260, 506, -823 }, 0, { 246, -6 }, { -31, 61, -99 }, 50 } },
    { .n = { { 161, 232, -959 }, 0, { 676, 274 }, { 19, 28, -115 }, 50 } },
    { .n = { { 425, 276, -862 }, 0, { 948, 229 }, { 51, 33, -103 }, 50 } },
    { .n = { { 688, 276, -671 }, 0, { 1217, 229 }, { 83, 33, -80 }, 50 } },
    { .n = { { 863, 232, -449 }, 0, { 1395, 274 }, { 104, 28, -54 }, 50 } },
    { .n = { { 962, 232, -144 }, 0, { 1497, 274 }, { 115, 28, -17 }, 50 } },
    { .n = { { 951, 276, 138 }, 0, { 1486, 229 }, { 114, 33, 17 }, 50 } },
    { .n = { { 851, 276, 447 }, 0, { 1383, 229 }, { 102, 33, 54 }, 50 } },
    { .n = { { 694, 232, 682 }, 0, { 1222, 274 }, { 83, 28, 82 }, 50 } },
    { .n = { { 434, 232, 870 }, 0, { 956, 274 }, { 52, 28, 104 }, 50 } },
    { .n = { { 162, 276, 947 }, 0, { 678, 229 }, { 19, 33, 114 }, 50 } },
    { .n = { { -162, 276, 947 }, 0, { 346, 229 }, { -19, 33, 114 }, 50 } },
    { .n = { { -434, 232, 870 }, 0, { 68, 274 }, { -52, 28, 104 }, 50 } },
    { .n = { { -694, 232, 682 }, 0, { -198, 274 }, { -83, 28, 82 }, 50 } },
    { .n = { { -851, 276, 447 }, 0, { -359, 229 }, { -102, 33, 54 }, 50 } },
    { .n = { { -951, 276, 138 }, 0, { -462, 229 }, { -114, 33, 17 }, 50 } },
    { .n = { { -962, 232, -144 }, 0, { -473, 274 }, { -115, 28, -17 }, 50 } },
    { .n = { { -863, 232, -449 }, 0, { -371, 274 }, { -104, 28, -54 }, 50 } },
    { .n = { { -688, 276, -671 }, 0, { -193, 229 }, { -83, 33, -80 }, 50 } },
    { .n = { { -425, 276, -862 }, 0, { 76, 229 }, { -51, 33, -103 }, 50 } },
    { .n = { { -161, 232, -959 }, 0, { 348, 274 }, { -19, 28, -115 }, 50 } },
    { .n = { { 0, 0, -1000 }, 0, { 512, 512 }, { 0, 0, -120 }, 50 } },
    { .n = { { 309, 0, -951 }, 0, { 828, 512 }, { 37, 0, -114 }, 50 } },
    { .n = { { 588, 0, -809 }, 0, { 1114, 512 }, { 71, 0, -97 }, 50 } },
    { .n = { { 809, 0, -588 }, 0, { 1340, 512 }, { 97, 0, -71 }, 50 } },
    { .n = { { 951, 0, -309 }, 0, { 1486, 512 }, { 114, 0, -37 }, 50 } },
    { .n = { { 1000, 0, 0 }, 0, { 1536, 512 }, { 120, 0, 0 }, 50 } },
    { .n = { { 951, 0, 309 }, 0, { 1486, 512 }, { 114, 0, 37 }, 50 } },
    { .n = { { 809, 0, 588 }, 0, { 1340, 512 }, { 97, 0, 71 }, 50 } },
    { .n = { { 588, 0, 809 }, 0, { 1114, 512 }, { 71, 0, 97 }, 50 } },
    { .n = { { 309, 0, 951 }, 0, { 828, 512 }, { 37, 0, 114 }, 50 } },
    { .n = { { 0, 0, 1000 }, 0, { 512, 512 }, { 0, 0, 120 }, 50 } },
    { .n = { { -309, 0, 951 }, 0, { 196, 512 }, { -37, 0, 114 }, 50 } },
    { .n = { { -588, 0, 809 }, 0, { -90, 512 }, { -71, 0, 97 }, 50 } },
    { .n = { { -809, 0, 588 }, 0, { -316, 512 }, { -97, 0, 71 }, 50 } },
    { .n = { { -951, 0, 309 }, 0, { -462, 512 }, { -114, 0, 37 }, 50 } },
    { .n = { { -1000, 0, 0 }, 0, { -512, 512 }, { -120, 0, 0 }, 50 } },
    { .n = { { -951, 0, -309 }, 0, { -462, 512 }, { -114, 0, -37 }, 50 } },
    { .n = { { -809, 0, -588 }, 0, { -316, 512 }, { -97, 0, -71 }, 50 } },
    { .n = { { -588, 0, -809 }, 0, { -90, 512 }, { -71, 0, -97 }, 50 } },
    { .n = { { -309, 0, -951 }, 0, { 196, 512 }, { -37, 0, -114 }, 50 } },
    { .n = { { 162, -276, -947 }, 0, { 678, 795 }, { 19, -33, -114 }, 50 } },
    { .n = { { 434, -232, -870 }, 0, { 956, 750 }, { 52, -28, -104 }, 50 } },
    { .n = { { 694, -232, -682 }, 0, { 1222, 750 }, { 83, -28, -82 }, 50 } },
    { .n = { { 851, -276, -447 }, 0, { 1383, 795 }, { 102, -33, -54 }, 50 } },
    { .n = { { 951, -276, -138 }, 0, { 1486, 795 }, { 114, -33, -17 }, 50 } },
    { .n = { { 962, -232, 144 }, 0, { 1497, 750 }, { 115, -28, 17 }, 50 } },
    { .n = { { 863, -232, 449 }, 0, { 1395, 750 }, { 104, -28, 54 }, 50 } },
    { .n = { { 688, -276, 671 }, 0, { 1217, 795 }, { 83, -33, 80 }, 50 } },
    { .n = { { 425, -276, 862 }, 0, { 948, 795 }, { 51, -33, 103 }, 50 } },
    { .n = { { 161, -232, 959 }, 0, { 676, 750 }, { 19, -28, 115 }, 50 } },
    { .n = { { -161, -232, 959 }, 0, { 348, 750 }, { -19, -28, 115 }, 50 } },
    { .n = { { -425, -276, 862 }, 0, { 76, 795 }, { -51, -33, 103 }, 50 } },
    { .n = { { -688, -276, 671 }, 0, { -193, 795 }, { -83, -33, 80 }, 50 } },
    { .n = { { -863, -232, 449 }, 0, { -371, 750 }, { -104, -28, 54 }, 50 } },
    { .n = { { -962, -232, 144 }, 0, { -473, 750 }, { -115, -28, 17 }, 50 } },
    { .n = { { -951, -276, -138 }, 0, { -462, 795 }, { -114, -33, -17 }, 50 } },
    { .n = { { -851, -276, -447 }, 0, { -359, 795 }, { -102, -33, -54 }, 50 } },
    { .n = { { -694, -232, -682 }, 0, { -198, 750 }, { -83, -28, -82 }, 50 } },
    { .n = { { -434, -232, -870 }, 0, { 68, 750 }, { -52, -28, -104 }, 50 } },
    { .n = { { -162, -276, -947 }, 0, { 346, 795 }, { -19, -33, -114 }, 50 } },
    { .n = { { 0, -526, -851 }, 0, { 512, 1050 }, { 0, -63, -102 }, 50 } },
    { .n = { { 273, -506, -818 }, 0, { 792, 1030 }, { 33, -61, -98 }, 50 } },
    { .n = { { 526, -447, -724 }, 0, { 1050, 970 }, { 63, -54, -87 }, 50 } },
    { .n = { { 694, -506, -513 }, 0, { 1222, 1030 }, { 83, -61, -62 }, 50 } },
    { .n = { { 809, -526, -263 }, 0, { 1340, 1050 }, { 97, -63, -32 }, 50 } },
    { .n = { { 863, -506, 7 }, 0, { 1395, 1030 }, { 104, -61, 1 }, 50 } },
    { .n = { { 851, -447, 276 }, 0, { 1383, 970 }, { 102, -54, 33 }, 50 } },
    { .n = { { 702, -506, 501 }, 0, { 1231, 1030 }, { 84, -61, 60 }, 50 } },
    { .n = { { 500, -526, 688 }, 0, { 1024, 1050 }, { 60, -63, 83 }, 50 } },
    { .n = { { 260, -506, 823 }, 0, { 778, 1030 }, { 31, -61, 99 }, 50 } },
    { .n = { { 0, -447, 894 }, 0, { 512, 970 }, { 0, -54, 107 }, 50 } },
    { .n = { { -260, -506, 823 }, 0, { 246, 1030 }, { -31, -61, 99 }, 50 } },
    { .n = { { -500, -526, 688 }, 0, { 0, 1050 }, { -60, -63, 83 }, 50 } },
    { .n = { { -702, -506, 501 }, 0, { -207, 1030 }, { -84, -61, 60 }, 50 } },
    { .n = { { -851, -447, 276 }, 0, { -359, 970 }, { -102, -54, 33 }, 50 } },
    { .n = { { -863, -506, 7 }, 0, { -371, 1030 }, { -104, -61, 1 }, 50 } },
    { .n = { { -809, -526, -263 }, 0, { -316, 1050 }, { -97, -63, -32 }, 50 } },
    { .n = { { -694, -506, -513 }, 0, { -198, 1030 }, { -83, -61, -62 }, 50 } },
    { .n = { { -526, -447, -724 }, 0, { -26, 970 }, { -63, -54, -87 }, 50 } },
    { .n = { { -273, -506, -818 }, 0, { 232, 1030 }, { -33, -61, -98 }, 50 } },
    { .n = { { 162, -724, -671 }, 0, { 678, 1253 }, { 19, -87, -80 }, 50 } },
    { .n = { { 434, -675, -597 }, 0, { 956, 1203 }, { 52, -81, -72 }, 50 } },
    { .n = { { 588, -724, -362 }, 0, { 1114, 1253 }, { 71, -87, -43 }, 50 } },
    { .n = { { 688, -724, -53 }, 0, { 1217, 1253 }, { 83, -87, -6 }, 50 } },
    { .n = { { 702, -675, 228 }, 0, { 1231, 1203 }, { 84, -81, 27 }, 50 } },
    { .n = { { 526, -724, 447 }, 0, { 1050, 1253 }, { 63, -87, 54 }, 50 } },
    { .n = { { 263, -724, 638 }, 0, { 781, 1253 }, { 32, -87, 77 }, 50 } },
    { .n = { { 0, -675, 738 }, 0, { 512, 1203 }, { 0, -81, 89 }, 50 } },
    { .n = { { -263, -724, 638 }, 0, { 243, 1253 }, { -32, -87, 77 }, 50 } },
    { .n = { { -526, -724, 447 }, 0, { -26, 1253 }, { -63, -87, 54 }, 50 } },
    { .n = { { -702, -675, 228 }, 0, { -207, 1203 }, { -84, -81, 27 }, 50 } },
    { .n = { { -688, -724, -53 }, 0, { -193, 1253 }, { -83, -87, -6 }, 50 } },
    { .n = { { -588, -724, -362 }, 0, { -90, 1253 }, { -71, -87, -43 }, 50 } },
    { .n = { { -434, -675, -597 }, 0, { 68, 1203 }, { -52, -81, -72 }, 50 } },
    { .n = { { -162, -724, -671 }, 0, { 346, 1253 }, { -19, -87, -80 }, 50 } },
    { .n = { { 0, -894, -447 }, 0, { 512, 1428 }, { 0, -107, -54 }, 50 } },
    { .n = { { 309, -851, -425 }, 0, { 828, 1383 }, { 37, -102, -51 }, 50 } },
    { .n = { { 425, -894, -138 }, 0, { 948, 1428 }, { 51, -107, -17 }, 50 } },
    { .n = { { 500, -851, 162 }, 0, { 1024, 1383 }, { 60, -102, 19 }, 50 } },
    { .n = { { 263, -894, 362 }, 0, { 781, 1428 }, { 32, -107, 43 }, 50 } },
    { .n = { { 0, -851, 526 }, 0, { 512, 1383 }, { 0, -102, 63 }, 50 } },
    { .n = { { -263, -894, 362 }, 0, { 243, 1428 }, { -32, -107, 43 }, 50 } },
    { .n = { { -500, -851, 162 }, 0, { 0, 1383 }, { -60, -102, 19 }, 50 } },
    { .n = { { -425, -894, -138 }, 0, { 76, 1428 }, { -51, -107, -17 }, 50 } },
    { .n = { { -309, -851, -425 }, 0, { 196, 1383 }, { -37, -102, -51 }, 50 } },
    { .n = { { 161, -962, -221 }, 0, { 676, 1497 }, { 19, -115, -27 }, 50 } },
    { .n = { { 260, -962, 84 }, 0, { 778, 1497 }, { 31, -115, 10 }, 50 } },
    { .n = { { 0, -962, 273 }, 0, { 512, 1497 }, { 0, -115, 33 }, 50 } },
    { .n = { { -260, -962, 84 }, 0, { 246, 1497 }, { -31, -115, 10 }, 50 } },
    { .n = { { -161, -962, -221 }, 0, { 348, 1497 }, { -19, -115, -27 }, 50 } },
    { .n = { { 0, -1000, 0 }, 0, { 512, 1536 }, { 0, -120, 0 }, 50 } },
};

enum { GOMESS_LIGHT_ORB_FRAMES = 59, GOMESS_DEFEAT_WINDUP_FRAMES = 35, GOMESS_SCYTHE_FALL_FRAMES = 56 };
_Static_assert(GOMESS_DEFEAT_WINDUP_FRAMES + GOMESS_SCYTHE_FALL_FRAMES > GOMESS_LIGHT_ORB_FRAMES,
               "Gomess hit effects must expire before the shared flame buffer is used");

typedef struct {
    StandaloneDuel duel;
    EnemyGomessBat* bats[GOMESS_BAT_COUNT];
    ColliderTris spin;
    ColliderTrisElement spinFaces[2];
    ColliderQuad scythe;
    ColliderJntSph core;
    ColliderJntSphElement coreElement;
    Vec3f corePos;
    Vec3f looseCorePos;
    union {
        Vec3f damageEffectPos[14];
        Vec3f defeatFlamePos[56];
    };
    u8 defeatFlameAlpha[56];
    f32 looseCoreVelocity;
    f32 droppedScytheVelocity;
    s16 looseCoreRotation;
    MtxF scytheHandleMtx;
    MtxF droppedScytheMtx;
    s16 droppedScytheYaw;
    Vec3f scytheWorldPos;
    Vec3f scytheScreenPos;
    u8 scytheSoundPoseValid;
    u8 deathStage;
    u8 droppedScythe;
    u8 looseCore;
    u8 explosiveCooldown;
    u8 lightOrbTimer;
    u8 lightArrowFillTimer;
    u32 scytheHandlePoseFrame;
    u32 corePoseFrame;
    u32 scythePoseFrame;
    s32 scytheTrail;
    s16 cloakUpperRot;
    s16 cloakLowerRot;
    u8 scythePoseKind;
    u8 scytheTrailActive;
    u8 corePoseValid;
    u8 spinFrames;
    u8 floatTimer;
    s8 scytheAfterImages;
    u16 batDropCooldown;
    u32 batDeathSfxFrame;
    u32 batLoopSfxFrame;
    u8 batsInitialized;
    u8 swarmLaunched;
    u16 swarmWaitTimer;
    u8 swarmHits;
    u8 batScatterRequest;
    u8 scatterRequest;
} StandaloneGomess;

enum {
    WART_BUBBLE_COUNT = 82,
    WART_BUBBLE_WANDER = 0,
    WART_BUBBLE_RETURN,
    WART_BUBBLE_CHASE,
    WART_HOOK_PULLS_ACTOR = 1 << 9,
    WART_HOOK_ATTACHED = 1 << 13,
};
typedef struct {
    Vec3f pos;
    Vec3f velocity;
    f32 floor;
    f32 scaleX;
    f32 scaleY;
    f32 attachFraction;
    f32 attachSpeed;
    s16 yaw;
    s16 spin;
    s16 spinSpeed;
    u8 landingStretch;
    u8 mode;
    u8 pause;
    u8 hitCooldown;
    u8 contactCooldown;
    u8 attachAnnounced;
    u8 targetMode;
    u16 returnDelay;
} WartBubble;
typedef struct {
    StandaloneDuel duel;
    ColliderJntSph eye;
    ColliderJntSph shell;
    ColliderJntSphElement eyeElement;
    ColliderJntSphElement shellElement;
    u32 collisionPoseFrame;
    u8 collisionPoseValid;
    ColliderCylinder bubbleReceive[WART_BUBBLE_COUNT];
    ColliderCylinder bubbleAttack[WART_BUBBLE_COUNT];
    WartBubble bubbles[WART_BUBBLE_COUNT];
    Actor hookTarget;
    u8 hookedBubble;
    f32 eyelid;
    s16 eyeYaw;
    s16 shellPitch;
    s16 shellYaw;
    u8 popped;
    Vec3f hoverTarget;
    u16 phaseFrames;
    u8 detachRequests;
    f32 hoverSpeed;
    f32 shellBounds[4];
    u8 shellBoundMask;
    u8 turnTimer;
    u8 eyeTimer;
    u8 rollDelay;
    u8 rollWallCooldown;
    u8 rollImpactPause;
    u8 rollStarted;
} StandaloneWart;

s32 BgCheck_EntityLineTest1(CollisionContext*, Vec3f*, Vec3f*, Vec3f*, CollisionPoly**, s32, s32, s32, s32, s32*);

static void Wart_InitBounds(StandaloneWart* wart, PlayState* play)
{
    const Vec3f origin = wart->duel.actor.world.pos;
    Vec3f from = origin;
    for (int i = 0; i < 4; ++i)
    {
        Vec3f end = origin, hit;
        CollisionPoly* poly;
        s32 bgId;
        f32 offset = (i & 1) ? 1000.f : -1000.f;
        end.y += 200.f;
        if (i < 2)
            end.x += offset;
        else
            end.z += offset;
        if (BgCheck_EntityLineTest1(&play->colCtx, &from, &end, &hit, &poly, 1, 0, 0, 1, &bgId))
        {
            f32 coordinate = i < 2 ? hit.x : hit.z;
            f32 center = i < 2 ? origin.x : origin.z;
            if (fabsf(coordinate - center) > 20.f)
            {
                wart->shellBounds[i] = coordinate + ((i & 1) ? -20.f : 20.f);
                wart->shellBoundMask |= 1u << i;
            }
        }
    }
}

typedef EnemyNativeHookshot WartHookshot;

static void Wart_HookTargetUpdate(Actor* actor, PlayState* play)
{
    (void)actor;
    (void)play;
}

static void Wart_ReleaseHook(StandaloneWart* wart, PlayState* play)
{
    for (Actor* actor = play->actorCtx.actors[ACTORCAT_ITEMACTION].first; actor; actor = actor->next)
    {
        if (actor->id != ACTOR_ARMS_HOOK)
            continue;
        WartHookshot* hook = (WartHookshot*)actor;
        if (hook->attached == &wart->hookTarget)
            hook->attached = NULL;
        if (hook->collider.base.at == &wart->hookTarget)
        {
            hook->collider.base.atFlags &= ~AT_HIT;
            hook->collider.base.at = NULL;
        }
    }
    wart->hookTarget.flags &= ~WART_HOOK_ATTACHED;
    wart->hookedBubble = 0;
}

static int Wart_HookBubble(StandaloneWart* wart, ColliderElement* element, int index)
{
    Collider* hit = element->acHit;
    if (!hit || !hit->actor || hit->actor->id != ACTOR_ARMS_HOOK)
        return 0;
    WartHookshot* hook = (WartHookshot*)hit->actor;
    if (hook->collider.elem.atHitElem != element)
        return 0;
    if (wart->hookedBubble)
        return 0;

    Actor* target = &wart->hookTarget;
    target->world.pos = wart->bubbles[index].pos;
    target->flags = WART_HOOK_PULLS_ACTOR;
    target->update = Wart_HookTargetUpdate;
    wart->hookedBubble = index + 1;
    if (hook->attached == &wart->duel.actor)
    {
        hook->attached = target;
        target->flags |= WART_HOOK_ATTACHED;
        wart->duel.actor.flags &= ~WART_HOOK_ATTACHED;
        hook->offset.x = target->world.pos.x - hook->actor.world.pos.x;
        hook->offset.y = target->world.pos.y - hook->actor.world.pos.y;
        hook->offset.z = target->world.pos.z - hook->actor.world.pos.z;
    }
    hook->collider.base.at = target;
    return 1;
}

static void Wart_InitBubbles(StandaloneWart* wart, PlayState* play)
{
    static ColliderJntSphElementInit eyeElement = {
        { ELEM_MATERIAL_UNK3,
          { 0xf7cfffff, 0, 8 },
          { 0xf7cfffff, 0, 0 },
          ATELEM_ON | ATELEM_SFX_NORMAL,
          ACELEM_ON,
          OCELEM_ON },
        { 1, { { 0, -200, 0 }, 50 }, 100 },
    };
    static ColliderJntSphElementInit shellElement = {
        { ELEM_MATERIAL_UNK3,
          { 0xf7cfffff, 0, 8 },
          { 0xf7cfffff, 0, 0 },
          ATELEM_ON | ATELEM_SFX_NORMAL,
          ACELEM_ON | ACELEM_NO_DAMAGE,
          OCELEM_ON },
        { 1, { { 0, 720, 0 }, 70 }, 100 },
    };
    static ColliderJntSphInit eye = {
        { COL_MATERIAL_HIT3, AT_ON | AT_TYPE_ENEMY, AC_ON | AC_TYPE_PLAYER, OC1_ON | OC1_TYPE_PLAYER, OC2_TYPE_1,
          COLSHAPE_JNTSPH },
        1,
        &eyeElement,
    };
    static ColliderJntSphInit shell = {
        { COL_MATERIAL_METAL, AT_ON | AT_TYPE_ENEMY, AC_ON | AC_TYPE_PLAYER, OC1_ON | OC1_TYPE_PLAYER, OC2_TYPE_1,
          COLSHAPE_JNTSPH },
        1,
        &shellElement,
    };
    static ColliderCylinderInit receive = {
        { COL_MATERIAL_HIT3, AT_ON | AT_TYPE_ENEMY, AC_ON | AC_TYPE_PLAYER, OC1_ON | OC1_TYPE_ALL, OC2_TYPE_1,
          COLSHAPE_CYLINDER },
        { ELEM_MATERIAL_UNK3,
          { 0xF7CFFFFF, 0, 4 },
          { 0xFFFFFFFF, 0, 0 },
          ATELEM_ON | ATELEM_SFX_NORMAL,
          ACELEM_ON | ACELEM_HOOKABLE | ACELEM_NO_DAMAGE,
          OCELEM_ON },
        { 30, 50, -25, { 0, 0, 0 } },
    };
    static ColliderCylinderInit attack = {
        { COL_MATERIAL_HIT3, AT_ON | AT_TYPE_ENEMY, AC_ON | AC_TYPE_PLAYER, OC1_ON | OC1_TYPE_ALL, OC2_TYPE_1,
          COLSHAPE_CYLINDER },
        { ELEM_MATERIAL_UNK3,
          { 0xF7CFFFFF, 0, 4 },
          { 0xF7FFFFFF, 0, 0 },
          ATELEM_ON | ATELEM_SFX_NORMAL,
          ACELEM_ON | ACELEM_HOOKABLE,
          OCELEM_ON },
        { 22, 42, -21, { 0, 0, 0 } },
    };
    Actor* actor = &wart->duel.actor;
    Collider_InitJntSph(play, &wart->eye);
    Collider_SetJntSph(play, &wart->eye, actor, &eye, &wart->eyeElement);
    Collider_InitJntSph(play, &wart->shell);
    Collider_SetJntSph(play, &wart->shell, actor, &shell, &wart->shellElement);
    actor->flags |= WART_HOOK_PULLS_ACTOR;
    Wart_InitBounds(wart, play);
    for (int i = 0; i < WART_BUBBLE_COUNT; ++i)
    {
        Collider_InitCylinder(play, &wart->bubbleReceive[i]);
        Collider_SetCylinder(play, &wart->bubbleReceive[i], actor, &receive);
        Collider_InitCylinder(play, &wart->bubbleAttack[i]);
        Collider_SetCylinder(play, &wart->bubbleAttack[i], actor, &attack);
        WartBubble* bubble = &wart->bubbles[i];
        bubble->spin = (s16)(Rand_ZeroOne() * 65536.f);
        bubble->spinSpeed = (1400 + (s16)(Rand_ZeroOne() * 800.f)) * (Rand_ZeroOne() < .5f ? -1 : 1);
    }
}

enum {
    DUEL_APPROACH,
    DUEL_ATTACK,
    DUEL_RECOVER,
    DUEL_HURT,
    DUEL_DEFEAT,
    DUEL_IGOS_DOWN,
    DUEL_KEETA_JUMP,
    DUEL_KEETA_DROP,
    DUEL_KEETA_LAND
};

#if defined(GAME_OOT)
s32 FOREIGN_ENEMY_API_COLLISION_AC(PlayState*, CollisionCheckContext*, Collider*);
void FOREIGN_ENEMY_API_SFX_ACTOR(Actor*, u16);
#define Duel_AC FOREIGN_ENEMY_API_COLLISION_AC
#define Duel_Sfx FOREIGN_ENEMY_API_SFX_ACTOR
#define Duel_ToMtx ((Mtx* (*)(Mtx*))0x800ab8d8)
#define Duel_Pitch ((s16 (*)(Vec3f*, Vec3f*))0x80063f34)
#else
#define Duel_AC CollisionCheck_SetAC
#define Duel_Sfx Actor_PlaySfx
#define Duel_ToMtx ((Mtx* (*)(Mtx*))0x80181a18)
#define Duel_Pitch Math_Vec3f_Pitch
#endif

enum { GOMESS_DAMAGE_FIRE = 2, GOMESS_DAMAGE_ICE, GOMESS_DAMAGE_LIGHT, GOMESS_DAMAGE_EXPLOSIVE = 15 };

static DamageTable sGomessDamage = { {
    DMG_ENTRY(0, 0),
    DMG_ENTRY(1, 0),
    DMG_ENTRY(0, 0),
    DMG_ENTRY(1, GOMESS_DAMAGE_EXPLOSIVE),
    DMG_ENTRY(1, 0),
    DMG_ENTRY(1, 0),
    DMG_ENTRY(0, 0),
    DMG_ENTRY(1, 0),
    DMG_ENTRY(1, 0),
    DMG_ENTRY(1, 0),
    DMG_ENTRY(0, 0),
    DMG_ENTRY(1, GOMESS_DAMAGE_FIRE),
    DMG_ENTRY(1, GOMESS_DAMAGE_ICE),
    DMG_ENTRY(2, GOMESS_DAMAGE_LIGHT),
    DMG_ENTRY(1, 0),
    DMG_ENTRY(0, 0),
    DMG_ENTRY(1, 0),
    DMG_ENTRY(2, 0),
    DMG_ENTRY(0, 0),
    DMG_ENTRY(0, 0),
    DMG_ENTRY(0, 0),
    DMG_ENTRY(0, 0),
    DMG_ENTRY(1, 0),
    DMG_ENTRY(1, 0),
    DMG_ENTRY(1, 0),
    DMG_ENTRY(0, 0),
    DMG_ENTRY(0, 0),
    DMG_ENTRY(0, 0),
    DMG_ENTRY(0, 0),
    DMG_ENTRY(0, 0),
    DMG_ENTRY(0, 0),
    DMG_ENTRY(1, GOMESS_DAMAGE_EXPLOSIVE),
} };
enum { GARO_DAMAGE_STUN = 1, GARO_DAMAGE_FIRE, GARO_DAMAGE_ICE, GARO_DAMAGE_LIGHT, GARO_DAMAGE_ELECTRIC };

static DamageTable sGaroMasterDamage = { {
    DMG_ENTRY(0, GARO_DAMAGE_STUN),
    DMG_ENTRY(1, 0),
    DMG_ENTRY(0, 0),
    DMG_ENTRY(1, 0),
    DMG_ENTRY(1, 0),
    DMG_ENTRY(1, 0),
    DMG_ENTRY(0, 0),
    DMG_ENTRY(0, GARO_DAMAGE_STUN),
    DMG_ENTRY(1, 0),
    DMG_ENTRY(1, 0),
    DMG_ENTRY(1, 0),
    DMG_ENTRY(2, GARO_DAMAGE_FIRE),
    DMG_ENTRY(2, GARO_DAMAGE_ICE),
    DMG_ENTRY(2, GARO_DAMAGE_LIGHT),
    DMG_ENTRY(1, 0),
    DMG_ENTRY(0, GARO_DAMAGE_STUN),
    DMG_ENTRY(1, 0),
    DMG_ENTRY(2, 0),
    DMG_ENTRY(0, GARO_DAMAGE_STUN),
    DMG_ENTRY(0, GARO_DAMAGE_ELECTRIC),
    DMG_ENTRY(0, 0),
    DMG_ENTRY(0, 0),
    DMG_ENTRY(1, 0),
    DMG_ENTRY(1, 0),
    DMG_ENTRY(1, 0),
    DMG_ENTRY(0, 0),
    DMG_ENTRY(0, 0),
    DMG_ENTRY(0, 0),
    DMG_ENTRY(0, 0),
    DMG_ENTRY(0, 0),
    DMG_ENTRY(0, 0),
    DMG_ENTRY(1, 0),
} };

static DamageTable sSnapperDamage = { {
    DMG_ENTRY(0, 1), DMG_ENTRY(1, 0), DMG_ENTRY(0, 0), DMG_ENTRY(1, 0), DMG_ENTRY(1, 0), DMG_ENTRY(1, 0),
    DMG_ENTRY(0, 0), DMG_ENTRY(0, 1), DMG_ENTRY(1, 0), DMG_ENTRY(1, 0), DMG_ENTRY(1, 0), DMG_ENTRY(2, 2),
    DMG_ENTRY(2, 3), DMG_ENTRY(2, 4), DMG_ENTRY(1, 0), DMG_ENTRY(0, 1), DMG_ENTRY(1, 0), DMG_ENTRY(2, 0),
    DMG_ENTRY(0, 1), DMG_ENTRY(0, 5), DMG_ENTRY(0, 0), DMG_ENTRY(0, 0), DMG_ENTRY(1, 0), DMG_ENTRY(1, 0),
    DMG_ENTRY(1, 0), DMG_ENTRY(1, 0), DMG_ENTRY(0, 0), DMG_ENTRY(0, 0), DMG_ENTRY(0, 0), DMG_ENTRY(0, 0),
    DMG_ENTRY(0, 0), DMG_ENTRY(1, 0),
} };

static DamageTable sSlimeDamage = { {
    DMG_ENTRY(0, 1), DMG_ENTRY(1, 0),  DMG_ENTRY(0, 0),  DMG_ENTRY(1, 15), DMG_ENTRY(1, 0),  DMG_ENTRY(1, 0),
    DMG_ENTRY(0, 0), DMG_ENTRY(0, 14), DMG_ENTRY(1, 15), DMG_ENTRY(1, 0),  DMG_ENTRY(1, 15), DMG_ENTRY(1, 2),
    DMG_ENTRY(1, 3), DMG_ENTRY(2, 4),  DMG_ENTRY(1, 15), DMG_ENTRY(0, 13), DMG_ENTRY(1, 0),  DMG_ENTRY(0, 0),
    DMG_ENTRY(0, 1), DMG_ENTRY(0, 5),  DMG_ENTRY(0, 0),  DMG_ENTRY(0, 0),  DMG_ENTRY(1, 15), DMG_ENTRY(1, 0),
    DMG_ENTRY(1, 0), DMG_ENTRY(1, 0),  DMG_ENTRY(0, 0),  DMG_ENTRY(0, 0),  DMG_ENTRY(0, 0),  DMG_ENTRY(0, 0),
    DMG_ENTRY(0, 0), DMG_ENTRY(1, 15),
} };

static ColliderCylinderInit sDuelBody = {
    { COL_MATERIAL_HIT0, AT_NONE, AC_ON | AC_TYPE_PLAYER, OC1_ON | OC1_TYPE_PLAYER, OC2_TYPE_1, COLSHAPE_CYLINDER },
    { ELEM_MATERIAL_UNK0, { 0, 0, 0 }, { 0xffffffff, 0, 0 }, ATELEM_NONE, ACELEM_ON, OCELEM_ON },
    { 29, 75, 0, { 0, 0, 0 } },
};

static ColliderCylinderInit sDuelWeapon = {
    { COL_MATERIAL_NONE, AT_ON | AT_TYPE_ENEMY, AC_NONE, OC1_NONE, OC2_NONE, COLSHAPE_CYLINDER },
    { ELEM_MATERIAL_UNK0, { 0x20000000, 0, 8 }, { 0, 0, 0 }, ATELEM_ON | ATELEM_SFX_NORMAL, ACELEM_NONE, OCELEM_NONE },
    { 32, 56, 8, { 0, 0, 0 } },
};

typedef struct {
    u16 id;
    u8 limbs;
    u8 health;
    u32 skeleton;
    u16 idle;
    u16 attack;
    u16 hurt;
    u16 defeat;
    u16 strikeSfx;
    u16 hurtSfx;
    u16 deathSfx;
    float scale;
} DuelForm;

static const DuelForm sForms[] = {
    { 0x182, DUEL_GARO_LIMBS, 14, 0x3168, 0xae80, 0x3530, 0x44f8, 0x5d5c, 0x39c7, 0x39c5, 0x39c6, .042f },
    { 0x043, DUEL_GOMESS_LIMBS, 20, 0xad08, 0x3cac, 0x0e64, 0x1f80, 0x2de8, 0x3aac, 0x3ab1, 0x3ab2, .01f },
    { 0x12c, DUEL_WART_LIMBS, 16, 0x45e8, 0x004c, 0x004c, 0x004c, 0x004c, 0x3a16, 0x3a13, 0x3a14, .075f },
    { 0x007, DUEL_SNAPPER_LIMBS, 3, 0xdf98, 0x66b4, 0x347c, 0x5d54, 0x4298, 0x3961, 0x38d3, 0x38d6, .007f },
    { 0x065, DUEL_SLIME_LIMBS, 10, 0xdf98, 0xf990, 0x70c4, 0x5d54, 0x4298, 0x3961, 0x38d3, 0x38d6, .01f },
    { 0x115, DUEL_IGOS_LIMBS, 14, 0x201a8, 0x40e0, 0xbcf4, 0x4974, 0x5d30, 0x3a30, 0x3a3a, 0x3a5b, .017f },
    { 0x28f, DUEL_KEETA_LIMBS, 24, 0xc3e0, 0x3238, 0x0c50, 0x1390, 0x2af4, 0x3aaa, 0x3aa5, 0x3aa6, .04f },
};

enum {
    KEETA_DAMAGE_STUN = 1,
    KEETA_DAMAGE_FIRE,
    KEETA_DAMAGE_ICE,
    KEETA_DAMAGE_LIGHT,
    KEETA_DAMAGE_ELECTRIC,
    KEETA_DAMAGE_SPIN = 12,
    KEETA_DAMAGE_MELEE,
    KEETA_DAMAGE_EXPLOSIVE,
    KEETA_DAMAGE_PROJECTILE,
};

static DamageTable sKeetaDamage = { {
    DMG_ENTRY(0, 1),  DMG_ENTRY(1, 13), DMG_ENTRY(0, 0),  DMG_ENTRY(1, 14), DMG_ENTRY(0, 15), DMG_ENTRY(0, 15),
    DMG_ENTRY(0, 0),  DMG_ENTRY(0, 15), DMG_ENTRY(1, 13), DMG_ENTRY(1, 13), DMG_ENTRY(1, 13), DMG_ENTRY(0, 2),
    DMG_ENTRY(0, 3),  DMG_ENTRY(0, 4),  DMG_ENTRY(0, 15), DMG_ENTRY(0, 12), DMG_ENTRY(0, 15), DMG_ENTRY(1, 14),
    DMG_ENTRY(0, 1),  DMG_ENTRY(1, 5),  DMG_ENTRY(0, 0),  DMG_ENTRY(0, 0),  DMG_ENTRY(0, 15), DMG_ENTRY(1, 13),
    DMG_ENTRY(1, 13), DMG_ENTRY(0, 0),  DMG_ENTRY(0, 0),  DMG_ENTRY(0, 0),  DMG_ENTRY(0, 0),  DMG_ENTRY(0, 0),
    DMG_ENTRY(0, 0),  DMG_ENTRY(1, 13),
} };

static DamageTable sWartDamage = { {
    DMG_ENTRY(1, 15), DMG_ENTRY(1, 15), DMG_ENTRY(0, 0),  DMG_ENTRY(2, 15), DMG_ENTRY(1, 15), DMG_ENTRY(2, 15),
    DMG_ENTRY(0, 0),  DMG_ENTRY(1, 15), DMG_ENTRY(2, 15), DMG_ENTRY(2, 15), DMG_ENTRY(2, 15), DMG_ENTRY(2, 2),
    DMG_ENTRY(2, 3),  DMG_ENTRY(2, 4),  DMG_ENTRY(2, 15), DMG_ENTRY(1, 15), DMG_ENTRY(1, 15), DMG_ENTRY(1, 15),
    DMG_ENTRY(1, 15), DMG_ENTRY(0, 0),  DMG_ENTRY(0, 15), DMG_ENTRY(0, 15), DMG_ENTRY(2, 15), DMG_ENTRY(2, 15),
    DMG_ENTRY(2, 15), DMG_ENTRY(0, 0),  DMG_ENTRY(0, 0),  DMG_ENTRY(0, 0),  DMG_ENTRY(0, 0),  DMG_ENTRY(0, 0),
    DMG_ENTRY(0, 0),  DMG_ENTRY(1, 15),
} };

static DamageTable sArenaDamage = { {
    DMG_ENTRY(0, 1), DMG_ENTRY(1, 0), DMG_ENTRY(0, 0), DMG_ENTRY(1, 2), DMG_ENTRY(1, 1), DMG_ENTRY(1, 1),
    DMG_ENTRY(0, 0), DMG_ENTRY(1, 1), DMG_ENTRY(2, 2), DMG_ENTRY(2, 0), DMG_ENTRY(2, 2), DMG_ENTRY(2, 2),
    DMG_ENTRY(2, 3), DMG_ENTRY(2, 4), DMG_ENTRY(1, 2), DMG_ENTRY(1, 2), DMG_ENTRY(1, 1), DMG_ENTRY(0, 0),
    DMG_ENTRY(1, 2), DMG_ENTRY(1, 1), DMG_ENTRY(0, 0), DMG_ENTRY(0, 0), DMG_ENTRY(1, 1), DMG_ENTRY(1, 0),
    DMG_ENTRY(2, 0), DMG_ENTRY(0, 0), DMG_ENTRY(0, 0), DMG_ENTRY(0, 0), DMG_ENTRY(0, 0), DMG_ENTRY(0, 0),
    DMG_ENTRY(0, 0), DMG_ENTRY(2, 2),
} };

enum {
    IGOS_DAMAGE_STUN = 1,
    IGOS_DAMAGE_FIRE,
    IGOS_DAMAGE_ICE,
    IGOS_DAMAGE_LIGHT,
    IGOS_DAMAGE_ELECTRIC,
    IGOS_DAMAGE_RAY,
    IGOS_DAMAGE_POUND
};
enum {
    IGOS_EFFECT_FIRE = 0,
    IGOS_EFFECT_ICE_QUIET = 10,
    IGOS_EFFECT_ICE_STEAM = 11,
    IGOS_EFFECT_LIGHT = 20,
    IGOS_EFFECT_ELECTRIC = 30
};

static DamageTable sIgosDamage = { {
    DMG_ENTRY(0, IGOS_DAMAGE_STUN),
    DMG_ENTRY(1, 0),
    DMG_ENTRY(0, 0),
    DMG_ENTRY(1, 0),
    DMG_ENTRY(0, IGOS_DAMAGE_STUN),
    DMG_ENTRY(3, 0),
    DMG_ENTRY(0, 0),
    DMG_ENTRY(0, 0),
    DMG_ENTRY(1, 0),
    DMG_ENTRY(1, 0),
    DMG_ENTRY(0, IGOS_DAMAGE_POUND),
    DMG_ENTRY(1, IGOS_DAMAGE_FIRE),
    DMG_ENTRY(1, IGOS_DAMAGE_ICE),
    DMG_ENTRY(2, IGOS_DAMAGE_LIGHT),
    DMG_ENTRY(1, 0),
    DMG_ENTRY(0, IGOS_DAMAGE_STUN),
    DMG_ENTRY(2, 0),
    DMG_ENTRY(1, 0),
    DMG_ENTRY(0, 0),
    DMG_ENTRY(0, IGOS_DAMAGE_ELECTRIC),
    DMG_ENTRY(0, 0),
    DMG_ENTRY(0, IGOS_DAMAGE_RAY),
    DMG_ENTRY(1, 0),
    DMG_ENTRY(1, 0),
    DMG_ENTRY(1, 0),
    DMG_ENTRY(0, 0),
    DMG_ENTRY(0, 0),
    DMG_ENTRY(0, 0),
    DMG_ENTRY(0, 0),
    DMG_ENTRY(0, 0),
    DMG_ENTRY(0, 0),
    DMG_ENTRY(1, 0),
} };

static AnimationHeader* Duel_Anim(u32 offset)
{
    return (AnimationHeader*)(0x06000000u | offset);
}

static void Garo_InitSwords(StandaloneGaroMaster* garo, PlayState* play)
{
    static ColliderQuadInit sword = {
        { COL_MATERIAL_NONE, AT_ON | AT_TYPE_ENEMY, AC_NONE, OC1_NONE, OC2_NONE, COLSHAPE_QUAD },
        { ELEM_MATERIAL_UNK0,
#if defined(GAME_OOT)
          { 0xf7cfffff, HIT_SPECIAL_EFFECT_KNOCKBACK, 16 },
#else
          { 0xf7cfffff, 9, 16 },
#endif
          { 0, 0, 0 },
          ATELEM_ON | ATELEM_SFX_NORMAL | ATELEM_UNK7,
          ACELEM_NONE,
          OCELEM_NONE },
        { { { 0 } } },
    };
    static EffectBlureInit1 trail = {
        .p1StartColor = { 255, 0, 0, 128 },
        .p2StartColor = { 255, 0, 0, 64 },
        .p1EndColor = { 255, 0, 0, 0 },
        .p2EndColor = { 255, 0, 255, 0 },
        .elemDuration = 8,
        .calcMode = 2,
    };
    for (int i = 0; i < ARRAY_COUNT(garo->swords); ++i)
    {
        Collider_InitQuad(play, &garo->swords[i]);
        Collider_SetQuad(play, &garo->swords[i], &garo->duel.actor, &sword);
        garo->trails[i] = -1;
        Effect_Add(play, &garo->trails[i], EFFECT_BLURE1, 0, 0, &trail);
    }
}

static void Igos_InitEquipment(StandaloneIgos* igos, PlayState* play)
{
    Actor* actor = &igos->duel.actor;
    igos->bodyAlpha = 255;
    igos->shadowAlphaFactor = 180;
    igos->duel.actor.shape.shadowAlpha = 180;
    ColliderJntSphElementInit element = {
        { ELEM_MATERIAL_UNK2,
          { 0xf7cfffff, 0, 8 },
          { 0xf7cfffff, 0, 0 },
          ATELEM_ON | ATELEM_SFX_NORMAL,
          ACELEM_ON | ACELEM_NO_DAMAGE,
          OCELEM_ON },
        { 0, { { 0, 0, 0 }, 27 }, 100 },
    };
    ColliderJntSphInit init = {
        { COL_MATERIAL_METAL, AT_ON | AT_TYPE_ENEMY, AC_ON | AC_HARD | AC_TYPE_PLAYER, OC1_ON | OC1_TYPE_PLAYER,
          OC2_TYPE_1, COLSHAPE_JNTSPH },
        1,
        &element,
    };
    Collider_InitJntSph(play, &igos->shield);
    Collider_SetJntSph(play, &igos->shield, actor, &init, &igos->shieldElement);
    if ((u16)actor->params == 0x807f)
    {
        element.dim.modelSphere.radius = 32;
        element.base.atDmgInfo.damage = 16;
    }
    Collider_InitJntSph(play, &igos->sword);
    Collider_SetJntSph(play, &igos->sword, actor, &init, &igos->swordElement);

    static ColliderJntSphElementInit bodyElements[] = {
        { { ELEM_MATERIAL_UNK3,
            { 0xf7cfffff, 0, 8 },
            { 0xf7efffff, 0, 0 },
            ATELEM_NONE,
            ACELEM_ON | ACELEM_HOOKABLE,
            OCELEM_ON },
          { 0, { { 0, 0, 0 }, 15 }, 100 } },
        { { ELEM_MATERIAL_UNK3,
            { 0xf7cfffff, 0, 8 },
            { 0xf7efffff, 0, 0 },
            ATELEM_NONE,
            ACELEM_ON | ACELEM_HOOKABLE,
            OCELEM_ON },
          { 1, { { 0, 0, 0 }, 17 }, 100 } },
    };
    static ColliderJntSphInit body = {
        { COL_MATERIAL_HIT3, AT_NONE, AC_ON | AC_TYPE_PLAYER, OC1_ON | OC1_TYPE_PLAYER | OC1_TYPE_1, OC2_TYPE_1,
          COLSHAPE_JNTSPH },
        ARRAY_COUNT(bodyElements),
        bodyElements,
    };
    Collider_InitJntSph(play, &igos->bodySpheres);
    Collider_SetJntSph(play, &igos->bodySpheres, actor, &body, igos->bodyElements);
}

static void Igos_InitHead(IgosFlyingHead* igos, PlayState* play)
{
    static ColliderCylinderInit attack = {
        { COL_MATERIAL_METAL, AT_ON | AT_TYPE_ENEMY, AC_ON | AC_HARD | AC_TYPE_PLAYER, OC1_NONE, OC2_NONE,
          COLSHAPE_CYLINDER },
        { ELEM_MATERIAL_UNK3,
          { 0xf7cfffff, 0, 16 },
          { 0xf7cfffff, 0, 0 },
          ATELEM_ON | ATELEM_SFX_NORMAL,
          ACELEM_ON,
          OCELEM_NONE },
        { 15, 20, -10, { 0, 0, 0 } },
    };
    static ColliderCylinderInit bite = {
        { COL_MATERIAL_HIT3, AT_NONE, AC_ON | AC_TYPE_PLAYER | AC_TYPE_ENEMY | AC_TYPE_OTHER, OC1_NONE, OC2_NONE,
          COLSHAPE_CYLINDER },
        { ELEM_MATERIAL_UNK3, { 0, 0, 0 }, { 0xf7ffffff, 0, 0 }, ATELEM_NONE, ACELEM_ON, OCELEM_NONE },
        { 20, 40, 0, { 0, 0, 0 } },
    };
    SkelAnime_InitFlex(play, &igos->headSkel, (FlexSkeletonHeader*)0x060201a8, Duel_Anim(0x3008), igos->headJoints,
                       igos->headMorph, 29);
    igos->headSkel.curFrame = Animation_GetLastFrame(Duel_Anim(0x3008));
    igos->headSkel.playSpeed = 0.f;
    SkelAnime_Update(&igos->headSkel);
    igos->randTimer = (u16)(Rand_ZeroOne() * 1000.f);
    Collider_InitCylinder(play, &igos->headCollider);
    Collider_SetCylinder(play, &igos->headCollider, &igos->actor, &bite);
    Collider_InitCylinder(play, &igos->headAttackCollider);
    Collider_SetCylinder(play, &igos->headAttackCollider, &igos->actor, &attack);
}

static void Duel_Init(Actor* actor, PlayState* play)
{
    if (DUEL_FIXED_FORM == DUEL_IGOS && (u16)actor->params == 0x837f)
    {
        actor->focus.pos = actor->world.pos;
        actor->colChkInfo.health = 1;
        Igos_InitHead((IgosFlyingHead*)actor, play);
        return;
    }
    if (DUEL_FIXED_FORM == DUEL_IGOS) {
        StandaloneIgos* igos = (StandaloneIgos*)actor;
        igos->knightEncounter = (u16)actor->params == 0x907f;
        if (igos->knightEncounter)
            actor->params = 0x807f;
        else if ((u16)actor->params == 0x847f)
            actor->params = 0x817f;
    }
    StandaloneDuel* duel = (StandaloneDuel*)actor;
    const DuelForm* form = &sForms[DUEL_FIXED_FORM];
    actor->flags |= ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED;
    if (DUEL_FIXED_FORM == DUEL_SLIME || DUEL_FIXED_FORM == DUEL_SNAPPER)
        actor->flags |= ACTOR_FLAG_DRAW_CULLING_DISABLED;
    actor->gravity = DUEL_FIXED_FORM == DUEL_IGOS ? -1.5f : -2.f;
    Actor_SetScale(actor, form->scale);
    if (DUEL_FIXED_FORM == DUEL_SNAPPER || DUEL_FIXED_FORM == DUEL_SLIME)
    {
        f32 shadowScale = DUEL_FIXED_FORM == DUEL_SNAPPER ? 55.f : 550.f / 7.f;
        ActorShape_Init(&actor->shape, 0.f, ActorShadow_DrawCircle, shadowScale);
    }
    else if (DUEL_FIXED_FORM == DUEL_GARO)
    {
        ActorShape_Init(&actor->shape, 960.f, ActorShadow_DrawCircle, 17.f);
        actor->gravity = -3.f;
        actor->attentionRangeType = ATTENTION_RANGE_5;
    }
    else if (DUEL_FIXED_FORM == DUEL_GOMESS)
    {
        ActorShape_Init(&actor->shape, 5500.f, ActorShadow_DrawCircle, 80.f);
        actor->gravity = 0.f;
        actor->targetArrowOffset = 6000.f;
        actor->attentionRangeType = ATTENTION_RANGE_5;
    }
    actor->shape.yOffset = DUEL_FIXED_FORM == DUEL_GOMESS ? 5500.f
                           : DUEL_FIXED_FORM == DUEL_GARO ? 960.f
                           : DUEL_FIXED_FORM == DUEL_WART ? 900.f
                                                          : 0.f;
    actor->colChkInfo.health = form->health;
    actor->colChkInfo.damageTable = DUEL_FIXED_FORM == DUEL_GOMESS    ? &sGomessDamage
                                    : DUEL_FIXED_FORM == DUEL_GARO    ? &sGaroMasterDamage
                                    : DUEL_FIXED_FORM == DUEL_IGOS    ? &sIgosDamage
                                    : DUEL_FIXED_FORM == DUEL_SNAPPER ? &sSnapperDamage
                                    : DUEL_FIXED_FORM == DUEL_SLIME   ? &sSlimeDamage
                                    : DUEL_FIXED_FORM == DUEL_KEETA   ? &sKeetaDamage
                                    : DUEL_FIXED_FORM == DUEL_WART    ? &sWartDamage
                                                                      : &sArenaDamage;
    void* skeleton = (void*)(0x06000000u | form->skeleton);
    if (DUEL_FIXED_FORM == DUEL_IGOS && (u16)actor->params != 0x807f)
    {
        skeleton = (void*)0x06020374;
        Actor_SetScale(actor, .013f);
        actor->colChkInfo.health = 6;
    }
    if (DUEL_FIXED_FORM == DUEL_KEETA)
        SkelAnime_Init(play, &duel->skel, skeleton, Duel_Anim(form->idle), duel->joints, duel->morph, form->limbs);
    else
        SkelAnime_InitFlex(play, &duel->skel, skeleton, Duel_Anim(form->idle), duel->joints, duel->morph, form->limbs);
    Collider_InitCylinder(play, &duel->body);
    if (DUEL_FIXED_FORM == DUEL_GARO)
    {
        static ColliderCylinderInit body = {
            { COL_MATERIAL_NONE, AT_ON | AT_TYPE_ENEMY, AC_ON | AC_HARD | AC_TYPE_PLAYER, OC1_ON | OC1_TYPE_ALL,
              OC2_TYPE_1, COLSHAPE_CYLINDER },
            { ELEM_MATERIAL_UNK0,
              { 0xf7cfffff, 8, 4 },
              { 0xf7cfffff, 0, 0 },
              ATELEM_ON | ATELEM_SFX_NORMAL,
              ACELEM_ON,
              OCELEM_ON },
            { 22, 70, 0, { 0, 0, 0 } },
        };
        Collider_SetCylinder(play, &duel->body, actor, &body);
        actor->colChkInfo.mass = 80;
    }
    else
    {
        Collider_SetCylinder(play, &duel->body, actor, &sDuelBody);
    }
    Collider_InitCylinder(play, &duel->weapon);
    Collider_SetCylinder(play, &duel->weapon, actor, &sDuelWeapon);
    if (DUEL_FIXED_FORM == DUEL_IGOS)
    {
        Igos_InitEquipment((StandaloneIgos*)duel, play);
        if ((u16)actor->params == 0x807f)
            duel->specialCooldown = 300;
        ((StandaloneIgos*)duel)->randTimerOffset = (u16)(Rand_ZeroOne() * 1000.f) - play->gameplayFrames;
    }
    if (DUEL_FIXED_FORM == DUEL_GARO)
    {
        StandaloneGaroMaster* garo = (StandaloneGaroMaster*)duel;
#if defined(GAME_MM)
        actor->hintId = TATL_HINT_ID_GARO_MASTER;
#endif
        garo->flameScroll = (u8)(Rand_ZeroOne() * 7.f);
        Garo_InitSwords(garo, play);
    }
    if (DUEL_FIXED_FORM == DUEL_WART)
    {
        actor->colChkInfo.health = 20;
        duel->shield = WART_BUBBLE_COUNT;
        ((StandaloneWart*)duel)->eyeTimer = 60;
        Wart_InitBubbles((StandaloneWart*)duel, play);
        duel->body.dim.radius = 37;
    }
    if (DUEL_FIXED_FORM == DUEL_SNAPPER)
    {
        StandaloneSnapper* snapper = (StandaloneSnapper*)duel;
        static ColliderJntSphElementInit elements[] = {
            { { ELEM_MATERIAL_UNK1, { 0xf7cfffff, 0, 0 }, { 0xf7cfffff, 0, 0 }, ATELEM_NONE, ACELEM_ON, OCELEM_ON },
              { 1, { { 0, 0, 0 }, 32 }, 100 } },
            { { ELEM_MATERIAL_UNK0,
                { 0xf7cfffff, 0, 4 },
                { 0, 0, 0 },
                ATELEM_ON | ATELEM_SFX_WOOD,
                ACELEM_NONE,
                OCELEM_NONE },
              { 1, { { 0, 0, 0 }, 25 }, 100 } },
        };
        static ColliderJntSphInit body = {
            { COL_MATERIAL_HIT6, AT_TYPE_ENEMY, AC_ON | AC_TYPE_PLAYER, OC1_ON | OC1_TYPE_ALL, OC2_TYPE_1,
              COLSHAPE_JNTSPH },
            ARRAY_COUNT(elements),
            elements,
        };
        actor->gravity = -1.f;
        actor->colChkInfo.mass = 50;
        Collider_InitJntSph(play, &snapper->body);
        Collider_SetJntSph(play, &snapper->body, actor, &body, snapper->elements);
        snapper->elements[0].dim.worldSphere.radius = 32;
        snapper->elements[1].dim.worldSphere.radius = 25;
        duel->phase = DUEL_RECOVER;
        Animation_PlayOnce(&duel->skel, Duel_Anim(form->idle));
    }
    if (DUEL_FIXED_FORM == DUEL_SLIME)
    {
        StandaloneSlime* slime = (StandaloneSlime*)duel;
        static ColliderCylinderInit shell = {
            { COL_MATERIAL_HIT0, AT_NONE, AC_ON | AC_TYPE_PLAYER, OC1_NONE, OC2_NONE, COLSHAPE_CYLINDER },
            { ELEM_MATERIAL_UNK0, { 0, 0, 0 }, { 0xffffffff, 0, 0 }, ATELEM_NONE, ACELEM_ON, OCELEM_NONE },
            { 1, 1, 0, { 0, 0, 0 } },
        };
        for (int i = 0; i < ARRAY_COUNT(slime->shell); ++i)
        {
            Collider_InitCylinder(play, &slime->shell[i]);
            Collider_SetCylinder(play, &slime->shell[i], actor, &shell);
        }
        duel->body.base.ocFlags1 |= OC1_TYPE_ALL;
        duel->body.elem.acElemFlags |= ACELEM_HOOKABLE;
        duel->phase = SLIME_FORM;
        duel->timer = 28;
        Animation_PlayOnce(&duel->skel, Duel_Anim(0x2e0c));
    }
    if (DUEL_FIXED_FORM == DUEL_GOMESS)
    {
        StandaloneGomess* gomess = (StandaloneGomess*)duel;
        actor->colChkInfo.cylRadius = 28;
        actor->colChkInfo.cylHeight = 90;
        actor->colChkInfo.cylYShift = 20;
        actor->colChkInfo.mass = 100;
        duel->body.dim.radius = 35;
        duel->body.dim.height = 90;
        duel->body.dim.yShift = 20;
        duel->body.base.colMaterial = COL_MATERIAL_NONE;
        duel->body.base.acFlags = AC_NONE;
        duel->body.base.ocFlags1 = OC1_ON | OC1_TYPE_ALL;
        static ColliderTrisElementInit faces[2] = {
            [0 ... 1] = { { ELEM_MATERIAL_UNK2,
                            { 0xf7cfffff, 4, 0x20 },
                            { 0xf7cfffff, 0, 0 },
                            ATELEM_ON | ATELEM_SFX_NORMAL,
                            ACELEM_ON,
                            OCELEM_NONE } },
        };
        static ColliderTrisInit spin = {
            { COL_MATERIAL_METAL, AT_ON | AT_TYPE_ENEMY, AC_ON | AC_HARD | AC_TYPE_PLAYER, OC1_NONE, OC2_TYPE_1,
              COLSHAPE_TRIS },
            ARRAY_COUNT(faces),
            faces,
        };
        static ColliderQuadInit scythe = {
            { COL_MATERIAL_NONE, AT_ON | AT_TYPE_ENEMY, AC_NONE, OC1_NONE, OC2_TYPE_2, COLSHAPE_QUAD },
            { ELEM_MATERIAL_UNK0,
              { 0xf7cfffff, 4, 0x20 },
              { 0, 0, 0 },
              ATELEM_ON | ATELEM_SFX_NORMAL | ATELEM_UNK7,
              ACELEM_NONE,
              OCELEM_NONE },
        };
        static ColliderJntSphElementInit coreElement = {
            { ELEM_MATERIAL_UNK0, { 0, 0, 0 }, { 0xf7cfffff, 0, 0 }, ATELEM_NONE, ACELEM_ON, OCELEM_NONE },
            { 2, { { 0, 0, 0 }, 22 }, 100 },
        };
        static ColliderJntSphInit core = {
            { COL_MATERIAL_HIT3, AT_NONE, AC_ON | AC_TYPE_PLAYER, OC1_NONE, OC2_TYPE_1, COLSHAPE_JNTSPH },
            1,
            &coreElement,
        };
        static EffectBlureInit2 trail = {
            .flags = 8,
            .p1StartColor = { 100, 50, 100, 200 },
            .p2StartColor = { 100, 0, 0, 64 },
            .p1EndColor = { 100, 0, 0, 20 },
            .p2EndColor = { 50, 0, 0, 0 },
            .elemDuration = 8,
            .drawMode = 2,
        };
        duel->body.elem.acElemFlags = ACELEM_NONE;
        duel->body.base.ocFlags2 = OC2_TYPE_1;
        Collider_InitJntSph(play, &gomess->core);
        Collider_SetJntSph(play, &gomess->core, actor, &core, &gomess->coreElement);
        gomess->scytheTrail = -1;
        Effect_Add(play, &gomess->scytheTrail, EFFECT_BLURE2, 0, 0, &trail);
        Collider_InitQuad(play, &gomess->scythe);
        Collider_SetQuad(play, &gomess->scythe, actor, &scythe);
        Collider_InitTris(play, &gomess->spin);
        Collider_SetTris(play, &gomess->spin, actor, &spin, gomess->spinFaces);
        gomess->batDeathSfxFrame = play->gameplayFrames - 1;
        gomess->batLoopSfxFrame = play->gameplayFrames - 1;
        duel->timer = 100;
    }
    if (DUEL_FIXED_FORM == DUEL_KEETA)
    {
        StandaloneKeeta* keeta = (StandaloneKeeta*)duel;
        static ColliderJntSphElementInit elements[] = {
            { { ELEM_MATERIAL_UNK0, { 0, 0, 0 }, { 0xf7cfffff, 0, 0 }, ATELEM_NONE, ACELEM_ON, OCELEM_ON },
              { 10, { { 1000, 400, 0 }, 40 }, 100 } },
            { { ELEM_MATERIAL_UNK0,
                { 0xf7cfffff, 4, 8 },
                { 0, 0, 0 },
                ATELEM_ON | ATELEM_SFX_NORMAL,
                ACELEM_NONE,
                OCELEM_ON },
              { 9, { { 0, 700, 200 }, 35 }, 100 } },
            { { ELEM_MATERIAL_UNK0, { 0, 0, 0 }, { 0xf7cfffff, 0, 0 }, ATELEM_NONE, ACELEM_ON, OCELEM_ON },
              { 6, { { 100, 600, 0 }, 35 }, 100 } },
            { { ELEM_MATERIAL_UNK0, { 0, 0, 0 }, { 0xf7cfffff, 0, 0 }, ATELEM_NONE, ACELEM_ON, OCELEM_NONE },
              { 3, { { 400, 200, 0 }, 40 }, 100 } },
            { { ELEM_MATERIAL_UNK0, { 0, 0, 0 }, { 0xf7cfffff, 0, 0 }, ATELEM_NONE, ACELEM_ON, OCELEM_ON },
              { 13, { { 700, -100, 0 }, 35 }, 100 } },
            { { ELEM_MATERIAL_UNK0, { 0, 0, 0 }, { 0xf7cfffff, 0, 0 }, ATELEM_NONE, ACELEM_ON, OCELEM_ON },
              { 16, { { 200, 300, 0 }, 30 }, 100 } },
            { { ELEM_MATERIAL_UNK0, { 0, 0, 0 }, { 0xf7cfffff, 0, 0 }, ATELEM_NONE, ACELEM_ON, OCELEM_ON },
              { 19, { { 200, 300, 0 }, 30 }, 100 } },
        };
        static ColliderJntSphInit body = {
            { COL_MATERIAL_HIT6, AT_ON | AT_TYPE_ENEMY, AC_ON | AC_TYPE_PLAYER, OC1_ON | OC1_TYPE_PLAYER, OC2_TYPE_1,
              COLSHAPE_JNTSPH },
            ARRAY_COUNT(elements),
            elements,
        };
        Collider_InitJntSph(play, &keeta->bodySpheres);
        Collider_SetJntSph(play, &keeta->bodySpheres, actor, &body, keeta->bodyElements);
        duel->shield = 1;
    }
}

static int Gomess_CrowdCount(const StandaloneGomess* gomess)
{
    int crowd = 0;
    for (int i = 0; i < GOMESS_BAT_COUNT; ++i)
    {
        const EnemyGomessBat* bat = gomess->bats[i];
        if (bat)
            crowd += bat->crowdState;
        if (crowd >= 5)
            break;
    }
    return crowd;
}

#if defined(GAME_OOT)
void FOREIGN_ENEMY_API_SFX_POSITION(Vec3f*, u16);
void FOREIGN_ENEMY_API_SFX_WORLD(PlayState*, Vec3f*, s32, u16);
#define Gomess_CoreSound FOREIGN_ENEMY_API_SFX_WORLD
void FOREIGN_ENEMY_API_SFX_STOP_POSITION(Vec3f*, u16);
void FOREIGN_ENEMY_API_SFX_VOLUME_TRANSITION(Vec3f*, u16, u16);
#define Gomess_ScythePlay FOREIGN_ENEMY_API_SFX_POSITION
#define Gomess_ScytheStop FOREIGN_ENEMY_API_SFX_STOP_POSITION
#define Gomess_ScytheFadeIn FOREIGN_ENEMY_API_SFX_VOLUME_TRANSITION
#else
#define Gomess_ScythePlay Audio_PlaySfx_AtPos
#define Gomess_ScytheStop ((void (*)(Vec3f*, u16))0x801a7328)
#define Gomess_ScytheFadeIn ((void (*)(Vec3f*, u16, u16))0x801a479c)
#define Gomess_CoreSound SfxSource_PlaySfxAtFixedWorldPos
#endif

enum { GOMESS_SFX_SCYTHE = 0x3aac, GOMESS_SFX_SCYTHE_ROLL, GOMESS_SFX_SCYTHE_FALL, GOMESS_SFX_SCYTHE_GROUND };

static void Gomess_ProjectScytheSound(StandaloneGomess* gomess, PlayState* play)
{
    if (gomess->droppedScythe == 1)
    {
        const MtxF* m = &gomess->droppedScytheMtx;
        gomess->scytheWorldPos =
            (Vec3f){ m->xw - m->xy * 1084.f + m->xz * 7012.f, m->yw - m->yy * 1084.f + m->yz * 7012.f,
                     m->zw - m->zy * 1084.f + m->zz * 7012.f };
        gomess->scytheSoundPoseValid = 1;
    }
    if (gomess->scytheSoundPoseValid)
    {
#if defined(GAME_MM)
        const u32 project = 0x80138c88;
#else
        const u32 project = 0x8008ed08;
#endif
        ((void (*)(MtxF*, Vec3f*, Vec3f*))project)(&play->viewProjectionMtxF, &gomess->scytheWorldPos,
                                                   &gomess->scytheScreenPos);
    }
    else
    {
        gomess->scytheScreenPos = gomess->duel.actor.projectedPos;
    }
}

static int Duel_CanTakeHit(StandaloneDuel* duel, u8 effect)
{
    switch (DUEL_FIXED_FORM)
    {
        case DUEL_WART:
            return ((StandaloneWart*)duel)->eyelid > 3000;
        case DUEL_SNAPPER: {
            Actor* mount = duel->actor.child;
            return !mount || mount->id != ACTOR_ENEMY_SNAPPER_MOUNT || !mount->update ||
                   !((EnemySnapperMount*)mount)->carrying;
        }
        case DUEL_SLIME:
            return duel->exposed != 0;
        case DUEL_KEETA:
            return duel->shield == 0 && !duel->headCooldown;
        default:
            return 1;
    }
}

static void Slime_EndGrabReverb(StandaloneSlime* slime)
{
#if defined(GAME_MM)
    if (slime->grabReverb)
    {
        slime->grabReverb = 0;
        ((void (*)(s8))0x8019f540)(0);
    }
#endif
}

static void Igos_ReleasePlayer(Actor* actor, PlayState* play)
{
    Player* player = GET_PLAYER(play);
    if (DUEL_FIXED_FORM == DUEL_SLIME)
        Slime_EndGrabReverb((StandaloneSlime*)actor);
    Actor* head = DUEL_FIXED_FORM == DUEL_IGOS ? ((StandaloneIgos*)actor)->headTarget : NULL;
#if defined(GAME_MM)
    if (head && ((IgosFlyingHead*)head)->headBiteBlur)
    {
        ((IgosFlyingHead*)head)->headBiteBlur = 0;
        Play_DisableMotionBlur();
    }
#endif
    if (player && player->actor.parent && (player->actor.parent == actor || player->actor.parent == head))
    {
        player->av2.actionVar2 = 101;
        player->actor.parent = NULL;
        if (DUEL_FIXED_FORM == DUEL_IGOS)
        {
            if (head)
                head->flags |= ACTOR_FLAG_ATTENTION_ENABLED;
#if defined(GAME_MM)
            player->csMode = 0;
#else
            player->csAction = 0;
#endif
        }
    }
}

static void Slime_ClearFragments(StandaloneSlime* slime)
{
    for (int i = 0; i < ARRAY_COUNT(slime->fragments); ++i)
    {
        EnemyMinislime* fragment = slime->fragments[i];
        if (!fragment)
            continue;
        slime->fragments[i] = NULL;
        fragment->owner = NULL;
        fragment->ownerSlot = NULL;
        Actor_Kill(&fragment->actor);
    }
}

static void Garo_StopDashAudio(StandaloneGaroMaster* garo);

static void Duel_Destroy(Actor* actor, PlayState* play)
{
    if (DUEL_FIXED_FORM == DUEL_IGOS)
    {
        if ((u16)actor->params == 0x837f)
        {
            if (actor->parent && ((StandaloneIgos*)actor->parent)->headTarget == actor)
            {
                StandaloneIgos* owner = (StandaloneIgos*)actor->parent;
                Igos_ReleasePlayer(actor->parent, play);
                owner->headTarget = NULL;
                if (owner->duel.actor.colChkInfo.health)
                    owner->duel.actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED;
            }
            IgosFlyingHead* head = (IgosFlyingHead*)actor;
            Collider_DestroyCylinder(play, &head->headCollider);
            Collider_DestroyCylinder(play, &head->headAttackCollider);
            return;
        }
        StandaloneIgos* igos = (StandaloneIgos*)actor;
        Collider_DestroyJntSph(play, &igos->sword);
        Collider_DestroyJntSph(play, &igos->shield);
        Collider_DestroyJntSph(play, &igos->bodySpheres);
        Actor* head = igos->headTarget;
        if (head)
        {
            Igos_ReleasePlayer(actor, play);
            ((StandaloneIgos*)actor)->headTarget = NULL;
            head->parent = NULL;
            Actor_Kill(head);
        }
    }
    StandaloneDuel* duel = (StandaloneDuel*)actor;
    if (DUEL_FIXED_FORM == DUEL_GARO)
    {
        StandaloneGaroMaster* garo = (StandaloneGaroMaster*)duel;
        Garo_StopDashAudio(garo);
        for (int i = 0; i < ARRAY_COUNT(garo->swords); ++i)
        {
            Collider_DestroyQuad(play, &garo->swords[i]);
            if (garo->trails[i] < 0 || !Effect_GetByIndex(garo->trails[i]))
                continue;
#if defined(GAME_MM)
            Effect_Destroy(play, garo->trails[i]);
#else
            Effect_Delete(play, garo->trails[i]);
#endif
        }
    }
    if (DUEL_FIXED_FORM == DUEL_IGOS || DUEL_FIXED_FORM == DUEL_SLIME)
        Igos_ReleasePlayer(actor, play);
    if (DUEL_FIXED_FORM == DUEL_KEETA)
        Collider_DestroyJntSph(play, &((StandaloneKeeta*)duel)->bodySpheres);
    if (DUEL_FIXED_FORM == DUEL_SLIME)
    {
        StandaloneSlime* slime = (StandaloneSlime*)duel;
        for (int i = 0; i < ARRAY_COUNT(slime->shell); ++i)
            Collider_DestroyCylinder(play, &slime->shell[i]);
        Slime_ClearFragments(slime);
    }
    if (DUEL_FIXED_FORM == DUEL_SNAPPER)
    {
        Collider_DestroyJntSph(play, &((StandaloneSnapper*)duel)->body);
        if (actor->child && actor->child->id == ACTOR_ENEMY_SNAPPER_MOUNT)
            Actor_Kill(actor->child);
    }
    if (DUEL_FIXED_FORM == DUEL_GOMESS)
    {
        StandaloneGomess* gomess = (StandaloneGomess*)duel;
        for (u16 sfx = GOMESS_SFX_SCYTHE; sfx <= GOMESS_SFX_SCYTHE_GROUND; ++sfx)
            Gomess_ScytheStop(&gomess->scytheScreenPos, sfx);
        if (gomess->scytheTrail >= 0)
        {
#if defined(GAME_MM)
            Effect_Destroy(play, gomess->scytheTrail);
#else
            void Effect_Delete(PlayState*, s32);
            Effect_Delete(play, gomess->scytheTrail);
#endif
        }
        Collider_DestroyJntSph(play, &gomess->core);
        Collider_DestroyQuad(play, &gomess->scythe);
        Collider_DestroyTris(play, &gomess->spin);
        for (int i = 0; i < GOMESS_BAT_COUNT; ++i)
        {
            if (gomess->bats[i])
            {
                gomess->bats[i]->ownerSlot = NULL;
                gomess->bats[i]->followerSlot = NULL;
                gomess->bats[i]->scatterRequest = NULL;
                gomess->bats[i]->swarmHits = NULL;
                gomess->bats[i]->dropCooldown = NULL;
                gomess->bats[i]->deathSfxFrame = NULL;
                gomess->bats[i]->loopSfxFrame = NULL;
                gomess->bats[i]->actor.parent = NULL;
                Actor_Kill(&gomess->bats[i]->actor);
            }
        }
    }
    if (DUEL_FIXED_FORM == DUEL_WART)
    {
        StandaloneWart* wart = (StandaloneWart*)duel;
        Wart_ReleaseHook(wart, play);
        Collider_DestroyJntSph(play, &wart->eye);
        Collider_DestroyJntSph(play, &wart->shell);
        for (int i = 0; i < WART_BUBBLE_COUNT; ++i)
        {
            Collider_DestroyCylinder(play, &wart->bubbleReceive[i]);
            Collider_DestroyCylinder(play, &wart->bubbleAttack[i]);
        }
    }
    Collider_DestroyCylinder(play, &duel->body);
    Collider_DestroyCylinder(play, &duel->weapon);
}

enum {
    GARO_CIRCLE = DUEL_KEETA_LAND + 1,
    GARO_WINDUP,
    GARO_DASH,
    GARO_SLASH,
    GARO_SWING,
    GARO_RETREAT,
    GARO_GUARD,
    GARO_SPIN,
    GARO_STUN,
    GARO_VANISH,
    GARO_FALL,
    GARO_APPEAR
};

#if defined(GAME_OOT)
static int Garo_GuardsSword(PlayState* play, Actor* actor, ColliderQuad* sword)
{
    Player* player = GET_PLAYER(play);
    Actor* link = &player->actor;
    if (!(player->stateFlags1 & (1u << 22)) || !player->currentShield)
        return 0;
    s16 facing = Math_Vec3f_Yaw(&link->world.pos, &actor->world.pos) - link->shape.rot.y;
    if (ABS(facing) >= 0x2000)
        return 0;

    Cylinder16* body = &player->cylinder.dim;
    f32 bottom = body->pos.y + body->yShift;
    f32 top = bottom + body->height;
    f32 radiusSq = (f32)body->radius * body->radius;
    static const u8 corners[] = { 0, 1, 3, 2, 0 };
    for (int edge = 0; edge < ARRAY_COUNT(corners) - 1; ++edge)
    {
        Vec3f* a = &sword->dim.quad[corners[edge]];
        Vec3f* b = &sword->dim.quad[corners[edge + 1]];
        if (MAX(a->y, b->y) < bottom || MIN(a->y, b->y) > top)
            continue;
        f32 lo = 0.f;
        f32 hi = 1.f;
        f32 dy = b->y - a->y;
        if (fabsf(dy) > .01f)
        {
            f32 first = (bottom - a->y) / dy;
            f32 last = (top - a->y) / dy;
            lo = MAX(0.f, MIN(first, last));
            hi = MIN(1.f, MAX(first, last));
        }
        f32 dx = b->x - a->x;
        f32 dz = b->z - a->z;
        f32 lengthSq = dx * dx + dz * dz;
        f32 t = lengthSq > .01f ? ((body->pos.x - a->x) * dx + (body->pos.z - a->z) * dz) / lengthSq : lo;
        t = MAX(lo, MIN(t, hi));
        f32 x = a->x + t * dx - body->pos.x;
        f32 z = a->z + t * dz - body->pos.z;
        if (x * x + z * z <= radiusSq)
            return 1;
    }
    return 0;
}
#endif

static void Duel_Weapon(StandaloneDuel* duel, PlayState* play)
{
    Actor* actor = &duel->actor;
    if (duel->weapon.base.atFlags & (AT_HIT | AT_BOUNCED))
        return;
    if (DUEL_FIXED_FORM == DUEL_GARO)
    {
        StandaloneGaroMaster* garo = (StandaloneGaroMaster*)duel;
#if defined(GAME_OOT)
        for (int i = 0; i < ARRAY_COUNT(garo->swords); ++i)
        {
            if ((garo->poseMask & (1 << i)) && play->gameplayFrames - garo->poseFrame[i] <= 1 &&
                Garo_GuardsSword(play, actor, &garo->swords[i]))
            {
                duel->weapon.base.atFlags |= AT_BOUNCED;
                Duel_Sfx(actor, NA_SE_IT_SHIELD_BOUND);
                return;
            }
        }
#endif
        for (int i = 0; i < ARRAY_COUNT(garo->swords); ++i)
            if ((garo->poseMask & (1 << i)) && play->gameplayFrames - garo->poseFrame[i] <= 1)
                CollisionCheck_SetAT(play, &play->colChkCtx, &garo->swords[i].base);
        return;
    }
    if (DUEL_FIXED_FORM == DUEL_IGOS)
    {
        StandaloneIgos* igos = (StandaloneIgos*)duel;
        if (igos->hasSwordPose && play->gameplayFrames - igos->swordPoseFrame <= 1)
        {
            Player* player = GET_PLAYER(play);
            const Vec3s* center = &igos->swordElement.dim.worldSphere.center;
            Vec3f sword = { center->x, center->y, center->z };
            Vec3f origin = actor->world.pos;
            Vec3f target = player->actor.world.pos;
            origin.y += 30.f;
            target.y += player->cylinder.dim.yShift + player->cylinder.dim.height * .5f;
            if (!enemyFlightClear(play, &origin, &sword))
                return;
            Duel_AC(play, &play->colChkCtx, &igos->sword.base);
            if (!enemyFlightClear(play, &sword, &target))
                return;
#if defined(GAME_OOT)
            f32 radius = igos->swordElement.dim.worldSphere.radius;
            f32 dx = center->x - player->cylinder.dim.pos.x;
            f32 dz = center->z - player->cylinder.dim.pos.z;
            f32 bottom = player->cylinder.dim.pos.y + player->cylinder.dim.yShift;
            f32 reach = radius + player->cylinder.dim.radius;
            s16 facing = Math_Vec3f_Yaw(&player->actor.world.pos, &actor->world.pos) - player->actor.shape.rot.y;
            if (player->currentShield > 0 && (player->stateFlags1 & PLAYER_STATE1_SHIELDING) && ABS(facing) < 0x3000 &&
                dx * dx + dz * dz <= reach * reach && center->y + radius >= bottom &&
                center->y - radius <= bottom + player->cylinder.dim.height)
            {
                if (!igos->swordShieldContact)
                {
                    igos->swordShieldContact = 1;
                    igos->sword.base.atFlags |= AT_BOUNCED;
                    Player_PlaySfx(player, NA_SE_IT_SHIELD_REFLECT_SW);
                }
                return;
            }
#endif
            CollisionCheck_SetAT(play, &play->colChkCtx, &igos->sword.base);
        }
        return;
    }
    duel->weapon.dim.pos.x = actor->world.pos.x + Math_SinS(actor->shape.rot.y) * 48.f;
    duel->weapon.dim.pos.y = actor->world.pos.y;
    duel->weapon.dim.pos.z = actor->world.pos.z + Math_CosS(actor->shape.rot.y) * 48.f;
    CollisionCheck_SetAT(play, &play->colChkCtx, &duel->weapon.base);
}

static void Garo_Change(StandaloneDuel* duel, u8 phase, u8 timer, u16 offset, u8 mode)
{
    AnimationHeader* anim = Duel_Anim(offset);
    duel->phase = phase;
    duel->timer = timer;
    Animation_Change(&duel->skel, anim, phase == GARO_CIRCLE ? 2.f : 1.f, 0.f, Animation_GetLastFrame(anim), mode,
                     -2.f);
}

static void Garo_StartCircle(StandaloneGaroMaster* garo, PlayState* play, u8 timer)
{
    garo->circleAngle = GET_PLAYER(play)->actor.shape.rot.y;
    garo->circleStep = 0x258;
    Garo_Change(&garo->duel, GARO_CIRCLE, timer, 0xb5f0, ANIMMODE_LOOP);
}

s32 BgCheck_EntityLineTest1(CollisionContext*, Vec3f*, Vec3f*, Vec3f*, CollisionPoly**, s32, s32, s32, s32, s32*);
#if defined(GAME_MM) || defined(GARO_MASTER_TELEPORT)
s32 SurfaceType_GetSceneExitIndex(CollisionContext*, CollisionPoly*, s32);
s32 BgCheck_SphVsFirstPoly(CollisionContext*, Vec3f*, f32);
#if defined(GAME_OOT)
s32 SurfaceType_GetMaterial(CollisionContext*, CollisionPoly*, s32);
enum { SURFACE_MATERIAL_LAVA = 7 };
#endif

static int Garo_Blocked(PlayState* play, Vec3f* from, Vec3f* to)
{
    Vec3f hit;
    CollisionPoly* poly;
    s32 bgId;
    return BgCheck_EntityLineTest1(&play->colCtx, from, to, &hit, &poly, 1, 1, 1, 0, &bgId);
}

static __attribute__((noinline)) int Garo_FindLanding(Actor* actor, PlayState* play, Vec3f* landing)
{
    Actor* player = &GET_PLAYER(play)->actor;
    CollisionContext* ctx = &play->colCtx;
    CollisionPoly* poly = player->floorPoly;
    s32 bgId = player->floorBgId;
    if (actor->room != play->roomCtx.curRoom.num || !poly || bgId != BGCHECK_SCENE ||
        (player->bgCheckFlags & (BGCHECKFLAG_GROUND | BGCHECKFLAG_WATER)) != BGCHECKFLAG_GROUND ||
        poly->normal.y < 28000 || SurfaceType_GetFloorType(ctx, poly, bgId) ||
        SurfaceType_GetMaterial(ctx, poly, bgId) == SURFACE_MATERIAL_LAVA ||
        SurfaceType_GetSceneExitIndex(ctx, poly, bgId))
        return 0;

    Vec3f from = actor->world.pos;
    Vec3f center = player->world.pos;
    from.y = actor->floorHeight + 42.f;
    center.y += 42.f;
    if (Garo_Blocked(play, &from, &center))
        return 0;
    *landing = player->world.pos;
    for (f32 height = 42.f; height <= 300.f; height += 30.f)
    {
        center.y = player->world.pos.y + height;
        if (BgCheck_SphVsFirstPoly(ctx, &center, 32.f))
            break;
        landing->y = center.y;
    }
    if (landing->y >= player->world.pos.y + 282.f)
    {
        center.y = player->world.pos.y + 300.f;
        if (!BgCheck_SphVsFirstPoly(ctx, &center, 32.f))
            landing->y = center.y;
    }
    return landing->y >= player->world.pos.y + 102.f;
}
static int Garo_FindLandingFallback(Actor* actor, PlayState* play, Vec3f* landing)
{
    Actor* player = &GET_PLAYER(play)->actor;
    CollisionContext* ctx = &play->colCtx;
    CollisionPoly* floor = player->floorPoly;
    s32 bgId = player->floorBgId;
    if (actor->room != play->roomCtx.curRoom.num || !floor ||
        (player->bgCheckFlags & (BGCHECKFLAG_GROUND | BGCHECKFLAG_WATER)) != BGCHECKFLAG_GROUND ||
        floor->normal.y < 28000 || SurfaceType_GetMaterial(ctx, floor, bgId) == SURFACE_MATERIAL_LAVA ||
        SurfaceType_GetSceneExitIndex(ctx, floor, bgId))
        return 0;

    Vec3f from = player->world.pos;
    from.y += 60.f;
    for (f32 height = 180.f; height >= 120.f; height -= 20.f)
    {
        Vec3f candidate = player->world.pos;
        candidate.y += height;
        if (!Garo_Blocked(play, &from, &candidate) && !BgCheck_SphVsFirstPoly(ctx, &candidate, 32.f))
        {
            *landing = candidate;
            return 1;
        }
    }
    return 0;
}
#endif

static void Garo_SetKnockback(StandaloneGaroMaster* garo, f32 strength)
{
    s16 yaw = garo->duel.actor.yawTowardsPlayer;
    garo->knockback.x = -strength * Math_SinS(yaw);
    garo->knockback.z = -strength * Math_CosS(yaw);
}

static void Garo_MoveKnockback(StandaloneGaroMaster* garo, PlayState* play)
{
    Actor* actor = &garo->duel.actor;
    Vec3f* velocity = &garo->knockback;
    if (!(actor->bgCheckFlags & BGCHECKFLAG_GROUND) || (!velocity->x && !velocity->z))
        return;

    Vec3f from = actor->world.pos;
    from.y += 24.f;
    Vec3f next = { from.x + velocity->x, from.y, from.z + velocity->z };
    if (!enemyFlightClear(play, &from, &next))
    {
        velocity->x = velocity->z = 0.f;
        return;
    }
    CollisionPoly* floor = NULL;
    s32 bgId;
    f32 height = BgCheck_EntityRaycastDown4(&play->colCtx, &floor, &bgId, actor, &next);
    if (!floor || height < actor->floorHeight - 30.f)
    {
        velocity->x = velocity->z = 0.f;
        return;
    }
    actor->world.pos.x = next.x;
    actor->world.pos.z = next.z;
    Math_StepToF(&velocity->x, 0.f, 2.f);
    Math_StepToF(&velocity->z, 0.f, 2.f);
}

static void Garo_JumpBack(StandaloneDuel* duel)
{
    duel->actor.flags &= ~ACTOR_FLAG_LOCK_ON_DISABLED;
    duel->weapon.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
    duel->actor.velocity.y = 20.f;
    duel->actor.world.rot.y = -duel->actor.world.rot.y;
    duel->actor.speed = 7.f;
    Garo_Change(duel, GARO_RETREAT, 0, 0x3d5c, ANIMMODE_ONCE);
}

#if defined(GAME_OOT)
void FOREIGN_ENEMY_API_SFX_STOP_POSITION(Vec3f*, u16);
#endif

static void Garo_StopDashAudio(StandaloneGaroMaster* garo)
{
    if (!garo->dashAudio)
        return;
    garo->dashAudio = 0;
#if defined(GAME_MM)
    ((void (*)(Vec3f*, u16, u8))0x801a0654)(&garo->duel.actor.projectedPos, 0x39be, 0);
#else
    FOREIGN_ENEMY_API_SFX_STOP_POSITION(&garo->duel.actor.projectedPos, 0x39be);
#endif
}

#if defined(GAME_OOT)
void FOREIGN_ENEMY_API_ICE_SHATTER(PlayState*, Actor*, Vec3f*, s32, s32, f32, f32);
#endif

static void Garo_ShatterIce(StandaloneGaroMaster* garo, PlayState* play)
{
    if (garo->damageEffect != 10 && garo->damageEffect != 11)
        return;
    if (garo->damagePointCount)
    {
#if defined(GAME_MM)
        Actor_SpawnIceEffects(play, &garo->duel.actor, garo->damagePoints, garo->damagePointCount, 2, .7f, .4f);
#else
        FOREIGN_ENEMY_API_ICE_SHATTER(play, &garo->duel.actor, garo->damagePoints, garo->damagePointCount, 2, .7f, .4f);
#endif
    }
    garo->damageEffect = 0;
    garo->damageTimer = 0;
}

static void Garo_CheckLedge(StandaloneDuel* duel, PlayState* play)
{
    Actor* actor = &duel->actor;
    if (actor->speed == 0.f || (duel->phase != GARO_WINDUP && duel->phase != GARO_DASH && duel->phase != GARO_RETREAT &&
                                duel->phase != GARO_SPIN && duel->phase != GARO_SLASH))
        return;

    Vec3f probe = actor->world.pos;
    probe.x += actor->speed * Math_SinS(actor->world.rot.y);
    probe.z += actor->speed * Math_CosS(actor->world.rot.y);
    probe.y += 24.f;
    CollisionPoly* floor = NULL;
    s32 bgId;
    f32 height = BgCheck_EntityRaycastDown4(&play->colCtx, &floor, &bgId, actor, &probe);
    if (!floor || height < actor->floorHeight - 30.f)
        actor->speed = 0.f;
}

static void Garo_CheckMovement(Actor* actor, PlayState* play)
{
#if defined(GAME_MM)
    const u32 flags =
        UPDBGCHECKINFO_FLAG_MM_1 | UPDBGCHECKINFO_FLAG_MM_4 | UPDBGCHECKINFO_FLAG_MM_8 | UPDBGCHECKINFO_FLAG_MM_10;
#else
    const u32 flags =
        UPDBGCHECKINFO_FLAG_OOT_0 | UPDBGCHECKINFO_FLAG_OOT_2 | UPDBGCHECKINFO_FLAG_OOT_3 | UPDBGCHECKINFO_FLAG_OOT_4;
#endif
    Actor_UpdateBgCheckInfo(play, actor, 35.f, 60.f, 60.f, flags);
}

static void Garo_Defeat(StandaloneDuel* duel, PlayState* play)
{
    Actor* actor = &duel->actor;
    Garo_StopDashAudio((StandaloneGaroMaster*)duel);
    if (duel->shield == 6)
    {
        if (!duel->timer)
            Actor_Kill(actor);
        return;
    }
    if (!duel->shield)
    {
        Math_SmoothStepToS(&actor->world.rot.y, actor->yawTowardsPlayer, 10, 0xfa0, 0x14);
        actor->shape.rot.y = actor->world.rot.y;
    }
    else
    {
        Math_SmoothStepToS(&actor->shape.rot.y, actor->yawTowardsPlayer, 10, 0xfa0, 0x14);
    }
    if (duel->shield == 0 || duel->shield == 5)
    {
        if (duel->timer)
            goto movement;
    }
    else if (duel->shield == 2 || duel->shield == 3)
    {
        if (!Animation_OnFrame(&duel->skel, duel->skel.endFrame))
            goto movement;
    }
    else if (duel->skel.curFrame < duel->skel.endFrame)
    {
        goto movement;
    }
    switch (duel->shield++)
    {
        case 0:
            Garo_Change(duel, DUEL_DEFEAT, 0, 0x67f0, ANIMMODE_ONCE);
            break;
        case 1:
            Garo_Change(duel, DUEL_DEFEAT, 0, 0x63a4, ANIMMODE_LOOP);
            break;
        case 2:
            break;
        case 3:
            Garo_Change(duel, DUEL_DEFEAT, 0, 0x4018, ANIMMODE_ONCE);
            break;
        case 4:
            duel->timer = 10;
            break;
        case 5: {
            StandaloneGaroMaster* garo = (StandaloneGaroMaster*)duel;
            Vec3f blast = actor->world.pos;
            blast.y += 24.f;
            if (garo->bombPoseValid && play->gameplayFrames - garo->bombPoseFrame <= 1)
                blast = garo->bombPos;
            Actor* bomb = Actor_Spawn(&play->actorCtx, play, ACTOR_EN_BOM, blast.x, blast.y, blast.z, 0, 0, 0, 0);
            if (bomb && bomb->id == ACTOR_EN_BOM && !bomb->init && bomb->update)
            {
                ((EnemyNativeBomb*)bomb)->timer = 0;
            }
            else
            {
                if (bomb && bomb->id == ACTOR_EN_BOM)
                    Actor_Kill(bomb);
                Vec3f velocity = { 0.f, 0.f, 0.f };
                Vec3f accel = { 0.f, .1f, 0.f };
                EffectSsBomb2_SpawnLayered(play, &blast, &velocity, &accel, 100, 19);
                Actor_PlaySfx(actor, NA_SE_IT_BOMB_EXPLOSION);
                duel->weapon.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
                duel->weapon.dim.radius = 65;
                duel->weapon.dim.pos = (Vec3s){ blast.x, blast.y, blast.z };
                CollisionCheck_SetAT(play, &play->colChkCtx, &duel->weapon.base);
            }
            actor->draw = NULL;
            actor->shape.shadowScale = 0.f;
            duel->timer = 2;
            break;
        }
    }
movement:
    Actor_MoveWithGravity(actor);
    Garo_CheckMovement(actor, play);
    if (duel->shield != 6)
    {
        Collider_UpdateCylinder(actor, &duel->body);
        CollisionCheck_SetOC(play, &play->colChkCtx, &duel->body.base);
    }
}

#if defined(GARO_MASTER_TELEPORT)
#if defined(GAME_OOT)
void FOREIGN_ENEMY_API_DAMAGE_EFFECTS(PlayState*, Actor*, Vec3f*, s16, f32, f32, f32, u8);
#define Garo_DrawDamageEffects FOREIGN_ENEMY_API_DAMAGE_EFFECTS
#else
#define Garo_DrawDamageEffects Actor_DrawDamageEffects
#endif

static void Garo_DrawDamage(StandaloneGaroMaster* garo, PlayState* play)
{
    if (!garo->damageTimer || !garo->damagePointCount)
        return;
    if (garo->damageEffect == 10 || garo->damageEffect == 11)
    {
        Math_StepToF(&garo->damageScale, .5f, .3f);
        Math_ApproachF(&garo->steamScale, garo->damageScale, .1f, .04f);
    }
    Garo_DrawDamageEffects(play, &garo->duel.actor, garo->damagePoints, garo->damagePointCount, garo->damageScale,
                           garo->steamScale, garo->damageTimer * .05f, garo->damageEffect);
}
#endif

static void Garo_SlashDust(StandaloneDuel* duel, PlayState* play)
{
    Actor* actor = &duel->actor;
    Actor_SpawnFloorDustRing(play, actor, &actor->world.pos, actor->shape.shadowScale, 1, 8.f, 500, 10, 1);
}

static void Garo_StartSlash(StandaloneDuel* duel, PlayState* play)
{
    Garo_SlashDust(duel, play);
    duel->walking = 0;
    duel->weapon.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
    Math_ApproachF(&duel->actor.speed, 0.f, .3f, 3.f);
    Duel_Sfx(&duel->actor, NA_SE_IT_SWORD_SWING_HARD);
    Garo_Change(duel, GARO_SLASH, 0, 0x378c, ANIMMODE_ONCE);
}

static void Garo_RecordAfterimage(StandaloneGaroMaster* garo)
{
    StandaloneDuel* duel = &garo->duel;
    if (duel->phase != GARO_WINDUP && duel->phase != GARO_DASH && duel->phase != GARO_VANISH &&
        duel->phase != GARO_FALL)
    {
        garo->afterimageCount = 0;
        return;
    }

    Actor* actor = &duel->actor;
    garo->afterimageIndex = (garo->afterimageIndex + 1) % ARRAY_COUNT(garo->afterimages);
    if (garo->afterimageCount < ARRAY_COUNT(garo->afterimages))
        ++garo->afterimageCount;
    int i = garo->afterimageIndex;
    garo->afterimages[i].pos = actor->world.pos;
    garo->afterimages[i].pos.y += 40.f;
    garo->afterimages[i].rot = actor->world.rot;
    if (duel->phase == GARO_WINDUP)
        garo->afterimages[i].rot.x = actor->shape.rot.x;
    memcpy(garo->afterimages[i].joints, duel->joints, sizeof(garo->afterimages[i].joints));
}

static void Garo_Combat(StandaloneDuel* duel, PlayState* play, int finished)
{
    Actor* actor = &duel->actor;
    s16 yaw = actor->yawTowardsPlayer;
    StandaloneGaroMaster* garo = (StandaloneGaroMaster*)duel;
    if (duel->phase != GARO_DASH)
        Garo_StopDashAudio(garo);
    for (int i = 0; i < ARRAY_COUNT(garo->swords); ++i)
    {
        duel->weapon.base.atFlags |= garo->swords[i].base.atFlags & (AT_HIT | AT_BOUNCED);
        garo->swords[i].base.atFlags &= ~(AT_HIT | AT_BOUNCED);
    }
    if (duel->phase != GARO_SLASH && duel->phase != GARO_DASH && duel->phase != GARO_CIRCLE &&
        duel->phase != GARO_RETREAT && duel->phase != GARO_SPIN && duel->phase != GARO_WINDUP)
        actor->speed = 0.f;
    switch (duel->phase)
    {
        case DUEL_HURT:
            if (actor->velocity.y > 0.f || actor->colorFilterTimer || !(actor->bgCheckFlags & BGCHECKFLAG_GROUND))
                break;
            duel->weapon.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
            Duel_Sfx(actor, NA_SE_IT_SWORD_SWING_HARD);
            Garo_Change(duel, GARO_SPIN, 15, 0x72ac, ANIMMODE_LOOP);
            actor->speed = 12.f;
            break;
        case GARO_SPIN:
            actor->shape.rot.y -= 0x1770;
            Actor_SpawnFloorDustRing(play, actor, &actor->world.pos, actor->shape.shadowScale, 1, 4.f, 300, 5, 1);
            Math_SmoothStepToS(&actor->world.rot.y, yaw, 1, 0xfa0, 0x14);
            if (!duel->timer || (duel->weapon.base.atFlags & (AT_HIT | AT_BOUNCED)))
            {
                Garo_JumpBack(duel);
            }
            break;
        case GARO_STUN:
            if (!duel->timer && !garo->damageTimer)
            {
                Garo_ShatterIce(garo, play);
                Garo_JumpBack(duel);
            }
            break;
        case GARO_GUARD:
            if (duel->skel.curFrame >= duel->skel.endFrame)
                Garo_StartCircle(garo, play, duel->timer);
            break;
        case DUEL_APPROACH:
            if (actor->xzDistToPlayer < 340.f && fabsf(actor->yDistanceFromLink) < 85.f)
            {
                if (garo->swordsDrawn)
                {
                    Garo_StartCircle(garo, play, 48);
                }
                else
                {
                    actor->shape.shadowScale = 0.f;
                    Garo_SlashDust(duel, play);
                    Duel_Sfx(actor, 0x3812);
                    Garo_Change(duel, GARO_APPEAR, 0, 0x5778, ANIMMODE_ONCE);
                }
            }
            break;
        case GARO_APPEAR:
            actor->world.rot.y = actor->shape.rot.y = yaw;
            if (Animation_OnFrame(&duel->skel, 18.f))
                Garo_SlashDust(duel, play);
            Math_ApproachF(&actor->shape.shadowScale, 17.f, .4f, 4.f);
            if (Animation_OnFrame(&duel->skel, 45.f))
            {
                garo->swordsDrawn = 1;
                Duel_Sfx(actor, 0x39c7);
                Actor_PlaySfx(actor, NA_SE_EV_FLAME_IGNITION);
            }
            if (garo->swordsDrawn)
            {
                for (int i = 0; i < ARRAY_COUNT(garo->flameScale); ++i)
                    Math_ApproachF(&garo->flameScale[i], .001f * (3 - i), .3f, .0005f);
            }
            if (duel->skel.curFrame >= duel->skel.endFrame)
            {
                garo->swordsDrawn = 1;
                Garo_StartCircle(garo, play, 0);
            }
            break;
        case GARO_CIRCLE:
            if (actor->xzDistToPlayer > 390.f || fabsf(actor->yDistanceFromLink) >= 100.f)
            {
                Garo_Change(duel, DUEL_APPROACH, 0, 0xae80, ANIMMODE_LOOP);
            }
            else
            {
                Duel_Sfx(actor, 0x39bd - SFX_FLAG);
                if (duel->skel.curFrame < duel->skel.endFrame || (actor->bgCheckFlags & BGCHECKFLAG_GROUND))
                    SkelAnime_Update(&duel->skel);
                if (Animation_OnFrame(&duel->skel, 6.f))
                {
                    actor->velocity.y = 10.f;
                    if (!(play->gameplayFrames & 1))
                        Duel_Sfx(actor, 0x39c0);
                }
                if (Animation_OnFrame(&duel->skel, 12.f))
                {
                    actor->speed = 0.f;
                    Duel_Sfx(actor, 0x39c1);
                    if (Rand_ZeroOne() < .5f)
                        garo->circleStep = -garo->circleStep;
                }
                if (!duel->timer)
                {
                    duel->weapon.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
                    Garo_Change(duel, GARO_WINDUP, 0, 0x3530, ANIMMODE_ONCE);
                    actor->world.rot.y = -yaw;
                    actor->speed = 10.f;
                    actor->velocity.y = 20.f;
                    break;
                }
                Math_SmoothStepToS(&actor->shape.rot.y, yaw, 10, 0xfa0, 0x14);
                Math_ApproachF(&actor->speed, 5.f, .3f, 2.f);
                garo->circleAngle += garo->circleStep;
                Vec3f target = GET_PLAYER(play)->actor.world.pos;
                target.x += 200.f * Math_SinS(garo->circleAngle);
                target.z += 200.f * Math_CosS(garo->circleAngle);
                Math_SmoothStepToS(&actor->world.rot.y, Math_Vec3f_Yaw(&actor->world.pos, &target), 10, 0xfa0, 0x14);
                Vec3f from = actor->world.pos;
                from.y += 20.f;
                Vec3f next = { from.x + 24.f * Math_SinS(actor->world.rot.y), from.y,
                               from.z + 24.f * Math_CosS(actor->world.rot.y) };
                if (!enemyFlightClear(play, &from, &next))
                    actor->speed = 0.f;
                else if (actor->bgCheckFlags & BGCHECKFLAG_GROUND)
                {
                    CollisionPoly* floor = NULL;
                    s32 bgId;
                    f32 height = BgCheck_EntityRaycastDown4(&play->colCtx, &floor, &bgId, actor, &next);
                    if (!floor || height < actor->floorHeight - 30.f)
                        actor->speed = 0.f;
                }
            }
            break;
        case GARO_WINDUP:
            actor->shape.rot.x += 0x1770;
            Math_SmoothStepToS(&actor->shape.rot.y, yaw, 10, 0xfa0, 0x14);
            if (actor->bgCheckFlags & BGCHECKFLAG_WALL)
                actor->speed = 0.f;
            if (actor->velocity.y <= 0.f && (actor->bgCheckFlags & BGCHECKFLAG_GROUND))
            {
                actor->shape.rot.x = 0;
                actor->shape.rot.y = actor->world.rot.y = yaw;
                actor->speed = 15.f;
                actor->velocity.y = 13.f;
                Duel_Sfx(actor, 0x3812);
                Duel_Sfx(actor, 0x39be);
                garo->dashAudio = 1;
                duel->phase = GARO_DASH;
                duel->timer = 40;
            }
            break;
        case GARO_DASH: {
            if (duel->weapon.base.atFlags & AT_BOUNCED)
            {
                Garo_StopDashAudio(garo);
                Garo_SetKnockback(garo, 10.f);
                Garo_JumpBack(duel);
                break;
            }
            duel->weapon.base.atFlags &= ~AT_HIT;
            if (actor->velocity.y < 0.f && (actor->bgCheckFlags & BGCHECKFLAG_GROUND))
            {
#if defined(GAME_MM) || defined(GARO_MASTER_TELEPORT)
            if (Rand_ZeroOne() >= .7f)
                {
                    actor->flags |= ACTOR_FLAG_LOCK_ON_DISABLED;
                    Garo_StopDashAudio(garo);
                    Duel_Sfx(actor, 0x39c3);
                    Garo_Change(duel, GARO_VANISH, 20, 0x3530, ANIMMODE_ONCE);
                    actor->speed = 0.f;
                    actor->gravity = 0.f;
                    actor->velocity.y = 10.f;
                    break;
                }
#endif
            actor->velocity.y = 13.f;
            }
            s16 angle = yaw - actor->shape.rot.y;
            if ((actor->bgCheckFlags & BGCHECKFLAG_WALL) ||
                ((finished || duel->skel.curFrame >= duel->skel.endFrame) &&
                 (!duel->timer || actor->xzDistToPlayer < 120.f || ABS(angle) > 0x4300)))
            {
                if (actor->bgCheckFlags & BGCHECKFLAG_WALL)
                    actor->speed = 0.f;
                else
                    Math_ApproachF(&actor->speed, 0.f, .3f, 3.f);
                Garo_StopDashAudio(garo);
                Garo_StartSlash(duel, play);
            }
            break;
        }
        case GARO_SLASH:
            if ((play->gameplayFrames & 7) == 0)
                Garo_SlashDust(duel, play);
            if (actor->bgCheckFlags & BGCHECKFLAG_WALL)
                actor->speed = 0.f;
            else
                Math_ApproachF(&actor->speed, 0.f, .5f, 5.f);
            if (duel->weapon.base.atFlags & AT_HIT)
            {
                duel->walking = 1;
                duel->weapon.base.atFlags &= ~AT_HIT;
            }
            if (duel->weapon.base.atFlags & AT_BOUNCED)
            {
                Garo_StopDashAudio(garo);
                Garo_SetKnockback(garo, 10.f);
                Garo_JumpBack(duel);
                break;
            }
            if (duel->skel.curFrame >= duel->skel.endFrame)
            {
                actor->speed = 0.f;
                Garo_Change(duel, GARO_SWING, duel->walking ? 20 : 40, duel->walking ? 0x38ac : 0x788c, ANIMMODE_LOOP);
            }
            break;
        case GARO_SWING:
            if (!duel->timer)
            {
                Garo_StartCircle(garo, play, 30 + (u8)(Rand_ZeroOne() * 30.f));
            }
            break;
#if defined(GAME_MM) || defined(GARO_MASTER_TELEPORT)
    case GARO_VANISH: {
            actor->shape.rot.y -= 0x1d4c;
            f32 scale = actor->scale.x;
            Math_ApproachF(&scale, 0.f, .3f, .01f);
            Actor_SetScale(actor, scale);
            Math_ApproachF(&actor->shape.shadowScale, 0.f, .3f, 3.f);
            if (!duel->timer)
            {
                Vec3f landing;
                int hasLanding = Garo_FindLanding(actor, play, &landing);
                if (!hasLanding)
                    hasLanding = Garo_FindLandingFallback(actor, play, &landing);
                if (hasLanding)
                {
                    actor->world.pos = actor->prevPos = landing;
                    duel->weapon.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
                    actor->velocity.y = 0.f;
                    actor->bgCheckFlags &= ~(BGCHECKFLAG_GROUND | BGCHECKFLAG_GROUND_TOUCH);
                    actor->gravity = -3.f;
                    Duel_Sfx(actor, 0x39c4);
                    duel->phase = GARO_FALL;
                    break;
                }
                actor->flags &= ~ACTOR_FLAG_LOCK_ON_DISABLED;
                Actor_SetScale(actor, sForms[DUEL_GARO].scale);
                actor->shape.shadowScale = 17.f;
                actor->gravity = -3.f;
                actor->velocity.y = 0.f;
                Garo_StartCircle(garo, play, 32);
            }
            break;
        }
        case GARO_FALL: {
            f32 scale = actor->scale.x;
            Math_ApproachF(&scale, sForms[DUEL_GARO].scale, .3f, .03f);
            Actor_SetScale(actor, scale);
            Math_ApproachF(&actor->shape.shadowScale, 17.f, .4f, 4.f);
            duel->weapon.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
            if (actor->bgCheckFlags & BGCHECKFLAG_GROUND)
            {
                actor->flags &= ~ACTOR_FLAG_LOCK_ON_DISABLED;
                Actor_SetScale(actor, sForms[DUEL_GARO].scale);
                actor->shape.shadowScale = 17.f;
                Garo_StartSlash(duel, play);
            }
            break;
        }
#endif
    case GARO_RETREAT:
            Math_SmoothStepToS(&actor->shape.rot.y, yaw, 10, 0xbb8, 0x14);
            if (actor->bgCheckFlags & (BGCHECKFLAG_GROUND | BGCHECKFLAG_WALL))
                actor->speed = 0.f;
            if (duel->skel.curFrame >= duel->skel.endFrame && (actor->bgCheckFlags & BGCHECKFLAG_GROUND))
            {
                actor->world.rot.x = 0;
                actor->world.rot.y = actor->shape.rot.y;
                actor->velocity.y = 0.f;
                Garo_StartCircle(garo, play, 10 + (u8)(Rand_ZeroOne() * 10.f));
            }
            break;
    }
    if (garo->swordsDrawn && (duel->phase == GARO_DASH || duel->phase == GARO_SPIN || duel->phase == GARO_FALL ||
                              (duel->phase == GARO_SLASH && !duel->walking)))
        Duel_Weapon(duel, play);
}

enum {
    IGOS_ADVANCE,
    IGOS_BLOCK,
    IGOS_SWING,
    IGOS_WINDUP,
    IGOS_HEAVY,
    IGOS_LOW,
    IGOS_RECOVER,
    IGOS_LEAP,
    IGOS_LAND,
    IGOS_BREATH_START,
    IGOS_BREATH,
    IGOS_BREATH_STOP,
    IGOS_DETACH,
    IGOS_REATTACH,
    IGOS_WAIT,
    IGOS_TURN,
    IGOS_BACKJUMP,
    IGOS_STRAFE,
    IGOS_STUN,
    IGOS_RETREAT,
    IGOS_MARCH_WAIT,
    IGOS_MARCH,
    IGOS_MARCH_END
};
enum {
    IGOS_HEAD_ATTACHED,
    IGOS_HEAD_FLOAT,
    IGOS_HEAD_ATTACK,
    IGOS_HEAD_DIVE,
    IGOS_HEAD_BITE,
    IGOS_HEAD_RETREAT,
    IGOS_HEAD_RETURN,
    IGOS_HEAD_ALIGN
};

static void Igos_Action(StandaloneDuel* duel, u8 action, u8 timer, u32 animation, u8 mode)
{
    StandaloneIgos* igos = (StandaloneIgos*)duel;
    if (action != IGOS_LAND || duel->walking != IGOS_LEAP)
        igos->swordShieldContact = 0;
    if (action == IGOS_WAIT)
        igos->waitNeckTarget = igos->neckYaw;
    if (action == IGOS_BREATH_START)
        igos->breathNeckTarget = igos->neckYaw;
    duel->walking = action;
    duel->timer = timer;
    duel->weapon.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
    duel->weapon.elem.atDmgInfo.effect = 0;
    AnimationHeader* anim = Duel_Anim(animation);
    f32 morph = -2.f;
    if (action == IGOS_BREATH_START || action == IGOS_BREATH_STOP || action == IGOS_WINDUP || action == IGOS_LOW ||
        action == IGOS_RETREAT || action == IGOS_DETACH || action == IGOS_WAIT || action == IGOS_ADVANCE)
        morph = -5.f;
    if (action == IGOS_LEAP)
        morph = -3.f;
    if (action == IGOS_HEAVY || action == IGOS_LAND || action == IGOS_BACKJUMP || animation == 0x22cac)
        morph = 0.f;
    if (action == IGOS_BREATH)
        morph = 0.f;
    if (action == IGOS_REATTACH)
        morph = -10.f;
    if (animation == 0xcde0)
        morph = -25.f;
    Animation_Change(&duel->skel, anim, 1.f, 0.f, Animation_GetLastFrame(anim), mode, morph);
}

static void Igos_Breath(StandaloneDuel* duel, PlayState* play)
{
    StandaloneIgos* igos = (StandaloneIgos*)duel;
    Actor* actor = &duel->actor;
    s16 facing = actor->yawTowardsPlayer - actor->shape.rot.y;
    if (ABS(facing) < 0x6000)
        igos->breathNeckTarget = CLAMP(facing, -0x3800, 0x3800);
    if (duel->walking == IGOS_BREATH)
        igos->breathNeckTarget +=
            (s16)(Math_SinS((u16)(play->gameplayFrames + igos->randTimerOffset) * 0x11dcu) * 0x1800);
    s16 yaw = actor->shape.rot.y + igos->neckYaw;
    Math_ApproachS(&igos->neckYaw, igos->breathNeckTarget, 3, 0x1000);
    s16 pitch = actor->shape.rot.x + 0x1600;
    f32 forward = 20.f * Math_CosS(pitch);
    Vec3f velocity = { Math_SinS(yaw) * forward, -20.f * Math_SinS(pitch), Math_CosS(yaw) * forward };
    Vec3f origin = actor->world.pos;
    if (igos->hasBreathOrigin)
    {
        origin.x += igos->breathOrigin.x;
        origin.y += igos->breathOrigin.y;
        origin.z += igos->breathOrigin.z;
    }
    else
    {
        origin.y += 65.f;
    }
    if (duel->timer >= 40)
        duel->breathAlpha = MIN(200, duel->breathAlpha + 5);
    else
        duel->breathAlpha = MAX(0, duel->breathAlpha - 5);
    if (!duel->breathAlpha)
        return;
    if (duel->breathAlpha > 60)
        Duel_Sfx(actor, NA_SE_EN_FREEZAD_BREATH - SFX_FLAG);

    int emitted = 0;
    for (int i = 0; i < IGOS_BREATH_PUFFS && emitted < 2; ++i)
    {
        IgosBreathPuff* puff = &igos->puffs[i];
        if (puff->alpha)
            continue;
        puff->pos = origin;
        puff->velocity = velocity;
        puff->motion = IGOS_PUFF_NEW;
        puff->scale = .002f;
        puff->alpha = duel->breathAlpha;
        puff->age = 0;
        origin.x -= velocity.x * .5f;
        origin.y -= velocity.y * .5f;
        origin.z -= velocity.z * .5f;
        ++emitted;
    }
}

static void Igos_UpdateBreath(StandaloneIgos* igos, PlayState* play)
{
    Actor* actor = &igos->duel.actor;
    Player* player = GET_PLAYER(play);
    if (igos->breathHitTimer)
        --igos->breathHitTimer;

    for (int i = 0; i < IGOS_BREATH_PUFFS; ++i)
    {
        IgosBreathPuff* puff = &igos->puffs[i];
        if (!puff->alpha)
            continue;
        Vec3f next = { puff->pos.x + puff->velocity.x, puff->pos.y + puff->velocity.y, puff->pos.z + puff->velocity.z };
        Vec3f hit;
        CollisionPoly* poly;
        s32 bgId;
        if (BgCheck_EntityLineTest1(&play->colCtx, &puff->pos, &next, &hit, &poly, 1, 1, 1, 0, &bgId))
        {
            if (!poly || poly->normal.y < 0x5800 || puff->velocity.y >= 0.f)
            {
                puff->alpha = 0;
                continue;
            }
            next = hit;
            next.y += 1.f;
            puff->alpha = MIN(puff->alpha, 110);
        }
        puff->pos = next;
        ++puff->age;
        f32 accelerationY = puff->motion == IGOS_PUFF_NEW ? 0.f : puff->motion == IGOS_PUFF_SPREAD ? .3f : 1.f;
        f32 scaleTarget = puff->motion == IGOS_PUFF_SPREAD ? .05f : .025f;
        puff->velocity.y += accelerationY;
        puff->motion = IGOS_PUFF_RISING;
        Math_ApproachF(&puff->scale, scaleTarget, .1f, scaleTarget * .1f);
        if (puff->alpha <= 110 || next.y < actor->floorHeight + 30.f)
        {
            puff->alpha = MIN(puff->alpha, 110);
            puff->motion = IGOS_PUFF_SPREAD;
            puff->velocity.x *= .85f;
            puff->velocity.y = MAX(0.f, puff->velocity.y);
            puff->velocity.z *= .85f;
        }
        puff->alpha = MAX(0, puff->alpha - 10);

        if (!igos->breathHitTimer)
        {
            Vec3f contact = player->actor.world.pos;
            contact.y += 25.f;
            f32 dx = contact.x - next.x;
            f32 dy = contact.y - next.y;
            f32 dz = contact.z - next.z;
            if (dx * dx + dy * dy + dz * dz < 2500.f * puff->scale / .025f && enemyFlightClear(play, &next, &contact))
            {
                igos->breathHitTimer = 10;
#if defined(GAME_MM)
                func_800B8D10(play, actor, 0.f, 0, 0.f, 1, 4);
#else
                if (!player->invincibilityTimer && !Player_InCsMode(play) &&
                    !(player->stateFlags1 & PLAYER_ACTOR_STATE_DEATH))
                {
                    void (*applyDamage)(PlayState*, Player*, s32, f32, f32, s16, s32) = OverlayAddr(0x80835d08);
                    u8 damage = player->actor.colChkInfo.damage;
                    player->actor.colChkInfo.damage = 4;
                    applyDamage(play, player, 0, 0.f, 0.f, 0, 20);
                    player->actor.colChkInfo.damage = damage;
                }
#endif
            }
        }
    }
}

static void Igos_UpdateBodyEffects(StandaloneIgos* igos, PlayState* play)
{
    if (igos->headTarget)
        igos->bodyAlpha = MAX(60, igos->bodyAlpha - 8);
    else
        igos->bodyAlpha = MIN(255, igos->bodyAlpha + 8);
    if (igos->duel.phase == DUEL_IGOS_DOWN)
        igos->shadowAlphaFactor = MAX(0, igos->shadowAlphaFactor - 10);
    else if (igos->duel.phase == DUEL_APPROACH && igos->duel.walking == IGOS_RECOVER &&
             igos->duel.skel.animation == Duel_Anim(0xcde0))
        igos->shadowAlphaFactor = MIN(180, igos->shadowAlphaFactor + 10);
    igos->duel.actor.shape.shadowAlpha = igos->shadowAlphaFactor * igos->bodyAlpha / 255;
    for (int i = 0; i < ARRAY_COUNT(igos->recoveryTrail); ++i)
        igos->recoveryTrail[i].alpha = MAX(0, igos->recoveryTrail[i].alpha - 8);
    StandaloneDuel* duel = &igos->duel;
    if (duel->phase == DUEL_APPROACH && duel->walking == IGOS_RECOVER && duel->timer > 10 &&
        duel->skel.animation == Duel_Anim(0xcde0) && !((play->gameplayFrames + igos->randTimerOffset) & 3))
    {
        IgosBodyImage* image = &igos->recoveryTrail[igos->recoveryTrailNext];
        image->pos = duel->actor.world.pos;
        image->rot = duel->actor.shape.rot;
        memcpy(image->joints, duel->joints, sizeof(image->joints));
        image->alpha = 120;
        igos->recoveryTrailNext = (igos->recoveryTrailNext + 1) % ARRAY_COUNT(igos->recoveryTrail);
    }
    Igos_UpdateBreath(igos, play);
}

static Vec3f Igos_BiteTarget(Player* player)
{
    Vec3f target = player->actor.world.pos;
    s16 yaw = player->actor.shape.rot.y;
    target.x += Math_SinS(yaw) * 7.f;
    target.z += Math_CosS(yaw) * 7.f;
#if defined(GAME_MM)
    static const f32 heights[] = { 85.f, 67.f, 60.f, 34.f, 40.f };
    u8 form = player->transformation;
    target.y += heights[form < ARRAY_COUNT(heights) ? form : MM_PLAYER_FORM_HUMAN];
#else
    target.y = player->actor.focus.pos.y;
#endif
    return target;
}

void FOREIGN_ENEMY_API_KNOCKBACK_LARGE(PlayState*, Actor*, f32, s16, f32, u32);

static int Igos_MoveHead(IgosFlyingHead* head, PlayState* play, Vec3f* next)
{
    Vec3f* pos = &head->actor.world.pos;
    if (!enemyFlightClear(play, pos, next))
        return 0;
    *pos = *next;
    return 1;
}

static void Igos_AdvanceHead(const IgosFlyingHead* head, Vec3f* next)
{
    if (!head->headSpeed)
        return;
    f32 forward = head->headSpeed * Math_CosS(head->headTravelPitch);
    next->x += forward * Math_SinS(head->headTravelYaw);
    next->y += head->headSpeed * Math_SinS(head->headTravelPitch);
    next->z += forward * Math_CosS(head->headTravelYaw);
}

static void Igos_FlightStep(IgosFlyingHead* head, Vec3f* next, Vec3f* target)
{
    f32 dx = target->x - next->x;
    f32 dz = target->z - next->z;
    Math_ApproachS(&head->headTravelYaw, Math_Vec3f_Yaw(next, target), 10, 0x800);
    f32 horizontal = sqrtf(dx * dx + dz * dz);
    f32 vertical = target->y - next->y;
#if defined(GAME_OOT)
    s16 pitch = Math_Atan2S(horizontal, vertical);
#else
    s16 pitch = Math_Atan2S(vertical, horizontal);
#endif
    Math_ApproachS(&head->headTravelPitch, pitch, 10, 0x800);
    Igos_AdvanceHead(head, next);
}

static void Igos_RegisterHead(IgosFlyingHead* igos, PlayState* play)
{
    if (igos->headAction < IGOS_HEAD_FLOAT || igos->headAction > IGOS_HEAD_ALIGN)
        return;

    int biting = igos->headAction == IGOS_HEAD_BITE;
    ColliderCylinder* head = biting ? &igos->headCollider : &igos->headAttackCollider;
    Vec3f pos = biting ? GET_PLAYER(play)->actor.world.pos : igos->actor.focus.pos;
    head->dim.pos = (Vec3s){ pos.x, pos.y, pos.z };
    Duel_AC(play, &play->colChkCtx, &head->base);
    if (!biting && igos->headAction != IGOS_HEAD_DIVE)
        CollisionCheck_SetAT(play, &play->colChkCtx, &head->base);
}

static void Igos_FloatHead(IgosFlyingHead* igos, PlayState* play, Player* player)
{
    Vec3f* target = &igos->chasePoint;
    Vec3f* pos = &igos->actor.world.pos;
    if (igos->headRetargetTimer)
        --igos->headRetargetTimer;
    if (!igos->headRetargetTimer || Math_Vec3f_DistXYZ(pos, target) < 50.f)
    {
        igos->headRetargetTimer = 50 + (u8)(Rand_ZeroOne() * 50.f);
        *target = player->actor.world.pos;
        if (Rand_ZeroOne() < .2f)
        {
            target->y += 25.f;
        }
        else
        {
            target->x += Rand_ZeroOne() * 500.f - 250.f;
            target->y += 50.f + Rand_ZeroOne() * 100.f;
            target->z += Rand_ZeroOne() * 500.f - 250.f;
        }
    }
    Vec3f next = *pos;
    Math_StepToF(&igos->headSpeed, 5.f, 5.f);
    Igos_FlightStep(igos, &next, target);
    if (!Igos_MoveHead(igos, play, &next))
        igos->headRetargetTimer = 0;
    Vec3f playerPos = player->actor.world.pos;
    playerPos.y += 34.f;
    Math_ApproachS(&igos->headYaw, Math_Vec3f_Yaw(pos, &playerPos), 10, 0x200);
    Math_ApproachS(&igos->headPitch, Duel_Pitch(pos, &playerPos), 10, 0x200);
}

static void Igos_UpdateHeadTrail(IgosFlyingHead* igos)
{
    for (int i = 0; i < ARRAY_COUNT(igos->headTrail); ++i)
        igos->headTrail[i].alpha = MAX(0, igos->headTrail[i].alpha - 15);
    if (igos->headAction == IGOS_HEAD_ATTACHED || igos->headAction == IGOS_HEAD_BITE)
        return;

    igos->headTrail[igos->headTrailNext] = (IgosHeadImage){
        igos->actor.world.pos, igos->headYaw, igos->headPitch, igos->headJaw, 100,
    };
    if (++igos->headTrailNext == ARRAY_COUNT(igos->headTrail))
        igos->headTrailNext = 0;
}

static Vec3f Igos_HeadSocket(Actor* body, PlayState* play)
{
    StandaloneIgos* igos = (StandaloneIgos*)body;
    Vec3f from = body->world.pos;
    Vec3f target = from;
    from.y += 15.f;
    if (igos->hasHeadSocket && play->gameplayFrames - igos->headSocketFrame <= 1)
    {
        target.x += igos->headSocketOffset.x;
        target.y += igos->headSocketOffset.y;
        target.z += igos->headSocketOffset.z;
    }
    else
    {
        target.y += 75.f;
    }
    Vec3f hit;
    CollisionPoly* poly;
    s32 bgId;
    if (BgCheck_EntityLineTest1(&play->colCtx, &from, &target, &hit, &poly, 1, 1, 1, 0, &bgId))
    {
        f32 distance = Math_Vec3f_DistXYZ(&from, &hit);
        f32 fraction = distance > 8.f ? 1.f - 8.f / distance : 0.f;
        target = (Vec3f){ from.x + (hit.x - from.x) * fraction, from.y + (hit.y - from.y) * fraction,
                          from.z + (hit.z - from.z) * fraction };
    }
    return target;
}

static void Igos_SetHeadTarget(StandaloneIgos* igos, PlayState* play, int active)
{
    Actor* body = &igos->duel.actor;
    Actor* actor = igos->headTarget;
    Player* player = GET_PLAYER(play);
#if defined(GAME_MM)
    Actor** lock = &player->lockOnActor;
#else
    Actor** lock = &player->focusActor;
#endif
    if (!active)
    {
        if (actor)
        {
            Igos_ReleasePlayer(body, play);
            if (*lock == actor)
                *lock = body->colChkInfo.health ? body : NULL;
            igos->headTarget = NULL;
            actor->parent = NULL;
            if (igos->duel.phase != DUEL_DEFEAT && igos->duel.walking == IGOS_REATTACH)
            {
                IgosFlyingHead* head = (IgosFlyingHead*)actor;
                head->headAction = IGOS_HEAD_ATTACHED;
                head->headTimer = ARRAY_COUNT(head->headTrail);
                actor->flags &= ~(ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE);
            }
            else
            {
                Actor_Kill(actor);
            }
        }
        return;
    }
    Vec3f pos = Igos_HeadSocket(body, play);
    actor = Actor_Spawn(&play->actorCtx, play, body->id, pos.x, pos.y, pos.z, 0, body->shape.rot.y, 0, 0x837f);
    if (!actor)
        return;

    actor->parent = body;
    actor->room = body->room;
    actor->attentionRangeType = body->attentionRangeType;
    actor->focus.pos = pos;
    igos->headTarget = actor;
    IgosFlyingHead* head = (IgosFlyingHead*)actor;
    head->headYaw = head->headTravelYaw = body->shape.rot.y;
    head->headTravelPitch = 0x2000;
    head->headAction = IGOS_HEAD_FLOAT;
    head->headTimer = 200;
    head->headHitCooldown = 10;
    head->headRetargetTimer = 20;
    head->chasePoint =
        (Vec3f){ pos.x + 500.f * Math_SinS(head->headYaw), pos.y + 500.f, pos.z + 500.f * Math_CosS(head->headYaw) };
    Vec3f hit;
    CollisionPoly* poly;
    s32 bgId;
    if (BgCheck_EntityLineTest1(&play->colCtx, &pos, &head->chasePoint, &hit, &poly, 1, 1, 1, 0, &bgId))
    {
        f32 distance = Math_Vec3f_DistXYZ(&pos, &hit);
        f32 reach = distance > 18.f ? (distance - 18.f) / distance : 0.f;
        head->chasePoint = (Vec3f){ pos.x + (hit.x - pos.x) * reach, pos.y + (hit.y - pos.y) * reach,
                                    pos.z + (hit.z - pos.z) * reach };
        if (distance < 68.f)
            head->headRetargetTimer = 0;
    }
    body->flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    if (*lock == body)
        *lock = actor;
    Duel_Sfx(body, 0x3a21);
}

static void Igos_RetreatHead(IgosFlyingHead* head)
{
    head->headAction = IGOS_HEAD_RETREAT;
    head->headTimer = 30;
    head->headRetargetTimer = 0;
}

static void Igos_UpdateHead(IgosFlyingHead* igos, StandaloneIgos* owner, PlayState* play)
{
    StandaloneDuel* duel = &owner->duel;
    Actor* actor = &duel->actor;
    Player* player = GET_PLAYER(play);
    Actor* grabOwner = &igos->actor;
    ++igos->randTimer;
    if (igos->headHitCooldown)
        --igos->headHitCooldown;
    if (igos->headTimer)
        --igos->headTimer;
    if (igos->headAction == IGOS_HEAD_DIVE)
        Math_ApproachS(&igos->headJaw, 0xfa0, 5, 0x1000);
    else
        igos->headJaw =
            (igos->headAction == IGOS_HEAD_BITE ? 3000 : 1000) + (s16)(1000.f * Math_SinS(igos->randTimer * 0x1200u));
    igos->headAttackCollider.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
    igos->headAttackCollider.base.acFlags &= ~(AC_HIT | AC_BOUNCED);
    switch (igos->headAction)
    {
        case IGOS_HEAD_FLOAT:
            Igos_FloatHead(igos, play, player);
            if (igos->headTimer == 40)
            {
                Duel_Sfx(grabOwner, 0x3a31);
                igos->headAction = IGOS_HEAD_ATTACK;
                igos->headTimer = 250;
            }
            break;
        case IGOS_HEAD_ATTACK:
        case IGOS_HEAD_DIVE: {
            int diving = igos->headAction == IGOS_HEAD_DIVE;
            Vec3f target = player->actor.world.pos;
            Vec3f next = igos->actor.world.pos;
            f32 dx = target.x - next.x;
            f32 dz = target.z - next.z;
            f32 horizontal = sqrtf(dx * dx + dz * dz);
            int enterDive = !diving && igos->headTimer && igos->headTimer <= 190 && horizontal < 100.f;
            if (diving)
            {
                target = Igos_BiteTarget(player);
                Math_StepToF(&igos->headSpeed, 0.f, 1.f);
                Math_ApproachF(&next.x, target.x, 1.f, igos->headDiveStep);
                Math_ApproachF(&next.y, target.y, 1.f, igos->headDiveStep);
                Math_ApproachF(&next.z, target.z, 1.f, igos->headDiveStep);
                Math_StepToF(&igos->headDiveStep, 20.f, .5f);
                Igos_AdvanceHead(igos, &next);
            }
            else
            {
                target.y += 100.f;
                Math_StepToF(&igos->headSpeed, 10.f, 1.f);
                Igos_FlightStep(igos, &next, &target);
            }
            int interrupted = !igos->headTimer;
#if defined(GAME_MM)
        if (diving && (player->stateFlags3 & 0x1000))
                interrupted = 1;
#endif
        if (!Igos_MoveHead(igos, play, &next) || interrupted)
            {
                Igos_RetreatHead(igos);
                break;
            }
            if (enterDive)
            {
                igos->headAction = IGOS_HEAD_DIVE;
                igos->headTimer = 60;
                igos->headDiveStep = 0.f;
            }
            Vec3f focus = player->actor.world.pos;
            focus.y += 34.f;
            Math_ApproachS(&igos->headYaw, Math_Vec3f_Yaw(&next, &focus), 10, 0x200);
            Math_ApproachS(&igos->headPitch, Duel_Pitch(&next, &focus), 10, 0x200);
            if (diving && horizontal < 20.f && fabsf(target.y - next.y) < 25.f &&
                enemyFlightClear(play, &next, &target))
            {
                if (player->actor.parent || !play->grabPlayer(play, player))
                {
                    igos->headTimer = 0;
                    break;
                }
                player->actor.parent = grabOwner;
#if defined(GAME_MM)
                Player_PlaySfx(player, NA_SE_VO_LI_DAMAGE_S + player->ageProperties->voiceSfxIdOffset);
#else
                Player_PlaySfx(player, comboIsLinkAdult()
                    ? NA_SE_VO_LI_DAMAGE_S
                    : NA_SE_VO_LI_DAMAGE_S_KID);
#endif
#if defined(GAME_MM)
            Play_EnableMotionBlur(150);
                igos->headBiteBlur = 1;
#endif
            grabOwner->flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
                igos->headAction = IGOS_HEAD_BITE;
                igos->headTimer = 80;
            }
            break;
        }
        case IGOS_HEAD_BITE: {
            if (player->actor.parent != grabOwner)
            {
                grabOwner->flags |= ACTOR_FLAG_ATTENTION_ENABLED;
                Igos_ReleasePlayer(actor, play);
                Igos_RetreatHead(igos);
                break;
            }
            if (play->state.input[0].press.button & (BTN_A | BTN_B))
                igos->headTimer = igos->headTimer > 5 ? igos->headTimer - 5 : 0;
            Vec3f target = Igos_BiteTarget(player);
            Vec3f next = igos->actor.world.pos;
            Math_ApproachF(&next.x, target.x, 1.f, igos->headDiveStep);
            Math_ApproachF(&next.y, target.y, 1.f, igos->headDiveStep);
            Math_ApproachF(&next.z, target.z, 1.f, igos->headDiveStep);
            Math_StepToF(&igos->headDiveStep, 20.f, .5f);
            Igos_AdvanceHead(igos, &next);
            if (!Igos_MoveHead(igos, play, &next))
                igos->headTimer = 0;
            player->av2.actionVar2 = 90;
            Math_ApproachS(&igos->headYaw, player->actor.shape.rot.y + 0x8000, 2, 0x1000);
            Math_ApproachS(&igos->headPitch, 0x4000, 2, 0x1000);
            Duel_Sfx(grabOwner, 0x3a51 - SFX_FLAG);
            if (!igos->headTimer)
            {
                Igos_ReleasePlayer(actor, play);
                grabOwner->flags |= ACTOR_FLAG_ATTENTION_ENABLED;
                Duel_Sfx(grabOwner, 0x3a45);
                Igos_RetreatHead(igos);
            }
            break;
        }
        case IGOS_HEAD_RETREAT:
            Igos_FloatHead(igos, play, player);
            if (!igos->headTimer)
            {
                igos->headAction = IGOS_HEAD_RETURN;
                igos->returnRoute = (EnemyFlightReturn){ 0 };
            }
            break;
        case IGOS_HEAD_RETURN:
        case IGOS_HEAD_ALIGN: {
            int aligning = igos->headAction == IGOS_HEAD_ALIGN;
            if (aligning)
                igos->headHitCooldown = 10;
            Vec3f target = Igos_HeadSocket(actor, play);
            Vec3f next = igos->actor.world.pos;
            f32 dx = next.x - target.x;
            f32 dy = next.y - target.y;
            f32 dz = next.z - target.z;
            f32 distanceSq = dx * dx + dy * dy + dz * dz;
            if (aligning)
                Math_StepToF(&igos->headSpeed, 0.f, .1f);
            else if (distanceSq < 100.f * 100.f)
                Math_StepToF(&igos->headSpeed, 1.5f, .3f);
            f32 step = aligning ? 4.f : igos->headSpeed;
            Math_ApproachF(&next.x, target.x, .5f, step);
            Math_ApproachF(&next.y, target.y, .5f, step);
            Math_ApproachF(&next.z, target.z, .5f, step);
            if (aligning)
            {
                Math_ApproachS(&igos->headYaw, actor->shape.rot.y, 5, 0x800);
                Math_ApproachS(&igos->headPitch, 0, 5, 0x800);
            }
            Vec3f* pos = &igos->actor.world.pos;
            Vec3f previous = *pos;
            int searched = !igos->returnRoute.hasWaypoint && !igos->returnRoute.retry;
            enemyFlightReturn(play, &igos->returnRoute, pos, &target, &next, distanceSq, 1, 0, enemyFlightClear);
            int blocked = searched && !igos->returnRoute.hasWaypoint && pos->x == previous.x && pos->y == previous.y &&
                          pos->z == previous.z && !enemyFlightClear(play, pos, &target);
            if (!aligning && duel->phase == DUEL_APPROACH && fabsf(pos->x - target.x) < 30.f &&
                fabsf(pos->y - target.y) < 30.f && fabsf(pos->z - target.z) < 30.f &&
                enemyFlightClear(play, &igos->actor.world.pos, &target))
            {
                igos->headAction = IGOS_HEAD_ALIGN;
                igos->headHitCooldown = 10;
                Igos_Action(duel, IGOS_REATTACH, 60, 0x79d4, ANIMMODE_ONCE);
                duel->skel.playSpeed = 0.f;
            }
            else if (blocked)
            {
                if (aligning && duel->walking == IGOS_REATTACH)
                    Igos_Action(duel, IGOS_WAIT, 20 + (u8)(Rand_ZeroOne() * 50.f), 0x40e0, ANIMMODE_LOOP);
                Igos_RetreatHead(igos);
            }
            break;
        }
    }
    if (igos->headCollider.base.acFlags & AC_HIT)
    {
        igos->headCollider.base.acFlags &= ~AC_HIT;
        int canReact = !igos->headHitCooldown;
        if (canReact)
            igos->headHitCooldown = 15;
        if (canReact && igos->headAction == IGOS_HEAD_BITE && player->actor.parent == grabOwner)
        {
            Igos_ReleasePlayer(actor, play);
            FOREIGN_ENEMY_API_KNOCKBACK_LARGE(play, grabOwner, 10.f, actor->yawTowardsPlayer, 5.f, 16);
            Duel_Sfx(grabOwner, 0x3a45);
            Igos_RetreatHead(igos);
        }
    }
    if (igos->headAction != IGOS_HEAD_ATTACHED && igos->headAction != IGOS_HEAD_BITE)
        Duel_Sfx(grabOwner, 0x3a52 - SFX_FLAG);
    grabOwner->focus.pos = grabOwner->world.pos;
    grabOwner->shape.rot.y = igos->headYaw;
    grabOwner->shape.rot.x = igos->headPitch;
    if (igos->headAction == IGOS_HEAD_BITE)
    {
        grabOwner->flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
#if defined(GAME_MM)
        if (player->lockOnActor == grabOwner)
            player->lockOnActor = NULL;
#else
        if (player->focusActor == grabOwner)
            ((void (*)(Player*))0x800798c4)(player);
#endif
    }
    else
        grabOwner->flags |= ACTOR_FLAG_ATTENTION_ENABLED;
    Igos_RegisterHead(igos, play);
    Igos_UpdateHeadTrail(igos);
}

static int Igos_VoiceVariant(const Actor* actor)
{
    u16 params = actor->params;
    return params == 0x817f ? 1 : params == 0x827f ? 2 : 0;
}

static void Igos_AttackVoice(Actor* actor)
{
    static const u16 voices[] = { 0x3a4a, 0x3a49, 0x3a48 };
    Duel_Sfx(actor, voices[Igos_VoiceVariant(actor)]);
}

static void Igos_DeathSound(Actor* actor, int finishing)
{
    static const u16 fallSounds[] = { 0x3a3d, 0x3a3f, 0x3a3e };
    static const u16 deathVoices[] = { 0x3a5b, 0x3a5d, 0x3a5c };
    int knight = Igos_VoiceVariant(actor);
    Duel_Sfx(actor, finishing ? deathVoices[knight] : fallSounds[knight]);
}

static void Igos_SetHitDrift(StandaloneIgos* igos, f32 speed)
{
    s16 yaw = igos->duel.actor.yawTowardsPlayer;
    igos->animDrift = (Vec3f){ speed * Math_SinS(yaw), 0.f, speed * Math_CosS(yaw) };
    igos->animDriftDelay = 1;
}

static void Igos_ApplyAnimDrift(StandaloneIgos* igos, PlayState* play)
{
    Actor* actor = &igos->duel.actor;
    Vec3f* drift = &igos->animDrift;
    if (igos->animDriftDelay)
        return;
    if (drift->x || drift->z)
    {
        Vec3f from = actor->world.pos;
        from.y += 24.f;
        Vec3f next = { from.x + drift->x, from.y, from.z + drift->z };
        CollisionPoly* floor = NULL;
        s32 bgId;
        f32 height = BgCheck_EntityRaycastDown4(&play->colCtx, &floor, &bgId, actor, &next);
        if (floor && fabsf(height - actor->floorHeight) <= 25.f && enemyFlightClear(play, &from, &next))
        {
            actor->world.pos.x = next.x;
            actor->world.pos.z = next.z;
        }
        else
        {
            drift->x = drift->z = 0.f;
        }
    }
}

static void Igos_DecayAnimDrift(StandaloneIgos* igos)
{
    Actor* actor = &igos->duel.actor;
    int grounded = actor->bgCheckFlags & BGCHECKFLAG_GROUND;
    if (grounded && actor->velocity.y <= 0.f)
        actor->velocity.y = -2.f;
    if (igos->animDriftDelay)
    {
        --igos->animDriftDelay;
        return;
    }
    if (grounded)
    {
        Math_StepToF(&igos->animDrift.x, 0.f, 1.f);
        Math_StepToF(&igos->animDrift.z, 0.f, 1.f);
    }
}

static void Igos_MoveFallen(StandaloneIgos* igos, PlayState* play)
{
    StandaloneDuel* duel = &igos->duel;
    Actor* actor = &duel->actor;
    Math_StepToF(&actor->speed, 0.f, 1.f);
    Actor_MoveWithGravity(actor);
    Igos_ApplyAnimDrift(igos, play);
    Actor_UpdateBgCheckInfo(play, actor, 24.f, 32.f, 65.f, 5);
    Igos_DecayAnimDrift(igos);

    if (duel->timer < 140 && (actor->bgCheckFlags & BGCHECKFLAG_GROUND))
    {
        int direction = igos->fallenBackwards ? 1 : -1;
        f32 distance = (u16)actor->params == 0x807f ? 75.f : 65.f;
        Vec3f from = actor->world.pos;
        from.y += 18.f;
        Vec3f end = { from.x - distance * direction * Math_SinS(actor->shape.rot.y), from.y + 82.f,
                      from.z - distance * direction * Math_CosS(actor->shape.rot.y) };
        CollisionPoly* floor = NULL;
        s32 bgId;
        f32 height = BgCheck_EntityRaycastDown4(&play->colCtx, &floor, &bgId, actor, &end);
        s16 pitch = 0;
        f32 heightDelta = height - actor->world.pos.y;
        end.y = height + 18.f;
        if (floor && fabsf(heightDelta) <= 75.f && enemyFlightClear(play, &from, &end))
            pitch = gomessBatAtan2S(heightDelta, distance) * direction;
        Math_ApproachS(&actor->shape.rot.x, pitch, 1, 0x800);
    }
}

static void Igos_StartElemental(StandaloneIgos* igos, u8 effect)
{
    switch (effect)
    {
        case IGOS_DAMAGE_FIRE:
            igos->elementalType = IGOS_EFFECT_FIRE;
            igos->elementalTimer = 80;
            break;
        case IGOS_DAMAGE_ICE:
            igos->elementalType = IGOS_EFFECT_ICE_STEAM;
            igos->elementalTimer = 80;
            break;
        case IGOS_DAMAGE_LIGHT:
            igos->elementalType = IGOS_EFFECT_LIGHT;
            igos->elementalTimer = 40;
            break;
        case IGOS_DAMAGE_ELECTRIC:
            igos->elementalType = IGOS_EFFECT_ELECTRIC;
            igos->elementalTimer = 50;
            break;
        default:
            return;
    }
    igos->elementalAlpha = 1.f;
    igos->elementalScale = effect == IGOS_DAMAGE_ELECTRIC ? .5f : 0.f;
    igos->iceSteamScale = 1.f;
}

static void Igos_ShatterIce(StandaloneIgos* igos, PlayState* play)
{
    if (play->gameplayFrames - igos->fallenPoseFrame > 1)
        return;
    static Color_RGBA8 prim = { 170, 255, 255, 255 };
    static Color_RGBA8 env = { 200, 200, 255, 255 };
    static Vec3f accel = { 0.f, -1.f, 0.f };
    if (igos->fallenPoseMask & (1 << 8))
        SfxSource_PlaySfxAtFixedWorldPos(play, &igos->fallenParts[8], 30, NA_SE_EV_ICE_BROKEN);
    for (int i = 0; i < ARRAY_COUNT(igos->fallenParts); ++i)
    {
        if (!(igos->fallenPoseMask & (1 << i)))
            continue;
        Vec3f velocity = { (Rand_ZeroOne() - .5f) * 7.f, Rand_ZeroOne() * 6.f + 4.f, (Rand_ZeroOne() - .5f) * 7.f };
        Vec3f pos = { igos->fallenParts[i].x + velocity.x, igos->fallenParts[i].y + velocity.y,
                      igos->fallenParts[i].z + velocity.z };
        EffectSsEnIce_Spawn(play, &pos, .7f + Rand_ZeroOne() * .5f, &velocity, &accel, &prim, &env, 30);
    }
}

static void Igos_UpdateElemental(StandaloneIgos* igos, PlayState* play)
{
    if (!igos->elementalAlpha)
        return;
    if (igos->elementalTimer)
        --igos->elementalTimer;
    switch (igos->elementalType)
    {
        case IGOS_EFFECT_FIRE:
            if (igos->elementalTimer)
                Math_ApproachF(&igos->elementalScale, .5f, .1f, .5f);
            else
                Math_StepToF(&igos->elementalAlpha, 0.f, .02f);
            break;
        case IGOS_EFFECT_ICE_STEAM:
        case IGOS_EFFECT_ICE_QUIET:
            if (!igos->elementalTimer)
            {
                Igos_ShatterIce(igos, play);
                igos->elementalAlpha = 0.f;
            }
            else
            {
                if (igos->elementalTimer == 50)
                    igos->elementalType = IGOS_EFFECT_ICE_QUIET;
                Math_StepToF(&igos->elementalScale, .5f, .08f);
                Math_ApproachF(&igos->iceSteamScale, .5f, .05f, .05f);
            }
            break;
        case IGOS_EFFECT_LIGHT:
            if (igos->elementalTimer)
                Math_ApproachF(&igos->elementalScale, .75f, .5f, .5f);
            else if (Math_StepToF(&igos->elementalScale, 0.f, .03f))
                igos->elementalAlpha = 0.f;
            break;
        case IGOS_EFFECT_ELECTRIC:
            if (!igos->elementalTimer && Math_StepToF(&igos->elementalScale, 0.f, .05f))
                igos->elementalAlpha = 0.f;
            break;
    }
}

static void Igos_FootDust(StandaloneIgos* igos, PlayState* play, int count)
{
    Actor* actor = &igos->duel.actor;
    if (igos->headTarget || play->gameplayFrames - igos->footPoseFrame > 1)
        return;
    static Color_RGBA8 prim = { 60, 50, 20, 255 };
    static Color_RGBA8 env = { 40, 30, 30, 255 };
    Vec3f accel = { 0.f, -.1f, 0.f };
    for (int i = 0; i < count; ++i)
    {
        for (int foot = 0; foot < ARRAY_COUNT(igos->footPos); ++foot)
        {
            if (!(igos->footPoseMask & (1 << foot)))
                continue;
            Vec3f pos = igos->footPos[foot];
            Vec3f velocity = { (Rand_ZeroOne() - .5f) * 3.f, Rand_ZeroOne(), (Rand_ZeroOne() - .5f) * 3.f };
            pos.x += (Rand_ZeroOne() - .5f) * 10.f;
            pos.y = actor->floorHeight - 5.f + Rand_ZeroOne() * 5.f;
            pos.z += (Rand_ZeroOne() - .5f) * 10.f;
            s16 scale = 150.f + Rand_ZeroOne() * 80.f;
            s16 life = 14.f + Rand_ZeroOne() * 5.f;
#if defined(GAME_MM)
            func_800B0EB0(play, &pos, &velocity, &accel, &prim, &env, scale, 10, life);
#else
            EffectSsDust_Spawn(play, 0, &pos, &velocity, &accel, &prim, &env, scale, 10, life, 0);
#endif
        }
    }
}

static void Igos_FastWalkEffects(StandaloneIgos* igos, PlayState* play)
{
    static const u16 sounds[] = { 0x3a26, 0x3a28, 0x3a27 };
    StandaloneDuel* duel = &igos->duel;
    Actor* actor = &duel->actor;
    if (Animation_OnFrame(&duel->skel, 0.f) || Animation_OnFrame(&duel->skel, 5.f))
        Duel_Sfx(actor, sounds[Igos_VoiceVariant(actor)]);
    if (!igos->headTarget && !((play->gameplayFrames + igos->randTimerOffset) & 3) &&
        (actor->speed > 1.f || igos->animDrift.x > 1.f || igos->animDrift.z > 1.f) &&
        (actor->bgCheckFlags & BGCHECKFLAG_GROUND))
        Igos_FootDust(igos, play, 1);
}

static int Igos_TryBackjump(StandaloneDuel* duel, PlayState* play, int special)
{
    StandaloneIgos* igos = (StandaloneIgos*)duel;
    Actor* actor = &duel->actor;
    if ((actor->bgCheckFlags & BGCHECKFLAG_WALL) || !(actor->bgCheckFlags & BGCHECKFLAG_GROUND))
        return 0;
    f32 sin = Math_SinS(actor->yawTowardsPlayer);
    f32 cos = Math_CosS(actor->yawTowardsPlayer);
    Vec3f from = actor->world.pos;
    from.y += 24.f;
    f32 travel = special ? 120.f : -70.f;
    Vec3f landing = { from.x + travel * sin, from.y, from.z + travel * cos };
    CollisionPoly* floor = NULL;
    s32 bgId;
    f32 height = BgCheck_EntityRaycastDown4(&play->colCtx, &floor, &bgId, actor, &landing);
    if (!floor || fabsf(height - actor->world.pos.y) > 24.f || !enemyFlightClear(play, &from, &landing))
        return 0;

    igos->specialBackjump = special;
    Igos_Action(duel, IGOS_BACKJUMP, special ? 20 : 10, 0x2174, ANIMMODE_ONCE);
    f32 launch = special ? 14.f : -15.f;
    igos->animDrift = (Vec3f){ launch * sin, 0.f, launch * cos };
    actor->speed = 0.f;
    actor->velocity.y = special ? 13.f : 7.5f;
    Igos_FootDust(igos, play, 5);
    Duel_Sfx(actor, special ? 0x3813 : 0x386c);
    return 1;
}

static void Igos_StartJumpAttack(StandaloneIgos* igos, PlayState* play)
{
    StandaloneDuel* duel = &igos->duel;
    Actor* actor = &duel->actor;
    Igos_Action(duel, IGOS_LEAP, 15, 0xafac, ANIMMODE_ONCE);
    s16 yaw = actor->yawTowardsPlayer;
    igos->animDrift = (Vec3f){ 7.f * Math_SinS(yaw), 0.f, 7.f * Math_CosS(yaw) };
    igos->animDriftDelay = 0;
    actor->speed = 0.f;
    actor->velocity.y = 10.f;
    Igos_FootDust(igos, play, 5);
    Duel_Sfx(actor, 0x386c);
}

static void Igos_SetupBlock(StandaloneDuel* duel, PlayState* play, int king)
{
    if (king && duel->actor.xzDistToPlayer <= 120.f && Rand_ZeroOne() < .1f && Igos_TryBackjump(duel, play, 1))
        return;
    if (duel->walking == IGOS_BLOCK)
    {
        duel->timer = 5;
    }
    else
    {
        ((StandaloneIgos*)duel)->blockFromRetreat = duel->walking == IGOS_RETREAT;
        Igos_Action(duel, IGOS_BLOCK, 5, 0x31f0, ANIMMODE_ONCE);
    }
}

static void Igos_StartStrafe(StandaloneIgos* igos, s16 angle)
{
    StandaloneDuel* duel = &igos->duel;
    int side = Rand_ZeroOne() < .5f ? 1 : -1;
    igos->strafeTarget = duel->actor.shape.rot.y + side * angle;
    igos->strafeNeckTarget = side * 0x3800;
    igos->strafeMaxStep = angle == 0x4000 ? 0 : 0x800;
    Igos_Action(duel, IGOS_STRAFE, 20, 0xef44, ANIMMODE_ONCE);
}

enum { IGOS_RETREAT_RUN, IGOS_RETREAT_IDLE, IGOS_RETREAT_BLOCK, IGOS_RETREAT_STRUCK };

static int Igos_BeamActive(PlayState* play)
{
#if defined(GAME_MM)
    const s16 id = ACTOR_MIR_RAY3;
    const size_t intensity = 0x214;
#else
    const s16 id = ACTOR_MIR_RAY;
    const size_t intensity = 0x22c;
#endif
    for (Actor* ray = play->actorCtx.actors[ACTORCAT_ITEMACTION].first; ray; ray = ray->next)
    {
        if (ray->id == id && ray->update && *(const f32*)((const u8*)ray + intensity) > .1f)
            return 1;
    }
    return 0;
}

static void Igos_StartRetreat(StandaloneIgos* igos, Vec3f* threat, int shielding)
{
    StandaloneDuel* duel = &igos->duel;
    Actor* actor = &duel->actor;
    f32 dx = threat->x - actor->world.pos.x;
    f32 dz = threat->z - actor->world.pos.z;
    igos->retreatTarget = actor->world.pos;
    igos->retreatTarget.x -= dx * 100.f;
    igos->retreatTarget.z -= dz * 100.f;
    if (dx * dx + dz * dz < .01f)
    {
        igos->retreatTarget.x -= Math_SinS(actor->shape.rot.y) * 100.f;
        igos->retreatTarget.z -= Math_CosS(actor->shape.rot.y) * 100.f;
    }
    duel->phase = DUEL_APPROACH;
    igos->retreatShield = shielding;
    if (duel->walking == IGOS_RETREAT && igos->retreatMode != IGOS_RETREAT_RUN && shielding)
        return;
    u8 timer = 10 + (u8)(Rand_ZeroOne() * 15.f);
    if (duel->walking == IGOS_RETREAT)
    {
        duel->timer = timer;
    }
    else
    {
        igos->retreatMode = IGOS_RETREAT_RUN;
        Igos_Action(duel, IGOS_RETREAT, timer, 0x3650, ANIMMODE_LOOP);
    }
}

static void Igos_CheckExplosiveRetreat(StandaloneIgos* igos, PlayState* play)
{
    StandaloneDuel* duel = &igos->duel;
    Actor* actor = &duel->actor;
    if (igos->headTarget)
        return;
    for (Actor* explosive = play->actorCtx.actors[ACTORCAT_EXPLOSIVE].first; explosive; explosive = explosive->next)
    {
        if (!explosive->update)
            continue;
        f32 dx = explosive->world.pos.x - actor->world.pos.x;
        f32 dz = explosive->world.pos.z - actor->world.pos.z;
        if (dx * dx + dz * dz >= 100.f * 100.f)
            continue;
        Igos_StartRetreat(igos, &explosive->world.pos, 0);
    }
}

#if defined(GAME_OOT)
static Actor* Duel_FindOotProjectile(PlayState* play, Actor* target, f32 radius);
#endif

static void Igos_DefendThreat(StandaloneIgos* igos, PlayState* play, Player* player, int king)
{
    StandaloneDuel* duel = &igos->duel;
    Actor* actor = &duel->actor;
    s16 facing = actor->yawTowardsPlayer - actor->shape.rot.y;
    s16 playerFacing = actor->yawTowardsPlayer - player->actor.shape.rot.y + 0x8000;
    if (igos->headTarget || ABS(facing) >= 0x3000 || ABS(playerFacing) >= 0x3000)
        return;
#if defined(GAME_MM)
    int melee = player->unk_ADC != 0;
    int projectile = player->unk_D57 != 0;
#else
    int melee = player->meleeWeaponState != 0;
    int projectile = (!melee || actor->xzDistToPlayer > 120.f) && Duel_FindOotProjectile(play, actor, 100.f);
#endif
    if (!projectile && (!melee || actor->xzDistToPlayer > 120.f))
        return;
    duel->phase = DUEL_APPROACH;
    int jump = Rand_ZeroOne() < (king ? .65f : .25f) && melee && duel->walking != IGOS_BLOCK;
    if (!jump || !Igos_TryBackjump(duel, play, 0))
        Igos_SetupBlock(duel, play, king);
}

static void Igos_Combat(StandaloneDuel* duel, PlayState* play, int finished, f32 prevFrame)
{
    Actor* actor = &duel->actor;
    Player* player = GET_PLAYER(play);
    f32 distance = actor->xzDistToPlayer;
    f32 frame = duel->skel.curFrame;
    int king = (u16)actor->params == 0x807f;
    StandaloneIgos* igos = (StandaloneIgos*)duel;
    igos->sword.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
    igos->sword.base.acFlags &= ~(AC_HIT | AC_BOUNCED);
    igos->shield.base.acFlags &= ~(AC_HIT | AC_BOUNCED);
    if (duel->specialCooldown)
        --duel->specialCooldown;
    Math_ApproachS(&actor->shape.rot.x, 0, 10, 0x200);
    int breathing =
        king && duel->phase != DUEL_HURT && duel->walking >= IGOS_BREATH_START && duel->walking <= IGOS_BREATH_STOP;
    if (king && !breathing)
        duel->breathAlpha = 0;
    if (duel->phase == DUEL_HURT ||
        (!breathing && duel->walking != IGOS_WAIT && duel->walking != IGOS_TURN && duel->walking != IGOS_STUN &&
         duel->walking != IGOS_RETREAT && duel->walking != IGOS_STRAFE && duel->walking != IGOS_ADVANCE &&
         duel->walking != IGOS_HEAVY && duel->walking != IGOS_LOW))
    {
        Math_ApproachS(&igos->neckYaw, 0, 3, 0x1000);
    }
    if (breathing)
    {
        Math_ApproachF(&actor->speed, 0.f, 1.f, 1.f);
        Math_ApproachS(&actor->world.rot.y, actor->yawTowardsPlayer, 5, 0x500);
    }
    else if (duel->phase != DUEL_HURT && duel->walking == IGOS_REATTACH)
    {
        actor->speed = 0.f;
        Math_SmoothStepToS(&actor->shape.rot.y, actor->yawTowardsPlayer, 3, 0x800, 0x40);
        actor->world.rot.y = actor->shape.rot.y;
    }
    duel->body.base.acFlags &= ~AC_HARD;

    int wasHurt = duel->phase == DUEL_HURT;
    int retreatAllowed = wasHurt ? king && duel->timer < 4
                                 : duel->walking == IGOS_WAIT || duel->walking == IGOS_TURN ||
                                       duel->walking == IGOS_ADVANCE || duel->walking == IGOS_BLOCK || breathing ||
                                       duel->walking == IGOS_RETREAT;
#if defined(GAME_MM)
    if (!king && duel->phase == DUEL_APPROACH)
    {
        int marching = !!(player->stateFlags3 & 0x20000000);
        if (retreatAllowed && marching)
            Igos_Action(duel, IGOS_MARCH_WAIT, 65 + (u8)(Rand_ZeroOne() * 10.f), 0x40e0, ANIMMODE_LOOP);
        if (duel->walking >= IGOS_MARCH_WAIT && duel->walking <= IGOS_MARCH_END)
        {
            igos->neckYaw = 0;
            if (duel->walking == IGOS_MARCH_WAIT)
            {
                Math_ApproachS(&actor->world.rot.y, actor->yawTowardsPlayer, 2, 0x500);
                Math_StepToF(&actor->speed, 0.f, 1.f);
                if (!duel->timer && ((s32 (*)(void))0x801a46f8)())
                    Igos_Action(duel, IGOS_MARCH, 200, 0xddcc, ANIMMODE_LOOP);
            }
            else
            {
                if (duel->walking == IGOS_MARCH)
                {
                    actor->speed = 1.f;
                    actor->world.rot.y += (u16)actor->params == 0x817f ? 0x100 : -0x100;
                }
                if (Animation_OnFrame(&duel->skel, 1.f) || Animation_OnFrame(&duel->skel, 11.f))
                    Duel_Sfx(actor, (u16)actor->params == 0x817f ? 0x3a28 : 0x3a27);
                if (duel->walking == IGOS_MARCH_END && !duel->timer)
                    Igos_Action(duel, IGOS_WAIT, 20, 0x40e0, ANIMMODE_LOOP);
            }
            if (duel->walking != IGOS_MARCH_END && (!marching || (duel->walking == IGOS_MARCH && !duel->timer)))
            {
                duel->walking = IGOS_MARCH_END;
                duel->timer = 10;
            }
            Math_ApproachS(&actor->shape.rot.y, actor->world.rot.y, 5, 0x500);
            return;
        }
    }
#endif
    int beamActive = retreatAllowed && !igos->headTarget && Igos_BeamActive(play);
    int beamThreat = beamActive && distance <= 300.f;
    if (duel->phase == DUEL_HURT)
    {
        Math_StepToF(&actor->speed, 0.f, 1.f);
        f32 choice = Rand_ZeroOne();
        if (!duel->timer)
        {
            duel->phase = DUEL_APPROACH;
            if (king && distance < 150.f)
            {
                if (choice < .25f)
                    Igos_Action(duel, IGOS_LOW, 0, 0x22728, ANIMMODE_ONCE);
                else if (choice < .5f)
                    Igos_Action(duel, IGOS_WINDUP, 6, 0x20950, ANIMMODE_LOOP);
                else if (choice >= .75f || !Igos_TryBackjump(duel, play, 1))
                    Igos_SetupBlock(duel, play, king);
            }
            else if (duel->skel.animation == Duel_Anim(0x0d9c))
            {
                Igos_StartStrafe(igos, 0x6000);
            }
            else
                Igos_Action(duel, IGOS_WAIT, 20 + (u8)(Rand_ZeroOne() * 50.f), 0x40e0, ANIMMODE_LOOP);
        }
        if (retreatAllowed)
        {
            Igos_CheckExplosiveRetreat(igos, play);
            if (beamThreat)
                Igos_StartRetreat(igos, &player->actor.world.pos, 1);
            Igos_DefendThreat(igos, play, player, king);
        }
        return;
    }

    switch (duel->walking)
    {
        case IGOS_WAIT: {
            s16 facing = actor->yawTowardsPlayer - actor->shape.rot.y;
            Math_StepToF(&actor->speed, 0.f, 1.f);
            if (ABS(facing) < (king ? 0x7fff : 0x6000))
            {
                duel->walking = IGOS_TURN;
                duel->timer = 10;
                Math_ApproachS(&igos->neckYaw, 0, 3, 0x1000);
            }
            else
            {
                if (!((play->gameplayFrames + igos->randTimerOffset) & 7) && Rand_ZeroOne() < .75f)
                    igos->waitNeckTarget = (s16)((Rand_ZeroOne() - .5f) * 28672.f);
                if (!duel->timer)
                    Igos_StartStrafe(igos, 0x4000);
                s16 neckTarget = duel->walking == IGOS_STRAFE ? igos->strafeNeckTarget : igos->waitNeckTarget;
                Math_ApproachS(&igos->neckYaw, neckTarget, 3, 0x1000);
            }
            break;
        }
        case IGOS_TURN: {
            s16 facing = actor->yawTowardsPlayer - actor->shape.rot.y;
            Math_StepToF(&actor->speed, 0.f, 1.f);
            Math_ApproachS(&igos->neckYaw, CLAMP(facing, -0x3800, 0x3800), 2, 0x3000);
            if (!duel->timer)
                Igos_Action(duel, IGOS_ADVANCE, 0, 0x4620, ANIMMODE_LOOP);
            break;
        }
        case IGOS_STUN:
            Math_StepToF(&actor->speed, 0.f, 1.f);
            if (duel->timer < 20)
                igos->neckYaw = duel->timer & 1 ? 0x708 : -0x708;
            else
                Math_ApproachS(&igos->neckYaw, 0, 3, 0x1000);
            if (!duel->timer)
                Igos_Action(duel, IGOS_WAIT, 20 + (u8)(Rand_ZeroOne() * 50.f), 0x40e0, ANIMMODE_LOOP);
            break;
        case IGOS_RETREAT: {
            Math_ApproachS(&igos->neckYaw, 0, 3, 0x1000);
            if (igos->retreatMode != IGOS_RETREAT_RUN)
            {
                if (igos->retreatMode == IGOS_RETREAT_IDLE && igos->lightRayTimer)
                {
                    igos->retreatMode = IGOS_RETREAT_BLOCK;
                    AnimationHeader* anim = Duel_Anim(0x31f0);
                    Animation_Change(&duel->skel, anim, 1.f, 0.f, Animation_GetLastFrame(anim), ANIMMODE_ONCE, -2.f);
                    static const u16 pauseVoices[] = { 0x3a47, 0x3a43, 0x3a40 };
                    Duel_Sfx(actor, pauseVoices[Igos_VoiceVariant(actor)]);
                }
                else if (igos->retreatMode == IGOS_RETREAT_STRUCK && finished)
                {
                    igos->retreatMode = IGOS_RETREAT_BLOCK;
                    AnimationHeader* anim = Duel_Anim(0x31f0);
                    Animation_Change(&duel->skel, anim, 1.f, 0.f, Animation_GetLastFrame(anim), ANIMMODE_ONCE, -2.f);
                }
                else if (igos->retreatMode == IGOS_RETREAT_BLOCK && !igos->lightRayTimer)
                {
                    igos->retreatMode = IGOS_RETREAT_IDLE;
                    AnimationHeader* anim = Duel_Anim(0x40e0);
                    Animation_Change(&duel->skel, anim, 1.f, 0.f, Animation_GetLastFrame(anim), ANIMMODE_LOOP, -5.f);
                }
                if (igos->retreatMode == IGOS_RETREAT_BLOCK && igos->lightRayTimer > 7)
                    Math_StepToF(&igos->shieldFlashScale, 1.f, .5f);
                Math_StepToF(&actor->speed, 0.f, 1.f);
                if (igos->retreatMode == IGOS_RETREAT_IDLE || Animation_GetLastFrame(duel->skel.animation) > 10.f)
                    Math_ApproachS(&actor->world.rot.y, actor->yawTowardsPlayer, 2, 0xe00);
                if (beamActive)
                    duel->timer = 10 + (u8)(Rand_ZeroOne() * 15.f);
                if (!duel->timer)
                    Igos_StartStrafe(igos, 0x7000);
                break;
            }
            if (igos->retreatShield && (!beamThreat || igos->lightRayTimer))
            {
                if (igos->lightRayTimer)
                {
                    igos->retreatMode = IGOS_RETREAT_STRUCK;
                    AnimationHeader* anim = Duel_Anim(0xe45c);
                    Animation_Change(&duel->skel, anim, 1.f, 0.f, Animation_GetLastFrame(anim), ANIMMODE_ONCE, -2.f);
                    Igos_StartElemental(igos, IGOS_DAMAGE_LIGHT);
                    igos->elementalAlpha = .5f;
                    igos->elementalTimer = 10;
                    Duel_Sfx(actor, 0x3a4b);
                }
                else
                {
                    igos->retreatMode = IGOS_RETREAT_IDLE;
                    AnimationHeader* anim = Duel_Anim(0x40e0);
                    Animation_Change(&duel->skel, anim, 1.f, 0.f, Animation_GetLastFrame(anim), ANIMMODE_LOOP, -5.f);
                }
                break;
            }
            s16 targetYaw = Math_Vec3f_Yaw(&actor->world.pos, &igos->retreatTarget);
            Math_ApproachS(&actor->world.rot.y, targetYaw, 5, 0xa00);
            Math_StepToF(&actor->speed, 5.f, 1.f);

            Vec3f from = actor->world.pos;
            from.y += 24.f;
            Vec3f next = from;
            f32 probe = actor->speed + 8.f;
            next.x += probe * Math_SinS(actor->world.rot.y);
            next.z += probe * Math_CosS(actor->world.rot.y);
            CollisionPoly* floor = NULL;
            s32 bgId;
            f32 height = BgCheck_EntityRaycastDown4(&play->colCtx, &floor, &bgId, actor, &next);
            if (!floor || fabsf(height - actor->world.pos.y) > 25.f || !enemyFlightClear(play, &from, &next))
                actor->speed = 0.f;
            Igos_FastWalkEffects(igos, play);
            if (!duel->timer)
                Igos_StartStrafe(igos, 0x7000);
            break;
        }
        case IGOS_STRAFE: {
            s16 facing = actor->yawTowardsPlayer - actor->shape.rot.y;
            Math_StepToF(&actor->speed, 0.f, 1.f);
            Math_ApproachS(&igos->neckYaw, igos->strafeNeckTarget, 2, 0x3000);
            if (duel->timer < 15)
            {
                Math_ApproachS(&actor->world.rot.y, igos->strafeTarget, 2, igos->strafeMaxStep);
                Math_ApproachS(&igos->strafeMaxStep, 0x800, 1, 0x200);
            }
            if (ABS(facing) < 0x6000)
            {
                duel->walking = IGOS_TURN;
                duel->timer = 10;
            }
            else if (!duel->timer)
                Igos_Action(duel, IGOS_WAIT, 20 + (u8)(Rand_ZeroOne() * 50.f), 0x40e0, ANIMMODE_LOOP);
            break;
        }
        case IGOS_ADVANCE: {
            if (distance > 380.f || fabsf(actor->yDistanceFromLink) > 85.f)
            {
                actor->speed = 0.f;
                break;
            }

            if (!((play->gameplayFrames + igos->randTimerOffset) & 63) && Rand_ZeroOne() < .5f)
            {
                static const u16 pauseVoices[] = { 0x3a47, 0x3a43, 0x3a40 };
                static const u16 laughVoices[] = { 0x3a47, 0x3a44, 0x3a42 };
                const u16* voices = Rand_ZeroOne() < .6666f ? pauseVoices : laughVoices;
                Duel_Sfx(actor, voices[Igos_VoiceVariant(actor)]);
            }

            f32 scale = king ? 1.3076924f : 1.f;
            f32 oldSpeed = actor->speed;
            s16 yaw = actor->yawTowardsPlayer;
            if (!king && distance > 150.f)
            {
                s16 playerYaw = player->actor.world.rot.y;
                f32 side = (u16)actor->params == 0x817f ? 100.f : -100.f;
                Vec3f flank = player->actor.world.pos;
                flank.x += side * Math_CosS(playerYaw) + 100.f * Math_SinS(playerYaw);
                flank.z += 100.f * Math_CosS(playerYaw) - side * Math_SinS(playerYaw);
                yaw = Math_Vec3f_Yaw(&actor->world.pos, &flank);
            }
            Math_ApproachS(&actor->world.rot.y, yaw, 5, 0xa00);
            Math_ApproachS(&actor->shape.rot.y, actor->yawTowardsPlayer, 5, 0x500);
            f32 desiredSpeed = distance > 150.f * scale  ? 5.f
                               : distance > 80.f * scale ? 2.5f
                               : distance > 40.f * scale ? 0.f
                                                         : -2.5f;
            Math_StepToF(&actor->speed, desiredSpeed, 1.f);
            if (actor->bgCheckFlags & BGCHECKFLAG_GROUND)
            {
                Vec3f probe = actor->world.pos;
                f32 step = actor->speed < 0.f ? actor->speed - 8.f : actor->speed + 8.f;
                probe.x += step * Math_SinS(actor->world.rot.y);
                probe.z += step * Math_CosS(actor->world.rot.y);
                probe.y += 20.f;
                CollisionPoly* floor = NULL;
                s32 bgId;
                f32 floorHeight = BgCheck_EntityRaycastDown4(&play->colCtx, &floor, &bgId, actor, &probe);
                if (!floor || floorHeight < actor->world.pos.y - 25.f)
                    actor->speed = 0.f;
            }
            if (distance > 150.f * scale)
                Igos_FastWalkEffects(igos, play);
            int spedUp = actor->speed > 3.f && oldSpeed <= 3.f;
            int slowed = actor->speed < 4.f && oldSpeed >= 4.f;
            int jump = (spedUp || slowed) && Rand_ZeroOne() < .25f && distance <= 200.f &&
                       (actor->bgCheckFlags & BGCHECKFLAG_GROUND) && !(actor->bgCheckFlags & BGCHECKFLAG_WALL);
            if (jump)
            {
                Igos_StartJumpAttack(igos, play);
            }
            else
            {
                u32 walk = 0;
                f32 morph = -5.f;
                if (spedUp)
                {
                    walk = 0x3650;
                    morph = -3.f;
                }
                if (slowed)
                {
                    walk = 0x4620;
                    morph = -5.f;
                }
                if (actor->speed < 1.f && oldSpeed >= 1.f)
                {
                    walk = 0x40e0;
                    morph = -10.f;
                }
                else if (actor->speed >= 1.f && oldSpeed < 1.f)
                {
                    walk = 0x4620;
                    morph = -5.f;
                }
                if (walk)
                {
                    AnimationHeader* anim = Duel_Anim(walk);
                    Animation_Change(&duel->skel, anim, 1.f, 0.f, Animation_GetLastFrame(anim), ANIMMODE_LOOP, morph);
                }
            }

            s16 facing = yaw - actor->shape.rot.y;
            if (ABS(facing) >= 0x6000)
            {
                Igos_Action(duel, IGOS_WAIT, 20 + (u8)(Rand_ZeroOne() * 50.f), 0x40e0, ANIMMODE_LOOP);
                break;
            }
            Math_ApproachS(&igos->neckYaw, CLAMP(facing, -0x3800, 0x3800), 3, 0x1000);
            if (distance <= 90.f * scale && !((play->gameplayFrames + igos->randTimerOffset) & 7))
            {
                if (Rand_ZeroOne() < .5f && (!igos->headTarget || player->actor.parent != igos->headTarget))
                    Igos_Action(duel, IGOS_SWING, 20, Rand_ZeroOne() < .5f ? 0xbcf4 : 0xc384, ANIMMODE_ONCE);
                else if (Rand_ZeroOne() < .5f)
                    Igos_Action(duel, IGOS_WINDUP, king ? 6 : 15, 0x20950, ANIMMODE_LOOP);
                else
                    Igos_Action(duel, IGOS_LOW, 0, 0x22728, ANIMMODE_ONCE);
            }
            if (king && !igos->headTarget && !duel->specialCooldown && distance > 100.f)
            {
                if (distance < 300.f && Rand_ZeroOne() > .5f)
                    Igos_Action(duel, IGOS_BREATH_START, 150, 0x9538, ANIMMODE_ONCE);
                else
                    Igos_Action(duel, IGOS_DETACH, 50, 0x3008, ANIMMODE_ONCE);
            }
            break;
        }
        case IGOS_BACKJUMP:
            if (igos->specialBackjump)
                Duel_Sfx(actor, 0x3814 - SFX_FLAG);
            if (king)
            {
                if (duel->timer == 13)
                    Duel_Sfx(actor, igos->specialBackjump ? 0x3a33 : 0x3a31);
                Math_ApproachS(&actor->world.rot.y, actor->yawTowardsPlayer, 2, 0x1000);
            }
            actor->speed = 0.f;
            if (actor->velocity.y <= 0.f && (actor->bgCheckFlags & BGCHECKFLAG_GROUND))
            {
                igos->animDrift = (Vec3f){ 0 };
                Duel_Sfx(actor, 0x387b);
                if (igos->specialBackjump || (king && Rand_ZeroOne() < .3f))
                {
                    int jump = Rand_ZeroOne() < .5f || (!igos->specialBackjump && igos->headTarget);
                    if (jump && distance <= 200.f)
                    {
                        Igos_StartJumpAttack(igos, play);
                        break;
                    }
                    if (!jump && !igos->headTarget)
                    {
                        Igos_Action(duel, IGOS_BREATH_START, 150, 0x9538, ANIMMODE_ONCE);
                        break;
                    }
                    if (!jump)
                    {
                        Igos_Action(duel, IGOS_WINDUP, 6, 0x20950, ANIMMODE_LOOP);
                        break;
                    }
                }
                Igos_Action(duel, IGOS_WAIT, 20 + (u8)(Rand_ZeroOne() * 50.f), 0x40e0, ANIMMODE_LOOP);
            }
            break;
        case IGOS_BLOCK:
            Math_ApproachS(&actor->world.rot.y, actor->yawTowardsPlayer, 2, 0x800);
            Math_StepToF(&actor->speed, 0.f, 1.f);
            if (!duel->timer)
            {
                if (igos->blockFromRetreat)
                {
                    igos->retreatShield = 1;
                    igos->retreatMode = IGOS_RETREAT_RUN;
                    Igos_Action(duel, IGOS_RETREAT, 10 + (u8)(Rand_ZeroOne() * 15.f), 0x3650, ANIMMODE_LOOP);
                }
                else
                {
                    Igos_Action(duel, IGOS_WAIT, 20 + (u8)(Rand_ZeroOne() * 50.f), 0x40e0, ANIMMODE_LOOP);
                    if (king && Rand_ZeroOne() < .4f)
                        Duel_Sfx(actor, 0x3a31);
                }
            }
            else if (igos->hasShieldPose && play->gameplayFrames - igos->shieldPoseFrame <= 1)
                Duel_AC(play, &play->colChkCtx, &igos->shield.base);
            break;
        case IGOS_SWING:
            Math_ApproachS(&actor->world.rot.y, actor->yawTowardsPlayer, 5, 0x500);
            Math_StepToF(&actor->speed, 0.f, 1.f);
            if (duel->timer == 16)
                Duel_Weapon(duel, play);
            if (finished)
            {
                if (Rand_ZeroOne() < .6f && distance <= 100.f)
                {
                    if (Rand_ZeroOne() < .5f)
                        Igos_Action(duel, IGOS_SWING, 20, Rand_ZeroOne() < .5f ? 0xbcf4 : 0xc384, ANIMMODE_ONCE);
                    else
                        Igos_Action(duel, IGOS_WINDUP, king ? 6 : 15, 0x20950, ANIMMODE_LOOP);
                }
                else
                    Igos_Action(duel, IGOS_WAIT, 20 + (u8)(Rand_ZeroOne() * 50.f), 0x40e0, ANIMMODE_LOOP);
            }
            break;
        case IGOS_WINDUP:
            Math_ApproachS(&actor->world.rot.y, actor->yawTowardsPlayer, 2, 0x500);
            Math_StepToF(&actor->speed, 0.f, 1.f);
            if (!duel->timer)
                Igos_Action(duel, IGOS_HEAVY, 0, 0x11298, ANIMMODE_ONCE);
            break;
        case IGOS_HEAVY:
            Math_ApproachS(&igos->neckYaw, 0, 3, 0x1000);
            Math_StepToF(&actor->speed, 0.f, 1.f);
            if (prevFrame >= 3.f && prevFrame <= 6.f)
            {
                igos->swordBlurAlpha = 255;
                igos->swordBlurLow = 0;
            }
            if (Animation_OnFrame(&duel->skel, 1.f))
                Igos_AttackVoice(actor);
            if (frame >= 2.f && frame <= 5.f)
                igos->animDrift =
                    (Vec3f){ 5.f * Math_SinS(actor->world.rot.y), 0.f, 5.f * Math_CosS(actor->world.rot.y) };
            Duel_Weapon(duel, play);
            if (finished)
            {
                if (Rand_ZeroOne() < .5f && distance <= 100.f)
                    Igos_Action(duel, IGOS_LOW, 0, 0x22728, ANIMMODE_ONCE);
                else
                    Igos_Action(duel, IGOS_WAIT, 20 + (u8)(Rand_ZeroOne() * 50.f), 0x40e0, ANIMMODE_LOOP);
            }
            break;
        case IGOS_LOW:
            Math_ApproachS(&igos->neckYaw, 0, 3, 0x1000);
            if (king)
            {
                Math_ApproachS(&actor->world.rot.y, actor->yawTowardsPlayer, 2, 0x800);
            }
            Math_StepToF(&actor->speed, 0.f, 1.f);
            if (prevFrame >= 12.f && prevFrame <= 15.f)
            {
                igos->swordBlurAlpha = 255;
                igos->swordBlurLow = 1;
            }
            if (Animation_OnFrame(&duel->skel, 12.f))
                Igos_AttackVoice(actor);
            if (frame >= 12.f)
            {
                if (frame <= 15.f)
                    igos->animDrift =
                        (Vec3f){ 5.f * Math_SinS(actor->world.rot.y), 0.f, 5.f * Math_CosS(actor->world.rot.y) };
                Duel_Weapon(duel, play);
            }
            if (finished)
                Igos_Action(duel, IGOS_RECOVER, king ? 6 : 15, 0x22cac, ANIMMODE_LOOP);
            break;
        case IGOS_LEAP:
        case IGOS_LAND:
            if (duel->timer > 1 && duel->timer < 6)
            {
                igos->swordBlurAlpha = 255;
                igos->swordBlurLow = 0;
            }
            actor->speed = 0.f;
            if (actor->velocity.y <= 0.f && (actor->bgCheckFlags & BGCHECKFLAG_GROUND))
            {
                igos->animDrift = (Vec3f){ 0 };
                Duel_Sfx(actor, 0x387b);
            }
            Duel_Weapon(duel, play);
            if (!duel->timer)
                Igos_Action(duel, IGOS_WAIT, 20 + (u8)(Rand_ZeroOne() * 50.f), 0x40e0, ANIMMODE_LOOP);
            else if (duel->walking == IGOS_LEAP && finished)
            {
                Igos_Action(duel, IGOS_LAND, duel->timer, 0xb5d4, ANIMMODE_ONCE);
                Igos_AttackVoice(actor);
            }
            break;
        case IGOS_DETACH:
            Math_StepToF(&actor->speed, 0.f, 1.f);
            if (!igos->headTarget && Animation_OnFrame(&duel->skel, 19.f))
                Igos_SetHeadTarget(igos, play, 1);
            if (!duel->timer)
            {
                Igos_Action(duel, IGOS_WAIT, 20 + (u8)(Rand_ZeroOne() * 50.f), 0x40e0, ANIMMODE_LOOP);
                Duel_Sfx(actor, 0x3a33);
            }
            break;
        case IGOS_REATTACH:
            if (duel->skel.playSpeed == 0.f && igos->headTarget)
            {
                IgosFlyingHead* head = (IgosFlyingHead*)igos->headTarget;
                Vec3f target = Igos_HeadSocket(actor, play);
                if (Math_Vec3f_DistXYZ(&head->actor.world.pos, &target) < 16.f &&
                    enemyFlightClear(play, &head->actor.world.pos, &target))
                {
                    duel->skel.playSpeed = 1.f;
                    duel->timer = 25;
                }
                break;
            }
            if (duel->timer <= 19 && igos->headTarget)
            {
                IgosFlyingHead* head = (IgosFlyingHead*)igos->headTarget;
                Vec3f target = Igos_HeadSocket(actor, play);
                if (Math_Vec3f_DistXYZ(&head->actor.world.pos, &target) < 16.f &&
                    enemyFlightClear(play, &head->actor.world.pos, &target))
                {
                    Igos_SetHeadTarget(igos, play, 0);
                    actor->flags |= ACTOR_FLAG_ATTENTION_ENABLED;
                }
                else
                {
                    head->headAction = IGOS_HEAD_RETURN;
                    Igos_Action(duel, IGOS_WAIT, 20 + (u8)(Rand_ZeroOne() * 50.f), 0x40e0, ANIMMODE_LOOP);
                }
            }
            if (!duel->timer)
            {
                Igos_Action(duel, IGOS_WAIT, 20 + (u8)(Rand_ZeroOne() * 50.f), 0x40e0, ANIMMODE_LOOP);
                duel->specialCooldown = 150 + (u16)(Rand_ZeroOne() * 100.f);
            }
            break;
        case IGOS_BREATH_START:
            Igos_Breath(duel, play);
            if (finished)
                Igos_Action(duel, IGOS_BREATH, duel->timer, 0x8d80, ANIMMODE_LOOP);
            break;
        case IGOS_BREATH:
            Igos_Breath(duel, play);
            if (!duel->timer)
                Igos_Action(duel, IGOS_BREATH_STOP, 0, 0xe15c, ANIMMODE_ONCE);
            break;
        case IGOS_BREATH_STOP:
            Igos_Breath(duel, play);
            if (finished)
            {
                Igos_Action(duel, IGOS_WAIT, 20 + (u8)(Rand_ZeroOne() * 50.f), 0x40e0, ANIMMODE_LOOP);
                duel->specialCooldown = 150 + (u16)(Rand_ZeroOne() * 150.f);
            }
            break;
        case IGOS_RECOVER:
            Math_StepToF(&actor->speed, 0.f, 1.f);
            if (!duel->timer)
                Igos_Action(duel, IGOS_WAIT, 20 + (u8)(Rand_ZeroOne() * 50.f), 0x40e0, ANIMMODE_LOOP);
            break;
    }
    if (!wasHurt && retreatAllowed &&
        (duel->walking == IGOS_WAIT || duel->walking == IGOS_TURN || duel->walking == IGOS_ADVANCE ||
         duel->walking == IGOS_BLOCK || breathing || duel->walking == IGOS_RETREAT))
    {
        Igos_CheckExplosiveRetreat(igos, play);
        if (beamThreat)
            Igos_StartRetreat(igos, &player->actor.world.pos, 1);
    }
    if (retreatAllowed &&
        (duel->walking == IGOS_WAIT || duel->walking == IGOS_TURN || duel->walking == IGOS_ADVANCE ||
         duel->walking == IGOS_BLOCK || (duel->walking >= IGOS_BREATH_START && duel->walking <= IGOS_BREATH_STOP) ||
         (duel->walking == IGOS_RETREAT && igos->retreatMode != IGOS_RETREAT_RUN)))
    {
        Igos_DefendThreat(igos, play, player, king);
    }
}

enum { GOMESS_SWING = GARO_FALL + 1, GOMESS_SWING_END, GOMESS_GUARD, GOMESS_SPIN, GOMESS_SWARM };

static void Gomess_Approach(StandaloneDuel* duel, u8 timer)
{
    duel->phase = DUEL_APPROACH;
    duel->timer = timer;
    duel->actor.speed = 1.5f;
    duel->actor.shape.rot.x = 0;
    StandaloneGomess* gomess = (StandaloneGomess*)duel;
    gomess->scythePoseKind = 0;
    gomess->scythe.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
    gomess->spin.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
    AnimationHeader* anim = Duel_Anim(0x3cac);
    Animation_Change(&duel->skel, anim, 1.f, 0.f, Animation_GetLastFrame(anim), ANIMMODE_LOOP, 10.f);
}

#if defined(GAME_OOT)
void FOREIGN_ENEMY_API_DAMAGE_EFFECTS(PlayState*, Actor*, Vec3f*, s16, f32, f32, f32, u8);
void FOREIGN_ENEMY_API_ICE_SHATTER(PlayState*, Actor*, Vec3f*, s32, s32, f32, f32);
#define Gekko_DrawDamageEffects FOREIGN_ENEMY_API_DAMAGE_EFFECTS
#define Gekko_SpawnIceEffects FOREIGN_ENEMY_API_ICE_SHATTER
#else
#define Gekko_DrawDamageEffects Actor_DrawDamageEffects
#define Gekko_SpawnIceEffects Actor_SpawnIceEffects
#endif

enum {
    GEKKO_DRAW_FIRE = 0,
    GEKKO_DRAW_ICE = 10,
    GEKKO_DRAW_LIGHT = 20,
    GEKKO_DRAW_ELECTRIC = 30,
};

static GekkoDamage* Gekko_GetDamage(StandaloneDuel* duel)
{
    return DUEL_FIXED_FORM == DUEL_SNAPPER ? &((StandaloneSnapper*)duel)->damage : &((StandaloneSlime*)duel)->damage;
}

static void Gekko_Thaw(StandaloneDuel* duel, PlayState* play)
{
    GekkoDamage* damage = Gekko_GetDamage(duel);
    if (damage->type != GEKKO_DRAW_ICE || !damage->timer)
        return;
    damage->timer = 0;
    if (damage->pointMask == (1u << ARRAY_COUNT(damage->points)) - 1)
        Gekko_SpawnIceEffects(play, &duel->actor, damage->points, ARRAY_COUNT(damage->points), 2, .3f, .2f);
    if (DUEL_FIXED_FORM == DUEL_SNAPPER)
    {
        StandaloneSnapper* snapper = (StandaloneSnapper*)duel;
        snapper->body.base.colMaterial = COL_MATERIAL_HIT6;
        snapper->elements[0].base.elemMaterial = ELEM_MATERIAL_UNK1;
    }
}

static void Gekko_ApplyEffects(StandaloneDuel* duel, PlayState* play, u8 effect)
{
    Actor* actor = &duel->actor;
    GekkoDamage* damage = Gekko_GetDamage(duel);
    if (effect != GEKKO_DAMAGE_ICE)
    {
        Gekko_Thaw(duel, play);
        if (damage->stun)
        {
            damage->stun = 0;
            duel->skel.playSpeed = 1.f;
        }
    }
    switch (effect)
    {
        case GEKKO_DAMAGE_FIRE:
        case GEKKO_DAMAGE_LIGHT:
            damage->type = effect == GEKKO_DAMAGE_FIRE ? GEKKO_DRAW_FIRE : GEKKO_DRAW_LIGHT;
            damage->timer = DUEL_FIXED_FORM == DUEL_SNAPPER && !((StandaloneSnapper*)duel)->mountPhase ? 80 : 60;
            damage->scale = .75f;
            if (effect == GEKKO_DAMAGE_LIGHT)
            {
                ColliderElement* hit =
                    DUEL_FIXED_FORM == DUEL_SNAPPER ? &((StandaloneSnapper*)duel)->elements[0].base : &duel->body.elem;
                Vec3s* pos = &hit->acDmgInfo.hitPos;
                Actor_Spawn(&play->actorCtx, play, ACTOR_EN_CLEAR_TAG, pos->x, pos->y, pos->z, 0, 0, 0, 4);
            }
            break;
        case GEKKO_DAMAGE_ICE:
            damage->type = GEKKO_DRAW_ICE;
            damage->stun = DUEL_FIXED_FORM == DUEL_SNAPPER ? 80 : 2;
            damage->timer = damage->stun;
            damage->scale = .75f;
            damage->steam = 1.125f;
            if (DUEL_FIXED_FORM == DUEL_SLIME)
                actor->flags &= ~SLIME_HOOK_PULLS_ACTOR;
            duel->skel.playSpeed = 0.f;
            Actor_SetColorFilter(actor, 0x4000, 255, 0, damage->stun);
            if (DUEL_FIXED_FORM == DUEL_SNAPPER)
            {
                StandaloneSnapper* snapper = (StandaloneSnapper*)duel;
                snapper->body.base.colMaterial = COL_MATERIAL_HIT3;
                snapper->elements[0].base.elemMaterial = ELEM_MATERIAL_UNK0;
            }
            break;
        case GEKKO_DAMAGE_ELECTRIC:
            damage->type = GEKKO_DRAW_ELECTRIC;
            damage->timer = 40;
            damage->scale = .75f;
            __attribute__((fallthrough));
        case GEKKO_DAMAGE_STUN:
        case GEKKO_DAMAGE_DEKU:
            damage->stun = 40;
            duel->skel.playSpeed = 0.f;
            Duel_Sfx(actor, 0x389e);
            Actor_SetColorFilter(actor, 0, 255, 0, 40);
            break;
    }
    if (DUEL_FIXED_FORM == DUEL_SNAPPER && damage->stun && duel->phase == DUEL_APPROACH &&
        duel->skel.animation == Duel_Anim(0x39c4))
    {
        duel->skel.curFrame = 0.f;
        SkelAnime_Update(&duel->skel);
    }
    if (DUEL_FIXED_FORM == DUEL_SLIME && damage->stun &&
        (duel->skel.animation == Duel_Anim(0x39c4) || duel->skel.animation == Duel_Anim(0x10f4)))
    {
        duel->skel.curFrame = 1.f;
    }
}

static void Gekko_StartRecoil(StandaloneDuel* duel)
{
    Actor* actor = &duel->actor;
    GekkoDamage* damage = Gekko_GetDamage(duel);
    damage->recoilSpeed = 10.f;
    actor->velocity.y = 0.f;
    actor->world.rot.y = damage->recoilYaw;
    if (DUEL_FIXED_FORM == DUEL_SNAPPER)
    {
        actor->shape.rot.y = damage->recoilYaw + 0x8000;
    }
    else if (DUEL_FIXED_FORM == DUEL_SLIME)
    {
        actor->gravity = -2.f;
        actor->flags &= ~SLIME_HOOK_ATTACHED;
    }
    duel->phase = DUEL_HURT;
    duel->timer = 20;
    duel->walking = 0;
    duel->weapon.base.atFlags &= ~AT_ON;
    if (DUEL_FIXED_FORM == DUEL_SNAPPER)
        ((StandaloneSnapper*)duel)->body.base.atFlags &= ~AT_ON;
    AnimationHeader* anim = Duel_Anim(DUEL_FIXED_FORM == DUEL_SNAPPER ? 0x5d54 : 0x276c);
    Animation_Change(&duel->skel, anim, 1.f, 0.f, Animation_GetLastFrame(anim), ANIMMODE_ONCE, -3.f);
    Actor_SetColorFilter(actor, 0x4000, 255, 0, 20);
    Duel_Sfx(actor, 0x38d3);
}

static void Snapper_CallMount(StandaloneSnapper* snapper);

static int Gekko_Recoil(StandaloneDuel* duel, PlayState* play)
{
    Actor* actor = &duel->actor;
    GekkoDamage* damage = Gekko_GetDamage(duel);
    if (damage->stun)
    {
        actor->speed = 0.f;
        return 1;
    }
    if (duel->phase != DUEL_HURT)
        return 0;
    if (!duel->timer)
    {
        duel->phase = DUEL_APPROACH;
        if (DUEL_FIXED_FORM == DUEL_SNAPPER)
        {
            StandaloneSnapper* snapper = (StandaloneSnapper*)duel;
            if (snapper->callMountAfterRecoil)
            {
                snapper->callMountAfterRecoil = 0;
                Snapper_CallMount(snapper);
                return 1;
            }
            duel->walking = 1;
            actor->world.rot.y = actor->shape.rot.y;
            actor->speed = 0.f;
            Animation_PlayLoop(&duel->skel, Duel_Anim(0x39c4));
            return 1;
        }
        duel->walking = 0;
        if (DUEL_FIXED_FORM == DUEL_SLIME)
            ((StandaloneSlime*)duel)->cycle = 0;
        return 0;
    }
    Math_StepToF(&damage->recoilSpeed, 0.f, .5f);
    actor->world.rot.y = damage->recoilYaw;
    actor->speed = damage->recoilSpeed;

    if (DUEL_FIXED_FORM == DUEL_SLIME)
        actor->shape.rot.y += 0x300 * MIN(duel->timer, 10);
    return 1;
}

enum {
    GEKKO_DISMOUNT = 110,
    GEKKO_WALL_JUMP,
    GEKKO_WALL_CLIMB,
    GEKKO_WALL_PAUSE,
    GEKKO_WALL_FALL,
    GEKKO_CALL_MOUNT,
    GEKKO_RUN_TO_MOUNT,
    GEKKO_MOUNT_JUMP,
    GEKKO_MOUNT_LAND,
    GEKKO_MOUNT_AIM,
    GEKKO_MOUNT_CHARGE
};

static void Gekko_TickEffects(StandaloneDuel* duel, PlayState* play)
{
    GekkoDamage* damage = Gekko_GetDamage(duel);
    int frozen = damage->timer && damage->type == GEKKO_DRAW_ICE;
    if (damage->timer && !frozen)
        --damage->timer;
    if (frozen && !Math_StepToF(&damage->steam, .75f, .01875f))
        Duel_Sfx(&duel->actor, NA_SE_EV_ICE_FREEZE - SFX_FLAG);
    if (damage->stun && !--damage->stun)
    {
        Gekko_Thaw(duel, play);
        duel->skel.playSpeed = DUEL_FIXED_FORM == DUEL_SNAPPER && duel->phase == GEKKO_WALL_PAUSE ? 1.5f : 1.f;
        if (DUEL_FIXED_FORM == DUEL_SNAPPER && !((StandaloneSnapper*)duel)->mountPhase)
        {
            StandaloneSnapper* snapper = (StandaloneSnapper*)duel;
            if (snapper->callMountAfterRecoil)
            {
                snapper->callMountAfterRecoil = 0;
                Snapper_CallMount(snapper);
                return;
            }
            duel->phase = DUEL_APPROACH;
            duel->walking = 1;
            duel->actor.speed = 0.f;
            duel->actor.world.rot.y = duel->actor.shape.rot.y;
            Animation_PlayLoop(&duel->skel, Duel_Anim(0x39c4));
        }
        if (DUEL_FIXED_FORM == DUEL_SLIME && duel->phase < SLIME_FORM)
        {
            StandaloneSlime* slime = (StandaloneSlime*)duel;
            if (frozen)
            {
                Gekko_StartRecoil(duel);
            }
            else
            {
                slime->cycle = 0;
            }
        }
    }
}

static void Gekko_DrawLimb(PlayState* play, s32 limb, Gfx** list, Vec3s* rot, void* arg)
{
    static const s8 bodyPoint[] = {
        -1, -1, 0, -1, 1, -1, 2, -1, 3, -1, 4, -1, 5, 6, -1, 7, 8, 9, -1, 10, -1, 11, -1, -1
    };
    static Vec3f origin = { 0.f, 0.f, 0.f };
    StandaloneDuel* duel = arg;
    Actor* actor = &duel->actor;
    GekkoDamage* damage = Gekko_GetDamage(duel);
    if (limb >= 0 && limb < ARRAY_COUNT(bodyPoint) && bodyPoint[limb] >= 0)
    {
        int index = bodyPoint[limb];
        Matrix_MultVec3f(&origin, &damage->points[index]);
        damage->pointMask |= 1u << index;
    }
    if (limb == 20)
    {
        Matrix_MultVec3f(&origin, &actor->focus.pos);
        damage->poseFrame = play->gameplayFrames;
        if (DUEL_FIXED_FORM == DUEL_SNAPPER)
        {
            StandaloneSnapper* snapper = (StandaloneSnapper*)duel;
            Vec3f offset = { 0.f, 2500.f, 0.f };
            Vec3f center;
            Matrix_MultVec3f(&offset, &center);
            snapper->elements[0].dim.worldSphere.center = (Vec3s){ center.x, center.y, center.z };
            snapper->elements[1].dim.worldSphere.center = (Vec3s){
                actor->focus.pos.x + 35.f * Math_SinS(actor->shape.rot.y),
                actor->focus.pos.y - 10.f,
                actor->focus.pos.z + 35.f * Math_CosS(actor->shape.rot.y),
            };
        }
    }
    else if (DUEL_FIXED_FORM == DUEL_SLIME && limb == 9)
    {
        Vec3f offset = { 1500.f, 2200.f, 0.f };
        Vec3f foot;
        Matrix_MultVec3f(&offset, &foot);
        damage->footY = foot.y;
    }
}

static void Gekko_DrawEffects(StandaloneDuel* duel, PlayState* play)
{
    GekkoDamage* damage = Gekko_GetDamage(duel);
    if (!damage->timer || damage->pointMask != (1u << ARRAY_COUNT(damage->points)) - 1)
        return;
    f32 alpha = damage->type == GEKKO_DRAW_ICE ? 1.f : damage->timer * .05f;
    f32 scale = damage->scale;
    if (damage->type != GEKKO_DRAW_ICE)
        scale = MIN(scale, .375f * (alpha + 1.f));
    Gekko_DrawDamageEffects(play, &duel->actor, damage->points, ARRAY_COUNT(damage->points), scale, damage->steam,
                            alpha, damage->type);
}

static void Snapper_Strike(StandaloneSnapper* snapper, PlayState* play)
{
    if (!snapper->damage.pointMask || play->gameplayFrames - snapper->damage.poseFrame > 1)
        return;
    Vec3s* center = &snapper->elements[1].dim.worldSphere.center;
    Vec3f from = { center->x, center->y, center->z };
    Vec3f target = GET_PLAYER(play)->actor.world.pos;
    target.y += 20.f;
    if (enemyFlightClear(play, &from, &target))
    {
        snapper->body.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
        snapper->body.base.atFlags |= AT_ON;
        CollisionCheck_SetAT(play, &play->colChkCtx, &snapper->body.base);
    }
}

static void Snapper_WaterEffects(Actor* actor, PlayState* play, int jumping)
{
    Vec3f surface = actor->world.pos;
    WaterBox* water;
    if (!WaterBox_GetSurface1_2(play, &play->colCtx, surface.x, surface.z, &surface.y, &water) ||
        surface.y <= actor->world.pos.y)
        return;
    EffectSsGRipple_Spawn(play, &surface, 150, 550, 0);
    if (jumping)
    {
        surface.y += 8.f;
        EffectSsGSplash_Spawn(play, &surface, NULL, NULL, 0, 550);
    }
}

static void Snapper_HopEffects(StandaloneDuel* duel, PlayState* play)
{
    if (Animation_OnFrame(&duel->skel, 1.f))
    {
        Duel_Sfx(&duel->actor, 0x395b);
    }
    else if (Animation_OnFrame(&duel->skel, 11.f))
    {
        Snapper_WaterEffects(&duel->actor, play, 1);
        Duel_Sfx(&duel->actor, 0x2902);
    }
}

static void Gekko_GroundCombat(StandaloneDuel* duel, PlayState* play, int finished)
{
    static const u16 attacks[] = { 0x347c, 0x70c4, 0x3f28, 0xf048 };
    static const u8 impact[] = { 2, 9, 2, 27 };
    static const u16 impactSounds[] = { 0x3961, 0x3961, 0x3966, 0x3961 };
    Actor* actor = &duel->actor;
    actor->speed = 0.f;
    if (DUEL_FIXED_FORM == DUEL_SNAPPER && (duel->phase == DUEL_RECOVER || duel->phase == DUEL_ATTACK) &&
        !(play->gameplayFrames % 14))
        Snapper_WaterEffects(actor, play, 0);
    if (DUEL_FIXED_FORM == DUEL_SNAPPER && duel->phase == DUEL_RECOVER && duel->skel.animation == Duel_Anim(0x66b4))
    {
        Math_SmoothStepToS(&actor->world.rot.y, actor->yawTowardsPlayer, 5, 0x400, 0x80);
        actor->shape.rot.y = actor->world.rot.y;
        if (!finished)
            return;
        duel->phase = DUEL_APPROACH;
        duel->walking = 1;
        Animation_PlayLoop(&duel->skel, Duel_Anim(0x39c4));
        return;
    }

    if (duel->phase == DUEL_APPROACH)
    {
        if (DUEL_FIXED_FORM != DUEL_SNAPPER &&
            (actor->xzDistToPlayer > 380.f || fabsf(actor->yDistanceFromLink) > 85.f))
            return;
        Math_SmoothStepToS(&actor->world.rot.y, actor->yawTowardsPlayer, 5, 0x1000, 0x80);
        actor->shape.rot.y = actor->world.rot.y;
        Collider* body = DUEL_FIXED_FORM == DUEL_SNAPPER ? &((StandaloneSnapper*)duel)->body.base : &duel->body.base;
        int contact = (body->ocFlags1 & OC1_HIT) && (body->ocFlags2 & OC2_HIT_PLAYER);
        s16 facing = actor->yawTowardsPlayer - actor->shape.rot.y;
        Vec3f from = actor->world.pos;
        from.y += 25.f;
        Vec3f target = GET_PLAYER(play)->actor.world.pos;
        target.y += 25.f;
        if (DUEL_FIXED_FORM != DUEL_SNAPPER && actor->xzDistToPlayer < 35.f)
            contact = 1;
        int attackRange = contact && ABS(facing) < 0x3000 && fabsf(actor->yDistanceFromLink) < 38.f &&
                          enemyFlightClear(play, &from, &target);
        if (DUEL_FIXED_FORM == DUEL_SNAPPER && duel->walking)
            Snapper_HopEffects(duel, play);
        if (attackRange && (DUEL_FIXED_FORM == DUEL_SNAPPER || (actor->bgCheckFlags & BGCHECKFLAG_GROUND)) &&
            (!duel->walking || duel->skel.curFrame <= 2.f || duel->skel.curFrame >= 11.f))
        {
            duel->phase = DUEL_ATTACK;
            duel->walking = 0;
            duel->shield = 7;
            Animation_PlayOnce(&duel->skel, Duel_Anim(0xf990));
        }
        else
        {
            if (!duel->walking)
            {
                Animation_PlayLoop(&duel->skel, Duel_Anim(0x39c4));
                duel->walking = 1;
            }
            if (DUEL_FIXED_FORM != DUEL_SNAPPER)
            {
                if (Animation_OnFrame(&duel->skel, 1.f))
                    Duel_Sfx(actor, 0x395b);
                if (Animation_OnFrame(&duel->skel, 11.f) && (actor->bgCheckFlags & BGCHECKFLAG_WATER))
                    Duel_Sfx(actor, 0x2902);
            }
            if (!(actor->bgCheckFlags & BGCHECKFLAG_GROUND) ||
                (duel->skel.curFrame > 1.f && duel->skel.curFrame < 12.f))
            {
                if (DUEL_FIXED_FORM == DUEL_SNAPPER)
                {
                    Vec3f next = from;
                    next.x += 16.f * Math_SinS(actor->world.rot.y);
                    next.z += 16.f * Math_CosS(actor->world.rot.y);
                    CollisionPoly* floor = NULL;
                    s32 bgId;
                    f32 height = BgCheck_EntityRaycastDown4(&play->colCtx, &floor, &bgId, actor, &next);
                    if (!floor || height < actor->floorHeight - 24.f || !enemyFlightClear(play, &from, &next))
                        return;
                }
                actor->speed = 8.f;
            }
        }
        return;
    }

    if (duel->phase == DUEL_ATTACK)
    {
        if (duel->walking && Animation_OnFrame(&duel->skel, impact[duel->walking - 1]))
        {
            duel->weapon.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
            if (DUEL_FIXED_FORM == DUEL_SNAPPER)
                Snapper_Strike((StandaloneSnapper*)duel, play);
            else
                Duel_Weapon(duel, play);
            Duel_Sfx(actor, impactSounds[duel->walking - 1]);
        }
        if (!finished)
            return;
        if (--duel->shield)
        {

            duel->walking = duel->shield == 6 ? 0 : 1 + (u8)(Rand_ZeroOne() * 4.f);
            Animation_PlayOnce(&duel->skel, Duel_Anim(duel->walking ? attacks[duel->walking - 1] : 0xf990));
            return;
        }
        duel->phase = DUEL_RECOVER;
        duel->timer = 45;
        Animation_PlayOnce(&duel->skel, Duel_Anim(0x66b4));
        return;
    }
    else if (duel->timer && !finished)
    {
        return;
    }

    duel->phase = DUEL_APPROACH;
    duel->walking = 0;
    Animation_PlayLoop(&duel->skel, Duel_Anim(sForms[DUEL_FIXED_FORM].idle));
}

static int Snapper_ReceivesHits(const StandaloneSnapper* snapper)
{
    const StandaloneDuel* duel = &snapper->duel;
    if (duel->phase == DUEL_HURT || duel->phase == GEKKO_DISMOUNT || duel->phase == GEKKO_WALL_JUMP ||
        duel->phase >= GEKKO_CALL_MOUNT || snapper->wallHit)
        return 0;
    return duel->phase != GEKKO_WALL_FALL ||
           ((duel->actor.bgCheckFlags & BGCHECKFLAG_GROUND) && duel->skel.animation == Duel_Anim(0x4298));
}

static int Snapper_FindWall(StandaloneDuel* duel, PlayState* play)
{
    Actor* actor = &duel->actor;
    Vec3f from = actor->world.pos;
    from.y += 45.f;
    for (int i = 0; i < 8; ++i)
    {
        s16 yaw = actor->yawTowardsPlayer + 0x8000 + i * 0x2000;
        Vec3f to = { from.x + Math_SinS(yaw) * 180.f, from.y, from.z + Math_CosS(yaw) * 180.f };
        Vec3f hit;
        CollisionPoly* poly;
        s32 bgId;
        if (!BgCheck_EntityLineTest1(&play->colCtx, &from, &to, &hit, &poly, 1, 0, 0, 0, &bgId) ||
            bgId != BGCHECK_SCENE || ABS(poly->normal.y) >= 0x4000)
            continue;
        Vec3f topFrom = from;
        Vec3f topTo = to;
        topFrom.y += 50.f;
        topTo.y += 50.f;
        Vec3f topHit;
        if (!BgCheck_EntityLineTest1(&play->colCtx, &topFrom, &topTo, &topHit, &poly, 1, 0, 0, 0, &bgId) ||
            bgId != BGCHECK_SCENE || ABS(poly->normal.y) >= 0x4000)
            continue;
        if (fabsf(topHit.x - hit.x) > 18.f || fabsf(topHit.z - hit.z) > 18.f)
            continue;
        hit.x -= 23.f * Math_SinS(yaw);
        hit.z -= 23.f * Math_CosS(yaw);
        int clear = 1;
        for (int sample = 1; sample <= 4; ++sample)
        {
            f32 fraction = sample * .25f;
            Vec3f probe = { from.x + (hit.x - from.x) * fraction, from.y + 10.f, from.z + (hit.z - from.z) * fraction };
            f32 height = BgCheck_EntityRaycastDown4(&play->colCtx, &poly, &bgId, actor, &probe);
            if (!poly || fabsf(height - actor->world.pos.y) > 24.f)
            {
                clear = 0;
                break;
            }
        }
        if (!clear)
            continue;
        duel->headPos = hit;
        duel->headYaw = yaw;
        return 1;
    }
    return 0;
}

static EnemySnapperMount* Snapper_GetMount(Actor* actor)
{
    Actor* child = actor->child;
    return child && child->id == ACTOR_ENEMY_SNAPPER_MOUNT && child->update ? (EnemySnapperMount*)child : NULL;
}

static void Snapper_SpawnMount(Actor* actor, PlayState* play)
{
    Vec3f pos = actor->world.pos;
    Vec3f from = pos;
    from.y += 30.f;
    for (int i = 0; i < 8; ++i)
    {
        s16 yaw = actor->yawTowardsPlayer + 0x8000 + i * 0x2000;
        Vec3f point = { from.x + Math_SinS(yaw) * 64.f, from.y, from.z + Math_CosS(yaw) * 64.f };
        Vec3f hit;
        CollisionPoly* poly;
        s32 bgId;
        if (BgCheck_EntityLineTest1(&play->colCtx, &from, &point, &hit, &poly, 1, 0, 1, 0, &bgId))
            continue;
        f32 height = BgCheck_EntityRaycastDown4(&play->colCtx, &poly, &bgId, actor, &point);
        if (!poly || fabsf(height - pos.y) > 16.f)
            continue;
        pos = (Vec3f){ point.x, height, point.z };
        break;
    }
#if defined(GAME_MM)
    Actor_SpawnAsChildAndCutscene(&play->actorCtx, play, ACTOR_ENEMY_SNAPPER_MOUNT, pos.x, pos.y, pos.z, 0,
                                  actor->shape.rot.y, 0, 0, CS_ID_NONE, 0x3ff, (int)actor);
#else
    Actor_SpawnAsChild(&play->actorCtx, actor, play, ACTOR_ENEMY_SNAPPER_MOUNT, pos.x, pos.y, pos.z, 0,
                       actor->shape.rot.y, 0, 0);
#endif
}

static void Snapper_PlayGroundHops(StandaloneDuel* duel)
{
    AnimationHeader* anim = Duel_Anim(0x39c4);
    Animation_Change(&duel->skel, anim, 2.f, 0.f, Animation_GetLastFrame(anim), ANIMMODE_LOOP, -2.f);
}

static void Snapper_GroundHop(StandaloneDuel* duel, PlayState* play)
{
    Actor* actor = &duel->actor;
    Snapper_HopEffects(duel, play);
    if (!(actor->bgCheckFlags & BGCHECKFLAG_GROUND) || (duel->skel.curFrame > 1.f && duel->skel.curFrame < 12.f))
        actor->speed = 12.f;
}

static void Snapper_RunToMount(StandaloneDuel* duel)
{
    Snapper_PlayGroundHops(duel);
    duel->phase = GEKKO_RUN_TO_MOUNT;
    duel->timer = 90;
}

enum { GEKKO_RIDER_IDLE, GEKKO_RIDER_WAVE, GEKKO_RIDER_WAVE_HOLD, GEKKO_RIDER_RISE };

#if defined(GAME_OOT)
static Actor* Duel_FindOotProjectile(PlayState* play, Actor* target, f32 radius)
{
    Actor* projectile = play->actorCtx.actors[ACTORCAT_ITEMACTION].first;
    for (int remaining = 200; projectile && remaining; --remaining)
    {
        u32 address = (u32)projectile;
        if (address < 0x80000000u || address > 0x80800000u - sizeof(Actor) || (address & 3))
            break;
        if (projectile->update &&
            (projectile->id == ACTOR_EN_ARROW || projectile->id == ACTOR_EN_BOOM || projectile->id == ACTOR_ARMS_HOOK))
        {
            f32 distance = Math_Vec3f_DistXYZ(&projectile->world.pos, &target->world.pos);
            s16 facing = Math_Vec3f_Yaw(&projectile->world.pos, &target->world.pos) - projectile->world.rot.y;
            if (projectile->speed > 0.f && distance < radius + projectile->speed * 2.5f && ABS(facing) < 0x1400 &&
                enemyFlightClear(play, &projectile->world.pos, &target->world.pos))
                return projectile;
        }
        projectile = projectile->next;
    }
    return NULL;
}
#endif

static Actor* Snapper_FindProjectile(PlayState* play, Actor* actor)
{
#if defined(GAME_OOT)
    return Duel_FindOotProjectile(play, actor, 60.f);
#else
    Actor* projectile = func_800BC270(play, actor, 60.f, 0x138b0);
    return projectile && enemyFlightClear(play, &projectile->world.pos, &actor->world.pos) ? projectile : NULL;
#endif
}

static void Snapper_MountedPose(StandaloneDuel* duel, EnemySnapperMount* mount, PlayState* play, int finished)
{
    Actor* actor = &duel->actor;
    switch (duel->walking)
    {
        case GEKKO_RIDER_WAVE:
            if (finished)
            {
                duel->walking = GEKKO_RIDER_WAVE_HOLD;
                duel->timer = 14;
            }
            break;
        case GEKKO_RIDER_WAVE_HOLD:
            if (!duel->timer)
            {
                duel->walking = GEKKO_RIDER_IDLE;
                Animation_PlayOnce(&duel->skel, Duel_Anim(0x52ec));
            }
            break;
        case GEKKO_RIDER_RISE:
            mount->riderLift = Math_SinS(duel->timer * 0xccc) * 100.f;
            if (!duel->timer)
            {
                duel->walking = GEKKO_RIDER_IDLE;
                Animation_PlayOnce(&duel->skel, Duel_Anim(0x5694));
            }
            break;
        default: {
            if (finished)
                Animation_PlayOnce(&duel->skel, Duel_Anim(Rand_ZeroOne() < .5f ? 0x4d50 : 0x4680));
            Actor* projectile = Snapper_FindProjectile(play, actor);
            if (!projectile)
                break;
            Vec3f rearingPoint = actor->world.pos;
            rearingPoint.y += 10.f;
            if (projectile->world.rot.x < Duel_Pitch(&projectile->world.pos, &rearingPoint))
            {
                duel->walking = GEKKO_RIDER_WAVE;
                Animation_PlayOnce(&duel->skel, Duel_Anim(0x4894));
            }
            else
            {
                duel->walking = GEKKO_RIDER_RISE;
                duel->timer = 10;
                actor->shape.rot.x = actor->shape.rot.z = 0;
                actor->shape.rot.y = mount->actor.world.rot.y;
                Animation_PlayOnce(&duel->skel, Duel_Anim(0x50b8));
            }
            break;
        }
    }
}

static void Snapper_StartWallFall(StandaloneDuel* duel, PlayState* play, EnemySnapperMount* mount)
{
    Actor* actor = &duel->actor;
    duel->phase = GEKKO_WALL_FALL;
    duel->timer = 0;
    actor->shape.rot.x = actor->shape.rot.z = 0;
    actor->colChkInfo.mass = 50;
    actor->velocity.y = 0.f;
    actor->gravity = -1.f;
    Animation_PlayOnce(&duel->skel, Duel_Anim(0x39c4));
    duel->skel.playSpeed = 0.f;

    if (mount)
        actor->shape.rot.y = actor->world.rot.y = Math_Vec3f_Yaw(&actor->world.pos, &mount->actor.world.pos);
    Vec3f origin = actor->world.pos;
    origin.y += 20.f;
    Vec3f next = origin;
    s16 away = Math_Vec3f_Yaw(&actor->world.pos, &actor->home.pos);
    next.x += 30.f * Math_SinS(away);
    next.z += 30.f * Math_CosS(away);
    CollisionPoly* floor;
    s32 bgId;
    f32 floorHeight = BgCheck_EntityRaycastDown4(&play->colCtx, &floor, &bgId, actor, &next);
    if (floor && floorHeight >= actor->home.pos.y - 25.f && enemyFlightClear(play, &origin, &next))
    {
        actor->world.pos.x = next.x;
        actor->world.pos.z = next.z;
    }
    actor->bgCheckFlags &= ~BGCHECKFLAG_GROUND;
}

static int Snapper_SetClimbSurface(StandaloneSnapper* snapper, CollisionPoly* poly, const Vec3f* contact,
                                   const Vec3f* center, Vec3f forward, int transitioning)
{
    Vec3f normal = { poly->normal.x * (1.f / 32767.f), poly->normal.y * (1.f / 32767.f),
                     poly->normal.z * (1.f / 32767.f) };
    f32 inward =
        (center->x - contact->x) * normal.x + (center->y - contact->y) * normal.y + (center->z - contact->z) * normal.z;
    if (inward < 0.f)
        normal = (Vec3f){ -normal.x, -normal.y, -normal.z };
    if (normal.y > .4f)
        return 0;
    if (transitioning)
    {
        Vec3f old = snapper->climbNormal;
        f32 cosine = old.x * normal.x + old.y * normal.y + old.z * normal.z;
        if (cosine < -.99f)
            return 0;
        Vec3f axis = { old.y * normal.z - old.z * normal.y, old.z * normal.x - old.x * normal.z,
                       old.x * normal.y - old.y * normal.x };
        Vec3f first = { axis.y * forward.z - axis.z * forward.y, axis.z * forward.x - axis.x * forward.z,
                        axis.x * forward.y - axis.y * forward.x };
        Vec3f second = { axis.y * first.z - axis.z * first.y, axis.z * first.x - axis.x * first.z,
                         axis.x * first.y - axis.y * first.x };
        f32 inverse = 1.f / (1.f + cosine);
        forward.x += first.x + second.x * inverse;
        forward.y += first.y + second.y * inverse;
        forward.z += first.z + second.z * inverse;
    }
    f32 dot = forward.x * normal.x + forward.y * normal.y + forward.z * normal.z;
    forward.x -= dot * normal.x;
    forward.y -= dot * normal.y;
    forward.z -= dot * normal.z;
    f32 length = sqrtf(forward.x * forward.x + forward.y * forward.y + forward.z * forward.z);
    if (length < .05f)
        return 0;
    snapper->climbNormal = normal;
    snapper->climbForward = (Vec3f){ forward.x / length, forward.y / length, forward.z / length };
    return 1;
}

static void Snapper_ClimbPose(StandaloneSnapper* snapper);

static int Snapper_BeginClimb(StandaloneSnapper* snapper, PlayState* play)
{
    StandaloneDuel* duel = &snapper->duel;
    Actor* actor = &duel->actor;
    Vec3f from = actor->world.pos;
    f32 height = snapper->damage.pointMask && play->gameplayFrames - snapper->damage.poseFrame <= 1
                     ? CLAMP(actor->focus.pos.y - actor->world.pos.y, 18.f, 55.f)
                     : 18.f;
    from.y += height;
    Vec3f to = from;
    to.x += 65.f * Math_SinS(duel->headYaw);
    to.z += 65.f * Math_CosS(duel->headYaw);
    Vec3f contact;
    CollisionPoly* poly;
    s32 bgId;
    if (!BgCheck_EntityLineTest1(&play->colCtx, &from, &to, &contact, &poly, 1, 0, 0, 0, &bgId) ||
        bgId != BGCHECK_SCENE || !Snapper_SetClimbSurface(snapper, poly, &contact, &from, (Vec3f){ 0.f, 1.f, 0.f }, 0))
        return 0;
    Vec3f normal = snapper->climbNormal;
    Vec3f perch = { contact.x + normal.x * 23.f, contact.y + normal.y * 23.f, contact.z + normal.z * 23.f };
    Vec3f origin = actor->world.pos;
    origin.y += 2.f;
    if (Math_Vec3f_DistXYZ(&from, &perch) > 45.f || !enemyFlightClear(play, &origin, &perch))
        return 0;
    actor->world.pos = perch;
    Snapper_ClimbPose(snapper);
    duel->phase = GEKKO_WALL_CLIMB;
    duel->timer = 35 + (u8)(Rand_ZeroOne() * 15.f);
    duel->walking = 10;
    duel->headPitch = 0;
    actor->gravity = actor->velocity.y = 0.f;
    actor->bgCheckFlags &= ~(BGCHECKFLAG_GROUND | BGCHECKFLAG_GROUND_TOUCH | BGCHECKFLAG_WALL | BGCHECKFLAG_CEILING);
    actor->colChkInfo.displacement = (Vec3f){ 0.f, 0.f, 0.f };
    actor->colChkInfo.mass = MASS_IMMOVABLE;
    Animation_PlayLoop(&duel->skel, Duel_Anim(0x1e14));
    Duel_Sfx(actor, 0x39a9);
    return 1;
}

static int Snapper_CrawlStep(StandaloneSnapper* snapper, PlayState* play)
{
    Actor* actor = &snapper->duel.actor;
    Vec3f from = actor->world.pos;
    Vec3f normal = snapper->climbNormal;
    Vec3f forward = snapper->climbForward;
    Vec3f next = { from.x + 8.f * forward.x, from.y + 8.f * forward.y, from.z + 8.f * forward.z };
    Vec3f ahead = { from.x + 36.f * forward.x, from.y + 36.f * forward.y, from.z + 36.f * forward.z };
    Vec3f contact;
    CollisionPoly* poly;
    s32 bgId;
    if (BgCheck_EntityLineTest1(&play->colCtx, &from, &ahead, &contact, &poly, 1, 1, 1, 0, &bgId))
    {
        if (bgId != BGCHECK_SCENE || !Snapper_SetClimbSurface(snapper, poly, &contact, &from, forward, 1))
            return 0;
    }
    else
    {
        Vec3f support = { next.x - 60.f * normal.x, next.y - 60.f * normal.y, next.z - 60.f * normal.z };
        if (!BgCheck_EntityLineTest1(&play->colCtx, &next, &support, &contact, &poly, 1, 0, 1, 0, &bgId) ||
            bgId != BGCHECK_SCENE || !Snapper_SetClimbSurface(snapper, poly, &contact, &next, forward, 1))
            return 0;
    }
    Vec3f supportNormal = snapper->climbNormal;
    next = (Vec3f){ contact.x + 23.f * supportNormal.x, contact.y + 23.f * supportNormal.y,
                    contact.z + 23.f * supportNormal.z };
    if (Math_Vec3f_DistXYZ(&from, &next) > 45.f || !enemyFlightClear(play, &from, &next))
    {
        snapper->climbNormal = normal;
        snapper->climbForward = forward;
        return 0;
    }
    actor->world.pos = next;
    return 1;
}

static void Snapper_ClimbPose(StandaloneSnapper* snapper)
{
    StandaloneDuel* duel = &snapper->duel;
    Actor* actor = &duel->actor;
    Vec3f normal = snapper->climbNormal;
    Vec3f forward = snapper->climbForward;
    Vec3f side = { normal.y * forward.z - normal.z * forward.y, normal.z * forward.x - normal.x * forward.z,
                   normal.x * forward.y - normal.y * forward.x };
    MtxF orientation = {
        .xx = side.x,
        .yx = side.y,
        .zx = side.z,
        .xy = normal.x,
        .yy = normal.y,
        .zy = normal.z,
        .xz = forward.x,
        .yz = forward.y,
        .zz = forward.z,
        .ww = 1.f,
    };
#if defined(GAME_OOT)
    Matrix_MtxFToYXZRotS(&orientation, &actor->shape.rot, 0);
#else
    ((void (*)(MtxF*, Vec3s*, s32))0x8018219c)(&orientation, &actor->shape.rot, 0);
#endif
    actor->world.rot = actor->shape.rot;
    actor->world.rot.x = -actor->world.rot.x;
    if (fabsf(normal.y) <= .55f)
    {
        Vec3f zero = { 0.f, 0.f, 0.f };
        duel->headYaw = Math_Vec3f_Yaw(&normal, &zero);
    }
}

static void Snapper_TurnClimb(StandaloneSnapper* snapper)
{
    Vec3f forward = snapper->climbForward;
    Vec3f normal = snapper->climbNormal;
    s16 angle = (s16)snapper->duel.headSpeed;
    f32 sine = Math_SinS(angle), cosine = Math_CosS(angle);
    Vec3f side = { normal.y * forward.z - normal.z * forward.y, normal.z * forward.x - normal.x * forward.z,
                   normal.x * forward.y - normal.y * forward.x };
    snapper->climbForward = (Vec3f){ forward.x * cosine + side.x * sine, forward.y * cosine + side.y * sine,
                                     forward.z * cosine + side.z * sine };
}

static void Snapper_CallMount(StandaloneSnapper* snapper)
{
    StandaloneDuel* duel = &snapper->duel;
    Actor* actor = &duel->actor;
    snapper->mountPhase = 1;
    actor->colChkInfo.health = 6;
    actor->speed = 0.f;
    actor->flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    s16 homeYaw = Math_Vec3f_Yaw(&actor->world.pos, &actor->home.pos);
    s16 playerAngle = actor->yawTowardsPlayer - homeYaw;
    actor->shape.rot.y = actor->world.rot.y = homeYaw + (playerAngle > 0 ? -0x2000 : 0x2000);
    duel->phase = GEKKO_CALL_MOUNT;
    duel->timer = 0;

    AnimationHeader* anim = Duel_Anim(0x1b08);
    Animation_Change(&duel->skel, anim, 1.f, 0.f, Animation_GetLastFrame(anim), ANIMMODE_ONCE, 3.f);
}

static void Gekko_DefeatEffects(Actor* actor, PlayState* play)
{
    Vec3f zero = { 0.f, 0.f, 0.f };
    Vec3f accel = { 0.f, -.5f, 0.f };
    Color_RGBA8 prim = { 250, 250, 250, 255 };
    Color_RGBA8 env = { 180, 180, 180, 255 };
    s16 yaw = Camera_GetCamDirYaw(Play_GetCamera(play, CAM_ID_MAIN)) + 0x8000;
    s16 size = DUEL_FIXED_FORM == DUEL_SLIME ? 500 : 800;
    Vec3f dust = { actor->world.pos.x + 20.f * Math_SinS(yaw),
                   actor->world.pos.y + (DUEL_FIXED_FORM == DUEL_SLIME ? 20.f : 25.f),
                   actor->world.pos.z + 20.f * Math_CosS(yaw) };
#if defined(GAME_MM)
    EffectSsDust_Spawn_2_Normal(play, &dust, &zero, &zero, &prim, &env, size, 50, 10);
#else
    EffectSsDust_Spawn(play, 0, &dust, &zero, &zero, &prim, &env, size, 50, 10, 0);
#endif
#if defined(GAME_MM)
    SoundSource_PlaySfxAtFixedWorldPos(play, &actor->world.pos, 40, 0x3a86);
#else
    FOREIGN_ENEMY_API_SFX_WORLD(play, &actor->world.pos, 40, 0x3a86);
#endif
    for (int i = 0; i < 25; ++i)
    {
        Vec3f velocity = { (Rand_ZeroOne() - .5f) * 5.f, Rand_ZeroOne() * 3.f + 4.f, (Rand_ZeroOne() - .5f) * 5.f };
        EffectSsHahen_Spawn(play, &actor->world.pos, &velocity, &accel, 0, 12 + (s16)(Rand_ZeroOne() * 3.f), -1, 10,
                            NULL);
    }
}

static void Snapper_Combat(StandaloneDuel* duel, PlayState* play, int finished)
{
    StandaloneSnapper* snapper = (StandaloneSnapper*)duel;
    Actor* actor = &duel->actor;
    EnemySnapperMount* mount = Snapper_GetMount(actor);
    if (snapper->wallHit)
    {
        actor->speed = 0.f;
        if (snapper->wallHit == 1)
        {
            actor->colorFilterTimer = 16;
            if (duel->timer)
                return;
            if (actor->gravity == 0.f)
            {
                actor->gravity = -1.f;
                actor->colChkInfo.mass = 50;
                return;
            }
            snapper->wallSpinYaw += 0xf00;
            if (actor->bgCheckFlags & BGCHECKFLAG_GROUND)
            {
                actor->shape.rot.y += snapper->wallSpinYaw;
                snapper->wallSpinYaw = 0;
                snapper->wallHit = 2;
                Gekko_Thaw(duel, play);
                Animation_PlayOnce(&duel->skel, Duel_Anim(0x4298));
                Snapper_WaterEffects(actor, play, 1);
                Duel_Sfx(actor, 0x2902);
            }
        }
        else if (snapper->wallHit == 3)
        {
            if (!duel->timer)
            {
                snapper->wallHit = 0;
                duel->phase = DUEL_DEFEAT;
                duel->timer = 38 + 20;
            }
        }
        else if (finished)
        {
            if (!actor->colChkInfo.health)
            {
                snapper->wallHit = 3;
                duel->timer = 4;
            }
            else if (duel->skel.animation == Duel_Anim(0x4298))
            {
                Animation_PlayOnce(&duel->skel, Duel_Anim(0x30e4));
            }
            else
            {
                snapper->wallHit = 0;
                if (mount)
                    Snapper_RunToMount(duel);
                else
                {
                    duel->phase = DUEL_APPROACH;
                    duel->walking = 0;
                }
            }
        }
        return;
    }
    if (Gekko_Recoil(duel, play))
        return;
    if (duel->phase >= GEKKO_CALL_MOUNT)
    {
        actor->speed = 0.f;
        if (!mount && duel->phase != GEKKO_CALL_MOUNT)
        {
            actor->shape.yOffset = 0.f;
            actor->gravity = -2.f;
            actor->flags |= ACTOR_FLAG_ATTENTION_ENABLED;
            duel->phase = DUEL_APPROACH;
            duel->exposed = 120;
            return;
        }
        switch (duel->phase)
        {
            case GEKKO_CALL_MOUNT:
                if (Animation_OnFrame(&duel->skel, 4.f))
                    Duel_Sfx(actor, 0x395c);
                if (duel->timer || !finished)
                    break;
                if (!mount)
                {
                    Snapper_SpawnMount(actor, play);
                    if (!Snapper_GetMount(actor))
                    {
                        duel->phase = DUEL_APPROACH;
                        duel->exposed = 120;
                        actor->flags |= ACTOR_FLAG_ATTENTION_ENABLED;
                    }
                    break;
                }
                if (mount->mode == SNAPPER_MOUNT_OVERTURNED)
                {
                    mount->mode = SNAPPER_MOUNT_EMERGE;
                    break;
                }
                if (mount->mode != SNAPPER_MOUNT_READY)
                    break;
                Snapper_RunToMount(duel);
                break;
            case GEKKO_RUN_TO_MOUNT: {
                Vec3f goal = mount->actor.world.pos;
                f32 distance = sqrtf(SQ(goal.x - actor->world.pos.x) + SQ(goal.z - actor->world.pos.z));
                Snapper_HopEffects(duel, play);
                Vec3f from = actor->world.pos;
                from.y += 25.f;
                Vec3f landing = goal;
                landing.y += mount->bodyHeight + 46.f;
                if (mount->mode == SNAPPER_MOUNT_OVERTURNED)
                    mount->mode = SNAPPER_MOUNT_EMERGE;
                if (mount->mode == SNAPPER_MOUNT_READY && distance < 120.f && Animation_OnFrame(&duel->skel, 0.f) &&
                    enemyFlightClear(play, &from, &landing))
                {
                    AnimationHeader* anim = Duel_Anim(0x4680);
                    Animation_Change(&duel->skel, anim, 1.f, 0.f, Animation_GetLastFrame(anim), ANIMMODE_ONCE, 6.f);
                    duel->phase = GEKKO_MOUNT_JUMP;
                    duel->timer = 6;
                    actor->velocity.y = actor->gravity = 0.f;
                    actor->world.rot.y = actor->shape.rot.y = Math_Vec3f_Yaw(&actor->world.pos, &goal);
                    Duel_Sfx(actor, 0x395c);
                    break;
                }
                if (!duel->timer)
                {
                    duel->phase = DUEL_APPROACH;
                    duel->exposed = 110;
                    actor->flags |= ACTOR_FLAG_ATTENTION_ENABLED;
                    break;
                }
                if (distance < 12.f)
                    break;
                Vec3f probe = from;
                f32 fraction = 12.f / distance;
                probe.x += (goal.x - actor->world.pos.x) * fraction;
                probe.z += (goal.z - actor->world.pos.z) * fraction;
                CollisionPoly* floor = NULL;
                s32 bgId;
                f32 height = BgCheck_EntityRaycastDown4(&play->colCtx, &floor, &bgId, actor, &probe);
                if (!floor || fabsf(height - actor->floorHeight) > 25.f || !enemyFlightClear(play, &from, &probe))
                {
                    duel->phase = DUEL_APPROACH;
                    duel->exposed = 110;
                    actor->flags |= ACTOR_FLAG_ATTENTION_ENABLED;
                    break;
                }
                actor->world.rot.y = actor->shape.rot.y = Math_Vec3f_Yaw(&actor->world.pos, &goal);
                if (!(actor->bgCheckFlags & BGCHECKFLAG_GROUND) ||
                    (duel->skel.curFrame > 1.f && duel->skel.curFrame < 12.f))
                    actor->speed = 12.f;
                break;
            }
            case GEKKO_MOUNT_JUMP: {
                mount->walkToRider = 0;
                Vec3f next = mount->actor.world.pos;
                next.y += mount->bodyHeight;
                if (duel->timer)
                {
                    f32 fraction = 1.f / duel->timer;
                    next.x = actor->world.pos.x + (next.x - actor->world.pos.x) * fraction;
                    next.z = actor->world.pos.z + (next.z - actor->world.pos.z) * fraction;
                    next.y += Math_SinS(0x6000 - duel->timer * 0x1000) * 65.f;
                }
                else
                {
                    next.y += 46.f;
                }
                if (mount->mode != SNAPPER_MOUNT_READY || !enemyFlightClear(play, &actor->world.pos, &next))
                {
                    actor->gravity = -2.f;
                    duel->phase = DUEL_RECOVER;
                    duel->timer = 30;
                    duel->exposed = 110;
                    actor->flags |= ACTOR_FLAG_ATTENTION_ENABLED;
                    break;
                }
                actor->world.pos = next;
                if (!duel->timer)
                {
                    mount->carrying = 1;
                    mount->mode = SNAPPER_MOUNT_REAR;
                    mount->riderLift = 0.f;
                    duel->walking = GEKKO_RIDER_IDLE;
                    duel->phase = GEKKO_MOUNT_LAND;
                    actor->shape.rot.y = mount->actor.shape.rot.y;
                    Animation_PlayOnce(&duel->skel, Duel_Anim(0x0994));
                }
                break;
            }
            case GEKKO_MOUNT_LAND:
                if (finished)
                {
                    actor->flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
                    duel->phase = mount->mode >= SNAPPER_MOUNT_SPINUP && mount->mode <= SNAPPER_MOUNT_CHARGE
                                      ? GEKKO_MOUNT_CHARGE
                                      : GEKKO_MOUNT_AIM;
                    Animation_PlayOnce(&duel->skel, Duel_Anim(0x4680));
                }
                break;
            case GEKKO_MOUNT_AIM:
                if (mount->mode >= SNAPPER_MOUNT_SPINUP && mount->mode <= SNAPPER_MOUNT_CHARGE)
                    duel->phase = GEKKO_MOUNT_CHARGE;
                Snapper_MountedPose(duel, mount, play, finished);
                break;
            case GEKKO_MOUNT_CHARGE:
                if (mount->mode == SNAPPER_MOUNT_RETRACT || mount->mode == SNAPPER_MOUNT_RECOVER)
                    duel->phase = GEKKO_MOUNT_AIM;
                Snapper_MountedPose(duel, mount, play, finished);
                break;
        }
        return;
    }
    if (duel->phase == GEKKO_DISMOUNT)
    {
        actor->shape.rot.x += 0x800;
        actor->shape.rot.z += 0x1000;
        Vec3f from = actor->world.pos;
        from.y += 20.f;
        Vec3f next = from;
        next.x += 14.f * Math_SinS(actor->world.rot.y);
        next.z += 14.f * Math_CosS(actor->world.rot.y);
        CollisionPoly* floor = NULL;
        s32 bgId;
        f32 height = BgCheck_EntityRaycastDown4(&play->colCtx, &floor, &bgId, actor, &next);
        actor->speed = floor && height >= actor->home.pos.y - 25.f && enemyFlightClear(play, &from, &next) ? 7.f : 0.f;
        if (actor->velocity.y > 0.f)
        {
            next = from;
            next.y += actor->velocity.y;
            if (!enemyFlightClear(play, &from, &next))
                actor->velocity.y = 0.f;
        }
        if (actor->bgCheckFlags & BGCHECKFLAG_GROUND)
        {
            actor->shape.rot.x = actor->shape.rot.z = 0;
            actor->speed = 0.f;
            if (Snapper_FindWall(duel, play))
            {
                duel->phase = GEKKO_WALL_JUMP;
                duel->timer = 50;
                actor->velocity.y = 0.f;
                actor->bgCheckFlags &= ~BGCHECKFLAG_WALL;
                Snapper_PlayGroundHops(duel);
                Duel_Sfx(actor, 0x39a1);
            }
            else
            {
                duel->phase = DUEL_RECOVER;
                duel->timer = 30;
                duel->exposed = 110;
                actor->flags |= ACTOR_FLAG_ATTENTION_ENABLED;
                Animation_PlayOnce(&duel->skel, Duel_Anim(0x4298));
            }
        }
        return;
    }
    if (duel->phase < GEKKO_WALL_JUMP)
    {
        if (snapper->mountPhase && mount && duel->phase == DUEL_APPROACH && !duel->exposed &&
            (actor->bgCheckFlags & BGCHECKFLAG_GROUND))
        {
            Snapper_RunToMount(duel);
        }
        else if (snapper->mountPhase && !mount && duel->phase == DUEL_APPROACH && !duel->exposed &&
                 actor->xzDistToPlayer < 300.f && fabsf(actor->yDistanceFromLink) < 85.f)
        {
            duel->phase = GEKKO_CALL_MOUNT;
            duel->timer = 24;
            actor->speed = 0.f;
            Duel_Sfx(actor, 0x395c);
            Animation_PlayOnce(&duel->skel, Duel_Anim(0x1b08));
        }
        else
        {
            Gekko_GroundCombat(duel, play, finished);
        }
        return;
    }
    actor->speed = 0.f;
    if (duel->phase == GEKKO_WALL_JUMP)
    {
        actor->shape.rot.y = actor->world.rot.y = duel->headYaw;
        f32 dx = actor->world.pos.x - duel->headPos.x;
        f32 dz = actor->world.pos.z - duel->headPos.z;
        int wall = (actor->bgCheckFlags & BGCHECKFLAG_WALL) && actor->wallBgId == BGCHECK_SCENE && actor->wallPoly &&
                   actor->wallPoly->normal.y < 0x4000;
        if ((actor->bgCheckFlags & BGCHECKFLAG_GROUND) && (wall || dx * dx + dz * dz < 16.f * 16.f))
        {
            if (wall)
                duel->headYaw = actor->wallYaw + 0x8000;
            if (!Snapper_BeginClimb(snapper, play))
                Snapper_StartWallFall(duel, play, mount);
        }
        else if (!duel->timer)
        {
            Snapper_StartWallFall(duel, play, mount);
        }
        else
        {
            Snapper_GroundHop(duel, play);
            if (actor->speed)
            {
                Vec3f from = actor->world.pos;
                from.y += 30.f;
                Vec3f next = from;
                next.x += 18.f * Math_SinS(duel->headYaw);
                next.z += 18.f * Math_CosS(duel->headYaw);
                CollisionPoly* floor = NULL;
                s32 bgId;
                f32 height = BgCheck_EntityRaycastDown4(&play->colCtx, &floor, &bgId, actor, &next);
                if (!floor || fabsf(height - actor->world.pos.y) > 25.f || !enemyFlightClear(play, &from, &next))
                {
                    actor->speed = 0.f;
                    if ((actor->bgCheckFlags & BGCHECKFLAG_GROUND) && !Snapper_BeginClimb(snapper, play))
                        Snapper_StartWallFall(duel, play, mount);
                }
            }
        }
    }
    else if (duel->phase == GEKKO_WALL_CLIMB)
    {
        int clear = Snapper_CrawlStep(snapper, play);
        Snapper_ClimbPose(snapper);
        if (Animation_OnFrame(&duel->skel, 15.f))
            Duel_Sfx(actor, 0x3828);
        if (!(play->gameplayFrames % 60) && Rand_ZeroOne() < .8f)
            Duel_Sfx(actor, 0x38d2);
        int descendingLow = actor->world.pos.y < actor->home.pos.y + 200.f && snapper->climbForward.y <= 0.f;
        if (!duel->timer || !clear || descendingLow)
        {
            if (--duel->walking)
            {
                duel->phase = GEKKO_WALL_PAUSE;
                if (!clear)
                {
                    duel->headSpeed = 0x8000 / 15.f;
                }
                else if (descendingLow)
                {
                    Vec3f normal = snapper->climbNormal;
                    Vec3f forward = snapper->climbForward;
                    f32 sideY = normal.z * forward.x - normal.x * forward.z;
                    duel->headSpeed = (sideY > 0.f ? 1.f : -1.f) * (0x4000 / 15.f);
                }
                else
                {
                    f32 turn = (0x1000 + Rand_ZeroOne() * 0x2000) / 15.f;
                    duel->headSpeed = (Rand_ZeroOne() < .5f ? -1.f : 1.f) * turn;
                }
                duel->timer = 15;
                duel->skel.playSpeed = 1.5f;
                Duel_Sfx(actor, 0x39ac);
            }
            else
            {
                Snapper_StartWallFall(duel, play, mount);
            }
        }
    }
    else if (duel->phase == GEKKO_WALL_PAUSE)
    {
        Snapper_TurnClimb(snapper);
        Snapper_ClimbPose(snapper);
        if (!(play->gameplayFrames % 60) && Rand_ZeroOne() < .8f)
            Duel_Sfx(actor, 0x38d2);
        if (!duel->timer)
        {
            duel->phase = GEKKO_WALL_CLIMB;
            duel->timer = 35 + (u8)(Rand_ZeroOne() * 15.f);
            duel->skel.playSpeed = 1.f;
            Duel_Sfx(actor, 0x39a9);
        }
    }
    else if (!(actor->bgCheckFlags & BGCHECKFLAG_GROUND))
    {
        Vec3f* destination = mount && actor->floorPoly ? &mount->actor.world.pos : &actor->home.pos;
        s16 yaw = Math_Vec3f_Yaw(&actor->world.pos, destination);
        Vec3f from = actor->world.pos;
        from.y += 20.f;
        Vec3f next = from;
        next.x += 5.f * Math_SinS(yaw);
        next.z += 5.f * Math_CosS(yaw);
        CollisionPoly* floor = NULL;
        s32 bgId;
        f32 height = BgCheck_EntityRaycastDown4(&play->colCtx, &floor, &bgId, actor, &next);
        if (floor && height >= actor->home.pos.y - 25.f && enemyFlightClear(play, &from, &next))
        {
            actor->world.rot.y = yaw;
            actor->speed = 5.f;
        }
    }
    else
    {
        if (mount)
        {
            actor->gravity = -2.f;
            if (mount->mode == SNAPPER_MOUNT_OVERTURNED)
                mount->mode = SNAPPER_MOUNT_EMERGE;
            Snapper_RunToMount(duel);
        }
        else
        {
            duel->phase = DUEL_RECOVER;
            duel->timer = 30;
            duel->exposed = 110;
            Animation_PlayOnce(&duel->skel, Duel_Anim(0x4298));
        }
    }
}

static u32 Slime_IceBreakMask(void)
{
#if defined(GAME_MM)
    return DMG_EXPLOSIVES | DMG_GORON_PUNCH | DMG_GORON_POUND | DMG_GORON_SPIKES | DMG_THROWN_OBJECT | DMG_POWDER_KEG;
#else
    return DMG_EXPLOSIVE | DMG_HAMMER;
#endif
}

static Vec3f Slime_VertexPosition(const StandaloneSlime* slime, int index)
{
    const Vec3f* origin = &slime->duel.actor.world.pos;
    const s16* vertex = slime->shellMesh[index].n.ob;
    return (Vec3f){ origin->x + vertex[0] * slime->shellScaleXZ,
                    origin->y + slime->shellOffsetY + vertex[1] * slime->shellScaleY,
                    origin->z + vertex[2] * slime->shellScaleXZ };
}

static void Slime_StartShatterEffects(StandaloneSlime* slime, int shockwave)
{
    Actor* actor = &slime->duel.actor;
    for (int i = 0; i < ARRAY_COUNT(slime->iceShards); ++i)
    {
        if (!slime->iceAlpha[i])
            continue;
        SlimeIceShard* shard = &slime->iceShards[i];
        shard->pos = Slime_VertexPosition(slime, i);
        shard->rot = (Vec3s){ Rand_Next() >> 16, Rand_Next() >> 16, Rand_Next() >> 16 };
        s16 pitch = 0x1000 + (s16)(Rand_ZeroOne() * 0x3000);
        f32 speed = 14.f + Rand_ZeroOne() * 5.f;
        f32 horizontal = speed * Math_CosS(pitch);
        f32 dx = shard->pos.x - actor->world.pos.x;
        f32 dz = shard->pos.z - actor->world.pos.z;
        f32 radius = MAX(1.f, sqrtf(dx * dx + dz * dz));
        shard->velocity = (Vec3f){ dx * horizontal / radius, speed * Math_SinS(pitch), dz * horizontal / radius };
        shard->scale = .002f + Rand_ZeroOne() * .006f;
    }
    slime->shockwavePos = actor->world.pos;
    slime->shockwavePos.y = actor->floorPoly ? actor->floorHeight + .5f : actor->world.pos.y;
    if (shockwave)
    {
        slime->shockwaveAlpha = 235;
        slime->shockwaveScale = .025f;
    }
    Duel_Sfx(actor, NA_SE_EV_ICE_BROKEN);
}

static void Slime_UpdateShatterEffects(StandaloneSlime* slime)
{
#if defined(GAME_OOT)
    for (int i = 0; i < ARRAY_COUNT(slime->meltSmoke); ++i)
    {
        if (!slime->meltSmoke[i].life)
            continue;
        --slime->meltSmoke[i].life;
        slime->meltSmoke[i].pos.y += 2.f;
    }
#endif
    if (slime->shockwaveAlpha)
    {
        slime->shockwaveAlpha = MAX(0, (int)slime->shockwaveAlpha - 10);
        slime->shockwaveScale += .0013f;
    }
    for (int i = 0; i < ARRAY_COUNT(slime->iceShards); ++i)
    {
        SlimeIceShard* shard = &slime->iceShards[i];
        if (!shard->scale)
            continue;
        shard->velocity.y -= 1.f;
        shard->pos.x += shard->velocity.x;
        shard->pos.y += shard->velocity.y;
        shard->pos.z += shard->velocity.z;
        if (shard->pos.y < slime->shockwavePos.y - 20.f)
        {
            shard->scale = 0.f;
            continue;
        }
        shard->rot.x += (s16)((Rand_Next() >> 23) + 0x700);
        shard->rot.y += (s16)((Rand_Next() >> 23) + 0x900);
        shard->rot.z += (s16)((Rand_Next() >> 23) + 0xb00);
    }
}

static void Slime_Shatter(StandaloneSlime* slime, PlayState* play)
{
    StandaloneDuel* duel = &slime->duel;
    Actor* actor = &duel->actor;
    duel->phase = DUEL_APPROACH;
    duel->walking = 1;
    duel->exposed = 150;
    duel->timer = 0;
    slime->cycle = 100;
    slime->jumpRounds = 2;
    slime->groundYaw = actor->yawTowardsPlayer + 0x8000;
    s16 quake = Quake_Request(Play_GetCamera(play, CAM_ID_MAIN), 3);
    Quake_SetSpeed(quake, 20000);
    Quake_SetPerturbations(quake, 15, 0, 0, 0);
    Quake_SetDuration(quake, 15);
    Rumble_Request(actor->xyzDistToPlayerSq, 180, 20, 100);
    Slime_StartShatterEffects(slime, 1);
    Duel_Sfx(actor, 0x38db);
    slime->waitingForShatterLanding = 1;
    actor->colChkInfo.mass = 50;
#if defined(GAME_MM)
    actor->hintId = TATL_HINT_ID_GEKKO_GIANT_SLIME;
#endif
    actor->flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    actor->bgCheckFlags &= ~BGCHECKFLAG_GROUND;
    actor->gravity = -2.f;
    actor->world.rot.y = actor->shape.rot.y = 0;
    Animation_PlayLoop(&duel->skel, Duel_Anim(0x39c4));
    static const u8 spawnVertices[MINISLIME_COUNT] = { 7, 9, 11, 13, 15, 52, 56, 60, 64, 68, 94, 98, 102, 106, 110 };
    Slime_ClearFragments(slime);
    for (int i = 0; i < MINISLIME_COUNT; ++i)
    {
        Vec3f pos = Slime_VertexPosition(slime, spawnVertices[i]);
        s16 yaw = Math_Vec3f_Yaw(&actor->world.pos, &pos);
        s16 pitch = 0x1000 + 0x333 * (MINISLIME_COUNT - i);
        Actor* child = Actor_Spawn(&play->actorCtx, play, ACTOR_ENEMY_MINISLIME, pos.x, pos.y, pos.z, 0, yaw, 0, i);
        if (!child)
            continue;
        EnemyMinislime* fragment = (EnemyMinislime*)child;
        slime->fragments[i] = fragment;
        fragment->owner = actor;
        fragment->ownerSlot = &slime->fragments[i];
        f32 speed = Math_CosS(pitch) * 15.f;
        child->velocity = (Vec3f){ Math_SinS(yaw) * speed, Math_SinS(pitch) * 15.f + 2.f, Math_CosS(yaw) * speed };
    }
}

static void Slime_NextGrabMove(StandaloneSlime* slime)
{
    static const u16 attacks[] = { 0x347c, 0x70c4, 0x3f28 };
    slime->grabMove = Rand_ZeroOne() * ARRAY_COUNT(attacks);
    Animation_PlayOnce(&slime->duel.skel, Duel_Anim(attacks[slime->grabMove]));
}

static f32 Slime_ThrowTravel(s32 timer)
{
    return timer > 5 ? 0.f : .5f - .5f * Math_CosS((5 - timer) * 0x8000 / 5);
}

static void Slime_UpdateThrowMesh(StandaloneSlime* slime, s32 timer)
{
    s32 windup = timer - 5;
    f32 scale = windup > 0 ? 1.f + Math_CosS(windup * 0x8000 / 27) : 2.f - 3.f * Slime_ThrowTravel(timer);
    for (int i = 0; i < ARRAY_COUNT(sGekkoSlimeVertices); ++i)
    {
        const s16* rest = sGekkoSlimeVertices[i].n.ob;
        s16* point = &slime->throwMesh[i].x;
        f32 perturbation = slime->throwPerturbation[i];
        for (int axis = 0; axis < 3; ++axis)
        {
            s32 target = rest[axis] - (s32)(scale * rest[axis] * perturbation);
            if (windup > 0)
                point[axis] += (target - point[axis]) / windup;
            else
                point[axis] = target;
        }
    }
}

static void Slime_CopyClippedMesh(StandaloneSlime* slime)
{
    for (int i = 0; i < ARRAY_COUNT(slime->throwMesh); ++i)
    {
        const s16* vertex = slime->shellMesh[i].n.ob;
        slime->throwMesh[i] = (Vec3s){ vertex[0], vertex[1], vertex[2] };
    }
}

static void Slime_GrabSfx(StandaloneSlime* slime, u16 sound)
{
    Actor* actor = &slime->duel.actor;
#if defined(GAME_MM)
    slime->grabReverb = 1;
    ((void (*)(s8))0x8019f540)(1);
    ((void (*)(Vec3f*, u16))0x8019f420)(&actor->projectedPos, sound);
#else
    Duel_Sfx(actor, sound);
#endif
}

static void Slime_StartThrow(StandaloneSlime* slime)
{
    StandaloneDuel* duel = &slime->duel;
    Actor* actor = &duel->actor;
    f32 sinYaw = Math_SinS(actor->world.rot.y), cosYaw = Math_CosS(actor->world.rot.y);
    Slime_CopyClippedMesh(slime);
    for (int i = 0; i < ARRAY_COUNT(slime->throwMesh); ++i)
    {
        Vec3s* point = &slime->throwMesh[i];
        f32 dot = (point->x * sinYaw + point->y + point->z * cosYaw) * .00070710678f;
        slime->throwPerturbation[i] = dot < .01f ? 0.f : dot * .5f;
    }
    slime->grabYaw = actor->world.rot.y;
    slime->grabOffset = -50.f;
    duel->phase = SLIME_THROW;
    duel->timer = 32;
    Slime_GrabSfx(slime, 0x3962);
    Animation_PlayOnce(&duel->skel, Duel_Anim(0xf048));
}

static int Slime_SetGrabbedPosition(PlayState* play, Player* player, Vec3f* destination)
{
    Vec3f from = player->actor.world.pos;
    Vec3f to = *destination;
    f32 clearance = MAX(12.f, player->cylinder.dim.height * .5f);
    from.y += clearance;
    to.y += clearance;
    if (!enemyFlightClear(play, &from, &to))
        return 0;
    player->actor.world.pos = *destination;
    return 1;
}

static void Slime_AbortGrab(StandaloneSlime* slime, PlayState* play)
{
    StandaloneDuel* duel = &slime->duel;
    Igos_ReleasePlayer(&duel->actor, play);
    Slime_CopyClippedMesh(slime);
    slime->throwRecovery = 1;
    duel->phase = SLIME_GROUND;
    duel->timer = 10;
}

static void Slime_GrabCombat(StandaloneSlime* slime, PlayState* play, Player* player, int finished)
{
    StandaloneDuel* duel = &slime->duel;
    Actor* actor = &duel->actor;
#if defined(GAME_MM)
    int held = player->stateFlags2 & PLAYER_STATE2_MM_80;
#else
    int held = player->stateFlags2 & (1 << 7);
#endif
    if (player->actor.parent != actor || !held)
    {
        Slime_AbortGrab(slime, play);
        return;
    }
    if (duel->phase == SLIME_GRAB &&
        slime->grabStruggle < SLIME_GRAB_STRUGGLE_PRESSES &&
        (play->state.input[0].press.button & (BTN_A | BTN_B)))
        ++slime->grabStruggle;
    player->av2.actionVar2 = 0;
    Math_ScaledStepToS(&slime->shellGekkoPitch, 0, 0x400);
    f32 grabY = actor->world.pos.y - 500.f * slime->shellScaleY;
    if (actor->floorPoly)
        grabY = MAX(actor->floorHeight, grabY);
    if (duel->phase == SLIME_THROW)
    {
        Slime_UpdateThrowMesh(slime, duel->timer);
        f32 travel = Slime_ThrowTravel(duel->timer);
        Vec3f destination = player->actor.world.pos;
        destination.y = grabY;
        if (duel->timer <= 5)
        {
            f32 distance = 1060.5f * slime->shellScaleXZ * travel;
            destination.x = actor->world.pos.x + Math_SinS(actor->world.rot.y) * distance;
            destination.z = actor->world.pos.z + Math_CosS(actor->world.rot.y) * distance;
            destination.y += 1060.5f * slime->shellScaleY * travel;
            slime->grabOffset *= 1.f - 1.f / (duel->timer + 1);
        }
        int clear = Slime_SetGrabbedPosition(play, player, &destination);
        if (duel->timer == 5)
        {
            Duel_Sfx(actor, 0x38d8);
            Slime_GrabSfx(slime, 0x3961);
        }
        if (!duel->timer)
        {
            Slime_EndGrabReverb(slime);
            player->actor.parent = NULL;
            player->av2.actionVar2 = 100;
            player->actor.velocity.y = 0.f;
            FOREIGN_ENEMY_API_KNOCKBACK_LARGE(play, actor, clear ? 10.f : 0.f, actor->world.rot.y, 10.f, 4);
            duel->phase = SLIME_GROUND;
            duel->timer = 10;
            slime->throwRecovery = 1;
        }
        return;
    }
    Vec3f destination = player->actor.world.pos;
    destination.y = grabY;
    if (!slime->grabRounds)
    {
        f32 step = 1.f / (duel->timer + 1);
        slime->shellScaleXZ += (.15f - slime->shellScaleXZ) * step;
        slime->shellScaleY += (.075f - slime->shellScaleY) * step;
        grabY = actor->world.pos.y - 500.f * slime->shellScaleY;
        if (actor->floorPoly)
            grabY = MAX(actor->floorHeight, grabY);
        slime->shellOffsetY *= 1.f - step;
        for (int i = 0; i < ARRAY_COUNT(slime->throwMesh); ++i)
        {
            const s16* rest = sGekkoSlimeVertices[i].n.ob;
            Vec3s* point = &slime->throwMesh[i];
            point->x += (s16)((rest[0] - point->x) * step);
            point->y += (s16)((rest[1] - point->y) * step);
            point->z += (s16)((rest[2] - point->z) * step);
        }
        destination.x += (actor->world.pos.x - destination.x) * step;
        destination.y = player->actor.world.pos.y + (grabY - player->actor.world.pos.y) * step;
        destination.z += (actor->world.pos.z - destination.z) * step;
        if (!Slime_SetGrabbedPosition(play, player, &destination))
        {
            Slime_AbortGrab(slime, play);
            return;
        }
        slime->grabOffset = -50.f * (14 - duel->timer) / 15.f;
        if (!duel->timer)
        {
            slime->grabRounds = 6;
            Slime_NextGrabMove(slime);
            slime->grabSwings = 1 + (u8)(Rand_ZeroOne() * 3.f);
        }
        return;
    }
    Duel_Sfx(actor, 0x3939 - SFX_FLAG);
    s16 pulse = slime->grabImpact * 0x3333;
    if (slime->grabImpact)
        --slime->grabImpact;
    if (Animation_OnFrame(&duel->skel, slime->grabMove == 1 ? 9.f : 2.f))
    {
        slime->grabImpact = 10;
        Slime_GrabSfx(slime, slime->grabMove == 2 ? 0x3966 : 0x3961);
        destination.x += 20.f * Math_SinS(slime->grabYaw);
        destination.z += 20.f * Math_CosS(slime->grabYaw);
    }
    else
    {
        Math_StepToF(&destination.x, actor->world.pos.x, 4.f);
        Math_StepToF(&destination.z, actor->world.pos.z, 4.f);
    }
    f32 strength = .2f + .08f * slime->grabImpact;
    slime->shellScaleXZ = .15f + .015f * strength * Math_SinS(pulse);
    slime->shellScaleY = .075f + .005f * strength * Math_CosS(pulse);
    destination.y = actor->world.pos.y - 500.f * slime->shellScaleY;
    if (actor->floorPoly)
        destination.y = MAX(destination.y, actor->floorHeight);
    if (!Slime_SetGrabbedPosition(play, player, &destination))
    {
        Slime_AbortGrab(slime, play);
        return;
    }
    if (!finished)
        return;
    if (slime->grabStruggle >= SLIME_GRAB_STRUGGLE_PRESSES)
    {
        Slime_StartThrow(slime);
        return;
    }
    if (!--slime->grabSwings)
    {
        if (!--slime->grabRounds || gSave.info.playerData.health < 5)
        {
            Slime_StartThrow(slime);
            return;
        }
        play->damagePlayer(play, -4);
#if defined(GAME_MM)
        Player_PlaySfx(player, NA_SE_VO_LI_DAMAGE_S + player->ageProperties->voiceSfxIdOffset);
#else
        Player_PlaySfx(player, comboIsLinkAdult()
            ? NA_SE_VO_LI_DAMAGE_S
            : NA_SE_VO_LI_DAMAGE_S_KID);
#endif
        s16 yawStep = 0x4000 + (s16)(Rand_ZeroOne() * 0x4000);
        slime->grabYaw += Rand_ZeroOne() < .5f ? -yawStep : yawStep;
        slime->grabOffset = -50.f;
        slime->grabSwings = 1 + (u8)(Rand_ZeroOne() * 3.f);
    }
    Slime_NextGrabMove(slime);
}

static const u8 sSlimeRingStart[] = { 0, 1, 6, 16, 31, 51, 71, 91, 111, 131, 146, 156, 161, 162 };

static void Slime_ConeVertex(Vtx* vertex, int ring, int inverse, int wide, f32 blend);

static void Slime_BeginRise(StandaloneSlime* slime)
{
    StandaloneDuel* duel = &slime->duel;
    Actor* actor = &duel->actor;
    slime->riseStart = actor->floorPoly ? actor->floorHeight : actor->world.pos.y;
    slime->riseVelocity = 0.f;
    slime->riseCounter = 0;
    if (slime->shellScaleXZ > .001f && slime->shellScaleY > .001f)
    {
        slime->riseScaleX = slime->shellScaleXZ;
        slime->riseScaleY = slime->shellScaleY;
        Slime_CopyClippedMesh(slime);
    }
    else
    {
        slime->riseScaleX = .15f;
        slime->riseScaleY = .025f;
        for (int i = 0; i < ARRAY_COUNT(sGekkoSlimeVertices); ++i)
        {
            const s16* v = sGekkoSlimeVertices[i].n.ob;
            slime->throwMesh[i] = (Vec3s){ v[0], v[1], v[2] };
        }
    }
    duel->phase = SLIME_RISE;
    actor->bgCheckFlags &= ~(BGCHECKFLAG_GROUND | BGCHECKFLAG_GROUND_TOUCH);
    slime->shellGrounded = 0;
    actor->gravity = 0.f;
    actor->velocity.y = 0.f;
    actor->world.rot.y = actor->shape.rot.y;
    Animation_PlayLoop(&duel->skel, Duel_Anim(0x7790));
    Duel_Sfx(actor, 0x3965);
    Duel_Sfx(actor, 0x3963);
    Duel_Sfx(actor, 0x392e);
}

static f32 Slime_RiseHeightScale(const StandaloneSlime* slime)
{
    return MIN(slime->riseScaleY, (slime->ceiling - slime->riseStart) * .0005f);
}

static void Slime_Rise(StandaloneSlime* slime)
{
    StandaloneDuel* duel = &slime->duel;
    Actor* actor = &duel->actor;
    actor->velocity.y = 0.f;
    if (slime->riseCounter >= 10 &&
        (fabsf(actor->world.pos.y - (slime->ceiling - 1000.f * Slime_RiseHeightScale(slime))) < 1.f ||
         (actor->bgCheckFlags & BGCHECKFLAG_CEILING)))
    {
        duel->phase = SLIME_CEILING;
        slime->ceilingMoveTimer = 320;
        slime->waveTimer = 0;
        Animation_PlayLoop(&duel->skel, Duel_Anim(0x7790));
        return;
    }

    int stage = slime->riseCounter;
    Math_ScaledStepToS(&slime->shellGekkoPitch, stage < 2 ? -0x4000 : 0, 0x400);
    if (stage < 2)
        Math_SmoothStepToF(&slime->riseScaleX, .13f, .2f, .039f, .0039f);
    else
        Math_SmoothStepToF(&slime->riseScaleX, .15f, .4f, .015f, .0015f);

    if (slime->riseCounter == 0)
    {
        if (Math_SmoothStepToF(&slime->riseScaleY, .3f, .5f, .015f, .00075f) < .01f)
            slime->riseCounter = 1;
    }
    else if (slime->riseCounter == 1)
    {
        if (Math_SmoothStepToF(&slime->riseScaleY, .075f, .4f, .0075f, .00075f) < .01f)
            slime->riseCounter = 2;
    }
    else if (slime->riseCounter < 10)
    {
        ++slime->riseCounter;
    }

    for (int ring = 0; ring < ARRAY_COUNT(sSlimeRingStart) - 1; ++ring)
    {
        for (int i = sSlimeRingStart[ring]; i < sSlimeRingStart[ring + 1]; ++i)
        {
            Vtx target = sGekkoSlimeVertices[i];
            if (slime->riseCounter < 2)
                Slime_ConeVertex(&target, ring, slime->riseCounter == 1, 0, 1.f);
            Vec3s* point = &slime->throwMesh[i];
            Math_SmoothStepToS(&point->x, target.n.ob[0], 5, 40, 5);
            Math_SmoothStepToS(&point->y, target.n.ob[1], 5, stage == 1 ? 50 : 550, 3);
            Math_SmoothStepToS(&point->z, target.n.ob[2], 5, 40, 5);
        }
    }
    f32 extent = 1000.f * Slime_RiseHeightScale(slime);
    f32 targetY = slime->riseCounter == 0 ? slime->riseStart + extent : slime->ceiling - extent;
    Math_StepToF(&slime->riseVelocity, 12.f, 1.f);
    Math_StepToF(&actor->world.pos.y, targetY, slime->riseVelocity);
}

static void Slime_ConstrainHeight(StandaloneSlime* slime, PlayState* play)
{
    StandaloneDuel* duel = &slime->duel;
    Actor* actor = &duel->actor;
    if (duel->phase < SLIME_RISE || duel->phase > SLIME_FROZEN || duel->phase == SLIME_GRAB)
        return;
    if (duel->phase == SLIME_CEILING || (duel->phase == SLIME_FALL && slime->dropWarningActive))
        return;
    f32 lower = Slime_RiseHeightScale(slime) * 1000.f;
    f32 upper = lower;
    if (duel->phase == SLIME_FROZEN)
    {
        lower = MAX(0.f, -slime->frozenMinY * slime->shellScaleY - slime->shellOffsetY);
        upper = MAX(0.f, slime->frozenMaxY * slime->shellScaleY + slime->shellOffsetY);
    }
    if (lower <= 0.f && upper <= 0.f)
        return;
    f32 floor = actor->floorHeight;
    actor->bgCheckFlags &= ~(BGCHECKFLAG_GROUND_TOUCH | BGCHECKFLAG_CEILING);
    if (actor->floorPoly && actor->world.pos.y - lower <= floor + .1f)
    {
        actor->world.pos.y = floor + lower;

        if (!slime->shellGrounded)
            actor->bgCheckFlags |= BGCHECKFLAG_GROUND_TOUCH;
        slime->shellGrounded = 1;
        actor->bgCheckFlags |= BGCHECKFLAG_GROUND;
        actor->velocity.y = 0.f;
    }
    else
    {
        slime->shellGrounded = 0;
        actor->bgCheckFlags &= ~BGCHECKFLAG_GROUND;
    }
    if (duel->phase == SLIME_FALL)
        return;
    Vec3f end = actor->world.pos;
    end.y += upper;
    Vec3f hit;
    CollisionPoly* poly;
    s32 bgId;
    if (upper > 0.f && BgCheck_EntityLineTest1(&play->colCtx, &actor->world.pos, &end, &hit, &poly, 0, 0, 1, 0, &bgId))
    {
        actor->world.pos.y = MAX(actor->floorPoly ? floor + lower : hit.y - upper, hit.y - upper);
        if (duel->phase == SLIME_RISE && hit.y - 2.f < slime->ceiling)
            slime->ceiling = MAX(slime->riseStart + 2.f, hit.y - 2.f);
        actor->bgCheckFlags |= BGCHECKFLAG_CEILING;
        if (actor->velocity.y > 0.f)
            actor->velocity.y = 0.f;
    }
}

enum { SLIME_WALL_CANDIDATES = 48 };
typedef struct {
    const CollisionPoly* walls[SLIME_WALL_CANDIDATES];
    int count;
    Vec3f center, extent;
} SlimeWallQuery;

static int Slime_WallList(SlimeWallQuery* query, u16 index, const SSNode* nodes, int nodeCount,
                          const CollisionPoly* polygons, int polygonCount)
{
    int budget = nodeCount;
    while (index != 0xffff)
    {
        if (!nodes || !polygons || index >= nodeCount || !budget--)
            return 0;
        int id = nodes[index].polyId;
        if (id < 0 || id >= polygonCount)
            return 0;
        const CollisionPoly* poly = &polygons[id];
        index = nodes[index].next;
        if (poly->flags_vIA & 0x4000)
            continue;
        f32 distance = query->center.x * poly->normal.x + query->center.y * poly->normal.y +
                       query->center.z * poly->normal.z + poly->dist * 32767.f;
        f32 radius = query->extent.x * ABS((s32)poly->normal.x) + query->extent.y * ABS((s32)poly->normal.y) +
                     query->extent.z * ABS((s32)poly->normal.z);
        if (fabsf(distance) > radius + 65534.f)
            continue;
        int i;
        for (i = 0; i < query->count; ++i)
            if (query->walls[i] == poly)
                break;
        if (i < query->count)
            continue;
        if (query->count == SLIME_WALL_CANDIDATES)
            return 0;
        query->walls[query->count++] = poly;
    }
    return 1;
}

static void Slime_FindWalls(SlimeWallQuery* query, PlayState* play, Actor* actor, Vtx* vertices, f32 height,
                            f32 scaleXZ, f32 scaleY)
{
    CollisionContext* col = &play->colCtx;
    Vec3f low = actor->world.pos, high = low;
    low.y += height;
    high.y += height;
    query->count = -1;
    if (!col->colHeader || !col->lookupTbl || col->subdivAmount.x <= 0 || col->subdivAmount.y <= 0 ||
        col->subdivAmount.z <= 0)
        return;
    for (int i = 0; i < SLIME_VERTEX_COUNT; ++i)
    {
        Vec3f point = { actor->world.pos.x + vertices[i].n.ob[0] * scaleXZ,
                        actor->world.pos.y + height + vertices[i].n.ob[1] * scaleY,
                        actor->world.pos.z + vertices[i].n.ob[2] * scaleXZ };
        low.x = MIN(low.x, point.x);
        high.x = MAX(high.x, point.x);
        low.y = MIN(low.y, point.y);
        high.y = MAX(high.y, point.y);
        low.z = MIN(low.z, point.z);
        high.z = MAX(high.z, point.z);
    }
    query->center = (Vec3f){ (low.x + high.x) * .5f, (low.y + high.y) * .5f, (low.z + high.z) * .5f };
    query->extent = (Vec3f){ (high.x - low.x) * .5f + 2.f, (high.y - low.y) * .5f + 2.f, (high.z - low.z) * .5f + 2.f };
    Vec3i first = { (low.x - 2.f - col->minBounds.x) * col->subdivLengthInv.x,
                    (low.y - 2.f - col->minBounds.y) * col->subdivLengthInv.y,
                    (low.z - 2.f - col->minBounds.z) * col->subdivLengthInv.z };
    Vec3i last = { (high.x + 2.f - col->minBounds.x) * col->subdivLengthInv.x,
                   (high.y + 2.f - col->minBounds.y) * col->subdivLengthInv.y,
                   (high.z + 2.f - col->minBounds.z) * col->subdivLengthInv.z };
    first.x = CLAMP(first.x, 0, col->subdivAmount.x - 1);
    last.x = CLAMP(last.x, 0, col->subdivAmount.x - 1);
    first.y = CLAMP(first.y, 0, col->subdivAmount.y - 1);
    last.y = CLAMP(last.y, 0, col->subdivAmount.y - 1);
    first.z = CLAMP(first.z, 0, col->subdivAmount.z - 1);
    last.z = CLAMP(last.z, 0, col->subdivAmount.z - 1);
    query->count = 0;
    for (int z = first.z; z <= last.z; ++z)
        for (int y = first.y; y <= last.y; ++y)
            for (int x = first.x; x <= last.x; ++x)
            {
                StaticLookup* cell = &col->lookupTbl[x + col->subdivAmount.x * (y + col->subdivAmount.y * z)];
                if (!Slime_WallList(query, cell->wall.head, col->polyNodes.tbl, col->polyNodes.count,
                                    col->colHeader->polyList, col->colHeader->numPolygons))
                    goto fallback;
            }
    for (int i = 0; i < BG_ACTOR_MAX; ++i)
    {
        if (!(col->dyna.bgActorFlags[i] & 1) || (col->dyna.bgActorFlags[i] & 2))
            continue;
        if (!Slime_WallList(query, col->dyna.bgActors[i].dynaLookup.wall.head, col->dyna.polyNodes.tbl,
                            col->dyna.polyNodes.count, col->dyna.polyList, col->dyna.polyListMax))
            goto fallback;
    }
    return;
fallback:
    query->count = -1;
}

static int Slime_WallRayNeeded(const SlimeWallQuery* query, const Vec3f* from, const Vec3f* to)
{
    if (query->count < 0)
        return 1;
    for (int i = 0; i < query->count; ++i)
    {
        const CollisionPoly* poly = query->walls[i];
        f32 a = from->x * poly->normal.x + from->y * poly->normal.y + from->z * poly->normal.z + poly->dist * 32767.f;
        f32 b = to->x * poly->normal.x + to->y * poly->normal.y + to->z * poly->normal.z + poly->dist * 32767.f;
        if (!((a > 65534.f && b > 65534.f) || (a < -65534.f && b < -65534.f)))
            return 1;
    }
    return 0;
}

static int Slime_ClipWallVertices(PlayState* play, Actor* actor, Vtx* vertices, f32 height, f32 scaleXZ, f32 scaleY)
{
    if (scaleXZ <= .001f)
        return 0;
    SlimeWallQuery walls;
    Slime_FindWalls(&walls, play, actor, vertices, height, scaleXZ, scaleY);
    if (!walls.count)
        return 0;
    int clipped = 0;
    for (int i = 0; i < ARRAY_COUNT(sGekkoSlimeVertices); ++i)
    {
        s16* point = vertices[i].n.ob;
        if (!point[0] && !point[2])
            continue;
        Vec3f center = actor->world.pos;
        center.y += height + point[1] * scaleY;
        Vec3f end = center, hit;
        end.x += point[0] * scaleXZ;
        end.z += point[2] * scaleXZ;
        CollisionPoly* poly;
        s32 bgId;
        if (Slime_WallRayNeeded(&walls, &center, &end) &&
            BgCheck_EntityLineTest1(&play->colCtx, &center, &end, &hit, &poly, 1, 0, 0, 0, &bgId))
        {
            point[0] = (hit.x - center.x) / scaleXZ;
            point[2] = (hit.z - center.z) / scaleXZ;
            if (point[1] != -1000)
                point[1] += 100;
            clipped = 1;
        }
    }
    return clipped;
}

static int Slime_CanReachPlayer(PlayState* play, Actor* actor, Player* player)
{
    Vec3f from = actor->world.pos;
    Vec3f to = player->actor.world.pos;
    Vec3f hit;
    CollisionPoly* poly;
    s32 bgId;
    from.y += 24.f;
    to.y += 24.f;
    return !BgCheck_EntityLineTest1(&play->colCtx, &from, &to, &hit, &poly, 1, 0, 0, 0, &bgId);
}

static void Slime_UpdateMeshNormals(Vtx* vertices);

static int Slime_ClipContactHeight(const Vec3f* in, int count, Vec3f* out, f32 height, int upper)
{
    int written = 0;
    for (int i = 0; i < count; ++i)
    {
        const Vec3f* a = &in[i];
        const Vec3f* b = &in[(i + 1) % count];
        int keepA = upper ? a->y <= height : a->y >= height;
        int keepB = upper ? b->y <= height : b->y >= height;
        if (keepA)
            out[written++] = *a;
        if (keepA != keepB)
        {
            f32 t = (height - a->y) / (b->y - a->y);
            out[written++] = (Vec3f){ a->x + t * (b->x - a->x), height, a->z + t * (b->z - a->z) };
        }
    }
    return written;
}

static void Slime_StartSquish(StandaloneSlime* slime)
{
    StandaloneDuel* duel = &slime->duel;
    Actor* actor = &duel->actor;
    Slime_CopyClippedMesh(slime);
    duel->phase = SLIME_GROUND;
    duel->timer = 20;
    slime->squishRecovery = 1;
    slime->shellScaleXZ = .2f;
    slime->shellScaleY = .05f;
    slime->shellOffsetY = 0.f;
    actor->velocity.y = 0.f;
    if (actor->floorPoly)
        actor->world.pos.y = actor->floorHeight + 50.f;
    Duel_Sfx(actor, 0x392f);
}

static int Slime_TryGrab(StandaloneSlime* slime, PlayState* play, Player* player)
{
    StandaloneDuel* duel = &slime->duel;
    Actor* actor = &duel->actor;
    if (duel->phase != SLIME_GROUND || !slime->squishRecovery || !play->grabPlayer(play, player))
        return 0;
    player->actor.parent = actor;
    player->av2.actionVar2 = 0;
    duel->phase = SLIME_GRAB;
    duel->timer = 15;
    slime->squishRecovery = 0;
    slime->grabRounds = slime->grabImpact = slime->grabSwings = 0;
    slime->grabStruggle = 0;
    slime->grabOffset = 0.f;
    slime->grabYaw = actor->world.rot.y;
    Slime_CopyClippedMesh(slime);
    Animation_PlayLoop(&duel->skel, Duel_Anim(0xf990));
    Duel_Sfx(actor, 0x38d4);
    return 1;
}

static void Slime_FrozenImpact(StandaloneSlime* slime, PlayState* play)
{
    Actor* actor = &slime->duel.actor;
    if (slime->frozenFallHit)
        return;
    slime->frozenFallHit = 1;
    FOREIGN_ENEMY_API_KNOCKBACK_LARGE(play, actor, 7.f, actor->yawTowardsPlayer, 5.f, 0x10);
}

static void Slime_ApplyPlayerPush(PlayState* play, Player* player, f32 x, f32 z)
{
    Actor* link = &player->actor;
    Vec3f from = link->world.pos;
    from.y += player->cylinder.dim.yShift + player->cylinder.dim.height * .5f;
    Vec3f to = { from.x + x, from.y, from.z + z };
    if (enemyFlightClear(play, &from, &to))
    {
        link->colChkInfo.displacement.x += x;
        link->colChkInfo.displacement.z += z;
    }
}

static void Slime_PushPlayer(StandaloneSlime* slime, PlayState* play, Vec3f* world)
{
    Actor* actor = &slime->duel.actor;
    int frozenFall = slime->duel.phase == SLIME_FROZEN && slime->frozenReady && !slime->frozenOnGround;
    Player* player = GET_PLAYER(play);
    Actor* link = &player->actor;
    f32 contactReach = MAX(200.f, slime->shellScaleXZ * 1000.f + player->cylinder.dim.radius);
    if (link->parent || actor->xzDistToPlayer > contactReach || player->cylinder.dim.radius <= 0)
        return;
    if (!Slime_CanReachPlayer(play, actor, player))
        return;

    Cylinder16* body = &player->cylinder.dim;
    f32 radius = body->radius;
    f32 bottom = body->pos.y + body->yShift;
    f32 top = bottom + body->height;
    f32 nearest = 1e10f;
    Vec3f offset = { 0.f, 0.f, 0.f };
    int inside = 0;
    for (int i = 0; i < ARRAY_COUNT(sSlimeTriangles); ++i)
    {
        const u8* face = sSlimeTriangles[i];
        Vec3f triangle[3] = { world[face[0]], world[face[1]], world[face[2]] };
        Vec3f low[5], clipped[5];
        int count = Slime_ClipContactHeight(triangle, 3, low, bottom, 0);
        count = Slime_ClipContactHeight(low, count, clipped, top, 1);
        if (count < 2)
            continue;
        int coversCenter = 0;
        f32 normalX = (triangle[1].y - triangle[0].y) * (triangle[2].z - triangle[0].z) -
                      (triangle[1].z - triangle[0].z) * (triangle[2].y - triangle[0].y);
        f32 normalZ = (triangle[1].x - triangle[0].x) * (triangle[2].y - triangle[0].y) -
                      (triangle[1].y - triangle[0].y) * (triangle[2].x - triangle[0].x);
        for (int j = 0; j < count; ++j)
        {
            const Vec3f* a = &clipped[j];
            const Vec3f* b = &clipped[(j + 1) % count];
            f32 x = a->x - body->pos.x;
            f32 z = a->z - body->pos.z;
            f32 dx = b->x - a->x;
            f32 dz = b->z - a->z;
            if ((z > 0.f) != (z + dz > 0.f) && x - z * dx / dz > 0.f)
                coversCenter ^= 1;
            f32 lengthSq = dx * dx + dz * dz;
            f32 t = lengthSq > .001f ? CLAMP(-(x * dx + z * dz) / lengthSq, 0.f, 1.f) : 0.f;
            x += t * dx;
            z += t * dz;
            f32 distanceSq = x * x + z * z;
            if (distanceSq < nearest)
            {
                nearest = distanceSq;
                offset = (Vec3f){ x, 0.f, z };
            }
        }
        if (coversCenter && Slime_TryGrab(slime, play, player))
            return;
        if (coversCenter && frozenFall)
        {
            Slime_FrozenImpact(slime, play);
            return;
        }
        if (coversCenter && normalX * normalX + normalZ * normalZ > .01f)
        {
            f32 length = sqrtf(normalX * normalX + normalZ * normalZ);
            Slime_ApplyPlayerPush(play, player, normalX * radius / length, normalZ * radius / length);
            return;
        }
        f32 y = (bottom + top) * .5f;
        Vec3f segment[2];
        int crossings = 0;
        for (int j = 0; j < 3; ++j)
        {
            Vec3f* a = &triangle[j];
            Vec3f* b = &triangle[(j + 1) % 3];
            if ((a->y > y) == (b->y > y))
                continue;
            f32 t = (y - a->y) / (b->y - a->y);
            segment[crossings++] = (Vec3f){ a->x + t * (b->x - a->x), y, a->z + t * (b->z - a->z) };
        }
        if (crossings == 2)
        {
            f32 z = segment[0].z - body->pos.z;
            f32 endZ = segment[1].z - body->pos.z;
            if ((z > 0.f) != (endZ > 0.f) &&
                segment[0].x - z * (segment[1].x - segment[0].x) / (endZ - z) > body->pos.x)
                inside ^= 1;
        }
    }
    if (nearest >= 1e10f)
        return;
    f32 distance = sqrtf(nearest);
    f32 overlap = radius + (inside ? distance : -distance);
    if (overlap <= 0.f || Slime_TryGrab(slime, play, player))
        return;
    if (frozenFall)
    {
        Slime_FrozenImpact(slime, play);
        return;
    }
    if (distance < .01f)
        return;
    f32 scale = (inside ? 1.f : -1.f) * overlap / distance;
    Slime_ApplyPlayerPush(play, player, offset.x * scale, offset.z * scale);
}

static void Slime_RegisterRingColliders(StandaloneSlime* slime, PlayState* play)
{
    Actor* actor = &slime->duel.actor;
    s16 minY[13], maxY[13];
    f32 radius[13];
    for (int ring = 0; ring < ARRAY_COUNT(radius); ++ring)
    {
        minY[ring] = 32767;
        maxY[ring] = -32768;
        f32 radiusSq = 0.f;
        for (int i = sSlimeRingStart[ring]; i < sSlimeRingStart[ring + 1]; ++i)
        {
            const s16* v = slime->shellMesh[i].n.ob;
            minY[ring] = MIN(minY[ring], v[1]);
            maxY[ring] = MAX(maxY[ring], v[1]);
            radiusSq = MAX(radiusSq, (f32)v[0] * v[0] + (f32)v[2] * v[2]);
        }

        radius[ring] = sqrtf(radiusSq) * slime->shellScaleXZ;
    }
    int receive =
        slime->duel.phase != SLIME_FROZEN || (slime->frozenReady && slime->frozenOnGround && !slime->frozenMelting);
    for (int i = 0; i < ARRAY_COUNT(slime->shell); ++i)
    {
        Cylinder16* dim = &slime->shell[i].dim;
        dim->pos =
            (Vec3s){ actor->world.pos.x, actor->world.pos.y + slime->shellOffsetY + minY[i + 1] * slime->shellScaleY,
                     actor->world.pos.z };
        dim->height = MAX(1, (s16)((maxY[i] - minY[i + 1]) * slime->shellScaleY));
        dim->radius = MAX(radius[i], radius[i + 1]);
        if (receive)
            Duel_AC(play, &play->colChkCtx, &slime->shell[i].base);
    }
}

static void Slime_RegisterShell(StandaloneSlime* slime, PlayState* play)
{
    StandaloneDuel* duel = &slime->duel;
    Actor* actor = &duel->actor;
    Vtx* vertices = slime->shellMesh;

    int forming = duel->phase == SLIME_FORM;
    int rising = duel->phase == SLIME_RISE;
    int grabbing = duel->phase == SLIME_GRAB;
    int restoring = duel->phase == SLIME_GROUND && slime->throwRecovery;
    int squishing = duel->phase == SLIME_GROUND && slime->squishRecovery;
    f32 restoreStep = restoring && duel->timer > 0 && duel->timer < 10 ? 1.f / duel->timer : 0.f;
    int wide = restoring || squishing;
    f32 blend = wide ? 1.f : 0.f;
    int onCeiling = duel->phase == SLIME_CEILING;
    int warning = duel->phase == SLIME_FALL && slime->dropWarningActive;
    int dropping = duel->phase == SLIME_FALL && !slime->dropWarningActive;
    int waving = onCeiling || warning || (grabbing && slime->grabRounds);
    f32 scaleX = onCeiling ? slime->riseScaleX : slime->shellScaleXZ;
    f32 scaleZRatio = onCeiling || (grabbing && slime->grabRounds) ? (.3f - scaleX) / scaleX : 1.f;
    if (duel->phase == SLIME_FALL)
        scaleZRatio = slime->dropScaleZ / slime->riseScaleX;
    f32 wave = 0.f;
    if (waving)
    {
        if (!slime->waveTimer)
        {
            slime->waveTimer = 24;
            slime->wavePhase = (int)(Rand_ZeroOne() * 6.f) >> 1;
            for (int i = 0; i < ARRAY_COUNT(slime->waveAmplitude); ++i)
            {
                slime->waveAmplitude[i] = .05f + Rand_ZeroOne() * .04f;
            }
        }
        wave = Math_SinS(--slime->waveTimer * (0x8000 / 12));
    }
    else
    {
        slime->waveTimer = 0;
    }
    int ring = 0;
    if (duel->phase == SLIME_FROZEN)
    {
        slime->frozenMinY = 32767;
        slime->frozenMaxY = -32768;
    }
    for (int i = 0; i < ARRAY_COUNT(sGekkoSlimeVertices); ++i)
    {
        if (i == sSlimeRingStart[ring + 1])
            ++ring;
        vertices[i] = sGekkoSlimeVertices[i];
        if (dropping)
        {
            const s16* rest = sGekkoSlimeVertices[i].n.ob;
            Vec3s* point = &slime->throwMesh[i];
            f32 heightScale = i > 145 ? .9f : i < 16 ? 1.3f : 1.f;
            Math_ScaledStepToS(&point->x, rest[0], 5);
            Math_ScaledStepToS(&point->y, rest[1] * heightScale, 5);
            Math_ScaledStepToS(&point->z, rest[2], 5);
            vertices[i].n.ob[0] = point->x;
            vertices[i].n.ob[1] = point->y;
            vertices[i].n.ob[2] = point->z * scaleZRatio;
        }
        else if (duel->phase == SLIME_THROW || duel->phase == SLIME_FROZEN || rising ||
                 (grabbing && !slime->grabRounds))
        {
            vertices[i].n.ob[0] = slime->throwMesh[i].x;
            vertices[i].n.ob[1] = slime->throwMesh[i].y;
            vertices[i].n.ob[2] = slime->throwMesh[i].z;
            if (duel->phase == SLIME_FROZEN)
            {
                slime->frozenMinY = MIN(slime->frozenMinY, vertices[i].n.ob[1]);
                slime->frozenMaxY = MAX(slime->frozenMaxY, vertices[i].n.ob[1]);
            }
        }
        else
        {
            f32 perturbation =
                waving ? wave * Math_SinS((i % 6 + slime->wavePhase) * (0x8000 / 3)) * slime->waveAmplitude[i / 6]
                       : 0.f;
            vertices[i].n.ob[0] *= 1.f + perturbation;
            vertices[i].n.ob[1] *= 1.f + perturbation;
            vertices[i].n.ob[2] *= (1.f + perturbation) * (warning ? 1.f : scaleZRatio);
            Slime_ConeVertex(&vertices[i], ring, 0, wide, blend);
            if (warning)
            {
                const s16* point = vertices[i].n.ob;
                slime->throwMesh[i] = (Vec3s){ point[0], point[1], point[2] };
            }
            if (warning)
                vertices[i].n.ob[2] *= scaleZRatio;
            if (restoring)
            {
                Vec3s* point = &slime->throwMesh[i];
                f32 step = restoreStep;
                point->x += (s16)((vertices[i].n.ob[0] - point->x) * step);
                point->y += (s16)((vertices[i].n.ob[1] - point->y) * step);
                point->z += (s16)((vertices[i].n.ob[2] - point->z) * step);
                vertices[i].n.ob[0] = point->x;
                vertices[i].n.ob[1] = point->y;
                vertices[i].n.ob[2] = point->z;
            }
            else if (squishing)
            {
                Vec3s* point = &slime->throwMesh[i];
                Math_SmoothStepToS(&point->x, vertices[i].n.ob[0], 5, 40, 5);
                Math_SmoothStepToS(&point->y, vertices[i].n.ob[1], 2, 600, 3);
                Math_SmoothStepToS(&point->z, vertices[i].n.ob[2], 5, 40, 5);
                vertices[i].n.ob[0] = point->x;
                vertices[i].n.ob[1] = point->y;
                vertices[i].n.ob[2] = point->z;
            }
        }
    }
    if (restoring)
    {
        f32 step = restoreStep;
        slime->shellScaleXZ += (.2f - slime->shellScaleXZ) * step;
        slime->shellScaleY += (.05f - slime->shellScaleY) * step;
        slime->shellOffsetY += (24.f - slime->shellOffsetY) * step;
    }
    else if (forming)
    {
        int live = 0;
        for (int i = 0; i < MINISLIME_COUNT; ++i)
        {
            EnemyMinislime* fragment = slime->fragments[i];
            if (fragment && fragment->actor.update)
                ++live;
        }
        int arrived = slime->formFragments ? MAX(0, slime->formFragments - live)
                                           : (10 - MIN(duel->timer, 10)) * MINISLIME_COUNT / 10;
        f32 growth = arrived && duel->timer <= 10 ? .4f + .6f * sqrtf(arrived * (2.f / 30.f)) : 0.f;
        Vec3f floor = actor->world.pos;
        if (actor->floorPoly)
            floor.y = actor->floorHeight;
        Vec3f roof = floor;
        roof.y += 160.f;
        Vec3f start = floor;
        start.y += 2.f;
        Vec3f hit;
        CollisionPoly* poly;
        s32 bgId;
        if (growth && BgCheck_EntityLineTest1(&play->colCtx, &start, &roof, &hit, &poly, 0, 0, 1, 0, &bgId))
            roof.y = hit.y - 2.f;
        slime->shellScaleXZ = .15f * growth;
        slime->shellScaleY = MIN(.079f * growth, MAX(0.f, roof.y - floor.y) * .0005f);
        slime->shellOffsetY = floor.y - actor->world.pos.y + slime->shellScaleY * 1000.f;
    }
    else if (!squishing && duel->phase != SLIME_FROZEN && duel->phase != SLIME_THROW && !grabbing)
    {
        int ceiling = duel->phase == SLIME_CEILING || duel->phase == SLIME_FALL;
        slime->shellScaleXZ = rising || ceiling ? slime->riseScaleX : .085f;
        slime->shellScaleY = rising || ceiling ? Slime_RiseHeightScale(slime) : .085f;
        slime->shellOffsetY = rising || ceiling ? 0.f : 24.f;
    }
    f32 sizeXZ = slime->shellScaleXZ;
    f32 sizeY = slime->shellScaleY;
    f32 height = slime->shellOffsetY;
    int clipWorld = (duel->phase == SLIME_GROUND && !squishing) ||
                    (duel->phase == SLIME_FROZEN && slime->frozenOnGround);
    int clipped = clipWorld ? Slime_ClipWallVertices(play, actor, vertices, height, sizeXZ, sizeY) : 0;
    int fallNearFloor = dropping && actor->floorPoly &&
        actor->world.pos.y - 1000.f * sizeY <= actor->floorHeight + 8.f;
    if (sizeY > .001f &&
        (rising || duel->phase == SLIME_CEILING || warning || fallNearFloor ||
         duel->phase == SLIME_FROZEN))
    {
        f32 originY = actor->world.pos.y + height;
        f32 upper = (slime->ceiling - originY) / sizeY;
        f32 lower = actor->floorPoly ? (actor->floorHeight - originY) / sizeY : -32768.f;
        for (int i = 0; i < ARRAY_COUNT(sGekkoSlimeVertices); ++i)
        {
            s16* y = &vertices[i].n.ob[1];
            s16 bounded = CLAMP(CLAMP(*y, MIN(lower, upper), upper), -32768.f, 32767.f);
            clipped |= bounded != *y;
            *y = bounded;
        }
    }
    int fastShell = dropping || warning || grabbing || squishing || duel->phase == SLIME_THROW;
    if (!fastShell &&
        (clipped || waving || rising || wide || duel->phase == SLIME_FROZEN))
        Slime_UpdateMeshNormals(vertices);
    if (forming)
        return;
    Slime_RegisterRingColliders(slime, play);
    if (rising || duel->phase == SLIME_CEILING || duel->phase == SLIME_FALL ||
        grabbing || duel->phase == SLIME_THROW)
        return;
    Player* nearbyPlayer = GET_PLAYER(play);
    if (nearbyPlayer && !nearbyPlayer->actor.parent && nearbyPlayer->cylinder.dim.radius > 0 &&
        actor->xzDistToPlayer <= MAX(200.f, sizeXZ * 1000.f + nearbyPlayer->cylinder.dim.radius))
    {
        Vec3f world[ARRAY_COUNT(sGekkoSlimeVertices)];
        for (int i = 0; i < ARRAY_COUNT(world); ++i)
        {
            world[i] = (Vec3f){
                actor->world.pos.x + vertices[i].n.ob[0] * sizeXZ,
                actor->world.pos.y + height + vertices[i].n.ob[1] * sizeY,
                actor->world.pos.z + vertices[i].n.ob[2] * sizeXZ,
            };
        }
        Slime_PushPlayer(slime, play, world);
    }
    if (duel->phase == SLIME_GROUND && slime->squishRecovery && !duel->timer)
    {
        slime->squishRecovery = 0;
        Slime_BeginRise(slime);
    }
}

static void Slime_CancelFragmentThrow(StandaloneSlime* slime)
{
    if (!slime->throwFragment)
        return;
    EnemyMinislime* fragment = slime->fragments[slime->throwFragment - 1];
    if (fragment && fragment->actor.update && fragment->mode == MINISLIME_TO_GEKKO && !fragment->launch)
    {
        fragment->mode = MINISLIME_ACTIVE;
        fragment->bounce = fragment->bounceWindup = 0;
        fragment->actor.velocity = (Vec3f){ 0.f, 0.f, 0.f };
        fragment->actor.bgCheckFlags &= ~BGCHECKFLAG_GROUND;
    }
    slime->throwFragment = 0;
}

static void Slime_ThrowFragment(StandaloneSlime* slime, int finished)
{
    StandaloneDuel* duel = &slime->duel;
    Actor* actor = &duel->actor;
    actor->speed = 0.f;
    Math_ScaledStepToS(&actor->shape.rot.y, actor->yawTowardsPlayer, 0x300);
    EnemyMinislime* fragment = slime->throwFragment ? slime->fragments[slime->throwFragment - 1] : NULL;
    if (fragment && fragment->mode == MINISLIME_TO_GEKKO && Animation_OnFrame(&duel->skel, 27.f))
    {
        Duel_Sfx(actor, 0x395e);
        fragment->launch = 1;
    }
    if (finished)
    {
        Slime_CancelFragmentThrow(slime);
        duel->phase = DUEL_APPROACH;
        duel->walking = 1;
        actor->world.rot.y = actor->shape.rot.y;
        slime->groundYaw = actor->yawTowardsPlayer + 0x8000;
        Animation_PlayLoop(&duel->skel, Duel_Anim(0x39c4));
    }
}

static void Slime_GroundMovement(StandaloneSlime* slime, PlayState* play, int finished)
{
    StandaloneDuel* duel = &slime->duel;
    Actor* actor = &duel->actor;
    actor->speed = 0.f;
    if (duel->phase == SLIME_FRAGMENT_LOOK)
    {
        if (finished)
            Animation_PlayOnce(&duel->skel, Duel_Anim(Rand_ZeroOne() < .25f ? 0x66b4 : 0x69fc));
        if (duel->skel.animation == Duel_Anim(0x69fc) &&
            Math_ScaledStepToS(&actor->shape.rot.y, slime->groundYaw, 0x400))
        {
            s16 homeYaw = Math_Vec3f_Yaw(&actor->world.pos, &actor->home.pos);
            s16 turn = 0x2000 + (Rand_Next() >> 20);
            slime->groundYaw = actor->shape.rot.y + ((s16)(homeYaw - actor->shape.rot.y) > 0 ? turn : -turn);
        }
        s16 playerAngle = actor->yawTowardsPlayer - actor->shape.rot.y;
        if (!duel->timer || (ABS(playerAngle) < 0x2800 && actor->xzDistToPlayer < 240.f))
        {
            duel->phase = SLIME_FRAGMENT_NOTICE;
            slime->groundYaw = actor->shape.rot.y + 0x8000;
            Animation_PlayOnce(&duel->skel, Duel_Anim(0x10f4));
            Duel_Sfx(actor, 0x395f);
        }
    }
    else if (duel->phase == SLIME_FRAGMENT_NOTICE)
    {
        if (duel->skel.curFrame > 10.f)
            Math_ScaledStepToS(&actor->shape.rot.y, slime->groundYaw, 0x800);
        if (finished)
        {
            duel->phase = DUEL_APPROACH;
            duel->walking = 1;
            actor->world.rot.y = actor->shape.rot.y;
            slime->groundYaw = actor->yawTowardsPlayer + 0x8000;
            slime->cycle = 100;
            Animation_PlayLoop(&duel->skel, Duel_Anim(0x39c4));
        }
    }
    else if (duel->phase == DUEL_APPROACH)
    {
        if (slime->waitingForShatterLanding)
        {
            if (!(actor->bgCheckFlags & BGCHECKFLAG_GROUND))
                return;
            slime->waitingForShatterLanding = 0;
            actor->flags |= ACTOR_FLAG_ATTENTION_ENABLED;
        }
        if (!slime->cycle && slime->jumpRounds)
        {
            --slime->jumpRounds;
            slime->cycle = 100;
            duel->phase = SLIME_FRAGMENT_LOOK;
            duel->timer = 60;
            s16 homeYaw = Math_Vec3f_Yaw(&actor->world.pos, &actor->home.pos);
            s16 turn = 0x2000 + (Rand_Next() >> 20);
            slime->groundYaw = actor->shape.rot.y + ((s16)(homeYaw - actor->shape.rot.y) > 0 ? turn : -turn);
            Animation_PlayOnce(&duel->skel, Duel_Anim(0x69fc));
            return;
        }
        if (!duel->walking)
        {
            Animation_PlayLoop(&duel->skel, Duel_Anim(0x39c4));
            duel->walking = 1;
        }
        if (!Math_SmoothStepToS(&actor->world.rot.y, slime->groundYaw, 5, 0x1000, 0x80))
        {
            s16 away = actor->yawTowardsPlayer + 0x8000;
            s16 homeYaw = Math_Vec3f_Yaw(&actor->world.pos, &actor->home.pos);
            s16 difference = homeYaw - away;
            f32 homeDx = actor->world.pos.x - actor->home.pos.x;
            f32 homeDz = actor->world.pos.z - actor->home.pos.z;
            slime->groundYaw = SQ(homeDx) + SQ(homeDz) > SQ(240.f)
                                   ? (ABS(difference) < 0x3000 ? homeYaw : away + difference / 2)
                                   : away;
        }
        actor->shape.rot.y = actor->world.rot.y;
        if (Animation_OnFrame(&duel->skel, 1.f))
            Duel_Sfx(actor, 0x395b);
        else if (Animation_OnFrame(&duel->skel, 11.f))
            Duel_Sfx(actor, 0x2902);
        if (!(actor->bgCheckFlags & BGCHECKFLAG_GROUND) || (duel->skel.curFrame > 1.f && duel->skel.curFrame < 12.f))
        {
            Vec3f from = actor->world.pos;
            from.y += 24.f;
            Vec3f next = from;
            next.x += 16.f * Math_SinS(actor->world.rot.y);
            next.z += 16.f * Math_CosS(actor->world.rot.y);
            CollisionPoly* floor = NULL;
            s32 bgId;
            f32 height = BgCheck_EntityRaycastDown4(&play->colCtx, &floor, &bgId, actor, &next);
            if (floor && height >= actor->floorHeight - 24.f && enemyFlightClear(play, &from, &next))
            {
                actor->speed = 8.f;
            }
            else
            {
                slime->groundYaw = actor->world.rot.y + 0x4000;
            }
        }
    }
    else
    {
        Gekko_GroundCombat(duel, play, finished);
    }
}

static void Slime_PropagateFreeze(StandaloneSlime* slime)
{
    StandaloneDuel* duel = &slime->duel;
    Actor* actor = &duel->actor;
    int start = duel->timer * 4;
    f32 sizeXZ = MAX(.001f, slime->shellScaleXZ);
    f32 sizeY = MAX(.001f, slime->shellScaleY);
    for (int i = start; i < start + 4 && i < ARRAY_COUNT(slime->throwMesh); ++i)
    {
        Vec3s* vertex = &slime->throwMesh[i];
        f32 displacement = (Rand_ZeroOne() - .5f) * 40.f;
        vertex->x += (s16)(displacement / sizeXZ);
        vertex->z += (s16)(displacement / sizeXZ);
        f32 height = vertex->y + (s16)(displacement / sizeY);
        if (actor->floorPoly)
            height = MAX(height, (actor->floorHeight - actor->world.pos.y - slime->shellOffsetY) / sizeY);
        vertex->y = (s16)height;
    }
    for (int i = 0; i < ARRAY_COUNT(slime->iceAlpha) - 4; ++i)
        slime->iceAlpha[i] = slime->iceAlpha[i + 4];
}

static void Slime_SpawnMeltSmoke(StandaloneSlime* slime, PlayState* play)
{
    int vertex = Rand_ZeroOne() * SLIME_VERTEX_COUNT;
    Vec3f pos = Slime_VertexPosition(slime, MIN(vertex, SLIME_VERTEX_COUNT - 1));
#if defined(GAME_OOT)
    for (int i = 0; i < ARRAY_COUNT(slime->meltSmoke); ++i)
    {
        if (slime->meltSmoke[i].life)
            continue;
        slime->meltSmoke[i].pos = pos;
        slime->meltSmoke[i].life = 50;
        break;
    }
#else
    Vec3f velocity = { 0.f, 2.f, 0.f };
    Vec3f accel = { 0.f, 0.f, 0.f };
    EffectSsIceSmoke_Spawn(play, &pos, &velocity, &accel, 600);
#endif
}

static void Slime_ShakeFrozenGround(StandaloneSlime* slime, PlayState* play)
{
    StandaloneDuel* duel = &slime->duel;
    Actor* actor = &duel->actor;
    if (duel->timer == 40)
    {
        slime->freezeAnchor = actor->world.pos;
    }
    else if (duel->timer && (duel->timer < 20 || (duel->timer < 40 && (duel->timer & 1))))
    {
        f32 inverse = 1.f / duel->timer;
        Vec3f next = slime->freezeAnchor;
        f32 xOffset = 1.f + inverse + Rand_ZeroOne() * (4.f * inverse);
        next.x += Rand_ZeroOne() < .5f ? -xOffset : xOffset;
        f32 zOffset = 1.f + inverse + Rand_ZeroOne() * (4.f * inverse);
        next.z += Rand_ZeroOne() < .5f ? -zOffset : zOffset;
        if (enemyFlightClear(play, &actor->world.pos, &next))
            actor->world.pos = next;
    }
}

static void Slime_MoveUnderCeiling(StandaloneSlime* slime, PlayState* play, f32 speed)
{
    Actor* actor = &slime->duel.actor;
    (void)play;
    actor->world.pos.x += speed * Math_SinS(actor->world.rot.y);
    actor->world.pos.z += speed * Math_CosS(actor->world.rot.y);
}

static void Slime_StartDefeat(StandaloneSlime* slime, PlayState* play)
{
    StandaloneDuel* duel = &slime->duel;
    Actor* actor = &duel->actor;
    Gekko_Thaw(duel, play);
    Igos_ReleasePlayer(actor, play);
    actor->flags &= ~(SLIME_HOOK_PULLS_ACTOR | SLIME_HOOK_ATTACHED);
    duel->phase = DUEL_DEFEAT;
    duel->timer = 40;
    actor->flags &= ~(ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE);
    actor->flags |= ACTOR_FLAG_LOCK_ON_DISABLED;
    actor->speed = 0.f;
    actor->gravity = -2.f;
    actor->velocity.y = 0.f;
    actor->world.rot.y = slime->damage.recoilYaw;
    AnimationHeader* anim = Duel_Anim(0x276c);
    Animation_Change(&duel->skel, anim, .5f, 0.f, Animation_GetLastFrame(anim), ANIMMODE_ONCE_INTERP, 0.f);
    for (int i = 0; i < MINISLIME_COUNT; ++i)
    {
        EnemyMinislime* fragment = slime->fragments[i];
        if (fragment && fragment->actor.update)
            fragment->mode = MINISLIME_DEFEAT_IDLE;
    }
    Enemy_StartFinishingBlow(play, actor);
    Duel_Sfx(actor, 0x38d6);
}

static void Slime_Defeat(StandaloneSlime* slime, PlayState* play)
{
    StandaloneDuel* duel = &slime->duel;
    Actor* actor = &duel->actor;
    if (duel->timer > 20)
    {
        f32 step = (duel->timer - 20) * .5f;
        Vec3f from = actor->world.pos;
        from.y += 20.f;
        Vec3f next = from;
        next.x += step * Math_SinS(slime->damage.recoilYaw);
        next.z += step * Math_CosS(slime->damage.recoilYaw);
        CollisionPoly* floor = NULL;
        s32 bgId;
        f32 height = BgCheck_EntityRaycastDown4(&play->colCtx, &floor, &bgId, actor, &next);
        if (floor && fabsf(height - actor->world.pos.y) < 25.f && enemyFlightClear(play, &from, &next))
        {
            actor->world.pos.x = next.x;
            actor->world.pos.z = next.z;
        }
        actor->shape.rot.y += 0x3000;
    }
    else if (duel->timer)
    {
        Actor_SetScale(actor, duel->timer * .00035f);
    }
    else
    {
        Gekko_DefeatEffects(actor, play);
        Actor_Kill(actor);
        return;
    }
    Actor_MoveWithGravity(actor);
    Actor_UpdateBgCheckInfo(play, actor, 24.f, 32.f, 65.f, 5);
}

static void Slime_Combat(StandaloneSlime* slime, PlayState* play, int finished)
{
    StandaloneDuel* duel = &slime->duel;
    Actor* actor = &duel->actor;
    Player* player = GET_PLAYER(play);
    if (duel->phase < SLIME_FORM && (actor->flags & SLIME_HOOK_ATTACHED))
    {
        if (enemyFlightClear(play, &slime->hookSafePos, &actor->world.pos))
        {
            duel->exposed = 2;
            actor->speed = actor->velocity.y = 0.f;
            return;
        }
        actor->world.pos = slime->hookSafePos;
        actor->flags &= ~SLIME_HOOK_ATTACHED;
    }
    if (Gekko_Recoil(duel, play))
        return;
    if (duel->phase < SLIME_FORM)
    {
        if (slime->cycle)
        {
            int wasThrowing = duel->phase == SLIME_FRAGMENT_THROW;
            if (wasThrowing)
            {
                Slime_ThrowFragment(slime, finished);
            }
            else
            {
                slime->throwFragment = 0;
                if (duel->phase == DUEL_APPROACH && !slime->waitingForShatterLanding)
                    --slime->cycle;
            }
            duel->exposed = 2;
            int regrouping = !slime->jumpRounds && slime->cycle < 30 && duel->phase != SLIME_FRAGMENT_LOOK &&
                             duel->phase != SLIME_FRAGMENT_NOTICE;
            for (int i = 0; i < MINISLIME_COUNT; ++i)
            {
                EnemyMinislime* fragment = slime->fragments[i];
                if (!fragment || !fragment->actor.update || slime->throwFragment == i + 1)
                    continue;
                if (regrouping)
                {
                    fragment->mode = MINISLIME_REGROUP;
                }
                else if (!wasThrowing && slime->cycle && duel->phase == DUEL_APPROACH &&
                         actor->xzDistToPlayer > 300.f && fragment->mode == MINISLIME_ACTIVE &&
                         (fragment->actor.bgCheckFlags & BGCHECKFLAG_GROUND) && !fragment->bounce &&
                         !fragment->bounceWindup && !fragment->throwTimer && !fragment->freeze && !fragment->melt &&
                         !fragment->knockback && (duel->body.base.ocFlags1 & OC1_HIT) &&
                         duel->body.base.oc == &fragment->actor &&
                         enemyFlightClear(play, &actor->focus.pos, &fragment->actor.world.pos))
                {
                    slime->throwFragment = i + 1;
                    fragment->mode = MINISLIME_TO_GEKKO;
                    duel->phase = SLIME_FRAGMENT_THROW;
                    actor->speed = 0.f;
                    Animation_PlayOnce(&duel->skel, Duel_Anim(0xf048));
                    Duel_Sfx(actor, 0x395d);
                }
            }
            if (!wasThrowing && duel->phase != SLIME_FRAGMENT_THROW)
                Slime_GroundMovement(slime, play, finished);
        }
        else
        {
            slime->formFragments = 0;
            for (int i = 0; i < MINISLIME_COUNT; ++i)
            {
                EnemyMinislime* fragment = slime->fragments[i];
                if (fragment && fragment->actor.update)
                {
                    fragment->mode = MINISLIME_REGROUP;
                    ++slime->formFragments;
                }
            }
            duel->phase = SLIME_FORM;
            duel->timer = 28;
            duel->exposed = 0;
            actor->speed = 0.f;
            actor->flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
            Animation_PlayOnce(&duel->skel, Duel_Anim(0x2e0c));
        }
        return;
    }

    actor->speed = 0.f;
    actor->gravity = 0.f;
    if (duel->phase == SLIME_FORM)
    {
        actor->velocity.y = 0.f;
        if (duel->timer == 27)
            Duel_Sfx(actor, 0x3938);
        if (duel->timer)
            return;
        Vec3f base = actor->world.pos;
        base.y += 12.f;
        CollisionPoly* floor = NULL;
        s32 bgId;
        f32 floorHeight = BgCheck_EntityRaycastDown4(&play->colCtx, &floor, &bgId, actor, &base);
        if (floor && floorHeight <= actor->world.pos.y + 8.f)
            base.y = floorHeight + 2.f;
        Vec3f target = base;
        target.y = floorHeight + SLIME_OPEN_AIR_HEIGHT;
        Vec3f hit;
        CollisionPoly* roof;
        slime->ceiling = BgCheck_EntityLineTest1(&play->colCtx, &base, &target, &hit, &roof, 0, 0, 1, 0, &bgId)
                             ? hit.y - 2.f
                             : target.y;
        if (!floor || floorHeight > actor->world.pos.y + 8.f || slime->ceiling - floorHeight < 48.f ||
            !enemyFlightClear(play, &actor->world.pos, &base))
        {
            duel->phase = DUEL_APPROACH;
            duel->walking = 0;
            duel->exposed = 2;
            actor->flags |= ACTOR_FLAG_ATTENTION_ENABLED;
            slime->cycle = 180;
            for (int i = 0; i < MINISLIME_COUNT; ++i)
            {
                EnemyMinislime* fragment = slime->fragments[i];
                if (fragment && fragment->actor.update)
                {
                    fragment->mode = MINISLIME_ACTIVE;
                    fragment->launch = fragment->bounce = fragment->bounceWindup = 0;
                    fragment->actor.velocity = (Vec3f){ 0.f, 0.f, 0.f };
                    fragment->actor.bgCheckFlags &= ~BGCHECKFLAG_GROUND;
                }
            }
            actor->gravity = floor ? -2.f : 0.f;
            Animation_PlayLoop(&duel->skel, Duel_Anim(0xdf98));
            return;
        }
        Slime_ClearFragments(slime);
        actor->floorHeight = floorHeight;
        actor->floorPoly = floor;
        actor->world.pos.y = floorHeight + 2.f;
        Slime_BeginRise(slime);
    }
    else if (duel->phase == SLIME_RISE)
    {
        Slime_Rise(slime);
    }
    else if (duel->phase == SLIME_CEILING)
    {
        Math_ScaledStepToS(&slime->shellGekkoPitch, 0, 0x400);
        if (slime->ceilingMoveTimer)
            --slime->ceilingMoveTimer;
        s16 phase = slime->ceilingMoveTimer * 0x800;
        slime->riseScaleX = .15f + .004f * Math_SinS(phase);
        slime->riseScaleY = .075f + .004f * Math_CosS(phase);
        int canDropNearPlayer = 0;
        if (actor->floorPoly && actor->xzDistToPlayer < 250.f)
        {
            Vec3f from = { actor->world.pos.x, actor->floorHeight + 24.f, actor->world.pos.z };
            Vec3f target = player->actor.world.pos;
            if (fabsf(target.y - actor->floorHeight) < 64.f)
            {
                target.y += 24.f;
                canDropNearPlayer = enemyFlightClear(play, &from, &target);
            }
        }
        if (!slime->ceilingMoveTimer || canDropNearPlayer)
        {
            duel->phase = SLIME_FALL;
            duel->timer = 30;
            slime->dropWarningActive = 1;
            slime->dropScaleZ = .3f - slime->riseScaleX;
            actor->velocity.y = 0.f;
        }
        else
        {
            f32 speed = 5.f + 3.f * fabsf(Math_SinS(phase));
            Math_SmoothStepToS(&actor->world.rot.y, actor->yawTowardsPlayer, 10, 0x800, 0x80);
            actor->shape.rot.y = actor->world.rot.y;
            Slime_MoveUnderCeiling(slime, play, speed);
        }
    }
    else if (duel->phase == SLIME_FALL || duel->phase == SLIME_FROZEN)
    {
        if (duel->phase == SLIME_FROZEN)
        {
            if (!slime->frozenReady)
            {
                if (duel->timer == 40)
                    return;
                Duel_Sfx(actor, NA_SE_EV_ICE_FREEZE - SFX_FLAG);
                duel->skel.playSpeed = duel->timer * (1.f / 40.f);
                Slime_PropagateFreeze(slime);
                if (actor->bgCheckFlags & BGCHECKFLAG_GROUND_TOUCH)
                {
                    if (!duel->timer)
                    {
                        Slime_Shatter(slime, play);
                    }
                    else
                    {
                        Slime_StartShatterEffects(slime, 0);
                        duel->skel.playSpeed = 1.f;
                        Slime_StartSquish(slime);
                    }
                    return;
                }
                if (duel->timer)
                    return;
                slime->frozenReady = 1;
                slime->frozenOnGround |= !!(actor->bgCheckFlags & BGCHECKFLAG_GROUND);
                if (slime->frozenOnGround)
                {
                    duel->timer = 80;
                }
                else
                {
                    actor->gravity = -2.f;
                    actor->velocity.y = 0.f;
                }
                return;
            }
            if (slime->frozenOnGround)
            {
                if (slime->frozenMelting)
                {
                    int elapsed = 100 - duel->timer;
                    if (elapsed > 0)
                    {
                        if (elapsed < 70 && (elapsed & 1))
                            Slime_SpawnMeltSmoke(slime, play);
                        Duel_Sfx(actor, SLIME_ICE_MELT_SFX - SFX_FLAG);
                        for (int i = ARRAY_COUNT(slime->iceAlpha) - 1; i >= 2; --i)
                            slime->iceAlpha[i] = slime->iceAlpha[i - 2];
                    }
                }
                else
                {
                    Slime_ShakeFrozenGround(slime, play);
                }
                if (!duel->timer)
                {
                    if (!slime->frozenMelting)
                        Slime_StartShatterEffects(slime, 0);
                    duel->phase = slime->preFreezePhase;
                    duel->timer = slime->preFreezeTimer;
                    duel->skel.playSpeed = 1.f;
                }
                return;
            }
        }
        if (duel->phase == SLIME_FALL)
        {
            Math_ScaledStepToS(&slime->shellGekkoPitch, 0x4000, 0x400);
            if (slime->dropWarningActive)
            {
                s16 phase = (slime->ceilingMoveTimer - (30 - duel->timer)) * 0x4000;
                slime->riseScaleX = .15f + .02f * Math_SinS(phase);
                slime->dropScaleZ = slime->riseScaleX;
                slime->riseScaleY = .075f + .015f * Math_CosS(phase);
                if (duel->timer)
                    return;
                actor->gravity = -2.f;
                Duel_Sfx(actor, 0x3964);
                slime->dropWarningActive = 2;
            }
            else
            {
                Math_StepToF(&slime->riseScaleX, .15f, .0025f);
                Math_StepToF(&slime->riseScaleY, .08f, .0025f);
                Math_StepToF(&slime->dropScaleZ, .15f, .0025f);
            }
        }
        actor->gravity = -2.f;
        if (!(actor->bgCheckFlags & BGCHECKFLAG_GROUND))
            return;
        if (duel->phase == SLIME_FROZEN)
            Slime_Shatter(slime, play);
        else
            Slime_StartSquish(slime);
    }
    else if (duel->phase == SLIME_GRAB || duel->phase == SLIME_THROW)
    {
        Slime_GrabCombat(slime, play, player, finished);
    }
    else if (duel->phase == SLIME_GROUND)
    {
        if (slime->squishRecovery)
        {
            Math_ScaledStepToS(&slime->shellGekkoPitch, 0, 0x400);
            Math_SmoothStepToF(&slime->shellScaleXZ, .35f, .4f, .035f, .0035f);
            Math_SmoothStepToF(&slime->shellScaleY, .03f, .4f, .003f, .0003f);
        }
        if (!duel->timer && !slime->squishRecovery)
        {
            slime->throwRecovery = 0;
            Slime_BeginRise(slime);
        }
    }
}

static int Gomess_ProjectileApproaching(Actor* actor, PlayState* play)
{
#if defined(GAME_OOT)
    Actor* projectile = Duel_FindOotProjectile(play, actor, 80.f);
#else
    Actor* projectile = func_800BC270(play, actor, 80.f, 0x138b0);
#endif
    if (!projectile)
        return 0;
    s16 yaw = Math_Vec3f_Yaw(&actor->world.pos, &projectile->world.pos) - actor->shape.rot.y;
    if (yaw <= -0x2000 || yaw >= 0x2000)
        return 0;
    s16 pitch = Duel_Pitch(&projectile->world.pos, &actor->focus.pos) - projectile->world.rot.x;
    if (pitch <= -0x3000 || pitch >= 0x3000)
        return 0;
    return 1;
}

static void Gomess_UpdateBats(StandaloneGomess* gomess, PlayState* play)
{
    StandaloneDuel* duel = &gomess->duel;
    Actor* parent = &duel->actor;
    if (!gomess->batsInitialized)
    {
        gomess->batsInitialized = 1;
        for (int i = GOMESS_BAT_COUNT - 1; i >= 0; --i)
        {
            s16 yaw = i * 0x3a00;
            Vec3f pos = parent->world.pos;
            pos.y += 15.f + 80.f * i / GOMESS_BAT_COUNT;
            Actor* actor =
                Actor_Spawn(&play->actorCtx, play, ACTOR_ENEMY_GOMESS_BAT, pos.x, pos.y, pos.z, 0, yaw, 0, i);
            if (!actor)
                continue;
            if (actor->id != ACTOR_ENEMY_GOMESS_BAT || actor->category != ACTORCAT_PROP)
            {
                Actor_Kill(actor);
                continue;
            }
            actor->parent = parent;
            gomess->bats[i] = (EnemyGomessBat*)actor;
            gomess->bats[i]->ownerSlot = &gomess->bats[i];
            gomess->bats[i]->swarmHits = &gomess->swarmHits;
            gomess->bats[i]->scatterRequest = &gomess->batScatterRequest;
            gomess->bats[i]->dropCooldown = &gomess->batDropCooldown;
            gomess->bats[i]->deathSfxFrame = &gomess->batDeathSfxFrame;
            gomess->bats[i]->loopSfxFrame = &gomess->batLoopSfxFrame;
            if (i + 1 < GOMESS_BAT_COUNT && (i + 1) % (GOMESS_BAT_COUNT / 2))
                gomess->bats[i]->followerSlot = &gomess->bats[i + 1];
        }
    }

    u8 batScatter = gomess->batScatterRequest;
    gomess->batScatterRequest = 0;
    u8 scatter = MAX(gomess->scatterRequest, batScatter);
    gomess->scatterRequest = 0;
    if (batScatter)
        gomess->lightArrowFillTimer = batScatter == 5 ? 5 : 20;
    if (scatter)
    {
        for (int i = 0; i < GOMESS_BAT_COUNT; ++i)
        {
            EnemyGomessBat* bat = gomess->bats[i];
            if (!bat)
                continue;
            bat->scatterTimer = scatter + 1;
            bat->crowdState = 0;
            bat->gather = 0;
            bat->recallDistance = 30;
            bat->scatterYaw = Math_Vec3f_Yaw(&parent->world.pos, &bat->actor.world.pos);
            Vec3f* pos = &bat->actor.world.pos;
            f32 dx = pos->x - parent->world.pos.x;
            f32 dz = pos->z - parent->world.pos.z;
            bat->scatterPitch = -gomessBatAtan2S(pos->y - parent->world.pos.y, sqrtf(dx * dx + dz * dz));
            bat->attackTimer = bat->leaderTimer = bat->following = bat->regrouping = 0;
            bat->returning = 1;
            bat->returnRoute = (EnemyFlightReturn){ 0 };
            bat->speed = 8.f;
        }
    }
    else if (gomess->swarmHits >= 5)
    {
        gomess->swarmHits = 0;
        for (int i = 0; i < GOMESS_BAT_COUNT; ++i)
        {
            EnemyGomessBat* bat = gomess->bats[i];
            if (!bat)
                continue;
            bat->attackTimer = bat->leaderTimer = 0;
            if (bat->following)
                bat->returning = 1;
        }
    }
}

static void Gomess_UpdateSpinTris(StandaloneGomess* gomess)
{
    Actor* actor = &gomess->duel.actor;
    f32 sin = Math_SinS(actor->shape.rot.y);
    f32 cos = Math_CosS(actor->shape.rot.y);
    Vec3f pos = actor->world.pos;
    Vec3f a = { pos.x - 70.f * cos + 75.f * sin, pos.y - 10.f, pos.z + 70.f * sin + 75.f * cos };
    Vec3f b = { pos.x + 70.f * cos + 65.f * sin, pos.y + 140.f, pos.z - 70.f * sin + 65.f * cos };
    Vec3f c = { a.x - 10.f * sin, b.y, a.z - 10.f * cos };
    Vec3f d = { b.x + 10.f * sin, a.y, b.z + 10.f * cos };
    Collider_SetTrisVertices(&gomess->spin, 0, &a, &b, &c);
    Collider_SetTrisVertices(&gomess->spin, 1, &a, &d, &b);
}

enum {
    GOMESS_DEATH_WINDUP,
    GOMESS_DEATH_DROP_SCYTHE,
    GOMESS_DEATH_DISSOLVE,
    GOMESS_DEATH_DROP_CORE,
    GOMESS_DEATH_SHATTER,
    GOMESS_DEATH_FADE
};

static void Gomess_Sparkle(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* prim,
                           Color_RGBA8* env)
{
#if defined(GAME_MM)
    ((void (*)(PlayState*, Vec3f*, Vec3f*, Vec3f*, Color_RGBA8*, Color_RGBA8*))0x800b16b8)(play, pos, velocity, accel,
                                                                                           prim, env);
#else
    EffectSsKiraKira_SpawnSmall(play, pos, velocity, accel, prim, env);
#endif
}

static int Gomess_FallPoint(StandaloneGomess* gomess, PlayState* play, Vec3f* pos, f32* velocity, f32 gravity)
{
    Vec3f next = *pos;
    next.y += *velocity;
    *velocity -= gravity;
    Vec3f hit;
    CollisionPoly* poly;
    s32 bgId;
    if (BgCheck_EntityLineTest1(&play->colCtx, pos, &next, &hit, &poly, 0, 1, 0, 0, &bgId))
    {
        *pos = hit;
        *velocity = 0.f;
        return 1;
    }
    if (next.y < gomess->duel.actor.home.pos.y - 250.f)
        return 1;
    *pos = next;
    return 0;
}

static void Gomess_UpdateLooseCore(StandaloneGomess* gomess, PlayState* play)
{
    if (!gomess->looseCore)
        return;
    gomess->looseCoreRotation += 0x1800;
    if (Gomess_FallPoint(gomess, play, &gomess->looseCorePos, &gomess->looseCoreVelocity, 1.f))
    {
        gomess->looseCore = 0;
        Gomess_CoreSound(play, &gomess->looseCorePos, 11, NA_SE_EN_EXTINCT);
        Vec3f zero = { 0.f, 0.f, 0.f };
#if defined(GAME_MM)
        func_800B3030(play, &gomess->looseCorePos, &zero, &zero, 100, 0, 0);
#else

        EffectSsDeadDb_Spawn(play, &gomess->looseCorePos, &zero, &zero, 100, 0, 255, 255, 255, 255, 255, 0, 0, 0, 9, 0);
#endif
    }
}

static int Gomess_ReceiveHit(StandaloneGomess* gomess, PlayState* play)
{
    StandaloneDuel* duel = &gomess->duel;
    Actor* actor = &duel->actor;
    u8 effect = actor->colChkInfo.damageEffect;
    if (Gomess_CrowdCount(gomess) >= 5)
    {
        gomess->looseCore = 1;
        Vec3s* center = &gomess->coreElement.dim.worldSphere.center;
        gomess->looseCorePos = (Vec3f){ center->x, center->y, center->z };
        gomess->looseCoreVelocity = -1.f;
        gomess->looseCoreRotation = actor->shape.rot.y;
        Gomess_CoreSound(play, &gomess->looseCorePos, 30, NA_SE_EN_FFLY_DEAD);
        if (effect == GOMESS_DAMAGE_LIGHT && !(duel->phase == GOMESS_SWARM && !gomess->swarmLaunched))
        {
            gomess->scatterRequest = 200;
            gomess->lightArrowFillTimer = 20;
            if (duel->phase == DUEL_APPROACH)
                duel->timer = 100;
        }
        else if (effect == GOMESS_DAMAGE_EXPLOSIVE)
        {
            gomess->explosiveCooldown = 10;
        }
        return 0;
    }
    if (effect == GOMESS_DAMAGE_EXPLOSIVE && gomess->explosiveCooldown)
        return 0;
    if (effect == GOMESS_DAMAGE_LIGHT)
    {
        gomess->lightOrbTimer = GOMESS_LIGHT_ORB_FRAMES;
        Vec3s hit = gomess->coreElement.base.acDmgInfo.hitPos;
        Actor* rays = Actor_Spawn(&play->actorCtx, play, ACTOR_EN_CLEAR_TAG, hit.x, hit.y, hit.z, 0, 0, 0, 4);
        if (!rays || !rays->update)
        {
            Vec3f pos = { hit.x, hit.y, hit.z };
            Vec3f zero = { 0.f, 0.f, 0.f };
            Color_RGBA8 prim = { 255, 245, 180, 255 };
            Color_RGBA8 env = { 255, 160, 30, 0 };
            Gomess_Sparkle(play, &pos, &zero, &zero, &prim, &env);
        }
    }
    return 1;
}

typedef struct {
    Actor actor;
    s16 radius;
    u16 timer;
    u8 alpha;
    Vec3f firedPos;
    f32 height;
    f32 screenFillIntensity;
    ActorFunc action;
} GomessLightArrow;
#if defined(GAME_OOT)
_Static_assert(__builtin_offsetof(GomessLightArrow, screenFillIntensity) == 0x154, "OoT retail Light Arrow intensity");
_Static_assert(sizeof(GomessLightArrow) == 0x15c, "OoT retail Light Arrow instance");
#else
_Static_assert(__builtin_offsetof(GomessLightArrow, screenFillIntensity) == 0x15c, "MM Light Arrow intensity");
_Static_assert(sizeof(GomessLightArrow) == 0x164, "MM Light Arrow instance");
#endif

static void Gomess_UpdateHitEffects(StandaloneGomess* gomess, PlayState* play)
{
    if (gomess->explosiveCooldown)
        --gomess->explosiveCooldown;
    if (gomess->lightOrbTimer)
        --gomess->lightOrbTimer;
    if (gomess->lightArrowFillTimer)
    {
        --gomess->lightArrowFillTimer;
        for (Actor* arrow = play->actorCtx.actors[ACTORCAT_ITEMACTION].first; arrow; arrow = arrow->next)
        {
            if (arrow->id == ACTOR_ARROW_LIGHT && arrow->update)
            {
                ((GomessLightArrow*)arrow)->screenFillIntensity = -100.f;
                break;
            }
        }
    }
}

static void Gomess_RegisterBody(StandaloneDuel* duel, PlayState* play)
{
    Actor* actor = &duel->actor;
    Collider_UpdateCylinder(actor, &duel->body);
    duel->body.dim.pos.x = actor->world.pos.x + Math_SinS(actor->shape.rot.y) * 3.f;
    duel->body.dim.pos.z = actor->world.pos.z + Math_CosS(actor->shape.rot.y) * 3.f;
    CollisionCheck_SetOC(play, &play->colChkCtx, &duel->body.base);
}

static void Gomess_StartDefeat(StandaloneGomess* gomess)
{
    StandaloneDuel* duel = &gomess->duel;
    Actor* actor = &duel->actor;
    duel->phase = DUEL_DEFEAT;
    duel->timer = GOMESS_DEFEAT_WINDUP_FRAMES;
    gomess->deathStage = GOMESS_DEATH_WINDUP;
    gomess->looseCore = 0;
    gomess->scythePoseKind = 0;
    actor->speed = 0.f;
    actor->shape.rot.x = 0;
    actor->shape.rot.y += 0x2000;
    actor->flags &= ~(ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE);
    Actor_SetColorFilter(actor, 0x4000, 255, 0, 30);
    Animation_PlayOnce(&duel->skel, Duel_Anim(0x2de8));
    for (int i = 0; i < GOMESS_BAT_COUNT; ++i)
    {
        EnemyGomessBat* bat = gomess->bats[i];
        if (!bat)
            continue;
        bat->attackTimer = bat->leaderTimer = bat->following = bat->gather = bat->launch = 0;
        bat->body.base.atFlags = AT_NONE;
        bat->body.base.acFlags = AC_NONE;
        bat->deathStage = GOMESS_BAT_DEATH_FLY;
        bat->crowdState = 0;
        bat->scatterYaw = bat->yaw;
        bat->scatterPitch = -bat->pitch;
        for (int j = 0; j < ARRAY_COUNT(bat->hitRecovery); ++j)
            bat->hitRecovery[j] = bat->falling[j] = 0;
    }
    Duel_Sfx(actor, 0x3ab2);
}

static void Gomess_Defeat(StandaloneGomess* gomess, PlayState* play, int finished)
{
    StandaloneDuel* duel = &gomess->duel;
    Actor* actor = &duel->actor;

    if (gomess->droppedScythe == 1 && gomess->deathStage >= GOMESS_DEATH_DROP_SCYTHE)
    {
        MtxF* matrix = &gomess->droppedScytheMtx;

        Vec3f pos = { matrix->xw, matrix->yw - 53.f, matrix->zw };
        int landed = Gomess_FallPoint(gomess, play, &pos, &gomess->droppedScytheVelocity, 1.5f);
        matrix->xw = pos.x;
        matrix->yw = pos.y + 53.f;
        matrix->zw = pos.z;
        if (landed || duel->timer < 3)
        {
            int turn = landed ? 0 : duel->timer;
            Matrix_Push();
            Matrix_Translate(matrix->xw, matrix->yw, matrix->zw, MTXMODE_NEW);
            Matrix_RotateY(gomess->droppedScytheYaw * (M_PI / 32768.f), MTXMODE_APPLY);
            Matrix_RotateX((0x1100 - turn * 0x1800) * (M_PI / 32768.f), MTXMODE_APPLY);
            Matrix_RotateZ(-0xa00 * (M_PI / 32768.f), MTXMODE_APPLY);
            Matrix_Scale(.01f, .01f, .01f, MTXMODE_APPLY);
            Matrix_Get(matrix);
            Matrix_Pop();
        }
        Gomess_ProjectScytheSound(gomess, play);
        if (landed)
        {
            gomess->droppedScythe = 2;
            Gomess_ScytheStop(&gomess->scytheScreenPos, GOMESS_SFX_SCYTHE_FALL);
            Gomess_ScythePlay(&gomess->scytheScreenPos, GOMESS_SFX_SCYTHE_GROUND);
        }
    }

    switch (gomess->deathStage)
    {
        case GOMESS_DEATH_WINDUP:
            if (duel->skel.animation == Duel_Anim(0x2de8))
            {
                actor->shape.rot.y += 0x2000;
                if (duel->skel.curFrame > 3.f && !gomess->droppedScythe &&
                    play->gameplayFrames - gomess->scytheHandlePoseFrame <= 1)
                {
                    gomess->droppedScythe = 1;
                    gomess->droppedScytheYaw = actor->world.rot.y - 0x3300;
                    Matrix_Push();
                    *Matrix_GetCurrent() = gomess->scytheHandleMtx;
                    Matrix_RotateX(-M_PI / 8.f, MTXMODE_APPLY);
                    Matrix_Get(&gomess->droppedScytheMtx);
                    Matrix_Pop();
                    gomess->droppedScytheMtx.yw += 18.f;
                    Duel_Sfx(actor, GOMESS_SFX_SCYTHE);
                }
                if (finished)
                    Animation_PlayLoop(&duel->skel, Duel_Anim(0x352c));
            }
            else if (!duel->timer)
            {
                gomess->deathStage = GOMESS_DEATH_DROP_SCYTHE;
                duel->timer = GOMESS_SCYTHE_FALL_FRAMES;
                Gomess_ProjectScytheSound(gomess, play);
                Gomess_ScytheFadeIn(&gomess->scytheScreenPos, GOMESS_SFX_SCYTHE_FALL, 0x41);
            }
            break;
        case GOMESS_DEATH_DROP_SCYTHE:
            if (!duel->timer)
            {
                gomess->deathStage = GOMESS_DEATH_DISSOLVE;
                duel->timer = 26;
                for (int i = 0; i < GOMESS_BAT_COUNT; ++i)
                {
                    EnemyGomessBat* bat = gomess->bats[i];
                    if (!bat)
                        continue;
                    bat->deathStage = GOMESS_BAT_DEATH_DIVE;
                    bat->deathVelocityY = 0.f;
                }
            }
            break;
        case GOMESS_DEATH_DISSOLVE: {
            int age = 26 - duel->timer;
            actor->shape.shadowAlpha = MAX(0, 252 - 14 * (age - 1));
            for (int i = 0; i < ARRAY_COUNT(gomess->defeatFlameAlpha); ++i)
            {
                if (!gomess->defeatFlameAlpha[i])
                    continue;
                gomess->defeatFlamePos[i].y += 1.7f;
                gomess->defeatFlameAlpha[i] = MAX(0, gomess->defeatFlameAlpha[i] - 30);
            }
            if (age > 0 && age <= 18)
            {
                Vec3f velocity = { 0.f, -1.5f, 0.f };
                Vec3f accel = { 0.f, -.2f, 0.f };
                Color_RGBA8 prim = { 0, 0, 0, 0 };
                Color_RGBA8 env = { 40, 40, 40, 0 };
                Camera* camera = GET_ACTIVE_CAM(play);
                s16 yaw = camera ? Camera_GetCamDirYaw(camera) + 0x8000 : actor->yawTowardsPlayer;
                f32 sin = Math_SinS(yaw);
                f32 cos = Math_CosS(yaw);
                Duel_Sfx(actor, 0x3a1f - SFX_FLAG);
                for (int i = 0; i < 7; ++i)
                {
                    int index = ((age - 1) * 7 + i) % ARRAY_COUNT(gomess->defeatFlamePos);
                    f32 offset = Rand_ZeroOne() * 100.f - 70.f;
                    Vec3f pos = actor->world.pos;
                    pos.x += 30.f * sin + offset * cos;
                    pos.y += Rand_ZeroOne() * 20.f + 4.f * (age - 1) - 30.f;
                    pos.z += 30.f * cos - offset * sin;
                    gomess->defeatFlamePos[index] = pos;
                    gomess->defeatFlameAlpha[index] = 255 - i * 4;
                    Gomess_Sparkle(play, &pos, &velocity, &accel, &prim, &env);
                }
            }
            if (!duel->timer)
            {
                gomess->deathStage = GOMESS_DEATH_DROP_CORE;
                gomess->looseCorePos = gomess->corePoseValid ? gomess->corePos : actor->focus.pos;
                gomess->looseCoreVelocity = -1.f;
            }
            break;
        }
        case GOMESS_DEATH_DROP_CORE:
            if (Gomess_FallPoint(gomess, play, &gomess->looseCorePos, &gomess->looseCoreVelocity, 1.f))
            {
                gomess->deathStage = GOMESS_DEATH_SHATTER;
                duel->timer = 19;
                Vec3f accel = { 0.f, -.2f, 0.f };
                Color_RGBA8 prim = { 0, 200, 100, 0 };
                Color_RGBA8 env = { 0, 100, 0, 0 };
                for (int i = 0; i < 40; ++i)
                {
                    Vec3f velocity = { Rand_ZeroOne() * 4.f - 2.f, Rand_ZeroOne() * 2.f + 3.f,
                                       Rand_ZeroOne() * 4.f - 2.f };
                    Gomess_Sparkle(play, &gomess->looseCorePos, &velocity, &accel, &prim, &env);
                }
                Duel_Sfx(actor, 0x3ab5);
            }
            break;
        case GOMESS_DEATH_SHATTER:
            if (!duel->timer)
            {
                gomess->deathStage = GOMESS_DEATH_FADE;
                duel->timer = 51;
            }
            break;
        case GOMESS_DEATH_FADE:
            for (int i = 0; i < GOMESS_BAT_COUNT; ++i)
            {
                if (gomess->bats[i])
                    gomess->bats[i]->deathAlpha = duel->timer * 5;
            }
            if (!duel->timer)
                Actor_Kill(actor);
            break;
    }
}

static void Gomess_SetupRecoil(StandaloneGomess* gomess)
{
    StandaloneDuel* duel = &gomess->duel;
    Actor* actor = &duel->actor;
    Actor* attacker = gomess->core.base.ac;
    ColliderElement* hit = gomess->coreElement.base.acHitElem;
#if defined(GAME_MM)
    const u32 projectiles = DMG_NORMAL_ARROW | DMG_FIRE_ARROW | DMG_ICE_ARROW | DMG_LIGHT_ARROW | DMG_DEKU_BUBBLE;
#else
    const u32 projectiles = DMG_ARROW_NORMAL | DMG_ARROW_FIRE | DMG_ARROW_ICE | DMG_ARROW_LIGHT | DMG_SLINGSHOT;
#endif
    actor->world.rot.y = actor->shape.rot.y + 0x8000;
    if (attacker)
    {
        actor->world.rot.y = hit && (hit->atDmgInfo.dmgFlags & projectiles)
                                 ? attacker->shape.rot.y
                                 : Math_Vec3f_Yaw(&attacker->world.pos, &actor->world.pos);
    }
    actor->shape.rot.x = 0;
    actor->speed = 10.f;
    Actor_SetColorFilter(actor, 0x4000, 255, 0, 15);
    gomess->scythePoseKind = 0;
    gomess->scythe.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
    gomess->spin.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
    for (int i = 0; i < GOMESS_BAT_COUNT; ++i)
    {
        EnemyGomessBat* bat = gomess->bats[i];
        if (!bat)
            continue;
        bat->attackTimer = bat->leaderTimer = bat->scatterTimer = 0;
        bat->gather = bat->launch = bat->following = bat->regrouping = 0;
        if (!bat->returning)
            bat->returnRoute = (EnemyFlightReturn){ 0 };
        bat->returning = 1;
        bat->recallDistance = 30;
    }
}

static void Gomess_RegisterScythe(StandaloneGomess* gomess, PlayState* play, int kind)
{
    if (gomess->scythePoseKind == kind && play->gameplayFrames - gomess->scythePoseFrame <= 1)
    {
        CollisionCheck_SetAT(play, &play->colChkCtx, &gomess->scythe.base);
    }
}

static int Gomess_CanReachPlayer(Actor* actor, PlayState* play)
{
    Vec3f from = actor->world.pos;
    Vec3f to = GET_PLAYER(play)->actor.world.pos;
    from.y += 40.f;
    to.y += 30.f;
    return enemyFlightClear(play, &from, &to);
}

static void Gomess_Float(StandaloneGomess* gomess, PlayState* play)
{
    Actor* actor = &gomess->duel.actor;
    Duel_Sfx(actor, 0x3a60 - SFX_FLAG);
    if (!gomess->floatTimer)
        gomess->floatTimer = 40;
    --gomess->floatTimer;

    if (actor->gravity)
        return;
    f32 base = fabsf(actor->floorHeight - actor->home.pos.y) <= 40.f ? MAX(actor->home.pos.y, actor->floorHeight)
                                                                     : actor->floorHeight;
    f32 height = base + (1.f - Math_CosS(gomess->floatTimer * 0x666)) * 7.5f;
    if (height > actor->world.pos.y)
    {
        Vec3f start = actor->world.pos;
        start.y += 110.f;
        Vec3f target = start;
        target.y += height - actor->world.pos.y;
        CollisionPoly* poly;
        s32 bgId;
        Vec3f hit;
        if (BgCheck_EntityLineTest1(&play->colCtx, &start, &target, &hit, &poly, 0, 0, 1, 0, &bgId))
            return;
    }
    actor->world.pos.y = height;
    actor->velocity.y = -actor->gravity;
}

static void Gomess_StartSpin(StandaloneGomess* gomess, int alreadySpinning)
{
    StandaloneDuel* duel = &gomess->duel;
    duel->phase = GOMESS_SPIN;
    duel->timer = alreadySpinning ? 0 : 10;
    gomess->spinFrames = alreadySpinning ? 10 : 0;
    duel->actor.speed = alreadySpinning ? 10.f : 0.f;
    gomess->spin.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
    if (!alreadySpinning)
    {
        gomess->scytheAfterImages = -3;
        gomess->scythePoseKind = 0;
        gomess->scythe.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
        AnimationHeader* anim = Duel_Anim(0xb508);
        Animation_Change(&duel->skel, anim, 1.f, 0.f, Animation_GetLastFrame(anim), ANIMMODE_LOOP, -3.f);
    }
}

static void Gomess_Combat(StandaloneDuel* duel, PlayState* play, int finished, f32 prevFrame)
{
    Actor* actor = &duel->actor;
    f32 distance = actor->xzDistToPlayer;
    StandaloneGomess* gomess = (StandaloneGomess*)duel;
    Gomess_UpdateBats(gomess, play);
    int hoverSupported = actor->floorPoly && (fabsf(actor->floorHeight - actor->home.pos.y) <= 40.f ||
                                              fabsf(actor->floorHeight - actor->world.pos.y) <= 40.f);
    actor->gravity = hoverSupported ? 0.f : -2.f;
    if (hoverSupported)
        actor->velocity.y = 0.f;
    if (duel->phase != GOMESS_SPIN)
    {
        Math_ScaledStepToS(&gomess->cloakUpperRot, 0, 0x800);
        Math_ScaledStepToS(&gomess->cloakLowerRot, 0, 0x800);
    }
    if (duel->phase == DUEL_HURT)
    {
        Gomess_Float(gomess, play);
        Math_StepToF(&actor->speed, 0.f, .5f);
        if (finished)
        {
            Gomess_StartSpin(gomess, 0);
            actor->world.rot.y = actor->shape.rot.y;
            Gomess_UpdateSpinTris(gomess);
        }
        return;
    }
    actor->speed = 0.f;
    if (duel->phase == GOMESS_SWING)
    {
        Math_SmoothStepToS(&actor->shape.rot.y, actor->yawTowardsPlayer, 4, 0x1000, 0x200);
    }
    else if (duel->phase != GOMESS_SWING_END && (duel->phase != GOMESS_SPIN || duel->timer != 0))
    {
        Math_SmoothStepToS(&actor->shape.rot.y, actor->yawTowardsPlayer, 8, 0x1000, 0x100);
    }
    actor->world.rot.y = actor->shape.rot.y;
    switch (duel->phase)
    {
        case DUEL_APPROACH: {
            int crowd = Gomess_CrowdCount(gomess);
            actor->speed = 1.5f;
            Gomess_Float(gomess, play);
            if (crowd < 5 && Rand_ZeroOne() < .4f && Gomess_ProjectileApproaching(actor, play))
            {
                duel->phase = GOMESS_GUARD;
                duel->timer = 30;
                actor->speed = 0.f;
                gomess->scytheAfterImages = -3;
                gomess->scythePoseKind = 0;
                gomess->scythe.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
                AnimationHeader* anim = Duel_Anim(0xb508);
                Animation_Change(&duel->skel, anim, 1.f, 0.f, Animation_GetLastFrame(anim), ANIMMODE_LOOP, -3.f);
            }
            else if (distance < 200.f && duel->timer < 100 && Gomess_CanReachPlayer(actor, play))
            {
                duel->phase = GOMESS_SWING;
                duel->timer = gomess->floatTimer = 0;
                actor->speed = 8.f;
                Animation_Change(&duel->skel, Duel_Anim(0x0e64), 1.f, 0.f, 10.f, ANIMMODE_ONCE, -3.f);
                Duel_Sfx(actor, 0x3ab3);
            }
            else if (!duel->timer)
            {
                if (crowd >= 5)
                {
                    Duel_Sfx(actor, 0x3ab0);
                    duel->phase = GOMESS_SWARM;
                    duel->timer = 0;
                    actor->speed = 0.f;
                    gomess->floatTimer = 0;
                    gomess->swarmLaunched = gomess->swarmHits = 0;
                    for (int i = 0; i < GOMESS_BAT_COUNT; ++i)
                    {
                        if (gomess->bats[i])
                            gomess->bats[i]->gather = 1;
                    }
                    AnimationHeader* anim = Duel_Anim(0x15b4);
                    Animation_Change(&duel->skel, anim, 1.f, 0.f, Animation_GetLastFrame(anim), ANIMMODE_ONCE, 5.f);
                }
                else
                {
                    Gomess_StartSpin(gomess, 0);
                }
            }
            break;
        }
        case GOMESS_SWING:
            if (fabsf(actor->floorHeight - actor->home.pos.y) <= 40.f)
                Math_StepToF(&actor->world.pos.y, MAX(actor->home.pos.y, actor->floorHeight), 3.f);
            actor->shape.rot.x = MAX(0.f, (100.f - distance) * .01f * 0x400);
            if (duel->skel.playSpeed == 0.f)
            {
                if (!duel->timer)
                {
                    duel->phase = GOMESS_SWING_END;
                    duel->skel.endFrame = Animation_GetLastFrame(Duel_Anim(0x0e64));
                    duel->skel.playSpeed = 1.f;
                    gomess->scythePoseKind = 0;
                    gomess->scythe.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
                }
            }
            else if (finished && distance < 100.f)
            {
                Gomess_ScythePlay(&gomess->scytheScreenPos, GOMESS_SFX_SCYTHE);
                duel->skel.playSpeed = 0.f;
                duel->timer = 2;
                actor->speed = 0.f;
            }
            else
            {
                actor->speed = 8.f;
            }
            break;
        case GOMESS_SWING_END:
            if (prevFrame > 20.f)
                Math_ScaledStepToS(&actor->shape.rot.x, 0, 0x200);
            if (finished)
                Gomess_Approach(duel, 140);
            else if (prevFrame <= 20.f)
                Gomess_RegisterScythe(gomess, play, 1);
            break;
        case GOMESS_GUARD:
            Gomess_Float(gomess, play);
            if (gomess->scytheAfterImages < 7)
                ++gomess->scytheAfterImages;
            if (Animation_OnFrame(&duel->skel, 0.f))
                Gomess_ScythePlay(&gomess->scytheScreenPos, GOMESS_SFX_SCYTHE_ROLL);
            if (!duel->timer)
            {
                if (distance > 200.f)
                    Gomess_StartSpin(gomess, 1);
                else
                    Gomess_Approach(duel, 100);
            }
            break;
        case GOMESS_SPIN:
            Gomess_Float(gomess, play);
            if (gomess->scytheAfterImages < 7)
                ++gomess->scytheAfterImages;
            if (Animation_OnFrame(&duel->skel, 0.f))
                Gomess_ScythePlay(&gomess->scytheScreenPos, GOMESS_SFX_SCYTHE_ROLL);
            if (!duel->timer)
                actor->speed = 10.f;
            ++gomess->spinFrames;
            s16 wave = gomess->spinFrames * 0x2000;
            Math_ScaledStepToS(&gomess->cloakUpperRot, 0x3000 + Math_SinS(wave) * 0x800, 0x1000);
            Math_ScaledStepToS(&gomess->cloakLowerRot, Math_SinS(wave - 0x8000) * 0x800, 0x1000);
            if (gomess->spinFrames >= 100 || (actor->bgCheckFlags & BGCHECKFLAG_WALL) ||
                ((gomess->scythe.base.atFlags | gomess->spin.base.atFlags) & AT_HIT))
            {
                Gomess_Approach(duel, 140);
                actor->world.rot.y = actor->shape.rot.y;
            }
            break;
        case GOMESS_SWARM: {
            if (!gomess->swarmLaunched)
            {
                Math_StepToF(&actor->world.pos.y, actor->home.pos.y, 5.f);
                if (!finished)
                    break;
                gomess->swarmLaunched = 1;
                gomess->swarmWaitTimer = 340;
                Animation_PlayLoop(&duel->skel, Duel_Anim(0x1834));
                for (int i = 0; i < GOMESS_BAT_COUNT; ++i)
                {
                    if (gomess->bats[i])
                        gomess->bats[i]->launch = 1;
                }
            }
            else
            {
                int crowd = Gomess_CrowdCount(gomess);
                if (crowd >= 5 || !--gomess->swarmWaitTimer)
                {
                    Gomess_Approach(duel, 100);
                }
                else
                {
                    int pending = 0;
                    for (int i = 0; i < GOMESS_BAT_COUNT; ++i)
                    {
                        const EnemyGomessBat* bat = gomess->bats[i];
                        if (bat && !bat->crowdState && (!bat->returning || bat->returnRoute.stalledFrames < 240))
                        {
                            pending = 1;
                            break;
                        }
                    }
                    if (!pending)
                        Gomess_Approach(duel, 100);
                }
            }
            break;
        }
    }
    if (duel->phase == GOMESS_GUARD || duel->phase == GOMESS_SPIN)
    {
        Gomess_UpdateSpinTris(gomess);
        Gomess_RegisterScythe(gomess, play, 2);
    }
}

static void Keeta_Play(StandaloneDuel* duel, u16 animation, u8 mode)
{
    AnimationHeader* anim = Duel_Anim(animation);
    Animation_Change(&duel->skel, anim, 1.f, 0.f, Animation_GetLastFrame(anim), mode, -2.f);
}

static void Keeta_ResumeWalk(StandaloneKeeta* keeta)
{
    StandaloneDuel* duel = &keeta->duel;
    duel->phase = DUEL_APPROACH;
    duel->walking = 1;
    duel->timer = Rand_ZeroOne() * 30.f;
    duel->actor.flags &= ~ACTOR_FLAG_LOCK_ON_DISABLED;
    duel->actor.gravity = -2.f;
    keeta->laughed = 0;
    Keeta_Play(duel, 0x0ff0, ANIMMODE_LOOP);
}

static void Keeta_Quake(PlayState* play, Actor* actor, int strength, int duration)
{
    s16 quake = Quake_Request(Play_GetCamera(play, CAM_ID_MAIN), 3);
    Quake_SetSpeed(quake, 20000);
    Quake_SetPerturbations(quake, strength, 0, 0, 0);
    Quake_SetDuration(quake, duration);
    Rumble_Request(actor->xyzDistToPlayerSq, 100, duration, 30);
}

static void Keeta_DrawLimb(PlayState* play, s32 limb, Gfx** list, Vec3s* rot, void* arg)
{
    StandaloneKeeta* keeta = arg;
    static const u8 limbs[] = { 10, 9, 6, 3, 13, 16, 19 };
    if (keeta->bodyPoseFrame != play->gameplayFrames)
    {
        keeta->bodyPoseFrame = play->gameplayFrames;
        keeta->bodyPoseMask = 0;
        keeta->footPoseMask = 0;
    }
    if (limb == 0x11 || limb == 0x14)
    {
        int foot = limb == 0x14;
        Vec3f origin = { 0.f, 0.f, 0.f };
        Matrix_MultVec3f(&origin, &keeta->footPos[foot]);
        keeta->footPoseMask |= 1u << foot;
    }
    for (int i = 0; i < ARRAY_COUNT(limbs); ++i)
    {
        if (limb != limbs[i])
            continue;
        Collider_UpdateSpheres(limb, &keeta->bodySpheres);
        keeta->bodyPoseMask |= 1u << i;
        break;
    }
}

static int Keeta_SafeStep(Actor* actor, PlayState* play, f32 distance)
{
    Vec3f from = actor->world.pos;
    from.y += 35.f;
    Vec3f next = from;
    next.x += distance * Math_SinS(actor->world.rot.y);
    next.z += distance * Math_CosS(actor->world.rot.y);
    CollisionPoly* floor = NULL;
    s32 bgId;
    f32 height = BgCheck_EntityRaycastDown4(&play->colCtx, &floor, &bgId, actor, &next);
    return floor && fabsf(height - actor->world.pos.y) <= 25.f && enemyFlightClear(play, &from, &next);
}

static void Keeta_RegisterBlade(StandaloneKeeta* keeta, PlayState* play, Player* player)
{
    if (!(keeta->bodyPoseMask & (1 << 1)) || play->gameplayFrames - keeta->bodyPoseFrame > 1)
        return;
    Vec3s* center = &keeta->bodyElements[1].dim.worldSphere.center;
    Vec3f blade = { center->x, center->y, center->z };
    Vec3f target = player->actor.world.pos;
    target.y += 30.f;
    if (Math_Vec3f_DistXYZ(&blade, &target) < 100.f && enemyFlightClear(play, &blade, &target))
        CollisionCheck_SetAT(play, &play->colChkCtx, &keeta->bodySpheres.base);
}

enum { KEETA_DEFEAT_AIRBORNE, KEETA_DEFEAT_FALLEN, KEETA_DEFEAT_FINAL_POSE };

static void Keeta_StartDefeat(StandaloneKeeta* keeta)
{
    StandaloneDuel* duel = &keeta->duel;
    Actor* actor = &duel->actor;
    duel->phase = DUEL_DEFEAT;
    duel->timer = 50;
    keeta->defeatStage = KEETA_DEFEAT_AIRBORNE;
    keeta->defeatSpin = 0x4000;
    actor->flags &= ~(ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE);
    actor->flags |= ACTOR_FLAG_LOCK_ON_DISABLED;
    actor->bgCheckFlags &= ~BGCHECKFLAG_GROUND;
    actor->speed = 0.f;
    actor->gravity = -2.f;
    actor->velocity.y = 30.f;
    actor->shape.rot.x = actor->shape.rot.z = 0;
    Actor_SetColorFilter(actor, 0x4000, 255, 0, 8);
    Keeta_Play(duel, 0x0400, ANIMMODE_ONCE);
    Duel_Sfx(actor, 0x3aa6);
}

static void Keeta_Defeat(StandaloneKeeta* keeta, PlayState* play, int finished)
{
    StandaloneDuel* duel = &keeta->duel;
    Actor* actor = &duel->actor;
    if (keeta->defeatStage == KEETA_DEFEAT_AIRBORNE)
    {
        if (actor->velocity.y >= -5.f)
        {
            actor->world.rot.y += keeta->defeatSpin;
            Math_SmoothStepToS(&keeta->defeatSpin, 0x1000, 1, 0x1f4, 0);
        }
        else
        {
            Math_SmoothStepToS(&actor->world.rot.y, actor->yawTowardsPlayer, 1, 0x3e8, 0);
        }
        actor->shape.rot.y = actor->world.rot.y;
        if (actor->velocity.y > 0.f)
        {
            Vec3f from = actor->world.pos;
            from.y += 120.f;
            Vec3f next = from;
            next.y += actor->velocity.y;
            if (!enemyFlightClear(play, &from, &next))
                actor->velocity.y = 0.f;
        }
        Actor_MoveWithGravity(actor);
        Actor_UpdateBgCheckInfo(play, actor, 30.f, 60.f, 40.f, 5);
        if ((actor->bgCheckFlags & BGCHECKFLAG_GROUND) || !duel->timer)
        {
            keeta->defeatStage = KEETA_DEFEAT_FALLEN;
            Keeta_Play(duel, 0x2590, ANIMMODE_ONCE);
            if (actor->bgCheckFlags & BGCHECKFLAG_GROUND)
            {
                Actor_SpawnFloorDustRing(play, actor, &actor->world.pos, 55.f, 20, 8.f, 1000, 100, 1);
                Duel_Sfx(actor, 0x387b);
                Keeta_Quake(play, actor, 4, 10);
            }
        }
    }
    else if (keeta->defeatStage == KEETA_DEFEAT_FALLEN && finished)
    {
        keeta->defeatStage = KEETA_DEFEAT_FINAL_POSE;
        Keeta_Play(duel, 0x7120, ANIMMODE_ONCE);
    }
    else if (keeta->defeatStage == KEETA_DEFEAT_FINAL_POSE && finished)
    {
        Actor_Kill(actor);
    }
}

static int Keeta_ReceiveHit(StandaloneKeeta* keeta)
{
    StandaloneDuel* duel = &keeta->duel;
    Actor* actor = &duel->actor;
    u8 effect = actor->colChkInfo.damageEffect;
    if (duel->headCooldown || duel->phase == DUEL_HURT)
        return 0;
    if (effect == KEETA_DAMAGE_MELEE || effect == KEETA_DAMAGE_SPIN || effect == KEETA_DAMAGE_ELECTRIC)
    {
        int wasChasing = duel->shield != 0;
        duel->shield = duel->chaseHasPoint = 0;
        if (effect == KEETA_DAMAGE_MELEE)
            return 1;
        if (effect == KEETA_DAMAGE_ELECTRIC)
        {
            keeta->damageEffect = 32;
            keeta->damageTimer = 40;
            if (!wasChasing)
            {
                duel->headCooldown = 40;
                Actor_SetColorFilter(actor, 0, 120, 0, 40);
                Duel_Sfx(actor, 0x389e);
            }
        }
        return 0;
    }
    if (effect == KEETA_DAMAGE_EXPLOSIVE && !duel->shield)
        return 1;
    if (effect == KEETA_DAMAGE_STUN && duel->shield)
        return 0;
    if (effect == KEETA_DAMAGE_FIRE)
    {
        keeta->damageEffect = 0;
        keeta->damageTimer = 40;
    }
    else if (effect == KEETA_DAMAGE_ICE)
    {
        if (keeta->damageTimer && (keeta->damageEffect == 10 || keeta->damageEffect == 11))
            return 0;
        keeta->damageEffect = 11;
        keeta->damageTimer = 80;
    }
    else if (effect == KEETA_DAMAGE_LIGHT)
    {
        keeta->damageEffect = 20;
        keeta->damageTimer = 40;
    }
    else if (effect != KEETA_DAMAGE_PROJECTILE && effect != KEETA_DAMAGE_EXPLOSIVE && effect != KEETA_DAMAGE_STUN)
    {
        return 0;
    }
    int stunned = !duel->shield || effect == KEETA_DAMAGE_ICE;
    if (effect == KEETA_DAMAGE_ICE)
        duel->shield = duel->chaseHasPoint = 0;
    actor->speed = 0.f;
    duel->walking = 0;
    duel->headCooldown = stunned ? 40 : 16;
    Keeta_Play(duel, 0x0400, ANIMMODE_ONCE);
    Duel_Sfx(actor, 0x3aa5);
    if (stunned)
    {
        Actor_SetColorFilter(actor, 0, 255, 0, 40);
        Duel_Sfx(actor, 0x389e);
    }
    return 0;
}

#if defined(GAME_OOT)
void FOREIGN_ENEMY_API_DAMAGE_EFFECTS(PlayState*, Actor*, Vec3f*, s16, f32, f32, f32, u8);
void FOREIGN_ENEMY_API_ICE_SHATTER(PlayState*, Actor*, Vec3f*, s32, s32, f32, f32);
#define Keeta_DrawDamageEffects FOREIGN_ENEMY_API_DAMAGE_EFFECTS
#define Keeta_ShatterIce FOREIGN_ENEMY_API_ICE_SHATTER
#else
#define Keeta_DrawDamageEffects Actor_DrawDamageEffects
#define Keeta_ShatterIce Actor_SpawnIceEffects
#endif

static void Keeta_EffectPositions(StandaloneKeeta* keeta, Vec3f* positions)
{
    for (int i = 0; i < ARRAY_COUNT(keeta->bodyElements); ++i)
    {
        Vec3s* center = &keeta->bodyElements[i].dim.worldSphere.center;
        positions[i] = (Vec3f){ center->x, center->y, center->z };
    }
}

static void Keeta_EndStun(StandaloneKeeta* keeta, PlayState* play)
{
    if (keeta->damageTimer && (keeta->damageEffect == 10 || keeta->damageEffect == 11))
    {
        if (keeta->bodyPoseMask == 0x7f && play->gameplayFrames - keeta->bodyPoseFrame <= 1)
        {
            Vec3f positions[ARRAY_COUNT(keeta->bodyElements)];
            Keeta_EffectPositions(keeta, positions);
            Keeta_ShatterIce(play, &keeta->duel.actor, positions, ARRAY_COUNT(positions), 2, 1.5f, .4f);
        }
        keeta->damageTimer = 0;
    }
    if (!keeta->duel.shield)
        Keeta_ResumeWalk(keeta);
    else
        Keeta_Play(&keeta->duel, 0x65d8, ANIMMODE_LOOP);
}

static void Keeta_DrawDamage(StandaloneKeeta* keeta, PlayState* play)
{
    if (!keeta->damageTimer || keeta->bodyPoseMask != 0x7f)
        return;
    Vec3f positions[ARRAY_COUNT(keeta->bodyElements)];
    Keeta_EffectPositions(keeta, positions);
    int frozen = keeta->damageEffect == 10 || keeta->damageEffect == 11;
    f32 scale = frozen ? MIN(.8f, (80 - keeta->damageTimer) * .04f) : .8f;
    Keeta_DrawDamageEffects(play, &keeta->duel.actor, positions, ARRAY_COUNT(positions), scale, 1.5f,
                            MIN(1.f, keeta->damageTimer * .05f), keeta->damageEffect);
}

static int Keeta_TrySlam(StandaloneKeeta* keeta, PlayState* play)
{
    StandaloneDuel* duel = &keeta->duel;
    Actor* actor = &duel->actor;
    if (!(actor->bgCheckFlags & BGCHECKFLAG_GROUND))
        return 0;
    Vec3f from = actor->world.pos;
    from.y += 80.f;
    Vec3f next = from;
    next.y += 45.f;
    if (!enemyFlightClear(play, &from, &next))
        return 0;

    duel->phase = DUEL_KEETA_JUMP;
    duel->walking = 0;
    actor->speed = 0.f;
    actor->velocity.y = 85.f;
    actor->gravity = -8.f;
    actor->flags |= ACTOR_FLAG_LOCK_ON_DISABLED;
    Duel_Sfx(actor, 0x3aaa);
    Duel_Sfx(actor, 0x386c);
    Keeta_Play(duel, 0x43a4, ANIMMODE_ONCE);
    if (play->gameplayFrames - keeta->bodyPoseFrame <= 1)
    {
        for (int i = 0; i < ARRAY_COUNT(keeta->footPos); ++i)
        {
            if (keeta->footPoseMask & (1 << i))
                Actor_SpawnFloorDustRing(play, actor, &keeta->footPos[i], 5.f, 10, 8.f, 1000, 100, 1);
        }
    }
    return 1;
}

static void Keeta_Combat(StandaloneKeeta* keeta, PlayState* play, int finished)
{
    StandaloneDuel* duel = &keeta->duel;
    Actor* actor = &duel->actor;
    Player* player = GET_PLAYER(play);
    actor->speed = 0.f;
    if (duel->phase == DUEL_APPROACH && duel->shield)
    {
        if (actor->xzDistToPlayer >= 380.f || fabsf(actor->yDistanceFromLink) >= 85.f)
            return;
        duel->shield = 0;
        duel->chaseHasPoint = 0;
        Keeta_ResumeWalk(keeta);
    }
    if (duel->phase == DUEL_KEETA_JUMP)
    {
        if (actor->velocity.y > 0.f)
        {
            Vec3f from = actor->world.pos;
            from.y += 80.f;
            Vec3f next = from;
            Math_ApproachF(&next.x, player->actor.world.pos.x, .5f, 30.f);
            Math_ApproachF(&next.z, player->actor.world.pos.z, .5f, 30.f);
            next.y += actor->velocity.y;
            if (enemyFlightClear(play, &from, &next))
            {
                actor->world.pos.x = next.x;
                actor->world.pos.z = next.z;
                return;
            }
            next.x = from.x;
            next.z = from.z;
            if (enemyFlightClear(play, &from, &next))
                return;
            actor->velocity.y = 0.f;
        }
        duel->phase = DUEL_KEETA_DROP;
        Keeta_Play(duel, 0x4510, ANIMMODE_ONCE);
    }
    if (duel->phase == DUEL_KEETA_DROP)
    {
        Vec3f target = player->actor.world.pos;
        target.y += 20.f;
        if (!duel->walking && fabsf(actor->yDistanceFromLink) < 40.f && actor->xzDistToPlayer < 70.f &&
            !player->invincibilityTimer && enemyFlightClear(play, &actor->world.pos, &target))
        {
            duel->walking = 1;
            FOREIGN_ENEMY_API_KNOCKBACK_LARGE(play, actor, 2.f, actor->yawTowardsPlayer, 5.f, 16);
        }
        if (actor->bgCheckFlags & BGCHECKFLAG_GROUND)
        {
            duel->phase = DUEL_KEETA_LAND;
            duel->walking = 0;
            Keeta_Play(duel, 0x1f90, ANIMMODE_ONCE);
            Actor_SpawnFloorDustRing(play, actor, &actor->world.pos, actor->shape.shadowScale, 10, 8.f, 2000, 100, 1);
            Keeta_Quake(play, actor, 10, 10);
            Duel_Sfx(actor, 0x387b);
        }
        return;
    }
    if (duel->phase == DUEL_KEETA_LAND)
    {
        if (!duel->walking)
        {
            duel->walking = 1;
            Keeta_Play(duel, 0xc790, ANIMMODE_ONCE);
        }
        else if (finished)
        {
            Keeta_ResumeWalk(keeta);
        }
        return;
    }
    if (duel->phase == DUEL_APPROACH || duel->phase == DUEL_ATTACK)
    {
        int bounced = keeta->bodySpheres.base.atFlags & AT_BOUNCED;
        int hit = keeta->bodySpheres.base.atFlags & AT_HIT;
        keeta->bodySpheres.base.atFlags &= ~(AT_BOUNCED | AT_HIT);
        int guarded = 0;
        if (duel->phase == DUEL_ATTACK && (player->stateFlags1 & (1u << 22)) && (keeta->bodyPoseMask & (1 << 1)) &&
            play->gameplayFrames - keeta->bodyPoseFrame <= 1)
        {
            Vec3s* center = &keeta->bodyElements[1].dim.worldSphere.center;
            Vec3f blade = { center->x, center->y, center->z };
            Vec3f target = player->actor.world.pos;
            target.y += 30.f;
            s16 facing = player->actor.shape.rot.y - actor->shape.rot.y + 0x8000;
            guarded = ABS(facing) < 0x2000 && Math_Vec3f_DistXYZ(&blade, &target) < 70.f &&
                      enemyFlightClear(play, &blade, &target);
        }
        if ((bounced || guarded) && duel->phase == DUEL_ATTACK)
        {
            Animation_Change(&duel->skel, Duel_Anim(0x0c50), -1.f, MAX(0.f, duel->skel.curFrame - 1.f), 0.f,
                             ANIMMODE_ONCE, 0.f);
            duel->phase = DUEL_RECOVER;
            duel->timer = 10;
            Duel_Sfx(actor, NA_SE_IT_SHIELD_BOUND);
            Duel_Sfx(actor, 0x3aa3);
            return;
        }
        if (hit && !bounced)
        {
            duel->phase = DUEL_RECOVER;
            keeta->laughed = 1;
            Duel_Sfx(actor, 0x3aa4);
            Keeta_Play(duel, 0x6c48, ANIMMODE_ONCE);
            return;
        }
    }
    if (duel->phase == DUEL_ATTACK)
    {
        if (finished)
            Keeta_ResumeWalk(keeta);
        else
            Keeta_RegisterBlade(keeta, play, player);
        return;
    }
    if (duel->phase == DUEL_HURT)
    {
        Vec3f from = actor->world.pos;
        from.y += 55.f;
        Vec3f next = { from.x + keeta->recoil.x, from.y, from.z + keeta->recoil.z };
        CollisionPoly* floor = NULL;
        s32 bgId;
        f32 height = BgCheck_EntityRaycastDown4(&play->colCtx, &floor, &bgId, actor, &next);
        if (floor && fabsf(height - actor->world.pos.y) <= 30.f && enemyFlightClear(play, &from, &next))
        {
            actor->world.pos.x = next.x;
            actor->world.pos.z = next.z;
        }
        else
        {
            keeta->recoil.x = keeta->recoil.z = 0.f;
        }
        Math_StepToF(&keeta->recoil.x, 0.f, 2.f);
        Math_StepToF(&keeta->recoil.z, 0.f, 2.f);
        if (finished && !(actor->colChkInfo.health < 10 && Keeta_TrySlam(keeta, play)))
            Keeta_ResumeWalk(keeta);
        return;
    }
    if (duel->phase != DUEL_APPROACH)
    {
        if (keeta->laughed ? finished : !duel->timer)
            Keeta_ResumeWalk(keeta);
        return;
    }
    if (duel->walking != 1)
    {
        duel->walking = 1;
        Keeta_Play(duel, 0x0ff0, ANIMMODE_LOOP);
    }
    Math_SmoothStepToS(&actor->world.rot.y, actor->yawTowardsPlayer, 1, 0xbb8, 0);
    actor->shape.rot.y = actor->world.rot.y;
    f32 height = fabsf(actor->yDistanceFromLink);
    if (!duel->timer && height >= 100.f && actor->xzDistToPlayer < 500.f && Keeta_TrySlam(keeta, play))
        return;
    s16 yaw = actor->yawTowardsPlayer - actor->shape.rot.y;
    if (!duel->timer && height < 20.f && actor->xzDistToPlayer < 180.f && ABS(yaw) < 0x11c7)
    {
        duel->phase = DUEL_ATTACK;
        Duel_Sfx(actor, 0x3aaa);
        Keeta_Play(duel, 0x0c50, ANIMMODE_ONCE);
        return;
    }
    if (actor->xzDistToPlayer < 500.f && Keeta_SafeStep(actor, play, 24.f))
        actor->speed = 2.f;
    int firstFoot = Animation_OnFrame(&duel->skel, 4.f) || Animation_OnFrame(&duel->skel, 8.f);
    if (firstFoot || Animation_OnFrame(&duel->skel, 2.f) || Animation_OnFrame(&duel->skel, 6.f))
    {
        Duel_Sfx(actor, 0x3aa0);
        if (actor->speed)
        {
            Keeta_Quake(play, actor, 1, 2);
            int foot = !firstFoot;
            if ((keeta->footPoseMask & (1 << foot)) && play->gameplayFrames - keeta->bodyPoseFrame <= 1 &&
                (actor->bgCheckFlags & BGCHECKFLAG_GROUND))
            {
                Vec3f pos = keeta->footPos[foot];
                pos.y = actor->floorHeight;
                Actor_SpawnFloorDustRing(play, actor, &pos, 20.f, 1, 5.f, 300, 10, 1);
            }
        }
    }
    Keeta_RegisterBlade(keeta, play, player);
}

static Vec3f Wart_ShellPosition(StandaloneWart* wart, int index)
{
    static const struct {
        u8 count;
        s8 x;
        u8 radius;
    } rings[] = {
        { 16, 0, 100 }, { 15, 38, 92 }, { 15, -38, 92 }, { 11, 71, 71 }, { 11, -71, 71 },
        { 6, 92, 38 },  { 6, -92, 38 }, { 1, 100, 0 },   { 1, -100, 0 },
    };
    int ring = 0;
    while (index >= rings[ring].count)
        index -= rings[ring++].count;
    Actor* actor = &wart->duel.actor;
    s16 pitch = wart->shellPitch + (index + 1) * (65536 / rings[ring].count);
    f32 z = Math_CosS(pitch) * rings[ring].radius;
    f32 x = rings[ring].x;
    f32 cosYaw = Math_CosS(wart->shellYaw);
    f32 sinYaw = Math_SinS(wart->shellYaw);
    f32 shellX = (x * cosYaw + z * sinYaw) * actor->scale.x * 13.f;
    f32 shellZ = (-x * sinYaw + z * cosYaw) * actor->scale.z * 16.f + 10.f;
    cosYaw = Math_CosS(actor->shape.rot.y);
    sinYaw = Math_SinS(actor->shape.rot.y);
    return (Vec3f){
        actor->world.pos.x + shellX * cosYaw + shellZ * sinYaw,
        actor->world.pos.y + actor->shape.yOffset * actor->scale.y -
            Math_SinS(pitch) * rings[ring].radius * actor->scale.y * 13.f,
        actor->world.pos.z - shellX * sinYaw + shellZ * cosYaw,
    };
}

static void Wart_PopBubble(WartBubble* bubble, PlayState* play)
{
    Color_RGBA8 prim = { 255, 255, 255, 255 };
    Color_RGBA8 env = { 255, 100, 100, 255 };
    for (int particle = 0; particle < 15; ++particle)
    {
        s16 yaw = (s16)(Rand_ZeroOne() * 65536.f);
        s16 pitch = (s16)(Rand_ZeroOne() * 65536.f);
        f32 speed = Rand_ZeroOne() * 10.f + 5.f;
        f32 horizontal = speed * Math_CosS(pitch);
        Vec3f velocity = { Math_SinS(yaw) * horizontal, -Math_SinS(pitch) * speed, Math_CosS(yaw) * horizontal };
        Vec3f accel = { velocity.x * -.03f, velocity.y * -.03f, velocity.z * -.03f };
        Vec3f pos = { bubble->pos.x + velocity.x, bubble->pos.y + velocity.y, bubble->pos.z + velocity.z };
        EffectSsDtBubble_SpawnCustomColor(play, &pos, &velocity, &accel, &prim, &env, 200 + Rand_ZeroOne() * 100.f,
                                          15 + Rand_ZeroOne() * 5.f, 0);
    }
}

static void Wart_Defeat(StandaloneWart* wart, PlayState* play)
{
    StandaloneDuel* duel = &wart->duel;
    Actor* actor = &duel->actor;
    if (duel->timer >= 45)
    {
        f32 scale = sForms[DUEL_WART].scale;
        s16 angle = (duel->timer - 40) * 0x3000;
        f32 burst = duel->timer == 45 ? 1.3f : 1.f;
        actor->scale.x = scale * (1.f + .1f * Math_CosS(angle)) * burst;
        actor->scale.y = scale * (1.f + .1f * Math_SinS(angle)) * burst;
        actor->scale.z = actor->scale.x;
    }
    if (duel->timer == 45 || duel->timer == 42)
    {
#if defined(GAME_MM)
        Vec3f* pos = &actor->world.pos;
        Actor_Spawn(&play->actorCtx, play, ACTOR_EN_CLEAR_TAG, pos->x, pos->y, pos->z,
                    0, 0, 0, CLEAR_TAG_LARGE_EXPLOSION);
        Actor_PlaySfx(actor, NA_SE_IT_BIG_BOMB_EXPLOSION);
#else
        Vec3f velocity = { 0.f, 0.f, 0.f };
        Vec3f accel = { 0.f, .1f, 0.f };
        EffectSsBomb2_SpawnLayered(play, &actor->world.pos, &velocity, &accel, 160, 22);
        EffectSsBlast_SpawnWhiteShockwave(play, &actor->world.pos, &velocity, &velocity);
        Actor_PlaySfx(actor, NA_SE_IT_BOMB_EXPLOSION);
#endif
    }
    if (duel->timer == 99)
    {
        Wart_ReleaseHook(wart, play);
        for (int i = 0; i < WART_BUBBLE_COUNT; ++i)
        {
            if (wart->bubbles[i].mode != 2)
                Wart_PopBubble(&wart->bubbles[i], play);
            wart->bubbles[i].mode = 2;
        }
    }
    if (duel->timer == 43)
        Actor_SetScale(actor, 0.f);
    if (!duel->timer)
        Actor_Kill(actor);
}

static int Wart_BubbleTakesDamage(u32 flags)
{
#if defined(GAME_MM)
    return flags & 0x89c7ff3b;
#else
    return flags & (DMG_DEKU_NUT | DMG_DEKU_STICK | DMG_SLINGSHOT | DMG_EXPLOSIVE | DMG_BOOMERANG | DMG_ARROW |
                    DMG_SWORD | DMG_HAMMER | DMG_MAGIC_FIRE | DMG_MAGIC_ICE);
#endif
}

static void Wart_UpdateBubbles(StandaloneWart* wart, PlayState* play)
{
    StandaloneDuel* duel = &wart->duel;
    Actor* actor = &duel->actor;
    ++wart->phaseFrames;
    if (actor->colChkInfo.health > 10 && wart->phaseFrames >= 0x500)
    {
        wart->phaseFrames -= 0x300;
        if (wart->detachRequests < WART_BUBBLE_COUNT)
            ++wart->detachRequests;
    }
    wart->shellPitch += 60 + wart->popped * 4;
    wart->shellYaw += 93 + wart->popped * 4;
    Vec3f player = GET_PLAYER(play)->actor.world.pos;
    for (int i = 0; i < WART_BUBBLE_COUNT; ++i)
    {
        WartBubble* bubble = &wart->bubbles[i];
        ColliderCylinder* receive = &wart->bubbleReceive[i];
        ColliderCylinder* attack = &wart->bubbleAttack[i];
        ColliderElement* element = &receive->elem;
        if (bubble->hitCooldown)
            --bubble->hitCooldown;
        f32 playerDx = bubble->pos.x - player.x;
        f32 playerDz = bubble->pos.z - player.z;
        f32 playerXZSq = playerDx * playerDx + playerDz * playerDz;
        if (bubble->contactCooldown && playerXZSq < 100.f * 100.f)
            --bubble->contactCooldown;
        if (wart->hookedBubble == i + 1)
        {
            bubble->pos = wart->hookTarget.world.pos;
            f32 dx = bubble->pos.x - player.x;
            f32 dz = bubble->pos.z - player.z;
            if (!(wart->hookTarget.flags & WART_HOOK_ATTACHED) || dx * dx + dz * dz < 80.f * 80.f)
            {
                Wart_ReleaseHook(wart, play);
                bubble->contactCooldown = 25;
                bubble->targetMode = WART_BUBBLE_RETURN;
            }
        }
        if (bubble->landingStretch)
            --bubble->landingStretch;
        if (bubble->returnDelay)
            --bubble->returnDelay;
        int hook = (element->acElemFlags & ACELEM_HIT) && Wart_HookBubble(wart, element, i);
        int hit = (element->acElemFlags & ACELEM_HIT) && (!bubble->hitCooldown || hook);
        element->acElemFlags &= ~ACELEM_HIT;
        receive->base.acFlags &= ~AC_HIT;
        int detach =
            bubble->mode == 0 && (actor->colChkInfo.health <= 10 || (wart->detachRequests && Rand_ZeroOne() < .1f));
        if (hit || detach)
        {
            if (bubble->mode == 0)
            {
                if (wart->detachRequests)
                    --wart->detachRequests;
                bubble->mode = 1;
                bubble->hitCooldown = 15;
                bubble->returnDelay = 200 + (u16)(Rand_ZeroOne() * 100.f);
                bubble->yaw = Math_Vec3f_Yaw(&actor->world.pos, &bubble->pos);
                bubble->velocity = (Vec3f){ 10.f * Math_SinS(bubble->yaw), 0.f, 10.f * Math_CosS(bubble->yaw) };
                bubble->floor = actor->floorHeight + 20.f;
                bubble->targetMode = hook ? WART_BUBBLE_RETURN : WART_BUBBLE_CHASE;
                --duel->shield;
                if (hit)
                    Duel_Sfx(actor, 0x3a11);
            }
            else if (bubble->mode == 1)
            {
                u32 flags = element->acHitElem ? element->acHitElem->atDmgInfo.dmgFlags : 0;
                if (!hook && !(flags & (1u << 7)))
                {
                    bubble->hitCooldown = flags & 0x300000 ? 10 : 15;
                    bubble->pause = 0;
                    bubble->yaw = Math_Vec3f_Yaw(&bubble->pos, &player) + 0x8000;
                    f32 knockback = flags & 0x300000 ? 10.f : 20.f;
                    bubble->velocity.x = Math_SinS(bubble->yaw) * knockback;
                    bubble->velocity.z = Math_CosS(bubble->yaw) * knockback;
                    if (!(flags & 0x300000))
                    {
                        bubble->mode = Wart_BubbleTakesDamage(flags) ? 3 : 4;
                        Duel_Sfx(actor, bubble->mode == 3 ? 0x3a12 : 0x3a11);
                    }
                }
            }
        }
        if (bubble->mode == 2)
            continue;
        bubble->spin += bubble->spinSpeed;
        if (bubble->mode == 0)
        {
            Vec3f shell = Wart_ShellPosition(wart, i);
            if (bubble->attachFraction == 1.f || bubble->scaleX == 0.f)
            {
                bubble->pos = shell;
                bubble->attachFraction = 1.f;
            }
            else
            {
                Math_ApproachF(&bubble->pos.x, shell.x, bubble->attachFraction, bubble->attachSpeed);
                Math_ApproachF(&bubble->pos.y, shell.y, bubble->attachFraction, bubble->attachSpeed);
                Math_ApproachF(&bubble->pos.z, shell.z, bubble->attachFraction, bubble->attachSpeed);
                if (!bubble->attachAnnounced &&
                    fabsf(bubble->pos.x - shell.x) + fabsf(bubble->pos.y - shell.y) + fabsf(bubble->pos.z - shell.z) <
                        2.f)
                {
                    bubble->attachAnnounced = 1;
                    Duel_Sfx(actor, 0x38d4);
                }
                Math_StepToF(&bubble->attachFraction, 1.f, .02f);
                Math_StepToF(&bubble->attachSpeed, 20.f, 1.f);
            }
        }
        else if (wart->hookedBubble == i + 1)
        {
            bubble->velocity = (Vec3f){ 0.f, 0.f, 0.f };
            bubble->pause = 0;
        }
        else if (bubble->pause && --bubble->pause)
        {
            if (bubble->pause == 1)
            {
                f32 dx = bubble->pos.x - actor->world.pos.x;
                f32 dz = bubble->pos.z - actor->world.pos.z;
                if (actor->colChkInfo.health > 10 && bubble->targetMode != WART_BUBBLE_WANDER &&
                    dx * dx + dz * dz < 120.f * 120.f)
                {
                    bubble->mode = 0;
                    bubble->targetMode = WART_BUBBLE_WANDER;
                    bubble->attachFraction = bubble->attachSpeed = 0.f;
                    bubble->attachAnnounced = 0;
                    ++duel->shield;
                }
                else
                {
                    bubble->landingStretch = 10;
                    Duel_Sfx(actor, 0x3a0f);
                }
            }
        }
        else
        {
            Vec3f next = {
                bubble->pos.x + bubble->velocity.x,
                bubble->pos.y + bubble->velocity.y,
                bubble->pos.z + bubble->velocity.z,
            };
            if (((wart->shellBoundMask & 1) && next.x < wart->shellBounds[0]) ||
                ((wart->shellBoundMask & 2) && next.x > wart->shellBounds[1]))
            {
                next.x = bubble->pos.x;
                bubble->velocity.x = -bubble->velocity.x;
            }
            if (((wart->shellBoundMask & 4) && next.z < wart->shellBounds[2]) ||
                ((wart->shellBoundMask & 8) && next.z > wart->shellBounds[3]))
            {
                next.z = bubble->pos.z;
                bubble->velocity.z = -bubble->velocity.z;
            }
            Vec3f hit;
            CollisionPoly* poly = NULL;
            s32 bgId;
            if (BgCheck_EntityLineTest1(&play->colCtx, &bubble->pos, &next, &hit, &poly, 1, 0, 0, 0, &bgId))
            {
                f32 nx = poly->normal.x * (1.f / 32767.f);
                f32 nz = poly->normal.z * (1.f / 32767.f);
                f32 dot = bubble->velocity.x * nx + bubble->velocity.z * nz;
                bubble->velocity.x -= 2.f * dot * nx;
                bubble->velocity.z -= 2.f * dot * nz;
                next.x = bubble->pos.x;
                next.z = bubble->pos.z;
            }
            if (bubble->velocity.y <= 0.f && next.y <= bubble->floor + 28.f)
            {
                Vec3f probe = next;
                probe.y = bubble->pos.y + 30.f;
                poly = NULL;
                f32 y = BgCheck_EntityRaycastDown4(&play->colCtx, &poly, &bgId, actor, &probe);
                if (poly && y > -30000.f)
                    bubble->floor = y + 20.f;
            }
            if (next.y <= bubble->floor)
            {
                if (bubble->mode == 3)
                {
                    bubble->pos = next;
                    bubble->pos.y = bubble->floor;
                    bubble->mode = 2;
                    ++wart->popped;
                    Wart_PopBubble(bubble, play);
                    continue;
                }
                if (bubble->mode == 4)
                {
                    next.y = bubble->floor;
                    bubble->velocity.y = 0.f;
                    bubble->velocity.x *= .5f;
                    bubble->velocity.z *= .5f;
                    Math_ApproachF(&bubble->scaleX, 1.2f, .75f, 1.5f);
                    Math_ApproachF(&bubble->scaleY, .75f, .75f, 1.5f);
                    if (fabsf(bubble->velocity.x) < .1f && fabsf(bubble->velocity.z) < .1f)
                    {
                        bubble->mode = 1;
                        bubble->targetMode = WART_BUBBLE_RETURN;
                    }
                    bubble->pos = next;
                    continue;
                }
                next.y = bubble->floor;
                bubble->pause = 5;
                Duel_Sfx(actor, 0x3a10);
                f32 speed;
                if (bubble->targetMode == WART_BUBBLE_RETURN)
                {
                    bubble->yaw = Math_Vec3f_Yaw(&next, &actor->world.pos);
                    speed = MIN(
                        10.f,
                        sqrtf(bubble->velocity.x * bubble->velocity.x + bubble->velocity.z * bubble->velocity.z) + 2.f);
                }
                else if (bubble->targetMode == WART_BUBBLE_CHASE)
                {
                    bubble->yaw = Math_Vec3f_Yaw(&next, &player) + (s16)(Rand_ZeroOne() * 20000.f);
                    speed = 7.f + Rand_ZeroOne() * 7.f;
                    if (actor->colChkInfo.health > 10 && !bubble->returnDelay)
                        bubble->targetMode = WART_BUBBLE_RETURN;
                }
                else
                {
                    bubble->yaw = Rand_ZeroOne() < .2f ? Math_Vec3f_Yaw(&next, &actor->world.pos)
                                                       : (s16)(Rand_ZeroOne() * 65536.f);
                    speed = 5.f + Rand_ZeroOne() * 5.f;
                }
                bubble->velocity.x = Math_SinS(bubble->yaw) * speed;
                bubble->velocity.y = 12.f + Rand_ZeroOne() * 5.f;
                bubble->velocity.z = Math_CosS(bubble->yaw) * speed;
            }
            else
            {
                bubble->velocity.y -= 2.f;
            }
            bubble->pos = next;
        }
        if (bubble->pause)
        {
            Math_ApproachF(&bubble->scaleX, 1.2f, .75f, 1.5f);
            Math_ApproachF(&bubble->scaleY, .75f, .75f, 1.5f);
        }
        else if (bubble->landingStretch)
        {
            Math_ApproachF(&bubble->scaleX, .75f, .5f, 1.f);
            Math_ApproachF(&bubble->scaleY, 1.2f, .5f, 1.f);
        }
        else
        {
            Math_ApproachF(&bubble->scaleX, 1.f, .1f, .2f);
            Math_ApproachF(&bubble->scaleY, 1.f, .1f, .2f);
        }
        if (bubble->mode == 3)
            continue;
        s16 angle = Math_Vec3f_Yaw(&actor->world.pos, &bubble->pos) - actor->yawTowardsPlayer;
        if (bubble->mode == 0 && (angle < -0x3000 || angle > 0x3000))
            continue;
        Vec3s position = { bubble->pos.x, bubble->pos.y, bubble->pos.z };
        receive->dim.pos = position;
        receive->dim.radius = bubble->mode == 0 ? 25 : 30;
        receive->dim.height = bubble->mode == 0 ? 46 : 50;
        receive->dim.yShift = bubble->mode == 0 ? -23 : -25;
        Duel_AC(play, &play->colChkCtx, &receive->base);
        f32 dx = bubble->pos.x - player.x;
        f32 dz = bubble->pos.z - player.z;
        if (!bubble->contactCooldown && dx * dx + dz * dz < 100.f * 100.f)
        {
            attack->dim.pos = position;
            CollisionCheck_SetAT(play, &play->colChkCtx, &attack->base);
        }
    }
}

static void Wart_SelectHoverPoint(StandaloneWart* wart, PlayState* play)
{
    Actor* actor = &wart->duel.actor;
    wart->turnTimer = 50 + (u8)(Rand_ZeroOne() * 100.f);
    wart->hoverSpeed = Rand_ZeroOne() * 3.f;
    Vec3f point = GET_PLAYER(play)->actor.world.pos;
    if (Rand_ZeroOne() >= .1f)
    {
        f32 centerX =
            (wart->shellBoundMask & 3) == 3 ? (wart->shellBounds[0] + wart->shellBounds[1]) * .5f : actor->home.pos.x;
        f32 centerZ =
            (wart->shellBoundMask & 12) == 12 ? (wart->shellBounds[2] + wart->shellBounds[3]) * .5f : actor->home.pos.z;
        point.x = centerX + Rand_ZeroOne() * 600.f - 300.f;
        point.z = centerZ + Rand_ZeroOne() * 600.f - 300.f;
    }
    point.y = actor->world.pos.y;
    if (wart->shellBoundMask & 1)
        point.x = MAX(point.x, wart->shellBounds[0]);
    if (wart->shellBoundMask & 2)
        point.x = MIN(point.x, wart->shellBounds[1]);
    if (wart->shellBoundMask & 4)
        point.z = MAX(point.z, wart->shellBounds[2]);
    if (wart->shellBoundMask & 8)
        point.z = MIN(point.z, wart->shellBounds[3]);
    CollisionPoly* poly;
    s32 bgId;
    Vec3f hit;
    if (BgCheck_EntityLineTest1(&play->colCtx, &actor->world.pos, &point, &hit, &poly, 1, 0, 0, 0, &bgId))
    {
        point.x = (actor->world.pos.x + hit.x) * .5f;
        point.z = (actor->world.pos.z + hit.z) * .5f;
    }
    if (actor->bgCheckFlags & BGCHECKFLAG_WALL)
    {
        point.x = actor->world.pos.x - 60.f * Math_SinS(actor->world.rot.y);
        point.z = actor->world.pos.z - 60.f * Math_CosS(actor->world.rot.y);
        if (BgCheck_EntityLineTest1(&play->colCtx, &actor->world.pos, &point, &hit, &poly, 1, 0, 0, 0, &bgId))
            wart->hoverSpeed = 0.f;
    }
    wart->hoverTarget = point;
}

static void Wart_RollWater(StandaloneWart* wart, PlayState* play, int impact)
{
    Actor* actor = &wart->duel.actor;
    Vec3f pos = actor->world.pos;
    f32 waterY = pos.y;
    WaterBox* box;
    if (!WaterBox_GetSurface1_2(play, &play->colCtx, pos.x, pos.z, &waterY, &box) || waterY <= actor->floorHeight ||
        fabsf(waterY - pos.y) > 75.f)
        return;
    pos.y = waterY + 2.f;
    EffectSsGRipple_Spawn(play, &pos, 1400, 500, 0);
    if (impact)
        EffectSsGSplash_Spawn(play, &pos, NULL, NULL, 0, 500);
}

static void Wart_FollowTerrain(StandaloneWart* wart, PlayState* play)
{
    Actor* actor = &wart->duel.actor;
    if (actor->colChkInfo.health <= 10)
        return;
    Vec3f probe = actor->world.pos;
    probe.y = MAX(actor->world.pos.y, actor->prevPos.y) + 80.f;
    CollisionPoly* poly = NULL;
    s32 bgId;
#if defined(GAME_MM)
    f32 floorY = BgCheck_EntityRaycastFloor5_2(play, &play->colCtx, &poly, &bgId, actor, &probe);
#else
    f32 floorY = BgCheck_EntityRaycastDown4(&play->colCtx, &poly, &bgId, actor, &probe);
#endif
    if (!poly || floorY <= -30000.f || floorY >= 30000.f)
    {
        actor->world.pos.x = actor->prevPos.x;
        actor->world.pos.z = actor->prevPos.z;
        wart->turnTimer = 0;
        return;
    }
    actor->floorHeight = floorY;
    actor->floorPoly = poly;
    actor->floorBgId = bgId;
    Vec3f floor = { actor->world.pos.x, floorY + 12.f, actor->world.pos.z };
    Vec3f ceiling = { floor.x, floorY + 220.f, floor.z }, hit;

    f32 height = floorY + 120.f + 7.5f * Math_SinS(wart->phaseFrames * 512);
    if (BgCheck_EntityLineTest1(&play->colCtx, &floor, &ceiling, &hit, &poly, 0, 0, 1, 0, &bgId))
        height = MIN(height, hit.y - 50.f);
    height = MAX(height, floorY + 50.f);
    Math_ApproachF(&actor->world.pos.y, height, .25f, 8.f);
    actor->world.pos.y = MAX(actor->world.pos.y, floorY + 50.f);
}

static void Wart_Move(StandaloneWart* wart, PlayState* play)
{
    StandaloneDuel* duel = &wart->duel;
    Actor* actor = &duel->actor;
    int hurt = duel->phase == DUEL_HURT && duel->timer;
    if (wart->eyeTimer)
        --wart->eyeTimer;
    if (hurt && duel->timer == 10)
    {
        wart->eyeTimer = 60 + (u8)(Rand_ZeroOne() * 100.f);
        duel->walking = 0;
    }
    else if (!wart->eyeTimer)
    {
        wart->eyeTimer = 60 + (u8)(Rand_ZeroOne() * 100.f);
        duel->walking ^= 1;
    }
    Math_ApproachF(&wart->eyelid, duel->walking ? 10000.f : 0.f, .2f, 1000.f);
    s16 yaw = actor->yawTowardsPlayer - actor->shape.rot.y;
    Math_ApproachS(&wart->eyeYaw, ABS(yaw) < 0x6000 ? CLAMP(yaw, -0x2500, 0x2500) : 0, 5, 0x800);
    if (wart->eyelid > 3000.f)
        actor->flags |= ACTOR_FLAG_ATTENTION_ENABLED;
    else
        actor->flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    if (!hurt)
        duel->phase = DUEL_APPROACH;
    Duel_Sfx(actor, 0x3a15 - SFX_FLAG);
    if (actor->colChkInfo.health > 10)
    {
        actor->shape.yOffset = 0.f;
        actor->gravity = 0.f;
        actor->velocity.y = 0.f;
        if (wart->turnTimer)
            --wart->turnTimer;
        if (!wart->turnTimer || (actor->bgCheckFlags & BGCHECKFLAG_WALL))
            Wart_SelectHoverPoint(wart, play);
        s16 targetYaw = Math_Vec3f_Yaw(&actor->world.pos, &wart->hoverTarget);
        Math_ApproachS(&actor->world.rot.y, targetYaw, 5, 0x200);
        Math_ApproachS(&actor->shape.rot.y, actor->yawTowardsPlayer, 10, 0x200);
        Math_StepToF(&actor->speed, wart->hoverSpeed, .5f);
    }
    else
    {
        if (!wart->rollStarted)
        {
            wart->rollStarted = 1;
            wart->rollDelay = 60;
            actor->world.rot.y = actor->shape.rot.y;
            actor->speed = 0.f;
        }
        actor->gravity = -3.f;
        actor->shape.yOffset = 900.f;
        if (wart->rollWallCooldown)
            --wart->rollWallCooldown;
        if (wart->rollDelay)
            --wart->rollDelay;
        if (wart->rollImpactPause)
            --wart->rollImpactPause;
        int wall = actor->bgCheckFlags & BGCHECKFLAG_WALL;
        if ((hurt && duel->timer == 14) || (wall && !wart->rollWallCooldown))
        {
            wart->rollWallCooldown = 20;
            if (Rand_ZeroOne() < .2f && !hurt)
            {
                actor->world.rot.y = actor->yawTowardsPlayer;
                wart->eyeTimer = 100;
                duel->walking = 1;
            }
            else
            {
                actor->world.rot.y += 0x8000 + (s16)(Rand_ZeroOne() * 8000.f);
            }
            actor->speed = 0.f;
            if (wall)
            {
                wart->rollImpactPause = 15;
                Duel_Sfx(actor, NA_SE_IT_BOMB_EXPLOSION);
                s16 quake = Quake_Request(Play_GetCamera(play, CAM_ID_MAIN), 3);
                Quake_SetSpeed(quake, 20000);
                Quake_SetPerturbations(quake, 15, 0, 0, 0);
                Quake_SetDuration(quake, 10);
                Rumble_Request(actor->xyzDistToPlayerSq, 180, 20, 100);
                Wart_RollWater(wart, play, 1);
            }
        }
        if (!wart->rollImpactPause)
        {
            Math_ApproachS(&actor->shape.rot.y, actor->world.rot.y, 5, 0x1000);
            if (!wart->rollDelay && (actor->bgCheckFlags & BGCHECKFLAG_GROUND))
            {
                Duel_Sfx(actor, 0x3a16 - SFX_FLAG);
                Math_StepToF(&actor->speed, 20.f, 1.f);
                if (!(wart->phaseFrames & 3))
                    Wart_RollWater(wart, play, 0);
            }
        }
        Math_ApproachS(&actor->shape.rot.x, Math_SinS(wart->phaseFrames * 0xfb8) * 5000.f, 5, 0x800);
        Math_ApproachS(&actor->shape.rot.z, Math_SinS(wart->phaseFrames * 0xcd0) * 3000.f, 5, 0x800);
    }
}

static s32 Wart_OverrideLimb(PlayState* play, s32 limb, Gfx** dList, Vec3f* pos, Vec3s* rot, void* arg)
{
    StandaloneWart* wart = arg;
    if (limb == 5 || limb == 7)
        rot->y += wart->eyelid - 0x500;
    if (limb == 3)
    {
        rot->y += 2000;
        rot->z += wart->eyeYaw;
        if (wart->duel.phase == DUEL_HURT)
        {
            rot->y += Math_SinS(play->gameplayFrames * 0x3000) * (wart->duel.timer * 500);
            rot->z += Math_SinS(play->gameplayFrames * 0x3500) * (wart->duel.timer * 300);
        }
    }
    return 0;
}

static void Wart_DrawLimb(PlayState* play, s32 limb, Gfx** dList, Vec3s* rot, void* arg)
{
    if (limb != 1)
        return;
    StandaloneWart* wart = arg;
    Collider_UpdateSpheres(limb, &wart->eye);
    Collider_UpdateSpheres(limb, &wart->shell);
    Vec3f focusOffset = { 0.f, -500.f, 0.f };
    Matrix_MultVec3f(&focusOffset, &wart->duel.actor.focus.pos);
    wart->collisionPoseFrame = play->gameplayFrames;
    wart->collisionPoseValid = 1;
}

static int Igos_WaitForKnights(StandaloneIgos* igos, PlayState* play)
{
    if (!igos->knightEncounter)
        return 0;
    Actor* actor = &igos->duel.actor;
    for (int i = 0; i < 2; ++i) {
        if (igos->knightsSpawned & (1 << i))
            continue;
        s16 yaw = actor->shape.rot.y + (i ? 0x2000 : -0x2000);
        Vec3f pos = actor->world.pos;
        pos.x += Math_SinS(yaw) * 70.f;
        pos.z += Math_CosS(yaw) * 70.f;
        pos.y += 60.f;
        CollisionPoly* floor = NULL;
#if defined(GAME_OOT)
        f32 y = BgCheck_EntityRaycastDown1(&play->colCtx, &floor, &pos);
#else
        f32 y = BgCheck_EntityRaycastFloor1(&play->colCtx, &floor, &pos);
#endif
        if (floor && fabsf(y - actor->world.pos.y) < 100.f)
            pos.y = y;
        else
            pos = actor->world.pos;
        Actor* knight = Actor_Spawn(&play->actorCtx, play, actor->id, pos.x, pos.y, pos.z, 0,
            actor->shape.rot.y, 0, i ? 0x827f : 0x817f);
        if (knight) {
            knight->parent = actor;
            knight->room = actor->room;
            igos->knightsSpawned |= 1 << i;
        }
    }
    int alive = igos->knightsSpawned != 3;
    for (int category = 0; category < ACTORCAT_MAX; ++category)
        for (Actor* knight = play->actorCtx.actors[category].first; knight; knight = knight->next)
            if (knight->parent == actor && knight->id == actor->id &&
                ((u16)knight->params == 0x817f || (u16)knight->params == 0x827f))
                alive = 1;
    if (!alive) {
        igos->knightEncounter = 0;
        actor->flags |= ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE;
        return 0;
    }
    actor->flags &= ~(ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE);
    actor->speed = 0.f;
    igos->duel.body.base.acFlags &= ~AC_HIT;
    igos->bodySpheres.base.acFlags &= ~AC_HIT;
    Actor_MoveWithGravity(actor);
    Actor_UpdateBgCheckInfo(play, actor, 24.f, 32.f, 65.f, 5);
    SkelAnime_Update(&igos->duel.skel);
    return 1;
}

static void Duel_Update(Actor* actor, PlayState* play)
{
    if (DUEL_FIXED_FORM == DUEL_IGOS && (u16)actor->params == 0x837f)
    {
        IgosFlyingHead* head = (IgosFlyingHead*)actor;
        if (head->headAction == IGOS_HEAD_ATTACHED && head->headTimer)
        {
            Igos_UpdateHeadTrail(head);
            if (!--head->headTimer)
                Actor_Kill(actor);
            return;
        }
        StandaloneIgos* owner = (StandaloneIgos*)actor->parent;
        if (!owner || !owner->duel.actor.update || owner->headTarget != actor)
        {
            Actor_Kill(actor);
        }
        else if (owner->duel.phase == DUEL_DEFEAT)
        {
            Igos_SetHeadTarget(owner, play, 0);
        }
        else
        {
            Igos_UpdateHead((IgosFlyingHead*)actor, owner, play);
        }
        return;
    }
    if (DUEL_FIXED_FORM == DUEL_IGOS && Igos_WaitForKnights((StandaloneIgos*)actor, play))
        return;
    StandaloneDuel* duel = (StandaloneDuel*)actor;
    const DuelForm* form = &sForms[DUEL_FIXED_FORM];
    int wallStunned = DUEL_FIXED_FORM == DUEL_SNAPPER &&
                      (duel->phase == GEKKO_WALL_CLIMB || duel->phase == GEKKO_WALL_PAUSE) &&
                      ((StandaloneSnapper*)duel)->damage.stun;
    f32 prevFrame = duel->skel.curFrame;
    int igosStunned = DUEL_FIXED_FORM == DUEL_IGOS && duel->walking == IGOS_STUN && duel->phase == DUEL_APPROACH;
    int garoCircle = DUEL_FIXED_FORM == DUEL_GARO && duel->phase == GARO_CIRCLE;
    int finished = wallStunned || igosStunned || garoCircle ? 0 : SkelAnime_Update(&duel->skel);
    if (DUEL_FIXED_FORM == DUEL_SLIME)
    {
        StandaloneSlime* slime = (StandaloneSlime*)duel;
        if (slime->dropWarningActive == 2)
            slime->dropWarningActive = 0;
        Slime_UpdateShatterEffects(slime);
    }
    if (DUEL_FIXED_FORM == DUEL_SNAPPER || DUEL_FIXED_FORM == DUEL_SLIME)
        Gekko_TickEffects(duel, play);
    if (duel->timer && !wallStunned)
        --duel->timer;
    if (duel->exposed)
        --duel->exposed;
    if (DUEL_FIXED_FORM == DUEL_IGOS)
    {
        Igos_UpdateElemental((StandaloneIgos*)duel, play);
        StandaloneIgos* igos = (StandaloneIgos*)duel;
        if (igos->lightRayTimer)
            --igos->lightRayTimer;
        if (igos->damageFlashTimer)
            --igos->damageFlashTimer;
        Math_StepToF(&igos->shieldFlashScale, 0.f, .2f);
        if ((u16)actor->params == 0x807f)
        {
            StandaloneIgos* igos = (StandaloneIgos*)duel;
            if (duel->phase == DUEL_DEFEAT)
                Igos_SetHeadTarget(igos, play, 0);
        }
        if (duel->phase == DUEL_IGOS_DOWN)
        {
            StandaloneIgos* igos = (StandaloneIgos*)duel;
            Igos_MoveFallen(igos, play);
            if (duel->timer == (igos->fallenBackwards ? 135 : 123))
            {
                actor->flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
                actor->flags |= ACTOR_FLAG_LOCK_ON_DISABLED;
                Duel_Sfx(actor, 0x387a);
                if (igos->elementalType == IGOS_EFFECT_ICE_QUIET)
                    igos->elementalTimer = 0;
            }
            u8 effect = actor->colChkInfo.damageEffect;
            if (igos->fallenHitTimer)
                --igos->fallenHitTimer;
            int hit = !igos->fallenHitTimer && (igos->bodySpheres.base.acFlags & AC_HIT);
            int light = hit && (effect == IGOS_DAMAGE_LIGHT || effect == IGOS_DAMAGE_RAY);
            if (hit)
                igos->fallenHitTimer = 10;
            igos->bodySpheres.base.acFlags &= ~AC_HIT;
            for (int i = 0; i < ARRAY_COUNT(igos->bodyElements); ++i)
                igos->bodyElements[i].base.acElemFlags &= ~ACELEM_HIT;
            if (light)
            {
                igos->damageFlashTimer = 15;
                actor->colChkInfo.health = 0;
                actor->flags &= ~(ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE);
                actor->flags |= ACTOR_FLAG_LOCK_ON_DISABLED;
                Player* player = GET_PLAYER(play);
#if defined(GAME_MM)
                if (player->lockOnActor == actor)
                    player->lockOnActor = NULL;
#else
                if (player->focusActor == actor)
                    ((void (*)(Player*))0x800798c4)(player);
#endif
                Igos_DeathSound(actor, 1);
                Igos_StartElemental(igos, IGOS_DAMAGE_LIGHT);
                duel->phase = DUEL_DEFEAT;
                duel->timer = 50;
            }
            else if (!duel->timer)
            {
                actor->flags &= ~ACTOR_FLAG_LOCK_ON_DISABLED;
                actor->colChkInfo.health = (u16)actor->params == 0x807f ? 14 : 6;
                actor->flags |= ACTOR_FLAG_ATTENTION_ENABLED;
                duel->phase = DUEL_APPROACH;
                Igos_Action(duel, IGOS_RECOVER, 35, 0xcde0, ANIMMODE_LOOP);
                igos->damageInvulnerability = 25;
                static const u16 getUpVoices[] = { 0x3a47, 0x3a43, 0x3a40 };
                Duel_Sfx(actor, getUpVoices[Igos_VoiceVariant(actor)]);
            }
            else if (igos->bodyPoseMask == 3 && play->gameplayFrames - igos->bodyPoseFrame <= 1)
            {
                Duel_AC(play, &play->colChkCtx, &igos->bodySpheres.base);
            }
            Igos_UpdateBodyEffects(igos, play);
            return;
        }
    }
    if (DUEL_FIXED_FORM == DUEL_WART && duel->phase != DUEL_DEFEAT)
        Wart_UpdateBubbles((StandaloneWart*)duel, play);
    if (DUEL_FIXED_FORM == DUEL_GOMESS)
    {
        StandaloneGomess* gomess = (StandaloneGomess*)duel;
        if (gomess->droppedScythe != 1 || gomess->deathStage < GOMESS_DEATH_DROP_SCYTHE)
            Gomess_ProjectScytheSound(gomess, play);
        Gomess_UpdateHitEffects(gomess, play);
    }
    if (duel->phase == DUEL_DEFEAT)
    {
        if (DUEL_FIXED_FORM == DUEL_GOMESS)
        {
            StandaloneGomess* gomess = (StandaloneGomess*)duel;
            Gomess_Defeat(gomess, play, finished);
            if (gomess->deathStage != GOMESS_DEATH_FADE)
                Gomess_RegisterBody(duel, play);
        }
        else if (DUEL_FIXED_FORM == DUEL_GARO)
            Garo_Defeat(duel, play);
        else if (DUEL_FIXED_FORM == DUEL_WART)
            Wart_Defeat((StandaloneWart*)duel, play);
        else if (DUEL_FIXED_FORM == DUEL_KEETA)
            Keeta_Defeat((StandaloneKeeta*)duel, play, finished);
        else if (DUEL_FIXED_FORM == DUEL_IGOS)
        {
            StandaloneIgos* igos = (StandaloneIgos*)duel;
            if (!duel->timer)
            {
                Actor_Kill(actor);
            }
            else
            {
                Math_ApproachF(&actor->scale.y, 0.f, .1f, .0015f);
                Igos_UpdateBodyEffects(igos, play);
                int part = Rand_ZeroOne() * (ARRAY_COUNT(igos->fallenParts) - .1f);
                f32 offsetX = (Rand_ZeroOne() - .5f) * 20.f;
                f32 offsetZ = (Rand_ZeroOne() - .5f) * 20.f;
                f32 scale = 20.f + Rand_ZeroOne() * 20.f;
                if ((igos->fallenPoseMask & (1 << part)) && play->gameplayFrames - igos->fallenPoseFrame <= 1)
                {
                    Vec3f pos = igos->fallenParts[part];
                    Vec3f velocity = { 0.f, 0.f, 0.f };
                    Vec3f accel = { 0.f, .03f, 0.f };
                    pos.x += offsetX;
                    pos.y -= 5.f;
                    pos.z += offsetZ;
                    EffectSsKFire_Spawn(play, &pos, &velocity, &accel, scale, 0);
                }
                Duel_Sfx(actor, 0x3a1f - SFX_FLAG);
            }
        }
        else if (DUEL_FIXED_FORM == DUEL_SLIME)
            Slime_Defeat((StandaloneSlime*)duel, play);
        else
        {
            if (!duel->timer)
            {
                if (DUEL_FIXED_FORM == DUEL_SNAPPER)
                {
                    EnemySnapperMount* mount = Snapper_GetMount(actor);
                    Gekko_DefeatEffects(actor, play);
                    if (mount)
                    {
                        actor->child = NULL;
                        mount->actor.parent = NULL;
                        mount->carrying = 0;
                        mount->mode = SNAPPER_MOUNT_DEFEAT;
                    }
                }
                Actor_Kill(actor);
            }
            else if (DUEL_FIXED_FORM == DUEL_SNAPPER)
            {
                actor->colorFilterTimer = 16;
                if (duel->timer <= 20)
                    Actor_SetScale(actor, form->scale * duel->timer / 20.f);
            }
            else if (duel->timer < 16)
                Actor_SetScale(actor, form->scale * duel->timer / 16.f);
        }
        return;
    }

    if (DUEL_FIXED_FORM == DUEL_SNAPPER && duel->phase >= GEKKO_MOUNT_LAND)
    {
        EnemySnapperMount* mount = Snapper_GetMount(actor);
        if (mount && (mount->mode == SNAPPER_MOUNT_FLIPPING || mount->mode == SNAPPER_MOUNT_OVERTURNED))
        {
            actor->world.pos.y += actor->shape.yOffset * actor->scale.y;
            actor->shape.yOffset = 0.f;
            actor->gravity = -1.f;
            actor->velocity.y = 15.f;
            actor->speed = 7.f;
            actor->bgCheckFlags &= ~BGCHECKFLAG_GROUND;
            actor->world.rot.y = actor->shape.rot.y = mount->actor.world.rot.y + 0x8000;
            duel->phase = GEKKO_DISMOUNT;
            duel->timer = 30;
            actor->flags |= ACTOR_FLAG_ATTENTION_ENABLED;
            Duel_Sfx(actor, sForms[DUEL_SNAPPER].hurtSfx);
            Animation_PlayOnce(&duel->skel, Duel_Anim(0x1f20));
        }
    }
    if (DUEL_FIXED_FORM == DUEL_SLIME)
    {
        StandaloneSlime* slime = (StandaloneSlime*)duel;
        if (slime->itemDropCooldown)
            --slime->itemDropCooldown;
        u32 damageFlags = 0;
        int shellHit = 0;
        for (int i = 0; i < ARRAY_COUNT(slime->shell); ++i)
        {
            ColliderCylinder* shell = &slime->shell[i];
            if (shell->base.acFlags & AC_HIT)
            {
                shellHit = 1;
                if ((shell->elem.acElemFlags & ACELEM_HIT) && shell->elem.acHitElem)
                    damageFlags |= shell->elem.acHitElem->atDmgInfo.dmgFlags;
            }
            shell->base.acFlags &= ~AC_HIT;
            shell->elem.acElemFlags &= ~ACELEM_HIT;
        }
        if (shellHit)
        {
            if (duel->phase == SLIME_FROZEN)
            {
                if (slime->frozenOnGround && slime->frozenReady)
                {
                    if (damageFlags & Slime_IceBreakMask())
                        Slime_Shatter(slime, play);
                    else if (damageFlags & (1u << 11))
                    {
                        slime->frozenMelting = 1;
                        slime->iceAlpha[0] = slime->iceAlpha[1] = 0;
                        for (int i = 0; i < 20; ++i)
                            slime->iceAlpha[i + 2] = 10 * i;
                        duel->timer = 100;
                    }
                }
            }
            else
            {

                if (damageFlags & (1u << 12))
                    actor->colChkInfo.damageEffect = GEKKO_DAMAGE_ICE;
                u8 effect = actor->colChkInfo.damageEffect;
                if (effect != GEKKO_DAMAGE_STUN && effect != GEKKO_DAMAGE_ICE && !slime->itemDropCooldown &&
                    actor->floorPoly)
                {
                    f32 roll = Rand_ZeroOne();
                    if (roll < .3f)
                    {
                        Vec3f drop = actor->world.pos;
                        drop.y = CLAMP(drop.y, actor->floorHeight + 10.f, actor->floorHeight + 60.f);
                        Item_DropCollectible(play, &drop, roll < .15f ? ITEM00_ARROWS_10 : ITEM00_MAGIC_SMALL);
                    }
                    slime->itemDropCooldown = 40;
                }
                duel->body.base.acFlags |= AC_HIT;
            }
        }
    }
    Collider* damageCollider = &duel->body.base;
    if (DUEL_FIXED_FORM == DUEL_IGOS)
        damageCollider = &((StandaloneIgos*)duel)->bodySpheres.base;
    if (DUEL_FIXED_FORM == DUEL_WART)
        damageCollider = &((StandaloneWart*)duel)->eye.base;
    if (DUEL_FIXED_FORM == DUEL_KEETA)
        damageCollider = &((StandaloneKeeta*)duel)->bodySpheres.base;
    if (DUEL_FIXED_FORM == DUEL_SNAPPER)
        damageCollider = &((StandaloneSnapper*)duel)->body.base;
    if (DUEL_FIXED_FORM == DUEL_KEETA)
    {
        StandaloneKeeta* keeta = (StandaloneKeeta*)duel;
        if (keeta->damageTimer && --keeta->damageTimer < 60 && keeta->damageEffect == 11)
            keeta->damageEffect = 10;
        if (duel->headCooldown)
        {
            damageCollider->acFlags &= ~(AC_HIT | AC_BOUNCED);
            if (!--duel->headCooldown)
                Keeta_EndStun(keeta, play);
            return;
        }
    }
    if (DUEL_FIXED_FORM == DUEL_SLIME && duel->phase > SLIME_FORM && duel->phase != SLIME_FROZEN &&
        (duel->body.base.acFlags & AC_HIT) && actor->colChkInfo.damageEffect == 3)
    {
        duel->body.base.acFlags &= ~AC_HIT;
        Igos_ReleasePlayer(actor, play);
        StandaloneSlime* slime = (StandaloneSlime*)duel;
        slime->frozenMinY = 32767;
        slime->frozenMaxY = -32768;
        for (int i = 0; i < ARRAY_COUNT(slime->throwMesh); ++i)
        {
            const s16* vertex = slime->shellMesh[i].n.ob;
            slime->throwMesh[i] = (Vec3s){ vertex[0], vertex[1], vertex[2] };
            slime->frozenMinY = MIN(slime->frozenMinY, vertex[1]);
            slime->frozenMaxY = MAX(slime->frozenMaxY, vertex[1]);
        }
        memset(slime->iceAlpha, 0, sizeof(slime->iceAlpha));
        for (int i = 0; i < 20; ++i)
            slime->iceAlpha[i + 138] = 10 * i;
        for (int i = 158; i < ARRAY_COUNT(slime->iceAlpha); ++i)
            slime->iceAlpha[i] = 200;
        slime->frozenOnGround =
            (actor->bgCheckFlags & BGCHECKFLAG_GROUND) || (duel->phase == SLIME_GROUND && actor->floorPoly);
        slime->frozenReady = slime->frozenMelting = slime->frozenFallHit = 0;
        slime->preFreezePhase = duel->phase;
        slime->preFreezeTimer = duel->timer;
        if (duel->phase == SLIME_GRAB || duel->phase == SLIME_THROW)
        {
            slime->preFreezePhase = SLIME_GROUND;
            slime->preFreezeTimer = 24;
        }
        duel->phase = SLIME_FROZEN;
        duel->timer = 40;
        actor->speed = 0.f;
        if (slime->frozenOnGround)
        {
            actor->velocity.y = 0.f;
            actor->gravity = 0.f;
        }
        else if (slime->preFreezePhase == SLIME_RISE && slime->riseCounter < 2)
        {
            actor->gravity = -2.f;
        }
    }
    if (DUEL_FIXED_FORM == DUEL_GARO)
    {
        StandaloneGaroMaster* garo = (StandaloneGaroMaster*)duel;
        if (garo->damageTimer)
        {
            --garo->damageTimer;
            if (garo->damageEffect == 11 && garo->damageTimer < 60)
                garo->damageEffect = 10;
        }
    }
    if (DUEL_FIXED_FORM == DUEL_IGOS)
    {
        StandaloneIgos* igos = (StandaloneIgos*)duel;
        if (igos->shieldFxTimer)
            --igos->shieldFxTimer;
        if (igos->shieldElement.base.acElemFlags & ACELEM_HIT)
        {
            igos->shieldElement.base.acElemFlags &= ~ACELEM_HIT;
            Player* player = GET_PLAYER(play);
            if (!igos->shieldFxTimer && player->meleeWeaponState &&
                player->meleeWeaponAnimation >= PLAYER_MWA_SPIN_ATTACK_1H)
            {
                Vec3f impact = igos->shieldFlashPos;
                igos->shieldFxTimer = 5;
                Actor_PlaySfx(actor, NA_SE_IT_SHIELD_REFLECT_SW);
#if defined(GAME_OOT)
                ((void (*)(PlayState*, Vec3f*))0x800511b8)(play, &impact);
#else
                CollisionCheck_SpawnShieldParticlesMetal(play, &impact);
#endif
            }
            igos->shieldInvulnerability = 5;
            Igos_SetHitDrift(igos, (u16)actor->params == 0x807f ? -6.f : -4.f);
        }
        if (igos->damageInvulnerability)
            --igos->damageInvulnerability;
        if (igos->shieldInvulnerability || igos->damageInvulnerability)
            damageCollider->acFlags &= ~(AC_HIT | AC_BOUNCED);
    }
    if (DUEL_FIXED_FORM == DUEL_GOMESS)
        damageCollider = &((StandaloneGomess*)duel)->core.base;
    if (damageCollider->acFlags & (DUEL_FIXED_FORM == DUEL_GOMESS ? AC_HIT : (AC_HIT | AC_BOUNCED)))
    {
        damageCollider->acFlags &= ~(AC_HIT | AC_BOUNCED);
        int vulnerable = DUEL_FIXED_FORM == DUEL_GOMESS  ? Gomess_ReceiveHit((StandaloneGomess*)duel, play)
                         : DUEL_FIXED_FORM == DUEL_KEETA ? Keeta_ReceiveHit((StandaloneKeeta*)duel)
                                                         : Duel_CanTakeHit(duel, actor->colChkInfo.damageEffect);
        if (DUEL_FIXED_FORM == DUEL_KEETA && duel->headCooldown)
            return;
        if (DUEL_FIXED_FORM == DUEL_IGOS &&
            (duel->walking == IGOS_REATTACH || (duel->walking == IGOS_BACKJUMP && duel->timer) ||
             (duel->walking == IGOS_BLOCK && actor->colChkInfo.damageEffect != 4 &&
              (((StandaloneIgos*)duel)->shield.base.acFlags & (AC_HIT | AC_BOUNCED)))))
            vulnerable = 0;
        if (DUEL_FIXED_FORM == DUEL_GARO)
        {
            StandaloneGaroMaster* garo = (StandaloneGaroMaster*)duel;
            u8 effect = actor->colChkInfo.damageEffect;
            int frozen = garo->damageTimer && (garo->damageEffect == 10 || garo->damageEffect == 11);
            int stunHit = effect == GARO_DAMAGE_STUN || effect == GARO_DAMAGE_ELECTRIC;
            if (stunHit && !frozen && duel->phase != DUEL_HURT && duel->phase != GARO_RETREAT &&
                duel->phase != GARO_SPIN && duel->phase != GARO_VANISH && duel->phase != GARO_APPEAR)
            {
                actor->shape.rot.x = 0;
                actor->world.rot.x = 0;
                actor->flags &= ~ACTOR_FLAG_LOCK_ON_DISABLED;
                if (duel->phase == GARO_FALL)
                {
                    Actor_SetScale(actor, sForms[DUEL_GARO].scale);
                    actor->shape.shadowScale = 17.f;
                }
                Garo_StopDashAudio(garo);
                Duel_Sfx(actor, 0x389e);
                Actor_SetColorFilter(actor, 0, 255, 0, 40);
                Garo_SetKnockback(garo, 10.f);
                Garo_Change(duel, GARO_STUN, 40, sForms[DUEL_GARO].hurt, ANIMMODE_LOOP);
                if (effect == GARO_DAMAGE_ELECTRIC)
                {
                    garo->damageEffect = 32;
                    garo->damageTimer = 40;
                    garo->damageScale = .5f;
                }
            }
            else if (!stunHit && duel->phase == GARO_CIRCLE)
            {
                actor->speed = 0.f;
                actor->world.rot.y = actor->yawTowardsPlayer;
                Actor_PlaySfx(actor, NA_SE_IT_SHIELD_BOUND);
                Garo_Change(duel, GARO_GUARD, duel->timer, 0x4858, ANIMMODE_ONCE);
            }
            vulnerable =
                !stunHit && (duel->phase == GARO_SLASH || duel->phase == GARO_SWING || duel->phase == GARO_STUN);
            if (frozen && (effect == GARO_DAMAGE_ICE || effect == GARO_DAMAGE_LIGHT))
                vulnerable = 0;
            if (vulnerable && actor->colChkInfo.damage)
            {
                if (frozen)
                    Garo_ShatterIce(garo, play);
                if (effect == GARO_DAMAGE_FIRE || effect == GARO_DAMAGE_LIGHT || effect == GARO_DAMAGE_ICE)
                {
                    garo->damageEffect = effect == GARO_DAMAGE_FIRE ? 0 : effect == GARO_DAMAGE_LIGHT ? 20 : 11;
                    garo->damageTimer = effect == GARO_DAMAGE_FIRE ? 40 : effect == GARO_DAMAGE_LIGHT ? 20 : 80;
                    garo->damageScale = effect == GARO_DAMAGE_ICE ? 0.f : .5f;
                    garo->steamScale = 1.5f;
                    if (effect == GARO_DAMAGE_LIGHT)
                    {
                        Vec3f* pos = &actor->focus.pos;
                        Actor_Spawn(&play->actorCtx, play, ACTOR_EN_CLEAR_TAG, pos->x, pos->y, pos->z, 0, 0, 0, 4);
                    }
                }
            }
        }
        if (DUEL_FIXED_FORM == DUEL_SNAPPER && !Snapper_ReceivesHits((StandaloneSnapper*)duel))
            vulnerable = 0;
        if ((DUEL_FIXED_FORM == DUEL_SNAPPER || DUEL_FIXED_FORM == DUEL_SLIME) && vulnerable &&
            duel->phase != DUEL_HURT)
        {
            u8 effect = actor->colChkInfo.damageEffect;
            GekkoDamage* damage = Gekko_GetDamage(duel);
            ColliderElement* element =
                DUEL_FIXED_FORM == DUEL_SNAPPER ? &((StandaloneSnapper*)duel)->elements[0].base : &duel->body.elem;
            int frozenBlocked = damage->timer && damage->type == GEKKO_DRAW_ICE && element->acHitElem &&
                                (element->acHitElem->atDmgInfo.dmgFlags & 0xdb0b3);
            if (DUEL_FIXED_FORM == DUEL_SLIME && duel->phase == SLIME_FRAGMENT_THROW && !frozenBlocked)
                Slime_CancelFragmentThrow((StandaloneSlime*)duel);
            if (effect == GEKKO_DAMAGE_HOOK || frozenBlocked)
            {
                vulnerable = 0;
            }
            else if (actor->colChkInfo.damage || effect == GEKKO_DAMAGE_STUN || effect == GEKKO_DAMAGE_ELECTRIC ||
                     effect == GEKKO_DAMAGE_DEKU)
            {
                if (actor->colChkInfo.damage)
                {
                    if (DUEL_FIXED_FORM == DUEL_SLIME)
                        actor->flags &= ~SLIME_HOOK_ATTACHED;
                    Actor* attacker = damageCollider->ac;
                    u32 flags = element->acHitElem ? element->acHitElem->atDmgInfo.dmgFlags : 0;
                    damage->recoilYaw = !attacker           ? actor->yawTowardsPlayer + 0x8000
                                        : (flags & 0x13820) ? attacker->shape.rot.y
                                                            : Math_Vec3f_Yaw(&attacker->world.pos, &actor->world.pos);
                }
                Gekko_ApplyEffects(duel, play, effect);
            }
        }
        if (DUEL_FIXED_FORM == DUEL_IGOS && vulnerable && duel->phase != DUEL_HURT)
        {
            u8 effect = actor->colChkInfo.damageEffect;
            if (effect == IGOS_DAMAGE_RAY)
                ((StandaloneIgos*)duel)->lightRayTimer = 10;
            if (effect == IGOS_DAMAGE_POUND && (actor->bgCheckFlags & BGCHECKFLAG_GROUND))
            {
                Igos_SetHitDrift((StandaloneIgos*)duel, -4.f);
                actor->velocity.y = 5.f;
                Igos_FootDust((StandaloneIgos*)duel, play, 5);
            }
            if (actor->colChkInfo.damage || effect == IGOS_DAMAGE_ELECTRIC)
                Igos_StartElemental((StandaloneIgos*)duel, effect);
            if (effect == IGOS_DAMAGE_STUN || effect == IGOS_DAMAGE_ICE || effect == IGOS_DAMAGE_ELECTRIC)
            {
                StandaloneIgos* igos = (StandaloneIgos*)duel;
                duel->walking = IGOS_STUN;
                duel->timer = effect == IGOS_DAMAGE_ICE ? 80 : 40;
                igos->damageInvulnerability = actor->colChkInfo.damage ? 10 : 15;
                Actor_SetColorFilter(actor, 0, 120, 0, duel->timer);
                Actor_PlaySfx(actor, 0x389e);
                if (actor->colChkInfo.damage && actor->colChkInfo.damage < actor->colChkInfo.health)
                {
                    Actor_ApplyDamage(actor);
                    igos->damageFlashTimer = 15;
                    vulnerable = 0;
                }
            }
        }
        if ((actor->colChkInfo.damage || DUEL_FIXED_FORM == DUEL_GOMESS) && vulnerable && duel->phase != DUEL_HURT)
        {
            Actor_ApplyDamage(actor);
            if (DUEL_FIXED_FORM == DUEL_SNAPPER && !actor->colChkInfo.health && !((StandaloneSnapper*)duel)->mountPhase)
            {
                actor->colChkInfo.health = 6;
                StandaloneSnapper* snapper = (StandaloneSnapper*)duel;
                snapper->callMountAfterRecoil = 1;
                if (snapper->damage.stun)
                {

                    duel->phase = DUEL_HURT;
                    duel->timer = 0;
                    actor->speed = 0.f;
                }
                else
                {
                    Gekko_StartRecoil(duel);
                }
                return;
            }
            if (DUEL_FIXED_FORM != DUEL_IGOS)
                actor->speed = 0.f;
            if (DUEL_FIXED_FORM == DUEL_IGOS)
            {
                StandaloneIgos* igos = (StandaloneIgos*)duel;
                igos->damageInvulnerability = 10;
                igos->damageFlashTimer = 15;
            }
            if (DUEL_FIXED_FORM == DUEL_IGOS && !actor->colChkInfo.health)
            {
                Igos_ReleasePlayer(actor, play);
                duel->phase = DUEL_IGOS_DOWN;
                duel->timer = 150;
                duel->walking = 0;
                actor->colChkInfo.health = 1;
                Igos_SetHeadTarget((StandaloneIgos*)duel, play, 0);
                actor->flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
                actor->flags |= ACTOR_FLAG_LOCK_ON_DISABLED;
                Player* player = GET_PLAYER(play);
#if defined(GAME_MM)
                if (player->lockOnActor == actor)
                    player->lockOnActor = NULL;
#else
                if (player->focusActor == actor)
                    ((void (*)(Player*))0x800798c4)(player);
#endif
                s16 facing = actor->yawTowardsPlayer - actor->shape.rot.y;
                StandaloneIgos* igos = (StandaloneIgos*)duel;
                igos->fallenBackwards = ABS(facing) < 0x4000;
                igos->animDrift = (Vec3f){ -15.f * Math_SinS(actor->yawTowardsPlayer), 0.f,
                                           -15.f * Math_CosS(actor->yawTowardsPlayer) };
                igos->fallenPoseMask = 0;
                igos->fallenHitTimer = 10;
                Animation_PlayOnce(&duel->skel, Duel_Anim(igos->fallenBackwards ? 0x1cdc : form->defeat));
                Enemy_StartFinishingBlow(play, actor);
                Igos_DeathSound(actor, 0);
                Igos_UpdateBodyEffects(igos, play);
                return;
            }
            if (DUEL_FIXED_FORM == DUEL_SNAPPER && (duel->phase == GEKKO_WALL_CLIMB || duel->phase == GEKKO_WALL_PAUSE))
            {
                StandaloneSnapper* snapper = (StandaloneSnapper*)duel;
                u8 effect = actor->colChkInfo.damageEffect;
                if (actor->colChkInfo.health && (effect == GEKKO_DAMAGE_STUN || effect == GEKKO_DAMAGE_ELECTRIC))
                    return;
                Snapper_StartWallFall(duel, play, Snapper_GetMount(actor));
                snapper->wallHit = 1;
                snapper->damage.stun = 0;
                duel->timer = 10;
                actor->gravity = 0.f;
                Animation_PlayOnce(&duel->skel, Duel_Anim(0x1f20));
                Actor_SetColorFilter(actor, 0x4000, 255, 0, 16);
                if (!actor->colChkInfo.health)
                {
                    actor->flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
                    Enemy_StartFinishingBlow(play, actor);
                }
                Duel_Sfx(actor, actor->colChkInfo.health ? 0x38d3 : 0x38d6);
                return;
            }
            if (!actor->colChkInfo.health)
            {
                if (DUEL_FIXED_FORM == DUEL_SLIME)
                {
                    Slime_StartDefeat((StandaloneSlime*)duel, play);
                    return;
                }
                if (DUEL_FIXED_FORM == DUEL_GOMESS)
                {
                    Enemy_StartFinishingBlow(play, actor);
                    Gomess_StartDefeat((StandaloneGomess*)duel);
                    Gomess_RegisterBody(duel, play);
                    return;
                }
                if (DUEL_FIXED_FORM == DUEL_KEETA)
                {
                    Keeta_StartDefeat((StandaloneKeeta*)duel);
                    return;
                }
                actor->flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
                duel->phase = DUEL_DEFEAT;
                duel->timer = DUEL_FIXED_FORM == DUEL_WART ? 100 : 48;
                if (DUEL_FIXED_FORM == DUEL_GARO)
                {
                    duel->shield = 0;
                    duel->timer = 30;
                    actor->flags &= ~ACTOR_FLAG_HOSTILE;
                    actor->flags |= ACTOR_FLAG_LOCK_ON_DISABLED;
                    actor->shape.rot.x = actor->world.rot.x = 0;
                    actor->shape.rot.z = actor->world.rot.z = 0;
                    StandaloneGaroMaster* garo = (StandaloneGaroMaster*)duel;
                    garo->knockback = (Vec3f){ 0.f, 0.f, 0.f };
                    Garo_StopDashAudio(garo);
                    Garo_ShatterIce(garo, play);
                    Garo_Change(duel, DUEL_DEFEAT, 30, form->hurt, ANIMMODE_LOOP);
                    Enemy_StartFinishingBlow(play, actor);
                }
                else
                {
                    Animation_PlayOnce(&duel->skel, Duel_Anim(form->defeat));
                }
                Duel_Sfx(actor, form->deathSfx);
                return;
            }
            if (DUEL_FIXED_FORM == DUEL_SNAPPER || DUEL_FIXED_FORM == DUEL_SLIME)
            {
                if (actor->colChkInfo.damageEffect != GEKKO_DAMAGE_ICE)
                    Gekko_StartRecoil(duel);
                return;
            }
            if (DUEL_FIXED_FORM == DUEL_GARO && actor->colChkInfo.damageEffect == GARO_DAMAGE_ICE)
            {
                Garo_SetKnockback((StandaloneGaroMaster*)duel, 10.f);
                Garo_Change(duel, GARO_STUN, 30, form->hurt, ANIMMODE_LOOP);
                return;
            }
            if (DUEL_FIXED_FORM == DUEL_KEETA)
            {
                StandaloneKeeta* keeta = (StandaloneKeeta*)duel;
                s16 yaw = actor->yawTowardsPlayer + 0x8000 + (s16)(Rand_ZeroOne() * 0x4000) - 0x2000;
                keeta->recoil = (Vec3f){ Math_SinS(yaw) * 30.f, 0.f, Math_CosS(yaw) * 30.f };
                duel->phase = DUEL_HURT;
                duel->walking = 0;
                Actor_SetColorFilter(actor, 0x4000, 255, 0, 8);
                Keeta_Play(duel, 0x1390, ANIMMODE_ONCE);
                Duel_Sfx(actor, 0x3aa5);
                return;
            }
            duel->walking = 0;
            duel->phase = DUEL_HURT;
            duel->timer = 18;
            if (DUEL_FIXED_FORM == DUEL_GOMESS)
            {
                Gomess_SetupRecoil((StandaloneGomess*)duel);
                finished = 0;
            }
            if (DUEL_FIXED_FORM == DUEL_GARO)
            {
                Garo_SetKnockback((StandaloneGaroMaster*)duel, 20.f);
#if defined(GAME_MM)
                for (int i = 0; i < 3; ++i)
                    ((void (*)(PlayState*, void*, Vec3f*))0x800e2d88)(play, NULL, &actor->focus.pos);
#endif
                Garo_Change(duel, DUEL_HURT, 8, form->hurt, ANIMMODE_LOOP);
                actor->velocity.y = 10.f;
                Actor_SetColorFilter(actor, 0x4000, 255, 0, 8);
            }
            if (DUEL_FIXED_FORM == DUEL_WART)
            {
                duel->timer = 15;
                Actor_SetColorFilter(actor, 0x4000, 255, 0, 15);
                actor->velocity.y = actor->colChkInfo.health <= 10 ? 5.f : 0.f;
            }
            if (DUEL_FIXED_FORM == DUEL_IGOS)
            {
                s16 facing = actor->yawTowardsPlayer - actor->shape.rot.y;
                Animation_PlayOnce(&duel->skel, Duel_Anim(ABS(facing) < 0x4000 ? form->hurt : 0x0d9c));
                duel->timer = 10;
                int fire = actor->colChkInfo.damageEffect == IGOS_DAMAGE_FIRE;
                Actor_SetColorFilter(actor, 0x4000, fire ? 120 : 255, 0, fire ? 60 : 15);

                Igos_SetHitDrift((StandaloneIgos*)duel, -15.f);
            }
            else if (DUEL_FIXED_FORM != DUEL_GARO)
            {
                Animation_PlayOnce(&duel->skel, Duel_Anim(form->hurt));
            }
            if (DUEL_FIXED_FORM == DUEL_IGOS)
            {
                static const u16 voices[] = { 0x3a3a, 0x3a3c, 0x3a3b };
                Duel_Sfx(actor, voices[Igos_VoiceVariant(actor)]);
            }
            else
            {
                Duel_Sfx(actor, form->hurtSfx);
            }
        }
    }

    if (DUEL_FIXED_FORM == DUEL_SLIME)
    {
        Slime_Combat((StandaloneSlime*)duel, play, finished);
    }
    else if (DUEL_FIXED_FORM == DUEL_WART)
    {
        Wart_Move((StandaloneWart*)duel, play);
    }
    else if (DUEL_FIXED_FORM == DUEL_GARO)
    {
        Garo_Combat(duel, play, finished);
    }
    else if (DUEL_FIXED_FORM == DUEL_GOMESS)
    {
        Gomess_UpdateLooseCore((StandaloneGomess*)duel, play);
        Gomess_Combat(duel, play, finished, prevFrame);
    }
    else if (DUEL_FIXED_FORM == DUEL_SNAPPER)
    {
        ((StandaloneSnapper*)duel)->body.base.atFlags &= ~AT_ON;
        if (wallStunned)
            actor->speed = 0.f;
        else
            Snapper_Combat(duel, play, finished);
    }
    else if (DUEL_FIXED_FORM == DUEL_IGOS)
    {
        Igos_Combat(duel, play, finished, prevFrame);
    }
    else if (DUEL_FIXED_FORM == DUEL_KEETA)
    {
        Keeta_Combat((StandaloneKeeta*)duel, play, finished);
    }
    else if (duel->phase == DUEL_APPROACH)
    {
        Math_SmoothStepToS(&actor->shape.rot.y, actor->yawTowardsPlayer, 3, 0x800, 0x80);
        actor->world.rot.y = actor->shape.rot.y;
        actor->speed = 0.f;
        if (actor->xzDistToPlayer < 380.f && fabsf(actor->yDistanceFromLink) < 85.f)
        {
            if (actor->xzDistToPlayer < 88.f)
            {
                duel->phase = DUEL_ATTACK;
                duel->timer = 32;
                Animation_PlayOnce(&duel->skel, Duel_Anim(form->attack));
                duel->walking = 0;
            }
            else
                actor->speed = DUEL_FIXED_FORM == DUEL_WART && actor->colChkInfo.health <= 8 ? 5.f : 3.f;
        }
    }
    else if (duel->phase == DUEL_ATTACK)
    {
        if (duel->timer == 20)
            Duel_Sfx(actor, form->strikeSfx);
        actor->speed = duel->timer <= 21 && duel->timer >= 13 && DUEL_FIXED_FORM == DUEL_WART ? 5.f : 0.f;
        if (duel->timer <= 20 && duel->timer >= 12)
            Duel_Weapon(duel, play);
        if (!duel->timer)
        {
            duel->weapon.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
            duel->phase = DUEL_RECOVER;
            duel->timer = 24;
        }
    }
    else if (!duel->timer)
    {
        duel->phase = DUEL_APPROACH;
        duel->weapon.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
        Animation_PlayLoop(&duel->skel, Duel_Anim(form->idle));
    }
    if (DUEL_FIXED_FORM == DUEL_SNAPPER && (duel->phase == GEKKO_WALL_CLIMB || duel->phase == GEKKO_WALL_PAUSE ||
                                            duel->phase == GEKKO_MOUNT_JUMP || duel->phase == GEKKO_MOUNT_LAND))
    {
        actor->colChkInfo.displacement = (Vec3f){ 0.f, 0.f, 0.f };
    }
    else
    {
        if (DUEL_FIXED_FORM == DUEL_GARO)
            Garo_CheckLedge(duel, play);
        if (DUEL_FIXED_FORM != DUEL_SLIME || !(actor->flags & SLIME_HOOK_ATTACHED))
            Actor_MoveWithGravity(actor);
        if (DUEL_FIXED_FORM == DUEL_IGOS)
            Igos_ApplyAnimDrift((StandaloneIgos*)duel, play);
        if (DUEL_FIXED_FORM == DUEL_GARO)
            Garo_MoveKnockback((StandaloneGaroMaster*)duel, play);
        if (DUEL_FIXED_FORM == DUEL_GOMESS)
        {
            f32 wallRadius = duel->phase == GOMESS_SPIN ? 50.f : 100.f;
            Actor_UpdateBgCheckInfo(play, actor, 40.f, wallRadius, 40.f, 7);
        }
        else if (DUEL_FIXED_FORM == DUEL_GARO)
        {
            Garo_CheckMovement(actor, play);
        }
        else
        {
            f32 wallRadius = 32.f;
            if (DUEL_FIXED_FORM == DUEL_SNAPPER)
                wallRadius = ((StandaloneSnapper*)duel)->wallHit == 1 ? 3.f : 15.f;
            u32 bgFlags = 5;
            if (DUEL_FIXED_FORM == DUEL_SLIME)
            {
                if (duel->phase == SLIME_CEILING || duel->phase == SLIME_FALL)
                    bgFlags = 4;
            }
            Actor_UpdateBgCheckInfo(play, actor, 24.f, wallRadius, 65.f, bgFlags);
        }
        if (DUEL_FIXED_FORM == DUEL_IGOS)
        {
            StandaloneIgos* igos = (StandaloneIgos*)duel;
            Igos_DecayAnimDrift(igos);
            if (duel->walking != IGOS_ADVANCE)
                Math_ApproachS(&actor->shape.rot.y, actor->world.rot.y, 3, 0xa00);
            Igos_UpdateBodyEffects(igos, play);
        }
    }
    if (DUEL_FIXED_FORM == DUEL_WART)
        Wart_FollowTerrain((StandaloneWart*)duel, play);
    if (DUEL_FIXED_FORM == DUEL_SLIME)
        Slime_ConstrainHeight((StandaloneSlime*)duel, play);
    int hasPoseFocus = 0;
    if (DUEL_FIXED_FORM == DUEL_GOMESS)
    {
        StandaloneGomess* gomess = (StandaloneGomess*)duel;
        hasPoseFocus = gomess->corePoseValid && play->gameplayFrames - gomess->corePoseFrame <= 1;
    }
    else if (DUEL_FIXED_FORM == DUEL_WART)
    {
        StandaloneWart* wart = (StandaloneWart*)duel;
        hasPoseFocus = wart->collisionPoseValid && play->gameplayFrames - wart->collisionPoseFrame <= 1;
    }
    else if (DUEL_FIXED_FORM == DUEL_SNAPPER)
    {
        GekkoDamage* damage = &((StandaloneSnapper*)duel)->damage;
        hasPoseFocus = damage->pointMask && play->gameplayFrames - damage->poseFrame <= 1;
    }
    if (DUEL_FIXED_FORM == DUEL_GARO)
    {
        Garo_RecordAfterimage((StandaloneGaroMaster*)duel);
        if (duel->phase != GARO_CIRCLE && duel->phase != GARO_WINDUP && duel->phase != DUEL_HURT &&
            duel->phase != GARO_SPIN && duel->phase != GARO_VANISH && duel->phase != GARO_RETREAT)
            actor->shape.rot.y = actor->world.rot.y;
    }
    if (!hasPoseFocus)
    {
        actor->focus.pos = actor->world.pos;
        actor->focus.pos.y += DUEL_FIXED_FORM == DUEL_GARO ? 80.f : 48.f;
    }
    if (DUEL_FIXED_FORM == DUEL_GARO && (duel->phase == GARO_SPIN || duel->phase == GARO_VANISH))
        return;
    if (DUEL_FIXED_FORM != DUEL_GOMESS)
        Collider_UpdateCylinder(actor, &duel->body);
    if (DUEL_FIXED_FORM == DUEL_SLIME)
    {
        StandaloneSlime* slime = (StandaloneSlime*)duel;
        GekkoDamage* damage = &slime->damage;
        duel->body.dim.radius = 30;
        duel->body.dim.height = 60;
        duel->body.dim.yShift = 0;
        if (damage->pointMask && play->gameplayFrames - damage->poseFrame <= 1)
            duel->body.dim.pos.y = damage->footY;
        int inWater = duel->phase < SLIME_FORM && !damage->stun && actor->floorPoly &&
                      (actor->bgCheckFlags & BGCHECKFLAG_WATER) && duel->body.dim.pos.y < actor->floorHeight + 3.f;
        if (inWater && (!slime->waterRippleActive || !(play->gameplayFrames & 3)))
        {
            Vec3f water = actor->world.pos;
            water.y += actor->depthInWater;
            EffectSsGRipple_Spawn(play, &water, 150, 550, 0);
        }
        slime->waterRippleActive = inWater;
        if (duel->phase < SLIME_FORM)
        {
            if (damage->timer && damage->type == GEKKO_DRAW_ICE)
                actor->flags &= ~SLIME_HOOK_PULLS_ACTOR;
            else
                actor->flags |= SLIME_HOOK_PULLS_ACTOR;
            slime->hookSafePos = actor->world.pos;
        }
        else
        {
            actor->flags &= ~(SLIME_HOOK_PULLS_ACTOR | SLIME_HOOK_ATTACHED);
        }
        if (duel->phase == SLIME_FORM)
            Slime_RegisterShell(slime, play);
    }
    if (DUEL_FIXED_FORM == DUEL_GARO)
    {
        duel->body.base.acFlags &= ~AC_HARD;
        if (duel->phase == GARO_CIRCLE || duel->phase == GARO_GUARD || duel->phase == GARO_APPEAR ||
            duel->phase == GARO_WINDUP || duel->phase == GARO_DASH || duel->phase == GARO_FALL)
            duel->body.base.acFlags |= AC_HARD;
    }
    if (DUEL_FIXED_FORM == DUEL_GOMESS)
    {
        StandaloneGomess* gomess = (StandaloneGomess*)duel;
        Gomess_RegisterBody(duel, play);
        if (duel->phase != DUEL_HURT && hasPoseFocus)
        {
            int exposed = Gomess_CrowdCount(gomess) < 5;
            gomess->coreElement.dim.worldSphere.center = (Vec3s){
                gomess->corePos.x,
                gomess->corePos.y + 5.f * exposed,
                gomess->corePos.z,
            };
            gomess->coreElement.dim.worldSphere.radius = 22 + 5 * exposed;
            Duel_AC(play, &play->colChkCtx, &gomess->core.base);
        }
        if (duel->phase == GOMESS_GUARD || duel->phase == GOMESS_SPIN)
        {
            CollisionCheck_SetAT(play, &play->colChkCtx, &gomess->spin.base);
            Duel_AC(play, &play->colChkCtx, &gomess->spin.base);
        }
    }
    else if (DUEL_FIXED_FORM == DUEL_IGOS)
    {
        StandaloneIgos* igos = (StandaloneIgos*)duel;
        if (!igos->headTarget && duel->walking != IGOS_DETACH && duel->walking != IGOS_REATTACH &&
            igos->bodyPoseMask == 3 && play->gameplayFrames - igos->bodyPoseFrame <= 1)
        {
            if (igos->shieldInvulnerability)
                --igos->shieldInvulnerability;
            else
                Duel_AC(play, &play->colChkCtx, &igos->bodySpheres.base);
            CollisionCheck_SetOC(play, &play->colChkCtx, &igos->bodySpheres.base);
        }
    }
    else if (DUEL_FIXED_FORM == DUEL_WART)
    {
        StandaloneWart* wart = (StandaloneWart*)duel;
        if (wart->collisionPoseValid && play->gameplayFrames - wart->collisionPoseFrame <= 1)
        {
            if (duel->phase != DUEL_HURT && wart->eyelid > 3000.f)
                Duel_AC(play, &play->colChkCtx, &wart->eye.base);
            Duel_AC(play, &play->colChkCtx, &wart->shell.base);
            CollisionCheck_SetOC(play, &play->colChkCtx, &wart->shell.base);
            Vec3s* center = &wart->shellElement.dim.worldSphere.center;
            Vec3f from = { center->x, center->y, center->z };
            Vec3f target = GET_PLAYER(play)->actor.world.pos;
            target.y += 25.f;
            if (Math_Vec3f_DistXYZ(&from, &target) < 105.f && enemyFlightClear(play, &from, &target))
                CollisionCheck_SetAT(play, &play->colChkCtx, &wart->shell.base);
        }
    }
    else if (DUEL_FIXED_FORM == DUEL_KEETA)
    {
        StandaloneKeeta* keeta = (StandaloneKeeta*)duel;
        if (keeta->bodyPoseMask == 0x7f && play->gameplayFrames - keeta->bodyPoseFrame <= 1)
        {
            if (duel->phase != DUEL_HURT && (duel->phase < DUEL_KEETA_JUMP || duel->phase == DUEL_KEETA_LAND))
                Duel_AC(play, &play->colChkCtx, &keeta->bodySpheres.base);
            CollisionCheck_SetOC(play, &play->colChkCtx, &keeta->bodySpheres.base);
        }
    }
    else if (DUEL_FIXED_FORM == DUEL_SNAPPER)
    {
        StandaloneSnapper* snapper = (StandaloneSnapper*)duel;
        if (snapper->damage.pointMask && play->gameplayFrames - snapper->damage.poseFrame <= 1 &&
            duel->phase < GEKKO_CALL_MOUNT)
        {
            if (Snapper_ReceivesHits(snapper))
                Duel_AC(play, &play->colChkCtx, &snapper->body.base);
            CollisionCheck_SetOC(play, &play->colChkCtx, &snapper->body.base);
        }
    }
    else if (DUEL_FIXED_FORM == DUEL_SLIME && duel->phase > SLIME_FORM)
        Slime_RegisterShell((StandaloneSlime*)duel, play);
    else if ((duel->phase != DUEL_HURT || DUEL_FIXED_FORM == DUEL_GARO) &&
             (DUEL_FIXED_FORM != DUEL_SLIME ||
              (duel->phase < SLIME_FORM && !((StandaloneSlime*)duel)->waitingForShatterLanding)) &&
             (DUEL_FIXED_FORM != DUEL_SNAPPER || duel->phase != GEKKO_MOUNT_JUMP))
        Duel_AC(play, &play->colChkCtx, &duel->body.base);
    if (DUEL_FIXED_FORM != DUEL_IGOS && DUEL_FIXED_FORM != DUEL_KEETA && DUEL_FIXED_FORM != DUEL_SNAPPER &&
        DUEL_FIXED_FORM != DUEL_WART && DUEL_FIXED_FORM != DUEL_GOMESS &&
        (DUEL_FIXED_FORM != DUEL_SLIME || duel->phase <= SLIME_FORM))
        CollisionCheck_SetOC(play, &play->colChkCtx, &duel->body.base);
}

static void Garo_UpdateSwordPose(StandaloneGaroMaster* garo, PlayState* play, int side)
{
    static Vec3f baseOffset = { 0.f, 0.f, 0.f };
    static Vec3f quadTip = { 1700.f, 0.f, 0.f };
    static Vec3f trailTip = { 1600.f, 0.f, 0.f };
    ColliderQuad* sword = &garo->swords[side];
    Vec3f base, tip, trailEnd;
    int mask = 1 << side;
    int consecutive = (garo->poseMask & mask) && play->gameplayFrames - garo->poseFrame[side] == 1;
    Matrix_MultVec3f(&baseOffset, &base);
    Matrix_MultVec3f(&quadTip, &tip);
    if (consecutive)
    {
        Vec3f previousBase = sword->dim.quad[0];
        Vec3f previousTip = sword->dim.quad[1];
        Collider_SetQuadVertices(sword, &base, &tip, &previousBase, &previousTip);
    }
    else
    {
        Collider_SetQuadVertices(sword, &base, &tip, &base, &tip);
    }
    garo->poseMask |= mask;
    garo->poseFrame[side] = play->gameplayFrames;

    void* trail = garo->trails[side] < 0 ? NULL : Effect_GetByIndex(garo->trails[side]);
    if (!trail)
        return;
    int phase = garo->duel.phase;
    if (phase == GARO_DASH || phase == GARO_SLASH || phase == GARO_SPIN || phase == GARO_FALL)
    {
        Matrix_MultVec3f(&trailTip, &trailEnd);
        EffectBlure_AddVertex(trail, &trailEnd, &base);
        garo->trailMask |= mask;
    }
    else if (garo->trailMask & mask)
    {
#if defined(GAME_MM)
        ((void (*)(void*))0x800a8514)(trail);
#else
        EffectBlure_AddSpace(trail);
#endif
        garo->trailMask &= ~mask;
    }
}

#if defined(GARO_MASTER_TELEPORT)
static void Garo_DrawAfterimages(StandaloneGaroMaster* garo, PlayState* play)
{
    StandaloneDuel* duel = &garo->duel;
    if (duel->phase == DUEL_DEFEAT || !garo->afterimageCount)
        return;
    GraphicsContext* gfx = play->state.gfxCtx;
    Gfx_SetupDL25_Xlu(gfx);
    Scene_SetRenderModeXlu(play, 1, 2);
    OPEN_DISPS(gfx);
    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, 128);
    Matrix_Push();
    int index = garo->afterimageIndex;
    for (int i = 0; i < garo->afterimageCount; ++i)
    {
        Vec3f* pos = &garo->afterimages[index].pos;
        Vec3s* rot = &garo->afterimages[index].rot;
        Matrix_Translate(pos->x, pos->y, pos->z, MTXMODE_NEW);
        Matrix_Scale(duel->actor.scale.x, duel->actor.scale.y, duel->actor.scale.z, MTXMODE_APPLY);
        Matrix_RotateY(rot->y * (M_PI / 32768.f), MTXMODE_APPLY);
        Matrix_RotateX(rot->x * (M_PI / 32768.f), MTXMODE_APPLY);
        Matrix_RotateZ(rot->z * (M_PI / 32768.f), MTXMODE_APPLY);
        POLY_XLU_DISP = SkelAnime_DrawFlex(play, duel->skel.skeleton, garo->afterimages[index].joints,
                                           duel->skel.dListCount, NULL, NULL, &duel->actor, POLY_XLU_DISP);
        index = (index + ARRAY_COUNT(garo->afterimages) - 1) % ARRAY_COUNT(garo->afterimages);
    }
    Matrix_Pop();
    CLOSE_DISPS();
}
#endif

static s32 Garo_OverrideLimb(PlayState* play, s32 limb, Gfx** dList, Vec3f* pos, Vec3s* rot, void* arg)
{
    StandaloneGaroMaster* garo = arg;
    if (garo->duel.phase == DUEL_DEFEAT)
    {
        if (limb == 6 || (limb == 4 && !garo->duel.shield))
            *dList = NULL;
    }
    else if ((limb == 4 || limb == 6) && !garo->swordsDrawn)
    {
        *dList = NULL;
    }
    return 0;
}

static void Garo_DrawLimb(PlayState* play, s32 limb, Gfx** dList, Vec3s* rot, void* arg)
{
    StandaloneDuel* duel = arg;
    StandaloneGaroMaster* garo = (StandaloneGaroMaster*)duel;
    if (limb == 12)
    {
        GraphicsContext* gfx = play->state.gfxCtx;
        Mtx* matrix = GRAPH_ALLOC(gfx, sizeof(Mtx));
        if (matrix)
        {
            OPEN_DISPS(gfx);
            gSPMatrix(POLY_OPA_DISP++, Duel_ToMtx(matrix), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
            gSPDisplayList(POLY_OPA_DISP++, 0x06002ed8);
            CLOSE_DISPS();
        }
    }
    if (limb == 1 && !(duel->phase == DUEL_DEFEAT && duel->shield))
        garo->damagePointCount = 0;
    if (!(duel->phase == DUEL_DEFEAT && duel->shield) &&
        (limb == 4 || (limb >= 6 && limb <= 12) || limb == 14 || limb == 16 || limb == 17 || limb == 19) &&
        garo->damagePointCount < ARRAY_COUNT(garo->damagePoints))
    {
        Vec3f origin = { 0.f, 0.f, 0.f };
        Matrix_MultVec3f(&origin, &garo->damagePoints[garo->damagePointCount++]);
    }
    if (limb == 4 || limb == 6)
        Garo_UpdateSwordPose((StandaloneGaroMaster*)duel, play, limb == 6);
    int frozen = garo->damageTimer && (garo->damageEffect == 10 || garo->damageEffect == 11);
    if ((limb == 4 || limb == 6) && duel->phase != DUEL_DEFEAT && duel->phase != GARO_VANISH &&
        duel->phase != GARO_FALL && !frozen && garo->swordsDrawn)
    {
#if defined(GAME_OOT)
        const u32 fire = 0x04052a10;
#else
        const u32 fire = 0x0407d590;
#endif
        static const Vec3f normalOffsets[] = {
            { 800.f, -20.f, 50.f },
            { 500.f, -20.f, 70.f },
            { 300.f, -20.f, 90.f },
        };
        static const Vec3f slashOffsets[] = {
            { 600.f, -100.f, -100.f },
            { 300.f, -100.f, -80.f },
            { 100.f, -100.f, -60.f },
        };
        const Vec3f* offsets = duel->phase == GARO_SLASH || duel->phase == GARO_SWING ? slashOffsets : normalOffsets;
        GraphicsContext* gfx = play->state.gfxCtx;
        OPEN_DISPS(gfx);
        gSPSegment(POLY_XLU_DISP++, 8,
                   Gfx_TwoTexScroll(gfx, 0, 0, 0, 32, 64, 1, 0,
                                    (garo->flameScroll * 10 - play->state.frameCount * 20) & 0x1ff, 32, 128));
        gDPPipeSync(POLY_XLU_DISP++);
        gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 255, 255, 170, 255);
        gDPSetEnvColor(POLY_XLU_DISP++, 255, 50, 0, 255);
        for (int i = 0; i < ARRAY_COUNT(normalOffsets); ++i)
        {
            Vec3f offset = offsets[i];
            if (limb == 4)
                offset.z = -offset.z;
            Vec3f pos;
            Matrix_MultVec3f(&offset, &pos);
            Matrix_Push();
            Matrix_Translate(pos.x, pos.y, pos.z, MTXMODE_NEW);
            Matrix_ReplaceRotation(&play->billboardMtxF);
            f32 scale = garo->flameScale[i];
            Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
            gSPMatrix(POLY_XLU_DISP++, Duel_ToMtx(GRAPH_ALLOC(gfx, sizeof(Mtx))),
                      G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
            gSPDisplayList(POLY_XLU_DISP++, fire);
            Matrix_Pop();
        }
        CLOSE_DISPS();
        return;
    }
    if (limb != 10 || duel->phase != DUEL_DEFEAT || duel->shield != 5)
        return;
#if defined(GAME_OOT)
    const u32 cap = 0x04007a50, body = 0x04007860;
#else
    const u32 cap = 0x04015fa0, body = 0x04015db0;
#endif
    OPEN_DISPS(play->state.gfxCtx);
    Matrix_Push();
    Vec3f offset = { 900.f, 50.f, -330.f };
    Matrix_MultVec3f(&offset, &garo->bombPos);
    garo->bombPoseFrame = play->gameplayFrames;
    garo->bombPoseValid = 1;
    Matrix_Translate(offset.x, offset.y, offset.z, MTXMODE_APPLY);
    Matrix_ReplaceRotation(&play->billboardMtxF);
    Matrix_Scale(.24f, .24f, .24f, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, Duel_ToMtx(GRAPH_ALLOC(play->state.gfxCtx, sizeof(Mtx))),
              G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(POLY_OPA_DISP++, cap);
    Matrix_RotateX(M_PI / 2.f, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, Duel_ToMtx(GRAPH_ALLOC(play->state.gfxCtx, sizeof(Mtx))),
              G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 160, 0, 40, 255);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 160, 0, 40, 255);
    gSPDisplayList(POLY_OPA_DISP++, body);
    Matrix_Pop();
    CLOSE_DISPS();
}

static int Igos_HeadDetached(const StandaloneDuel* duel)
{
    return duel->phase != DUEL_DEFEAT && ((const StandaloneIgos*)duel)->headTarget != NULL;
}

static s32 Igos_OverrideLimb(PlayState* play, s32 limb, Gfx** dList, Vec3f* pos, Vec3s* rot, void* arg)
{
    StandaloneDuel* duel = arg;
    if ((u16)duel->actor.params == 0x827f) {
        if (limb == 15)
            *dList = (Gfx*)0x06013020;
        else if (limb == 18)
            *dList = (Gfx*)0x06012db0;
        else if (limb == 19)
            *dList = (Gfx*)0x06012400;
    }
    if (limb == 15)
        rot->x += ((StandaloneIgos*)duel)->neckYaw;
    if (Igos_HeadDetached(duel) && (limb == 18 || limb == 19))
        *dList = NULL;
    return 0;
}

static void Igos_DrawLimb(PlayState* play, s32 limb, Gfx** dList, Vec3s* rot, void* arg)
{
    StandaloneIgos* igos = arg;
    f32 scale = (u16)igos->duel.actor.params == 0x807f ? 1.3076924f : 1.f;
    if (limb == 20 || limb == 21)
    {
        if (igos->bodyPoseFrame != play->gameplayFrames)
        {
            igos->bodyPoseFrame = play->gameplayFrames;
            igos->bodyPoseMask = 0;
        }
        int part = limb == 21;
        ColliderJntSphElement* element = &igos->bodyElements[part];
        Vec3f offset = part ? (Vec3f){ 700.f, -500.f, 0.f } : (Vec3f){ 1000.f, 0.f, 0.f };
        Vec3f center;
        Matrix_MultVec3f(&offset, &center);
        element->dim.worldSphere.center = (Vec3s){ center.x, center.y, center.z };
        element->dim.worldSphere.radius =
            element->dim.modelSphere.radius * scale * (igos->duel.phase == DUEL_IGOS_DOWN ? 2.f : 1.f);
        igos->bodyPoseMask |= 1u << part;
    }
    if (limb == 25 || limb == 28)
    {
        if (igos->footPoseFrame != play->gameplayFrames)
        {
            igos->footPoseFrame = play->gameplayFrames;
            igos->footPoseMask = 0;
        }
        int foot = limb == 28;
        const MtxF* pose = Matrix_GetCurrent();
        igos->footPos[foot] = (Vec3f){ pose->xw, pose->yw, pose->zw };
        igos->footPoseMask |= 1u << foot;
    }
    if (igos->duel.phase == DUEL_IGOS_DOWN || igos->duel.phase == DUEL_DEFEAT || igos->elementalAlpha)
    {
        static const u8 limbs[] = { 20, 21, 4, 5, 6, 9, 10, 11, 18, 23, 24, 25, 26, 27, 28 };
        if (igos->fallenPoseFrame != play->gameplayFrames)
        {
            igos->fallenPoseFrame = play->gameplayFrames;
            igos->fallenPoseMask = 0;
        }
        for (int i = 0; i < ARRAY_COUNT(limbs); ++i)
        {
            if (limb != limbs[i])
                continue;
            const MtxF* pose = Matrix_GetCurrent();
            igos->fallenParts[i] = (Vec3f){ pose->xw, pose->yw, pose->zw };
            igos->fallenPoseMask |= 1u << i;
            break;
        }
    }
    if (limb == 6 || limb == 11)
    {
        Vec3f offset = limb == 6 ? (Vec3f){ 0.f, 600.f, 0.f } : (Vec3f){ 1000.f, 0.f, 0.f };
        Vec3f center;
        Matrix_MultVec3f(&offset, &center);
        ColliderJntSphElement* element = limb == 6 ? &igos->shieldElement : &igos->swordElement;
        element->dim.worldSphere.center = (Vec3s){ center.x, center.y, center.z };
        element->dim.worldSphere.radius = element->dim.modelSphere.radius * scale;
        if (limb == 6)
        {
            igos->shieldPoseFrame = play->gameplayFrames;
            igos->hasShieldPose = 1;
            Vec3f spark = { 0.f, 600.f, 1000.f };
            Matrix_MultVec3f(&spark, &igos->shieldFlashPos);
        }
        else
        {
            igos->swordPoseFrame = play->gameplayFrames;
            igos->hasSwordPose = 1;
        }
    }
    if (limb == 18)
    {
        Vec3f origin = { 0.f, 0.f, 0.f };
        Vec3f* pos = &igos->duel.actor.world.pos;
        Matrix_MultVec3f(&origin, &igos->headSocketOffset);
        igos->headSocketOffset.x -= pos->x;
        igos->headSocketOffset.y -= pos->y;
        igos->headSocketOffset.z -= pos->z;
        igos->headSocketFrame = play->gameplayFrames;
        igos->hasHeadSocket = 1;
    }
    if (limb != 15)
        return;
    Vec3f offset = { 300.f, 500.f, 0.f };
    Matrix_MultVec3f(&offset, &igos->breathOrigin);
    Vec3f* pos = &igos->duel.actor.world.pos;
    igos->breathOrigin.x -= pos->x;
    igos->breathOrigin.y -= pos->y;
    igos->breathOrigin.z -= pos->z;
    igos->hasBreathOrigin = 1;
}

static s32 Igos_HeadLimb(PlayState* play, s32 limb, Gfx** list, Vec3f* pos, Vec3s* rot, void* arg, Gfx** dl)
{
    s16 jaw = *(s16*)arg;
    if (limb == 18)
        rot->z += jaw;
    else if (limb == 19)
        rot->z -= jaw;
    else
        *list = NULL;
    return 0;
}

static void Igos_DrawHeadModel(IgosFlyingHead* head, PlayState* play, Gfx** dl, const Vec3f* pos, s16 yaw, s16 pitch,
                               s16 jaw)
{
    Matrix_Push();
    Matrix_Translate(pos->x, pos->y, pos->z, MTXMODE_NEW);
    Matrix_RotateYS(yaw, MTXMODE_APPLY);
    Matrix_RotateX(pitch * (M_PI / 32768.f), MTXMODE_APPLY);
    Matrix_Scale(.017f, .017f, .017f, MTXMODE_APPLY);
    Matrix_Translate(300.f, -3800.f, -900.f, MTXMODE_APPLY);
    *dl = SkelAnime_DrawFlex(play, head->headSkel.skeleton, head->headJoints, head->headSkel.dListCount, Igos_HeadLimb,
                             NULL, (Actor*)&jaw, *dl);
    Matrix_Pop();
}

static Gfx* Igos_TranslucentMaterial(GraphicsContext* gfx, u8 alpha)
{
    Gfx* material = GRAPH_ALLOC(gfx, 6 * sizeof(Gfx));
    Gfx* dl = material;
    gDPPipeSync(dl++);
    gDPSetCombineLERP(dl++, TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, COMBINED, 0, ENVIRONMENT,
                      0);
    gDPSetEnvColor(dl++, 255, 255, 255, alpha);
    gDPSetRenderMode(dl++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_XLU_SURF2);
    gSPClearGeometryMode(dl++, G_CULL_BACK);
    gSPEndDisplayList(dl++);
    return material;
}

static Gfx sIgosOpaqueMaterial[] = { gsSPEndDisplayList() };

static s32 Igos_OverrideLimbXlu(PlayState* play, s32 limb, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* actor, Gfx** gfx)
{
    return Igos_OverrideLimb(play, limb, dList, pos, rot, actor);
}

static void Igos_DrawLimbXlu(PlayState* play, s32 limb, Gfx** dList, Vec3s* rot, Actor* actor, Gfx** gfx)
{
    Igos_DrawLimb(play, limb, dList, rot, actor);
}

static void Igos_DrawTranslucentBody(StandaloneIgos* igos, PlayState* play)
{
    GraphicsContext* gfx = play->state.gfxCtx;
    SkelAnime* skel = &igos->duel.skel;
    Gfx_SetupDL25_Xlu(gfx);
    OPEN_DISPS(gfx);
    gSPSegment(POLY_XLU_DISP++, 9, Igos_TranslucentMaterial(gfx, igos->bodyAlpha));
    gSPSegment(POLY_XLU_DISP++, 10, sIgosOpaqueMaterial);
    POLY_XLU_DISP = SkelAnime_DrawFlex(play, skel->skeleton, skel->jointTable, skel->dListCount, Igos_OverrideLimbXlu,
                                       Igos_DrawLimbXlu, &igos->duel.actor, POLY_XLU_DISP);
    CLOSE_DISPS();
}

static void Igos_DrawRecoveryTrail(StandaloneIgos* igos, PlayState* play)
{
    SkelAnime* skel = &igos->duel.skel;
    GraphicsContext* gfx = play->state.gfxCtx;
    OPEN_DISPS(gfx);
    for (int i = 0; i < ARRAY_COUNT(igos->recoveryTrail); ++i)
    {
        IgosBodyImage* image = &igos->recoveryTrail[(igos->recoveryTrailNext + i) % ARRAY_COUNT(igos->recoveryTrail)];
        if (!image->alpha)
            continue;
        Vec3f* scale = &igos->duel.actor.scale;
        Gfx_SetupDL25_Xlu(gfx);
        Matrix_Push();
        Matrix_Translate(image->pos.x, image->pos.y, image->pos.z, MTXMODE_NEW);
        Matrix_RotateZYX(image->rot.x, image->rot.y, image->rot.z, MTXMODE_APPLY);
        Matrix_Scale(scale->x, scale->y, scale->z, MTXMODE_APPLY);
        Gfx* material = Igos_TranslucentMaterial(gfx, image->alpha);
        gSPSegment(POLY_XLU_DISP++, 9, material);
        gSPSegment(POLY_XLU_DISP++, 10, material);
        POLY_XLU_DISP = SkelAnime_DrawFlex(play, skel->skeleton, image->joints, skel->dListCount, NULL, NULL,
                                           &igos->duel.actor, POLY_XLU_DISP);
        Matrix_Pop();
    }
    CLOSE_DISPS();
}

static void Igos_DrawFlyingHead(IgosFlyingHead* head, PlayState* play)
{
    GraphicsContext* gfx = play->state.gfxCtx;
    Gfx_SetupDL25_Opa(gfx);
    OPEN_DISPS(gfx);
    gSPSegment(POLY_OPA_DISP++, 9, sIgosOpaqueMaterial);
    gSPSegment(POLY_OPA_DISP++, 10, sIgosOpaqueMaterial);
    if (head->headAction != IGOS_HEAD_ATTACHED)
        Igos_DrawHeadModel(head, play, &POLY_OPA_DISP, &head->actor.world.pos, head->headYaw, head->headPitch,
                           head->headJaw);
    int newest = (head->headTrailNext + ARRAY_COUNT(head->headTrail) - 1) % ARRAY_COUNT(head->headTrail);
    if (head->headTrail[newest].alpha)
    {
        Gfx_SetupDL25_Xlu(gfx);
        for (int i = 0; i < ARRAY_COUNT(head->headTrail); ++i)
        {
            int index = (head->headTrailNext + i) % ARRAY_COUNT(head->headTrail);
            const IgosHeadImage* image = &head->headTrail[index];
            if (!image->alpha)
                continue;
            Gfx* trailMaterial = Igos_TranslucentMaterial(gfx, image->alpha);
            gSPSegment(POLY_XLU_DISP++, 9, trailMaterial);
            gSPSegment(POLY_XLU_DISP++, 10, trailMaterial);
            Igos_DrawHeadModel(head, play, &POLY_XLU_DISP, &image->pos, image->yaw, image->pitch, image->jaw);
        }
    }
    CLOSE_DISPS();
}

static void Igos_DrawSwordBlur(StandaloneIgos* igos, PlayState* play)
{
    if (!igos->swordBlurAlpha)
    {
        igos->swordBlurFrame = 0;
        return;
    }
    static const u32 models[] = { 0x06018af0, 0x060189f0, 0x060188f8 };
    GraphicsContext* gfx = play->state.gfxCtx;
    Actor* actor = &igos->duel.actor;
    int king = (u16)actor->params == 0x807f;
    Gfx_SetupDL25_Xlu(gfx);
    AnimatedMaterial* material = (AnimatedMaterial*)(gSegments[6] + K0BASE + 0x18bc4);
#if defined(GAME_OOT)
    void AnimatedMat_Draw(PlayState*, AnimatedMaterial*);
    AnimatedMat_Draw(play, material);
#else
    ((void (*)(PlayState*, AnimatedMaterial*))0x80131758)(play, material);
#endif
    OPEN_DISPS(gfx);
    Matrix_Push();
    Matrix_Translate(actor->world.pos.x, actor->world.pos.y, actor->world.pos.z, MTXMODE_NEW);
    Matrix_RotateYS(actor->shape.rot.y, MTXMODE_APPLY);
    if (igos->swordBlurLow)
    {
        Matrix_Translate(-20.f, 47.f, 0.f, MTXMODE_APPLY);
        Matrix_RotateX(M_PI, MTXMODE_APPLY);
        Matrix_RotateZ(.47123894f, MTXMODE_APPLY);
    }
    else
    {
        Matrix_Translate(-19.f, 46.f, 17.f, MTXMODE_APPLY);
        Matrix_RotateX(-.5497787f, MTXMODE_APPLY);
        Matrix_RotateZ(-1.1780972f, MTXMODE_APPLY);
        Matrix_RotateY(2.9059734f, MTXMODE_APPLY);
    }
    f32 scale = king ? .021f * 1.3076924f : .021f;
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    gSPDisplayList(POLY_XLU_DISP++, 0x08000000);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, king ? 180 : 255, 255, king ? 255 : 200, igos->swordBlurAlpha);
    gSPMatrix(POLY_XLU_DISP++, Duel_ToMtx(GRAPH_ALLOC(gfx, sizeof(Mtx))), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(POLY_XLU_DISP++, models[igos->swordBlurFrame]);
    Matrix_Pop();
    CLOSE_DISPS();
    igos->swordBlurAlpha = MAX(0, igos->swordBlurAlpha - 40);
    if (igos->swordBlurFrame < 2)
        ++igos->swordBlurFrame;
}

static void Igos_DrawShieldFlash(StandaloneIgos* igos, PlayState* play)
{
    if (igos->shieldFlashScale <= .01f || igos->shieldPoseFrame != play->gameplayFrames)
        return;
#if defined(GAME_OOT)
    u32 material = (u32)comboEnemyKeepAsset(0x1001, 0x23348) & 0x1fffffff;
    u32 model = (u32)comboEnemyKeepAsset(0x1001, 0x23428) & 0x1fffffff;
    if (!material || !model)
        return;
#else
    const u32 material = 0x04023348;
    const u32 model = 0x04023428;
#endif
    GraphicsContext* gfx = play->state.gfxCtx;
    Gfx_SetupDL25_Xlu(gfx);
    OPEN_DISPS(gfx);
    Matrix_Push();
    Matrix_Translate(igos->shieldFlashPos.x, igos->shieldFlashPos.y, igos->shieldFlashPos.z, MTXMODE_NEW);
    Matrix_ReplaceRotation(&play->billboardMtxF);
    Matrix_RotateZ((s16)(play->gameplayFrames * 0x80) * (M_PI / 32768.f), MTXMODE_APPLY);
    f32 scale = 6.f * igos->shieldFlashScale;
    Matrix_Scale(scale, scale, 1.f, MTXMODE_APPLY);
    gSPDisplayList(POLY_XLU_DISP++, material);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 220, 170);
    gDPSetEnvColor(POLY_XLU_DISP++, 255, 255, 100, 128);
    gSPMatrix(POLY_XLU_DISP++, Duel_ToMtx(GRAPH_ALLOC(gfx, sizeof(Mtx))), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(POLY_XLU_DISP++, model);
    Matrix_Pop();
    CLOSE_DISPS();
}

static void Igos_DrawAttackEffects(StandaloneDuel* duel, PlayState* play)
{
    StandaloneIgos* igos = (StandaloneIgos*)duel;
    int hasBreath = 0;
    for (int i = 0; i < IGOS_BREATH_PUFFS; ++i)
        hasBreath |= igos->puffs[i].alpha;
    GraphicsContext* gfx = play->state.gfxCtx;
#if defined(GAME_OOT)
    u32 material = 0, model = 0;
    if (hasBreath)
    {
        material = (u32)comboEnemyKeepAsset(0x1001, 0x51180) & 0x1fffffff;
        model = (u32)comboEnemyKeepAsset(0x1001, 0x51238) & 0x1fffffff;
    }
#else
    const u32 material = 0x04051180;
    const u32 model = 0x04051238;
#endif
    OPEN_DISPS(gfx);
    if (hasBreath && material && model)
    {
        Gfx_SetupDL25_Xlu(gfx);
        gDPPipeSync(POLY_XLU_DISP++);
        gDPSetColorDither(POLY_XLU_DISP++, G_CD_BAYER);
        gDPSetAlphaDither(POLY_XLU_DISP++, G_AD_PATTERN);
        gSPDisplayList(POLY_XLU_DISP++, material);
        gDPSetEnvColor(POLY_XLU_DISP++, 155, 145, 155, 128);
        Matrix_Push();
        for (int i = 0; i < IGOS_BREATH_PUFFS; ++i)
        {
            IgosBreathPuff* puff = &igos->puffs[i];
            if (!puff->alpha)
                continue;
            f32 scale = puff->scale;
            u32 scroll = puff->age;
            Vec3f pos = puff->pos;
            gDPPipeSync(POLY_XLU_DISP++);
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 155, 155, 255, puff->alpha);
            gSPSegment(POLY_XLU_DISP++, 8, Gfx_TwoTexScroll(gfx, 0, scroll * 3, scroll * 15, 32, 64, 1, 0, 0, 32, 32));
            Matrix_Translate(pos.x, pos.y, pos.z, MTXMODE_NEW);
            Matrix_ReplaceRotation(&play->billboardMtxF);
            Matrix_Scale(scale, scale, 1.f, MTXMODE_APPLY);
            gSPMatrix(POLY_XLU_DISP++, Duel_ToMtx(GRAPH_ALLOC(gfx, sizeof(Mtx))),
                      G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
            gSPDisplayList(POLY_XLU_DISP++, model);
        }
        Matrix_Pop();
    }
    CLOSE_DISPS();
}

static void Slime_ConeVertex(Vtx* vertex, int ring, int inverse, int wide, f32 blend)
{
    if (!ring || ring == 12 || blend <= 0.f)
        return;
    f32 x = vertex->n.ob[0], y = vertex->n.ob[1], z = vertex->n.ob[2];
    f32 radial = sqrtf(x * x + z * z), radialScale = 1.f, targetY;
    if (wide)
    {
        if (ring < 6)
        {
            targetY = ((Math_CosS(ring * (0x8000 / 6)) + 1.f) * .925f - .85f) * 1000.f;
            radialScale = 200.f * ring / radial;
        }
        else
        {
            targetY = (Math_CosS((ring - 6) * (0x8000 / 16)) * .05f - 1.f) * 1000.f;
            radialScale = 125.f * (14 - ring) / radial;
        }
    }
    else if (inverse)
    {
        if (ring < 6)
        {
            targetY = (Math_CosS((ring - 1) * (0x8000 / 10)) * .1f + .9f) * 1000.f;
        }
        else
        {
            targetY = ((Math_CosS((ring - 6) * (0x8000 / 5)) + 1.f) * .925f - 1.f) * 1000.f;
            radialScale = 125.f * (12 - ring) / radial;
        }
    }
    else if (ring < 6)
    {
        targetY = ((Math_CosS((ring - 1) * (0x8000 / 5)) + 1.f) * .925f - .85f) * 1000.f;
        radialScale = 150.f * ring / radial;
    }
    else
    {
        targetY = (Math_CosS((ring - 6) * (0x8000 / 12)) * .05f - 1.f) * 1000.f;
    }
    vertex->n.ob[0] = x * (1.f + (radialScale - 1.f) * blend);
    vertex->n.ob[1] = y + (targetY - y) * blend;
    vertex->n.ob[2] = z * (1.f + (radialScale - 1.f) * blend);
}

static void Slime_UpdateMeshNormals(Vtx* vertices)
{

    Vec3f normals[ARRAY_COUNT(sGekkoSlimeVertices)] = { 0 };
    for (int i = 0; i < ARRAY_COUNT(sSlimeTriangles); ++i)
    {
        const u8* triangle = sSlimeTriangles[i];
        const s16* a = vertices[triangle[0]].n.ob;
        const s16* b = vertices[triangle[1]].n.ob;
        const s16* c = vertices[triangle[2]].n.ob;
        Vec3f u = { b[0] - a[0], b[1] - a[1], b[2] - a[2] };
        Vec3f v = { c[0] - a[0], c[1] - a[1], c[2] - a[2] };
        Vec3f face = { u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x };
        f32 length = sqrtf(face.x * face.x + face.y * face.y + face.z * face.z);
        if (length < 1.f)
            continue;
        f32 scale = 1.f / length;
        for (int j = 0; j < 3; ++j)
        {
            Vec3f* normal = &normals[triangle[j]];
            normal->x += face.x * scale;
            normal->y += face.y * scale;
            normal->z += face.z * scale;
        }
    }
    for (int i = 0; i < ARRAY_COUNT(normals); ++i)
    {
        Vec3f* n = &normals[i];
        f32 length = sqrtf(n->x * n->x + n->y * n->y + n->z * n->z);
        if (length < .001f)
            continue;
        f32 scale = 120.f / length;
        vertices[i].n.n[0] = n->x * scale;
        vertices[i].n.n[1] = n->y * scale;
        vertices[i].n.n[2] = n->z * scale;
    }
}

#if defined(GAME_OOT)
extern void AnimatedMat_DrawStepOpa(PlayState*, AnimatedMaterial*, u32);
#define Duel_DrawMaterialStep AnimatedMat_DrawStepOpa
#else
#define Duel_DrawMaterialStep ((void (*)(PlayState*, AnimatedMaterial*, u32))0x801318c8)
#endif

static s32 Gomess_OverrideLimb(PlayState* play, s32 limb, Gfx** dList, Vec3f* pos, Vec3s* rot, void* arg)
{
    StandaloneGomess* gomess = arg;
    if (limb == 2 || (gomess->droppedScythe && (limb == 20 || limb == 21)))
        *dList = NULL;
    if (gomess->duel.phase == DUEL_DEFEAT && gomess->deathStage == GOMESS_DEATH_DISSOLVE)
    {
        static const u8 hideAt[] = {
            30, 30, 10, 2, 12, 17, 17, 17, 17, 17, 17, 17, 14, 10, 12, 9, 9, 12, 9, 9, 30, 30
        };
        int age = 26 - gomess->duel.timer;
        if (limb < ARRAY_COUNT(hideAt) && age >= hideAt[limb])
            *dList = NULL;
        else if (limb == 13 && age > 5)
        {
            f32 scale = 1.f - .1f * (age - 5);
            Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
        }
    }
    if (limb == 12)
        rot->z += gomess->cloakUpperRot;
    if (limb == 13)
        rot->z += gomess->cloakLowerRot;
    return 0;
}

static void Gomess_DrawLimb(PlayState* play, s32 limb, Gfx** dList, Vec3s* rot, void* arg)
{
    StandaloneGomess* gomess = arg;
    StandaloneDuel* duel = &gomess->duel;
    static const s8 partIndex[] = { -1, -1, -1, 12, -1, 0, -1, -1, -1, -1, -1, -1, -1, 7, 1, 2, 3, 4, 5, 6, -1, -1 };
    if (gomess->lightOrbTimer && (u32)limb < ARRAY_COUNT(partIndex) && partIndex[limb] >= 0)
    {
        int index = partIndex[limb];
        if (index == 7)
        {
            static Vec3f offsets[] = { { 1000.f, 2000.f, 0.f },
                                       { 1500.f, 1000.f, 4000.f },
                                       { 1500.f, 1000.f, -4000.f },
                                       { 4000.f, 4000.f, 2000.f },
                                       { 4000.f, 4000.f, -2000.f } };
            for (int i = 0; i < ARRAY_COUNT(offsets); ++i)
                Matrix_MultVec3f(&offsets[i], &gomess->damageEffectPos[index + i]);
        }
        else
        {
            const MtxF* pose = Matrix_GetCurrent();
            static Vec3f lower = { 0.f, -2000.f, 0.f };
            gomess->damageEffectPos[index] = (Vec3f){ pose->xw, pose->yw, pose->zw };
            if (index == 12)
                Matrix_MultVec3f(&lower, &gomess->damageEffectPos[13]);
        }
    }
    if (limb == 2)
    {
        const MtxF* pose = Matrix_GetCurrent();
        gomess->corePos = (Vec3f){ pose->xw, pose->yw, pose->zw };
        duel->actor.focus.pos = gomess->corePos;
        gomess->corePoseValid = 1;
        gomess->corePoseFrame = play->gameplayFrames;
        Matrix_Push();
        Matrix_ReplaceRotation(&play->billboardMtxF);
        OPEN_DISPS(play->state.gfxCtx);
        gSPMatrix(POLY_OPA_DISP++, Duel_ToMtx(GRAPH_ALLOC(play->state.gfxCtx, sizeof(Mtx))),
                  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
        gSPDisplayList(POLY_OPA_DISP++, 0x06009988);
        if (duel->phase != DUEL_DEFEAT && Gomess_CrowdCount(gomess) >= 5)
            gSPDisplayList(POLY_OPA_DISP++, 0x06009ba0);
        CLOSE_DISPS();
        Matrix_Pop();
    }
    else if (limb == 20 &&
             ((duel->phase == DUEL_DEFEAT && !gomess->droppedScythe) ||
              (gomess->scytheAfterImages > 1 && (duel->phase == GOMESS_GUARD || duel->phase == GOMESS_SPIN))))
    {
        Matrix_Get(&gomess->scytheHandleMtx);
        gomess->scytheHandlePoseFrame = play->gameplayFrames;
    }
    if (limb != 21)
        return;
    if (!gomess->droppedScythe)
    {
        const MtxF* matrix = Matrix_GetCurrent();
        gomess->scytheWorldPos = (Vec3f){ matrix->xw, matrix->yw, matrix->zw };
        gomess->scytheSoundPoseValid = 1;
    }
    int swing = duel->phase == GOMESS_SWING_END && duel->skel.curFrame <= 21.f;
    int spinning = duel->phase == GOMESS_GUARD || duel->phase == GOMESS_SPIN;
    void* trail = gomess->scytheTrail < 0 ? NULL : Effect_GetByIndex(gomess->scytheTrail);
    if (!swing && !spinning)
    {
        if (trail && gomess->scytheTrailActive)
        {
#if defined(GAME_MM)
            ((void (*)(void*))0x800a8514)(trail);
#else
            EffectBlure_AddSpace(trail);
#endif
        }
        gomess->scytheTrailActive = 0;
        return;
    }

    int kind = swing ? 1 : 2;
    if (gomess->scythePoseKind == kind && gomess->scythePoseFrame == play->gameplayFrames)
        return;
    Vec3f modelA = swing ? (Vec3f){ -1000.f, 12000.f, 0.f } : (Vec3f){ 0.f, 5000.f, 0.f };
    Vec3f modelB = swing ? (Vec3f){ 6000.f, 0.f, 0.f } : (Vec3f){ -2000.f, -2500.f, 0.f };
    Vec3f a, b;
    Matrix_MultVec3f(&modelA, &a);
    Matrix_MultVec3f(&modelB, &b);
    ColliderQuad* scythe = &gomess->scythe;
    if (gomess->scythePoseKind == kind && play->gameplayFrames - gomess->scythePoseFrame == 1)
    {
        Vec3f previousA = scythe->dim.quad[0];
        Vec3f previousB = scythe->dim.quad[1];
        Collider_SetQuadVertices(scythe, &a, &b, &previousA, &previousB);
        if (trail)
        {
            EffectBlure_AddVertex(trail, &b, &a);
            gomess->scytheTrailActive = 1;
        }
    }
    else
    {
        Collider_SetQuadVertices(scythe, &a, &b, &a, &b);
    }
    gomess->scythePoseFrame = play->gameplayFrames;
    gomess->scythePoseKind = kind;
}

#if defined(GAME_OOT)
static void Slime_DrawMeltSmoke(StandaloneSlime* slime, PlayState* play)
{
    int hasSmoke = 0;
    for (int i = 0; i < ARRAY_COUNT(slime->meltSmoke); ++i)
        hasSmoke |= slime->meltSmoke[i].life;
    if (!hasSmoke)
        return;
    u32 material = (u32)comboEnemyKeepAsset(0x1001, 0x51180) & 0x1fffffff;
    u32 model = (u32)comboEnemyKeepAsset(0x1001, 0x51238) & 0x1fffffff;
    if (!material || !model)
        return;
    GraphicsContext* gfx = play->state.gfxCtx;
    OPEN_DISPS(gfx);
    Gfx_SetupDL25_Xlu(gfx);
    gSPDisplayList(POLY_XLU_DISP++, material);
    gDPSetEnvColor(POLY_XLU_DISP++, 195, 235, 235, 100);
    Matrix_Push();
    for (int i = 0; i < ARRAY_COUNT(slime->meltSmoke); ++i)
    {
        int life = slime->meltSmoke[i].life;
        if (!life)
            continue;
        Vec3f* pos = &slime->meltSmoke[i].pos;
        u8 alpha = MIN(100, 10 * MIN(life, 50 - life));
        gDPPipeSync(POLY_XLU_DISP++);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 195, 235, 235, alpha);
        gSPSegment(POLY_XLU_DISP++, 8, Gfx_TwoTexScroll(gfx, 0, life * 3, life * 15, 32, 64, 1, 0, 0, 32, 32));
        Matrix_Translate(pos->x, pos->y, pos->z, MTXMODE_NEW);
        Matrix_ReplaceRotation(&play->billboardMtxF);
        Matrix_Scale(.06f, .06f, 1.f, MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, Duel_ToMtx(GRAPH_ALLOC(gfx, sizeof(Mtx))),
                  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
        gSPDisplayList(POLY_XLU_DISP++, model);
    }
    Matrix_Pop();
    CLOSE_DISPS();
}
#endif

static void Slime_DrawShatterEffects(StandaloneSlime* slime, PlayState* play)
{
    GraphicsContext* gfx = play->state.gfxCtx;
    OPEN_DISPS(gfx);
    if (slime->shockwaveAlpha)
    {
        u32 frame = play->gameplayFrames;
        Vec3f* pos = &slime->shockwavePos;
        f32 scale = slime->shockwaveScale;
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 195, 225, 235, slime->shockwaveAlpha);
        gSPSegment(POLY_XLU_DISP++, 0x0d,
                   Gfx_TwoTexScroll(gfx, 0, frame % 128, (u8)(frame * 8), 32, 64, 1, (0u - frame * 2) & 63, 0, 16, 16));
        Matrix_Push();
        Matrix_Translate(pos->x, pos->y, pos->z, MTXMODE_NEW);
        Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, Duel_ToMtx(GRAPH_ALLOC(gfx, sizeof(Mtx))),
                  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
        gSPDisplayList(POLY_XLU_DISP++, 0x0600fb40);
        Matrix_Pop();
    }
    int materialSet = 0;
    for (int i = 0; i < ARRAY_COUNT(slime->iceShards); ++i)
    {
        SlimeIceShard* shard = &slime->iceShards[i];
        if (!shard->scale)
            continue;
        if (!materialSet)
        {
            gSPSegment(POLY_XLU_DISP++, 8,
                       Gfx_TwoTexScroll(gfx, 0, 0, 0, 64, 64, 1, 0, 0u - play->gameplayFrames, 32, 32));
            gSPDisplayList(POLY_XLU_DISP++, 0x06010f20);
            materialSet = 1;
        }
        Matrix_Push();
        Matrix_SetTranslateRotateYXZ(shard->pos.x, shard->pos.y, shard->pos.z, &shard->rot);
        Matrix_Scale(shard->scale, shard->scale, shard->scale, MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, Duel_ToMtx(GRAPH_ALLOC(gfx, sizeof(Mtx))),
                  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
        gSPDisplayList(POLY_XLU_DISP++, 0x06010fe0);
        Matrix_Pop();
    }
    CLOSE_DISPS();
}

static void Gomess_SetupScytheXlu(StandaloneGomess* gomess, PlayState* play)
{
    PreDraw2(&gomess->duel.actor, play, 0);
    Scene_SetRenderModeXlu(play, 1, 2);
}

static int Gomess_HasXluSpace(const GraphicsContext* gfx, size_t commands)
{
    return (uintptr_t)gfx->polyXlu.end >= (uintptr_t)gfx->polyXlu.append + (commands + 2) * sizeof(Gfx);
}

static void Gomess_DrawScytheAfterImages(StandaloneGomess* gomess, PlayState* play)
{
    u8 phase = gomess->duel.phase;
    if ((phase != GOMESS_GUARD && phase != GOMESS_SPIN) || gomess->scytheAfterImages <= 1 ||
        gomess->scytheHandlePoseFrame != play->gameplayFrames)
        return;

    GraphicsContext* gfx = play->state.gfxCtx;
    int images = gomess->scytheAfterImages - 1;
    if (!Gomess_HasXluSpace(gfx, 6 * images + 8))
        return;
    Mtx* matrices = Gomess_AllocDrawData(gfx, 2 * images * sizeof(*matrices), 0);
    if (!matrices)
        return;

    Gfx_SetupDL25_Xlu(gfx);
    Gomess_SetupScytheXlu(gomess, play);
    OPEN_DISPS(gfx);
    Matrix_Push();
    for (int i = 1; i < gomess->scytheAfterImages; ++i)
    {
        gDPPipeSync(POLY_XLU_DISP++);
        gDPSetEnvColor(POLY_XLU_DISP++, 30, 30, 0, 255 - i * 35);
        *Matrix_GetCurrent() = gomess->scytheHandleMtx;
        Matrix_RotateZYX(i * 0x2100, 0, 0, MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, Duel_ToMtx(matrices++), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
        gSPDisplayList(POLY_XLU_DISP++, 0x06006f88);
        Matrix_Translate(0.f, -1084.f, 7012.f, MTXMODE_APPLY);
        Matrix_RotateZYX(-0x4000, 0, -0x4000, MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, Duel_ToMtx(matrices++), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
        gSPDisplayList(POLY_XLU_DISP++, 0x060073d0);
    }
    Matrix_Pop();
    CLOSE_DISPS();
}

static void Gomess_DrawFlame(Gfx** xlu, PlayState* play, Vec3f* pos, u8 alpha, int index, Gfx** scrollLists,
                             const Mtx* billboard, int* lastPhase, int* lastAlpha)
{
#if defined(GAME_MM)
    const u32 model = 0x0407d590;
#else
    const u32 model = 0x04052a10;
#endif
    GraphicsContext* gfx = play->state.gfxCtx;
    index &= 63;
    int changePhase = index != *lastPhase;
    int changeAlpha = alpha != *lastAlpha;
    if (!Gomess_HasXluSpace(gfx, 2 + changePhase + changeAlpha))
        return;
    if (!scrollLists[index])
    {
        u32 scroll = ((play->gameplayFrames + index * 10u) * -20u) & 511;
        Gfx* tiles = Gomess_AllocDrawData(gfx, 5 * sizeof(*tiles), 0);
        if (!tiles)
            return;
        gDPTileSync(&tiles[0]);
        gDPSetTileSize(&tiles[1], 0, 0, 0, 31 * 4, 63 * 4);
        gDPTileSync(&tiles[2]);
        gDPSetTileSize(&tiles[3], 1, 0, scroll, 31 * 4, scroll + 127 * 4);
        gSPEndDisplayList(&tiles[4]);
        scrollLists[index] = tiles;
    }
    Mtx* matrix = Gomess_AllocDrawData(gfx, sizeof(Mtx), 0);
    if (!matrix)
        return;
    if (changeAlpha)
    {
        gDPSetPrimColor((*xlu)++, 0x80, 0x80, 165, 255, 215, alpha);
        *lastAlpha = alpha;
    }
    if (changePhase)
    {
        gSPSegment((*xlu)++, 8, scrollLists[index]);
        *lastPhase = index;
    }
    *matrix = *billboard;
    u16* words = (u16*)matrix;
    s32 xyz[3] = { pos->x * 65536.f, pos->y * 65536.f, pos->z * 65536.f };
    for (int axis = 0; axis < 3; ++axis)
    {
        words[12 + axis] = xyz[axis] >> 16;
        words[28 + axis] = xyz[axis];
    }
    gSPMatrix((*xlu)++, matrix, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList((*xlu)++, model);
}

static int Gomess_BatVisible(PlayState* play, EnemyGomessBat* bat)
{
    Vec3f projected;
    f32 w;
    SkinMatrix_Vec3fMtxFMultXYZW(&play->viewProjectionMtxF, &bat->actor.world.pos, &projected, &w);
#if defined(GAME_MM)
    return ((s32(*)(PlayState*, Actor*, Vec3f*, f32))0x800ba2fc)(play, &bat->actor, &projected, w);
#else
    return ((s32(*)(PlayState*, Actor*, Vec3f*, f32))0x800249dc)(play, &bat->actor, &projected, w);
#endif
}

static void Gomess_DrawDefeatFlames(StandaloneGomess* gomess, PlayState* play, u32 visibleBats)
{
    if (gomess->duel.phase != DUEL_DEFEAT || gomess->deathStage < GOMESS_DEATH_DISSOLVE)
        return;

    int numFlames = 0;
    if (gomess->deathStage == GOMESS_DEATH_DISSOLVE)
        numFlames = MIN((26 - gomess->duel.timer) * 7, ARRAY_COUNT(gomess->defeatFlameAlpha));
    if (!visibleBats && numFlames <= 0)
        return;

    GraphicsContext* gfx = play->state.gfxCtx;
    if (!Gomess_HasXluSpace(gfx, 2))
        return;
    Gfx* scrollLists[64] = { 0 };
    int lastPhase = -1;
    int lastAlpha = -1;
    Gfx_SetupDL25_Xlu(gfx);
    OPEN_DISPS(gfx);
    gDPSetEnvColor(POLY_XLU_DISP++, 230, 135, 25, 0);
    Matrix_Push();
    Matrix_Translate(0.f, 0.f, 0.f, MTXMODE_NEW);
    Matrix_ReplaceRotation(&play->billboardMtxF);
    Matrix_Scale(.00405f, .003f, .00405f, MTXMODE_APPLY);
    Mtx billboard;
    Duel_ToMtx(&billboard);
    if (numFlames > 0)
    {
        for (int i = 0; i < numFlames; ++i)
        {
            if (gomess->defeatFlameAlpha[i])
                Gomess_DrawFlame(&POLY_XLU_DISP, play, &gomess->defeatFlamePos[i], gomess->defeatFlameAlpha[i], i,
                                 scrollLists, &billboard, &lastPhase, &lastAlpha);
        }
    }
    for (int i = 0; i < GOMESS_BAT_COUNT; ++i)
    {
        EnemyGomessBat* bat = gomess->bats[i];
        if (!(visibleBats & (1u << i)) || !bat->deathAlpha)
            continue;
        for (int j = 0; j < ARRAY_COUNT(bat->effects); ++j)
        {
            Vec3f pos = bat->effects[j];
            pos.y -= 12.f;
            Gomess_DrawFlame(&POLY_XLU_DISP, play, &pos, bat->deathAlpha, i + j, scrollLists, &billboard, &lastPhase,
                             &lastAlpha);
        }
    }
    Matrix_Pop();
    CLOSE_DISPS();
}

static void Gomess_DrawHitAndDefeatEffects(StandaloneGomess* gomess, PlayState* play, int batMaterialReady)
{
    GraphicsContext* gfx = play->state.gfxCtx;
    StandaloneDuel* duel = &gomess->duel;
    if (!gomess->lightOrbTimer && !gomess->looseCore && duel->phase != DUEL_DEFEAT)
        return;
    if (gomess->lightOrbTimer && gomess->corePoseValid)
    {
        f32 alpha = gomess->lightOrbTimer * .05f;
        Gekko_DrawDamageEffects(play, &duel->actor, gomess->damageEffectPos, ARRAY_COUNT(gomess->damageEffectPos),
                                MIN(.8f, (alpha + 1.f) * .4f), 0.f, alpha, GEKKO_DRAW_LIGHT);
    }
    int drawCore = gomess->looseCore || (duel->phase == DUEL_DEFEAT && gomess->deathStage == GOMESS_DEATH_DROP_CORE);
    if (!drawCore && !gomess->droppedScythe)
        return;
    Mtx* matrices = Gomess_AllocDrawData(gfx, (drawCore + (gomess->droppedScythe ? 2 : 0)) * sizeof(*matrices), 8);
    if (!matrices)
        return;

    OPEN_DISPS(gfx);
    Matrix_Push();
    if (drawCore)
    {
        Vec3f* pos = &gomess->looseCorePos;
        Matrix_Translate(pos->x, pos->y, pos->z, MTXMODE_NEW);
        if (gomess->looseCore)
        {
            Matrix_RotateZYX(0x4000, gomess->looseCoreRotation, 0, MTXMODE_APPLY);
            Matrix_Scale(.007f, .007f, .007f, MTXMODE_APPLY);
            if (!batMaterialReady)
            {
                Gfx_SetupDL25_Opa(gfx);
                gSPDisplayList(POLY_OPA_DISP++, 0x06009f10);
            }
        }
        else
        {
            Matrix_ReplaceRotation(&play->billboardMtxF);
            Matrix_Scale(.01f, .01f, .01f, MTXMODE_APPLY);
        }
        gSPMatrix(POLY_OPA_DISP++, Duel_ToMtx(matrices++), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
        gSPDisplayList(POLY_OPA_DISP++, gomess->looseCore ? 0x06009f88 : 0x06009988);
    }
    if (gomess->droppedScythe && (gomess->deathStage != GOMESS_DEATH_FADE || Gomess_HasXluSpace(gfx, 14)))
    {
        int fading = gomess->deathStage == GOMESS_DEATH_FADE;
        if (fading)
        {
#if defined(GAME_OOT)
            ((void (*)(GraphicsContext*))0x8007e404)(gfx);
#else
            Gfx_SetupDL44_Xlu(gfx);
#endif
            Gomess_SetupScytheXlu(gomess, play);
        }
        Gfx* dl = fading ? POLY_XLU_DISP : POLY_OPA_DISP;
        if (fading)
        {
            gDPPipeSync(dl++);
            gDPSetEnvColor(dl++, 30, 30, 0, duel->timer * 5);
        }
        *Matrix_GetCurrent() = gomess->droppedScytheMtx;
        gSPMatrix(dl++, Duel_ToMtx(matrices++), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
        gSPDisplayList(dl++, 0x06006f88);
        Matrix_Translate(0.f, -1084.f, 7012.f, MTXMODE_APPLY);
        Matrix_RotateZYX(-0x4000, 0, -0x4000, MTXMODE_APPLY);
        gSPMatrix(dl++, Duel_ToMtx(matrices++), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
        gSPDisplayList(dl++, 0x060073d0);
        if (fading)
            POLY_XLU_DISP = dl;
        else
            POLY_OPA_DISP = dl;
    }
    Matrix_Pop();
    CLOSE_DISPS();
}

static void Duel_Draw(Actor* actor, PlayState* play)
{
    static Gfx material[] = { gsDPSetPrimColor(0, 0, 110, 60, 130, 255), gsSPEndDisplayList() };
    if (DUEL_FIXED_FORM == DUEL_IGOS && (u16)actor->params == 0x837f)
    {
        Igos_DrawFlyingHead((IgosFlyingHead*)actor, play);
        return;
    }
    StandaloneDuel* duel = (StandaloneDuel*)actor;
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    if (DUEL_FIXED_FORM == DUEL_GARO)
    {
        Gfx_SetupDL25_Xlu(play->state.gfxCtx);
#if defined(GAME_MM)
        Scene_SetRenderModeXlu(play, 0, 1);
#endif
    }
    OPEN_DISPS(play->state.gfxCtx);
    if (DUEL_FIXED_FORM == DUEL_IGOS)
    {
        gSPSegment(POLY_OPA_DISP++, 9, sIgosOpaqueMaterial);
        gSPSegment(POLY_OPA_DISP++, 10, sIgosOpaqueMaterial);
    }
    else if (DUEL_FIXED_FORM != DUEL_SLIME && DUEL_FIXED_FORM != DUEL_SNAPPER)
    {
        for (int i = 8; i <= 11; ++i)
            gSPSegment(POLY_OPA_DISP++, i, material);
    }
    if (DUEL_FIXED_FORM == DUEL_GOMESS)
    {
        StandaloneGomess* gomess = (StandaloneGomess*)duel;
        if (duel->phase != DUEL_DEFEAT || gomess->deathStage < GOMESS_DEATH_DROP_CORE)
            Duel_DrawMaterialStep(play, (AnimatedMaterial*)(gSegments[6] + K0BASE + 0xcbc0), 23);
        if (duel->phase != DUEL_DEFEAT || gomess->deathStage <= GOMESS_DEATH_DROP_CORE)
            Duel_DrawMaterialStep(play, (AnimatedMaterial*)(gSegments[6] + K0BASE + 0xcb84), play->gameplayFrames);
        if (duel->phase != DUEL_DEFEAT || gomess->deathStage <= GOMESS_DEATH_DROP_CORE)
        {
            Scene_SetRenderModeXlu(play, 0, 1);
            gDPSetEnvColor(POLY_OPA_DISP++, 30, 30, 0, 255);
        }
    }
    int igosDefeated = DUEL_FIXED_FORM == DUEL_IGOS && duel->phase == DUEL_DEFEAT;
    int igosFlash = DUEL_FIXED_FORM == DUEL_IGOS && (((StandaloneIgos*)duel)->damageFlashTimer & 1);
    if (igosDefeated || igosFlash)
    {
        s32 red = igosDefeated ? 0 : 255;
#if defined(GAME_OOT)
        POLY_OPA_DISP =
            ((Gfx * (*)(Gfx*, s32, s32, s32, s32, s32, s32))0x8007dc40u)(POLY_OPA_DISP, red, 0, 0, 255, 900, 1099);
#else
        POLY_OPA_DISP = Gfx_SetFog(POLY_OPA_DISP, red, 0, 0, 255, 900, 1099);
#endif
    }
    int gekkoFilter = (DUEL_FIXED_FORM == DUEL_SLIME || DUEL_FIXED_FORM == DUEL_SNAPPER) &&
                      (duel->phase == DUEL_HURT || duel->phase == DUEL_DEFEAT || actor->colorFilterTimer);
    if (gekkoFilter)
    {
        Color_RGBA8 color = { 255, 0, 0, 0 };
        s16 timer = duel->phase == DUEL_DEFEAT ? 20 : duel->timer;
        s16 duration = 20;
        if (duel->phase != DUEL_DEFEAT && (duel->phase != DUEL_HURT || Gekko_GetDamage(duel)->stun))
        {
            timer = actor->colorFilterTimer;
            duration = MAX(1, actor->colorFilterParams & 0xff);
            if (!(actor->colorFilterParams & 0x4000))
                color = (Color_RGBA8){ 0, 0, 255, 0 };
        }

        gDPPipeSync(POLY_OPA_DISP++);
        gDPSetFogColor(POLY_OPA_DISP++, color.r, color.g, color.b, color.a);
        gSPFogPosition(POLY_OPA_DISP++, 0, (s16)(2800.f * fabsf(Math_CosS((0x4000 / duration) * timer))) + 1700);
    }
    int slimeGrab = DUEL_FIXED_FORM == DUEL_SLIME && (duel->phase == SLIME_GRAB || duel->phase == SLIME_THROW);
    if (slimeGrab)
    {
        StandaloneSlime* slime = (StandaloneSlime*)duel;
        s16 yaw = duel->phase == SLIME_THROW ? actor->world.rot.y : slime->grabYaw;
        Matrix_Push();
        f32 frogY = actor->world.pos.y + slime->grabOffset * .8f;
        if (actor->floorPoly)
            frogY = MAX(frogY, actor->floorHeight);
        Matrix_Translate(actor->world.pos.x + slime->grabOffset * Math_SinS(yaw), frogY,
                         actor->world.pos.z + slime->grabOffset * Math_CosS(yaw), MTXMODE_NEW);
        Matrix_RotateY(yaw * (M_PI / 32768.f), MTXMODE_APPLY);
        Matrix_Scale(actor->scale.x, actor->scale.y, actor->scale.z, MTXMODE_APPLY);
    }
    int slimeTilt = DUEL_FIXED_FORM == DUEL_SLIME &&
                    (duel->phase == SLIME_RISE || duel->phase == SLIME_FALL || duel->phase == SLIME_FROZEN ||
                     slimeGrab || (duel->phase == SLIME_GROUND && ((StandaloneSlime*)duel)->squishRecovery));
    if (slimeTilt)
    {
        Matrix_Push();
        Matrix_RotateX(((StandaloneSlime*)duel)->shellGekkoPitch * (M_PI / 32768.f), MTXMODE_APPLY);
    }
    if (DUEL_FIXED_FORM == DUEL_SNAPPER && ((StandaloneSnapper*)duel)->wallSpinYaw)
        Matrix_RotateY(((StandaloneSnapper*)duel)->wallSpinYaw * (M_PI / 32768.f), MTXMODE_APPLY);
    if (DUEL_FIXED_FORM == DUEL_KEETA)
    {
        SkelAnime_DrawOpa(play, duel->skel.skeleton, duel->skel.jointTable, NULL, Keeta_DrawLimb, actor);
        Keeta_DrawDamage((StandaloneKeeta*)duel, play);
    }
    else if (DUEL_FIXED_FORM == DUEL_IGOS && ((StandaloneIgos*)duel)->bodyAlpha < 254)
        Igos_DrawTranslucentBody((StandaloneIgos*)duel, play);
    else if (DUEL_FIXED_FORM != DUEL_GOMESS || duel->phase != DUEL_DEFEAT ||
             ((StandaloneGomess*)duel)->deathStage < GOMESS_DEATH_DROP_CORE)
        SkelAnime_DrawFlexOpa(play, duel->skel.skeleton, duel->skel.jointTable, duel->skel.dListCount,
                              DUEL_FIXED_FORM == DUEL_IGOS     ? Igos_OverrideLimb
                              : DUEL_FIXED_FORM == DUEL_WART   ? Wart_OverrideLimb
                              : DUEL_FIXED_FORM == DUEL_GOMESS ? Gomess_OverrideLimb
                              : DUEL_FIXED_FORM == DUEL_GARO   ? Garo_OverrideLimb
                                                               : NULL,
                              DUEL_FIXED_FORM == DUEL_GARO                                         ? Garo_DrawLimb
                              : DUEL_FIXED_FORM == DUEL_IGOS                                       ? Igos_DrawLimb
                              : DUEL_FIXED_FORM == DUEL_GOMESS                                     ? Gomess_DrawLimb
                              : DUEL_FIXED_FORM == DUEL_WART                                       ? Wart_DrawLimb
                              : (DUEL_FIXED_FORM == DUEL_SNAPPER || DUEL_FIXED_FORM == DUEL_SLIME) ? Gekko_DrawLimb
                                                                                                   : NULL,
                              actor);
    if (igosDefeated || igosFlash || gekkoFilter)
    {
        gDPPipeSync(POLY_OPA_DISP++);
#if defined(GAME_OOT)
        POLY_OPA_DISP = ((Gfx * (*)(PlayState*, Gfx*))0x8009a574u)(play, POLY_OPA_DISP);
#else
        POLY_OPA_DISP = Play_SetFog(play, POLY_OPA_DISP);
#endif
    }
    if (DUEL_FIXED_FORM == DUEL_SNAPPER || DUEL_FIXED_FORM == DUEL_SLIME)
        Gekko_DrawEffects(duel, play);
#if defined(GARO_MASTER_TELEPORT)
    Garo_DrawAfterimages((StandaloneGaroMaster*)duel, play);
    Garo_DrawDamage((StandaloneGaroMaster*)duel, play);
#endif
    if (DUEL_FIXED_FORM == DUEL_GOMESS)
    {
        StandaloneGomess* gomess = (StandaloneGomess*)duel;
        u32 visibleBats = 0;
        int batMaterialReady = 0;
        GomessBatDrawPose batPose;
        batPose.valid = 0;
        for (int i = 0; i < GOMESS_BAT_COUNT; ++i)
        {
            EnemyGomessBat* bat = gomess->bats[i];
            if (!bat || !bat->deathAlpha || bat->actor.init || !bat->actor.update)
                continue;
            u32 effects = GomessBat_VisibleEffects(bat);
            if ((!effects && !bat->deathStage) || !Gomess_BatVisible(play, bat))
                continue;
            visibleBats |= 1u << i;
            GomessBat_DrawEffects(bat, play, &batMaterialReady, &batPose, effects);
        }
        if (batMaterialReady)
            Matrix_Pop();
        Gomess_DrawHitAndDefeatEffects(gomess, play, batMaterialReady);
        Gomess_DrawScytheAfterImages(gomess, play);
        Gomess_DrawDefeatFlames(gomess, play, visibleBats);
    }
    if (slimeTilt)
        Matrix_Pop();
    if (slimeGrab)
        Matrix_Pop();
    if (DUEL_FIXED_FORM == DUEL_SLIME && duel->phase != DUEL_DEFEAT)
    {
        StandaloneSlime* slime = (StandaloneSlime*)duel;
        if (duel->phase > SLIME_FORM && actor->floorPoly && actor->floorPoly->normal.y > 0x7000)
        {
            GraphicsContext* gfx = play->state.gfxCtx;
            f32 extent = slime->shellScaleY * 1000.f;
            f32 height = actor->world.pos.y + slime->shellOffsetY - extent - actor->floorHeight;
            f32 shade = 1.f - CLAMP(height, 0.f, 100.f) * (1.f / 1550.f);
#if defined(GAME_OOT)
            ((void (*)(GraphicsContext*))0x8007e404)(gfx);
#else
            Gfx_SetupDL44_Xlu(gfx);
#endif
            Matrix_Push();
            Matrix_Translate(actor->world.pos.x, actor->floorHeight + 1.f, actor->world.pos.z, MTXMODE_NEW);

            f32 radiusX = 0.f, radiusZ = 0.f;
            for (int i = 0; i < SLIME_VERTEX_COUNT; ++i)
            {
                radiusX = MAX(radiusX, ABS(slime->shellMesh[i].n.ob[0]));
                radiusZ = MAX(radiusZ, ABS(slime->shellMesh[i].n.ob[2]));
            }
            Matrix_Scale(radiusX * .0001f * slime->shellScaleXZ * shade, 1.f,
                         radiusZ * .0001f * slime->shellScaleXZ * shade, MTXMODE_APPLY);
            gSPSegment(POLY_XLU_DISP++, 6, gSegments[6] + K0BASE);
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 0, 0, 0, (u8)(175.f * shade));
            gSPMatrix(POLY_XLU_DISP++, Duel_ToMtx(GRAPH_ALLOC(gfx, sizeof(Mtx))),
                      G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
            gSPDisplayList(POLY_XLU_DISP++, 0x06011050);
            Matrix_Pop();
        }
        Gfx_SetupDL25_Xlu(play->state.gfxCtx);
        if (duel->phase > SLIME_FORM || (duel->phase == SLIME_FORM && slime->shellScaleY > .001f))
        {
            size_t dataSize = sizeof(slime->shellMesh) + 29 * sizeof(Mtx);
            if (duel->phase == SLIME_FROZEN)
                dataSize += sizeof(slime->shellMesh) + 8 * sizeof(Gfx);
            GraphicsContext* gfx = play->state.gfxCtx;
            if ((uintptr_t)gfx->polyOpa.end < (uintptr_t)POLY_OPA_DISP + dataSize + 16 * sizeof(Gfx) ||
                (uintptr_t)gfx->polyXlu.end < (uintptr_t)POLY_XLU_DISP + 80 * sizeof(Gfx))
            {
                return;
            }
            PreDraw2(actor, play, 0);
            Vtx* vertices = GRAPH_ALLOC(play->state.gfxCtx, sizeof(sGekkoSlimeVertices));
            memcpy(vertices, slime->shellMesh, sizeof(slime->shellMesh));
            Matrix_Push();
            Matrix_Translate(actor->world.pos.x, actor->world.pos.y + slime->shellOffsetY, actor->world.pos.z,
                             MTXMODE_NEW);
            Matrix_Scale(slime->shellScaleXZ, slime->shellScaleY, slime->shellScaleXZ, MTXMODE_APPLY);
            gSPSegment(POLY_XLU_DISP++, 6, gSegments[6] + K0BASE);
            gSPSegment(POLY_XLU_DISP++, 9, vertices);
            gSPMatrix(POLY_XLU_DISP++, Duel_ToMtx(GRAPH_ALLOC(play->state.gfxCtx, sizeof(Mtx))),
                      G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
            gSPDisplayList(POLY_XLU_DISP++, 0x06010530);
            gSPDisplayList(POLY_XLU_DISP++, 0x060105e8);
            if (duel->phase == SLIME_FROZEN)
            {
                Vtx* ice = GRAPH_ALLOC(play->state.gfxCtx, sizeof(slime->shellMesh));
                memcpy(ice, slime->shellMesh, sizeof(slime->shellMesh));
                for (int i = 0; i < ARRAY_COUNT(slime->iceAlpha); ++i)
                    ice[i].n.a = slime->iceAlpha[i];
                u32 frame = play->gameplayFrames;
                gSPSegment(POLY_XLU_DISP++, 8,
                           Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, 0, 64, 64, 1, 0, 0u - frame, 32, 32));
                gSPSegment(POLY_XLU_DISP++, 9, ice);
                gSPDisplayList(POLY_XLU_DISP++, 0x06010b80);
                gSPDisplayList(POLY_XLU_DISP++, 0x060105e8);
            }
            static const f32 bubbleOffsets[] = {
                .3f,  .1f, .45f, .5f, .6f, .2f,  .4f,  .25f, .35f, .65f, .25f, .55f, .3f, .1f,
                .45f, .5f, .6f,  .2f, .4f, .15f, .35f, .65f, .25f, .3f,  .15f, .45f, .3f, .25f,
            };
            int bubbleStep = (duel->phase >= SLIME_FORM && duel->phase <= SLIME_FALL) ? 2 :
                             (duel->phase == SLIME_GROUND && slime->squishRecovery) ? 4 : 1;
            int bubbleCount = (duel->phase == SLIME_FALL || duel->phase == SLIME_GRAB ||
                               duel->phase == SLIME_THROW) ? 0 : ARRAY_COUNT(bubbleOffsets);
            for (int i = 0; i < bubbleCount; i += bubbleStep)
            {
                const s16* point = vertices[MIN(i * 6, SLIME_VERTEX_COUNT - 1)].n.ob;
                f32 offset = bubbleOffsets[i];
                Matrix_Translate(actor->world.pos.x + point[0] * slime->shellScaleXZ * offset,
                                 actor->world.pos.y + slime->shellOffsetY + point[1] * slime->shellScaleY * offset,
                                 actor->world.pos.z + point[2] * slime->shellScaleXZ * offset, MTXMODE_NEW);
                Matrix_ReplaceRotation(&play->billboardMtxF);
                Matrix_Scale(.005f, .005f, .005f, MTXMODE_APPLY);
                gSPMatrix(POLY_XLU_DISP++, Duel_ToMtx(GRAPH_ALLOC(play->state.gfxCtx, sizeof(Mtx))),
                          G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
                gSPDisplayList(POLY_XLU_DISP++, 0x0600f9d0);
            }
            Matrix_Pop();
        }
        Slime_DrawShatterEffects(slime, play);
#if defined(GAME_OOT)
        Slime_DrawMeltSmoke(slime, play);
#endif
    }
    if (DUEL_FIXED_FORM == DUEL_IGOS)
    {
        StandaloneIgos* igos = (StandaloneIgos*)duel;
        Igos_DrawRecoveryTrail(igos, play);
        Igos_DrawAttackEffects(duel, play);
        Igos_DrawSwordBlur(igos, play);
        Igos_DrawShieldFlash(igos, play);
        if (igos->elementalAlpha && igos->fallenPoseMask == 0x7fff && igos->fallenPoseFrame == play->gameplayFrames)
#if defined(GAME_OOT)
            FOREIGN_ENEMY_API_DAMAGE_EFFECTS(play, actor, igos->fallenParts, ARRAY_COUNT(igos->fallenParts),
                                             igos->elementalScale, igos->iceSteamScale, igos->elementalAlpha,
                                             igos->elementalType);
#else
            Actor_DrawDamageEffects(play, actor, igos->fallenParts, ARRAY_COUNT(igos->fallenParts),
                                    igos->elementalScale, igos->iceSteamScale, igos->elementalAlpha,
                                    igos->elementalType);
#endif
    }
    if (DUEL_FIXED_FORM == DUEL_WART && (duel->phase != DUEL_DEFEAT || duel->timer > 43))
    {
        StandaloneWart* wart = (StandaloneWart*)duel;
        Gfx_SetupDL25_Xlu(play->state.gfxCtx);
        gSPDisplayList(POLY_XLU_DISP++, 0x06003450);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, 150);
        u8 order[WART_BUBBLE_COUNT];
        f32 depth[WART_BUBBLE_COUNT];
        int count = 0;
        for (int i = 0; i < WART_BUBBLE_COUNT; ++i)
        {
            WartBubble* bubble = &wart->bubbles[i];
            if (bubble->mode == 2)
                continue;
            Vec3f projected;
            f32 w;
            SkinMatrix_Vec3fMtxFMultXYZW(&play->viewProjectionMtxF, &bubble->pos, &projected, &w);
            int at = count++;
            while (at > 0 && depth[at - 1] < projected.z)
            {
                depth[at] = depth[at - 1];
                order[at] = order[at - 1];
                --at;
            }
            depth[at] = projected.z;
            order[at] = i;
        }
        for (int i = 0; i < count; ++i)
        {
            WartBubble* bubble = &wart->bubbles[order[i]];
            f32 spin = bubble->spin * (M_PI / 32768.f);
            Matrix_Push();
            Matrix_Translate(bubble->pos.x, bubble->pos.y, bubble->pos.z, MTXMODE_NEW);
            Matrix_ReplaceRotation(&play->billboardMtxF);
            Matrix_Scale(bubble->scaleX, bubble->scaleY, 1.f, MTXMODE_APPLY);
            Matrix_RotateZ(spin, MTXMODE_APPLY);
            Matrix_Scale(.13f, .143f, .13f, MTXMODE_APPLY);
            Matrix_RotateZ(-spin, MTXMODE_APPLY);
            gSPMatrix(POLY_XLU_DISP++, Duel_ToMtx(GRAPH_ALLOC(play->state.gfxCtx, sizeof(Mtx))),
                      G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
            gSPDisplayList(POLY_XLU_DISP++, 0x060034c8);
            Matrix_Pop();
        }
    }
    CLOSE_DISPS();
}
