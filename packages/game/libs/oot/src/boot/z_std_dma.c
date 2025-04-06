/**
 * @file z_std_dma.c
 *
 * This file implements a system for structuring the ROM image and retrieving data. It is designed to have the same
 * external interface regardless of whether the ROM segments are compressed or not.
 *
 * The ROM image is partitioned into regions that are entered into the DMA data table `gDmaDataTable`. External code
 * does not directly address locations on the ROM image directly, instead a "Virtual ROM" addressing is used. Virtual
 * ROM is defined to be the ROM address of a segment in a totally uncompressed ROM. For uncompressed ROMs, "physical"
 * ROM and VROM addresses coincide. The DMA data table converts VROM to ROM addresses so that code may exclusively use
 * VROM addresses even if the ROM is compressed.
 *
 * External code typically submits requests to the DMA Manager asking for a transfer in terms of Virtual ROM; the DMA
 * Manager translates this to the physical ROM address, performs the transfer to RAM and decompresses the data if
 * required.
 * Requests are processed in the order they are received and may be submitted both synchronously and asynchronously.
 *
 * There are some additional provisions to ensure that audio DMA is particularly high-speed, the audio data is assumed
 * to be uncompressed and the request queue and address translation is skipped.
 */
#include <combo.h>
#include "global.h"
#include "fault.h"
#include "stack.h"
#include "terminal.h"
#if PLATFORM_N64
#include "n64dd.h"
#endif

#pragma increment_block_number "gc-eu:128 gc-eu-mq:128 gc-jp:128 gc-jp-ce:128 gc-jp-mq:128 gc-us:128 gc-us-mq:128" \
                               "ntsc-1.2:88 pal-1.0:86 pal-1.1:86"

StackEntry sDmaMgrStackInfo;
OSMesgQueue sDmaMgrMsgQueue;
OSMesg sDmaMgrMsgBuf[32];

u32 gDmaMgrVerbose = 0;
size_t gDmaMgrDmaBuffSize = DMAMGR_DEFAULT_BUFSIZE;
u32 sDmaMgrIsRomCompressed = false;

OSThread sDmaMgrThread;
STACK(sDmaMgrStack, 0x500);

#if DEBUG_FEATURES

const char* sDmaMgrCurFileName;
s32 sDmaMgrCurFileLine;

// dmadata filenames
#define DEFINE_DMA_ENTRY(_0, nameString) nameString,

const char* sDmaMgrFileNames[] = {
#include "tables/dmadata_table.h"
};

#endif

#undef DEFINE_DMA_ENTRY

#if PLATFORM_N64 || DEBUG_FEATURES
/**
 * Compares `str1` and `str2`.
 *
 * @return
 *  0 if str1 and str2 are the same,
 *  -1 if the first character that does not match has a smaller value in str1 than str2,
 *  +1 if the first character that does not match has a greater value in str1 than str2
 */
s32 DmaMgr_StrCmp(const char* str1, const char* str2) {
    while (*str1 != '\0') {
        if (*str1 > *str2) {
            return 1;
        }
        if (*str1 < *str2) {
            return -1;
        }
        str1++;
        str2++;
    }

    if (*str2 > '\0') {
        return -1;
    }
    return 0;
}
#endif

/**
 * Transfer `size` bytes from physical ROM address `rom` to `ram`.
 *
 * This function is intended for internal use only, however it is possible to use this function externally in which
 * case it behaves as a synchronous transfer, data is available as soon as this function returns.
 *
 * Transfers are divided into chunks based on the current value of `gDmaMgrDmaBuffSize` to avoid congestion of the PI
 * so that higher priority transfers can still be carried out in a timely manner. The transfers are sent in a queue to
 * the OS PI Manager which performs the transfer.
 *
 * @return 0 if successful, -1 if the DMA could not be queued with the PI Manager.
 */
s32 DmaMgr_DmaRomToRam(uintptr_t rom, void* ram, size_t size) {
    OSIoMesg ioMsg;
    OSMesgQueue queue;
    OSMesg msg;
    s32 ret;
    size_t buffSize = gDmaMgrDmaBuffSize;

    if (buffSize == 0) {
        buffSize = DMAMGR_DEFAULT_BUFSIZE;
    }

    osInvalICache(ram, size);
    osInvalDCache(ram, size);
    osCreateMesgQueue(&queue, &msg, 1);

    while (size > buffSize) {
        // The system avoids large DMAs as these would stall the PI for too long, potentially causing issues with
        // audio. To allow audio to continue to DMA whenever it needs to, other DMAs are split into manageable chunks.

        if (1) {} // Necessary to match

        ioMsg.hdr.pri = OS_MESG_PRI_NORMAL;
        ioMsg.hdr.retQueue = &queue;
        ioMsg.devAddr = rom;
        ioMsg.dramAddr = ram;
        ioMsg.size = buffSize;

        if (gDmaMgrVerbose == 10) {
            PRINTF(T("%10lld ノーマルＤＭＡ %08x %08x %08x (%d)\n", "%10lld Normal DMA %08x %08x %08x (%d)\n"),
                   OS_CYCLES_TO_USEC(osGetTime()), ioMsg.dramAddr, ioMsg.devAddr, ioMsg.size,
                   MQ_GET_COUNT(&gPiMgrCmdQueue));
        }

        ret = osEPiStartDma(gCartHandle, &ioMsg, OS_READ);
        if (ret != 0) {
            goto end;
        }

        if (gDmaMgrVerbose == 10) {
            PRINTF(T("%10lld ノーマルＤＭＡ START (%d)\n", "%10lld Normal DMA START (%d)\n"),
                   OS_CYCLES_TO_USEC(osGetTime()), MQ_GET_COUNT(&gPiMgrCmdQueue));
        }

        osRecvMesg(&queue, NULL, OS_MESG_BLOCK);
        if (gDmaMgrVerbose == 10) {
            PRINTF(T("%10lld ノーマルＤＭＡ END (%d)\n", "%10lld Normal DMA END (%d)\n"),
                   OS_CYCLES_TO_USEC(osGetTime()), MQ_GET_COUNT(&gPiMgrCmdQueue));
        }

        size -= buffSize;
        rom += buffSize;
        ram = (u8*)ram + buffSize;
    }

    if (1) { // Also necessary to match
        s32 pad[2];
    }

    ioMsg.hdr.pri = OS_MESG_PRI_NORMAL;
    ioMsg.hdr.retQueue = &queue;
    ioMsg.devAddr = rom;
    ioMsg.dramAddr = ram;
    ioMsg.size = size;

    if (gDmaMgrVerbose == 10) {
        PRINTF(T("%10lld ノーマルＤＭＡ %08x %08x %08x (%d)\n", "%10lld Normal DMA %08x %08x %08x (%d)\n"),
               OS_CYCLES_TO_USEC(osGetTime()), ioMsg.dramAddr, ioMsg.devAddr, ioMsg.size,
               MQ_GET_COUNT(&gPiMgrCmdQueue));
    }

    ret = osEPiStartDma(gCartHandle, &ioMsg, OS_READ);
    if (ret != 0) {
        goto end;
    }

    osRecvMesg(&queue, NULL, OS_MESG_BLOCK);
    if (gDmaMgrVerbose == 10) {
        PRINTF(T("%10lld ノーマルＤＭＡ END (%d)\n", "%10lld Normal DMA END (%d)\n"), OS_CYCLES_TO_USEC(osGetTime()),
               MQ_GET_COUNT(&gPiMgrCmdQueue));
    }

end:
    osInvalICache(ram, size);
    osInvalDCache(ram, size);

    return ret;
}

/**
 * Callback function to facilitate audio DMA. Audio DMA does not use the request queue as audio data is often needed
 * very soon after the request is sent, requiring a higher priority method for enqueueing a DMA on the OS PI command
 * queue.
 *
 * @param pihandle Cartridge ROM PI Handle.
 * @param mb IO Message describing the transfer.
 * @param direction Read or write. (Only read is allowed)
 * @return 0 if the IO Message was successfully put on the OS PI command queue, < 0 otherwise
 */
s32 DmaMgr_AudioDmaHandler(OSPiHandle* pihandle, OSIoMesg* mb, s32 direction) {
    s32 ret;

    ASSERT(pihandle == gCartHandle, "pihandle == carthandle", "../z_std_dma.c", 530);
    ASSERT(direction == OS_READ, "direction == OS_READ", "../z_std_dma.c", 531);
    ASSERT(mb != NULL, "mb != NULL", "../z_std_dma.c", 532);

#if PLATFORM_N64
    if (D_80121212) {
        while (D_80121214) {
            Sleep_Msec(1000);
        }
    }
#endif

    if (gDmaMgrVerbose == 10) {
        PRINTF(T("%10lld サウンドＤＭＡ %08x %08x %08x (%d)\n", "%10lld Sound DMA %08x %08x %08x (%d)\n"),
               OS_CYCLES_TO_USEC(osGetTime()), mb->dramAddr, mb->devAddr, mb->size, MQ_GET_COUNT(&gPiMgrCmdQueue));
    }

    ret = osEPiStartDma(pihandle, mb, direction);
    if (ret != 0) {
        PRINTF("OOPS!!\n");
    }
    return ret;
}

/**
 * DMA read from disk drive. Blocks the current thread until DMA completes.
 *
 * @param ram RAM address to write data to.
 * @param rom ROM address to read from.
 * @param size Size of transfer.
 */
void DmaMgr_DmaFromDriveRom(void* ram, uintptr_t rom, size_t size) {
#if PLATFORM_N64
    s32 pad;
#endif
    OSPiHandle* handle = osDriveRomInit();
    OSMesgQueue queue;
    OSMesg msg;
    OSIoMesg ioMsg;

    osInvalICache(ram, size);
    osInvalDCache(ram, size);
    osCreateMesgQueue(&queue, &msg, 1);

    ioMsg.hdr.retQueue = &queue;
    ioMsg.hdr.pri = OS_MESG_PRI_NORMAL;
    ioMsg.devAddr = rom;
    ioMsg.dramAddr = ram;
    ioMsg.size = size;
    handle->transferInfo.cmdType = 2;

    osEPiStartDma(handle, &ioMsg, OS_READ);
    osRecvMesg(&queue, NULL, OS_MESG_BLOCK);
}

#if DEBUG_FEATURES
/**
 * DMA error encountered, print error messages and bring up the crash screen.
 *
 * @param req DMA Request causing the error.
 * @param filename DMA data filename associated with the operation that errored.
 * @param errorName Error name string.
 * @param errorDesc Error description string.
 *
 * This function does not return.
 */
NORETURN void DmaMgr_Error(DmaRequest* req, const char* filename, const char* errorName, const char* errorDesc) {
    uintptr_t vrom = req->vromAddr;
    void* ram = req->dramAddr;
    size_t size = req->size;
    char buff1[80];
    char buff2[80];

    PRINTF("%c", BEL);
    PRINTF(VT_FGCOL(RED));
    PRINTF(T("DMA致命的エラー(%s)\nROM:%X RAM:%X SIZE:%X %s\n", "DMA Fatal Error (%s)\nROM:%X RAM:%X SIZE:%X %s\n"),
           errorDesc != NULL ? errorDesc : (errorName != NULL ? errorName : "???"), vrom, ram, size,
           filename != NULL ? filename : "???");

    if (req->filename != NULL) { // Source file name that issued the DMA request
        PRINTF("DMA ERROR: %s %d", req->filename, req->line);
    } else if (sDmaMgrCurFileName != NULL) {
        PRINTF("DMA ERROR: %s %d", sDmaMgrCurFileName, sDmaMgrCurFileLine);
    }

    PRINTF(VT_RST);

    if (req->filename != NULL) {
        sprintf(buff1, "DMA ERROR: %s %d", req->filename, req->line);
    } else if (sDmaMgrCurFileName != NULL) {
        sprintf(buff1, "DMA ERROR: %s %d", sDmaMgrCurFileName, sDmaMgrCurFileLine);
    } else {
        sprintf(buff1, "DMA ERROR: %s", errorName != NULL ? errorName : "???");
    }

    sprintf(buff2, "%07X %08X %X %s", vrom, ram, size, filename != NULL ? filename : "???");
    Fault_AddHungupAndCrashImpl(buff1, buff2);
}

#define DMA_ERROR(req, filename, errorName, errorDesc, file, line1, line2, line3) \
    DmaMgr_Error(req, filename, errorName, errorDesc)
#elif OOT_VERSION < NTSC_1_1
#define DMA_ERROR(req, filename, errorName, errorDesc, file, line1, line2, line3) Fault_AddHungupAndCrash(file, line1)
#elif OOT_VERSION < GC_JP
#define DMA_ERROR(req, filename, errorName, errorDesc, file, line1, line2, line3) Fault_AddHungupAndCrash(file, line2)
#else
#define DMA_ERROR(req, filename, errorName, errorDesc, file, line1, line2, line3) Fault_AddHungupAndCrash(file, line3)
#endif

const char* DmaMgr_GetFileName(uintptr_t vrom) {
#if PLATFORM_GC && DEBUG_FEATURES
    const char* ret = DmaMgr_FindFileName(vrom);

    if (ret == NULL) {
        return "(unknown)";
    }

    if (DmaMgr_StrCmp(ret, "kanji") == 0 || DmaMgr_StrCmp(ret, "link_animetion") == 0) {
        // This check may be related to these files being too large to be loaded all at once, however a NULL filename
        // does not prevent them from being loaded.
        return NULL;
    }
    return ret;
#elif PLATFORM_GC
    return "";
#elif PLATFORM_N64
    return "??";
#endif
}

void DmaMgr_ProcessRequest(DmaRequest* req) {
    uintptr_t vrom = req->vromAddr;
    void* ram = req->dramAddr;
    size_t size = req->size;
    FileDmaData dma;
    int fileIndex;
    u32 fileOffset;

    if (!File_DmaData(FILEID_OOT_DMADATA, vrom, &dma))
        for (;;) {}

    fileIndex = File_IndexFromID(dma.id);
    if (fileIndex == -1)
        for (;;) {}
    fileOffset = File_Offset(fileIndex);
    fileOffset += req->vromAddr - dma.vstart;

    DmaMgr_DmaRomToRam(fileOffset, ram, size);
}

void DmaMgr_ThreadEntry(void* arg) {
    OSMesg msg;
    DmaRequest* req;

    PRINTF(T("ＤＭＡマネージャスレッド実行開始\n", "DMA manager thread execution start\n"));

    while (true) {
        // Wait for DMA Requests to arrive from other threads
        osRecvMesg(&sDmaMgrMsgQueue, &msg, OS_MESG_BLOCK);
        req = (DmaRequest*)msg;
        if (req == NULL) {
            break;
        }

        if (0) {
            PRINTF(T("ＤＭＡ登録受付", "DMA registration acceptance") " dmap=%08x\n", req);
        }

        // Process the DMA request
        DmaMgr_ProcessRequest(req);

        // Notify the sender that the request has been processed
        if (req->notifyQueue != NULL) {
            osSendMesg(req->notifyQueue, req->notifyMsg, OS_MESG_NOBLOCK);
            if (0) {
                PRINTF("osSendMesg: dmap=%08x, mq=%08x, m=%08x \n", req, req->notifyQueue, req->notifyMsg);
            }
        }
    }

    PRINTF(T("ＤＭＡマネージャスレッド実行終了\n", "DMA manager thread execution end\n"));
}

/**
 * Submit an asynchronous DMA request. Unlike other DMA requests, this will not block the current thread. Data arrival
 * is not immediate however, ensure that the request has completed by awaiting a message sent to `queue` when the DMA
 * operation has completed.
 *
 * @param req DMA request structure, filled out internally.
 * @param ram Location in DRAM for data to be written.
 * @param vrom Virtual ROM location for data to be read.
 * @param size Transfer size.
 * @param queue Message queue to notify with `msg` once the transfer is complete.
 * @param msg Message to send to `queue` once the transfer is complete.
 * @return 0
 */
s32 DmaMgr_RequestAsync(DmaRequest* req, void* ram, uintptr_t vrom, size_t size, u32 unk, OSMesgQueue* queue,
                        OSMesg msg) {
    static s32 sDmaMgrQueueFullLogged = 0;

    req->vromAddr = vrom;
    req->dramAddr = ram;
    req->size = size;
    req->unk_14 = 0;
    req->notifyQueue = queue;
    req->notifyMsg = msg;

    osSendMesg(&sDmaMgrMsgQueue, (OSMesg)req, OS_MESG_BLOCK);
    return 0;
}

/**
 * Submit a synchronous DMA request. This will block the current thread until the requested transfer is complete. Data
 * is immediately available as soon as this function returns.
 *
 * @param ram Location in DRAM for data to be written.
 * @param vrom Virtual ROM location for data to be read.
 * @param size Transfer size.
 * @return 0
 */
s32 DmaMgr_RequestSync(void* ram, uintptr_t vrom, size_t size) {
    DmaRequest req;
    OSMesgQueue queue;
    OSMesg msg;
    s32 ret;

    osCreateMesgQueue(&queue, &msg, 1);
    ret = DmaMgr_RequestAsync(&req, ram, vrom, size, 0, &queue, NULL);
    if (ret == -1) { // DmaMgr_RequestAsync only returns 0
        return ret;
    }

    osRecvMesg(&queue, NULL, OS_MESG_BLOCK);
    return 0;
}

void DmaMgr_Init(void) {
    const char** name;
    s32 idx;
    DmaEntry* iter;

    // Start the DMA manager
    osCreateMesgQueue(&sDmaMgrMsgQueue, sDmaMgrMsgBuf, ARRAY_COUNT(sDmaMgrMsgBuf));
    StackCheck_Init(&sDmaMgrStackInfo, sDmaMgrStack, STACK_TOP(sDmaMgrStack), 0, 0x100, "dmamgr");
    osCreateThread(&sDmaMgrThread, THREAD_ID_DMAMGR, DmaMgr_ThreadEntry, NULL, STACK_TOP(sDmaMgrStack),
                   THREAD_PRI_DMAMGR);
    osStartThread(&sDmaMgrThread);
}

#if DEBUG_FEATURES
/**
 * Asynchronous DMA Request with source file and line info for debugging.
 *
 * @see DmaMgr_RequestAsync
 */
s32 DmaMgr_RequestAsyncDebug(DmaRequest* req, void* ram, uintptr_t vrom, size_t size, u32 unk5, OSMesgQueue* queue,
                             OSMesg msg, const char* file, int line) {
    req->filename = file;
    req->line = line;
    return DmaMgr_RequestAsync(req, ram, vrom, size, unk5, queue, msg);
}

/**
 * Synchronous DMA Request with source file and line info for debugging.
 *
 * @see DmaMgr_RequestSync
 */
s32 DmaMgr_RequestSyncDebug(void* ram, uintptr_t vrom, size_t size, const char* file, int line) {
    DmaRequest req;
    s32 ret;
    OSMesgQueue queue;
    OSMesg msg;
    s32 pad;

    req.filename = file;
    req.line = line;
    osCreateMesgQueue(&queue, &msg, 1);
    ret = DmaMgr_RequestAsync(&req, ram, vrom, size, 0, &queue, NULL);
    if (ret == -1) { // DmaMgr_RequestAsync only returns 0
        return ret;
    }

    osRecvMesg(&queue, NULL, OS_MESG_BLOCK);
    return 0;
}
#endif
