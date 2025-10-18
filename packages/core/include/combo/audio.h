#ifndef COMBO_AUDIO_H
#define COMBO_AUDIO_H

#include <combo/types.h>
#include <combo/math/vec.h>

typedef struct
{
    u32 romAddr;
    u32 size;
    s8  medium;
    s8  cachePolicy;
    u8  data[6];
}
AudioTableEntry;

typedef struct
{
    u16             count;
    s16             unk_02;
    u32             romAddr;
    char            pad[0x8];
}
AudioTableHeader;

typedef struct
{
    AudioTableHeader header;
    AudioTableEntry  entries[];
}
AudioTable;

typedef struct PlayState PlayState;

void    Audio_SetFileSelectSettings(u8 audioSetting);
void    Audio_SetCutsceneFlag(int flag);
u16     Audio_GetActiveSeqId(int channel);
void    Audio_DisplayMusicName(PlayState* play);
void    Audio_PlaySequenceWithSeqPlayerIO(u8 seqPlayerIndex, u16 seqId, u8 fadeInDuration, s8 ioPort, s8 ioData);

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
void Audio_PlaySfxRiver(Vec3f* pos, f32 freqScale);
void Audio_LowerMainBgmVolume(u8 volume);
void Audio_UpdateMalonSinging(f32 dist, u16 seqId);
void Audio_ToggleMalonSinging(u8 malonSingingDisabled);
extern u8* gFrogsSongPtr;
#endif

#endif
