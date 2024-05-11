#ifndef COMBO_AUDIO_H
#define COMBO_AUDIO_H

#include <combo/types.h>

typedef struct GameState_Play GameState_Play;

u16     Audio_GetActiveSeqId(int channel);
void    Audio_DisplayMusicName(GameState_Play* play);

#endif
