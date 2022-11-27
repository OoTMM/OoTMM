#include <combo.h>

/* Use our own DMA table */
PATCH_REL_HILO(kComboDmaData)
.int 0x800808f4
.int 0x800808f8
.int 0x800809d4
.int 0x800809d8
.int 0x80080a44
.int 0x80080a48
.int 0x80080a9c
.int 0x80080aa0
.int 0x80080b00
.int 0x80080b04
.int 0x80080d24
.int 0x80080d28
.int 0x80080d34
.int 0x80080d38
PATCH_END

/* Avoid loading the DMA table twice */
PATCH_START 0x80080d2c
  nop
PATCH_END

PATCH_START 0x80080d3c
  li t7,0
PATCH_END

PATCH_START 0x80080d64
  nop
PATCH_END
