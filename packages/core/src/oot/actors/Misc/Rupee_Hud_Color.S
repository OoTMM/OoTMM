#include <combo.h>

PATCH_START 0x8006d8e0 + 0x7f24
  addiu   t8, s1, 0x0008
  sw      t8, 0x02b0(s4)
  jal     Hud_RupeeIconColor
  sw      t9, 0x0000(s1)
  lhu     t4, 0x0252(s7)
  move    at, v0
PATCH_END