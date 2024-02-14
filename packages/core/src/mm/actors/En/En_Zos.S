#include <combo.h>

/* Skip the evan cutscene */
PATCH_START 0x80bbbc6c
  la t5,0x80bbb354
PATCH_END

/* Reloc */
PATCH_START 0x80bbc930
.int 0
.int 0
PATCH_END

/* GI renumbering */
PATCH_START 0x80bbb3f0
  li a2,GI_MM_RUPEE_PURPLE
PATCH_END

PATCH_START 0x80bbb3f4
  li a2,GI_MM_HEART_PIECE
PATCH_END
