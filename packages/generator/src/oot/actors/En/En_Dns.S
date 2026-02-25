#include <combo.h>

/* Override the scrub spawn flag */
/* Actually part of En_Shopnuts */
PATCH_START 0x80b40180
  nop
  nop
  nop
  nop
PATCH_END

EnShopnuts_AfterInit_Trampoline:
  move    a0,s0
  lw      s0,0x28(sp)
   j EnDns_MaybeDestroy
  addiu   sp,0x38

PATCH_START 0x80b4019c
  j     EnShopnuts_AfterInit_Trampoline
   nop
PATCH_END

/* Disable downgrading arrows to seeds */
PATCH_START 0x80a74cc4
  nop
PATCH_END
