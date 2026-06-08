#include <combo.h>

/* GI renumbering */
PATCH_START 0x8088dfec
  li a2,GI_MM_SCOOP
PATCH_END

/* Resize the actor */

PATCH_START 0x808909cc
.int 0x280
PATCH_END

/* Init Wrapper */

PATCH_START 0x808909d0
.int EnElf_InitWrapper
PATCH_END

PATCH_START 0x80890fb4
.int 0
PATCH_END

/* Give item */

/*
Replaces:
  LW      A0, 0x003C (SP)
  JAL     0x80115908 Health_ChangeBy
   ADDIU  A1, R0, 0x0080
*/
PATCH_START 0x8088DEDC
  lw      a1, 0x003C (sp)
  jal     EnElf_GiveItem
   or     a0, s0, zero
PATCH_END

/*
Replaces:
  LUI     AT, 0x801F
  SH      R0, 0x0686 (AT)
*/
PATCH_START 0x8088DF2C
  nop
  nop
PATCH_END

/* Override health accumulator */

/*
Replaces:
  SH      T8, 0x35CA (AT)
  LH      T9, 0x0246 (S0)
  ADDIU   T0, T9, 0x0001
  B       .+0x30
   SH     T0, 0x0246 (S0)
*/
PATCH_START 0x8088E1B0
  lw      a1, 0x0034 (sp)
  jal     Fairy_SetHealthAccumulator
   or     a0, s0, zero
  b       .+0x30
   nop
PATCH_END

/* Override health SFX */

/*
Replaces:
  JAL     0x800B8EC8 Actor_PlaySfx
   ADDIU  A1, R0, 0x20A8 NA_SE_EV_FIATY_HEAL
*/
PATCH_START 0x8088E2E8
  jal     EnElf_PlayItemSfx
   lw     a1, 0x0034 (sp)
PATCH_END

/* Spawn of group of fairies */

/*
Replaces:
  lw      a0,0x3c(sp)
*/
PATCH_START 0x8088d264
  or      a0, s0, zero
PATCH_END

/*
Replaces:
  SW      V1, 0x0040 (SP)
*/
PATCH_START 0x8088D278
  sw      v1, 0x0028 (sp)
PATCH_END

/*
Replaces:
  JAL     0x800BAC60 Actor_Spawn
*/
PATCH_START 0x8088D28C
  jal     EnElf_SpawnFairyGroupMember
PATCH_END

/*
Replaces:
  LW      V1, 0x0040 (SP)
*/
PATCH_START 0x8088D294
  lw      v1, 0x0028 (sp)
PATCH_END

PATCH_START 0x8088ded4
  lui a2, 0x41c8
PATCH_END


/*

*/
PATCH_START 0x8088FFF0
  li at, 4
  or a0, s0, zero
  div zero, v0, at
  mfhi v0
  li at, 3
  beql v0, at, .+0x48
  lw t9, 0x0010(v1)
  beql v0, zero, .+0x08
  addiu v0, zero, 5
  jal 0x80151938
  addiu a1, v0, 0x0245
  b .+0xb8
  lw a0, 0x0030(sp)
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
PATCH_END
