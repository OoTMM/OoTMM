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

#if defined(GAME_MM) // TODO: Move elsewhere

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

#endif

#endif
