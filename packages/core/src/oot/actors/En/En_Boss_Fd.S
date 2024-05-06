#include <combo.h>

volvagia_flying_hitbox:

    /* Displaced code */
    addiu   a2, zero, 0x0004
    andi    t6, a1, 0x0002
    lb      t7, 0x00af(s0) /* this->actor.colChkInfo.health */
    bnel    t7, zero, return
     nop
    addiu   t6, zero, 0x0000 /* if volvagia has 0 health, cancel the collision check */

return:
    jr      ra
     nop

PATCH_START 0x8094cd8c
  jal volvagia_flying_hitbox
   nop
PATCH_END