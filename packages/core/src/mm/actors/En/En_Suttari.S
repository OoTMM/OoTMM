#include <combo.h>

/*
  File:  Sakon - Actor
  Start: 0x80baa6d0 -- 0x100f810
  End:   0x80baef70 -- 0x101a0b0
  Size:  0x48a0
*/

/* References
 * https://github.com/ZoeyZolotova/mm-rando/blob/dev/assembly/c/Sakon.c
 * https://github.com/ZoeyZolotova/mm-rando/blob/dev/assembly/src/Speedups.asm#L108-L117
 * https://github.com/ZoeyZolotova/mm-rando/blob/dev/assembly/src/Speedups.Patch.asm#L39-L78
*/

Sakon_ShouldEndThiefEscape_Hook:
  addiu   sp, sp, -0x18
  sw      ra, 0x0014 (sp)

  jal     Sakon_ShouldEndThiefEscape
  or      a1, s1, zero

  lw      ra, 0x0014 (sp)
  jr      ra
  addiu   sp, sp, 0x18

PATCH_START 0x80baa6d0 + 0x2df4
  b .+0x8c
PATCH_END

PATCH_START 0x80baa6d0 + 0x2e80
  jal     Sakon_ShouldEndThiefEscape_Hook
  or      a0, s0, zero
  addiu   a0, s0, 0x0070
  beqz    v0, .+0x5c
  lui     a1, 0x4080
PATCH_END

PATCH_START 0x80baa6d0 + 0x2ec0
  nop
PATCH_END

PATCH_START 0x80baa6d0 + 0x2ee0
  addiu   a0, s0, 0x0070
  lui     a1, 0x4080
PATCH_END
