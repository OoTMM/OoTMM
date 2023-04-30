#include <combo.h>

/* Disable the env check for the clock town scrub */
PATCH_START 0x80add204
  nop
PATCH_END

/* Faster TF fly path and disable the flying away cutscene lock */
PATCH_START 0x80ADC4FC
  nop
PATCH_END

PATCH_START 0x80ADD200
  li t8,4
PATCH_END