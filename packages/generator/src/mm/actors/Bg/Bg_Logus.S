#include <combo.h>

/*
Replaces:
  lui     t0, 0x801f
  lbu     t0, 0xf670 (t0)
  li      at, 0x0003
  li      t1, 0x0028
  beq     t0, at, .+0x28
   li     t5, 0x0001
*/
PATCH_START 0x80ad6a18
  or      a0, s0, zero
  jal     BgLotus_ShouldSink
   lw     a1, 0x003c (sp)
  beqz    v0, .+0x2c
   li     t5, 0x0001
  li      t1, 0x0028
PATCH_END
