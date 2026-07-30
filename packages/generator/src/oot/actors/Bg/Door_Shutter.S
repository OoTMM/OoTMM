#include <combo.h>

PATCH_GROUP PG_OOT_SKIP_CUTSCENES

// Disable the waiting time for the bars to raise
PATCH_START 0x808b904c
  li t6, 0
PATCH_END

PATCH_START 0x808b8bb4
  li t6, 0
PATCH_END

PATCH_GROUP_END