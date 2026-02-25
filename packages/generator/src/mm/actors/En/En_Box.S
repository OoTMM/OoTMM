#include <combo.h>

/* Never spawn the stray fairy actor */
PATCH_START 0x80869128
  nop
PATCH_END

/* Disable a check for stray fairy */
PATCH_START 0x80868484
  li at,0xfff
PATCH_END

PATCH_START 0x80868e54
  li at,0xfff
PATCH_END

PATCH_START 0x808694dc
  li at,0xfff
PATCH_END

/* Use our handlers */
PATCH_START 0x80869b40
.int EnBox_InitWrapper
PATCH_END

PATCH_START 0x80869b4c
.int EnBox_DrawWrapper
PATCH_END

/* Remove relocs */
PATCH_START 0x80869d74
.int 0
PATCH_END

PATCH_START 0x80869d80
.int 0
PATCH_END
