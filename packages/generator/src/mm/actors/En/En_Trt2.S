#include <combo.h>

/* GI renumbering */
PATCH_START 0x80ad4070
  li a2,GI_NOTHING
PATCH_END

PATCH_START 0x80ad408c
  li a2,GI_MM_BOTTLE_POTION_RED
PATCH_END

/* Do not check for empty bottles */
PATCH_START 0x80ad3df8
  li a0,1
PATCH_END

PATCH_START 0x80ad3e70
  li a0,1
PATCH_END

PATCH_START 0x80ad3f2c
  li a0,1
PATCH_END
