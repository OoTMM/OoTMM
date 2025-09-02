#include <combo.h>

/* Disable 64dd entierely */
PATCH_START 0x800ad410
  jr ra
   nop
PATCH_END
