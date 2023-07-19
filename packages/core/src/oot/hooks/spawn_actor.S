#include <combo.h>

PATCH_START _SpawnActor
  j comboSpawnActor
   nop
PATCH_END

.globl SpawnActor
SpawnActor:
  addiu   sp,-0x58
  j       _SpawnActor+0x8
    sw    a2,0x60(sp)
