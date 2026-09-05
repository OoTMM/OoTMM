#ifndef COMBO_CONTEXT_H
#define COMBO_CONTEXT_H

#include <combo/util.h>

typedef struct PACKED ALIGNED(4)
{
    char    magic[8];
    u32     valid;
    u32     saveIndex;
    u32     entrance;
    u32     multiSeqGame;
    u32     multiSeqNet;
    u32     isDungeonEntranceSpawn:1;
    u32     isFwSpawn:1;
    u32     fwSpawnAge:1;
    u32     isMultiConnected:1;
}
ComboContext;

extern ComboContext gComboCtx;

void Context_Init(void);
void Context_Export(void);

#endif
