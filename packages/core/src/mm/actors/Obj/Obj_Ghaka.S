#include <combo.h>

/* Fast push */
PATCH_START 0x80b3c648
  li a2,3
PATCH_END


/*
Replaces:
  lbu        t4, 0x14b (a2)
  li         at, 0x1
  bnel       t4, at, .+0x1c
*/
PATCH_START 0x80b3c498
  jal       Player_HasStrength
   addiu    a0, zero, 0x0003
  beqzl     v0, .+0x4c
PATCH_END
