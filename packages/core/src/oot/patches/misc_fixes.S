#include <combo.h>

/* Makes the item screen default when pressing start the first time */
PATCH_START 0x8005B9DC
 b .+0x4c
PATCH_END

/* Fix a very long-standing vanilla bug where the minimap would corrupt on the first few frames when pausing, causing issues in rando */
PATCH_START 0x8006ca8a
.short 0x03
PATCH_END
