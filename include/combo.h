#ifndef COMBO_COMBO_H
#define COMBO_COMBO_H

#include <types.h>
#include <string.h>
#include <strings.h>
#include <libultra.h>
#include <oot/save.h>
#include <mm/save.h>
#include <combo/defs.h>
#include <combo/common/audio.h>

void comboDisableInterrupts(void);
void comboDma(void* addr, uint32_t cartAddr, uint32_t size);

typedef struct PACKED ALIGNED(4)
{
    char magic[8];
    u32  valid;
    u32  saveIndex;
}
ComboContext;

extern ComboContext gComboCtx;

void comboLoadContext(void);
void comboExportContext(void);

/* Init */
void comboInitDma(void);

/* Flash */
void comboReadWriteFlash(u32 devAddr, void* dramAddr, u32 size, s32 direction);

/* Save */
void comboSaveRead(void);
void comboSaveWrite(void);

/* Switch */
void comboGameSwitch(void);

#endif /* COMBO_COMBO_H */
