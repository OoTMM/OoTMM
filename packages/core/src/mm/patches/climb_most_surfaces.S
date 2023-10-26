#include <combo.h>

PATCH_GROUP PG_MM_CLIMB_MOST_SURFACES
PATCH_START 0x8084c574
  nop
PATCH_END

PATCH_START 0x8083e3a0
.half 0x1000
PATCH_END

PATCH_START 0x8083d8d0
.half 0x8c46
PATCH_END

Player_GetCollisionType_Hook:
  addiu   sp, sp, -0x18
  sw      ra, 0x0014 (sp)

  jal     Player_GetCollisionType
  nop

  // Displaced code.
  li      t0, 0
  andi    t1, v0, 0x0008

  lw      ra, 0x0014 (sp)
  jr      ra
   addiu   sp, sp, 0x18

PATCH_START 0x8083D8D4
  jal     Player_GetCollisionType_Hook
   nop
PATCH_END
PATCH_GROUP_END
