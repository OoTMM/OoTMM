#ifndef COMBO_COMBO_H
#define COMBO_COMBO_H

#include <types.h>
#include <string.h>
#include <strings.h>

#include <libultra.h>

#include <combo/actor_ovl.h>
#include <combo/audio.h>
#include <combo/defs.h>
#include <combo/equipment.h>
#include <combo/gi.h>
#include <combo/items.h>
#include <combo/object.h>
#include <combo/save.h>
#include <combo/shader.h>

#if defined(GAME_OOT)
# include <combo/oot/actor.h>
# include <combo/oot/play.h>
# include <combo/oot/item_etc.h>
#endif

#if defined(GAME_MM)
# include <combo/mm/actor.h>
# include <combo/mm/play.h>
#endif

void comboDisableInterrupts(void);
void comboDma(void* addr, uint32_t cartAddr, uint32_t size);
void comboDma_NoCacheInval(void* addr, uint32_t cartAddr, uint32_t size);

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

/* Text */
void comboTextHijackItem(GameState_Play* play, u16 itemId);

/* Progressive */
s32 comboProgressiveChestItem(s32 gi);

#endif /* COMBO_COMBO_H */
