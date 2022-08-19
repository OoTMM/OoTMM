#ifndef OOT_SAVE_H
#define OOT_SAVE_H

#include <types.h>

typedef struct PACKED ALIGNED(4)
{
    u32     entrance;
    u32     age;
    u32     cutscene;
    u16     worldTime;
    u16     unk_0e;
    u32     isNight;
    char    unk_14[0x8];
    char    newf[6];
    s16     deathCount;
    char    playerName[8];
    s16     ddOnlyFlag;
    s16     healthMax;
    s16     health;
    s8      magicSize;
    s8      magicAmount;
    u16     rupees;
    char    unk_36[0x3e];
    u8      inventory[0x18];
    char    data[0x12c6];
    u16     checksum;
}
OotSave;

typedef struct PACKED ALIGNED(4)
{
    OotSave save;
    u32     fileIndex;
}
OotSaveContext;

_Static_assert(sizeof(OotSave) == 0x1354, "OotSave size is wrong");

#if defined(GAME_OOT)
ALIGNED(16) extern OotSaveContext gSaveContext;
# define gOotSave   (gSaveContext.save)
# define gSave      gOotSave
#else
ALIGNED(16) extern OotSave gOotSave;
#endif

#endif /* OOT_SAVE_H */
