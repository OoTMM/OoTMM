#ifndef COMBO_ENTRANCE_H
#define COMBO_ENTRANCE_H

#include <combo/types.h>

#define ENTR_EXTENDED 0xfffe
#define ENTR_FW_CROSS 0xfffd

typedef struct GameState_Play GameState_Play;

int  comboIsEntranceValidEpona(u32 entranceId);
void comboTransition(GameState_Play* play, u32 entranceId);

#endif
