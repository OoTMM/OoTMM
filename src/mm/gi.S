#include <combo/asm.h>

/* Hook LoadObjectGi */
PATCH_START(0x80838830)
  j hook_LoadObjectGi
   nop
PATCH_END

/* Use out extended table */
PATCH_REL_HILO(kExtendedGetItems)
.int 0x8082ece4
.int 0x8082ece8
.int 0x8083d288
.int 0x8083d28c
.int 0x8083d3e0
.int 0x8083d3e4
.int 0x8083d470
.int 0x8083d474
.int 0x8084831c
.int 0x80848320
.int 0x80853ab4
.int 0x80853b10
.int 0x80853c3c
.int 0x80853c48
PATCH_END

/* Zero the original GI table relocs */
PATCH_WRITE32(0x0)
.int 0x8085e818
.int 0x8085e81c
.int 0x8085fc08
.int 0x8085fc0c
.int 0x8085fc3c
.int 0x8085fc40
.int 0x8085fc44
.int 0x8085fc48
.int 0x808609b8
.int 0x808609bc
.int 0x80861c4c
.int 0x80861c50
.int 0x80861c64
.int 0x80861c68
PATCH_END
