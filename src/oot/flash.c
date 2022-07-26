#include <strings.h>
#include <string.h>
#include <oot.h>

#define FLASH_BLOCK_SIZE        128
#define FLASH_ADDR_STATUS       0x08000000
#define FLASH_ADDR_COMMAND      (FLASH_ADDR_STATUS | 0x10000)

#define FLASH_CMD_READ          0xf0000000
#define FLASH_CMD_WRITE         0xb4000000
#define FLASH_CMD_ACTIVE(x)     (0xa5000000 | (x))
#define FLASH_CMD_TRANSFER      0xd2000000

static OSMesgQueue  sQueue;
static OSIoMesg     sMb;

static void readFlash(u32 devAddr, void* dramAddr, u32 size)
{
    OSMesg msg;
    u32 tmp;

    /* Set the flash to read mode */
    osEPiRawWriteIo(&__osPiHandle, FLASH_ADDR_COMMAND, FLASH_CMD_READ);
    osEPiRawReadIo(&__osPiHandle, FLASH_ADDR_STATUS, &tmp);

    /* Read */
    sMb.dramAddr = dramAddr;
    sMb.devAddr = ((devAddr & 0xff000000) | ((devAddr & 0x00ffffff) >> 1));
    sMb.size = size;
    osEPiStartDma(&__osPiHandle, &sMb, OS_READ);
    osRecvMesg(&sQueue, NULL, OS_MESG_BLOCK);
}

static void writeFlashBlock(u32 blockId, void* data)
{
    OSMesg msg;
    u32 tmp;

    /* Set the flash to write mode */
    osEPiRawWriteIo(&__osPiHandle, FLASH_ADDR_COMMAND, FLASH_CMD_WRITE);
    osEPiRawReadIo(&__osPiHandle, FLASH_ADDR_STATUS, &tmp);

    /* Set the flash block */
    osEPiRawWriteIo(&__osPiHandle, FLASH_ADDR_COMMAND, FLASH_CMD_ACTIVE(blockId));
    osEPiRawReadIo(&__osPiHandle, FLASH_ADDR_STATUS, &tmp);

    /* Set the RDRAM pointer via DMA */
    sMb.dramAddr = data;
    sMb.devAddr = FLASH_ADDR_STATUS;
    sMb.size = FLASH_BLOCK_SIZE;
    osEPiStartDma(&__osPiHandle, &sMb, OS_WRITE);
    osRecvMesg(&sQueue, NULL, OS_MESG_BLOCK);

    /* Perform the transfer */
    osEPiRawWriteIo(&__osPiHandle, FLASH_ADDR_COMMAND, FLASH_CMD_TRANSFER);
    osEPiRawReadIo(&__osPiHandle, FLASH_ADDR_STATUS, &tmp);
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
        size = maxSize;

    /* Read the block */
    readFlash(devAddrSeg | blockId * FLASH_BLOCK_SIZE, buffer, FLASH_BLOCK_SIZE);

    /* Copy the data */
    memcpy(buffer + offset, dramAddr, size);

    /* Write the block */
    writeFlashBlock(blockId, buffer);

    return size;
}

static void writeFlash(u32 devAddr, void* dramAddr, u32 size)
{
    s32 ret;
    u32 blockId;

    if (size && (devAddr % FLASH_BLOCK_SIZE) != 0)
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

    osCreateMesgQueue(&sQueue, &msg, 1);
    sMb.hdr.pri = 0;
    sMb.hdr.retQueue = &sQueue;

    if (direction == OS_READ)
        readFlash(devAddr, dramAddr, size);
    else
        writeFlash(devAddr, dramAddr, size);
}

void comboReadWriteFlashHook(u32 devAddr, void* dramAddr, u32 size, s32 direction)
{
    comboReadWriteFlash(devAddr + 0x20000, dramAddr, size, direction);
}
