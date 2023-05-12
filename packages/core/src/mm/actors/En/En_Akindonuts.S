#include <combo.h>

/* Prevent scrubs from actually moving */
PATCH_START 0x80bee984
  li t6,0
PATCH_END

PATCH_START 0x80bee9e8
  li t4,0
PATCH_END

PATCH_START 0x80beea4c
  li t2,0
PATCH_END

PATCH_START 0x80beeab0
  li t0,0
PATCH_END

/* Do not remove the last deed directly */
PATCH_START 0x80bed014
  nop
PATCH_END

/* Disable the flying away cutscene lock */
PATCH_START 0x80BEF6B8
  nop
PATCH_END

PATCH_START 0x80BEFCF0
  nop
PATCH_END
