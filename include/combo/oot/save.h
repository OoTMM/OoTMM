#ifndef OOT_SAVE_H
#define OOT_SAVE_H

#include <combo/types.h>
#include <combo/util.h>

typedef union
{
    struct
    {
        u32 chests;
        u32 switches;
        u32 roomClear;
        u32 collectibles;
        u32 unused;
        u32 visitedRooms;
        u32 visitedFloors;
    };
    char raw[0x1c];
}
OotPermanentSceneFlags;

typedef struct
{
    u16  boots:4;
    u16  tunics:4;
    u16  shields:4;
    u16  swords:4;
}
OotSaveEquipment;

typedef struct
{
    u32  unused:9;
    u32  dekuNut:3;
    u32  dekuStick:3;
    u32  bulletBag:3;
    u32  wallet:2;
    u32  dive:3;
    u32  strength:3;
    u32  bombBag:3;
    u32  quiver:3;
}
OotSaveUpgrades;

typedef struct
{
    u32  heartPieces:4;
    u32  unused:4;
    u32  goldToken:1;
    u32  gerudoCard:1;
    u32  agonyStone:1;
    u32  stoneSapphire:1;
    u32  stoneRuby:1;
    u32  stoneEmerald:1;
    u32  songStorms:1;
    u32  songTime:1;
    u32  songSun:1;
    u32  songSaria:1;
    u32  songEpona:1;
    u32  songZelda:1;
    u32  songTpLight:1;
    u32  songTpShadow:1;
    u32  songTpSpirit:1;
    u32  songTpWater:1;
    u32  songTpFire:1;
    u32  songTpForest:1;
    u32  medallionLight:1;
    u32  medallionShadow:1;
    u32  medallionSpirit:1;
    u32  medallionWater:1;
    u32  medallionFire:1;
    u32  medallionForest:1;
}
OotSaveQuest;

typedef struct
{
    u8 unused:5;
    u8 map:1;
    u8 compass:1;
    u8 bossKey:1;
}
OotDungeonItems;

typedef struct
{
    u32                     entrance;
    u32                     age;
    u32                     cutscene;
    u16                     worldTime;
    u16                     unk_0e;
    u32                     isNight;
    char                    unk_14[0x4];
    u32                     tradeQuestFlag;
    char                    newf[6];
    s16                     deathCount;
    u8                      playerName[8];
    s16                     ddOnlyFlag;
    s16                     healthMax;
    s16                     health;
    s8                      magicSize;
    s8                      magicAmount;
    u16                     rupees;
    u16                     swordHealth;
    char                    unk_38[2];
    u8                      magicUpgrade;
    char                    unk_3b[1];
    u8                      magicUpgrade2;
    u8                      doubleDefense;
    u8                      isBiggoronSword;
    char                    unk_3f[0x27];
    u16                     sceneId;
    u8                      buttons[7];
    OotSaveEquipment        currentEquipment;
    char                    unk_72[0x2];
    u8                      inventory[0x18];
    u8                      ammo[0xf];
    u8                      beans;
    OotSaveEquipment        equipment;
    u16                     unk_9e;
    OotSaveUpgrades         upgrades;
    OotSaveQuest            quest;
    OotDungeonItems         dungeonItems[0x14];
    s8                      dungeonKeys[0x13];
    u8                      doubleDefenseHearts;
    u16                     goldTokens;
    OotPermanentSceneFlags  perm[101];
    char                    unk_be0[0x2f4];
    u16                     eventsChk[14];
    u16                     eventsItem[4];
    u16                     eventsMisc[30];
    char                    unk_f34[0x41e];
    u16                     checksum;
}
OotSave;

ASSERT_OFFSET(OotSave, unk_3f,      0x3f);
ASSERT_OFFSET(OotSave, sceneId,     0x66);
ASSERT_OFFSET(OotSave, buttons,     0x68);
ASSERT_OFFSET(OotSave, equipment,   0x9c);
ASSERT_OFFSET(OotSave, perm,        0xd4);
ASSERT_OFFSET(OotSave, unk_be0,     0xbe0);
ASSERT_OFFSET(OotSave, checksum,    0x1352);

typedef struct
{
    OotSave save;
    u32     fileIndex;
    char    unk_1358[0x04];
    s32     gameMode;
    char    unk_1360[0x37];
    u8      grottoChestFlag;
    char    unk_1398[0x34];
    s16     rupeesDelta;
    char    unk_13ce[0x28];
    s16     magicTarget;
    char    unk_13f8[0x0a];
    u16     mapIndex;
    char    unk_1404[0x20];
    u16     healthDelta;
    char    unk_1426[0x2a];
}
OotSaveContext;

ASSERT_OFFSET(OotSaveContext, gameMode,         0x135c);
ASSERT_OFFSET(OotSaveContext, unk_1360,         0x1360);
ASSERT_OFFSET(OotSaveContext, grottoChestFlag,  0x1397);
ASSERT_OFFSET(OotSaveContext, unk_1398,         0x1398);
ASSERT_OFFSET(OotSaveContext, unk_1404,         0x1404);
ASSERT_OFFSET(OotSaveContext, unk_1426,         0x1426);

_Static_assert(sizeof(OotSave) == 0x1354, "OotSave size is wrong");
_Static_assert(sizeof(OotSaveContext) == 0x1450, "OotSaveContext size is wrong");

#if defined(GAME_OOT)
ALIGNED(16) extern OotSaveContext gSaveContext;
# define gOotSave   (gSaveContext.save)
# define gSave      gOotSave
#else
ALIGNED(16) extern OotSave gOotSave;
#endif

/* Custom */
typedef struct
{
    u16 child;
    u16 adult;
}
OotExtraTrade;

typedef struct
{
    u8 unused;
    u8 hookshot;
    u8 shield;
    u8 ocarina;
}
OotExtraItems;

typedef struct
{
    u32 ganon:1;
    u32 greatFairies:6;
    u32 fishingChild:1;
    u32 fishingAdult:1;
    u32 tunicGoron:1;
    u32 biggoron:1;
    u32 tunicZora:1;
    u32 unused:20;
}
OotExtraFlags;

#define gOotExtraTrade (*((OotExtraTrade*)(gOotSave.perm[0].raw + 0x10)))
#define gOotExtraItems (*((OotExtraItems*)(gOotSave.perm[1].raw + 0x10)))
#define gOotExtraFlags (*((OotExtraFlags*)(gOotSave.perm[2].raw + 0x10)))

#endif
