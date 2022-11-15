#include <combo.h>

/* Expand the GI buffer */
PATCH_START 0x808420c8
  li a0,0x6000
PATCH_END

/* Force GetItemCollectBehavior return value */
PATCH_START 0x8083d554
  li v0,0xff
PATCH_END

PATCH_START 0x8083d400
  li v0,0xff
PATCH_END

PATCH_START 0x8083d428
  li v0,0xff
PATCH_END

PATCH_START 0x8083d554
  li v0,0xff
PATCH_END

PATCH_START 0x8083d2a0
  li v0,0xff
PATCH_END
