#ifndef COMBO_COMBO_H
#define COMBO_COMBO_H

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#include <string.h>
#include <stdlib.h>
#include <ultra64.h>
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
# include <combo/oot/scene.h>
# include <combo/oot/actor.h>
# include <combo/oot/play.h>
# include <combo/oot/item_etc.h>
# include <combo/oot/events.h>
# include <combo/oot/player.h>
# include <combo/oot/actor_item_custom.h>
#endif

#if defined(GAME_MM)
# include <combo/mm/actor.h>
# include <combo/mm/play.h>
#endif

void comboDisableInterrupts(void);
void comboDma(void* addr, u32 cartAddr, u32 size);
void comboDma_NoCacheInval(void* addr, u32 cartAddr, u32 size);

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
s32 comboGetCollectibleOverride(u16 scene, u8 id);
s32 comboGetSpecialOverride(u8 id);
s32 comboGetNpcOverride(u8 id);

/* Text */
void comboTextHijackItem(GameState_Play* play, u16 itemId);

/* Progressive */
s32 comboProgressiveChestItem(s32 gi);

/* Actor */
void comboSpawnSpecial(GameState_Play* play, float x, float y, float z, u8 specialId, u16 gi);

/* Objects */
void    comboObjectsReset(void);
void*   comboGetObject(u16 objectId);
u32     comboLoadObject(void* buffer, u16 objectId);

/* Draw */
void comboDrawObject(GameState_Play* play, Actor* actor, u16 objectId, u16 shaderId);

#endif /* COMBO_COMBO_H */
