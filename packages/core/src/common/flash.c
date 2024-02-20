#include <combo.h>
#include <PR/os_internal_flash.h>

#define FLASH_BLOCK_MASK (FLASH_BLOCK_SIZE - 1)

#if defined(GAME_OOT)

u32         __osFlashID[4] ALIGNED(8);
OSIoMesg    __osFlashMsg ALIGNED(8);
OSMesgQueue __osFlashMessageQ ALIGNED(8);
OSPiHandle  __osFlashHandler ALIGNED(8);
OSMesg      __osFlashMsgBuf[1];
s32         __osFlashVersion;

u64  __osDisableInt(void);
void __osRestoreInt(u64);

s32 osEPiLinkHandle(OSPiHandle* handle)
{
    u64 intmask;

    intmask = __osDisableInt();
    handle->next = __osPiTable;
    __osPiTable = handle;
    __osRestoreInt(intmask);

    return 0;
}

u32 osFlashGetAddr(u32 pageNum)
{
    if (__osFlashVersion == OLD_FLASH)
        return pageNum * (FLASH_BLOCK_SIZE >> 1);
    else
        return pageNum * FLASH_BLOCK_SIZE;
}

OSPiHandle* osFlashInit(void)
{
    u32 flash_type;
    u32 flash_maker;

    osCreateMesgQueue(&__osFlashMessageQ, __osFlashMsgBuf, 1);
    if (__osFlashHandler.baseAddress == K0_TO_K1(FLASH_START_ADDR))
        return &__osFlashHandler;

    __osFlashHandler.type = DEVICE_TYPE_FLASH;
    __osFlashHandler.baseAddress = K0_TO_K1(FLASH_START_ADDR);
    __osFlashHandler.latency = FLASH_LATENCY;
    __osFlashHandler.pulse = FLASH_PULSE;
    __osFlashHandler.pageSize = FLASH_PAGE_SIZE;
    __osFlashHandler.relDuration = FLASH_REL_DURATION;
    __osFlashHandler.domain = PI_DOMAIN2;
    __osFlashHandler.speed = 0;
    bzero(&__osFlashHandler.transferInfo, sizeof(__osFlashHandler.transferInfo));
    osEPiLinkHandle(&__osFlashHandler);

    /* Read maker to determine flash version */
    osFlashReadId(&flash_type, &flash_maker);
    switch (flash_maker)
    {
    case FLASH_VERSION_MX_PROTO_A:
    case FLASH_VERSION_MX_A:
    case FLASH_VERSION_MX_C:
        __osFlashVersion = OLD_FLASH;
        break;
    default:
        __osFlashVersion = NEW_FLASH;
        break;
    }

    return &__osFlashHandler;
}

void osFlashReadStatus(u8* status)
{
    u32 tmp;

    osEPiWriteIo(&__osFlashHandler, __osFlashHandler.baseAddress | FLASH_CMD_REG, FLASH_CMD_STATUS);
    osEPiReadIo(&__osFlashHandler, __osFlashHandler.baseAddress, &tmp);
    osEPiWriteIo(&__osFlashHandler, __osFlashHandler.baseAddress | FLASH_CMD_REG, FLASH_CMD_STATUS);
    osEPiReadIo(&__osFlashHandler, __osFlashHandler.baseAddress, &tmp);

    *status = tmp & 0xff;
}

void osFlashReadId(u32 *flash_type, u32 *flash_maker)
{
    u8 status;

    osFlashReadStatus(&status);
    osEPiWriteIo(&__osFlashHandler, __osFlashHandler.baseAddress | FLASH_CMD_REG, FLASH_CMD_ID);
    __osFlashMsg.hdr.pri = OS_MESG_PRI_NORMAL;
    __osFlashMsg.hdr.retQueue = &__osFlashMessageQ;
    __osFlashMsg.dramAddr = __osFlashID;
    __osFlashMsg.devAddr = 0;
    __osFlashMsg.size = 8;
    osInvalDCache(__osFlashID, 0x10);
    osEPiStartDma(&__osFlashHandler, &__osFlashMsg, OS_READ);
    osRecvMesg(&__osFlashMessageQ, NULL, OS_MESG_BLOCK);
    *flash_type = __osFlashID[0];
    *flash_maker = __osFlashID[1];
}

void osFlashClearStatus(void)
{
    osEPiWriteIo(&__osFlashHandler, __osFlashHandler.baseAddress | FLASH_CMD_REG, FLASH_CMD_STATUS);
    osEPiWriteIo(&__osFlashHandler, __osFlashHandler.baseAddress, 0);
}

s32 osFlashWriteBuffer(OSIoMesg* mb, s32 priority, void* dramAddr, OSMesgQueue* mq)
{
    osEPiWriteIo(&__osFlashHandler, __osFlashHandler.baseAddress | FLASH_CMD_REG, FLASH_CMD_PAGE_PROGRAM);
    mb->hdr.pri = priority;
    mb->hdr.retQueue = mq;
    mb->dramAddr = dramAddr;
    mb->devAddr = 0;
    mb->size = FLASH_BLOCK_SIZE;

    return osEPiStartDma(&__osFlashHandler, mb, OS_WRITE);
}

s32 osFlashWriteArray(u32 pageNum)
{
    u32 status;
    OSTimer timer;
    OSMesgQueue mq;
    OSMesg msg;

    /* New flash somehow needs this init */
    if (__osFlashVersion == NEW_FLASH)
        osEPiWriteIo(&__osFlashHandler, __osFlashHandler.baseAddress | FLASH_CMD_REG, FLASH_CMD_PAGE_PROGRAM);

    /* Start the transfer */
    osEPiWriteIo(&__osFlashHandler, __osFlashHandler.baseAddress | FLASH_CMD_REG, FLASH_CMD_PROGRAM_PAGE | pageNum);

    /* Wait for completion */
    osCreateMesgQueue(&mq, &msg, 1);
    for (;;)
    {
        osSetTimer(&timer, OS_USEC_TO_CYCLES(200), 0, &mq, &msg);
        osRecvMesg(&mq, &msg, OS_MESG_BLOCK);
        osEPiReadIo(&__osFlashHandler, __osFlashHandler.baseAddress, &status);

        if (!(status & FLASH_STATUS_WRITE_BUSY))
            break;
    }

    /* Fetch the latched status reg */
    osEPiReadIo(&__osFlashHandler, __osFlashHandler.baseAddress, &status);
    osFlashClearStatus();

    /* Check the status */
    if ((status & 0xff) == 0x04) return FLASH_STATUS_WRITE_OK;
    if ((status & 0xff) == 0x44) return FLASH_STATUS_WRITE_OK;
    if ((status & 0x04) == 0x04) return FLASH_STATUS_WRITE_OK;

    /* Status isn't good */
    return FLASH_STATUS_WRITE_ERROR;
}

s32 osFlashReadArray(OSIoMesg* mb, s32 priority, u32 pageNum, void* dramAddr, u32 pageCount, OSMesgQueue* mq)
{
    u32 dummy;
    u32 last_page;
    u32 pages;

    /* select read array mode */
    osEPiWriteIo(&__osFlashHandler, __osFlashHandler.baseAddress | FLASH_CMD_REG, FLASH_CMD_READ_ARRAY);

    /* dummy read to initiate "fast-page" reads? */
    osEPiReadIo(&__osFlashHandler, __osFlashHandler.baseAddress, &dummy);

    /* DMA requested pages */
    mb->hdr.pri = priority;
    mb->hdr.retQueue = mq;
    mb->dramAddr = dramAddr;

    last_page = pageNum + pageCount - 1;

    if ((last_page & 0xF00) != (pageNum & 0xF00))
    {
        pages = 256 - (pageNum & 0xFF);
        pageCount -= pages;
        mb->size = pages * FLASH_BLOCK_SIZE;
        mb->devAddr = osFlashGetAddr(pageNum);
        osEPiStartDma(&__osFlashHandler, mb, OS_READ);
        osRecvMesg(mq, NULL, OS_MESG_BLOCK);
        pageNum = (pageNum + 256) & 0xF00;
        mb->dramAddr = ((char*)mb->dramAddr + mb->size);
    }

    while (pageCount > 256)
    {
        pages = 256;
        pageCount -= 256;
        mb->size = pages * FLASH_BLOCK_SIZE;
        mb->devAddr = osFlashGetAddr(pageNum);
        osEPiStartDma(&__osFlashHandler, mb, OS_READ);
        osRecvMesg(mq, NULL, OS_MESG_BLOCK);
        pageNum += 256;
        mb->dramAddr = ((char*)mb->dramAddr + mb->size);
    }

    mb->size = pageCount * FLASH_BLOCK_SIZE;
    mb->devAddr = osFlashGetAddr(pageNum);
    return osEPiStartDma(&__osFlashHandler, mb, OS_READ);
}

static s32          sFlashromIsInit;
static OSMesgQueue  sFlashromMesgQueue;
static OSMesg       sFlashromMesg[1];

s32 SysFlashrom_IsInit(void)
{
    return sFlashromIsInit;
}

s32 SysFlashrom_InitFlash(void)
{
    osCreateMesgQueue(&sFlashromMesgQueue, sFlashromMesg, 1);
    osFlashInit();
    sFlashromIsInit = 1;

    return 0;
}

s32 SysFlashrom_ReadData(void* addr, u32 pageNum, u32 pageCount)
{
    OSIoMesg msg;

    if (!SysFlashrom_IsInit()) {
        return -1;
    }
    osInvalDCache(addr, pageCount * FLASH_BLOCK_SIZE);
    osFlashReadArray(&msg, OS_MESG_PRI_NORMAL, pageNum, addr, pageCount, &sFlashromMesgQueue);
    osRecvMesg(&sFlashromMesgQueue, NULL, OS_MESG_BLOCK);
    return 0;
}
#else
extern OSMesgQueue  sFlashromMesgQueue;
#endif

static char sFlashBuffer[FLASH_BLOCK_SIZE] ALIGNED(16);

static s32 doWrite(void* addr, u32 pageNum, u32 pageCount)
{
    OSIoMesg msg;
    s32 result;
    u32 i;

    if (!SysFlashrom_IsInit()) {
        return -1;
    }
    osWritebackDCache(addr, pageCount * FLASH_BLOCK_SIZE);
    for (i = 0; i < pageCount; i++)
    {
        osFlashWriteBuffer(&msg, OS_MESG_PRI_NORMAL, (u8*)addr + i * FLASH_BLOCK_SIZE, &sFlashromMesgQueue);
        osRecvMesg(&sFlashromMesgQueue, NULL, OS_MESG_BLOCK);
        result = osFlashWriteArray(i + pageNum);
        if (result)
            return result;
    }
    return 0;
}

static int tryWrite(void* addr, u32 pageNum, u32 pageCount)
{
    int errors;
    int ret;

    errors = 0;
    for (;;)
    {
        ret = doWrite(addr, pageNum, pageCount);
        if (ret == 0)
            return 0;
        errors++;
        if (errors >= 3)
            return ret;
    }
}

static void writeFlash(u32 devAddr, void* dramAddr, u32 size)
{
    u32 misalign;
    u32 tmp;

    /* Check for misaligned start */
    misalign = devAddr & FLASH_BLOCK_MASK;
    if (misalign)
    {
        /* Read-edit-write cycle */
        tmp = FLASH_BLOCK_SIZE - misalign;
        if (tmp > size)
            tmp = size;
        SysFlashrom_ReadData(sFlashBuffer, devAddr / FLASH_BLOCK_SIZE, 1);
        memcpy(sFlashBuffer + misalign, dramAddr, tmp);
        tryWrite(sFlashBuffer, devAddr / FLASH_BLOCK_SIZE, 1);
        SysFlashrom_ReadData(sFlashBuffer, devAddr / FLASH_BLOCK_SIZE, 1);
        devAddr += tmp;
        dramAddr = ((char*)dramAddr + tmp);
        size -= tmp;
    }

    if (size >= FLASH_BLOCK_SIZE)
    {
        if (!((u32)dramAddr & 7))
        {
            /* DMA-friendly */
            tmp = size / FLASH_BLOCK_SIZE;
            tryWrite(dramAddr, devAddr / FLASH_BLOCK_SIZE, tmp);
            SysFlashrom_ReadData(dramAddr, devAddr / FLASH_BLOCK_SIZE, tmp);
            devAddr += tmp * FLASH_BLOCK_SIZE;
            dramAddr = ((char*)dramAddr + tmp * FLASH_BLOCK_SIZE);
            size -= tmp * FLASH_BLOCK_SIZE;
        }
        else
        {
            /* Non-DMA-friendly */
            while (size >= FLASH_BLOCK_SIZE)
            {
                memcpy(sFlashBuffer, dramAddr, FLASH_BLOCK_SIZE);
                tryWrite(sFlashBuffer, devAddr / FLASH_BLOCK_SIZE, 1);
                SysFlashrom_ReadData(sFlashBuffer, devAddr / FLASH_BLOCK_SIZE, 1);
                devAddr += FLASH_BLOCK_SIZE;
                dramAddr = ((char*)dramAddr + FLASH_BLOCK_SIZE);
                size -= FLASH_BLOCK_SIZE;
            }
        }
    }

    /* Misaligned end */
    if (size)
    {
        SysFlashrom_ReadData(sFlashBuffer, devAddr / FLASH_BLOCK_SIZE, 1);
        memcpy(sFlashBuffer, dramAddr, size);
        tryWrite(sFlashBuffer, devAddr / FLASH_BLOCK_SIZE, 1);
        SysFlashrom_ReadData(sFlashBuffer, devAddr / FLASH_BLOCK_SIZE, 1);
    }
}

static void readFlash(u32 devAddr, void* dramAddr, u32 size)
{
    u32 misalign;
    u32 tmp;

    /* Check for misaligned start */
    misalign = devAddr & FLASH_BLOCK_MASK;
    if (misalign)
    {
        tmp = FLASH_BLOCK_SIZE - misalign;
        if (tmp > size)
            tmp = size;
        SysFlashrom_ReadData(sFlashBuffer, devAddr / FLASH_BLOCK_SIZE, 1);
        memcpy(dramAddr, sFlashBuffer + misalign, tmp);
        devAddr += tmp;
        dramAddr = ((char*)dramAddr + tmp);
        size -= tmp;
    }

    /* Check the middle part */
    if (size >= FLASH_BLOCK_SIZE)
    {
        /* At this point we are aligned relative to the flash */
        if (!((u32)dramAddr & 7))
        {
            /* DMA-friendly */
            tmp = size / FLASH_BLOCK_SIZE;
            SysFlashrom_ReadData(dramAddr, devAddr / FLASH_BLOCK_SIZE, tmp);
            devAddr += tmp * FLASH_BLOCK_SIZE;
            dramAddr = ((char*)dramAddr + tmp * FLASH_BLOCK_SIZE);
            size -= tmp * FLASH_BLOCK_SIZE;
        }
        else
        {
            /* Non-DMA-friendly */
            while (size >= FLASH_BLOCK_SIZE)
            {
                SysFlashrom_ReadData(sFlashBuffer, devAddr / FLASH_BLOCK_SIZE, 1);
                memcpy(dramAddr, sFlashBuffer, FLASH_BLOCK_SIZE);
                devAddr += FLASH_BLOCK_SIZE;
                dramAddr = ((char*)dramAddr + FLASH_BLOCK_SIZE);
                size -= FLASH_BLOCK_SIZE;
            }
        }
    }

    /* Check for misaligned end */
    if (size)
    {
        SysFlashrom_ReadData(sFlashBuffer, devAddr / FLASH_BLOCK_SIZE, 1);
        memcpy(dramAddr, sFlashBuffer, size);
    }
}

void comboReadWriteFlash(u32 devAddr, void* dramAddr, u32 size, s32 direction)
{
    if (!SysFlashrom_IsInit())
        SysFlashrom_InitFlash();

    devAddr &= 0x1ffff;
    if (direction == OS_READ)
        readFlash(devAddr, dramAddr, size);
    else
        writeFlash(devAddr, dramAddr, size);
}
