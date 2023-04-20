#include <combo.h>

/* Do not remove fierce deity */
PATCH_START 0x80968938
  nop
PATCH_END

/* Remove check for fierce deity */
PATCH_START 0x8096a654
  li t7,0xfff
PATCH_END

/* Disable camera zooms */
PATCH_START 0x8096a27c
  nop
PATCH_END

/* Reroute majora dialogue before fight */
PATCH_START 0x8096a528
  nop
PATCH_END

/* Relocs */
PATCH_START 0x8096aedc
.int 0
PATCH_END

PATCH_START 0x8096aee8
.int 0
PATCH_END
