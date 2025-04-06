#include "global.h"

AudioContext gAudioCtx;
AudioCustomUpdateFunction gAudioCustomUpdateFunction;
s32 D_801755D8[3]; // unused

const TempoData gTempoData = {
    0x1C00,            // unk_00
    SEQTICKS_PER_BEAT, // seqTicksPerBeat
};

// TODO: Extract from table?
#define NUM_SOUNDFONTS 38
#define SFX_SEQ_SIZE 0x6A90
#define SFX_SOUNDFONT_1_SIZE 0x3AA0
#define SFX_SOUNDFONT_2_SIZE 0x17B0

// Sizes of everything on the init pool
#define AI_BUFFERS_SIZE (AIBUF_SIZE * ARRAY_COUNT(gAudioCtx.aiBuffers))
#define SOUNDFONT_LIST_SIZE (NUM_SOUNDFONTS * sizeof(SoundFont))
#if OOT_VERSION < PAL_1_0 || PLATFORM_GC
#define PERMANENT_POOL_SIZE (SFX_SEQ_SIZE + SFX_SOUNDFONT_1_SIZE + SFX_SOUNDFONT_2_SIZE)
#else
#define PERMANENT_POOL_SIZE (SFX_SEQ_SIZE + SFX_SOUNDFONT_1_SIZE + SFX_SOUNDFONT_2_SIZE + 0x10)
#endif

const AudioHeapInitSizes gAudioHeapInitSizes = {
    ALIGN16(sizeof(gAudioHeap) - 0x100),                                  // audio heap size
    ALIGN16(PERMANENT_POOL_SIZE + AI_BUFFERS_SIZE + SOUNDFONT_LIST_SIZE), // init pool size
    ALIGN16(PERMANENT_POOL_SIZE),                                         // permanent pool size
};

#define DEFAULT_REVERB_SETTINGS \
    { 1, 0x30, 0x3000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x3000, 0x0, 0x0 }

ReverbSettings sReverbSettings[][3] = {
    {
        DEFAULT_REVERB_SETTINGS,
        { 1, 0x20, 0x0800, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x0000, 0x0, 0x0 },
    },
    {
        DEFAULT_REVERB_SETTINGS,
        { 1, 0x30, 0x1800, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x0000, 0xB, 0xB },
    },
    {
        DEFAULT_REVERB_SETTINGS,
        { 1, 0x38, 0x2800, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x0000, 0x7, 0x7 },
    },
    {
        DEFAULT_REVERB_SETTINGS,
        { 1, 0x50, 0x5000, 0, 0, 0x7FFF, 0x1000, 0x1000, 0xFF, 0x3000, 0x7, 0x7 },
    },
    {
        DEFAULT_REVERB_SETTINGS,
        { 1, 0x40, 0x5000, 0, 0, 0x7FFF, 0x1800, 0x1800, 0xFF, 0x3000, 0x7, 0x7 },
    },
    {
        DEFAULT_REVERB_SETTINGS,
        { 1, 0x40, 0x5C00, 0, 0, 0x7FFF, 0x2000, 0x2000, 0xFF, 0x3000, 0x4, 0x4 },
    },
    {
        DEFAULT_REVERB_SETTINGS,
        { 1, 0x30, 0x6000, 0, 0, 0x7FFF, 0x1000, 0x1000, 0xFF, 0x3000, 0xA, 0xA },
    },
    {
        DEFAULT_REVERB_SETTINGS,
        { 1, 0x30, 0x6800, 0, 0, 0x7FFF, 0x1400, 0x1400, 0xFF, 0x3000, 0x6, 0x6 },
    },
    {
        DEFAULT_REVERB_SETTINGS,
        { 2, 0x50, 0x5000, 0, 0, 0x7FFF, 0xD000, 0x3000, 0xFF, 0x3000, 0x0, 0x0 },
    },
    {
        DEFAULT_REVERB_SETTINGS,
        { 1, 0x20, 0x0000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x0000, 0x0, 0x0 },
    },
    {
        DEFAULT_REVERB_SETTINGS,
        { 1, 0x30, 0x1800, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x0000, 0xB, 0xB },
    },
    {
        DEFAULT_REVERB_SETTINGS,
    },
    {
        DEFAULT_REVERB_SETTINGS,
        { 1, 0x40, 0x5000, 0, 0, 0x7FFF, 0x0000, 0x0000, 0xFF, 0x3000, 0x0, 0x0 },
    },
};

AudioSpec gAudioSpecs[18] = {
    { 32000, 1, 24, 4, 0, 0, 2, sReverbSettings[0], 0x300, 0x200, 0x7FFF, 0x7F0, 0xE00, 0, 0x4000, 0x2880, 0, 0, 0 },
    { 32000, 1, 24, 4, 0, 0, 2, sReverbSettings[1], 0x300, 0x200, 0x7FFF, 0x7F0, 0xE00, 0, 0x3800, 0x2880, 0, 0, 0 },
    { 32000, 1, 24, 4, 0, 0, 2, sReverbSettings[2], 0x300, 0x200, 0x7FFF, 0x7F0, 0xE00, 0, 0x3800, 0x2880, 0, 0, 0 },
    { 32000, 1, 23, 4, 0, 0, 2, sReverbSettings[4], 0x300, 0x200, 0x7FFF, 0x7F0, 0xE00, 0, 0x3800, 0x2880, 0, 0, 0 },
    { 32000, 1, 23, 4, 0, 0, 2, sReverbSettings[5], 0x300, 0x200, 0x7FFF, 0x7F0, 0xE00, 0, 0x3800, 0x2880, 0, 0, 0 },
    { 32000, 1, 24, 4, 0, 0, 2, sReverbSettings[6], 0x300, 0x200, 0x7FFF, 0x7F0, 0xE00, 0, 0x3800, 0x2880, 0, 0, 0 },
    { 32000, 1, 24, 4, 0, 0, 2, sReverbSettings[7], 0x300, 0x200, 0x7FFF, 0x7F0, 0xE00, 0, 0x3800, 0x2880, 0, 0, 0 },
    { 32000, 1, 23, 4, 0, 0, 2, sReverbSettings[8], 0x300, 0x200, 0x7FFF, 0x7F0, 0xE00, 0, 0x3800, 0x2880, 0, 0, 0 },
    { 32000, 1, 24, 4, 0, 0, 2, sReverbSettings[9], 0x300, 0x200, 0x7FFF, 0x7F0, 0xE00, 0, 0x3800, 0x2880, 0, 0, 0 },
    { 32000, 1, 23, 4, 0, 0, 2, sReverbSettings[8], 0x300, 0x200, 0x7FFF, 0x7F0, 0xE00, 0, 0x3800, 0x2880, 0, 0, 0 },
    { 32000, 1, 28, 3, 0, 0, 2, sReverbSettings[10], 0x300, 0x200, 0x7FFF, 0x7F0, 0xE00, 0, 0x2800, 0x2880, 0, 0, 0 },
    { 32000, 1, 28, 3, 0, 0, 1, sReverbSettings[11], 0x300, 0x200, 0x7FFF, 0, 0x4800, 0, 0x4000, 0, 0, 0, 0 },
    { 32000, 1, 28, 3, 0, 0, 1, sReverbSettings[11], 0x300, 0x200, 0x7FFF, 0, 0, 0, 0x4000, 0x4800, 0, 0, 0 },
    { 32000, 1, 22, 4, 0, 0, 2, sReverbSettings[0], 0x300, 0x200, 0x7FFF, 0x7F0, 0xE00, 0, 0x3800, 0x2880, 0, 0, 0 },
    { 32000, 1, 22, 4, 0, 0, 2, sReverbSettings[8], 0x300, 0x200, 0x7FFF, 0x7F0, 0xE00, 0, 0x3800, 0x2880, 0, 0, 0 },
    { 32000, 1, 16, 4, 0, 0, 2, sReverbSettings[0], 0x300, 0x200, 0x7FFF, 0x7F0, 0xE00, 0, 0x3800, 0x2880, 0, 0, 0 },
    { 22050, 1, 24, 4, 0, 0, 2, sReverbSettings[0], 0x300, 0x200, 0x7FFF, 0x7F0, 0xE00, 0, 0x3800, 0x2880, 0, 0, 0 },
    { 32000, 1, 24, 4, 0, 0, 2, sReverbSettings[2], 0x300, 0x200, 0x7FFF, 0x7F0, 0xE00, 0, 0x3600, 0x2600, 0, 0, 0 },
};
