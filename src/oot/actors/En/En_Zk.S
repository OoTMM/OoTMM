#include <combo.h>

/* Disable the message check for zora tunic */
PATCH_START 0x80ad6454
  li t3,0x4012
PATCH_END

PATCH_START 0x80ad5e24
  li v0,0x4012
PATCH_END

/* Disable the trade quest timer */
PATCH_START 0x80ad6d1c
  li t9,0xff
PATCH_END

/* Skip Eyeball Frog Dialogue */
PATCH_START 0x80ad63d8
  li t7,2
  sh t7,0x1d0(s0)
PATCH_END

/* Skip check for adult trade slot */
PATCH_START 0x80ad63b0
  li t5,0x34
PATCH_END

/* Disable check for zora sapphire */
PATCH_START 0x80ad5d74
  li t7,0
PATCH_END

PATCH_START 0x80ad6408
  li t2,0
PATCH_END

/* Move instantly */
PATCH_START 0x80ad6ac4
  li v0,1
PATCH_END

/* Disable relocs */
PATCH_START 0x80ad71c4
.int 0
PATCH_END
