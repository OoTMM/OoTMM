#ifndef COMBO_ENTRANCE_H
#define COMBO_ENTRANCE_H

#include <combo/types.h>

typedef struct GameState_Play GameState_Play;

typedef struct
{
    u16 id;
    u8  isMM;
}
EntranceDescr;

void comboGetDungeonExit(EntranceDescr* dst, int dungeonId);
void comboTransition(GameState_Play* play, u32 entranceId);
void comboTransitionDescr(GameState_Play* play, const EntranceDescr* descr);

#endif
