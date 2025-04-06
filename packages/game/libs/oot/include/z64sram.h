#ifndef Z64SRAM_H
#define Z64SRAM_H

#include "PR/ultratypes.h"

struct FileSelectState;
struct GameState;

void Sram_InitDebugSave(void);
void Sram_OnLoad(void);
void Sram_InitSram(void);

#endif
