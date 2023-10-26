#include <combo.h>

override_changelinkanimdefaultstop:
  addiu   sp, sp, -0x28
  sw      ra, 0x0024(sp)
  sw      a0, 0x0028(sp)
  sw      a1, 0x002c(sp)
  sw      a2, 0x0030(sp)
  jal     0x8008a194      #SkelAnime_GetFrameCount
  lw      a0, 0x0030(sp)
  mtc1    v0, $f4
  mtc1    zero, $f0
  addiu   t6, zero, 0x0002
  cvt.s.w $f6, $f4
  sw      t6, 0x0018(sp)
  lw      a0, 0x0028(sp)
  lw      a1, 0x002c(sp)
  lw      a2, 0x0030(sp)
  lui     a3, 0x3f80

  li      t0, 0x040032b0
  bne     a2, t0, skip   #if next animation is heavy rock pickup, change speed
  nop
  lui     a3, 0x4040       #3.0f
skip:
  swc1    $f6, 0x0014(sp)
  swc1    $f0, 0x0010(sp)
  jal     0x8008c000       #SkelAnime_ChangeLinkAnim
  swc1    $f0, 0x001c(sp)
  lw      ra, 0x0024(sp)
  lw      ra, 0x0024(sp)
  jr      ra
  addiu   sp, sp, 0x28

heavy_block_set_switch:
  addiu   a1, s0, 0x01a4 #displaced
  addiu   sp, sp, -0x20
  sw      ra, 0x14(sp)
  sw      a1, 0x18(sp)
  lh      a1, 0x1c(s1)
  sra     a1, a1, 8
  jal     0x800204d0     #set switch flag
  andi    a1, a1, 0x3f
  lw      a1, 0x18(sp)
  lw      ra, 0x14(sp)
  jr      ra
  addiu   sp, sp, 0x20

heavy_block_posrot:
  sw      t9, 0x66c(s0)  #displaced
  lw      t2, 0x428(s0)  #interactActor (block)
  lw      t1, gPlay 
  lw      t1, 0x1c44(t1)
  lh      t3, 0xB6(t2)   #block angle
  addi    t3, t3, 0x8000 #180 deg
  jr      ra
   sh      t3, 0xB6(t1)   #store to links angle to make him face block

heavy_block_set_link_action:
  lw      t0, gPlay 
  lw      t0, 0x1c44(t0)
  lb      t2, 0x0434(t0)
  li      t3, 0x08
  bne     t2, t3, return
  li      t1, 0x07       #action 7
  sb      t1, 0x0434(t0) #links action
return:
  jr      ra
   lwc1    $f6, 0x0c(s0)   #displaced

heavy_block_shorten_anim:
  lw      t0, gPlay 
  lw      t0, 0x1c44(t0)
  lw      t1, 0x01ac(t0)   #current animation
  li      t2, 0x04002f98   #heavy block lift animation
  bne     t1, t2, return1 #return if not heavy block lift
  lw      t3, 0x1bc(t0)    #current animation frame
  li      t4, 0x42cf0000
  bne     t3, t4, check_end
  li      t5, 0x43640000   #228.0f
  b       return1
  sw      t5, 0x1bc(t0)    #throw block
check_end:
  li      t4, 0x43790000   #249.0f
  bne     t3, t4, return1
  li      t1, 0x803a967c
  sw      t1, 0x664(t0)
return1:
  jr      ra
  addiu   a1, s0, 0x01A4   #displaced

/* Bg_Heavy_Block modifications */ 
PATCH_START 0x80941030 + 0xc9c
  nop
PATCH_END

PATCH_START 0x80941030 + 0xcb4
  nop
PATCH_END

PATCH_START 0x80941030 + 0xcd0
  jal     heavy_block_set_switch
PATCH_END

PATCH_START 0x80941030 + 0xee8
  jal     heavy_block_set_link_action
  swc1    $f4, 0x34(sp)
PATCH_END

PATCH_START 0x80941030 + 0x1040
  addiu      a1, zero, 0x1e
PATCH_END

PATCH_START 0x808301c0 + 0x80e8
  jal      heavy_block_posrot
  or       t9, t8, at
PATCH_END

PATCH_START 0x808301c0 + 0x14058
  jal    heavy_block_shorten_anim
  sw     a1, 0x34(sp)
PATCH_END

PATCH_START 0x808301c0 + 0x14128
  lui    at, 0x4218
PATCH_END

PATCH_START 0x808301c0 + 0x68
  jal     override_changelinkanimdefaultstop
PATCH_END