#include <combo.h>

/* Skip intro */
PATCH_START 0x809d1f74
  li t8,1
PATCH_END

PATCH_START 0x809d2040
  li t8,1
PATCH_END

PATCH_START 0x809d2128
  li t7,1
PATCH_END

/* Skip cutscenes */
PATCH_START 0x809d774c
  li  at,0x3a2
  sh  at,0x142(s0)
PATCH_END

PATCH_START 0x809d7a24
  b .+0x38
PATCH_END
