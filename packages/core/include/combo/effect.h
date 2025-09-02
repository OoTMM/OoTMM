#ifndef COMBO_EFFECT_H
#define COMBO_EFFECT_H

#include <combo/util.h>
#include <combo/math/vec.h>
#include <combo/common/color.h>

typedef enum
{
    KAKERA_COLOR_NONE = -1,
    KAKERA_COLOR_WHITE,
    KAKERA_COLOR_BROWN
}
KakeraColorIndex;

typedef struct Actor Actor;

typedef struct
{
    Vec3f       pos;
    Vec3f       velocity;
    Vec3f       accel;
    void*       update;
    void*       draw;
    Vec3f       vec;
    void*       gfx;
    Actor*      actor;
    s16         regs[13];
    u16         flags;
    s16         life;
    u8          priority;
    u8          type;
}
EffectSs;

ASSERT_SIZE(EffectSs, 0x60);
ASSERT_OFFSET(EffectSs, pos,        0x00);
ASSERT_OFFSET(EffectSs, velocity,   0x0c);
ASSERT_OFFSET(EffectSs, accel,      0x18);
ASSERT_OFFSET(EffectSs, update,     0x24);
ASSERT_OFFSET(EffectSs, draw,       0x28);
ASSERT_OFFSET(EffectSs, vec,        0x2c);
ASSERT_OFFSET(EffectSs, gfx,        0x38);
ASSERT_OFFSET(EffectSs, actor,      0x3c);
ASSERT_OFFSET(EffectSs, regs,       0x40);
ASSERT_OFFSET(EffectSs, flags,      0x5a);
ASSERT_OFFSET(EffectSs, life,       0x5c);
ASSERT_OFFSET(EffectSs, priority,   0x5e);
ASSERT_OFFSET(EffectSs, type,       0x5f);

typedef struct
{
    EffectSs*   data;
    u32         searchIndex;
    u32         size;
}
EffectSsTable;

extern EffectSsTable gEffectSsTable;

void EffectSs_Delete(EffectSs* effectSs);
void EffectSsBubble_Spawn(PlayState* play, Vec3f* pos, f32 yPosOffset, f32 yPosRandScale, f32 xzPosRandScale, f32 scale);
void EffectSsGRipple_Spawn(PlayState* play, Vec3f* pos, s16 radius, s16 radiusMax, s16 life);
void EffectSsGSplash_Spawn(PlayState* play, Vec3f* pos, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 type, s16 scale);
void EffectSsKakera_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* arg3, s16 gravity, s16 arg5, s16 arg6, s16 arg7, s16 arg8, s16 scale, s16 arg10, s16 arg11, s32 life, s16 colorIdx, s16 objId, Gfx* dList);
void func_800B0F18(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 scale, s16 scaleStep, s16 life);

#if defined(GAME_MM) // TODO: Move elsewhere

void EffectSsDeadDb_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* prim, Color_RGBA8* env, s16 scale, s16 scaleStep, s32 life);


#define EFFECT_BLURE_COLOR_COUNT 4

#define EFFECT_BLURE_FLAG_2         (1 << 1)
#define EFFECT_BLURE_FLAG_4         (1 << 2)
#define EFFECT_BLURE_FLAG_10        (1 << 4)

#define EFFECT_BLURE_ELEMENT_FLAG_1         (1 << 0)
#define EFFECT_BLURE_ELEMENT_FLAG_2         (1 << 1)
#define EFFECT_BLURE_ELEMENT_FLAG_4         (1 << 2)
#define EFFECT_BLURE_ELEMENT_FLAG_8         (1 << 3)
#define EFFECT_BLURE_ELEMENT_FLAG_10        (1 << 4)
#define EFFECT_BLURE_ELEMENT_FLAG_20        (1 << 5)

typedef enum EffectBlureDrawMode {
    /* 0 */ EFF_BLURE_DRAW_MODE_SIMPLE,
    /* 1 */ EFF_BLURE_DRAW_MODE_SIMPLE_ALT_COLORS,
    /* 2 */ EFF_BLURE_DRAW_MODE_SMOOTH,
    /* 3 */ EFF_BLURE_DRAW_MODE_MAX
} EffectBlureDrawMode;

typedef struct EffectBlureInit2 {
    /* 0x00 */ s32 calcMode;
    /* 0x04 */ u16 flags;
    /* 0x06 */ s16 addAngleChange;
    /* 0x08 */ u8 p1StartColor[EFFECT_BLURE_COLOR_COUNT];
    /* 0x0C */ u8 p2StartColor[EFFECT_BLURE_COLOR_COUNT];
    /* 0x10 */ u8 p1EndColor[EFFECT_BLURE_COLOR_COUNT];
    /* 0x14 */ u8 p2EndColor[EFFECT_BLURE_COLOR_COUNT];
    /* 0x18 */ u8 elemDuration;
    /* 0x19 */ u8 unkFlag;
    /* 0x1A */ u8 drawMode; // EffectBlureDrawMode enum
    /* 0x1B */ u8 mode4Param;
    /* 0x1C */ Color_RGBA8 altPrimColor; // used with EFF_BLURE_DRAW_MODE_SIMPLE_ALT_COLORS
    /* 0x20 */ Color_RGBA8 altEnvColor; // used with EFF_BLURE_DRAW_MODE_SIMPLE_ALT_COLORS
} EffectBlureInit2; // size = 0x24

typedef enum EffectType {
    /* 0 */ EFFECT_SPARK,
    /* 1 */ EFFECT_BLURE1,
    /* 2 */ EFFECT_BLURE2,
    /* 3 */ EFFECT_SHIELD_PARTICLE,
    /* 4 */ EFFECT_TIRE_MARK,
    /* 5 */ EFFECT_MAX
} EffectType;

void EffectBlure_AddVertex(void* this, Vec3f* p1, Vec3f* p2); // FIXME void* this => EffectBlure* this
void func_800AE5A0(PlayState* play); // FIXME eff_ss_dead

#else

#define DEADSOUND_REPEAT_MODE_OFF 1

typedef struct EffectBlureInit1 {
    /* 0x000 */ char unk_00[0x184];
    /* 0x184 */ u8 p1StartColor[4];
    /* 0x188 */ u8 p2StartColor[4];
    /* 0x18C */ u8 p1EndColor[4];
    /* 0x190 */ u8 p2EndColor[4];
    /* 0x194 */ s32 elemDuration;
    /* 0x198 */ s32 unkFlag;
    /* 0x19C */ s32 calcMode;
} EffectBlureInit1;

typedef enum EffectType {
    /* 0x00 */ EFFECT_SPARK,
    /* 0x01 */ EFFECT_BLURE1,
    /* 0x02 */ EFFECT_BLURE2,
    /* 0x03 */ EFFECT_SHIELD_PARTICLE
} EffectType;

typedef struct EffectBlureElement {
    /* 0x00 */ s32 state;
    /* 0x04 */ s32 timer;
    /* 0x08 */ Vec3s p1;
    /* 0x0E */ Vec3s p2;
    /* 0x14 */ u16 flags;
} EffectBlureElement;

typedef struct EffectBlure {
    /* 0x000 */ EffectBlureElement elements[16];
    /* 0x180 */ s32 calcMode;
    /* 0x184 */ f32 mode4Param;
    /* 0x188 */ u16 flags;
    /* 0x18A */ s16 addAngleChange;
    /* 0x18C */ s16 addAngle;
    /* 0x18E */ u8 p1StartColor[4];
    /* 0x192 */ u8 p2StartColor[4];
    /* 0x196 */ u8 p1EndColor[4];
    /* 0x19A */ u8 p2EndColor[4];
    /* 0x19E */ u8 numElements; // "now_edge_num"
    /* 0x19F */ u8 elemDuration;
    /* 0x1A0 */ u8 unkFlag;
    /* 0x1A1 */ u8 drawMode; // 0: simple; 1: simple with alt colors; 2+: smooth
    /* 0x1A2 */ Color_RGBA8 altPrimColor; // used with drawMode 1
    /* 0x1A6 */ Color_RGBA8 altEnvColor; // used with drawMode 1
} EffectBlure;

typedef struct GraphicsContext GraphicsContext;
void EffectBlure_AddVertex(EffectBlure* this, Vec3f* p1, Vec3f* p2);
void EffectBlure_AddSpace(EffectBlure* this);
void EffectBlure_Init1(void* thisx, void* initParamsx);
void EffectBlure_Init2(void* thisx, void* initParamsx);
void EffectBlure_Destroy(void* thisx);
s32 EffectBlure_Update(void* thisx);
void EffectBlure_Draw(void* thisx, GraphicsContext* gfxCtx);
void EffectShieldParticle_Init(void* thisx, void* initParamsx);
void EffectShieldParticle_Destroy(void* thisx);
s32 EffectShieldParticle_Update(void* thisx);
void EffectShieldParticle_Draw(void* thisx, GraphicsContext* gfxCtx);
void EffectSpark_Init(void* thisx, void* initParamsx);
void EffectSpark_Destroy(void* thisx);
s32 EffectSpark_Update(void* thisx);
void EffectSpark_Draw(void* thisx, GraphicsContext* gfxCtx);
void func_80026230(PlayState* play, Color_RGBA8* color, s16 arg2, s16 arg3);
void func_80026400(PlayState* play, Color_RGBA8* color, s16 arg2, s16 arg3);
void func_80026608(PlayState* play);
void func_80026690(PlayState* play, Color_RGBA8* color, s16 arg2, s16 arg3);
void func_80026860(PlayState* play, Color_RGBA8* color, s16 arg2, s16 arg3);
void func_80026A6C(PlayState* play);
PlayState* Effect_GetPlayState(void);
void* Effect_GetByIndex(s32 index);
void Effect_InitContext(PlayState* play);
void Effect_Add(PlayState* play, s32* pIndex, s32 type, u8 arg3, u8 arg4, void* initParams);
void Effect_DrawAll(GraphicsContext* gfxCtx);
void Effect_UpdateAll(PlayState* play);
void Effect_Delete(PlayState* play, s32 index);
void Effect_DeleteAll(PlayState* play);
void EffectSs_InitInfo(PlayState* play, s32 tableSize);
void EffectSs_ClearAll(PlayState* play);
void EffectSs_Delete(EffectSs* effectSs);
void EffectSs_Reset(EffectSs* effectSs);
void EffectSs_Insert(PlayState* play, EffectSs* effectSs);
void EffectSs_Spawn(PlayState* play, s32 type, s32 priority, void* initParams);
void EffectSs_UpdateAll(PlayState* play);
void EffectSs_DrawAll(PlayState* play);
s16 EffectSs_LerpInv(s16 a, s16 b, s32 weightInv);
s16 EffectSs_LerpS16(s16 a, s16 b, f32 weight);
u8 EffectSs_LerpU8(u8 a, u8 b, f32 weight);
void EffectSs_DrawGEffect(PlayState* play, EffectSs* this, void* texture);
void EffectSsDust_Spawn(PlayState* play, u16 drawFlags, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 scale, s16 scaleStep, s16 life, u8 updateMode);
void func_8002829C(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 scale, s16 scaleStep);
void func_80028304(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 scale, s16 scaleStep);
void func_8002836C(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 scale, s16 scaleStep, s16 life);
void func_800283D4(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 scale, s16 scaleStep, s16 life);
void func_8002843C(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 scale, s16 scaleStep, s16 life);
void func_800284A4(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 scale, s16 scaleStep);
void func_80028510(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 scale, s16 scaleStep);
void func_8002857C(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel);
void func_800285EC(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel);
void func_8002865C(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep);
void func_800286CC(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep);
void func_8002873C(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep, s16 life);
void func_800287AC(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep, s16 life);
void func_8002881C(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor, Color_RGBA8* envColor);
void func_80028858(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor, Color_RGBA8* envColor);
void func_80028990(PlayState* play, f32 randScale, Vec3f* srcPos);
void func_80028A54(PlayState* play, f32 randScale, Vec3f* srcPos);
void EffectSsKiraKira_SpawnSmallYellow(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel);
void EffectSsKiraKira_SpawnSmall(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor, Color_RGBA8* envColor);
void EffectSsKiraKira_SpawnDispersed(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 scale, s32 life);
void EffectSsKiraKira_SpawnFocused(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 scale, s32 life);
void EffectSsBomb_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel);
void EffectSsBomb2_SpawnFade(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel);
void EffectSsBomb2_SpawnLayered(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep);
void EffectSsBlast_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* innerColor, Color_RGBA8* outerColor, s16 scale, s16 scaleStep, s16 scaleStepDecay, s16 life);
void EffectSsBlast_SpawnWhiteShockwaveSetScale(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep, s16 life);
void EffectSsBlast_SpawnShockwaveSetColor(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* innerColor, Color_RGBA8* outerColor, s16 life);
void EffectSsBlast_SpawnWhiteShockwave(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel);
void EffectSsGSpk_SpawnAccel(PlayState* play, Actor* actor, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 scale, s16 scaleStep);
void EffectSsGSpk_SpawnNoAccel(PlayState* play, Actor* actor, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 scale, s16 scaleStep);
void EffectSsGSpk_SpawnFuse(PlayState* play, Actor* actor, Vec3f* pos, Vec3f* velocity, Vec3f* accel);
void EffectSsGSpk_SpawnRandColor(PlayState* play, Actor* actor, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep);
void EffectSsGSpk_SpawnSmall(PlayState* play, Actor* actor, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor, Color_RGBA8* envColor);
void EffectSsDFire_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep, s16 alpha, s16 fadeDelay, s32 life);
void EffectSsDFire_SpawnFixedScale(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 alpha, s16 fadeDelay);
void EffectSsBubble_Spawn(PlayState* play, Vec3f* pos, f32 yPosOffset, f32 yPosRandScale, f32 xzPosRandScale, f32 scale);
void EffectSsGRipple_Spawn(PlayState* play, Vec3f* pos, s16 radius, s16 radiusMax, s16 life);
void EffectSsGSplash_Spawn(PlayState* play, Vec3f* pos, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 type, s16 scale);
void EffectSsGMagma_Spawn(PlayState* play, Vec3f* pos);
void EffectSsGFire_Spawn(PlayState* play, Vec3f* pos);
void EffectSsLightning_Spawn(PlayState* play, Vec3f* pos, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 scale, s16 yaw, s16 life, s16 numBolts);
void EffectSsDtBubble_SpawnColorProfile(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 life, s16 colorProfile, s16 randXZ);
void EffectSsDtBubble_SpawnCustomColor(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 scale, s16 life, s16 randXZ);
void EffectSsHahen_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 unused, s16 scale, s16 objId, s16 life, Gfx* dList);
void EffectSsHahen_SpawnBurst(PlayState* play, Vec3f* pos, f32 burstScale, s16 unused, s16 scale, s16 randScaleRange, s16 count, s16 objId, s16 life, Gfx* dList);
void EffectSsStick_Spawn(PlayState* play, Vec3f* pos, s16 yaw);
void EffectSsSibuki_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 moveDelay, s16 direction, s16 scale);
void EffectSsSibuki_SpawnBurst(PlayState* play, Vec3f* pos);
void EffectSsSibuki2_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale);
void EffectSsGMagma2_Spawn(PlayState* play, Vec3f* pos, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 updateRate, s16 drawMode, s16 scale);
void EffectSsStone1_Spawn(PlayState* play, Vec3f* pos, s32 arg2);
void EffectSsHitMark_Spawn(PlayState* play, s32 type, s16 scale, Vec3f* pos);
void EffectSsHitMark_SpawnFixedScale(PlayState* play, s32 type, Vec3f* pos);
void EffectSsHitMark_SpawnCustomScale(PlayState* play, s32 type, s16 scale, Vec3f* pos);
void EffectSsFhgFlash_SpawnLightBall(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, u8 param);
void EffectSsFhgFlash_SpawnShock(PlayState* play, Actor* actor, Vec3f* pos, s16 scale, u8 param);
void EffectSsKFire_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scaleMax, u8 type);
void EffectSsSolderSrchBall_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 unused, s16* linkDetected);
void EffectSsKakera_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* arg3, s16 gravity, s16 arg5, s16 arg6, s16 arg7, s16 arg8, s16 scale, s16 arg10, s16 arg11, s32 life, s16 colorIdx, s16 objId, Gfx* dList);
void EffectSsIcePiece_Spawn(PlayState* play, Vec3f* pos, f32 scale, Vec3f* velocity, Vec3f* accel, s32 life);
void EffectSsIcePiece_SpawnBurst(PlayState* play, Vec3f* refPos, f32 scale);
void EffectSsEnIce_SpawnFlyingVec3f(PlayState* play, Actor* actor, Vec3f* pos, s16 primR, s16 primG, s16 primB, s16 primA, s16 envR, s16 envG, s16 envB, f32 scale);
void EffectSsEnIce_SpawnFlyingVec3s(PlayState* play, Actor* actor, Vec3s* pos, s16 primR, s16 primG, s16 primB, s16 primA, s16 envR, s16 envG, s16 envB, f32 scale);
void EffectSsEnIce_Spawn(PlayState* play, Vec3f* pos, f32 scale, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor, Color_RGBA8* envColor, s32 life);
void EffectSsFireTail_Spawn(PlayState* play, Actor* actor, Vec3f* pos, f32 scale, Vec3f* arg4, s16 arg5, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 type, s16 bodyPart, s32 life);
void EffectSsFireTail_SpawnFlame(PlayState* play, Actor* actor, Vec3f* pos, f32 arg3, s16 bodyPart, f32 colorIntensity);
void EffectSsFireTail_SpawnFlameOnPlayer(PlayState* play, f32 scale, s16 bodyPart, f32 colorIntensity);
void EffectSsEnFire_SpawnVec3f(PlayState* play, Actor* actor, Vec3f* pos, s16 scale, s16 arg4, s16 flags, s16 bodyPart);
void EffectSsEnFire_SpawnVec3s(PlayState* play, Actor* actor, Vec3s* pos, s16 scale, s16 arg4, s16 flags, s16 bodyPart);
void EffectSsExtra_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scoreIdx);
void EffectSsFCircle_Spawn(PlayState* play, Actor* actor, Vec3f* pos, s16 radius, s16 height);
void EffectSsDeadDb_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep, s16 primR, s16 primG, s16 primB, s16 primA, s16 envR, s16 envG, s16 envB, s16 unused, s32 arg14, s16 playSfx);
void EffectSsDeadDd_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep, s16 primR, s16 primG, s16 primB, s16 alpha, s16 envR, s16 envG, s16 envB, s16 alphaStep, s32 life);
void EffectSsDeadDd_SpawnRandYellow(PlayState* play, Vec3f* pos, s16 scale, s16 scaleStep, f32 randPosScale, s32 randIter, s32 life);
void EffectSsDeadDs_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep, s16 alpha, s32 life);
void EffectSsDeadDs_SpawnStationary(PlayState* play, Vec3f* pos, s16 scale, s16 scaleStep, s16 alpha, s32 life);
void EffectSsDeadSound_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, u16 sfxId, s16 lowerPriority, s16 repeatMode, s32 life);
void EffectSsDeadSound_SpawnStationary(PlayState* play, Vec3f* pos, u16 sfxId, s16 lowerPriority, s16 repeatMode, s32 life);
void EffectSsIceSmoke_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale);

#endif

#endif
