#include <combo.h>

/* Accept smaller fish */
PATCH_START 0x80a42388
  lui at,0x4248
PATCH_END

PATCH_START 0x80a42404
  lui at,0x4248
PATCH_END

PATCH_START 0x80a42410
  lui at,0x4230
PATCH_END

PATCH_START 0x80a4247c
  lui at,0x4230
PATCH_END

/* Disable fishing anti-piracy */
PATCH_START 0x80a35160
  li v0,0
PATCH_END

/* Make fishing easier */
EasierFishing:
  lw      t2,(gSaveContext+0x04)
  bnez    t2,.Lafter
   andi   t8,t3,1
  bnez    t8,.Lret
   lui    t8, 0x4230
  lui     t8, 0x4250
  jr      ra
   nop

.Lafter:
  bnez t8,.Lret
  lui  t8,0x4210
  lui  t8,0x4238

.Lret:
  jr   ra
   nop

/* This doesn't work as intended, probably because of the hardcoded address */
FishBite:
  /* T0 needs to point to the fishing BSS */
  la        t0,(gActorOvl + 0x20 * AC_EN_FISHING)
  lw        t0,0x10(t0)
  li        at,0x139D0
  addu      t0,at

  lw        at,0x009C(t0)
  bnez      at,.LFishBite_ret
   mul.s    $f4,$f10,$f2
  lui       t0,0x3f80
  mtc1      t0,$f4
.LFishBite_ret:
  jr ra
   nop

PATCH_START 0x80a35908
  jal     EasierFishing
   lui    at,0x4282
  mtc1    at,$f8
  mtc1    t8,$f18
  swc1    $f18,0x019c(s2)
PATCH_END

PATCH_START 0x80a35964
  nop
PATCH_END

/* Ugly fix to circumvent an overlapping range (TODO: find a better way)*/
PATCH_GROUP PG_OOT_NOT_FISHING_POND_SHUFFLE
PATCH_START 0x80a35988
  nop
PATCH_END
PATCH_GROUP_END

PATCH_START 0x80a3d570
  jal     FishBite
   lwc1   $f10,0x0198(s0)
PATCH_END

PATCH_START 0x80A3C7E0
  nop
  nop
  nop
PATCH_END

PATCH_START 0x80a3ec80
  nop
PATCH_END

PATCH_START 0x80a3ec9c
  nop
PATCH_END

PATCH_START 0x80a3ecac
  nop
PATCH_END

PATCH_START 0x80A3ED08
  move at,t5
PATCH_END

/* Make sinking lure always available */
PATCH_START 0x80a393e0
  nop
PATCH_END

PATCH_START 0x80a393f0
  nop
PATCH_END

/* Don't reset lure position */
PATCH_START 0x80a42544
  nop
PATCH_END

PATCH_START 0x80a4254c
  nop
PATCH_END

PATCH_START 0x80a4260c
  nop
PATCH_END

PATCH_START 0x80a42614
  nop
PATCH_END

/* Don't reset lure - relocs */
PATCH_START 0x80a47640
.int 0
.int 0
PATCH_END

PATCH_START 0x80a47650
.int 0
.int 0
PATCH_END

/* Ignore sinking lure for prizes */
PATCH_START 0x80a4239c
  nop
  nop
PATCH_END

PATCH_START 0x80a423fc
  nop
  nop
PATCH_END

/* Display the normal text for the sinking lure */
PATCH_START 0x80a420b8
  li t8,0x4086
PATCH_END

PATCH_START 0x80A3CA0C
  li at,0x00c8
PATCH_END

PATCH_START 0x80A3CA20
  sh at,0x0192(s0)
PATCH_END

PATCH_START 0x80A3CA30
  swc1 $f6,0x01ac(s0)
PATCH_END

PATCH_GROUP PG_OOT_FISHING_POND_SHUFFLE

/*
Replaces:
  LUI       V0, 0x80A4 // Relocation
  ADDIU     V0, V0, 0x4A50 // Relocation
  LWC1      F0, 0x0000 (V0)
*/
PATCH_START 0x80A41F90
  lui       a0, 0x80A4
  jal       Fishing_CheckFishOnHand
  addiu     a0, a0, 0x7EEC
PATCH_END

/*
Replaces:
  NOP
*/
PATCH_START 0x80A41Fb8
  lw a0,0x34(sp)
PATCH_END

PATCH_START 0x80a4757c
.word     0x4600DA88
PATCH_END

/*
Replaces:
  LUI       T1, 0x80A4 // Relocation DD98
  LBU       T1, 0x7EEC (T1) // Relocation DD9C
  LUI       V0, 0x80A4 // Relocation DDA0
  LUI       A3, 0x80A4 // Relocation DDA4
  BNEZ      T1, 0x80A42618
  ADDIU     V0, V0, 0x4A50 // Relocation DDAC
  LUI       V0, 0x80A4 // Relocation DDB0
  ADDIU     V0, V0, 0x4A50 // Relocation DDB4
  LWC1      F6, 0x0000 (V0)
*/
PATCH_START 0x80a422a8
  lui       a0, 0x80A4
  jal       Fishing_TakeFishOnHand
  addiu     a0, a0, 0x7EEC
  lui       v0, 0x80A4
  lui       a3, 0x80A4
  lui       t1, 0x80A4
  lbu       t1, 0x7EEC (t1)
  bnez      t1, .+0x354
  addiu     v0, v0, 0x4A50
PATCH_END

PATCH_START 0x80a47604
  .word     0x4600DDA0
  .word     0x4500DDAC
  .word     0x4600DDB0
  .word     0x4500DDA8
PATCH_END

PATCH_START 0x80a47658
  .word     0x4500DDA4
  .word     0x4600DDB8
PATCH_END

/*
Replaces:
  SWC1      F6, 0x0000 (A3)
*/
PATCH_START 0x80a422d8
  swc1      $f0, 0x0000 (a3)
PATCH_END

/*
Replaces:
  ADDIU     T6, R0, 0x0014
  SH        T9, 0x010E (V0)
  SH        T6, 0x014C (V0)
  B         .+0x10
  LW        V0, 0x0030 (SP)
*/
PATCH_START 0x80a420dc
  sh        t9, 0x010E (v0)
  jal       Fishing_DiscardFish
   move     a0,v0
  b         .+0x10
   lw       a0,0x34(sp)
PATCH_END

/*
Replaces:
  BNEZL     T5, 0x80a3f198
*/
PATCH_START 0x80a3f0bc
  b         .+0xDC
PATCH_END

/*
Replaces:
  LUI       AT, 0x80A4
  ADDIU     T4, R0, 0x4099
  SH        T4, 0x7EF6 (AT)
  SB        R0, 0x01C5 (S0)
*/
PATCH_START 0x80a3f224
  lui       a1, 0x80A4 // Relocation
  addiu     a1, a1, 0x7EF6 // Relocation
  jal       Fishing_SetCaughtId
  or        a0, s0, zero
PATCH_END

PATCH_START 0x80a471c0
  .word     0x4600AD18
PATCH_END

/*
Replaces:
  BEQZL     AT, .+0x660
  SLTI      AT, V0, 0x0073
*/
PATCH_START 0x80a35140
  beqzl     at, .+0x65C
  nop
PATCH_END

/*
Replaces:
  SLTI      AT, V0, 0x0073
  BNEZ      AT, .+0x10
  ADDIU     AT, R0, 0x00C8
  BNE       V0, AT, .+0x50
*/
PATCH_START 0x80a3579c
  jal       Fishing_IsFishLoach
  or        a0, v0, zero
  nop
  bnez      v0, .+0x50
PATCH_END

/*
Replaces:
  LUI       T8, 0x80A4
  LBU       T8, 0x7EE5 (T8)
  ADDIU     AT, R0, 0x0001
  BNE       T8, AT, .+0x18
  LUI       AT, 0x80A4
  LWC1      F6, 0x542C (AT)
  LWC1      F4, 0x019C (S2)
  MUL.S     F8, F4, F6
  SWC1      F8, 0x019C (S2)
*/
PATCH_START 0x80a35968
  lui       a0, 0x80A4
  lbu       a0, 0x7EE5 (a0)
  nop
  nop
  lui       a1, 0x80A4
  lw        a1, 0x542C (a1)
  jal       Fishing_OverrideInitFishLength
  or        a2, s2, zero
  nop
PATCH_END

/*
Replaces:
  MTC1      R0, F6
  NOP
  C.EQ.S    F6, F0
  NOP
  BC1FL     .+0x54
*/
PATCH_START 0x80a3f3a0
  or        a0, s0, zero
  jal       Fishing_FishGiveItem
  or        a1, s1, zero
  b         .+0xE8
  nop
PATCH_END

/*
Replaces:
  ADDIU     AT, R0, 0x0004 // TEXT_STATE_CHOICE
  BEQ       V0, AT, .+0x18
  LW        A0, 0x004C (SP)
  JAL       0x800DD464 // Message_GetState
  NOP
  BNEZ      V0, .+0x6C
  NOP
  JAL       0x800D6110 // Message_ShouldAdvance
  OR        A0, S1, R0
  BEQZ      V0, .+0x5C
*/
PATCH_START 0x80a3f4b4
  addiu     at, zero, 0x0002 // TEXT_STATE_CLOSING
  beq       v0, at, .+0x10
  nop
  bnez      v0, .+0x74
  nop
  nop
  nop
  nop
  nop
  nop
PATCH_END

/*
Replaces:
  LUI       T4, 0x0001
  ADDU      T4, T4, S1
  LBU       T4, 0x04BD (T4)
  BEQZ      T4, .+0x38
  NOP
*/
PATCH_START 0x80a3f4e8
  jal       ActorDestroy
  or        a0, s0, zero
  nop
  b         .+0x38
  nop
PATCH_END

/*
Replaces:
  LBU       T8, 0x0140 (S0)
*/
PATCH_START 0x80a3c0b4
  lbu       t8, 0x0143 (s0)
PATCH_END

/*
Replaces:
  LBU       T1, 0x0140 (S0)
*/
PATCH_START 0x80a3f598
  lbu       t1, 0x0143 (s0)
PATCH_END

/*
Replaces:
  LBU       T6, 0x0140 (S0)
*/
PATCH_START 0x80a40530
  lbu       t6, 0x0143 (s0)
PATCH_END

PATCH_GROUP_END
