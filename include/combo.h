#ifndef COMBO_COMBO_H
#define COMBO_COMBO_H

#if !defined(__ASSEMBLER__)
# include <stddef.h>
# include <string.h>
# include <stdlib.h>

# define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

# include <ultra64.h>
# include <combo/actor_ovl.h>
# include <combo/audio.h>
# include <combo/defs.h>
# include <combo/equipment.h>
# include <combo/object.h>
# include <combo/save.h>
# include <combo/shader.h>
# include <combo/scenes.h>
# include <combo/patch.h>

# if defined(GAME_OOT)
#  include <combo/oot/play.h>
#  include <combo/oot/item_etc.h>
#  include <combo/oot/player.h>
# endif

# if defined(GAME_MM)
#  include <combo/mm/play.h>
#  include <combo/mm/player.h>
#  include <combo/mm/actor_engirla.h>
#  include <combo/mm/actor_ensob1.h>
# endif

# include <combo/common/actor.h>
# include <combo/common/events.h>
# include <combo/common/actor_init.h>
# include <combo/common/actor_item_custom.h>
# include <combo/common/api.h>
#endif

/* Shared with assembler */
#include <combo/gi.h>
#include <combo/items.h>

#if !defined(__ASSEMBLER__)
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
void comboTextHijackItemShop(GameState_Play* play, u16 itemId, s16 price, int confirm);

/* Progressive */
s32 comboProgressive(s32 gi);
s32 comboProgressiveOot(s32 gi);
s32 comboProgressiveMm(s32 gi);

/* Actor */
void comboSpawnSpecial(GameState_Play* play, float x, float y, float z, u8 specialId, u16 gi);
void comboSpawnCollectible(GameState_Play* play, float x, float y, float z, u8 flag, u16 gi);

/* Objects */
void    comboObjectsReset(void);
void*   comboGetObject(u16 objectId);
u32     comboLoadObject(void* buffer, u16 objectId);

/* Draw */
void comboDrawObject(GameState_Play* play, Actor* actor, u16 objectId, u16 shaderId);
void comboDrawGI(GameState_Play* play, Actor* actor, int gi);

/* Event */
void comboOotSetEventChk(u16 flag);
void comboMmSetEventWeek(u16 flag);

/* GI */
u16 comboItemFromGI(s32 gi);

/* Item */
extern const u8 kMaxSticks[];
extern const u8 kMaxNuts[];
extern const u8 kMaxBombs[];
extern const u8 kMaxArrows[];
extern const u8 kMaxSeeds[];
extern const u16 kMaxRupees[];
extern const u8 kOotTradeChild[];
extern const u8 kOotTradeAdult[];

void comboAddItem(u16 itemId);
void comboAddItemMm(u16 itemId);
void comboAddItemOot(u16 itemId);

int comboGiveItem(Actor* actor, GameState_Play* play, s16 itemId, float a, float b);

# if defined(GAME_MM)
void comboAfterBuy(Actor_EnGirlA* girlA, GameState_Play* play);
void comboShopDisplayTextBox(GameState_Play* play, Actor_EnGirlA* girlA, int price);
void comboShopDisplayTextBoxConfirm(GameState_Play* play, Actor_EnGirlA* girlA, int price);
# endif

/* libc */
int toupper(int c);

#else
# include <combo/asm.h>
#endif

#endif /* COMBO_COMBO_H */
