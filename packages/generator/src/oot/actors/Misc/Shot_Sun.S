#include <combo.h>

.LShotSunTrampoline:
  lw  a0,0x60(sp)
  j ShotSun_GiveFireArrow
   move a1,s0

/* Hook into the Fire Arrow event */
PATCH_START 0x80b22a80
  jal .LShotSunTrampoline
PATCH_END

/* Alter the branch */
PATCH_START 0x80b22a94
  li at,1
PATCH_END

PATCH_START 0x80b22a9c
  move t9,v0
PATCH_END

PATCH_START 0x80b226c0 + 0x478
  nop
PATCH_END

/* Resize the actor */
PATCH_START 0x80b22cbc
.int 0x1A0
PATCH_END

PATCH_START 0x80b22cc0
.int ShotSun_InitWrapper
PATCH_END

PATCH_START 0x80b22d6c
.int 0
PATCH_END

/*
Replaces:
  addiu   a0, a1, 0x1c24
*/
PATCH_START 0x80b227f4
  or      a0, s0, zero
PATCH_END

/*
Replaces:
  jal     SpawnActor
*/
PATCH_START 0x80b22844
  jal     ShotSun_SpawnFairy
PATCH_END
