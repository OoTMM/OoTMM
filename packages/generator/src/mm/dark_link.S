#include <combo.h>

.text
.balign 4
.globl DarkLink_OriginalHostileLockOn
.type DarkLink_OriginalHostileLockOn, @function
DarkLink_OriginalHostileLockOn:
  .word 0x8c820730, 0x5040000d, 0x8c820a74, 0x8c4e0004
  .word 0x24010005, 0x31cf0005, 0x55e10008, 0x8c820a74
  .word 0x8c980a74, 0x3c018000, 0x24020001, 0x0301c825
  .word 0x03e00008, 0xac990a74, 0x8c820a74, 0x00024000
  .word 0x0503000d, 0x00001025, 0x44802000, 0xc4860ad0
  .word 0x3c017fff, 0x3421ffff, 0x46062032, 0x00414824
  .word 0xac890a74, 0x45020004, 0x00001025, 0x848a00be
  .word 0xa48a0ad4, 0x00001025, 0x03e00008, 0x00000000
.size DarkLink_OriginalHostileLockOn, . - DarkLink_OriginalHostileLockOn

/* Pass the actor that burned or landed; native damage otherwise uses GET_PLAYER. */
Player_EnvironmentalDamageS0:
  j Player_InflictEnvironmentalDamage
   move a2,s0

Player_EnvironmentalDamageS1:
  j Player_InflictEnvironmentalDamage
   move a2,s1

Player_EnvironmentalDamageS4:
  j Player_InflictEnvironmentalDamage
   move a2,s4

PATCH_START 0x80834474
  jal Player_EnvironmentalDamageS4
PATCH_END

PATCH_START 0x80834634
  jal Player_EnvironmentalDamageS0
PATCH_END

PATCH_START 0x80836fe0
  jal Player_EnvironmentalDamageS1
PATCH_END

PATCH_START 0x808547b0
  jal Player_EnvironmentalDamageS0
PATCH_END

/* The fall and environmental damage hooks */
PATCH_START 0x8085ee38
  .int 0
PATCH_END

PATCH_START 0x8085ee74
  .int 0
PATCH_END

PATCH_START 0x8085ee88
  .int 0
PATCH_END

PATCH_START 0x8085ef9c
  .int 0
PATCH_END

PATCH_START 0x8085f008
  .int 0
PATCH_END

PATCH_START 0x8085f208
  .int 0
PATCH_END

PATCH_START 0x80861d6c
  .int 0
PATCH_END

PATCH_START 0x80861d70
  .int 0
PATCH_END
