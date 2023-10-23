#ifndef COMBO_MM_EN_SUTTARI_H
#define COMBO_MM_EN_SUTTARI_H

#include <combo/common/actor.h>

typedef struct 
{
    /* 0x000 */ Actor base;
    /* 0x144 */ char pad144[0x4];
    /* 0x148 */ void* function;
    /* 0x14C */ char pad14C[0x98];
    /* 0x1E4 */ u16 unk1E4;
    /* 0x1E6 */ u16 flags;
    /* 0x1E8 */ char pad1E8[0x10];
    /* 0x1F8 */ s32 escapeStatus;
    /* 0x1FC */ char pad1FC[0x254];
    /* 0x450 */ u32 runningState;
    /* 0x454 */ char pad454[0x2];
    /* 0x456 */ s16 actorCutscene1;
    /* 0x458 */ s16 actorCutscene2;
    /* 0x45A */ char pad45A[0x2];
} Actor_EnSuttari; // size = 0x45C

#endif
