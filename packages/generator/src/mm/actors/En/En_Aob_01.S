#include <combo.h>

/* Dog Race QoL */
PATCH_START 0x809c30e8
  lui     t8,0x801f
  lb      t8,-0x8f8(t8)
  bltz    t8,.+0x14
  li      t8,9
  li      a3,0xd
  beql    t8,a1,.+0x8
  li      a3,0
  andi    t8,a1,1
  beqz    t8,.+0x38
  nop
  sra     v1,a1,1
PATCH_END

/* GI renumbering */
PATCH_START 0x809c2bc0
  li a2,GI_MM_HEART_PIECE
PATCH_END

PATCH_START 0x809c2ba0
  li a2,GI_MM_RUPEE_RED
PATCH_END
