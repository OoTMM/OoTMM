#ifndef COMBO_CONTEXT_H
#define COMBO_CONTEXT_H

#include <combo/util.h>

typedef struct PACKED ALIGNED(4)
{
    char magic[8];
    u32  valid;
    u32  saveIndex;
    u32  entrance;
    s32  isFwSpawn;
    s32  isDungeonEntranceSpawn;
}
ComboContext;

extern ComboContext gComboCtx;

void Context_Init(void);
void Context_Export(void);

#endif
