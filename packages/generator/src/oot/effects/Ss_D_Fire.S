#include <combo.h>

PATCH_START 0x80b2f024
  sw    a3,0x160(sp)
  addiu a0,a3,0x17a4
  lui   t0,1
  addu  a0,a0,t0
  jal   comboOotDodongoFireObject
   move a1,t6
  sltiu v1,v0,1
  subu  v1,zero,v1
  sw    v0,0x54(sp)
  move  v0,v1
  nop
  nop
  nop
PATCH_END
