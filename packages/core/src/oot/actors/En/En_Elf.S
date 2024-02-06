#include <combo.h>

/* Resize the actor */
PATCH_START 0x80889e7c
.int 0x2cc
PATCH_END

PATCH_START 0x80889e80
.int EnElf_InitWrapper
PATCH_END

PATCH_START 0x8088a44c
.int 0
PATCH_END

/*
Replaces:
  lw      a0,0x40(sp)
*/
PATCH_START 0x80886728
  or      a0, s0, zero
PATCH_END

/*
Replaces:
  sw      v1, 0x0044 (sp)
*/
PATCH_START 0x8088673c
  sw      v1, 0x0028 (sp)
PATCH_END

/*
Replaces:
  jal     SpawnActor
*/
PATCH_START 0x80886750
  jal     EnElf_SpawnFairyGroupMember
PATCH_END

/*
Replaces:
  jal     Player_InCsMode
*/
PATCH_START 0x8088726c
  jal     EnElf_CantGiveItem
PATCH_END

/*
Replaces:
  lw      v1, 0x0044 (sp)
*/
PATCH_START 0x80886758
  lw      v1, 0x0028 (sp)
PATCH_END

/*
Replaces:
  LW      A0, 0x003C (SP)
  JAL     Health_ChangeBy
   ADDIU  A1, R0, 0x0080
*/
PATCH_START 0x808872cc
  lw      a1, 0x003C (sp)
  jal     EnElf_GiveItem
   or     a0, s0, zero
PATCH_END

/*
Replaces:
  JAL     Magic_Refill
*/
PATCH_START 0x808872e8
  nop
PATCH_END
