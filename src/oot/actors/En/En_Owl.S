#include <combo.h>

/* Hook after Init */
Trampoline_EnOwl_AfterInit:
  move    a0,s0
  lw      s0,0x28(sp)
  j       EnOwl_AfterInit
   addiu  sp,0x50

PATCH_START 0x80aaf6f4
  j Trampoline_EnOwl_AfterInit
    nop
PATCH_END

/* Don't check the magic flag */
PATCH_START 0x80ab06a0
  li t6,1
PATCH_END
