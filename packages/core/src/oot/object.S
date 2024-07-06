#include <combo.h>

PATCH_START _Object_GetSlot
  j comboObjectGetSlot
   nop
PATCH_END

.globl Object_GetSlot
Object_GetSlot:
  sll   a3,a1,0x10
  j     _Object_GetSlot+0x8
   sra  a3,a3,0x10

PATCH_START _Object_IsLoaded
  j comboObjectIsLoaded
   nop
PATCH_END

.globl Object_IsLoaded
Object_IsLoaded:
  sll   t6,a1,0x4
  j     _Object_IsLoaded+0x8
   addu t6,a1

/* Disable some actor draw code */
PATCH_START 0x800243b8
  nop
  nop
PATCH_END

PATCH_START 0x800243d8
  nop
PATCH_END

PATCH_START 0x800243ec
  nop
  nop
PATCH_END

PATCH_START 0x8002440c
  nop
PATCH_END

/* Hook after object loading */
PATCH_START 0x80081e84
  j comboAfterRoomObjectLoad
PATCH_END
