#include <combo.h>

/* Use our draw wrapper */
PATCH_START 0x80acd63c
  lui t8,%HI(EnElforg_DrawWrapper)
PATCH_END

PATCH_START 0x80acd644
  addiu t8,%LO(EnElforg_DrawWrapper)
PATCH_END

PATCH_START 0x80acd738
  lui t9,%HI(EnElforg_DrawWrapper)
PATCH_END

PATCH_START 0x80acd744
  addiu t9,%LO(EnElforg_DrawWrapper)
PATCH_END

PATCH_START 0x80acda2c
.int EnElforg_DrawWrapper
PATCH_END

/* Remove relocs */
PATCH_START 0x80acdc40
.int 0
.int 0
PATCH_END

PATCH_START 0x80acdc64
.int 0
.int 0
PATCH_END

PATCH_START 0x80acdc84
.int 0
PATCH_END
