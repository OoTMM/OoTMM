#include <combo.h>

/* Skip cutscenes */
PATCH_START 0x809d774c
  li  at,0x3a2
  sh  at,0x142(s0)
PATCH_END

PATCH_START 0x809d7a24
  b .+0x38
PATCH_END
