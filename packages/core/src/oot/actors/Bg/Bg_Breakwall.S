#include <combo.h>
/* Starts at VRAM 0x80900D00 - VROM 0xC9E4C0 */

// enable ice arrow collition on mud walls
PATCH_GROUP PG_OOT_BLUE_FIRE_ARROWS
  PATCH_START 0x80900D00 + 0xB76
  .byte 0x10
  PATCH_END
PATCH_GROUP_END
