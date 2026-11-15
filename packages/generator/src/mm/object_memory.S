#include <combo.h>

MmObjectBufferAlloc:
  addiu sp,sp,-0x28
  sw ra,0x24(sp)
  sw a0,0x10(sp)
  jal comboMmObjectBufferSize
   nop
  move a1,v0
  sw v0,0x4c(sp)
  lw a0,0x10(sp)
  jal 0x80172ac8
   nop
  lw ra,0x24(sp)
  jr ra
   addiu sp,sp,0x28

PATCH_START 0x8012f494
  jal MmObjectBufferAlloc
PATCH_END
