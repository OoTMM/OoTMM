#ifndef COMBO_ENTRANCE_H
#define COMBO_ENTRANCE_H

#include <combo/types.h>

#define ENTR_EXTENDED      0xfffe
#define ENTR_FW_CROSS      0xfffd
#define ENTR_CROSS_RESPAWN 0xfffc

typedef struct PlayState PlayState;

int  comboIsEntranceValidEpona(u32 entranceId);
void comboTransition(PlayState* play, u32 entranceId);

#endif
