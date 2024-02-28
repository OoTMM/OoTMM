#include <combo.h>
#include <combo/item.h>
#include <combo/net.h>
#include <combo/time.h>

#if !defined(GAME_OOT)
ALIGNED(16) OotSave gOotSave;
#endif

#if !defined(GAME_MM)
ALIGNED(16) MmSave gMmSave;
#endif

ALIGNED(16) SharedCustomSave gSharedCustomSave;

#if defined(GAME_MM)
# define GRACE 0x1e0

void gracePeriod(void)
{
    u32 linear;
    u32 moonCrash;
    u32 linearGrace;
    u8 d;
    u16 t;

    linear = Time_Game2Linear(gMmSave.day, gMmSave.time);
    moonCrash = Time_LinearMoonCrash();
    if (Time_IsMoonCrashLinear(linear) || (linear < moonCrash && linear + GRACE >= moonCrash))
    {
        linearGrace = moonCrash - GRACE;
        Time_Linear2Game(&d, &t, linearGrace);
        gMmSave.day = d;
        gMmSave.time = t;
    }
}
#endif

void comboOnSaveLoad(void)
{
    NetContext* net;

    /* Clear custom trigger data */
    bzero(&gComboTriggersData, sizeof(gComboTriggersData));

    /* Clear "next item" */
    g.itemQuery = NULL;
    g.itemQueryBox = NULL;

    /* Clear network */
    net = netMutexLock();
    net->ledgerBase = gSaveLedgerBase;
    bzero(&net->cmdIn, sizeof(net->cmdIn));
    bzero(&net->cmdOut, sizeof(net->cmdOut));
    netMutexUnlock();

    comboWalletRefresh();

#if defined(GAME_MM)
    gracePeriod();
#endif
}

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

    /* Compute save args */
    base = 0x8000 + 0x8000 * gSaveContext.fileIndex;

    /* Set the checksum */
    gMmSave.checksum = 0;
    gMmSave.checksum = computeChecksumMm(&gMmSave, sizeof(gMmSave));

    /* Write the save data to flash */
    comboReadWriteFlash(base, &gMmSave, sizeof(gMmSave), OS_WRITE);
    comboReadWriteFlash(base + 0x4000, &gMmSave, sizeof(gMmSave), OS_WRITE);
}

void comboReadOwnSave(void)
{
    u32 fileIndex = gSaveContext.fileIndex;

#if defined(GAME_OOT)
    comboReadWriteFlash(0x20 + 0x1450 * fileIndex, &gOotSave, sizeof(gOotSave), OS_READ);
#endif

#if defined(GAME_MM)
    comboReadWriteFlash(0x8000 + 0x8000 * fileIndex, &gMmSave, sizeof(gMmSave), OS_READ);
#endif
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

    comboReadWriteFlash(0x18000 + 0x4000 * fileIndex, &gSharedCustomSave, sizeof(gSharedCustomSave), OS_READ);
}

static void saveFixup(void)
{
    /* Instantly fill the magic bar */
    if (gSaveContext.magicState == MAGIC_STATE_FILL)
        gSave.playerData.magicAmount = gSaveContext.magicFillTarget;
}

void comboWriteSave(void)
{
    NetContext* net;

    if (gSaveContext.fileIndex == 0xff)
        return;

    /* Save the network part */
    net = netMutexLock();
    netWaitCmdClear();
    net->ledgerBase = gSaveLedgerBase;
    netMutexUnlock();

    saveFixup();
    comboSyncItems();
    saveOot();
    saveMm();

    /* Write the custom save */
    comboReadWriteFlash(0x18000 + 0x4000 * gSaveContext.fileIndex, &gSharedCustomSave, sizeof(gSharedCustomSave), OS_WRITE);
}

static void copyRawSave(u32 dst, u32 src, int size)
{
    ALIGNED(16) char buf[0x100];

    for (int i = 0; i < size / sizeof(buf); ++i)
    {
        comboReadWriteFlash(src, buf, sizeof(buf), OS_READ);
        comboReadWriteFlash(dst, buf, sizeof(buf), OS_WRITE);
        src += sizeof(buf);
        dst += sizeof(buf);
    }
}

void comboCopyMmSave(int fileIndexDst, int fileIndexSrc)
{
    /* Copy the actual MM save */
    copyRawSave(0x8000 + 0x8000 * fileIndexDst, 0x8000 + 0x8000 * fileIndexSrc, 0x8000);

    /* Copy the custom save */
    copyRawSave(0x18000 + 0x4000 * fileIndexDst, 0x18000 + 0x4000 * fileIndexSrc, 0x4000);
}

void comboHandleAutoInvertClockSpeed(void)
{
    s32 invertSpeed;

    invertSpeed = -2;
    if(comboConfig(CFG_MM_CLOCK_SPEED_VERYSLOW))
        invertSpeed = 0;
    if(comboConfig(CFG_MM_CLOCK_SPEED_SLOW))
        invertSpeed = -1;
    if(comboConfig(CFG_MM_CLOCK_SPEED_FAST))
        invertSpeed = -4;
    if(comboConfig(CFG_MM_CLOCK_SPEED_VERYFAST))
        invertSpeed = -6;
    if(comboConfig(CFG_MM_CLOCK_SPEED_SUPERFAST))
        invertSpeed = -12;

    if (comboConfig(CFG_MM_AUTO_INVERT_ALWAYS) || (comboConfig(CFG_MM_AUTO_INVERT_FIRST_CYCLE) && gMmSave.playerData.songOfTimeCount == 0))
        gMmSave.daySpeed = invertSpeed;
}
