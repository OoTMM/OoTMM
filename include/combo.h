#ifndef COMBO_COMBO_H
#define COMBO_COMBO_H

#include <types.h>
#include <string.h>
#include <strings.h>
#include <libultra.h>
#include <combo/defs.h>
#include <combo/items.h>
#include <combo/common/audio.h>
#include <combo/common/object.h>
#include <combo/common/gi.h>
#include <combo/common/shader.h>

/* Both games need the save defs */
#include <oot/save.h>
#include <mm/save.h>

#if defined(GAME_OOT)
# include <oot/actor.h>
# include <oot/play.h>
#endif

#if defined(GAME_MM)
# include <mm/actor.h>
# include <mm/play.h>
#endif

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
void comboInit(void);
void comboInitDma(void);
void comboInitOverride(void);

/* Flash */
void comboReadWriteFlash(u32 devAddr, void* dramAddr, u32 size, s32 direction);

/* Save */
void comboReadForeignSave(void);
void comboWriteForeignSave(void);
void comboWriteSave(void);

/* Switch */
void comboGameSwitch(void);

/* Override */
s32 comboGetChestOverride(u16 scene, u8 id);

#endif /* COMBO_COMBO_H */
