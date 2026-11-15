#include <combo.h>

#define MM_AUDIO_RECLAIM                       0x00039000u
#define MM_AUDIO_HEAP_SIZE                     (*(u32*)0x801E1104u)
#define MM_AUDIO_SPECS_ADDR                    0x801DB958u
#define MM_AUDIO_SPEC_COUNT                    21
#define MM_AUDIO_SPEC_SIZE                     0x38u
#define MM_AUDIO_SPEC_TEMP_SAMPLE_CACHE_OFF    0x34u
#define MM_AUDIO_HEAP_END                      0x803824C0u

#define MM_MALLOC_INIT \
    ((void (*)(void*, size_t))0x80086F28u)

#define MM_SYSTEM_HEAP_RUN_INITS \
    ((void (*)(void))0x80086588u)

static int Audio_ReclaimHeapTail(void)
{
    s32 i;
    u32* temporarySampleCacheSize;

    if (MM_AUDIO_HEAP_SIZE < MM_AUDIO_RECLAIM)
    {
        return 0;
    }

    for (i = 0; i < MM_AUDIO_SPEC_COUNT; i++)
    {
        temporarySampleCacheSize = (u32*)(MM_AUDIO_SPECS_ADDR +
                                          i * MM_AUDIO_SPEC_SIZE +
                                          MM_AUDIO_SPEC_TEMP_SAMPLE_CACHE_OFF);

        if (*temporarySampleCacheSize < MM_AUDIO_RECLAIM)
        {
            return 0;
        }
    }

    MM_AUDIO_HEAP_SIZE -= MM_AUDIO_RECLAIM;

    for (i = 0; i < MM_AUDIO_SPEC_COUNT; i++)
    {
        temporarySampleCacheSize = (u32*)(MM_AUDIO_SPECS_ADDR +
                                          i * MM_AUDIO_SPEC_SIZE +
                                          MM_AUDIO_SPEC_TEMP_SAMPLE_CACHE_OFF);

        *temporarySampleCacheSize -= MM_AUDIO_RECLAIM;
    }

    return 1;
}

static void SystemHeap_InitMemoryReclaim(void* start, size_t size)
{
    uintptr_t oldStart;

    oldStart = (uintptr_t)start;

    if (oldStart != MM_AUDIO_HEAP_END)
    {
        MM_MALLOC_INIT(start, size);
        MM_SYSTEM_HEAP_RUN_INITS();
        return;
    }

    if (!Audio_ReclaimHeapTail())
    {
        MM_MALLOC_INIT(start, size);
        MM_SYSTEM_HEAP_RUN_INITS();
        return;
    }

    MM_MALLOC_INIT(
        (void*)(oldStart - MM_AUDIO_RECLAIM),
        size + MM_AUDIO_RECLAIM);

    MM_SYSTEM_HEAP_RUN_INITS();
}

PATCH_FUNC(0x800865F8, SystemHeap_InitMemoryReclaim);
