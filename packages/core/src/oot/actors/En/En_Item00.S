#include <combo.h>

/* Resize Item00 */
PATCH_START 0x800e775c
.int 0x1a8
PATCH_END

/* Hook the functions */
PATCH_START 0x800e7760
.int EnItem00_InitWrapper
PATCH_END

PATCH_START 0x800e7768
.int EnItem00_UpdateWrapper
PATCH_END

/* Make small keys use the same draw function as heart pieces */
PATCH_START 0x80105fe0
.int 0x80013014
PATCH_END

/* DrawGI IDs */
PATCH_START 0x800130d8
  li a1,(DRAWGI_RECOVERY_HEART - 1)
PATCH_END

PATCH_START 0x80013100
  li a1,(DRAWGI_OOT_SHIELD_DEKU - 1)
PATCH_END

PATCH_START 0x80013114
  li a1,(DRAWGI_OOT_SHIELD_HYLIAN - 1)
PATCH_END

PATCH_START 0x80013128
  li a1,(DRAWGI_OOT_TUNIC_ZORA - 1)
PATCH_END

PATCH_START 0x8001313c
  li a1,(DRAWGI_OOT_TUNIC_GORON - 1)
PATCH_END

PATCH_START 0x80012174
  nop
PATCH_END

/* Check for extended objects before writing the flag */
PATCH_START 0x80012f50
  jal EnItem00_SetExtendedFlag
   move a0,s0
PATCH_END

/* Handle the give item for extended objects */
PATCH_START 0x80012cb0
  jal EnItem00_GiveExtendedOrRupee
   move a1,s0
PATCH_END

/* Patch the sound effect */
PATCH_START 0x80012ee0
  jal EnItem00_PlayExtendedCollectedSound
   move a0,s0
PATCH_END

/* Use our custom post-collection handler */
PATCH_START 0x80012f90
  lui a1,%HI(EnItem00_ExtendedCollectedHandler)
PATCH_END

PATCH_START 0x80012f98
  addiu a1,%LO(EnItem00_ExtendedCollectedHandler)
PATCH_END
