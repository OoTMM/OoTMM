#ifndef COMBO_SFX_H
#define COMBO_SFX_H

#include <combo/sfx_id.h>
#include <combo/math/vec.h>

extern Vec3f gSfxDefaultPos;
extern f32 gSfxDefaultFreqAndVolScale;
extern s8 gSfxDefaultReverb;

void Audio_SetSfxBanksMute(u16 muteMask);
void Audio_QueueSeqCmdMute(u8 channelIndex);
void Audio_ClearBGMMute(u8 channelIndex);
void Audio_PlaySfxGeneral(u16 sfxId, Vec3f* pos, u8 token, f32* freqScale, f32* vol, s8* reverbAdd);
void Audio_ProcessSfxRequest(void);
void Audio_ChooseActiveSfx(u8 bankId);
void Audio_PlayActiveSfx(u8 bankId);
void Audio_StopSfxByBank(u8 bankId);
void Audio_RemoveSfxFromBankByPos(u8 bankId, Vec3f* pos);
void Audio_StopSfxByPosAndBank(u8 bankId, Vec3f* pos);
void Audio_StopSfxByPos(Vec3f* pos);
void Audio_StopSfxByPosAndId(Vec3f* pos, u16 sfxId);
void Audio_StopSfxByTokenAndId(u8 token, u16 sfxId);
void Audio_StopSfxById(u32 sfxId);
void Audio_ProcessSfxRequests(void);
void func_800F8F88(void);
u8 Audio_IsSfxPlaying(u32 sfxId);
void Audio_ResetSfx(void);

#endif
