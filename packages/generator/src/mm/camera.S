#include <combo.h>

.globl _Camera_SetFocalActor
_Camera_SetFocalActor:
  addiu sp, -0x18
  j Camera_SetFocalActor+0x8
   sw ra, 0x14(sp)

PATCH_START Camera_SetFocalActor
  j Camera_SetFocalActorHook
   nop
PATCH_END
