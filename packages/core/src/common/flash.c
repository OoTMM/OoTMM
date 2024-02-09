#include <combo.h>

#define FLASH_BLOCK_SIZE        128
#define FLASH_CMD_REG           0x10000
#define FLASH_CMD_CHIP_ERASE    0x3C000000
#define FLASH_CMD_SECTOR_ERASE  0x4B000000
#define FLASH_CMD_EXECUTE_ERASE 0x78000000
#define FLASH_CMD_PROGRAM_PAGE  0xA5000000
#define FLASH_CMD_PAGE_PROGRAM  0xB4000000
#define FLASH_CMD_STATUS        0xD2000000
#define FLASH_CMD_ID            0xE1000000
#define FLASH_CMD_READ_ARRAY    0xF0000000

ALIGNED(8) static OSMesgQueue   sFlashQueue;
ALIGNED(8) static OSIoMesg      sFlashIoMesg;
ALIGNED(8) static OSMesg        sFlashMesg[1];
ALIGNED(8) static OSPiHandle    sFlashHandle;
ALIGNED(8) static u32           sFlashId[2];
static int                      sFlashInitialized;
static int                      sFlashVersion;

static void flashWait(u32 mask)
{
    u32 status;

    for (;;)
    {
        osEPiReadIo(&sFlashHandle, sFlashHandle.baseAddress, &status);
        if ((status & mask) != mask)
            break;
    }
    osEPiReadIo(&sFlashHandle, sFlashHandle.baseAddress, &status);
}

static void flashReadStatus(u8* out)
{
    u32 status;

    osEPiWriteIo(&sFlashHandle, sFlashHandle.baseAddress | FLASH_CMD_REG, FLASH_CMD_STATUS);
    osEPiReadIo(&sFlashHandle, sFlashHandle.baseAddress, &status);
    osEPiWriteIo(&sFlashHandle, sFlashHandle.baseAddress | FLASH_CMD_REG, FLASH_CMD_STATUS);
    osEPiReadIo(&sFlashHandle, sFlashHandle.baseAddress, &status);

    *out = status & 0xff;
}

static void flashClearStatus(void)
{
    osEPiWriteIo(&sFlashHandle, sFlashHandle.baseAddress | FLASH_CMD_REG, FLASH_CMD_STATUS);
    osEPiWriteIo(&sFlashHandle, sFlashHandle.baseAddress, 0);
}

static void flashReadId(u32* type, u32* maker)
{
    u8 tmp;

    flashReadStatus(&tmp);

    // select silicon id read mode
    osEPiWriteIo(&sFlashHandle, sFlashHandle.baseAddress | FLASH_CMD_REG, FLASH_CMD_ID);

    // read silicon id using DMA
    sFlashIoMesg.hdr.pri = OS_MESG_PRI_NORMAL;
    sFlashIoMesg.hdr.retQueue = &sFlashQueue;
    sFlashIoMesg.dramAddr = sFlashId;
    sFlashIoMesg.devAddr = 0;
    sFlashIoMesg.size = 2 * sizeof(u32);

    osInvalDCache(sFlashId, sizeof(sFlashId));
    osEPiStartDma(&sFlashHandle, &sFlashIoMesg, OS_READ);
    osRecvMesg(&sFlashQueue, NULL, OS_MESG_BLOCK);

    *type = sFlashId[0];
    *maker = sFlashId[1];
}

static void flashInit(void)
{
    u32 type;
    u32 maker;

    osCreateMesgQueue(&sFlashQueue, sFlashMesg, 1);

    sFlashHandle.type = DEVICE_TYPE_FLASH;
    sFlashHandle.baseAddress = PHYS_TO_K1(FLASH_START_ADDR);
    sFlashHandle.latency = FLASH_LATENCY;
    sFlashHandle.pulse = FLASH_PULSE;
    sFlashHandle.pageSize = FLASH_PAGE_SIZE;
    sFlashHandle.relDuration = FLASH_REL_DURATION;
    sFlashHandle.domain = PI_DOMAIN2;
    sFlashHandle.speed = 0;

    bzero(&sFlashHandle.transferInfo, sizeof(__OSTranxInfo));

    flashReadId(&type, &maker);

    if (maker == FLASH_VERSION_MX_C || maker == FLASH_VERSION_MX_A || maker == FLASH_VERSION_MX_PROTO_A)
        sFlashVersion = OLD_FLASH;
    else
        sFlashVersion = NEW_FLASH;

    sFlashInitialized = 1;
}

static void readFlash(u32 devAddr, void* dramAddr, u32 size)
{
    u32 readSize;

    /* Writeback cache */
    osWritebackDCache(dramAddr, size);

    while (size)
    {
        readSize = 0x2000;
        if (readSize > size)
            readSize = size;

        /* Set the flash to read mode */
        osEPiWriteIo(&sFlashHandle, sFlashHandle.baseAddress | FLASH_CMD_REG, FLASH_CMD_READ_ARRAY);

        /* Read */
        sFlashIoMesg.dramAddr = dramAddr;
        sFlashIoMesg.devAddr = (devAddr & 0x00ffffff) >> (sFlashVersion == OLD_FLASH ? 1 : 0);
        sFlashIoMesg.size = readSize;
        osInvalDCache(dramAddr, readSize);
        osEPiStartDma(&sFlashHandle, &sFlashIoMesg, OS_READ);
        osRecvMesg(&sFlashQueue, NULL, OS_MESG_BLOCK);

        dramAddr = (void*)((u32)dramAddr + readSize);
        devAddr += readSize;
        size -= readSize;
    }
}

static void writeFlashBlock(u32 blockId, void* data)
{
    /* Write into the internal flash buffer */
    osEPiWriteIo(&sFlashHandle, sFlashHandle.baseAddress | FLASH_CMD_REG, FLASH_CMD_PAGE_PROGRAM);
    sFlashIoMesg.dramAddr = data;
    sFlashIoMesg.devAddr = 0;
    sFlashIoMesg.size = FLASH_BLOCK_SIZE;
    osEPiStartDma(&sFlashHandle, &sFlashIoMesg, OS_WRITE);
    osRecvMesg(&sFlashQueue, NULL, OS_MESG_BLOCK);

    /* Commit to flash memory */
    if (sFlashVersion == NEW_FLASH)
        osEPiWriteIo(&sFlashHandle, sFlashHandle.baseAddress | FLASH_CMD_REG, FLASH_CMD_PAGE_PROGRAM);
    osEPiWriteIo(&sFlashHandle, sFlashHandle.baseAddress | FLASH_CMD_REG, FLASH_CMD_PROGRAM_PAGE | blockId);
    flashWait(0x1);
    flashClearStatus();
}

static s32 writeFlashBlockMisaligned(u32 devAddr, void* dramAddr, u32 size)
{
    char buffer[FLASH_BLOCK_SIZE];
    u32 maxSize;
    u32 devAddrSeg;
    u32 devAddrOff;
    u32 blockId;
    u32 offset;

    /* Calculate the segment and offset */
    devAddrSeg = devAddr & 0xff000000;
    devAddrOff = devAddr & 0x00ffffff;

    /* Derive the block and the offset */
    blockId = devAddrOff / FLASH_BLOCK_SIZE;
    offset = devAddrOff & (FLASH_BLOCK_SIZE - 1);

    /* Fixup the size */
    maxSize = FLASH_BLOCK_SIZE - offset;
    if (size > maxSize)
    {
        size = maxSize;
    }

    /* Read the block */
    readFlash(devAddrSeg | blockId * FLASH_BLOCK_SIZE, buffer, FLASH_BLOCK_SIZE);

    /* Copy the data */
    memcpy(buffer + offset, dramAddr, size);

    /* Write the block */
    osWritebackDCache(buffer, FLASH_BLOCK_SIZE);
    writeFlashBlock(blockId, buffer);

    return size;
}

static void writeFlash(u32 devAddr, void* dramAddr, u32 size)
{
    s32 ret;
    u32 blockId;

    osWritebackDCache(dramAddr, size);
    if (size && ((devAddr % FLASH_BLOCK_SIZE) != 0))
    {
        /* Misaligned write */
        ret = writeFlashBlockMisaligned(devAddr, dramAddr, size);
        size -= ret;
        devAddr += ret;
        dramAddr = ((char*)dramAddr + ret);
    }
    while (size >= FLASH_BLOCK_SIZE)
    {
        blockId = (devAddr & 0x00ffffff) / FLASH_BLOCK_SIZE;
        writeFlashBlock(blockId, dramAddr);
        devAddr += FLASH_BLOCK_SIZE;
        dramAddr = ((char*)dramAddr + FLASH_BLOCK_SIZE);
        size -= FLASH_BLOCK_SIZE;
    }
    if (size)
    {
        writeFlashBlockMisaligned(devAddr, dramAddr, size);
    }

    /* Ares seems to require this */
    osEPiWriteIo(&sFlashHandle, sFlashHandle.baseAddress | FLASH_CMD_REG, FLASH_CMD_READ_ARRAY);
}

void comboReadWriteFlash(u32 devAddr, void* dramAddr, u32 size, s32 direction)
{
    if (!sFlashInitialized)
    {
        flashInit();
    }

    if (direction == OS_READ)
        readFlash(devAddr, dramAddr, size);
    else
        writeFlash(devAddr, dramAddr, size);
}
