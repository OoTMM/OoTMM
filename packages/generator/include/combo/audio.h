#ifndef COMBO_AUDIO_H
#define COMBO_AUDIO_H

#include <combo/types.h>
#include <combo/math/vec.h>

typedef struct PlayState PlayState;

void    Audio_SetFileSelectSettings(u8 audioSetting);
void    Audio_SetCutsceneFlag(int flag);
u16     Audio_GetActiveSeqId(int channel);
void    Audio_DisplayMusicName(PlayState* play);
void    Audio_PlaySequenceWithSeqPlayerIO(u8 seqPlayerIndex, u16 seqId, u8 fadeInDuration, s8 ioPort, s8 ioData);
void    Audio_PlaySubBgm(u16 seqId);
void    Music_Update(PlayState* play);

#if defined(GAME_OOT)
void Audio_PlaySfxTransposed(Vec3f* pos, u16 sfxId, s8 semitone);
void func_800F6964(u16);
void func_800F4C58(Vec3f* pos, u16 sfxId, u8);
void func_800F4E30(Vec3f* pos, f32);
void Audio_SetGanonsTowerBgmVolumeLevel(u8 ganonsTowerLevel);
void Audio_PlayNatureAmbienceSequence(u8 natureAmbienceId);
void Audio_ClearSariaBgm(void);
void Audio_ClearSariaBgm2(void);
void Audio_ClearSariaBgmAtPos(Vec3f* pos);
void Audio_PlaySariaBgm(Vec3f* pos, u16 seqId, u16 distMax);
void Audio_PlaySfx_AtPosWithReverb(Vec3f* pos, u16 sfxId, s8 reverbAdd);
void Audio_PlaySfxRiver(Vec3f* pos, f32 freqScale);
void Audio_LowerMainBgmVolume(u8 volume);
void Audio_UpdateMalonSinging(f32 dist, u16 seqId);
void Audio_ToggleMalonSinging(u8 malonSingingDisabled);
void Audio_SetMainBgmTempoFreqAfterFanfare(float scaleTempoAndFreq, u8 duration);
void Audio_PlaySfx_AtPosWithChannelIO(Vec3f* pos, u16 sfxId, u8 ioData);
extern u8* gFrogsSongPtr;
#define AudioSfx_SetChannelIO func_800F4C58
#endif

struct EnvelopePoint;

typedef struct AdpcmLoopHeader {
    u32 start;
    u32 loopEnd; /* s16 sample position where the loop ends */
    u32 count; /* The number of times the loop is played before the sound completes. Setting count to -1 indicates that the loop should play indefinitely. */
    u32 sampleEnd; /* total number of s16-samples in the sample audio clip */
} AdpcmLoopHeader;

typedef struct AdpcmLoop {
    AdpcmLoopHeader header;
    s16 predictorState[16]; /* only exists if count != 0. 8-byte aligned */
} AdpcmLoop;

typedef struct AdpcmBookHeader {
    s32 order;
    s32 numPredictors;
} AdpcmBookHeader;

/**
 * A table of prediction coefficients that the coder selects from to optimize sound quality.
 *
 * The procedure used to design the codeBook is based on an adaptive clustering algorithm.
 * The size of the codeBook is (8 * order * numPredictors) and is 8-byte aligned
 */
typedef struct AdpcmBook {
    AdpcmBookHeader header;
    s16 codeBook[1];
} AdpcmBook; /* size >= 0x8 */

typedef enum SampleCodec {
    /* 0 */ CODEC_ADPCM, /* 16 2-byte samples (32 bytes) compressed into 4-bit samples (8 bytes) + 1 header byte */
    /* 1 */ CODEC_S8, /* 16 2-byte samples (32 bytes) compressed into 8-bit samples (16 bytes) */
    /* 2 */ CODEC_S16_INMEMORY,
    /* 3 */ CODEC_SMALL_ADPCM, /* 16 2-byte samples (32 bytes) compressed into 2-bit samples (4 bytes) + 1 header byte */
    /* 4 */ CODEC_REVERB,
    /* 5 */ CODEC_S16,
    /* 6 */ CODEC_UNK6,
    /* 7 */ CODEC_UNK7 /* processed as uncompressed samples */
} SampleCodec;

typedef enum SampleMedium {
    /* 0 */ MEDIUM_RAM,
    /* 1 */ MEDIUM_UNK,
    /* 2 */ MEDIUM_CART,
    /* 3 */ MEDIUM_DISK_DRIVE,
    /* 5 */ MEDIUM_RAM_UNLOADED = 5
} SampleMedium;

typedef struct Sample {
    u32 unk_0 : 1;
    u32 codec : 3; /* The state of compression or decompression, See `SampleCodec` */
    u32 medium : 2; /* Medium where sample is currently stored. See `SampleMedium` */
    u32 unk_bit26 : 1;
    u32 isRelocated : 1; /* Has the sample header been relocated (offsets to pointers) */
    u32 size : 24; /* Size of the sample */
    u8* sampleAddr; /* Raw sample data. Offset from the start of the sample bank or absolute address to either rom or ram */
    AdpcmLoop* loop; /* Adpcm loop parameters used by the sample. Offset from the start of the sound font / pointer to ram */
    AdpcmBook* book; /* Adpcm book parameters used by the sample. Offset from the start of the sound font / pointer to ram */
} Sample;

typedef struct TunedSample {
    Sample* sample;
    f32 tuning; /* frequency scale factor */
} TunedSample;

/**
 * Stores an entry of decompressed samples in a reverb ring buffer.
 * By storing the sample in a ring buffer, the time it takes to loop
 * around back to the same sample acts as a delay, leading to an echo effect.
 */
typedef struct Instrument {
    u8 isRelocated; /* have the envelope and all samples been relocated (offsets to pointers) */
    u8 normalRangeLo;
    u8 normalRangeHi;
    u8 adsrDecayIndex; /* index used to obtain adsr decay rate from adsrDecayTable */
    struct EnvelopePoint* envelope;
    TunedSample lowPitchTunedSample;
    TunedSample normalPitchTunedSample;
    TunedSample highPitchTunedSample;
} Instrument;

typedef struct Drum {
    u8 adsrDecayIndex; /* index used to obtain adsr decay rate from adsrDecayTable */
    u8 pan;
    u8 isRelocated; /* have tunedSample.sample and envelope been relocated (offsets to pointers) */
    TunedSample tunedSample;
    struct EnvelopePoint* envelope;
} Drum;

typedef struct SoundEffect {
    TunedSample tunedSample;
} SoundEffect;

typedef struct SoundFont {
    u8 numInstruments;
    u8 numDrums;
    u8 sampleBankId1;
    u8 sampleBankId2;
    u16 numSfx;
    Instrument** instruments;
    Drum** drums;
    SoundEffect* soundEffects;
} SoundFont;

#define REVERB_INDEX_NONE -1

typedef enum ReverbDataType {
    /* 0 */ REVERB_DATA_TYPE_SETTINGS, /* Reverb Settings (Init) */
    /* 1 */ REVERB_DATA_TYPE_DELAY, /* Reverb Delay (numSamples) */
    /* 2 */ REVERB_DATA_TYPE_DECAY, /* Reverb Decay Ratio */
    /* 3 */ REVERB_DATA_TYPE_SUB_VOLUME, /* Reverb Sub-Volume */
    /* 4 */ REVERB_DATA_TYPE_VOLUME, /* Reverb Volume */
    /* 5 */ REVERB_DATA_TYPE_LEAK_RIGHT, /* Reverb Leak Right Channel */
    /* 6 */ REVERB_DATA_TYPE_LEAK_LEFT, /* Reverb Leak Left Channel */
    /* 7 */ REVERB_DATA_TYPE_FILTER_LEFT, /* Reverb Left Filter */
    /* 8 */ REVERB_DATA_TYPE_FILTER_RIGHT, /* Reverb Right Filter */
    /* 9 */ REVERB_DATA_TYPE_9 /* Reverb Unk */
} ReverbDataType;

typedef struct ReverbSettings {
    u8 downsampleRate;
    u16 delayNumSamples;
    u16 decayRatio; /* determines how fast reverb dissipates */
    u16 subDelay;
    u16 subVolume;
    u16 volume;
    u16 leakRtl;
    u16 leakLtr;
    s8 mixReverbIndex;
    u16 mixReverbStrength;
    s16 lowPassFilterCutoffLeft;
    s16 lowPassFilterCutoffRight;
} ReverbSettings;

/**
 * Stores an entry of decompressed samples in a reverb ring buffer.
 * By storing the sample in a ring buffer, the time it takes to loop
 * around back to the same sample acts as a delay, leading to an echo effect.
 */
typedef struct ReverbBufferEntry {
    s16 numSamplesAfterDownsampling; /* never read */
    s16 numSamples; /* never read */
    s16* toDownsampleLeft;
    s16* toDownsampleRight; /* data pointed to by left and right are adjacent in memory */
    s32 startPos; /* start pos in ring buffer */
    s16 size; /* first length in ring buffer (from startPos, at most until end) */
    s16 wrappedSize; /* second length in ring buffer (from pos 0) */
    u16 loadResamplePitch;
    u16 saveResamplePitch;
    u16 saveResampleNumSamples;
} ReverbBufferEntry;

typedef struct SynthesisReverb {
    u8 resampleFlags;
    u8 useReverb;
    u8 framesToIgnore;
    u8 curFrame;
    u8 downsampleRate;
    s8 mixReverbIndex; /* mix in reverb from this index. set to 0xFF to not mix any */
    u16 delayNumSamples; /* number of samples between echos */
    s16 mixReverbStrength; /* the gain/amount to mix in reverb from mixReverbIndex */
    s16 volume;
    u16 decayRatio; /* determines how fast reverb dissipate */
    u16 downsamplePitch;
    s16 leakRtl;
    s16 leakLtr;
    u16 subDelay; /* number of samples between sub echos */
    s16 subVolume; /* strength of the sub echos */
    u8 resampleEffectOn;
    s8 resampleEffectExtraSamples;
    u16 resampleEffectLoadUnk;
    u16 resampleEffectSaveUnk;
    u8 delayNumSamplesAfterDownsampling;
    s32 nextReverbBufPos;
    s32 delayNumSamplesUnk; /* May be bufSizePerChan */
    s16* leftReverbBuf;
    s16* rightReverbBuf;
    s16* leftLoadResampleBuf;
    s16* rightLoadResampleBuf;
    s16* leftSaveResampleBuf;
    s16* rightSaveResampleBuf;
    ReverbBufferEntry bufEntry[2][5];
    ReverbBufferEntry subBufEntry[2][5];
    s16* filterLeft;
    s16* filterRight;
#if defined (GAME_MM)
    s16* filterLeftInit;
    s16* filterRightInit;
#endif
    s16* filterLeftState;
    s16* filterRightState;
    TunedSample tunedSample;
    Sample sample;
    AdpcmLoop loop;
} SynthesisReverb;

extern ReverbSettings* gReverbSettingsTable[10];

/**
 * The high-level audio specifications requested when initializing or resetting the audio pool.
 * Most often resets during scene transitions, but will highly depend on game play.
 */
typedef struct {
    u32 samplingFreq; /* Target sampling rate in Hz */
    u8 unk_04;
    u8 numNotes;
    u8 numSequencePlayers;
    u8 unk_07; /* unused, set to zero */
    u8 unk_08; /* unused, set to zero */
    u8 numReverbs;
    ReverbSettings* reverbSettings;
    u16 sampleDmaBufSize1;
    u16 sampleDmaBufSize2;
    u16 unk_14;
    size_t persistentSeqCacheSize;        /* size of cache on audio pool to store sequences persistently */
    size_t persistentFontCacheSize;       /* size of cache on audio pool to store soundFonts persistently */
    size_t persistentSampleBankCacheSize; /* size of cache on audio pool to store entire sample banks persistently */
    size_t temporarySeqCacheSize;         /* size of cache on audio pool to store sequences temporarily */
    size_t temporaryFontCacheSize;        /* size of cache on audio pool to store soundFonts temporarily */
    size_t temporarySampleBankCacheSize;  /* size of cache on audio pool to store entire sample banks temporarily */
    size_t persistentSampleCacheSize;     /* size of cache on audio pool to store individual samples persistently */
    size_t temporarySampleCacheSize;      /* size of cache on audio pool to store individual samples temporarily */
} AudioSpec;

/**
 * The audio buffer stores the fully processed digital audio before it is sent to the audio interface (AI), then to the
 * digital-analog converter (DAC), then to play on the speakers. The audio buffer is written to by the rsp after
 * processing audio commands, and the audio buffer is read by AI which sends the data to the DAC.
 * This struct parameterizes that buffer.
 */
typedef struct {
    s16 specUnk4;
    u16 samplingFreq; /* Target sampling rate in Hz */
    u16 aiSamplingFreq; /* True sampling rate set to the audio interface (AI) for the audio digital-analog converter (DAC) */
    s16 numSamplesPerFrameTarget;
    s16 numSamplesPerFrameMax;
    s16 numSamplesPerFrameMin;
    s16 updatesPerFrame; /* for each frame of the audio thread (default 60 fps), number of updates to process audio */
    s16 numSamplesPerUpdate;
    s16 numSamplesPerUpdateMax;
    s16 numSamplesPerUpdateMin;
    s16 numSequencePlayers;
    f32 resampleRate;
    f32 updatesPerFrameInv; /* inverse (reciprocal) of updatesPerFrame */
    f32 updatesPerFrameInvScaled; /* updatesPerFrameInv scaled down by a factor of 256 */
    f32 updatesPerFrameScaled; /* updatesPerFrame scaled down by a factor of 4 */
} AudioBufferParameters;

typedef struct {
    union {
    u32 opArgs;
        struct {
            u8 op;
            u8 arg0;
            u8 arg1;
            u8 arg2;
        };
    };
    union {
        void* data;
        f32 asFloat;
        s32 asInt;
        u16 asUShort;
        s8 asSbyte;
        u8 asUbyte;
        u32 asUInt;
        void* asPtr;
    };
} AudioCmd;

typedef struct {
    OSTask task;
    OSMesgQueue* taskQueue;
    void* unk_44; /* probably a message that gets unused. */
    char unk_48[0x8];
} AudioTask;

typedef s32 (*DmaHandler)(OSPiHandle* handle, OSIoMesg* mb, s32 direction);

typedef enum SampleBankTableType {
    /* 0 */ SEQUENCE_TABLE,
    /* 1 */ FONT_TABLE,
    /* 2 */ SAMPLE_TABLE
} SampleBankTableType;

typedef struct AudioTableHeader {
    s16 numEntries;
    s16 unkMediumParam;
    uintptr_t romAddr;
    char pad[0x8];
} AudioTableHeader;

typedef struct AudioTableEntry {
    uintptr_t romAddr;
    size_t size;
    s8 medium;
    s8 cachePolicy;
    u8 data[6];
} AudioTableEntry;

typedef struct AudioTable {
    AudioTableHeader header;
    AudioTableEntry entries[1]; /* (dynamic size) */
} AudioTable; /* size >= 0x20 */

typedef enum AudioLoadStatus {
    /* 0 */ LOAD_STATUS_NOT_LOADED,
    /* 1 */ LOAD_STATUS_IN_PROGRESS,
    /* 2 */ LOAD_STATUS_COMPLETE,
    /* 3 */ LOAD_STATUS_DISCARDABLE,
    /* 4 */ LOAD_STATUS_MAYBE_DISCARDABLE,
    /* 5 */ LOAD_STATUS_PERMANENT
} AudioLoadStatus;

typedef enum AudioCacheType {
    /* 0 */ CACHE_TEMPORARY,
    /* 1 */ CACHE_PERSISTENT,
    /* 2 */ CACHE_EITHER,
    /* 3 */ CACHE_PERMANENT
} AudioCacheType;

typedef enum AudioCacheLoadType {
    /* 0 */ CACHE_LOAD_PERMANENT,
    /* 1 */ CACHE_LOAD_PERSISTENT,
    /* 2 */ CACHE_LOAD_TEMPORARY,
    /* 3 */ CACHE_LOAD_EITHER,
    /* 4 */ CACHE_LOAD_EITHER_NOSYNC
} AudioCacheLoadType;

typedef struct AudioAsyncLoad {
    s8 status;
    s8 delay;
    s8 medium;
    u8* ramAddr;
    uintptr_t curDevAddr;
    u8* curRamAddr;
    size_t bytesRemaining;
    size_t chunkSize;
    s32 unkMediumParam;
    u32 retMsg;
    OSMesgQueue* retQueue;
    OSMesgQueue msgQueue;
    OSMesg msg;
    OSIoMesg ioMesg;
} AudioAsyncLoad;

typedef struct AudioSlowLoad {
    u8 medium;
    u8 seqOrFontId;
    u16 instId;
    s32 unkMediumParam;
    uintptr_t curDevAddr;
    u8* curRamAddr;
    u8* ramAddr;
    s32 status;
    size_t bytesRemaining;
    s8* isDone; /* TODO: rename in OoT and sync up here. This is an external status while (s32 status) is an internal status */
    Sample sample;
    OSMesgQueue msgqueue;
    OSMesg msg;
    OSIoMesg ioMesg;
} AudioSlowLoad;

typedef struct SampleDma {
    u8* ramAddr;
    uintptr_t devAddr;
    u16 sizeUnused;
    u16 size;
    u8 unused;
    u8 reuseIndex; /* position in sSampleDmaReuseQueue1/2, if ttl == 0 */
    u8 ttl;        /* Time To Live: duration after which the DMA can be discarded */
} SampleDma;

typedef struct AudioPreloadReq {
    u32 endAndMediumKey;
    Sample* sample;
    u8* ramAddr;
    u32 encodedInfo;
    s32 isFree;
} AudioPreloadReq;

typedef struct AudioHeapInitSizes {
    size_t heapSize; /* total number of bytes allocated to the audio heap. Must be <= the size of `gAudioHeap` (ideally about the same size) */
    size_t initPoolSize; /* The entire audio heap is split into two pools. */
    size_t permanentPoolSize;
} AudioHeapInitSizes;

/**
 * Meta-data associated with a pool (contain within the Audio Heap)
 */
typedef struct AudioAllocPool {
    u8* startAddr; /* start addr of the pool */
    u8* curAddr; /* address of the next available memory for allocation */
    size_t size; /* size of the pool */
    s32 count; /* number of entries allocated to the pool */
} AudioAllocPool;

/**
 * Audio cache entry data to store a single entry containing either a sequence, soundfont, or entire sample banks
 */
typedef struct AudioCacheEntry {
    u8* addr;
    size_t size;
    s16 tableType;
    s16 id;
} AudioCacheEntry;

/**
 * Audio cache entry data to store a single entry containing an individual sample
 */
typedef struct SampleCacheEntry {
    s8 inUse;
    s8 origMedium;
    u8 sampleBankId;
    s8 pad03[0x5];
    u8* allocatedAddr;
    void* sampleAddr;
    size_t size;
} SampleCacheEntry;

#if defined (GAME_MM)
#define NUM_SAMPLE_CACHE_ENTRIES 128
#else
#define NUM_SAMPLE_CACHE_ENTRIES 32
#endif

/**
 * Audio cache entry data to store individual samples
 */
typedef struct AudioSampleCache {
    AudioAllocPool pool;
    SampleCacheEntry entries[NUM_SAMPLE_CACHE_ENTRIES];
    s32 numEntries;
} AudioSampleCache;

typedef struct AudioPersistentCache {
    u32 numEntries;
    AudioAllocPool pool;
    AudioCacheEntry entries[16];
} AudioPersistentCache;

typedef struct AudioTemporaryCache {
    u32 nextSide;
    AudioAllocPool pool;
    AudioCacheEntry entries[2];
} AudioTemporaryCache;

typedef struct AudioCache {
    AudioPersistentCache persistent;
    AudioTemporaryCache temporary;
    s8 pad100[0x10];
} AudioCache;

typedef struct AudioCachePoolSplit {
    size_t persistentCommonPoolSize;
    size_t temporaryCommonPoolSize;
} AudioCachePoolSplit;

typedef struct AudioCommonPoolSplit {
    size_t seqCacheSize;
    size_t fontCacheSize;
    size_t sampleBankCacheSize;
} AudioCommonPoolSplit;

typedef struct AudioSessionPoolSplit {
    size_t miscPoolSize;
    size_t unusedSizes[2];
    size_t cachePoolSize;
} AudioSessionPoolSplit;

typedef struct SeqScriptState {
    u8* pc; /* program counter */
    u8* stack[4];
    u8 remLoopIters[4]; /* remaining loop iterations */
    u8 depth;
    s8 value;
} SeqScriptState;

typedef enum SeqPlayerState {
    /* 0 */ SEQPLAYER_STATE_0,
    /* 1 */ SEQPLAYER_STATE_FADE_IN,
    /* 2 */ SEQPLAYER_STATE_FADE_OUT
} SeqPlayerState;

#define AUDIO_ERROR(fontId, id, err) (((fontId << 8) + id) + (err << 24))

typedef enum AudioError {
    AUDIO_ERROR_NO_INST = 1,
    AUDIO_ERROR_INVALID_INST_ID = 3,
    AUDIO_ERROR_INVALID_DRUM_SFX_ID,
    AUDIO_ERROR_NO_DRUM_SFX,
    AUDIO_ERROR_FONT_NOT_LOADED = 0x10
} AudioError;

typedef union {
    struct {
        u8 unused : 2;
        u8 type : 2;
        u8 strongRight : 1;
        u8 strongLeft : 1;
        u8 strongReverbRight : 1;
        u8 strongReverbLeft : 1;
    };
    u8 asByte;
} StereoData;

typedef struct NoteAttributes {
    u8 targetReverbVol;
    u8 gain; /* Increases volume by a multiplicative scaling factor. Represented as a UQ4.4 number */
    u8 pan;
#if defined (GAME_MM)
    u8 surroundEffectIndex;
#endif
    StereoData stereoData;
    u8 combFilterSize;
    u16 combFilterGain;
    f32 freqScale;
    f32 velocity;
    s16* filter;
#if defined (GAME_MM)
    s16* filterBuf;
#else
    s16 filterBuf[8];
#endif
} NoteAttributes;

typedef enum NotePlaybackStatus {
    /* 0 */ PLAYBACK_STATUS_0,
    /* 1 */ PLAYBACK_STATUS_1,
    /* 2 */ PLAYBACK_STATUS_2
} NotePlaybackStatus;
typedef struct AudioListItem {
    struct AudioListItem* prev;
    struct AudioListItem* next;
    union {
        void* value; /* either Note* or SequenceLayer* */
        s32 count;
    } u;
    struct NotePool* pool;
} AudioListItem;

typedef struct NotePool {
    AudioListItem disabled;
    AudioListItem decaying;
    AudioListItem releasing;
    AudioListItem active;
} NotePool;

struct Note;
struct SequencePlayer;

/* Multi-Point ADSR Envelope (Attack, Decay, Sustain, Release) */

typedef enum AdsrStatus {
    /* 0 */ ADSR_STATUS_DISABLED,
    /* 1 */ ADSR_STATUS_INITIAL,
    /* 2 */ ADSR_STATUS_START_LOOP,
    /* 3 */ ADSR_STATUS_LOOP,
    /* 4 */ ADSR_STATUS_FADE,
    /* 5 */ ADSR_STATUS_HANG,
    /* 6 */ ADSR_STATUS_DECAY,
    /* 7 */ ADSR_STATUS_RELEASE,
    /* 8 */ ADSR_STATUS_SUSTAIN
} AdsrStatus;

/* Special commands for `delay` in `EnvelopePoint` */
/* Any value above 0 is treated as a delay */
#define ADSR_DISABLE 0
#define ADSR_HANG -1
#define ADSR_GOTO -2
#define ADSR_RESTART -3

typedef struct EnvelopePoint {
    s16 delay;
    s16 arg;
} EnvelopePoint;

typedef struct AdsrSettings {
    u8 decayIndex; /* index used to obtain adsr decay rate from adsrDecayTable */
    u8 sustain;
    EnvelopePoint* envelope;
} AdsrSettings;

typedef struct AdsrState {
    union {
        struct {
            u8 unused : 1;
            u8 hang : 1;
            u8 decay : 1;
            u8 release : 1;
            u8 status : 4;
        } s;
        u8 asByte;
    } action;
    u8 envelopeIndex;
    s16 delay;
    f32 sustain;
    f32 velocity;
    f32 fadeOutVel;
    f32 current;
    f32 target;
    s8 pad18[4];
    EnvelopePoint* envelope;
} AdsrState;


/* Vibrato */

typedef struct VibratoSubStruct {
    u16 vibratoRateStart;
    u16 vibratoDepthStart;
    u16 vibratoRateTarget;
    u16 vibratoDepthTarget;
    u16 vibratoRateChangeDelay;
    u16 vibratoDepthChangeDelay;
    u16 vibratoDelay;
} VibratoSubStruct;

typedef struct VibratoState {
#if defined (GAME_MM)
    VibratoSubStruct* vibSubStruct; /* Something else? */
#else
    struct SequenceChannel* channel;
#endif
    u32 time; /* 0x400 is 1 unit of time, 0x10000 is 1 period */
    s16* curve;
    f32 depth;
    f32 rate;
    u8 active;
    u16 rateChangeTimer;
    u16 depthChangeTimer;
    u16 delay;
} VibratoState;


/* Portamento */

typedef enum PortamentoMode {
    /* 0 */ PORTAMENTO_MODE_OFF,
    /* 1 */ PORTAMENTO_MODE_1,
    /* 2 */ PORTAMENTO_MODE_2,
    /* 3 */ PORTAMENTO_MODE_3,
    /* 4 */ PORTAMENTO_MODE_4,
    /* 5 */ PORTAMENTO_MODE_5
} PortamentoMode;

#define PORTAMENTO_IS_SPECIAL(x) ((x).mode & 0x80)
#define PORTAMENTO_MODE(x) ((x).mode & ~0x80)

/* Pitch sliding by up to one octave in the positive direction. Negative */
/* direction is "supported" by setting extent to be negative. The code */
/* extrapolates exponentially in the wrong direction in that case, but that */
/* doesn't prevent seqplayer from doing it, AFAICT. */
typedef struct Portamento {
    u8 mode; /* bit 0x80 denotes something; the rest are an index 0-5 */
    u16 cur;
    u16 speed;
    f32 extent;
} Portamento;

typedef struct NotePlaybackState {
    u8 priority;
    u8 waveId;
    u8 harmonicIndex; /* the harmonic index for the synthetic wave contained in gWaveSamples (also matches the base 2 logarithm of the harmonic order) */
    u8 fontId;
    u8 status;
    u8 stereoHeadsetEffects;
    s16 adsrVolScaleUnused;
    f32 portamentoFreqScale;
    f32 vibratoFreqScale;
#if defined (GAME_MM)
    struct SequenceLayer* wantedParentLayer;
    struct SequenceLayer* parentLayer;
    struct SequenceLayer* prevParentLayer;
#else
    struct SequenceLayer* prevParentLayer;
    struct SequenceLayer* parentLayer;
    struct SequenceLayer* wantedParentLayer;
#endif
    NoteAttributes attributes;
    AdsrState adsr;
    Portamento portamento;
    VibratoState vibratoState;
#if defined (GAME_MM)
    s8 pad7C[0x4];
    u8 unk_80;
    u32 startSamplePos;
    s8 unk_BC[0x1C];
#endif
} NotePlaybackState;

typedef struct NoteSampleState {
    struct {
        vu8 enabled : 1;
        u8 needsInit : 1;
        u8 finished : 1;
        u8 unused : 1;
        u8 strongRight : 1;
        u8 strongLeft : 1;
        u8 strongReverbRight : 1;
        u8 strongReverbLeft : 1;
    } bitField0;
    struct {
        u8 reverbIndex : 3;
        u8 bookOffset : 2;
        u8 isSyntheticWave : 1;
        u8 hasTwoParts : 1;
        u8 useHaasEffect : 1;
    } bitField1;
    u8 gain; /* Increases volume by a multiplicative scaling factor. Represented as a UQ4.4 number */
    u8 haasEffectLeftDelaySize;
    u8 haasEffectRightDelaySize;
    u8 targetReverbVol;
    u8 harmonicIndexCurAndPrev; /* bits 3..2 store curHarmonicIndex, bits 1..0 store prevHarmonicIndex */
    u8 combFilterSize;
    u16 targetVolLeft;
    u16 targetVolRight;
    u16 frequencyFixedPoint;
    u16 combFilterGain;
    union {
        TunedSample* tunedSample;
        s16* waveSampleAddr; /* used for synthetic waves */
    };
    s16* filter;
    s8 unk_18;
    u8 surroundEffectIndex;
    s8 unk_1A[0x6];
} NoteSampleState;

/* size of a single sample point */
#define SAMPLE_SIZE sizeof(s16)

/* Samples are processed in groups of 16 called a "frame" */
#define SAMPLES_PER_FRAME ADPCMFSIZE

/* The length of one left/right channel is 13 frames */
#define DMEM_1CH_SIZE (13 * SAMPLES_PER_FRAME * SAMPLE_SIZE)
/* Both left and right channels */
#define DMEM_2CH_SIZE (2 * DMEM_1CH_SIZE)

/* Must be the same amount of samples as copied by aDuplicate() (audio microcode) */
#define WAVE_SAMPLE_COUNT 64

typedef struct NoteSynthesisBuffers {
    s16 adpcmState[16];
    s16 finalResampleState[16];
    s16 filterState[32];
    s16 unusedState[16];
    s16 haasEffectDelayState[32];
    s16 combFilterState[128];
#if defined (GAME_MM)
    s16 surroundEffectState[128];
#endif
} NoteSynthesisBuffers;

typedef struct NoteSynthesisState {
    u8 atLoopPoint : 1;
    u8 stopLoop : 1;
    u8 sampleDmaIndex;
    u8 prevHaasEffectLeftDelaySize;
    u8 prevHaasEffectRightDelaySize;
    u8 curReverbVol;
    u8 numParts;
    u16 samplePosFrac; /* Fractional part of the sample position */
#if defined (GAME_MM)
    u16 surroundEffectGain;
#endif
    s32 samplePosInt; /* Integer part of the sample position */
    NoteSynthesisBuffers* synthesisBuffers;
    s16 curVolLeft;
    s16 curVolRight;
    s8 unk_18[0x6];
    u8 combFilterNeedsInit;
#if defined (GAME_MM)
    /*      0x1F */ u8 unk_1F;
#endif
    s8 unk_20[0x4];
} NoteSynthesisState;

typedef struct NoteSubEu {
    struct {
        volatile u8 enabled : 1;
        u8 needsInit : 1;
        u8 finished : 1; /* ? */
        u8 unused : 1;
        u8 stereoStrongRight : 1;
        u8 stereoStrongLeft : 1;
        u8 stereoHeadsetEffects : 1;
        u8 usesHeadsetPanEffects : 1; /* ? */
    } bitField0;
    struct {
        u8 reverbIndex : 3;
        u8 bookOffset : 2;
        u8 isSyntheticWave : 1;
        u8 hasTwoParts : 1;
        u8 useHaasEffect : 1;
    } bitField1;
    u8 gain; /* Increases volume by a multiplicative scaling factor. Represented as a UQ4.4 number */
    u8 haasEffectLeftDelaySize;
    u8 haasEffectRightDelaySize;
    u8 reverbVol;
    u8 harmonicIndexCurAndPrev; /* bits 3..2 store curHarmonicIndex, bits 1..0 store prevHarmonicIndex */
    u8 combFilterSize;
    u16 targetVolLeft;
    u16 targetVolRight;
    u16 resamplingRateFixedPoint;
    u16 combFilterGain;
    union {
        TunedSample* tunedSample;
        s16* waveSampleAddr; /* used for synthetic waves */
    };
    s16* filter;
    char pad_18[0x8];
} NoteSubEu;

typedef struct Note {
    AudioListItem listItem;
    NoteSynthesisState synthesisState;
    NotePlaybackState playbackState;
#if defined (GAME_MM)
    /*      0xD8 */ NoteSampleState sampleState;
#else
    /* 0xB8      */ char unk_B8[0x4];
    /* 0xBC      */ u32 startSamplePos; /* initial position/index to start processing s16 samples */
    /* 0xC0      */ NoteSubEu noteSubEu;
#endif
} Note;

/* Also known as a Group, according to debug strings. */
typedef struct SequencePlayer {
    u8 enabled : 1;
    u8 finished : 1;
    u8 muted : 1;
    u8 seqDmaInProgress : 1;
    u8 fontDmaInProgress : 1;
    u8 recalculateVolume : 1;
    u8 stopScript : 1;
    u8 applyBend : 1;
    u8 state;
    u8 noteAllocPolicy;
    u8 muteFlags;
    u8 seqId;
    u8 defaultFont;
    u8 unk_06[1];
    s8 playerIndex;
    u16 tempo; /* tatums per minute */
    u16 tempoAcc;
    s16 tempoChange;
    s16 transposition;
    u16 delay;
    u16 fadeTimer;
    u16 storedFadeTimer;
    u16 unk_16;
    u8* seqData;
    f32 fadeVolume;
    f32 fadeVelocity;
    f32 volume;
    f32 muteVolumeScale;
    f32 fadeVolumeScale;
    f32 appliedFadeVolume;
    f32 bend;
    struct SequenceChannel* channels[16];
    SeqScriptState scriptState;
    u8* shortNoteVelocityTable;
    u8* shortNoteGateTimeTable;
    NotePool notePool;
    s32 skipTicks;
    u32 scriptCounter;
    s8 unk_E4[0x74]; /* unused struct members for sequence/sound font dma management, according to sm64 decomp */
    s8 seqScriptIO[8];
} SequencePlayer;

/* Also known as a SubTrack, according to sm64 debug strings. */
typedef struct SequenceChannel {
    u8 enabled : 1;
    u8 finished : 1;
    u8 stopScript : 1;
    u8 muted : 1; /* sets SequenceLayer.muted */
    u8 hasInstrument : 1;
    u8 stereoHeadsetEffects : 1;
    u8 largeNotes : 1; /* notes specify duration and velocity */
    u8 unused : 1;
    union {
        struct {
            u8 freqScale : 1;
            u8 volume : 1;
            u8 pan : 1;
        } s;
        u8 asByte;
    } changes;
    u8 noteAllocPolicy;
    u8 muteFlags;
    u8 targetReverbVol; /* or dry/wet mix */
    u8 notePriority; /* 0-3 */
    u8 someOtherPriority;
    u8 fontId;
    u8 reverbIndex;
    u8 bookOffset;
    u8 newPan;
    u8 panChannelWeight;  /* proportion of pan that comes from the channel (0..128) */
    u8 gain; /* Increases volume by a multiplicative scaling factor. Represented as a UQ4.4 number */
    u8 velocityRandomVariance;
    u8 gateTimeRandomVariance;
    u8 combFilterSize;
#if defined (GAME_MM)
    u8 surroundEffectIndex;
    u8 channelIndex;
#endif
    VibratoSubStruct vibrato;
    u16 delay;
    u16 combFilterGain;
    u16 unk_22; /* Used for indexing data */
    s16 instOrWave; /* either 0 (none), instrument index + 1, or */
                             /* 0x80..0x83 for sawtooth/triangle/sine/square waves. */
    s16 transposition;
    f32 volumeScale;
    f32 volume;
    s32 pan;
    f32 appliedVolume;
    f32 freqScale;
    u8 (*dynTable)[][2];
    Note* noteUnused;
    struct SequenceLayer* layerUnused;
    struct Instrument* instrument;
    SequencePlayer* seqPlayer;
    struct SequenceLayer* layers[4];
    SeqScriptState scriptState;
    AdsrSettings adsr;
    NotePool notePool;
    s8 seqScriptIO[8]; /* bridge between sound script and audio lib, "io ports" */
#if defined (GAME_MM)
    /*      0xD0 */ u8* sfxState; /* SfxChannelState */
#endif
    s16* filter;
    StereoData stereoData;
#if defined (GAME_MM)
    s32 startSamplePos;
    s32 unk_E0;
#endif
} SequenceChannel;

/* Might also be known as a Track, according to sm64 debug strings (?). */
typedef struct SequenceLayer {
    u8 enabled : 1;
    u8 finished : 1;
    u8 muted : 1;
    u8 continuousNotes : 1; /* keep the same note for consecutive notes with the same sound */
    u8 bit3 : 1; /* "loaded"? */
    u8 ignoreDrumPan : 1;
    u8 bit1 : 1; /* "has initialized continuous notes"? */
    u8 notePropertiesNeedInit : 1;
    StereoData stereoData;
    u8 instOrWave;
    u8 gateTime;
    u8 semitone;
    u8 portamentoTargetNote;
    u8 pan; /* 0..128 */
    u8 notePan;
#if defined (GAME_MM)
    u8 surroundEffectIndex;
    u8 targetReverbVol;
    union {
        struct {
            u16 bit_0 : 1;
            u16 bit_1 : 1;
            u16 bit_2 : 1;
            u16 useVibrato : 1;
            u16 bit_4 : 1;
            u16 bit_5 : 1;
            u16 bit_6 : 1;
            u16 bit_7 : 1;
            u16 bit_8 : 1;
            u16 bit_9 : 1;
            u16 bit_A : 1;
            u16 bit_B : 1;
            u16 bit_C : 1;
            u16 bit_D : 1;
            u16 bit_E : 1;
            u16 bit_F : 1;
        } s;
        u16 asByte;
    } unk_0A;
    VibratoSubStruct vibrato;
#endif
    s16 delay;
    s16 gateDelay;
    s16 delay2;
    u16 portamentoTime;
    s16 transposition; /* #semitones added to play commands */
                                  /* (seq instruction encoding only allows referring to the limited range */
                                  /* 0..0x3F; this makes 0x40..0x7F accessible as well) */
    s16 shortNoteDefaultDelay;
    s16 lastDelay;
    AdsrSettings adsr;
    Portamento portamento;
    Note* note;
    f32 freqScale;
    f32 bend;
    f32 velocitySquare2;
    f32 velocitySquare; /* not sure which one of those corresponds to the sm64 original */
    f32 noteVelocity;
    f32 noteFreqScale;
    struct Instrument* instrument;
    struct TunedSample* tunedSample;
    SequenceChannel* channel;
    SeqScriptState scriptState;
    AudioListItem listItem;
} SequenceLayer;

#if defined (GAME_MM)
#define NUM_SEQUENCE_PLAYERS 5
#else
#define NUM_SEQUENCE_PLAYERS 4
#endif

typedef struct AudioContext {
    char unk_0000;
    s8 numSynthesisReverbs;
    u16 unk_2; /* reads from audio spec unk_14, never used, always set to 0x7FFF */
    u16 unk_4;
    char unk_0006[0xA];
    s16* adpcmCodeBook;
    struct NoteSampleState* sampleStateList;
    SynthesisReverb synthesisReverbs[4];
    char unk_0B58[0x30];
    struct Sample* usedSamples[128];
    AudioPreloadReq preloadSampleStack[128];
    s32 numUsedSamples;
    s32 preloadSampleStackTop;
    AudioAsyncLoad asyncLoads[0x10];
    OSMesgQueue asyncLoadUnkMediumQueue;
    char unk_1D08[0x40];
    AudioAsyncLoad* curUnkMediumLoad;
    u32 slowLoadPos;
    AudioSlowLoad slowLoads[2];
    OSPiHandle* cartHandle;
    OSPiHandle* driveHandle;
    OSMesgQueue externalLoadQueue;
    OSMesg externalLoadMesgBuf[0x10];
    OSMesgQueue preloadSampleQueue;
    OSMesg preloadSampleMesgBuf[0x10];
    OSMesgQueue curAudioFrameDmaQueue;
    OSMesg currAudioFrameDmaMesgBuf[0x40];
    OSIoMesg currAudioFrameDmaIoMesgBuf[0x40];
    OSMesgQueue syncDmaQueue;
    OSMesg syncDmaMesg;
    OSIoMesg syncDmaIoMesg;
    SampleDma* sampleDmas;
    u32 sampleDmaCount;
    u32 sampleDmaListSize1;
    s32 unused2648;
    u8 sampleDmaReuseQueue1[0x100]; /* read pos <= write pos, wrapping mod 256 */
    u8 sampleDmaReuseQueue2[0x100];
    u8 sampleDmaReuseQueue1RdPos; /* Read position for dma 1 */
    u8 sampleDmaReuseQueue2RdPos; /* Read position for dma 2 */
    u8 sampleDmaReuseQueue1WrPos; /* Write position for dma 1 */
    u8 sampleDmaReuseQueue2WrPos; /* Write position for dma 2 */
    AudioTable* sequenceTable;
    AudioTable* soundFontTable;
    AudioTable* sampleBankTable;
#if defined (GAME_MM)
    char unk_285C[0x4];
#endif
    u8* sequenceFontTable;
    u16 numSequences;
    struct SoundFont* soundFontList;
    AudioBufferParameters audioBufferParameters;
    f32 unk_2870;
    s32 sampleDmaBufSize1;
    s32 sampleDmaBufSize2;
    char unk_287C[0x10];
    s32 sampleDmaBufSize;
    s32 maxAudioCmds;
    s32 numNotes;
    s16 maxTempo;
    s8 soundMode;
    s32 totalTaskCount; /* The total number of times the top-level function on the audio thread is run since the last audio reset */
    s32 curAudioFrameDmaCount;
    s32 rspTaskIndex;
    s32 curAiBufferIndex;
    Acmd* abiCmdBufs[2]; /* Pointer to audio heap where the audio binary interface command lists are stored. Two lists that alternative every frame */
    Acmd* curAbiCmdBuf;
    AudioTask* curTask;
#if defined (GAME_OOT)
    char unk_28BC[0x4];
#endif
    AudioTask rspTask[2];
    f32 unk_2960;
    /* 0x2984*/ s32 refreshRate;
    s16* aiBuffers[3]; /* Pointers to the audio buffer allocated on the initPool contained in the audio heap. Stores fully processed digital audio before transferring to the audio interface (AI) */
    s16 numSamplesPerFrame[3]; /* Number of samples to transfer to the audio interface buffer */
    u32 audioRandom;
    s32 audioErrorFlags;
    volatile u32 resetTimer;
#if defined (GAME_MM)
    u32 (*customSeqFunctions[4])(s8 value, SequenceChannel* channel);
#endif
    s8 unk_29B8;
    s32 numAbiCmdsMax; /* sMaxAbiCmdCnt */
    AudioAllocPool sessionPool; /* A sub-pool to main pool, contains all sub-pools and data that changes every audio reset */
    AudioAllocPool externalPool; /* pool allocated on an external device. Never used in game */
    AudioAllocPool initPool; /* A sub-pool to the main pool, contains all sub-pools and data that persists every audio reset */
    AudioAllocPool miscPool; /* A sub-pool to the session pool, */
    char unk_29D0[0x20]; /* probably two unused pools */
    AudioAllocPool cachePool; /* The common pool for all cache entries */
    AudioAllocPool persistentCommonPool; /* A sub-pool to the cache pool, contains all caches for data stored persistently */
    AudioAllocPool temporaryCommonPool; /* A sub-pool to the cache pool, contains all caches for data stored temporarily */
    AudioCache seqCache; /* Cache to store sequences */
    AudioCache fontCache; /* Cache to store soundFonts */
    AudioCache sampleBankCache; /* Cache for loading entire sample banks */
    AudioAllocPool permanentPool; /* Pool to stores audio data that is always loaded in. Primarily used for sfxs */
    AudioCacheEntry permanentEntries[32]; /* indificual entries to the permanent pool */
    AudioSampleCache persistentSampleCache; /* Stores individual samples persistently */
    AudioSampleCache temporarySampleCache; /* Stores individual samples temporarily */
    AudioSessionPoolSplit sessionPoolSplit; /* splits session pool into the cache pool and misc pool */
    AudioCachePoolSplit cachePoolSplit; /* splits cache pool into the persistent & temporary common pools */
    AudioCommonPoolSplit persistentCommonPoolSplit; /* splits persistent common pool into caches for sequences, soundFonts, sample banks */
    AudioCommonPoolSplit temporaryCommonPoolSplit; /* splits temporary common pool into caches for sequences, soundFonts, sample banks */
    u8 sampleFontLoadStatus[0x30];
    u8 fontLoadStatus[0x30];
    u8 seqLoadStatus[0x80];
    vu8 resetStatus;
    u8 specId;
    s32 audioResetFadeOutFramesLeft;
    f32* adsrDecayTable; /* A table on the audio heap that stores decay rates used for ADSR */
    u8* audioHeap;
    size_t audioHeapSize;
    struct Note* notes; /* dynamically sized based on gAudioCtx.numNotes */
    SequencePlayer seqPlayers[NUM_SEQUENCE_PLAYERS];
    SequenceLayer sequenceLayers[16*NUM_SEQUENCE_PLAYERS];
    SequenceChannel sequenceChannelNone;
    s32 sampleStateOffset; /* Start of the list of sample states for this update. Resets after each audio frame. */
    AudioListItem layerFreeList;
    NotePool noteFreeLists;
    u8 threadCmdWritePos;
    u8 threadCmdReadPos;
    u8 threadCmdQueueFinished;
    u16 threadCmdChannelMask[NUM_SEQUENCE_PLAYERS]; /* bit-packed for 16 channels. When processing an audio thread channel command on all channels, only process channels with their bit set. */
    OSMesgQueue* audioResetQueueP;
    OSMesgQueue* taskStartQueueP;
    OSMesgQueue* threadCmdProcQueueP;
    OSMesgQueue taskStartQueue;
    OSMesgQueue threadCmdProcQueue;
    OSMesgQueue audioResetQueue;
    OSMesg taskStartMsgs[1];
    OSMesg audioResetMesgs[1];
    OSMesg threadCmdProcMsgBuf[4];
    AudioCmd threadCmdBuf[0x100]; /* Audio commands used to transfer audio requests from the graph thread to the audio thread */
#if defined (GAME_MM)
    s8 unk_81F4[4];
#endif
} AudioContext;

extern AudioContext gAudioCtx;

enum SequencePlayState {
    SEQUENCE_PLAY_STATE_NONE            = 0b0000000000000000,
    SEQUENCE_PLAY_STATE_FIERCE_DEITY    = 0b0000000000000001,
    SEQUENCE_PLAY_STATE_GORON           = 0b0000000000000010,
    SEQUENCE_PLAY_STATE_ZORA            = 0b0000000000000100,
    SEQUENCE_PLAY_STATE_DEKU            = 0b0000000000001000,
    SEQUENCE_PLAY_STATE_HUMAN           = 0b0000000000010000,
    SEQUENCE_PLAY_STATE_DAY             = 0b0000000000100000,
    SEQUENCE_PLAY_STATE_NIGHT           = 0b0000000001000000,
    SEQUENCE_PLAY_STATE_OUTDOORS        = 0b0000000010000000,
    SEQUENCE_PLAY_STATE_INDOORS         = 0b0000000100000000,
    SEQUENCE_PLAY_STATE_CAVE            = 0b0000001000000000,
    SEQUENCE_PLAY_STATE_EPONA           = 0b0000010000000000,
    SEQUENCE_PLAY_STATE_SWIM            = 0b0000100000000000,
    SEQUENCE_PLAY_STATE_SPIKE_ROLLING   = 0b0001000000000000,
    SEQUENCE_PLAY_STATE_COMBAT          = 0b0010000000000000,
    SEQUENCE_PLAY_STATE_CRITICAL_HEALTH = 0b0100000000000000
};

typedef struct SequenceData {
    u16 playMask[0x10];
    union {
        struct {
            u16                : 1;
            u16 criticalHealth : 1;
            u16 combat         : 1;
            u16 spikeRolling   : 1;
            u16 swimming       : 1;
            u16 epona          : 1;
            u16 cave           : 1;
            u16 indoors        : 1;
            u16 outdoors       : 1;
            u16 night          : 1;
            u16 day            : 1;
            u16 human          : 1;
            u16 deku           : 1;
            u16 zora           : 1;
            u16 goron          : 1;
            u16 fierceDeity    : 1;
        };
        u16 value;
    } cumulativeStates;
    u8 hasFormMask;
    u8 pad2A[0xd]; /* everdrive rounds down to 0x10s when loading from rom */
} SequenceData;

typedef struct MusicState {
    SequenceData sequenceData;
    u8 loadedSequenceId;
    u8 isInCave  : 1;
    u8 isIndoors : 1;
    u8 padding   : 6;
    u16 currentState;
    u16 forceMute;
} MusicState;

#endif
