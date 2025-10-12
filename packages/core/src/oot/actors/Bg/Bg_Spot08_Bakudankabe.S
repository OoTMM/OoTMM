#include <combo.h>

/* Fix the broken stone dlist */
PATCH_START 0x80b54498
  lui s7, %HI(gOriginalFieldKakeraDL)
PATCH_END

PATCH_START 0x80b544a4
  addiu s7, %LO(gOriginalFieldKakeraDL)
PATCH_END
