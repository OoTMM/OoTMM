#include <combo.h>

/* Expand link's chest item to 16 bits */
PATCH_LOAD_STORE 16, 0
.int 0x80022cd0
.int 0x80830450
/*.int 0x808312e8*/
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

/* Make link's drawGiId unsigned */
PATCH_LOAD_STORE 8, 1
.int 0x8007b438
.int 0x8007b44c
.int 0x8007bdfc
.int 0x8007be18
.int 0x8007bed0
.int 0x8007bf04
.int 0x8007bf80
.int 0x80831318
.int 0x8083131c
.int 0x80848b88
.int 0x8084c040
.int 0x8084d26c
.int 0x8084d2d0
.int 0x8084d2dc
.int 0x8084d360
PATCH_END

/* Always use a dummy text ID */
PATCH_START 0x8084c110
  li a1,0x01
PATCH_END

/* Hook into our custom item adding function */
PATCH_START 0x8084c11c
  sw v1,0x24(sp)
PATCH_END

PATCH_START 0x8084c128
  lw a1,0x24(sp)
PATCH_END

PATCH_START 0x8084c130
  jal comboPlayerAddItem
   nop
PATCH_END

/* Disable default sound on item get */
PATCH_START 0x8084c200
  nop
PATCH_END

PATCH_START 0x8084c19c
  nop
PATCH_END

/* Fix magic jars */
PATCH_START 0x8007181c
  li t7,0xfff
PATCH_END
