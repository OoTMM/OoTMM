#ifndef COMBO_COMBO_H
#define COMBO_COMBO_H

#if defined(DEBUG)
# include <combo/debug.h>
#endif

#if !defined(__ASSEMBLER__)
# include <stddef.h>
# include <string.h>
# include <stdlib.h>

# define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

# include <ultra64.h>
# include <combo/actor_ovl.h>
# include <combo/defs.h>
# include <combo/equipment.h>
# include <combo/object.h>
# include <combo/patch.h>
# include <combo/npc.h>
# include <combo/text.h>

# if defined(GAME_OOT)
#  include <combo/oot/play.h>
#  include <combo/oot/player.h>
#  include <combo/oot/actors/Item_Etcetera.h>
#  include <combo/oot/actor_engirla.h>
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
#include <PR/gbi.h>

#include <combo/save.h>
#include <combo/gi.h>
#include <combo/items.h>
#include <combo/common/events.h>
#include <combo/scenes.h>
#include <combo/shader.h>
#include <combo/config.h>

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

typedef struct PACKED ALIGNED(4)
{
    u8 dungeonRewards[13];
    u8 lightArrows;
    u8 oathToOrder;
}
ComboDataHints;

typedef struct PACKED ALIGNED(4)
{
    u8             config[0x40];
    ComboDataHints hints;
}
ComboData;

extern ComboData gComboData;

void comboLoadContext(void);
void comboExportContext(void);

/* Init */
void comboInit(void);
void comboInitDma(void);
void comboInitOverride(void);
void comboInitData(void);

/* Flash */
void comboReadWriteFlash(u32 devAddr, void* dramAddr, u32 size, s32 direction);

/* Save */
void comboReadForeignSave(void);
void comboWriteForeignSave(void);
void comboWriteSave(void);
void comboCopyMmSave(int dst, int src);

/* Save util */
void comboCreateSaveMM(void);

/* Switch */
void comboGameSwitch(void);

/* Override */
#define OV_CHEST        0
#define OV_COLLECTIBLE  1
#define OV_NPC          2
#define OV_GS           3
#define OV_SF           4

#define OVF_PROGRESSIVE       (1 << 0)
#define OVF_DOWNGRADE         (1 << 1)

s16 comboOverride(int type, u16 sceneId, u16 id, s16 gi);
s16 comboOverrideEx(int type, u16 sceneId, u16 id, s16 gi, int flags);

/* Text */
int  comboMultibyteCharSize(u8 c);
void comboTextHijackItem(GameState_Play* play, u16 itemId);
void comboTextHijackItemShop(GameState_Play* play, u16 itemId, s16 price, int confirm);

#if defined(GAME_OOT)
void comboTextHijackDungeonRewardHints(GameState_Play* play, int base, int count);
void comboTextHijackSkullReward(GameState_Play* play, s16 itemId, int count);
void comboTextHijackLightArrows(GameState_Play* play);
#else
void comboTextHijackDungeonRewardHints(GameState_Play* play, int hint);
void comboTextHijackOathToOrder(GameState_Play* play);
#endif

/* Progressive */
s32 comboProgressive(s32 gi);
s32 comboProgressiveOot(s32 gi);
s32 comboProgressiveMm(s32 gi);
s16 comboDowngrade(s16 gi);

/* Objects */
void    comboObjectsReset(void);
void    comboObjectsGC(void);
void*   comboGetObject(u16 objectId);
u32     comboLoadObject(void* buffer, u16 objectId);
void    comboLoadCustomKeep(void);

/* Draw */
#define DRAW_NO_PRE1    0x01
#define DRAW_NO_PRE2    0x02
#define DRAW_RAW        (DRAW_NO_PRE1 | DRAW_NO_PRE2)

void comboSetObjectSegment(GfxContext* gfx, void* buffer);
void comboDrawObject(GameState_Play* play, Actor* actor, u16 objectId, u16 shaderId, int flags);
void comboDrawGI(GameState_Play* play, Actor* actor, int gi, int flags);
void comboDrawInit2D(GameState_Play* play);
void comboDrawBlit2D(GameState_Play* play, u32 segAddr, int w, int h, float x, float y, float scale);

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

void comboAddItemMm(u16 itemId, int noEffect);
void comboAddItemOot(u16 itemId, int noEffect);
void comboAddItemEffect(GameState_Play* play, u16 itemId);
void comboAddSmallKeyOot(u16 dungeonId);
void comboAddBossKeyOot(u16 dungeonId);
void comboAddCompassOot(u16 dungeonId);
void comboAddMapOot(u16 dungeonId);
void comboAddSmallKeyMm(u16 dungeonId);
void comboAddBossKeyMm(u16 dungeonId);
void comboAddStrayFairyMm(u16 dungeonId);

int  comboAddItem(GameState_Play* play, s16 gi);
int  comboAddItemNoEffect(s16 gi);

int comboIsItemUnavailable(s16 gi);
int comboIsItemMinor(s16 gi);

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
void comboRemoveTradeItem1(u16 xitemId);
void comboRemoveTradeItem2(u16 xitemId);
void comboRemoveTradeItem3(u16 xitemId);
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

/* System */
void comboInvalICache(void* addr, u32 size);
void comboInvalDCache(void* addr, u32 size);

/* Custom keep files */
extern void* gCustomKeep;

void comboDpadDraw(GameState_Play* play);
void comboDpadUpdate(GameState_Play* play);
int  comboDpadUse(GameState_Play* play);

int comboConfig(int flag);
int comboDoorIsUnlocked(GameState_Play* play, int flag);

/* DMA */
typedef struct
{
    u32 vstart;
    u32 vend;
    u32 pstart;
    u32 pend;
}
DmaEntry;

extern DmaEntry kComboDmaData[];

/* Misc */
int comboCanAccessMoon(void);

/* Hints */
#define HINT_GOSSIP         0x00
#define HINT_GOSSIP_GROTTO  0x01

#define HINT_TYPE_HERO          0x00
#define HINT_TYPE_FOOLISH       0x01
#define HINT_TYPE_ITEM_EXACT    0x02
#define HINT_TYPE_ITEM_REGION   0x03

void comboInitHints(void);
void comboHintGossip(u8 key, GameState_Play* play);

/* CSMC */
void comboCsmcInit(Actor* this, GameState_Play* play, s16 gi);
void comboCsmcPreDraw(Actor* this, GameState_Play* play, s16 gi);
int  comboCsmcChestSize(s16 gi);

/* Shop */
#define SC_OK               0x00
#define SC_OK_NOCUTSCENE    0x01
#define SC_ERR_CANNOTBUY    0x02
#define SC_ERR_NORUPEES     0x04

int comboShopPrecond(GameState_Play* play, Actor_EnGirlA* girlA);

#else
# include <combo/asm.h>
#endif

#endif /* COMBO_COMBO_H */
