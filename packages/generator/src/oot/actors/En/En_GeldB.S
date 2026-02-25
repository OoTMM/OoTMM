#include <combo.h>

/* Use our hook */
EnGeldB_Caught_Trampoline:
  HOOK_CALL EnGe1_AfterCaught

  /* Replaced code */
  lbu t6,0xa(a0)
  lui v0,0x8012

  jr ra
   nop

PATCH_START 0x80b45954
  jal EnGeldB_Caught_Trampoline
   nop
PATCH_END

PATCH_START 0x80b4599c
  nop
PATCH_END

PATCH_START 0x80b459c8
  nop
PATCH_END

PATCH_START 0x80b459d8
  nop
PATCH_END

