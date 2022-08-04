#ifndef COMBO_COMBO_H
#define COMBO_COMBO_H

#include <types.h>
#include <string.h>
#include <strings.h>
#include <libultra.h>

/* System */
typedef void (*EntryPoint)(void)  __attribute__ ((noreturn));

void comboDisableInterrupts(void);
void comboDma(void* addr, uint32_t cartAddr, uint32_t size);

typedef struct PACKED
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

#endif /* COMBO_COMBO_H */
