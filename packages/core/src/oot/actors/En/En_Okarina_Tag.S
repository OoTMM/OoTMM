#include <combo.h>

/* Disable windmill cutscene */
PATCH_START 0x80a873b0
  nop
PATCH_END

/* Disable temple of time cutscene */
PATCH_START 0x80a873cc
  la  t1,gSaveContext+0xed4+((EV_OOT_CHK_DOOR_TIME >> 4) * 2)
  lhu t2,(t1)
  ori t2,(1 << (EV_OOT_CHK_DOOR_TIME & 0xf))
  sh  t2,(t1)
PATCH_END

PATCH_START 0x80a873e4
  nop
PATCH_END

/* Disable royal tomb cutscene */
PATCH_START 0x80a8747c
  nop
PATCH_END

/* Disable relocs for the previous patch */
PATCH_START 0x80a8808c
.int 0
.int 0
PATCH_END
