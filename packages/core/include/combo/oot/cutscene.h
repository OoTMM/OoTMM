#ifndef INCLUDED_COMBO_OOT_PLAY_CUTSCENE_H
#define INCLUDED_COMBO_OOT_PLAY_CUTSCENE_H

#include <combo/util.h>

typedef struct PACKED
{
    char unk_00[4];
    u32  next;
    u8   active;
    char unk_09[0x1f];
    u16* unk_28;
    char unk_2c[0x4];
}
CutsceneContext;

#endif
