#ifndef OOT_PLAY_H
#define OOT_PLAY_H

#include <combo/game_state.h>

typedef struct PACKED ALIGNED(4)
{
    GameState    gs;
    u16          sceneId;
    char         unk_000a6[0xfde2];
    char         textBuffer[4]; /* Real size unknown */
    char         unk_0fe8c[0x1918];
    char         objTable[4]; /* Real size unknown */
    char         unk_117a8[0xd70];
}
GameState_Play;

_Static_assert(sizeof(GameState_Play) == 0x12518, "OoT GameState_Play size is wrong");

#endif
