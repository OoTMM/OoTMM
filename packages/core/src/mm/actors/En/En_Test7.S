#include <combo.h>

/* Use our warp hander */
PATCH_START 0x80af1af0
  la a1,EnTest7_TriggerWarpWrapper
PATCH_END

/* Relocs */
PATCH_START 0x80af3698
.int 0
.int 0
PATCH_END
