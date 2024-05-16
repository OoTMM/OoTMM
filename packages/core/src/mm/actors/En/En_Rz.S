#include <combo.h>

/* GI renumbering */
PATCH_START 0x80bfc2d0
  li a2,GI_MM_HEART_PIECE
PATCH_END

PATCH_START 0x80bfc33c
  li a2,GI_MM_HEART_PIECE
PATCH_END

/* Cutscene skip */
PATCH_START 0x80bfc388
  jal EnRz_DanceCutscene
PATCH_END

PATCH_START 0x80bfce9c
.int 0
PATCH_END

PATCH_START 0x80bfbfac
  jr ra
   li v0,1
PATCH_END
