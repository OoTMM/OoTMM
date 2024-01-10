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

PATCH_START 0x80824738 + 0xe0
  jal   KaleidoScope_LoadNamedItemCustom
PATCH_END

// Reloc for above line
PATCH_START 0x808160A0 + 0x1703c
.int 0
PATCH_END

PATCH_START 0x8081c604
  jal   KaleidoScope_ShowItemMessage
PATCH_END

PATCH_START 0x8081c5e4
  jal   KaleidoScope_ShowItemMessage
PATCH_END
