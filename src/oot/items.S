#include <combo/asm.h>

/* Use our little hook */
PATCH_START(0x80022d08)
  jal hook_SetChestItemInRange
PATCH_END

/* Expand link's chest item to 16 bits */
PATCH_EXPAND16
.int 0x80022cd0
.int 0x80830450
.int 0x808312e8
.int 0x80831944
.int 0x80838584
.int 0x808385b8
.int 0x8083c64c
.int 0x8083c6b4
.int 0x8083c7b4
.int 0x8083c880
.int 0x8083d9ec
.int 0x808439e8
.int 0x8084562c
.int 0x80845630
.int 0x8084563c
.int 0x80845820
.int 0x808458a8
.int 0x8084c04c
.int 0x8084c0cc
.int 0x8084c13c
.int 0x8084c234
.int 0x8084c29c
.int 0x8084c8a8
PATCH_END
