#ifndef COMBO_CONFIG_H
#define COMBO_CONFIG_H

#include <combo/generated_config.h>
#include <combo/prices.h>
#include <combo/types.h>

#define BOMBCHU_BEHAVIOR_FREE           0
#define BOMBCHU_BEHAVIOR_BOMB_BAG       1
#define BOMBCHU_BEHAVIOR_BAG_FIRST      2
#define BOMBCHU_BEHAVIOR_BAG_SEPARATE   3

#define SPECIAL_BRIDGE      0
#define SPECIAL_MOON        1
#define SPECIAL_LACS        2
#define SPECIAL_GANON_BK    3
#define SPECIAL_MAJORA      4

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
    ItemHint oathToOrder[6];
    ItemHint ganonBossKey;
}
ComboDataHints;

typedef struct
{
    u32 flags;
    u16 count;
    u16 zero;
}
SpecialCond;

typedef struct
{
    u8              playerId;
    u32             dungeonWarps[12];
    u32             dungeonEntrances[26];
    u32             mq;
    u32             preCompleted;
    u32             entrancesSong[6];
    u32             entrancesOwl[10];
    u32             entrancesSpawns[2];
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
    u8              strayFairyRewardCount;
    u8              bombchuBehaviorOot;
    u8              bombchuBehaviorMm;
}
ComboConfig;

extern ComboConfig gComboConfig;

void Config_Init(void);
int Config_Flag(int flag);
int Config_SpecialCond(int special);
int Config_IsGoal(void);
int Config_IsMq(int dungeonId);

#endif
