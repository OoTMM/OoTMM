#ifndef INCLUDED_COMBO_OOT_PLAY_CUTSCENE_H
#define INCLUDED_COMBO_OOT_PLAY_CUTSCENE_H

#include <combo/util.h>

typedef struct
{
    char    unk_00[0x4];
    void*   script;
    u8      state;
    float   timer;
    u16     curFrame;
    u16     unk_12;
    s32     subCamId;
    u16     camEyeSplinePointsAppliedFrame;
    u8      camAtReady;
    u8      camEyeReady;
    void*   camAtPoints;
    void*   camEyePoints;
    void*   playerCue;
    void*   actorCues[10];
}
CutsceneContext;

ASSERT_SIZE(CutsceneContext, 0x50);
ASSERT_OFFSET(CutsceneContext, unk_00,  0x00);
ASSERT_OFFSET(CutsceneContext, script,  0x04);
ASSERT_OFFSET(CutsceneContext, state,   0x08);
ASSERT_OFFSET(CutsceneContext, timer,   0x0c);

#endif
