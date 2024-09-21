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

/* Skip the pan out before gaining control of Link */
PATCH_START 0x80b05bbc
  li t9, 0x4
PATCH_END

PATCH_START 0x80b061c8
  li at, 0x1
PATCH_END
