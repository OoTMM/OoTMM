#include <combo.h>

/* Do not turn into child immediately */
PATCH_START 0x80b0a7c0
  nop
PATCH_END

/* Fast cutscenes */
PATCH_START 0x80b06ad8
  nop
PATCH_END

PATCH_START 0x80b0690c
  nop
PATCH_END

PATCH_START 0x80b07a30
  b .+0x10
PATCH_END

/* Skip from cutscene state 0 to 4 */
PATCH_START 0x80b05bbc
  li t9, 0x4
PATCH_END

/* Force subCam to be 0 in subsequent cutscene states */
PATCH_START 0x80b05b98
  sh zero, 0x38e(s0)
PATCH_END

/* Reduces the amount of frames needed to wait for camera pan out */
PATCH_START 0x80b061c8
  li at, 0x1
PATCH_END

/* Nops all the subCam instructions for cutscene state 4 */
PATCH_START 0x80b061ec
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
PATCH_END
