#include <combo.h>
#include <combo/dma.h>

void AudioLoad_InitTableAudioSeq(void* streamStruct, u32 romAddr, u32 unk)
{
    DmaEntry dmaEntry;

    if (comboDmaLookup(&dmaEntry, 0xe0000000))
        romAddr = dmaEntry.pstart;

    AudioLoad_InitTable(streamStruct, romAddr, unk);
}

PATCH_CALL(0x800b88d8, AudioLoad_InitTableAudioSeq);
