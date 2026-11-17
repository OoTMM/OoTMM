#include <combo.h>

PATCH_START Object_GetSlot
  j Object_GetSlotWrapper
   nop
PATCH_END

.globl _Object_GetSlot
_Object_GetSlot:
  sll a3,a1,0x10
  j Object_GetSlot+0x8
   sra a3,a3,0x10

PATCH_START Object_IsLoaded
  j Object_IsLoadedWrapper
   nop
PATCH_END

.globl _Object_IsLoaded
_Object_IsLoaded:
  sll t6,a1,0x4
  j Object_IsLoaded+0x8
   addu t6,t6,a1

PATCH_START 0x800b9b6c
  lui t9,%hi(gSegments+0x18)
  lw t9,%lo(gSegments+0x18)(t9)
  lui t5,0x8000
  or t9,t9,t5
  nop
  nop
PATCH_END

PATCH_START 0x800b9ba0
  lui t5,%hi(gSegments+0x18)
  lw t5,%lo(gSegments+0x18)(t5)
  addiu a1,sp,0x2c
  lui t1,0x8000
  or t5,t5,t1
  nop
  nop
PATCH_END

.macro enemy_effect_segment play, slot, result
  addiu sp,sp,-0x38
  sw ra,0x34(sp)
  sw a0,0x10(sp)
  sw a1,0x14(sp)
  sw a2,0x18(sp)
  sw a3,0x1c(sp)
  sw v0,0x20(sp)
  sw t4,0x24(sp)
  move a0,\play
  jal comboMmEnemyEffectSegment
   move a1,\slot
  move \result,v0
  lw t4,0x24(sp)
  lw v0,0x20(sp)
  lw a3,0x1c(sp)
  lw a2,0x18(sp)
  lw a1,0x14(sp)
  lw a0,0x10(sp)
  lw ra,0x34(sp)
  jr ra
   addiu sp,sp,0x38
.endm

mmHahenSegment:
  enemy_effect_segment a3,t9,t2

mmKakeraXluSegment:
  enemy_effect_segment a0,t5,t8

mmKakeraOpaSegment:
  enemy_effect_segment a0,t1,t2

mmIceBlockSegment:
  enemy_effect_segment s1,t9,t2

mmDodongoFireSegment:
  enemy_effect_segment t6,t7,t0

.macro enemy_fragment_segment address, trampoline, result, skip
PATCH_START \address
  jal \trampoline
   nop
  bnez \result,9f
   nop
  sw zero,0(v0)
  b .+(\skip-\address-0x14)
   sw zero,4(v0)
9:
  sw \result,4(v0)
PATCH_END
.endm

enemy_fragment_segment 0x8097b464, mmHahenSegment, t2, 0x8097b4f8
enemy_fragment_segment 0x8097b544, mmHahenSegment, t2, 0x8097b5d8
enemy_fragment_segment 0x8097def8, mmKakeraXluSegment, t8, 0x8097e11c
enemy_fragment_segment 0x8097df3c, mmKakeraOpaSegment, t2, 0x8097e11c

PATCH_START 0x8097934c
  jal mmDodongoFireSegment
   nop
  beqz t0,.+(0x809794c0-0x80979354)
   move a0,t0
  lui at,0x8000
  nop
  nop
PATCH_END

enemy_fragment_segment 0x80a22f14, mmIceBlockSegment, t2, 0x80a22fcc
