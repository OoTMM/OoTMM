#include <combo.h>

PATCH_START _SpawnActorEx
  j comboSpawnActorEx
   nop
PATCH_END

.globl SpawnActorEx
SpawnActorEx:
  addiu sp,-0x38
  sw    ra,0x14(sp)
  j     _SpawnActorEx+0x8
   nop
