#include <combo.h>

/* Remove a check for the biggoron sword flag */
PATCH_START 0x80b57b68
  li t6,0
PATCH_END

/* Remove link the goron flag checks */
PATCH_START 0x80b5a388
  li t3,0
PATCH_END

PATCH_START 0x80b5a3a0
  li t7,0
PATCH_END

/* Relocate the init */
PATCH_START 0x80b5bcec
.int EnGo2_InitWrapper
PATCH_END

/* Make gorons check for fire temple event */
PATCH_START 0x80b5a2ac
  lb t1,0xedc(a2)
PATCH_END

/* Make the rolling goron check work without a bomb bag */
PATCH_START 0x80b5715c
  nop
  nop
PATCH_END

/* Delete reloc */
PATCH_START 0x80b5c7d4
.int 0x0
PATCH_END

