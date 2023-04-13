#ifndef INCLUDED_COMBO_ITEMS_H
#define INCLUDED_COMBO_ITEMS_H

#include <combo/types.h>
#include <combo/data/items.h>

typedef struct GameState_Play GameState_Play;

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

void comboSyncItems(void);

int  comboAddItemMm(s16 gi, int noEffect);
int  comboAddItemOot(s16 gi, int noEffect);
void comboAddItemSharedMm(s16 gi, int noEffect);
void comboAddItemSharedOot(s16 gi, int noEffect);
int  comboAddItemEffect(GameState_Play* play, s16 gi);
void comboAddItemSharedForeignEffect(GameState_Play* play, s16 gi);

int  comboAddSmallKeyOot(u16 dungeonId);
void comboAddBossKeyOot(u16 dungeonId);
void comboAddCompassOot(u16 dungeonId);
void comboAddMapOot(u16 dungeonId);
int  comboAddSmallKeyMm(u16 dungeonId);
void comboAddBossKeyMm(u16 dungeonId);
int  comboAddStrayFairyMm(u16 dungeonId);
void comboAddMapMm(u16 dungeonId);
void comboAddCompassMm(u16 dungeonId);

void comboAddQuiverOot(int level);
void comboAddQuiverMm(int level);
void comboAddArrowsOot(int count);
void comboAddArrowsMm(int count);
void comboAddBombBagOot(int level);
void comboAddBombBagMm(int level);
void comboAddBombsOot(int count);
void comboAddBombsMm(int count);
void comboAddMagicUpgradeOot(int level);
void comboAddMagicUpgradeMm(int level);
void comboAddSticksOot(int count);
void comboAddSticksMm(int count);
void comboAddNutsOot(int count);
void comboAddNutsMm(int count);

void comboAddCommonItemOot(int sid, int noEffect);
void comboAddCommonItemMm(int sid, int noEffect);

int  comboAddItem(GameState_Play* play, s16 gi);
int  comboAddItemNoEffect(s16 gi);

int comboIsItemUnavailable(s16 gi);
int comboIsItemMinor(s16 gi);
int comboIsItemConsumable(s16 gi);

int comboItemPrecond(s16 gi, s16 price);

#endif
