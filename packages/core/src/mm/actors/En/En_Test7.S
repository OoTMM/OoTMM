#include <combo.h>

Ocarina_TriggerWarp_Hook:
    addiu   sp, sp, -0x18
    sw      ra, 0x0014 (sp)

    jal     EnTest7_TriggerWarp
     nop

    move    t9, v0

    /* Displaced Code */
    lui     v0, 0x801F
    addiu   v0, v0, 0xF670

    lw      ra, 0x0014 (sp)
    jr      ra
    addiu   sp, sp, 0x18

/* When Owl Warp entrance is set */
/*
Replaces:
  LH      T8, 0x00A4 (S1)
  ADDIU   V0, V0, 0xF670
  ADDIU   T9, R0, 0x2060
  BNE     T8, AT, 0x80AF2350 + 0x88
*/
PATCH_START 0x80af2350 + 0x68
  move    a0, s0
  jal     Ocarina_TriggerWarp_Hook
  move    a1, s1
  beqz    t9, . + 0x14 /* 0x80AF2350 + 0x88 */
PATCH_END

/* Allow skip soaring cutscene on button press */
PATCH_START 0x80AF1AF6
.half 0x2350 /* Replaces .half 0x1CA0 */
PATCH_END
