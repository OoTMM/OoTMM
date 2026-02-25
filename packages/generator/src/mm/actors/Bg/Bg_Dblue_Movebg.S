#include <combo.h>

/* Fast push */
PATCH_START 0x80a2a460
  li t9,30
PATCH_END

PATCH_START 0x80a2a468
  addiu t8,t7,2
PATCH_END

PATCH_START 0x80a2a814
  li t9,30
PATCH_END

PATCH_START 0x80a2a81c
  addiu t8,t7,2
PATCH_END
