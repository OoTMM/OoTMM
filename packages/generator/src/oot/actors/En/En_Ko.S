#include <combo.h>

/* Disable kokiri kid at forest */
PATCH_START 0x80ad41cc
  lui t4,0xffff
PATCH_END

/* Patch the odd mushroom kokiri spawn cond */
PATCH_START 0x80ad3a9c
  li v0,0
PATCH_END

/* Skip Poacher Saw dialogue */
PATCH_START 0x80ad3790
  li t1,2
PATCH_END

PATCH_START 0x80ad3798
  nop
  nop
PATCH_END

PATCH_START 0x80ad37a4
  sh t1,0x1d8(s0)
PATCH_END

PATCH_START 0x80ad4430
  nop
PATCH_END
