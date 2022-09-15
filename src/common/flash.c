#include <combo.h>

#define FLASH_BLOCK_SIZE        128
#define FLASH_ADDR_STATUS       0x08000000
#define FLASH_ADDR_COMMAND      (FLASH_ADDR_STATUS | 0x10000)

#define FLASH_CMD_READ          0xf0000000
#define FLASH_CMD_WRITE         0xb4000000
#define FLASH_CMD_ACTIVE(x)     (0xa5000000 | (x))
#define FLASH_CMD_TRANSFER      0xd2000000

static OSMesgQueue  sQueue;
static OSIoMesg     sMb;

static void flashWait(void)
{
    u32 status;

    for (;;)
    {
        osEPiReadIo((void*)&__osPiTable, FLASH_ADDR_STATUS, &status);
        if ((status & 1) == 0)
            break;
    }
}

static void readFlash(u32 devAddr, void* dramAddr, u32 size)
{
    u32 readSize;

    while (size)
    {
        readSize = size;
        if (readSize > 0x2000)
            readSize = 0x2000;

        /* Set the flash to read mode */
        osEPiWriteIo((void*)&__osPiTable, FLASH_ADDR_COMMAND, FLASH_CMD_READ);
        flashWait();

        /* Read */
        sMb.dramAddr = dramAddr;
        sMb.devAddr = ((devAddr & 0xff000000) | ((devAddr & 0x00ffffff) >> 1));
        sMb.size = readSize;
        osEPiStartDma((void*)&__osPiTable, &sMb, OS_READ);
        osRecvMesg(&sQueue, NULL, OS_MESG_BLOCK);
        flashWait();

        dramAddr = (void*)((u32)dramAddr + readSize);
        devAddr += readSize;
        size -= readSize;
    }

    /* Invalidate cache */
    osInvalDCache(dramAddr, size);
}

static void writeFlashBlock(u32 blockId, void* data)
{
    /* Set the flash to write mode */
    osEPiWriteIo((void*)&__osPiTable, FLASH_ADDR_COMMAND, FLASH_CMD_WRITE);
    flashWait();

    /* Set the RDRAM pointer via DMA */
    sMb.dramAddr = data;
    sMb.devAddr = FLASH_ADDR_STATUS;
    sMb.size = FLASH_BLOCK_SIZE;
    osEPiStartDma((void*)&__osPiTable, &sMb, OS_WRITE);
    osRecvMesg(&sQueue, NULL, OS_MESG_BLOCK);
    flashWait();

    /* Set the flash block */
    osEPiWriteIo((void*)&__osPiTable, FLASH_ADDR_COMMAND, FLASH_CMD_ACTIVE(blockId));
    flashWait();

    /* Perform the transfer */
    osEPiWriteIo((void*)&__osPiTable, FLASH_ADDR_COMMAND, FLASH_CMD_TRANSFER);
    flashWait();
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
    writeFlashBlockMisaligned(devAddr, dramAddr, size);
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
}

void comboReadWriteFlash(u32 devAddr, void* dramAddr, u32 size, s32 direction)
{
    OSMesg msg;

    devAddr |= 0x08000000;

    osCreateMesgQueue(&sQueue, &msg, 1);
    sMb.hdr.pri = 0;
    sMb.hdr.retQueue = &sQueue;

    if (direction == OS_READ)
        readFlash(devAddr, dramAddr, size);
    else
        writeFlash(devAddr, dramAddr, size);
}
