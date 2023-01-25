#include <combo.h>

/* Fix empty bomb */
EmptyBombFix:
  sb zero,0x141(v0)
  sb zero,0x142(v0)
  sb zero,0x144(v0)

  /* Replaced code */
  jr ra
   sw zero,0x11c(v0)

PATCH_START 0x80870ea4
  jal EmptyBombFix
PATCH_END
