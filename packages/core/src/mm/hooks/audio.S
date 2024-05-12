#include <combo.h>

/* Use our custom audio table */
PATCH_START 0x80190e14
  lui t1,%HI(gCustomAudioTable)
PATCH_END

PATCH_START 0x80190e24
  addiu t1,%LO(gCustomAudioTable)
PATCH_END

/* Use our custom seq -> bank table */
PATCH_START 0x80190e20
  lui t6,%HI(gCustomAudioSeqBanks)
PATCH_END

PATCH_START 0x80190e30
  addiu t6,%LO(gCustomAudioSeqBanks)
PATCH_END

/* Use our custom table init */
PATCH_START 0x80190e68
  jal Audio_InitCustom
PATCH_END

/* Rebase MM audio */
PATCH_START 0x80190e70
  la a1,0x20700 + MM_BASE
PATCH_END

PATCH_START 0x80190e84
  la a1,0x97f70 + MM_BASE
PATCH_END

