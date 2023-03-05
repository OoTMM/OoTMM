#include <combo.h>

/* Hook into Main, to load the payload */
PATCH_START 0x800005ac
  jal LOADER_ADDR
   nop
PATCH_END

/* Hook into ReadWriteSRAM */
PATCH_START 0x80091474
  j comboReadWriteFlash
   nop
PATCH_END

/* Make adult start with no items equipped */
PATCH_START 0x8006f8bc
  li v0,0xff
PATCH_END

PATCH_START 0x8006f898
  li t6,0xff
PATCH_END

PATCH_START 0x8006f8d8
  sb v0,0x6b(a1)
PATCH_END

PATCH_START 0x8006f888
  li t2,0xff
PATCH_END

/* Prevent eggs from hatching */
PATCH_START 0x80061bfc
  li v0,0
PATCH_END

PATCH_START 0x80061c10
  li v0,0
PATCH_END
