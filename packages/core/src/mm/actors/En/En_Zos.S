#include <combo.h>

/* Skip the evan cutscene */
PATCH_START 0x80bbbc6c
  lui t5,%HI(0x80bbb354)
  addiu t5,%LO(0x80bbb354)
PATCH_END

/* GI renumbering */
PATCH_START 0x80bbb3f0
  li a2,GI_MM_RUPEE_PURPLE
PATCH_END

PATCH_START 0x80bbb3f4
  li a2,GI_MM_HEART_PIECE
PATCH_END
