#ifndef COMBO_COMBO_H
#define COMBO_COMBO_H

#include <types.h>
#include <string.h>
#include <strings.h>
#include <libultra.h>
#include <oot/save.h>
#include <mm/save.h>

/* System */
typedef void (*EntryPoint)(void)  __attribute__ ((noreturn));

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

/* Saves */
extern OotSaveContext gSaveContextOoT;
extern MmSaveContext gSaveContextMM;

#if GAME_OOT
# define gSaveContext gSaveContextOoT
#endif

#if GAME_MM
# define gSaveContext gSaveContextMM
#endif

#endif /* COMBO_COMBO_H */
