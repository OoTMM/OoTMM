#include <combo.h>

/* Hijack after a set is spawned */
ObjGrassUnit_AfterSpawn_Trampoline:
  /* Replaced code */
  lwc1 $f18,0x10(s3)
  swc1 $f18,0x8(s2)

  j ObjGrassUnit_AfterSpawn
   move a0,s3

PATCH_START 0x809ac2e8
  jal ObjGrassUnit_AfterSpawn_Trampoline
   nop
PATCH_END
