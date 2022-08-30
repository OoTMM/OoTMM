#include <combo/asm.h>

/* Randomize the given item */
PATCH_START 0x80a7f0f0
  jal comboGiveItem
PATCH_END

PATCH_START 0x80a7f16c
  jal comboGiveItem
PATCH_END
