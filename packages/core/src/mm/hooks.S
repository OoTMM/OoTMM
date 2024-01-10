#include <combo.h>

#define DMA_START (0x1a500 + MM_BASE)
#define DMA_END (0x20700 + MM_BASE)

/* Rebase MM's DMA Table */
PATCH_START 0x80080d14
  la a0, DMA_START
  la t6, DMA_END
PATCH_END

/* Downsize the heap by 0x40000 + 0x40000 malloc zone */
/* DEBUG */
PATCH_START 0x80174c4c
  lui t8,0x8070
PATCH_END

/* Not sure about this
PATCH_START 0x80165e34
  lui a0,0x8076
PATCH_END

PATCH_START 0x801789b0
  lui t9,0x8076
PATCH_END
*/

/* Hook in Main to load the payload */
PATCH_START 0x80080df8
  j LOADER_ADDR
   nop
PATCH_END

/* Skip the N64 logo */
PATCH_START 0x808006cc
  li t9, 1
PATCH_END

PATCH_START 0x80800134
  jr ra
  nop
PATCH_END

/* Skip the intro */
PATCH_START 0x8096b974
  li v0, 0xd
PATCH_END

PATCH_START 0x8096c50c
  nop
PATCH_END

/* Disable normal flash handler - we use our own one */
PATCH_START 0x80185d78
  li v0,0
PATCH_END

/* Mute the title screen "START" sound */
PATCH_START 0x8096c568
  nop
PATCH_END

/* Disable the first cycle ocarina check */
PATCH_START 0x8016a4b8
  li t2,0
PATCH_END

/* Epona B button fix */
PATCH_START 0x8011231c
  li t8,1
PATCH_END

/* Fix rspboot anticheat */
PATCH_START 0x80096a0c
  nop
PATCH_END

PATCH_START 0x80096a1c
  nop
PATCH_END

PATCH_START 0x80096a28
  nop
PATCH_END

PATCH_START 0x80096a34
  nop
PATCH_END

PATCH_START 0x80096a3c
  nop
PATCH_END

PATCH_START 0x80096a58
  nop
PATCH_END

PATCH_START 0x80096aa4
  li s0,1
PATCH_END

/*
;==================================================================================================
; Show Warp message when OoT Warp Song is played
;==================================================================================================
*/

/*
; Replaces:
;   LW      T4, 0x1F00 (S1)
;   LBU     V0, 0x0001 (T4)
*/
PATCH_START 0x80154A68
  jal     Ocarina_BeforeSongPlayingProcessed
   move   a0, s2
PATCH_END

/*
;==================================================================================================
; Check if OoT Warp Song is played.
;==================================================================================================
*/

Ocarina_CheckCustomSongs_Hook:
    addiu   sp, sp, -0x18
    sw      ra, 0x0014 (sp)
    sw      a0, 0x0018 (sp)

    HOOK_CALL Ocarina_CheckCustomSongs

    lw      v0, 0x0018 (sp)

    // Displaced code
    lbu     t3, 0x801FD463
    lbu     t5, 0x801FD464
    lui     s0, 0x8020
    lui     s5, 0x801D

    lw      ra, 0x0014 (sp)
    jr      ra
    addiu   sp, sp, 0x18

/*
; Replaces:
;   LBU     T3, 0xD463 (T3)
;   LBU     T5, 0xD464 (T5)
;   LUI     S0, 0x8020
;   LUI     S5, 0x801D
*/
PATCH_START 0x8019BD6C
  or      a0, ra, zero
  jal     Ocarina_CheckCustomSongs_Hook
  nop
  or      ra, v0, zero
PATCH_END

/*
;==================================================================================================
; Prevent OoT songs on Termina Field song wall
;==================================================================================================
*/

/*
; Replaces:
;   BEQZ    V1, 0x8019D398
*/
PATCH_START 0x8019D2D8
    nop
PATCH_END

/*
;==================================================================================================
; Handle different hookshot reticle distances
;==================================================================================================
*/

PATCH_START 0x80129160
    jal     Player_DrawHookshotReticleCustom
PATCH_END
