#include <combo.h>

/* Hook into Main, to load the payload */
PATCH_START 0x80000f4c
  j LOADER_ADDR
   nop
PATCH_END

/* Hook into ReadWriteSRAM */
PATCH_START 0x80091474
  j Flash_ReadWrite
   nop
PATCH_END

/* Make adult start with no items equipped */
PATCH_START 0x8006f8bc
  li v0,0xff
PATCH_END

PATCH_START 0x8006f898
  li t6,0xff
PATCH_END

PATCH_START 0x8006f8d8
  sb v0,0x6b(a1)
PATCH_END

PATCH_START 0x8006f888
  li t2,0xff
PATCH_END

/* Prevent eggs from hatching */
PATCH_START 0x80061bfc
  li v0,0
PATCH_END

PATCH_START 0x80061c10
  li v0,0
PATCH_END

/* Make Navi always return the same message */
PATCH_START 0x8005977c
  jr ra
   li v0,0x0141
PATCH_END

/* Make using song of double time correctly trigger the cucco in the morning */
PATCH_START 0x8009ab88
  beq v0, at, .+0x10
   ori at, zero, 0x4555
  bne v0, at, .+0x64
   ori t6, zero, 0xfffd
  lw t9, 0x0014 (s2)
  lw t2, 0x0018 (s2)
  addiu t4, zero, 0x0001
  addiu t1, t9, 0x0001
  addiu t3, t2, 0x0001
  sw t1, 0x0014 (s2)
  sw t3, 0x0018 (s2)
  sb t4, 0x141c (s2)
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
PATCH_END
