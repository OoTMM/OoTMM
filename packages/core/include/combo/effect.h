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
void EffectSsBubble_Spawn(GameState_Play* play, Vec3f* pos, f32 yPosOffset, f32 yPosRandScale, f32 xzPosRandScale, f32 scale);
void EffectSsGRipple_Spawn(GameState_Play* play, Vec3f* pos, s16 radius, s16 radiusMax, s16 life);
void EffectSsGSplash_Spawn(GameState_Play* play, Vec3f* pos, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 type, s16 scale);
void EffectSsKakera_Spawn(GameState_Play* play, Vec3f* pos, Vec3f* velocity, Vec3f* arg3, s16 gravity, s16 arg5, s16 arg6, s16 arg7, s16 arg8, s16 scale, s16 arg10, s16 arg11, s32 life, s16 colorIdx, s16 objId, Gfx* dList);

#endif
