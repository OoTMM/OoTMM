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
    char data[0x22];
}
MmItemEquips;

typedef struct PACKED
{
    char data[0x88];
}
MmInventory;

typedef struct PACKED
{
    char data[0x1c];
}
MmPermanentSceneFlags;

typedef struct PACKED
{
    s16     scene;
    Vec3s   pos;
    s16     yaw;
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
}
MmSaveContext;

_Static_assert(sizeof(MmSave) == 0x100c, "MmSave size is wrong");

#endif /* MM_SAVE_H */
