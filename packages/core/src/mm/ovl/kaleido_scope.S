#include <combo.h>

/* Hook into out menu handler */
KaleidoScope_AfterSetCutsorColor_Trampoline:
  lw      a0,0x40(sp)
  lw      s0,0x14(sp)
  lw      s1,0x18(sp)
  j       KaleidoScope_AfterSetCutsorColor
   addiu  sp,0x40

PATCH_START 0x8081c674
  j KaleidoScope_AfterSetCutsorColor_Trampoline
   nop
PATCH_END

/* Relocs */
PATCH_START 0x8082cea4
.int 0
.int 0
PATCH_END

PATCH_START 0x8082cf50
.int 0
.int 0
PATCH_END

PATCH_START 0x8082d8f8
.int 0
PATCH_END

PATCH_START 0x8082d8fc
.int 0
.int 0
PATCH_END

/* Hide dummy sun song from quest screen */
KaleidoScope_HideDummySunSong:
  lui at,0x0002
  not at
  and s3,at
  li at,0x1
  jr ra
    and t8,v1,s3

PATCH_START 0x80816878
  jal  KaleidoScope_HideDummySunSong
   nop
PATCH_END

/*
==================================================================================================
 Handle OoT Hookshot name
==================================================================================================
*/

PATCH_START 0x80824818
  jal KaleidoScope_LoadNamedItemCustom
PATCH_END

PATCH_START 0x808160A0 + 0x1703c
.int 0
PATCH_END

PATCH_START 0x8081c604
  jal   KaleidoScope_ShowItemMessage
PATCH_END

PATCH_START 0x8081c5e4
  jal   KaleidoScope_ShowItemMessage
PATCH_END

KaleidoScope_LoadIcons_Hook:
  addiu   sp, sp, -0x20
  sw      ra, 0x0014 (sp)

  jal     KaleidoScope_LoadIcons
   addiu  a2, sp, 0x4c

  lw      ra, 0x0014 (sp)
  jr      ra
  addiu   sp, sp, 0x20

/*
Replaces:
  jal   CmpDma_LoadAllFiles
*/
PATCH_START 0x80828ac4
  jal   KaleidoScope_LoadIcons_Hook
PATCH_END

/*
==================================================================================================
 Pause Menu (Select Item subscreen) - Icon Draw
==================================================================================================
*/

KaleidoScope_DrawIconCustom_Hook:
  addiu   sp, sp, -0x20
  sw      ra, 0x001C (sp)

  // Parameter 5 (inventory slot index)
  sw      s1, 0x0010 (sp)

  // Parameter 6 (vertices index for Quadrangle)
  lw      at, 0x0030 (sp)
  sw      at, 0x0014 (sp)

  // Parameter 7 (Vertex buffer index)
  sw      s2, 0x0018 (sp)

  jal     KaleidoScope_DrawIconCustom
  // Overwrite A1 with item Id in V0 (we can calculate A1 with it later)
   or     a1, v0, zero

  lw      ra, 0x001C (sp)

  jr      ra
   addiu  sp, sp, 0x20

/*
Hook draw for item textures on "Select Item" screen.
Replaces:
  jal     KaleidoScope_DrawTexQuadRGBA32
*/
PATCH_START 0x8081BB70
  jal     KaleidoScope_DrawIconCustom_Hook
PATCH_END

/* Remove relocations. */
PATCH_START 0x8082C944
.int 0x00000000 // Removes: 0x44005AD0
PATCH_END

/*
==================================================================================================
 Pause Menu (Select Mask subscreen) - Icon Draw
==================================================================================================
*/

KaleidoScope_DrawMaskCustom_Hook:
  addiu   sp, sp, -0x20
  sw      ra, 0x001C (sp)

  // Parameter 5 (inventory slot index)
  addiu   at, s1, 0x0018
  sw      at, 0x0010 (sp)

  // Parameter 6 (vertices index for Quadrangle)
  lw      at, 0x0030 (sp)
  sw      at, 0x0014 (sp)

  // Parameter 7 (Vertex buffer index)
  sw      s4, 0x0018 (sp)

  jal     KaleidoScope_DrawIconCustom
  // Overwrite A1 with item Id in V0 (we can calculate A1 with it later)
   or     a1, v0, zero

  lw      ra, 0x001C (sp)

  jr      ra
   addiu  sp, sp, 0x20

/*
Hook draw for item textures on "Select Mask" screen.
Replaces:
  jal     KaleidoScope_DrawTexQuadRGBA32
*/
PATCH_START 0x80820458
  jal     KaleidoScope_DrawMaskCustom_Hook
PATCH_END

/* Remove relocations. */
PATCH_START 0x8082cc34
.int 0x00000000 // Removes: 0x4400a3b8
PATCH_END
