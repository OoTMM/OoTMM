#ifndef COMBO_ENTRANCE_H
#define COMBO_ENTRANCE_H

#include <combo/types.h>

typedef struct
{
    u16 id;
    u8  isMM;
}
EntranceDescr;

void comboGetDungeonExit(EntranceDescr* dst, int dungeonId);

#endif
