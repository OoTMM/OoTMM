#include <combo.h>

/* Fix the broken stone dlist */
PATCH_START 0x80b54ae8
  lui s6, %HI(gOriginalFieldKakeraDL)
PATCH_END

PATCH_START 0x80b54af4
  addiu s6, %LO(gOriginalFieldKakeraDL)
PATCH_END
