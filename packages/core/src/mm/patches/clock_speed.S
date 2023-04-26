#include <combo.h>

.macro CLOCK_SPEED group, speed, inverted
PATCH_GROUP \group

/* Clock speed setting */
PATCH_START 0x80130134
  lui     at,1
  bne     a3,v0,.+0x10
   addu   at,a2
  li      v0,0
  b       .+0x1c
   nop
  li      v0,\speed
  sh      v0,0x7006(at)
PATCH_END

PATCH_START 0x80119b1e
.short \inverted
PATCH_END

PATCH_START 0x8015764e
.short \inverted
PATCH_END

PATCH_GROUP_END
.endm

CLOCK_SPEED PG_MM_CLOCK_SPEED_VERYSLOW,    1,    0
CLOCK_SPEED PG_MM_CLOCK_SPEED_SLOW,        2,   -1
CLOCK_SPEED PG_MM_CLOCK_SPEED_FAST,        6,   -4
CLOCK_SPEED PG_MM_CLOCK_SPEED_VERYFAST,    9,   -6
CLOCK_SPEED PG_MM_CLOCK_SPEED_SUPERFAST,   18, -12
