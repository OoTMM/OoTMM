#ifndef INCLUDED_COMBO_ITEMS_H
#define INCLUDED_COMBO_ITEMS_H

#include <combo/types.h>
#include <combo/data/items.h>

/* Add funcs */
#define IA_OOT_RUPEE            0x00
#define IA_MM_RUPEE             0x01
#define IA_OOT_WALLET           0x02
#define IA_MM_WALLET            0x03
#define IA_OOT_BOMBS            0x04
#define IA_MM_BOMBS             0x05
#define IA_OOT_NUTS             0x06
#define IA_MM_NUTS              0x07
#define IA_NUT_UPGRADE          0x08
#define IA_OOT_BOMBCHU          0x09
#define IA_MM_BOMBCHU           0x0a
#define IA_OOT_ARROWS           0x0b
#define IA_MM_ARROWS            0x0c
#define IA_OOT_BOW              0x0d
#define IA_MM_BOW               0x0e
#define IA_SEEDS                0x0f
#define IA_SLINGSHOT            0x10
#define IA_OOT_NORMAL           0x11
#define IA_MM_NORMAL            0x12
#define IA_OOT_STICKS           0x13
#define IA_MM_STICKS            0x14
#define IA_STICK_UPGRADE        0x15
#define IA_OOT_HOOKSHOT         0x16
#define IA_MM_HOOKSHOT          0x17
#define IA_OOT_TRADE_CHILD      0x18
#define IA_OOT_TRADE_ADULT      0x19
#define IA_MM_TRADE1            0x1a
#define IA_MM_TRADE2            0x1b
#define IA_MM_TRADE3            0x1c
#define IA_OOT_OCARINA          0x1d
#define IA_MM_OCARINA           0x1e
#define IA_OOT_BOTTLE_NEW       0x1f
#define IA_OOT_BOTTLE_REFILL    0x20
#define IA_MM_BOTTLE_NEW        0x21
#define IA_MM_BOTTLE_REFILL     0x22
#define IA_OOT_BEANS            0x23
#define IA_MM_BEANS             0x24
#define IA_OOT_SWORD            0x25
#define IA_MM_SWORD             0x26
#define IA_OOT_BOMB_BAG         0x27
#define IA_MM_BOMB_BAG          0x28
#define IA_OOT_SHIELD           0x29
#define IA_MM_SHIELD            0x2a
#define IA_TUNIC                0x2b
#define IA_BOOTS                0x2c
#define IA_OOT_STRENGTH         0x2d
#define IA_OOT_SCALE            0x2e
#define IA_OOT_QUEST            0x2f
#define IA_MM_QUEST             0x30
#define IA_OOT_HEART            0x31
#define IA_MM_HEART             0x32
#define IA_OOT_DEFENSE_UPGRADE  0x33
#define IA_MM_DEFENSE_UPGRADE   0x34
#define IA_OOT_HEART_PIECE      0x35
#define IA_MM_HEART_PIECE       0x36
#define IA_OOT_SMALL_KEY        0x37
#define IA_MM_SMALL_KEY         0x38
#define IA_OOT_KEY_RING         0x39
#define IA_MM_KEY_RING          0x3a
#define IA_OOT_SKELETON_KEY     0x3b
#define IA_MM_SKELETON_KEY      0x3c
#define IA_OOT_BOSS_KEY         0x3d
#define IA_MM_BOSS_KEY          0x3e
#define IA_OOT_COMPASS          0x3f
#define IA_MM_COMPASS           0x40
#define IA_OOT_MAP              0x41
#define IA_MM_MAP               0x42
#define IA_SILVER_RUPEE         0x43
#define IA_SILVER_POUCH         0x44
#define IA_MAGICAL_RUPEE        0x45
#define IA_GS_TOKEN             0x46
#define IA_GS_TOKEN_SWAMP       0x47
#define IA_GS_TOKEN_OCEAN       0x48
#define IA_OOT_MAGIC_UPGRADE    0x49
#define IA_MM_MAGIC_UPGRADE     0x4a
#define IA_STRAY_FAIRY          0x4b
#define IA_TRIFORCE             0x4c
#define IA_COIN                 0x4d
#define IA_OOT_BUTTON           0x4e
#define IA_MM_BUTTON            0x4f
#define IA_KEG                  0x50
#define IA_SPIN_UPGRADE         0x51
#define IA_OOT_SOUL             0x52
#define IA_MM_SOUL              0x53
#define IA_POND_FISH            0x54
#define IA_WORLD_MAP            0x55
#define IA_OWL                  0x56
#define IA_OOT_MAGIC            0x57
#define IA_MM_MAGIC             0x58
#define IA_OOT_BOMBCHU_BAG      0x59
#define IA_MM_BOMBCHU_BAG       0x5a
#define IA_OOT_FAIRY_BIG        0x5b
#define IA_MM_FAIRY_BIG         0x5c
#define IA_MM_SCALE             0x5d
#define IA_MM_STRENGTH          0x5e
#define IA_MM_CLOCK             0x5f
#define IA_ENDGAME              0x60
#define IA_OOT_SONG_EMPTINESS   0x61
#define IA_NONE                 0xff

#define ITT_NONE        0x00
#define ITT_NORMAL      0x01
#define ITT_MINOR       0x02
#define ITT_MAJOR       0x03
#define ITT_RUPEE       0x04
#define ITT_BOSS_KEY    0x05
#define ITT_KEY         0x06
#define ITT_SKULL       0x07
#define ITT_FAIRY       0x08
#define ITT_HEART       0x09
#define ITT_SOUL        0x0a
#define ITT_MASK        0x0b

typedef struct GameState_Play GameState_Play;

typedef struct ComboItemQuery
{
    s16 gi;
    s16 giRenew;
    u16 ovFlags;
    u8  ovType;
    u8  sceneId;
    u8  roomId;
    u8  id;
    u8  from;
}
ComboItemQuery;

typedef struct ComboItemOverride
{
    u8  player;
    u8  playerFrom;
    s16 giRaw;
    s16 gi;
}
ComboItemOverride;

extern u16 gMaxRupees[];
extern u16 gWalletDigits[];

#if defined(GAME_OOT)
# define gOotMaxRupees gMaxRupees
extern u16 gMmMaxRupees[];
#endif

#if defined(GAME_MM)
# define gMmMaxRupees gMaxRupees
extern u16 gOotMaxRupees[];
#endif

extern const u8 kMaxSticks[];
extern const u8 kMaxNuts[];
extern const u8 kMaxBombs[];
extern const u8 kMaxArrows[];
extern const u8 kMaxSeeds[];
extern const u8 kOotTradeChild[];
extern const u8 kOotTradeAdult[];
extern const u8 kMmTrade1[];
extern const u8 kMmTrade2[];
extern const u8 kMmTrade3[];

void comboSyncItems(void);

void reloadSlotOot(GameState_Play* play, int slot);
void reloadSlotMm(GameState_Play* play, int slot);

int comboAddItemRaw(GameState_Play* play, s16 gi);
int comboAddItemRawEx(GameState_Play* play, const ComboItemQuery* q, int updateText);
int comboAddItemEx(GameState_Play* play, const ComboItemQuery* q, int updateText);

int isItemLicensed(s16 gi);
int isItemFastBuy(s16 gi);
int isItemBuyable(s16 gi);

int comboItemPrecondEx(const ComboItemQuery* q, s16 price);
s16 comboRenewable(s16 gi, s16 def);

#define ITEM_QUERY_INIT { 0 }

void comboGiveItem(Actor* actor, GameState_Play* play, const ComboItemQuery* q, float a, float b);
void comboGiveItemNpc(Actor* actor, GameState_Play* play, s16 gi, int npcId, float a, float b);
void comboGiveItemNpcEx(Actor* actor, GameState_Play* play, s16 gi, int npcId, int flags, float a, float b);
void comboItemOverride(ComboItemOverride* dst, const ComboItemQuery* q);
u8   comboItemType(s16 gi);

#endif

u8 comboSceneKey(u8 sceneId);

void comboPlayItemFanfare(s16 gi, int isShort);

s16 comboItemResolve(GameState_Play* play, s16 gi);
