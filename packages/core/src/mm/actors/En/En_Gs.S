#include <combo.h>

/* Force the default gossip message */
PATCH_START 0x80997e10
  li t6,0x20d0
PATCH_END

/* Alter the param2 of the message call */
PATCH_START 0x80997e9c
  move a1,a2
PATCH_END

/* GI renumbering */
PATCH_START 0x809988c4
  li  t6,GI_MM_RUPEE_SILVER
PATCH_END

PATCH_START 0x8099891c
  li  t8,GI_MM_RUPEE_SILVER
PATCH_END

PATCH_START 0x809988f0
  li  t3,GI_MM_RUPEE_SILVER
PATCH_END

PATCH_START 0x80998940
  li  t5,GI_MM_HEART_PIECE
PATCH_END
