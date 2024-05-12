#ifndef COMBO_AUDIO_H
#define COMBO_AUDIO_H

#include <combo/types.h>

typedef struct
{
    u32 romAddr;
    u32 size;
    s8  medium;
    s8  cachePolicy;
    s16 data[3];
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

typedef struct
{
    AudioTableHeader header;
    AudioTableEntry  entries[256];
}
CustomAudioTable;

typedef struct GameState_Play GameState_Play;

u16     Audio_GetActiveSeqId(int channel);
void    Audio_DisplayMusicName(GameState_Play* play);

#endif
