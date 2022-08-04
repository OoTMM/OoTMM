#include <combo/asm.h>

/* Hook into Main, to load the payload */
PATCH_START(0x800005ac)
  jal 0x80006600
  nop
PATCH_END

/* Hook into ReadWriteSRAM */
PATCH_START(0x80091474)
  j comboReadWriteFlash
  nop
PATCH_END

/* Patch Play_Init */
PATCH_START(0x800f13e8)
  .long hookPlay_Init
PATCH_END


/* DEBUG: Replace rupy object with MM Deku Mask */
/*PATCH_START(0x800f9bf0)
.int 0x05999000
.int 0x0599ac30
PATCH_END*/
