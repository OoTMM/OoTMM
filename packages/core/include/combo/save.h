#ifndef COMBO_SAVE_H
#define COMBO_SAVE_H

#define BTN_ENABLED     0x00
#define BTN_DISABLED    0xff

#define GAMEMODE_NORMAL         0
#define GAMEMODE_TITLE_SCREEN   1
#define GAMEMODE_FILE_SELECT    2
#define GAMEMODE_END_CREDIT     3
#if defined(GAME_MM)
# define GAMEMODE_OWL_SAVE      4
#endif

#if !defined(__ASSEMBLER__)
# include <combo/oot/save.h>
# include <combo/mm/save.h>

typedef struct ALIGNED(16)
{
    OotCustomSave   oot;
    MmCustomSave    mm;
    s16             netGiSkip[16];
    u16             coins[4];
    u16             ocarinaButtonMaskOot;
    u16             ocarinaButtonMaskMm;
    u8              soulsEnemyOot[8];
    u8              soulsEnemyMm[8];
    u8              soulsBossOot[2];
    u8              soulsBossMm[1];
    u8              soulsNpcOot[8];
    u8              soulsNpcMm[8];
    u8              soulsMiscOot[1];
    u8              soulsMiscMm[1];
    u8              caughtChildFishWeight[20]; /* first item is length. should this be in OotCustomSave? */
    u8              caughtAdultFishWeight[20]; /* first item is length. should this be in OotCustomSave? */
    u8              caughtFishFlags[5];
    RespawnData     respawn[1];
    u8              foundMasterSword:1;
    u8              storedSirloin:1;
    u8              extraSwordsOot:2;
#if defined(DEBUG)
    u8              cheats[4];
#endif
}
SharedCustomSave;

extern SharedCustomSave gSharedCustomSave;

#define SF_OWL          0x01
#define SF_NOCOMMIT     0x02
#define SF_PASSIVE      0x04

void Save_LoadOptions(void);
void Save_ReadOwn(void);
void Save_ReadForeign(void);
void Save_Write(void);
void Save_CreateMM(void);
void Save_CopyMM(int dst, int src);
void Save_OnLoad(void);
void Save_DoSave(PlayState* play, int saveFlags);

void Flash_ReadWrite(u32 devAddr, void* dramAddr, u32 size, s32 direction);

# if defined(GAME_OOT)
#  define gCustomSave gSharedCustomSave.oot
# else
#  define gCustomSave gSharedCustomSave.mm
# endif
#endif

#define SAVE_OFFSET_MM_1            0x08000
#define SAVE_OFFSET_MM_1_BACKUP     0x0c000
#define SAVE_OFFSET_MM_2            0x10000
#define SAVE_OFFSET_MM_2_BACKUP     0x14000
#define SAVE_OFFSET_MM_3            0x18000
#define SAVE_OFFSET_MM_3_BACKUP     0x1c000

#if defined(__ASSEMBLER__)
# if defined(GAME_OOT)
#  define gOotSave gSaveContext
# else
#  define gMmSave gSaveContext
# endif
# define SAVE_EXTRA_RECORD(type, index) (gOotSave + 0xd4 + 0x1c * (index) + 0x10)
#else
# define SAVE_EXTRA_RECORD(type, index) (*((type*)(gOotSave.info.perm[index].raw + 0x10)))
typedef struct
{
    u32 wispMmSwamp:1;
    u32 wispMmMountain:1;
    u32 wispMmOcean:1;
    u32 wispMmValley:1;
    u32 wispOotLake:1;
    u32 unused:27;
}
MiscFlags;
#endif

#define gOotExtraTrade              SAVE_EXTRA_RECORD(OotExtraTrade,     0)
#define gOotExtraItems              SAVE_EXTRA_RECORD(OotExtraItems,     1)
#define gOotExtraFlags              SAVE_EXTRA_RECORD(OotExtraFlags,     2)
#define gMmExtraBoss                SAVE_EXTRA_RECORD(MmExtraBoss,       3)
#define gMmExtraItems               SAVE_EXTRA_RECORD(MmExtraItems,      4)
#define gMmExtraTrade               SAVE_EXTRA_RECORD(MmExtraTrade,      5)
#define gMmExtraFlags               SAVE_EXTRA_RECORD(MmExtraFlags,      6)
#define gMmExtraFlags2              SAVE_EXTRA_RECORD(MmExtraFlags2,     7)
#define gMiscFlags                  SAVE_EXTRA_RECORD(MiscFlags,         8)
#define gCowFlags                   SAVE_EXTRA_RECORD(u32,               9)
#define gOotExtraTradeSave          SAVE_EXTRA_RECORD(OotExtraTrade,    10)
#define gMmOwlFlags                 SAVE_EXTRA_RECORD(u32,              11)
#define gSaveLedgerBase             SAVE_EXTRA_RECORD(u32,              12)
#define gMmExtraFlags3              SAVE_EXTRA_RECORD(MmExtraFlags3,    13)
#define gOotSilverRupeeCounts1      SAVE_EXTRA_RECORD(u32,              14)
#define gOotSilverRupeeCounts2      SAVE_EXTRA_RECORD(u32,              15)
#define gOotSilverRupeeCounts3      SAVE_EXTRA_RECORD(u32,              16)
#define gOotSilverRupeeCounts4      SAVE_EXTRA_RECORD(u32,              17)
#define gOotSilverRupeeCounts5      SAVE_EXTRA_RECORD(u32,              18)
#define gTriforceCount              SAVE_EXTRA_RECORD(u32,              19)

#endif
