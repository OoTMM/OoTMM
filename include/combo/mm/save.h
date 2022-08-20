#ifndef MM_SAVE_H
#define MM_SAVE_H

#include <types.h>

typedef struct PACKED
{
    char    newf[6];
    u16     deaths;
    char    playerName[8];
    s16     healthCapacity;
    s16     health;
    s8      magicLevel;
    s8      magic;
    s16     rupees;
    u16     swordHealth;
    u16     tatlTimer;
    u8      magicAcquired;
    u8      doubleMagic;
    u8      doubleDefense;
    u8      unk_1F;
    u16     unk_20;
    u16     owlActivationFlags;
    u16     unk_24;
    s16     savedSceneNum;
}
MmSavePlayerData;

typedef struct PACKED
{
    u8      buttonItems[4][4];
    u8      cButtonSlots[4][4];
    u16     equipment;
}
MmItemEquips;

typedef struct PACKED
{
    u8      items[48];
    s8      ammo[24];
    u32     upgrades;
    u32     questItems;
    u8      dungeonItems[10];
    s8      dungeonKeys[9];
    s8      defenseHearts;
    s8      strayFairies[10];
    char    dekuPlaygroundPlayerName[3][8];
    u16     unk;
}
MmInventory;

typedef struct PACKED
{
    char data[0x1c];
}
MmPermanentSceneFlags;

typedef struct PACKED
{
    s16         scene;
    Vector3s    pos;
    s16         yaw;
}
MmHorseData;

typedef struct PACKED
{
    s32                     entranceIndex;
    u8                      equippedMask;
    u8                      isFirstCycle;
    char                    unk_006;
    u8                      linkAge;
    s32                     cutscene;
    u16                     time;
    u16                     owlLocation;
    s32                     isNight;
    s32                     daySpeed;
    s32                     day;
    u32                     daysElapsed;
    u8                      playerForm;
    u8                      snowheadCleared;
    u8                      hasTatl;
    u8                      isOwlSave;
    MmSavePlayerData        playerData;
    MmItemEquips            itemEquips;
    char                    unk_06e[0x2];
    MmInventory             inventory;
    MmPermanentSceneFlags   permanentSceneFlags[120];
    char                    unk_e18[0x1e8];
    MmHorseData             horseData;
    u16                     checksum;
}
MmSave;

typedef struct PACKED
{
    MmSave save;
    char   unk[0x2c94];
    u32    fileIndex;
}
MmSaveContext;

_Static_assert(sizeof(MmSave) == 0x100c, "MmSave size is wrong");

#if defined(GAME_MM)
ALIGNED(16) extern MmSaveContext gSaveContext;
# define gMmSave    (gSaveContext.save)
# define gSave      gMmSave
#else
ALIGNED(16) extern MmSave gMmSave;
#endif

#endif /* MM_SAVE_H */
