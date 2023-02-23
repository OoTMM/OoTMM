#ifndef COMBO_SAVE_H
#define COMBO_SAVE_H

#if !defined(__ASSEMBLER__)
# include <combo/oot/save.h>
# include <combo/mm/save.h>
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
# define SAVE_EXTRA_RECORD(type, index) (*((type*)(gOotSave.perm[index].raw + 0x10)))
typedef struct
{
    u32 erSpring:1;
    u32 erSwampClear:1;
    u32 erCoastClear:1;
    u32 unused:29;
}
MiscFlags;
#endif

#define gOotExtraTrade SAVE_EXTRA_RECORD(OotExtraTrade, 0)
#define gOotExtraItems SAVE_EXTRA_RECORD(OotExtraItems, 1)
#define gOotExtraFlags SAVE_EXTRA_RECORD(OotExtraFlags, 2)
#define gMmExtraBoss   SAVE_EXTRA_RECORD(MmExtraBoss,   3)
#define gMmExtraTrade  SAVE_EXTRA_RECORD(MmExtraTrade,  4)
#define gMmExtraFlags  SAVE_EXTRA_RECORD(MmExtraFlags,  5)
#define gMmExtraFlags2 SAVE_EXTRA_RECORD(MmExtraFlags2, 6)
#define gMiscFlags     SAVE_EXTRA_RECORD(MiscFlags,     7)

#endif
