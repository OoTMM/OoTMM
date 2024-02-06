#include <combo.h>

LoadRupeeColor:
  la t8,gInterfaceRupeeColor
  lw t8,0(t8)
  bnez t8,.+0xc
    nop
  li t6,0
  or t8,t6
  jr ra
   sw t8,0x4(a1)
PATCH_END

PATCH_START 0x8011f4a4
  jal LoadRupeeColor
   nop
PATCH_END
