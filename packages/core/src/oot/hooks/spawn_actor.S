#include <combo.h>

PATCH_START Actor_Spawn
  j Actor_SpawnWrapper
   nop
PATCH_END

.globl _Actor_Spawn
_Actor_Spawn:
  addiu   sp,-0x58
  j       Actor_Spawn+0x8
    sw    a2,0x60(sp)
