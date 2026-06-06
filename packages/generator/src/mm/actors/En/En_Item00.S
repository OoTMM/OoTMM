#include <combo.h>

/* Resize */
PATCH_START 0x801adecc
.int 0x1b0
PATCH_END

/* Use our init */
PATCH_START 0x801aded0
.int EnItem00_InitWrapper
PATCH_END

UpdateEnd:
  /* Original */
  sh    zero,0x14a(s0)

  j EnItem00_SetXflagCollectedHandler
   move  a0,s0

/* Use our xflag post-collection handler */
PATCH_START 0x800a7108
  sh    zero,0x14a(s0)
  jal   EnItem00_SetXflagCollectedHandler
   move  a0,s0
PATCH_END

PATCH_START 0x800a6348
  nop
PATCH_END

/* Use our sound effect handler */
PATCH_START 0x800a7080
  jal EnItem00_PlaySoundXflag
   move a0,s0
PATCH_END

PATCH_START 0x800a5e54
  or    a0, s0, zero
  lw    a1, 0x004c (sp)
  addiu a2, sp, 0x003c
  addiu a3, sp, 0x0038
  addiu at, sp, 0x0030
  jal   EnItem00_InitType
   sw   at, 0x0010 (sp)
  b     .+0x2a0
   nop
PATCH_END

PATCH_START 0x800a6198
  mtc1    zero, $f0
  sh      t1, 0x0152 (s0)
  sh      t2, 0x014c (s0)
  swc1    $f0, 0x0070 (s0)
  swc1    $f0, 0x0068 (s0)
  swc1    $f0, 0x0074 (s0)
  or      a0, s0, zero
  lw      a1, 0x004c (sp)
  jal     EnItem00_InitGiveItem
   addiu  a2, sp, 0x0034
  b       .+0x154
   nop
PATCH_END

PATCH_START 0x800a6e0c
  or      a0, s0, zero
  lw      a1, 0x004c (sp)
  jal     EnItem00_UpdateGiveItem
   addiu  a2, sp, 0x0034
  b       .+0x1bc
   lw     a2, 0x004c (sp)
PATCH_END

PATCH_START 0x800a6898
  jal     EnItem00_SetSpawningRotation
   or     a0, s0, zero
  b       .+0x50
   nop
PATCH_END
