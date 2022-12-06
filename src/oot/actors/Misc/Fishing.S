#include <combo.h>

/* Accept smaller fish */
PATCH_START 0x80a42388
  lui at,0x4248
PATCH_END

PATCH_START 0x80a42404
  lui at,0x4248
PATCH_END

PATCH_START 0x80a42410
  lui at,0x4230
PATCH_END

PATCH_START 0x80a4247c
  lui at,0x4230
PATCH_END

/* Remove some fishing check */
PATCH_START 0x80a35160
  li v0,0
PATCH_END
