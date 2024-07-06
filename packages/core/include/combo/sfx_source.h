#ifndef COMBO_SFX_SOURCE_H
#define COMBO_SFX_SOURCE_H

#include <combo/util.h>
#include <combo/math/vec.h>

typedef struct GameState_Play GameState_Play;

typedef struct SfxSource
{
    u16     countdown;
    Vec3f   worldPos;
    Vec3f   projectedPos;
}
SfxSource;

ASSERT_SIZE(SfxSource, 0x1c);
ASSERT_OFFSET(SfxSource, countdown, 0x0);
ASSERT_OFFSET(SfxSource, worldPos, 0x4);
ASSERT_OFFSET(SfxSource, projectedPos, 0x10);

#define SFX_SOURCE_COUNT 16

void SfxSource_InitAll(GameState_Play* play);
void SfxSource_UpdateAll(GameState_Play* play);
void SfxSource_PlaySfxAtFixedWorldPos(GameState_Play* play, Vec3f* worldPos, s32 duration, u16 sfxId);

#endif
