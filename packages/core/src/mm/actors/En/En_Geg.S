#include <combo.h>

/* Skip a check for the mask */
PATCH_START 0x80bb31e0
  li t7,0xff
PATCH_END

/* Cutscene QoLs */
PATCH_START 0x80BB2398
  nop
PATCH_END

PATCH_START 0x80BB340C
  lh      t8, 0x04AA(s0)
  addiu   t8, t8, -0x0001
  sh      t8, 0x04AA(s0)
PATCH_END

PATCH_START 0x80BB3420
  bnez    t8, .+0x28
PATCH_END

PATCH_START 0x80BB2598
  jal     DonGero_StartCutscene
PATCH_END

PATCH_START 0x80BB332C
  jal     DonGero_GetRollTarget
  lh      a0, 0x04D8 (s0)
  beqz    v0, .+0xE0
  nop
  addiu   a0, s0, 0x0024
  move    a1, v0
  nop
  nop
  nop
  nop
PATCH_END

PATCH_START 0x80BB3360
  jal     DonGero_GetRollTarget
  lh      a0, 0x04D8 (s0)
  move    a1, v0
  nop
  nop
  nop
  nop
PATCH_END

PATCH_START 0x80BB4428
.word 0x00000000
.word 0x00000000
.word 0x00000000
.word 0x00000000
PATCH_END

/* GI renumbering */
PATCH_START 0x80bb31e4
  li a2,GI_MM_MASK_DON_GERO
PATCH_END

PATCH_START 0x80bb3214
  li a2,GI_MM_RUPEE_PURPLE
PATCH_END

PATCH_START 0x80bb3220
  li a2,GI_MM_RUPEE_PURPLE
PATCH_END

PATCH_START 0x80bb325c
  li at,GI_MM_MASK_DON_GERO
PATCH_END
