#include <combo/asm.h>

/* Replace SaveGame */
PATCH_START 0x800905d4
  j comboWriteSave
   nop
PATCH_END
