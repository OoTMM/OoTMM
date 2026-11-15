#include <combo.h>

/* Use our hook */
EnWonderTalk2_AfterInit_Trampoline:
  move    a0,a2
  lw      a1,0x1c(sp)

  j       EnWonderTalk2_AfterInit
   addiu  sp,0x18

PATCH_START 0x80b23494
  j EnWonderTalk2_AfterInit_Trampoline
   nop
PATCH_END

.set push
.set noreorder
PATCH_START 0x80b233fc
  lw ra,0x14(sp)
  move a0,a2
  j Actor_Kill
   addiu sp,sp,0x18
PATCH_END
.set pop
