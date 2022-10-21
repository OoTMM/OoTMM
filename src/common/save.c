#include <combo.h>

#if !defined(GAME_OOT)
ALIGNED(16) OotSave gOotSave;
#endif

#if !defined(GAME_MM)
ALIGNED(16) MmSave gMmSave;
#endif

static u16 computeChecksumOot(void* data, int len)
{
    u16 checksum;
    u16* p;

    checksum = 0;
    len = len / 2;
    p = (u16*)data;
    while (len > 0)
    {
        checksum += *p;
        ++p;
        len--;
    }
    return checksum;
}

static u16 computeChecksumMm(void* data, int len)
{
    u16 checksum;
    u8* p;

    checksum = 0;
    p = (u8*)data;
    while (len > 0)
    {
        checksum += *p;
        ++p;
        len--;
    }
    return checksum;
}

static void saveOot(void)
{
    u32 base;

    /* Set the checksum */
    gOotSave.checksum = 0;
    gOotSave.checksum = computeChecksumOot(&gOotSave, sizeof(gOotSave));

    /* Write the save data to flash */
    base = 0x20 + 0x1450 * gSaveContext.fileIndex;
    comboReadWriteFlash(base, &gOotSave, sizeof(gOotSave), OS_WRITE);
    comboReadWriteFlash(base + 0x3cf0, &gOotSave, sizeof(gOotSave), OS_WRITE);
}

static void saveMm(void)
{
    u32 base;
    u32 saveSize;

    /* Compute save args */
    base = 0x8000 + 0x8000 * gSaveContext.fileIndex;

    /* Set the checksum */
    gMmSave.checksum = 0;
    gMmSave.checksum = computeChecksumMm(&gMmSave, sizeof(gMmSave));

    /* Write the save data to flash */
    comboReadWriteFlash(base, &gMmSave, sizeof(gMmSave), OS_WRITE);
    comboReadWriteFlash(base + 0x4000, &gMmSave, sizeof(gMmSave), OS_WRITE);
}

void comboReadForeignSave(void)
{
    u32 fileIndex = gSaveContext.fileIndex;

#if !defined(GAME_OOT)
    comboReadWriteFlash(0x20 + 0x1450 * fileIndex, &gOotSave, sizeof(gOotSave), OS_READ);
#endif

#if !defined(GAME_MM)
    comboReadWriteFlash(0x8000 + 0x8000 * fileIndex, &gMmSave, sizeof(gMmSave), OS_READ);
#endif
}

void comboWriteForeignSave(void)
{
#if !defined(GAME_OOT)
    saveOot();
#endif

#if !defined(GAME_MM)
    saveMm();
#endif
}

void comboWriteSave(void)
{
    saveOot();
    saveMm();
}
