#include <combo.h>

/* Hook for rupee color */
LoadRupeeColor:
  la t6,gInterfaceRupeeColor
  lw t6,0(t6)
  bnez t6,.+0xc
    nop
  li t5,0
  or t6,t5
  jr ra
   sw t6,0x4(s1)

PATCH_START 0x80075824
  jal LoadRupeeColor
   nop
PATCH_END

/* Hook for moving item icon */
LoadEquipItemTexture_Trampoline:
  HOOK_CALL LoadEquipItemTexture
  jr ra
   nop

PATCH_START 0x80076828
 jal LoadEquipItemTexture_Trampoline
  nop
.rept 0x33
  nop
.endr
PATCH_END

/* Use our own update button wrapper */
.globl Interface_UpdateButtonsPart2
Interface_UpdateButtonsPart2:
  addiu sp,-0x38
  j     _Interface_UpdateButtonsPart2+0x08
   lui  a2,0x8012

PATCH_START _Interface_UpdateButtonsPart2
  j Interface_UpdateButtonsPart2Wrapper
   nop
PATCH_END
