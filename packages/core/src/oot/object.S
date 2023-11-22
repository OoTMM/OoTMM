#include <combo.h>

PATCH_START _GetObjectSlot
  j comboGetObjectSlot
   nop
PATCH_END

.globl GetObjectSlot
GetObjectSlot:
  sll   a3,a1,0x10
  j     _GetObjectSlot+0x8
   sra  a3,a3,0x10

PATCH_START _IsObjectSlotLoaded
  j comboIsObjectSlotLoaded
   nop
PATCH_END

.globl IsObjectSlotLoaded
IsObjectSlotLoaded:
  sll   t6,a1,0x4
  j     _IsObjectSlotLoaded+0x8
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
