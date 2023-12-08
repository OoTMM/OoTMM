#include <combo.h>

/* Disable silver gauntlets warp */
PATCH_START(0x8084c22c)
  li at,0xffff
PATCH_END

/* Disable bounds check on gi */
PATCH_START(0x8083c6c0)
  xori at,v0,0x7e
PATCH_END

/* Disable the blue rupee fallback */
PATCH_START(0x8083c824)
  move at,a0
PATCH_END

/* Use out extended table */
PATCH_REL_HILO kExtendedGetItems
.int 0x808312ec
.int 0x808312f0
.int 0x808392b8
.int 0x808392c8
.int 0x8083c6d8
.int 0x8083c6dc
.int 0x8083c80c
.int 0x8083c810
.int 0x8084c0f8
.int 0x8084c0fc
.int 0x8084d25c
.int 0x8084d2a4
PATCH_END

/* Zero the original GI table relocs */
PATCH_WRITE32(0x0)
.int 0x808533f8
.int 0x808533fc
.int 0x80853f80
.int 0x80853f84
.int 0x808544ac
.int 0x808544b0
.int 0x808544e0
.int 0x808544e4
.int 0x80855c38
.int 0x80855c3c
.int 0x80855d88
.int 0x80855d8c
PATCH_END

/* Remove negative gi checks */
PATCH_START(0x8083130c)
  nop
PATCH_END

/* Force chest kicks */
PATCH_START(0x8083c938)
  li t2,-1
PATCH_END
