#ifndef COMBO_AUDIO_H
#define COMBO_AUDIO_H

#include <combo/types.h>

typedef struct
{
    u32 romAddr;
    u32 size;
    s8  medium;
    s8  cachePolicy;
    u8  data[6];
}
AudioTableEntry;

typedef struct
{
    u16             count;
    s16             unk_02;
    u32             romAddr;
    char            pad[0x8];
}
AudioTableHeader;

typedef struct
{
    AudioTableHeader header;
    AudioTableEntry  entries[];
}
AudioTable;

typedef struct GameState_Play GameState_Play;

void    Audio_SetCutsceneFlag(int flag);
u16     Audio_GetActiveSeqId(int channel);
void    Audio_DisplayMusicName(GameState_Play* play);

#endif
