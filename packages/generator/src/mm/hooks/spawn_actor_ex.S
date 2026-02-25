#include <combo.h>

PATCH_START Actor_SpawnAsChildAndCutscene
  j Actor_SpawnAsChildAndCutsceneWrapper
   nop
PATCH_END

.globl _Actor_SpawnAsChildAndCutscene
_Actor_SpawnAsChildAndCutscene:
  addiu sp,-0x38
  sw    ra,0x14(sp)
  j     Actor_SpawnAsChildAndCutscene+0x8
   nop
