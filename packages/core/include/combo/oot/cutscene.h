#ifndef INCLUDED_COMBO_OOT_PLAY_CUTSCENE_H
#define INCLUDED_COMBO_OOT_PLAY_CUTSCENE_H

#include <combo/util.h>

typedef struct
{
    char unk_00[4];
    u32  next;
    u8   active;
    char unk_09[0x1f];
    u16* unk_28;
    char unk_2c[0x4];
}
CutsceneContext;

ASSERT_SIZE(CutsceneContext, 0x30);
ASSERT_OFFSET(CutsceneContext, unk_00,  0x00);
ASSERT_OFFSET(CutsceneContext, next,    0x04);
ASSERT_OFFSET(CutsceneContext, active,  0x08);
ASSERT_OFFSET(CutsceneContext, unk_09,  0x09);
ASSERT_OFFSET(CutsceneContext, unk_28,  0x28);
ASSERT_OFFSET(CutsceneContext, unk_2c,  0x2c);

#endif
