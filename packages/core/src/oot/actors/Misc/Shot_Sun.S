#include <combo.h>

.LShotSunTrampoline:
  lw  a0,0x60(sp)
  j ShotSun_GiveFireArrow
   move a1,s0

/* Hook into the Fire Arrow event */
PATCH_START 0x80b22a80
  jal .LShotSunTrampoline
PATCH_END

/* Alter the branch */
PATCH_START 0x80b22a94
  li at,1
PATCH_END

PATCH_START 0x80b22a9c
  move t9,v0
PATCH_END

PATCH_START 0x80b226c0 + 0x478
  nop
PATCH_END