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

/* Remove check for buying the bomb bag */
PATCH_START 0x80bed2a4
  nop
PATCH_END

PATCH_START 0x80bed2ac
  nop
  nop
PATCH_END

PATCH_START 0x80bed2bc
  nop
PATCH_END

PATCH_START 0x80bed2c4
  nop
  nop
  nop
PATCH_END

/* Do not remove the last deed directly */
PATCH_START 0x80bed014
  nop
PATCH_END

/* Disable the flying away cutscene lock */
PATCH_START 0x80BEF6B8
  nop
PATCH_END

PATCH_START 0x80ADC4FC
  nop
PATCH_END