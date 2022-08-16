#ifndef OOT_PLAY_H
#define OOT_PLAY_H

#include <oot/game_state.h>

typedef struct PACKED ALIGNED(4)
{
    GameState    gs;
    u16          sceneId;
    char         unk[0x12472];
}
GameState_Play;

_Static_assert(sizeof(GameState_Play) == 0x12518, "OoT GameState_Play size is wrong");

#endif
