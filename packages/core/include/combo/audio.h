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
    char            pad[0x0e];
}
AudioTableHeader;

typedef struct
{
    AudioTableHeader header;
    AudioTableEntry  entries[];
}
AudioTable;

typedef struct GameState_Play GameState_Play;

u16     Audio_GetActiveSeqId(int channel);
void    Audio_DisplayMusicName(GameState_Play* play);

#endif
