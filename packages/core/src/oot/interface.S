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

/* Handle swordless better */
/* FIXME: Doesn't work very well, hence the PG */
PATCH_GROUP PG_OOT_SWORDLESS_ADULT
PATCH_START 0x8006e400
  nop
  nop
PATCH_END

/* Allow using bow on epona while swordless */
PATCH_START 0x8006ea14
  nop
PATCH_END

PATCH_START 0x8006e450
  nop
  nop
PATCH_END
PATCH_GROUP_END

/* Don't use the default rupee delta code */
PATCH_START 0x80078748
  b .+0x158
PATCH_END

/* Use our wrapper */
PATCH_START 0x8009bef8
  jal Interface_Update
PATCH_END
