#include <combo.h>

EnToryo_DefaultText:
  li v0,0x200f
  li at,1
  sw at,0x1d4(a0)
  lw ra,0x14(sp)
  jr ra
   addiu sp,0x20

/* Skip dialogue */
PATCH_START 0x80a8c124
  li at,4
  sw at,0x1d4(a0)
PATCH_END

PATCH_START 0x80a8c154
  j EnToryo_DefaultText
PATCH_END

PATCH_START 0x80a8c37c
  nop
PATCH_END
