#include <combo.h>

PATCH_START 0x80a8ba38
.int EnExRuppy_InitWrapper
PATCH_END

PATCH_START 0x80a8bc08
.int 0
PATCH_END

/*
Replaces:
  lh      a0, 0x148(s0)
  jal     Rupees_ChangeBy
   sw     v0, 0x4c(sp)
  jal     Sfx_PlaySfxCentered
   li     a0, 0x4803
  lw      v0, 0x4c(sp)
  or      a0, s0,zero
  lh      t0, 0x294(v0)
  addiu   t1, t0, 0x1
  jal     Actor_Kill
   sh     t1, 0x294(v0)
*/
PATCH_START 0x80a8b5c8
  or      a0, s0, zero
  lw      a1, 0x0054 (sp)
  jal     EnExRuppy_GiveItem
   or     a2, v0, zero
  nop
  nop
  nop
  nop
  nop
  nop
  nop
PATCH_END
