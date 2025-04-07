#include <combo.h>
#include <combo/file.h>
#include "yaz0.h"
#include "z64dma.h"

#if defined(__OOT__)
# define PRIO_LO 10
# define PRIO_HI 16
# define FILEID_DMADATA FILEID_OOT_DMADATA
#endif

#if defined(__MM__)
# define PRIO_LO 10
# define PRIO_HI 17
# define FILEID_DMADATA FILEID_MM_DMADATA
#endif

extern OSMesgQueue gDmaMgrMsgQueue;

void Dma_ProcessFile(void* dst, int fileIndex, u32 offset, u32 size) {
    u32 fileOffset;
    u32 fileFlags;
    u32 fileSize;

    fileOffset = File_Offset(fileIndex);
    fileFlags = File_Flags(fileIndex);

    if (fileFlags & 1)
    {
        /* Compressed */
        fileSize = File_SizeCompressed(fileIndex);
        osSetThreadPri(NULL, PRIO_LO);
        Yaz0_Decompress(fileOffset, dst, fileSize);
        osSetThreadPri(NULL, PRIO_HI);
    }
    else
    {
        /* Uncompressed */
        DmaMgr_DmaRomToRam(fileOffset + offset, dst, size);
    }
}

void DmaMgr_ProcessRequest(DmaRequest* req) {
    Dma_ProcessFile(req->dst, req->fileIndex, req->offset, req->size);
}

void Dma_PrepareVromRequest(DmaRequest* req, void* dst, uintptr_t vromAddr, size_t size, OSMesgQueue* queue, OSMesg msg)
{
    FileDmaData dma;
    int fileIndex;

    if (!File_DmaData(FILEID_DMADATA, vromAddr, &dma))
        for (;;) {}

    fileIndex = File_IndexFromID(dma.id);
    if (fileIndex == -1)
        for (;;) {}

    req->dst = dst;
    req->fileIndex = fileIndex;
    req->offset = vromAddr - dma.vstart;
    req->size = size;
    req->notifyQueue = queue;
    req->notifyMsg = msg;
}

/**
 * Submit an asynchronous DMA request. Unlike other DMA requests, this will not block the
 * current thread. Data arrival is not immediate however, ensure that the request has completed by awaiting a message
 * sent to `queue` when the DMA operation has completed.
 *
 * @param req DMA request, filled out internally.
 * @param ram Location in DRAM for data to be written.
 * @param vrom Virtual ROM location for data to be read.
 * @param size Transfer size.
 * @param queue Message queue to notify with `msg` once the transfer is complete.
 * @param msg Message to send to `queue` once the transfer is complete.
 * @return -2 if Irq is in NMI reset state, 0 otherwise
 */
s32 DmaMgr_RequestAsync(DmaRequest* req, void* ram, uintptr_t vrom, size_t size, int unused, OSMesgQueue* queue, OSMesg msg) {
    Dma_PrepareVromRequest(req, ram, vrom, size, queue, msg);
    osSendMesg(&gDmaMgrMsgQueue, (OSMesg)req, OS_MESG_BLOCK);
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
    OSMesg msg[1];
    s32 ret;

    osCreateMesgQueue(&queue, msg, 1);

    ret = DmaMgr_RequestAsync(&req, ram, vrom, size, 0, &queue, NULL);
    if (ret == -1) { // DmaMgr_RequestAsync does not return -1
        return ret;
    }

    osRecvMesg(&queue, NULL, OS_MESG_BLOCK);

    return 0;
}
