#include <combo.h>

ItemPreventEmptyEquip:
  sge at,t1,0xff
  beqz at,.Lret
  lbu v0,0x00(s1)
  li  v0,0xff

.Lret:
  jr    ra
   ori   at,zero,9

/* Item Menu - Left Check */
PATCH_START(0x80819d94)
  nop
PATCH_END

/* Item Menu - Right Check */
PATCH_START(0x80819e74)
  nop
PATCH_END

/* Item Menu - Bottom Check */
PATCH_START(0x8081a1dc)
  nop
PATCH_END

/* Item Menu - Top Check */
PATCH_START(0x8081a180)
  nop
PATCH_END

/* Item Menu - Disable "To Equip" on empty slots */
PATCH_START(0x8081a268)
  jal     ItemPreventEmptyEquip
   addu   s1,t6,t7
PATCH_END

/* Item Menu - Disable equipping items on empty slots */
PATCH_START(0x8081a2f0)
  jal     ItemPreventEmptyEquip
   nop
PATCH_END

deepHook_KaleidoSetCursorColor:
  HOOK_CALL KaleidoSetCursorColor
  jr ra
   nop

/* Use our color hook */
PATCH_START(0x8081f85c)
  jal deepHook_KaleidoSetCursorColor
  nop
PATCH_END

PATCH_START(0x8081f86c)
  li ra, 0x3c03c
PATCH_END

PATCH_START(0x8081f8d0)
  nop
PATCH_END

PATCH_START(0x8081f8e4)
  nop
  nop
PATCH_END

PATCH_START(0x8081f928)
  nop
PATCH_END

/* Disable AA and Divot in some cases */
PATCH_START 0x8009c5ac
  nop
PATCH_END
