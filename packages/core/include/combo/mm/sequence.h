#ifndef COMBO_MM_SEQUENCE_H
#define COMBO_MM_SEQUENCE_H

#include <combo/types.h>
#include <combo/math/vec.h>
#include <combo/data/sequence.h>

#define SEQ_FLAG_ASYNC 0x8000

typedef struct Actor Actor;

typedef enum SequencePlayerId {
    /*    0 */ SEQ_PLAYER_BGM_MAIN,
    /*    1 */ SEQ_PLAYER_FANFARE,
    /*    2 */ SEQ_PLAYER_SFX,
    /*    3 */ SEQ_PLAYER_BGM_SUB,
    /*    4 */ SEQ_PLAYER_AMBIENCE,
    /*    5 */ SEQ_PLAYER_MAX,
    /* 0xFF */ SEQ_PLAYER_INVALID = 0xFF
} SequencePlayerId;

typedef enum SequenceMode {
    /* 0 */ SEQ_MODE_DEFAULT,
    /* 1 */ SEQ_MODE_ENEMY,
    /* 2 */ SEQ_MODE_STILL, // Not moving or first-person view
    /* 3 */ SEQ_MODE_IGNORE
} SequenceMode;

typedef enum ChannelIOPort {
    /* 0 */ CHANNEL_IO_PORT_0,
    /* 1 */ CHANNEL_IO_PORT_1,
    /* 2 */ CHANNEL_IO_PORT_2,
    /* 3 */ CHANNEL_IO_PORT_3,
    /* 4 */ CHANNEL_IO_PORT_4,
    /* 5 */ CHANNEL_IO_PORT_5,
    /* 6 */ CHANNEL_IO_PORT_6,
    /* 7 */ CHANNEL_IO_PORT_7
} ChannelIOPort;

typedef enum VolumeScaleIndex {
    /* 0 */ VOL_SCALE_INDEX_BGM_MAIN,
    /* 1 */ VOL_SCALE_INDEX_FANFARE,
    /* 2 */ VOL_SCALE_INDEX_SFX,
    /* 3 */ VOL_SCALE_INDEX_BGM_SUB,
    /* 4 */ VOL_SCALE_INDEX_MAX
} VolumeScaleIndex; // May be worth using SequencePlayerId instead

typedef enum AmbienceChannelIndex {
    /* 0x0 */ AMBIENCE_CHANNEL_STREAM_0,
    /* 0x1 */ AMBIENCE_CHANNEL_CRITTER_0,
    /* 0x2 */ AMBIENCE_CHANNEL_CRITTER_1,
    /* 0x3 */ AMBIENCE_CHANNEL_CRITTER_2,
    /* 0x4 */ AMBIENCE_CHANNEL_CRITTER_3,
    /* 0x5 */ AMBIENCE_CHANNEL_CRITTER_4,
    /* 0x6 */ AMBIENCE_CHANNEL_CRITTER_5,
    /* 0x7 */ AMBIENCE_CHANNEL_CRITTER_6,
    /* 0x8 */ AMBIENCE_CHANNEL_CRITTER_7,
    /* 0xC */ AMBIENCE_CHANNEL_STREAM_1 = 12,
    /* 0xD */ AMBIENCE_CHANNEL_SOUND_MODE,
    /* 0xE */ AMBIENCE_CHANNEL_RAIN,
    /* 0xF */ AMBIENCE_CHANNEL_LIGHTNING
} AmbienceChannelIndex; // seqPlayerIndex = SEQ_PLAYER_AMBIENCE

typedef enum AmbienceId {
    /* 0x00 */ AMBIENCE_ID_00,
    /* 0x01 */ AMBIENCE_ID_01,
    /* 0x02 */ AMBIENCE_ID_02,
    /* 0x03 */ AMBIENCE_ID_03,
    /* 0x04 */ AMBIENCE_ID_04,
    /* 0x05 */ AMBIENCE_ID_05,
    /* 0x06 */ AMBIENCE_ID_06,
    /* 0x07 */ AMBIENCE_ID_07,
    /* 0x08 */ AMBIENCE_ID_08,
    /* 0x09 */ AMBIENCE_ID_09,
    /* 0x0A */ AMBIENCE_ID_0A,
    /* 0x0B */ AMBIENCE_ID_0B,
    /* 0x0C */ AMBIENCE_ID_0C,
    /* 0x0D */ AMBIENCE_ID_0D,
    /* 0x0E */ AMBIENCE_ID_0E,
    /* 0x0F */ AMBIENCE_ID_0F,
    /* 0x10 */ AMBIENCE_ID_10,
    /* 0x11 */ AMBIENCE_ID_11,
    /* 0x12 */ AMBIENCE_ID_12,
    /* 0x13 */ AMBIENCE_ID_13,
    /* 0xFF */ AMBIENCE_ID_DISABLED = 0xFF
} AmbienceId; // seqPlayerIndex = SEQ_PLAYER_AMBIENCE

typedef enum AmbienceStreamId {
    /* 0 */ AMBIENCE_STREAM_0,
    /* 1 */ AMBIENCE_STREAM_1,
    /* 2 */ AMBIENCE_STREAM_2,
    /* 3 */ AMBIENCE_STREAM_3
} AmbienceStreamId;

typedef enum AmbienceCritterId {
    /* 0x00 */ AMBIENCE_CRITTER_00,
    /* 0x01 */ AMBIENCE_CRITTER_01,
    /* 0x02 */ AMBIENCE_CRITTER_02,
    /* 0x03 */ AMBIENCE_CRITTER_03,
    /* 0x04 */ AMBIENCE_CRITTER_04,
    /* 0x05 */ AMBIENCE_CRITTER_05,
    /* 0x06 */ AMBIENCE_CRITTER_06,
    /* 0x07 */ AMBIENCE_CRITTER_07,
    /* 0x08 */ AMBIENCE_CRITTER_08,
    /* 0x09 */ AMBIENCE_CRITTER_09,
    /* 0x0A */ AMBIENCE_CRITTER_10,
    /* 0x0B */ AMBIENCE_CRITTER_11,
    /* 0x0C */ AMBIENCE_CRITTER_12,
    /* 0x0D */ AMBIENCE_CRITTER_13,
    /* 0x0E */ AMBIENCE_CRITTER_14,
    /* 0x0F */ AMBIENCE_CRITTER_15,
    /* 0x10 */ AMBIENCE_CRITTER_16,
    /* 0x11 */ AMBIENCE_CRITTER_17,
    /* 0x12 */ AMBIENCE_CRITTER_18,
    /* 0x13 */ AMBIENCE_CRITTER_19
} AmbienceCritterId;

#define AMBIENCE_IO_CRITTER_0_TYPE(type)        AMBIENCE_CHANNEL_CRITTER_0, CHANNEL_IO_PORT_2, type
#define AMBIENCE_IO_CRITTER_0_BEND_PITCH(bend)  AMBIENCE_CHANNEL_CRITTER_0, CHANNEL_IO_PORT_3, bend
#define AMBIENCE_IO_CRITTER_0_NUM_LAYERS(num)   AMBIENCE_CHANNEL_CRITTER_0, CHANNEL_IO_PORT_4, num
#define AMBIENCE_IO_CRITTER_0_PORT5(reverb)     AMBIENCE_CHANNEL_CRITTER_0, CHANNEL_IO_PORT_5, reverb

#define AMBIENCE_IO_CRITTER_1_TYPE(type)        AMBIENCE_CHANNEL_CRITTER_1, CHANNEL_IO_PORT_2, type
#define AMBIENCE_IO_CRITTER_1_BEND_PITCH(bend)  AMBIENCE_CHANNEL_CRITTER_1, CHANNEL_IO_PORT_3, bend
#define AMBIENCE_IO_CRITTER_1_NUM_LAYERS(num)   AMBIENCE_CHANNEL_CRITTER_1, CHANNEL_IO_PORT_4, num
#define AMBIENCE_IO_CRITTER_1_PORT5(reverb)     AMBIENCE_CHANNEL_CRITTER_1, CHANNEL_IO_PORT_5, reverb

#define AMBIENCE_IO_CRITTER_2_TYPE(type)        AMBIENCE_CHANNEL_CRITTER_2, CHANNEL_IO_PORT_2, type
#define AMBIENCE_IO_CRITTER_2_BEND_PITCH(bend)  AMBIENCE_CHANNEL_CRITTER_2, CHANNEL_IO_PORT_3, bend
#define AMBIENCE_IO_CRITTER_2_NUM_LAYERS(num)   AMBIENCE_CHANNEL_CRITTER_2, CHANNEL_IO_PORT_4, num
#define AMBIENCE_IO_CRITTER_2_PORT5(reverb)     AMBIENCE_CHANNEL_CRITTER_2, CHANNEL_IO_PORT_5, reverb

#define AMBIENCE_IO_CRITTER_3_TYPE(type)        AMBIENCE_CHANNEL_CRITTER_3, CHANNEL_IO_PORT_2, type
#define AMBIENCE_IO_CRITTER_3_BEND_PITCH(bend)  AMBIENCE_CHANNEL_CRITTER_3, CHANNEL_IO_PORT_3, bend
#define AMBIENCE_IO_CRITTER_3_NUM_LAYERS(num)   AMBIENCE_CHANNEL_CRITTER_3, CHANNEL_IO_PORT_4, num
#define AMBIENCE_IO_CRITTER_3_PORT5(reverb)     AMBIENCE_CHANNEL_CRITTER_3, CHANNEL_IO_PORT_5, reverb

#define AMBIENCE_IO_CRITTER_4_TYPE(type)        AMBIENCE_CHANNEL_CRITTER_4, CHANNEL_IO_PORT_2, type
#define AMBIENCE_IO_CRITTER_4_BEND_PITCH(bend)  AMBIENCE_CHANNEL_CRITTER_4, CHANNEL_IO_PORT_3, bend
#define AMBIENCE_IO_CRITTER_4_NUM_LAYERS(num)   AMBIENCE_CHANNEL_CRITTER_4, CHANNEL_IO_PORT_4, num
#define AMBIENCE_IO_CRITTER_4_PORT5(reverb)     AMBIENCE_CHANNEL_CRITTER_4, CHANNEL_IO_PORT_5, reverb

#define AMBIENCE_IO_CRITTER_5_TYPE(type)        AMBIENCE_CHANNEL_CRITTER_5, CHANNEL_IO_PORT_2, type
#define AMBIENCE_IO_CRITTER_5_BEND_PITCH(bend)  AMBIENCE_CHANNEL_CRITTER_5, CHANNEL_IO_PORT_3, bend
#define AMBIENCE_IO_CRITTER_5_NUM_LAYERS(num)   AMBIENCE_CHANNEL_CRITTER_5, CHANNEL_IO_PORT_4, num
#define AMBIENCE_IO_CRITTER_5_PORT5(reverb)     AMBIENCE_CHANNEL_CRITTER_5, CHANNEL_IO_PORT_5, reverb

#define AMBIENCE_IO_CRITTER_6_TYPE(type)        AMBIENCE_CHANNEL_CRITTER_6, CHANNEL_IO_PORT_2, type
#define AMBIENCE_IO_CRITTER_6_BEND_PITCH(bend)  AMBIENCE_CHANNEL_CRITTER_6, CHANNEL_IO_PORT_3, bend
#define AMBIENCE_IO_CRITTER_6_NUM_LAYERS(num)   AMBIENCE_CHANNEL_CRITTER_6, CHANNEL_IO_PORT_4, num
#define AMBIENCE_IO_CRITTER_6_PORT5(reverb)     AMBIENCE_CHANNEL_CRITTER_6, CHANNEL_IO_PORT_5, reverb

#define AMBIENCE_IO_STREAM_0_TYPE(type)         AMBIENCE_CHANNEL_STREAM_0, CHANNEL_IO_PORT_2, type
#define AMBIENCE_IO_STREAM_0_PORT3(data)        AMBIENCE_CHANNEL_STREAM_0, CHANNEL_IO_PORT_3, data
#define AMBIENCE_IO_STREAM_0_PORT4(data)        AMBIENCE_CHANNEL_STREAM_0, CHANNEL_IO_PORT_4, data

#define AMBIENCE_IO_STREAM_1_TYPE(type)         AMBIENCE_CHANNEL_STREAM_1, CHANNEL_IO_PORT_2, type
#define AMBIENCE_IO_STREAM_1_PORT3(data)        AMBIENCE_CHANNEL_STREAM_1, CHANNEL_IO_PORT_3, data
#define AMBIENCE_IO_STREAM_1_PORT4(data)        AMBIENCE_CHANNEL_STREAM_1, CHANNEL_IO_PORT_4, data

#define AMBIENCE_IO_RAIN_PORT4(data)            AMBIENCE_CHANNEL_RAIN, CHANNEL_IO_PORT_4, data

#define AMBIENCE_IO_ENTRIES_END 0xFF

typedef struct {
    /* 0x00 */ float volCur;
    /* 0x04 */ float volTarget;
    /* 0x08 */ float volStep;
    /* 0x0C */ float freqScaleCur;
    /* 0x10 */ float freqScaleTarget;
    /* 0x14 */ float freqScaleStep;
    /* 0x18 */ u16 volTimer;
    /* 0x1A */ u16 freqScaleTimer;
} ActiveSequenceChannelData; // size = 0x1C

typedef struct {
    /* 0x000 */ ActiveSequenceChannelData channelData[16]; // SEQ_NUM_CHANNELS
    /* 0x1C0 */ float volCur;
    /* 0x1C4 */ float volTarget;
    /* 0x1C8 */ float volStep;
    /* 0x1CC */ u32 tempoCmd;
    /* 0x1D0 */ float tempoCur;
    /* 0x1D4 */ float tempoTarget;
    /* 0x1D8 */ float tempoStep;
    /* 0x1DC */ u32 setupCmd[8]; // setup commands
    /* 0x1FC */ u32 startAsyncSeqCmd; // temporarily stores the seqCmd used in SEQCMD_PLAY_SEQUENCE, to be called again once the font is reloaded in
    /* 0x200 */ u16 volTimer;
    /* 0x202 */ u16 tempoOriginal;
    /* 0x204 */ u16 tempoTimer;
    /* 0x206 */ u16 freqScaleChannelFlags;
    /* 0x208 */ u16 volChannelFlags;
    /* 0x20A */ u16 seqId;
    /* 0x20C */ u16 prevSeqId; // last seqId played on a player
    /* 0x20E */ u16 channelPortMask;
    /* 0x210 */ u8 isWaitingForFonts;
    /* 0x211 */ u8 fontId;
    /* 0x212 */ u8 volScales[VOL_SCALE_INDEX_MAX];
    /* 0x216 */ u8 volFadeTimer;
    /* 0x217 */ u8 fadeVolUpdate;
    /* 0x218 */ u8 setupCmdTimer;
    /* 0x219 */ u8 setupCmdNum; // number of setup commands
    /* 0x21A */ u8 setupFadeTimer;
    /* 0x21B */ u8 isSeqPlayerInit;
} ActiveSequence; // size = 0x21C

typedef struct {
    /* 0x0 */ u8 seqId;
    /* 0x1 */ u8 priority;
} SeqRequest; // size = 0x2

void Audio_PlayBgm_StorePrevBgm(u16 seqId);
void Audio_RestorePrevBgm(void);
s16 Actor_WorldYawTowardPoint(Actor* actor, Vec3f* refPoint);

void Audio_SetSpec(u8 specId);

#endif
