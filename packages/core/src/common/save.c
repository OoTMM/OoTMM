#include <combo.h>
#include <combo/audio.h>
#include <combo/item.h>
#include <combo/net.h>
#include <combo/time.h>
#include <combo/magic.h>
#include <combo/config.h>
#include <combo/global.h>

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

void Save_LoadOptions(void)
{
    u8 ootHeader[0x20];

    Flash_ReadWrite(0x00000, ootHeader, sizeof(ootHeader), OS_READ);

#if defined(GAME_OOT)
    gSaveContext.audioSetting = ootHeader[0];
    gSaveContext.zTargetSetting = ootHeader[1];
#endif

#if defined(GAME_MM)
    gSaveContext.options.optionId = 0xa51d;
    gSaveContext.options.unk_02 = 1;
    gSaveContext.options.audio = ootHeader[0];
    gSaveContext.options.unk_04 = 0;
    gSaveContext.options.zTarget = ootHeader[1];
#endif
    Audio_SetFileSelectSettings(ootHeader[0]);
}

void Save_OnLoad(void)
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
    gOotSave.info.checksum = 0;
    gOotSave.info.checksum = computeChecksumOot(&gOotSave, sizeof(gOotSave));

    /* Write the save data to flash */
    base = 0x20 + 0x1450 * gSaveContext.fileNum;
    Flash_ReadWrite(base, &gOotSave, sizeof(gOotSave), OS_WRITE);
    Flash_ReadWrite(base + 0x3cf0, &gOotSave, sizeof(gOotSave), OS_WRITE);
}

static void saveMm(void)
{
    u32 base;

    /* Compute save args */
    base = 0x8000 + 0x8000 * gSaveContext.fileNum;

    /* Set the checksum */
    gMmSave.info.checksum = 0;
    gMmSave.info.checksum = computeChecksumMm(&gMmSave, sizeof(gMmSave));

    /* Write the save data to flash */
    Flash_ReadWrite(base, &gMmSave, sizeof(gMmSave), OS_WRITE);
    Flash_ReadWrite(base + 0x4000, &gMmSave, sizeof(gMmSave), OS_WRITE);
}

void Save_ReadOwn(void)
{
    u32 fileIndex = gSaveContext.fileNum;

#if defined(GAME_OOT)
    Flash_ReadWrite(0x20 + 0x1450 * fileIndex, &gOotSave, sizeof(gOotSave), OS_READ);
#endif

#if defined(GAME_MM)
    Flash_ReadWrite(0x8000 + 0x8000 * fileIndex, &gMmSave, sizeof(gMmSave), OS_READ);
#endif
}

void Save_ReadForeign(void)
{
    u32 fileIndex = gSaveContext.fileNum;

#if !defined(GAME_OOT)
    Flash_ReadWrite(0x20 + 0x1450 * fileIndex, &gOotSave, sizeof(gOotSave), OS_READ);
#endif

#if !defined(GAME_MM)
    Flash_ReadWrite(0x8000 + 0x8000 * fileIndex, &gMmSave, sizeof(gMmSave), OS_READ);
#endif

    Flash_ReadWrite(0x18000 + 0x4000 * fileIndex, &gSharedCustomSave, sizeof(gSharedCustomSave), OS_READ);
}

static void saveFixup(void)
{
    /* Instantly fill the magic bar */
    if (gSaveContext.magicState == MAGIC_STATE_FILL)
        gSave.info.playerData.magic = gSaveContext.magicFillTarget;
}

void Save_Write(void)
{
    NetContext* net;

    if (gSaveContext.fileNum == 0xff)
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
    Flash_ReadWrite(0x18000 + 0x4000 * gSaveContext.fileNum, &gSharedCustomSave, sizeof(gSharedCustomSave), OS_WRITE);
}

static void copyRawSave(u32 dst, u32 src, int size)
{
    ALIGNED(16) char buf[0x100];

    for (int i = 0; i < size / sizeof(buf); ++i)
    {
        Flash_ReadWrite(src, buf, sizeof(buf), OS_READ);
        Flash_ReadWrite(dst, buf, sizeof(buf), OS_WRITE);
        src += sizeof(buf);
        dst += sizeof(buf);
    }
}

void Save_CopyMM(int fileIndexDst, int fileIndexSrc)
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
    if(Config_Flag(CFG_MM_CLOCK_SPEED_VERYSLOW))
        invertSpeed = 0;
    if(Config_Flag(CFG_MM_CLOCK_SPEED_SLOW))
        invertSpeed = -1;
    if(Config_Flag(CFG_MM_CLOCK_SPEED_FAST))
        invertSpeed = -4;
    if(Config_Flag(CFG_MM_CLOCK_SPEED_VERYFAST))
        invertSpeed = -6;
    if(Config_Flag(CFG_MM_CLOCK_SPEED_SUPERFAST))
        invertSpeed = -12;

    if (Config_Flag(CFG_MM_AUTO_INVERT_ALWAYS) || (Config_Flag(CFG_MM_AUTO_INVERT_FIRST_CYCLE) && gMmSave.info.playerData.songOfTimeCount == 0))
        gMmSave.daySpeed = invertSpeed;
}

void comboClearCustomRespawn(CustomRespawnMode customRespawnMode)
{
    bzero(&gSharedCustomSave.respawn[customRespawnMode], sizeof(RespawnData));
}
