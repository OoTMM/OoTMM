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
    uintptr_t vrom = req->vromAddr;
    void* ram = req->dramAddr;
    size_t size = req->size;

    FileDmaData dma;
    int fileIndex;

    if (!File_DmaData(FILEID_DMADATA, vrom, &dma))
        for (;;) {}

    fileIndex = File_IndexFromID(dma.id);
    if (fileIndex == -1)
        for (;;) {}
    Dma_ProcessFile(ram, fileIndex, vrom - dma.vstart, size);
}
