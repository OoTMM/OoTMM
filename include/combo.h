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
# include <combo/shader.h>
# include <combo/patch.h>
# include <combo/npc.h>

# if defined(GAME_OOT)
#  include <combo/oot/play.h>
#  include <combo/oot/player.h>
#  include <combo/oot/actors/Item_Etcetera.h>
# endif

# if defined(GAME_MM)
#  include <combo/mm/play.h>
#  include <combo/mm/player.h>
#  include <combo/mm/actor_engirla.h>
#  include <combo/mm/actor_ensob1.h>
#  include <combo/mm/actor_enfsn.h>
# endif

# include <combo/common/actor.h>
# include <combo/common/actors/En_Item00.h>
# include <combo/common/actor_init.h>
# include <combo/common/actor_item_custom.h>
# include <combo/common/api.h>
#endif

/* Shared with assembler */
#include <combo/save.h>
#include <combo/gi.h>
#include <combo/items.h>
#include <combo/common/events.h>
#include <combo/scenes.h>

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
#define OV_CHEST        0
#define OV_COLLECTIBLE  1
#define OV_NPC          2

s16 comboOverride(int type, u16 sceneId, u16 id, s16 gi);

/* Text */
void comboTextHijackItem(GameState_Play* play, u16 itemId);
void comboTextHijackItemShop(GameState_Play* play, u16 itemId, s16 price, int confirm);

/* Progressive */
s32 comboProgressive(s32 gi);
s32 comboProgressiveOot(s32 gi);
s32 comboProgressiveMm(s32 gi);

/* Objects */
void    comboObjectsReset(void);
void*   comboGetObject(u16 objectId);
u32     comboLoadObject(void* buffer, u16 objectId);

/* Draw */
#define DRAW_NO_PRE1    0x01
#define DRAW_NO_PRE2    0x02
#define DRAW_RAW        (DRAW_NO_PRE1 | DRAW_NO_PRE2)

void comboDrawObject(GameState_Play* play, Actor* actor, u16 objectId, u16 shaderId, int flags);
void comboDrawGI(GameState_Play* play, Actor* actor, int gi, int flags);

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
extern const u8 kMmTrade1[];
extern const u8 kMmTrade2[];
extern const u8 kMmTrade3[];

int  comboAddItemGI(GameState_Play* play, s16 gi);
void comboAddItemMm(u16 itemId);
void comboAddItemOot(u16 itemId);

void comboToggleTrade(u8* slot, u32 flags, const u8* table, u32 tableSize);

#if defined(GAME_OOT)
void comboToggleTradeAdult(void);
void comboToggleTradeChild(void);
void comboToggleOcarina(void);
void comboToggleHookshot(void);
void comboRemoveTradeItemAdult(u16 xitemId);
void comboRemoveTradeItemChild(u16 xitemId);
#endif

#if defined(GAME_MM)
void comboToggleTrade1(void);
void comboToggleTrade2(void);
void comboToggleTrade3(void);
#endif

# if defined(GAME_MM)
void comboAfterBuy(Actor_EnGirlA* girlA, GameState_Play* play);
void comboShopDisplayTextBox(GameState_Play* play, Actor_EnGirlA* girlA, int price);
void comboShopDisplayTextBoxConfirm(GameState_Play* play, Actor_EnGirlA* girlA, int price);
# endif

void comboSpawnItemGiver(GameState_Play* play, u16 npcId);
void comboSpawnItemGivers(GameState_Play* play);

/* libc */
int toupper(int c);

/* Util */
u32     popcount(u32 x);
void*   actorAddr(u16 actorId, u32 addr);

#else
# include <combo/asm.h>
#endif

#endif /* COMBO_COMBO_H */
