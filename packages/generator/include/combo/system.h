#ifndef COMBO_SYSTEM_H
#define COMBO_SYSTEM_H

#include <combo/types.h>

void System_DisableInterrupts(void);
void System_InvalICache(void* addr, u32 size);
void System_InvalDCache(void* addr, u32 size);

#endif
