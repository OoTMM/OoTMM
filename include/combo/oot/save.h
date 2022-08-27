#ifndef OOT_SAVE_H
#define OOT_SAVE_H

#include <combo/types.h>

typedef struct PACKED ALIGNED(4)
{
    u32 chests;
    u32 switches;
    u32 roomClear;
    u32 collectibles;
    u32 unused;
    u32 visitedRooms;
    u32 visitedFloors;
}
OotPermanentSceneFlags;

typedef struct PACKED ALIGNED(2)
{
    u16  boots:4;
    u16  tunics:4;
    u16  shields:4;
    u16  swords:4;
}
OotSaveEquipment;

typedef struct PACKED ALIGNED(4)
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

typedef struct PACKED ALIGNED(4)
{
    u32  unused:9;
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

typedef struct PACKED ALIGNED(4)
{
    u32                     entrance;
    u32                     age;
    u32                     cutscene;
    u16                     worldTime;
    u16                     unk_0e;
    u32                     isNight;
    char                    unk_14[0x8];
    char                    newf[6];
    s16                     deathCount;
    char                    playerName[8];
    s16                     ddOnlyFlag;
    s16                     healthMax;
    s16                     health;
    s8                      magicSize;
    s8                      magicAmount;
    u16                     rupees;
    char                    unk_36[4];
    u8                      magicUpgrade;
    char                    unk_3b[1];
    u8                      magicUpgrade2;
    char                    unk_3d[0x37];
    u8                      inventory[0x18];
    u8                      ammo[0xf];
    u8                      beans;
    OotSaveEquipment        equipment;
    u16                     unk_9e;
    OotSaveUpgrades         upgrades;
    OotSaveQuest            quest;
    u8                      dungeonItems[0x14];
    u8                      dungeonKeys[0x14];
    char                    unk_0xd0[4];
    OotPermanentSceneFlags  perm[101];
    char                    unk_be0[0x2f4];
    u16                     eventsChk[14];
    u16                     eventsItem[4];
    u16                     eventsMisc[30];
    char                    unk_f34[0x41e];
    u16                     checksum;
}
OotSave;

typedef struct PACKED ALIGNED(4)
{
    OotSave save;
    u32     fileIndex;
    char    unk_1358[0x74];
    s16     rupeesDelta;
    char    unk_13ce[0x28];
    s16     magicTarget;
    char    unk_13f8[0x58];
}
OotSaveContext;

_Static_assert(sizeof(OotSave) == 0x1354, "OotSave size is wrong");
_Static_assert(sizeof(OotSaveContext) == 0x1450, "OotSaveContext size is wrong");

#if defined(GAME_OOT)
ALIGNED(16) extern OotSaveContext gSaveContext;
# define gOotSave   (gSaveContext.save)
# define gSave      gOotSave
#else
ALIGNED(16) extern OotSave gOotSave;
#endif

#endif /* OOT_SAVE_H */
