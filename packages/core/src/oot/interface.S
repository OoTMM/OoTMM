#include <combo.h>

LoadRupeeColor:
  la t6,gInterfaceRupeeColor
  lw t6,0(t6)
  bnez t6,.+0xc
    nop
  li t5,0
  or t6,t5
  jr ra
   sw t6,0x4(s1)
PATCH_END

PATCH_START 0x80075824
  jal LoadRupeeColor
   nop
PATCH_END
