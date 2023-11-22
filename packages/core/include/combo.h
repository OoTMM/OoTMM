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
# include <combo/common/ocarina.h>
# include <combo/common/actors/Obj_Mure2.h>

# if defined(GAME_OOT)
#  include <combo/oot/play.h>
#  include <combo/oot/player.h>
#  include <combo/oot/pause_state.h>
#  include <combo/oot/actors/Item_Etcetera.h>
#  include <combo/oot/actors/Obj_Tsubo.h>
#  include <combo/oot/actors/Obj_Timeblock_Warp2Block.h>
#  include <combo/oot/actors/En_Kusa.h>
#  include <combo/oot/actors/En_Tubo_Trap.h>
#  include <combo/oot/actors/En_Ossan.h>
# endif

# if defined(GAME_MM)
#  include <combo/mm/play.h>
#  include <combo/mm/player.h>
#  include <combo/mm/actor_ensob1.h>
#  include <combo/mm/actor_enfsn.h>
#  include <combo/mm/actor_entrt.h>
#  include <combo/mm/actor_enossan.h>
#  include <combo/mm/actor_ensuttari.h>
#  include <combo/mm/actor_arms_hook.h>
#  include <combo/mm/actor_objboat.h>
#  include <combo/mm/actors/Obj_Tsubo.h>
#  include <combo/mm/actors/Obj_Grass.h>
#  include <combo/mm/actors/Obj_Grass_Carry.h>
#  include <combo/mm/actors/En_Tubo_Trap.h>
#  include <combo/mm/actors/En_Kusa.h>
# endif

# include <combo/common/actors/En_Item00.h>
# include <combo/common/actors/En_GirlA.h>
# include <combo/common/actor_init.h>
# include <combo/common/actor_item_custom.h>
# include <combo/common/actors/Custom_Item.h>
# include <combo/common/actors/Custom_Triggers.h>
# include <combo/common/api.h>
#endif

/* Shared with assembler */
#include <PR/gbi.h>
#include <combo/common/actor.h>
#include <combo/save.h>
#include <combo/prices.h>
#include <combo/gi.h>
#include <combo/data/items.h>
#include <combo/entrances.h>
#include <combo/common/events.h>
#include <combo/scenes.h>
#include <combo/shader.h>
#include <combo/config.h>
#if defined (GAME_OOT)
# include <combo/oot/pause_state_defs.h>
#endif

#if !defined(__ASSEMBLER__)
void comboDisableInterrupts(void);
void comboDma(void* addr, u32 cartAddr, u32 size);
void comboDma_NoCacheInval(void* addr, u32 cartAddr, u32 size);
u32 comboLoadFile(void* dest, s32 fileIndex);
u32 comboLoadForeignFile(void* dest, s32 foreignFileIndex);

typedef struct PACKED ALIGNED(4)
{
    char magic[8];
    u32  valid;
    u32  saveIndex;
    s32  entrance;
    s32  shuffledEntrance;
}
ComboContext;

extern ComboContext gComboCtx;

typedef struct
{
    u8 region;
    u8 world;
}
ItemHint;

typedef struct
{
    ItemHint dungeonRewards[13];
    ItemHint lightArrows;
    ItemHint oathToOrder;
    ItemHint ganonBossKey;
}
ComboDataHints;

#define SPECIAL_BRIDGE      0
#define SPECIAL_MOON        1
#define SPECIAL_LACS        2
#define SPECIAL_GANON_BK    3
#define SPECIAL_MAJORA      4

#define PLAYER_SELF 0x00
#define PLAYER_ALL  0xff

typedef struct
{
    u32 flags;
    u16 count;
    u16 zero;
}
SpecialCond;

typedef struct
{
    u8              uuid[16];
    u8              playerId;
    u32             mq;
    u32             preCompleted;
    u32             entrancesSong[6];
    u32             entrancesOwl[10];
    u8              config[0x40];
    SpecialCond     special[5];
    u16             maxCoins[4];
    u16             prices[PRICES_MAX];
    u16             triforcePieces;
    u16             triforceGoal;
    ComboDataHints  hints;
    s8              staticHintsImportance[20];
    u16             giZoraSapphire;
    u8              boss[12];
    u8              dungeons[26];
}
ComboData;

extern ComboData gComboData;

void comboLoadContext(void);
void comboExportContext(void);

/* Init */
void comboInit(void);
void comboInitOverride(void);
void comboInitData(void);

/* Flash */
void comboReadWriteFlash(u32 devAddr, void* dramAddr, u32 size, s32 direction);

/* Save */
#define SF_OWL          0x01
#define SF_NOCOMMIT     0x02
#define SF_PASSIVE      0x04

void comboReadForeignSave(void);
void comboWriteSave(void);
void comboCopyMmSave(int dst, int src);
void comboOnSaveLoad(void);
void comboSave(GameState_Play* play, int saveFlags);

/* Save util */
void comboCreateSaveMM(void);

/* Switch */
NORETURN void comboGameSwitch(GameState_Play* play, s32 entrance);

#if defined(GAME_OOT)
void swapFarore(void);
#endif

/* Override */
#define OV_NONE         0x00
#define OV_CHEST        0x01
#define OV_COLLECTIBLE  0x02
#define OV_NPC          0x03
#define OV_GS           0x04
#define OV_SF           0x05
#define OV_COW          0x06
#define OV_SHOP         0x07
#define OV_SCRUB        0x08
#define OV_SR           0x09

#define OV_XFLAG0       0x10
#define OV_XFLAG1       0x11
#define OV_XFLAG2       0x12
#define OV_XFLAG3       0x13
#define OV_XFLAG4       0x14
#define OV_XFLAG5       0x15
#define OV_XFLAG6       0x16
#define OV_XFLAG7       0x17
#define OV_XFLAG8       0x18
#define OV_XFLAG9       0x19
#define OV_XFLAG10      0x1a
#define OV_XFLAG11      0x1b

#define OVF_RENEW             (1 << 2)
#define OVF_PRECOND           (1 << 3)

/* Text */
int  comboMultibyteCharSize(u8 c);
void comboTextHijackItem(GameState_Play* play, s16 gi, int count);
void comboTextHijackItemEx(GameState_Play* play, const ComboItemOverride* o, int count);
void comboTextHijackItemShop(GameState_Play* play, const ComboItemOverride* o, s16 price, int confirm);

#if defined(GAME_OOT)
void comboTextHijackDungeonRewardHints(GameState_Play* play, int base, int count);
void comboTextHijackLightArrows(GameState_Play* play);
#else
void comboTextHijackDungeonRewardHints(GameState_Play* play, int hint);
void comboTextHijackOathToOrder(GameState_Play* play);
#endif

/* Progressive */
s32 comboProgressive(s32 gi);
s32 comboProgressiveOot(s32 gi);
s32 comboProgressiveMm(s32 gi);

/* Cache */
void    comboCacheClear(void);
void    comboCacheGarbageCollect(void);
void*   comboCacheGetFile(u32 vrom);

/* Objects */
void    comboObjectsReset(void);
void    comboObjectsGC(void);
void*   comboGetObject(u16 objectId);
u32     comboLoadObject(void* buffer, u16 objectId);
void    comboLoadCustomKeep(void);
void    comboExObjectsReset(void);

/* Custom_Warp */
void comboSpawnCustomWarps(GameState_Play*);

/* Draw */
#define DRAW_NO_PRE1    0x01
#define DRAW_NO_PRE2    0x02
#define DRAW_RAW        (DRAW_NO_PRE1 | DRAW_NO_PRE2)

void comboSetObjectSegment(GfxContext* gfx, void* buffer);
void comboDrawObject(GameState_Play* play, Actor* actor, u16 objectId, u16 shaderId, int flags);
void comboDrawGI(GameState_Play* play, Actor* actor, int gi, int flags);
void comboDrawInit2D(Gfx** dl);
void comboDrawBlit2D(Gfx** dl, u32 segAddr, int w, int h, float x, float y, float scale);
void comboDrawBlit2D_IA4(Gfx** dl, u32 segAddr, int w, int h, float x, float y, float scale);

/* Event */
void comboOotSetEventChk(u16 flag);
void comboMmSetEventWeek(u16 flag);

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
void comboToggleOcarina(void);
void comboToggleHookshot(void);
#endif

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

/* Dpad */
#define DPF_ITEMS      0x01
#define DPF_EQUIP      0x02

void comboDpadDraw(GameState_Play* play);
void comboDpadUpdate(GameState_Play* play);
int  comboDpadUse(GameState_Play* play, int flags);

int comboConfig(int flag);
int comboDoorIsUnlocked(GameState_Play* play, Actor* actor);

/* Hints */
#define HINT_GOSSIP         0x00
#define HINT_GOSSIP_GROTTO  0x01

#define HINT_TYPE_PATH          0x00
#define HINT_TYPE_FOOLISH       0x01
#define HINT_TYPE_ITEM_EXACT    0x02
#define HINT_TYPE_ITEM_REGION   0x03
#define HINT_TYPE_JUNK          0x04

void comboInitHints(void);
void comboHintGossip(u8 key, GameState_Play* play);

/* Shop */
#define SC_OK               0x00
#define SC_OK_NOCUTSCENE    0x01
#define SC_ERR_CANNOTBUY    0x02
#define SC_ERR_NORUPEES     0x04

u8   comboShopItemSlot(GameState_Play* play, Actor_EnGirlA* girlA);
int  comboShopPrecond(GameState_Play* play, Actor_EnGirlA* girlA);
void comboShopDisplayTextBox(GameState_Play* play, Actor_EnGirlA* girlA);
void comboShopDisplayTextBoxConfirm(GameState_Play* play, Actor_EnGirlA* girlA);
void comboShopAfterBuy(GameState_Play* play, Actor_EnGirlA* girlA);
void comboShopSetupItem(GameState_Play* play, Actor_EnGirlA* girlA);
void comboShopUpdateItem(GameState_Play* play, Actor_EnGirlA* girlA);

/* Entrance */
void comboInitEntrances(void);
s32 comboEntranceOverride(s16 entranceId);
int comboBossLairIndex();

extern s8 gIsEntranceOverride;
extern s32 gLastEntrance;
extern s32 gLastScene;

/* Warp */
void comboTriggerWarp(GameState_Play* play, int index);

/* Custom Shaders */
void Shader_Xlu0(GameState_Play* play, s16 shaderId);

/* MQ */
void comboMqKaleidoHook(GameState_Play* play);

extern GameState_Play* gPlay;

#if defined(GAME_MM)
void Ocarina_HandleWarp(Actor_Player* player, GameState_Play* ctxt);
#endif

#if defined(GAME_MM)
extern int gNoTimeFlow;
#endif

/* Ocarina */
void comboCheckSong(const OcarinaSongButtons* songButtons, int songIndex);

extern u8 gCustomOcarinaSong;

/* Conds */
int comboSpecialCond(int special);
int comboGoalCond(void);

typedef struct ComboItemQuery ComboItemQuery;

/* Global data */
typedef struct
{
    u16                     initialEntrance;
    u8                      inGrotto;
    u8                      isCreditWarp;
    const ComboItemQuery*   itemQuery;
    const ComboItemQuery*   itemQueryBox;
    u8                      silverRupee;
    u8                      delayedSwitchFlag;
    u8                      roomEnemyLackSoul:1;
    u8                      spawnExtended:1;
    u8                      menuScreen;
    u8                      menuCursor;
    u8                      menuCursorMax;
    u8                      maxKeysOot[0x11];
    u8                      maxKeysMm[4];
    u8                      actorIndex;
    u8                      sceneSetupId;
    Actor_CustomItem*       customItemsList;
    s8                      keatonGrassMax;
}
ComboGlobal;

extern ComboGlobal g;

void comboCreditWarp(GameState_Play* play);

#if defined (GAME_OOT)
extern u16 gPrevPageIndex;
extern s16 gPrevCursorPoint;
extern s8 gSoaringIndexSelected;
#endif

void comboClearEpona(GameState_Play* play);

#else
# include <combo/asm.h>
#endif

#endif /* COMBO_COMBO_H */
