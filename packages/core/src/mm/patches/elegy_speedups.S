#include <combo.h>

Elegy_BlockSpeed_Hook:
  addiu   sp, sp, -0x18
  sw      ra, 0x0010 (sp)

  jal     Elegy_BlockSpeed
  move    a0, s0

  // Store result in actor field offset 0x70.
  swc1    $f0, 0x0070 (s0)

  lw      ra, 0x0010 (sp)
  jr      ra
  addiu   sp, sp, 0x18

Elegy_LockParams_Hook:
  // Displaced code
  move    a3, a1

  addiu   sp, sp, -0x20
  sw      ra, 0x0018 (sp)
  sw      a2, 0x0010 (sp)

  jal     Elegy_LockParams
  sw      a3, 0x0014 (sp)

  // Move hiword of result to T0.
  srl     t0, v0, 16
  // Move loword of result to T1.
  andi    t1, v0, 0xFFFF

  lw      a2, 0x0010 (sp)
  lw      a3, 0x0014 (sp)
  lw      ra, 0x0018 (sp)
  jr      ra
  addiu   sp, sp, 0x20

Elegy_DespawnCounter_Hook:
  addiu   sp, sp, -0x18
  sw      ra, 0x0014 (sp)
  sw      v1, 0x0010 (sp)

  move    a0, s1
  jal     Elegy_DespawnCounter
  move    a1, s0

  // Move result to T9.
  move    t9, v0

  lw      v1, 0x0010 (sp)
  lw      ra, 0x0014 (sp)
  jr      ra
  addiu   sp, sp, 0x18

Elegy_FadeSpeed_Hook:
  lw      at, 0x0044 (sp)

  addiu   sp, sp, -0x18
  sw      ra, 0x0014 (sp)
  sw      a1, 0x0010 (sp)

  move    a0, s0
  jal     Elegy_FadeSpeed
  move    a1, at

  lw      a1, 0x0010 (sp)
  lw      ra, 0x0014 (sp)
  jr      ra
  addiu   sp, sp, 0x18

Elegy_UpdateCamera_Hook:
  lw      at, 0x002C (sp)

  addiu   sp, sp, -0x20
  sw      ra, 0x0018 (sp)
  sw      a0, 0x0010 (sp)
  sw      a1, 0x0014 (sp)

  move    a0, a1
  jal     Elegy_UpdateCamera
  move    a1, at

  beqz    v0, no_update
  nop

  // Perform original call to update camera.
  lw      a0, 0x0010 (sp)
  jal     0x800f1d84 # HARDCODED
  lw      a1, 0x0014 (sp)

no_update:
  lw      ra, 0x0018 (sp)
  jr      ra
  addiu   sp, sp, 0x20

Elegy_Darken_Hook:
  lw      at, 0x002C (sp)

  addiu   sp, sp, -0x28
  sw      ra, 0x0024 (sp)
  swc1    $f2, 0x0010 (sp)
  sw      a0, 0x0014 (sp)
  sw      a1, 0x0018 (sp)
  sw      a2, 0x001C (sp)
  sw      a3, 0x0020 (sp)

  move    a0, s0
  jal     Elegy_Darken
  move    a1, at

  beqz    v0, no_darken
  nop

  // Perform original call to darken scene.
  lw      a0, 0x0014 (sp)
  lw      a1, 0x0018 (sp)
  lw      a2, 0x001C (sp)
  jal     0x800fd2b4  # HARDCODED
  lw      a3, 0x0020 (sp)

no_darken:
  lw      ra, 0x0024 (sp)
  jr      ra
  addiu   sp, sp, 0x28

Elegy_Update_Hook:
  // Displaced code
  move    s0, a0

  addiu   sp, sp, -0x18
  sw      ra, 0x0010 (sp)

  jal     Elegy_Update
  nop

  lw      ra, 0x0010 (sp)
  addiu   sp, sp, 0x18
  jr      ra
  lw      a1, 0x002C (sp)

PATCH_START 0x808a3400
  jal Elegy_FadeSpeed_Hook
  move    a1, v0
  addiu   a0, s0, 0x0192
  jal     0x800fef2c  # HARDCODED
  move    a2, v0
PATCH_END

PATCH_START 0x80a4c5d0
  sw      ra, 0x0024 (sp)
  sw      s0, 0x0020 (sp)
  jal     Elegy_Update_Hook
  sw      a1, 0x002c (sp)
PATCH_END

PATCH_START 0x80a4c734
  jal     Elegy_Darken_Hook
PATCH_END

PATCH_START 0x80a4c760
  jal     Elegy_UpdateCamera_Hook
PATCH_END

PATCH_START 0x80848684
  jal     Elegy_DespawnCounter_Hook
  lw      v1, 0x004c (sp)
  lh      t8, 0x00be (s0)
PATCH_END

PATCH_START 0x80855a84
  jal     Elegy_LockParams_Hook
  move    a2, a0
PATCH_END

PATCH_START 0x80855a98
  slt     v1, v0, t0
PATCH_END

PATCH_START 0x80855ACC
  addu    at, zero, t1
PATCH_END

PATCH_START 0x80bc4248
  lw      a1, 0x0024 (sp)
  jal     Elegy_BlockSpeed_Hook
  li      a2, 0
  lw      t6, 0x015C (s0)
  lw      v0, 0x0160 (s0)
PATCH_END

PATCH_START 0x80bc4468 // Offset: 0xAE8
  lw      a1, 0x0024 (sp)
  jal     Elegy_BlockSpeed_Hook
  li      a2, 1
  lw      v0, 0x0160 (s0)
  blez    v0, .+0x14 //0x80bc448c
  lui     t7, 0x80bc
PATCH_END

PATCH_START 0x80bc4754 
.word 0x45000afc
PATCH_END

// Song state - 

PATCH_START 0x80154CC8
  nop
  move    a0, s2
  jal     SongState_HandlePlayback
  move    a1, s0
  lui     at, 0x0001
  addu    at, at, s0
  srl     t2, v0, 24
  sb      t2, 0x1F22 (at)
  srl     t4, v0, 16
  sb      t4, 0x1F0A (at)
  srl     t6, v0, 8
  sb      t6, 0x2023 (at)
PATCH_END

PATCH_START 0x8015543C
  nop
  nop
  nop
  nop
PATCH_END