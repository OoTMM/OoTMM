#include <combo.h>

/*
==================================================================================================
 Handle Custom Songs
==================================================================================================
*/

/*
Replaces:
  beqz    v1, .+0x5c
*/
PATCH_START 0x800dded4
  beqz    v1, .+0x58
PATCH_END

/*
Replaces:
  bnel    v1, t8, .+0x4c
   lbu    t9, 0x0001 (a1)
*/
PATCH_START 0x800ddee8
  bnel    v1, t8, .+0x44
   nop
PATCH_END

Ocarina_BeforeSongPlayingProcessed_Hook:
  HOOK_SAVE
  jal   Ocarina_BeforeSongPlayingProcessed
   nop
  sw    v0, 0x003c (sp) // store into HOOK_SAVE's T9
  HOOK_RESTORE
  jr    ra
   nop

/*
Replaces:
  lw      a1, 0x62b8 (s2)
  lbu     t9, 0x0001 (a1)
*/
PATCH_START 0x800ddf2c
  jal     Ocarina_BeforeSongPlayingProcessed_Hook
   move   a0, s3
PATCH_END

Ocarina_CheckCustomSongs_Hook:
  addiu   sp,-0x18
  sw      ra,0x14(sp)
  sw      a0,0x18(sp)

  HOOK_CALL Ocarina_CheckCustomSongs

  lw      v0,0x18(sp)

  /* Displaced code */
  lbu     t3,0x80121f42
  lbu     t5,0x80121f43
  lui     s0,0x8012
  lui     t1,0x8010

  lw        ra,0x14(sp)
  jr        ra
   addiu    sp,0x18

PATCH_START 0x800c2860
  move    a0, ra
  jal     Ocarina_CheckCustomSongs_Hook
   nop
  move    ra,v0
PATCH_END

/* Call our button masking code */
PATCH_START 0x800c1f78
  j OcarinaMaskButtons
PATCH_END

/* Hook for handling sLastPlayedSong */
/*
Replaces:
  ADDIU   AT, R0, 0x0006
  BNEL    T3, AT, .+2C
  ADDIU   AT, R0, 0x0029
  LW      V0, 0x067C (A3)
  ADDIU   T4, R0, 0xFF20
  SH      T4, 0x0680 (A3)
  LW      T5, 0x0004 (V0)
  LUI     AT, 0x0001
  OR      T7, T5, AT
  SW      T7, 0x0004 (V0)
*/
PATCH_START 0x800e157c
  sw      a2, 0x0030 (sp)
  sw      a3, 0x0054 (sp)
  sw      t1, 0x0024 (sp)
  lw      a0, 0x0060 (sp)
  or      a1, a3, zero
  jal     Ocarina_HandleLastPlayedSong
  or      a2, t3, zero
  lw      a2, 0x0030 (sp)
  lw      a3, 0x0054 (sp)
  lw      t1, 0x0024 (sp)
PATCH_END
