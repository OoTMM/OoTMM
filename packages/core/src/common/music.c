#include <combo.h>
#include <combo/dma.h>

#if defined(GAME_OOT)
# define AUDIOSEQ_VROM 0xe0000000
# define AUDIOSEQ_LOAD 0x800b88d8
#else
# define AUDIOSEQ_VROM 0xe8000000
# define AUDIOSEQ_LOAD 0x80190e68
#endif

void AudioLoad_InitTableAudioSeq(void* streamStruct, u32 romAddr, u32 unk)
{
    DmaEntry dmaEntry;

    if (comboDmaLookup(&dmaEntry, AUDIOSEQ_VROM))
        romAddr = dmaEntry.pstart;

    AudioLoad_InitTable(streamStruct, romAddr, unk);
}

PATCH_CALL(AUDIOSEQ_LOAD, AudioLoad_InitTableAudioSeq);
