#include <combo.h>

/* Fix a vanilla bug that can cause a crash with some custom songs */
PATCH_START 0x80198ffc
  move t6,t5
PATCH_END

/* Make Kamaro use SEQ_PLAYER_BGM_SUB instead of SEQ_PLAYER_BGM_MAIN */
PATCH_START 0x800b9e14
  li a0, 3
PATCH_END

/* Let Final Hours preservation handling run for negative-scene entrances. */
PATCH_START 0x80166D38
    nop
PATCH_END

/* Allow positional actor music without incorrectly blocking SUB-BGM. */
PATCH_START 0x801A1FB4
    j       Audio_PlaySequenceAtPos_Hook
     nop
PATCH_END

/* Prevent positional music cleanup from stopping Final Hours. */
PATCH_START 0x801A2090
    j       Audio_UpdateSequenceAtPos_Hook
     nop
PATCH_END

PATCH_START 0x801a2e54
  j Audio_StoreBgmWithoutMiniboss
   nop
PATCH_END

.set push
.set noreorder
Audio_StoreBgmWithoutMiniboss:
  andi t0,a0,0xff
  addiu t1,zero,0x38
  bne t0,t1,2f
   nop
  addiu sp,sp,-0x20
  sw ra,0x14(sp)
  sw a0,0x18(sp)
  jal comboEnemyMusicSuppressed
   nop
  lw a0,0x18(sp)
  lw ra,0x14(sp)
  bnez v0,1f
   addiu sp,sp,0x20
2:
  addiu sp,sp,-0x20
  j 0x801a2e5c
   sw ra,0x14(sp)
1:
  jr ra
   nop
.set pop

PATCH_START 0x8019ee38
  j Audio_ApplyImportedEnemyFilter
   nop
PATCH_END

.set push
.set noreorder
Audio_ApplyImportedEnemyFilter:
  li t9,0x20
  bne a0,t9,1f
   nop
  bne t2,t9,1f
   lhu t3,0x22(a3)
  andi t3,t3,0xf3ff
  addiu t3,t3,-0x32e0
  sltiu t3,t3,0x120
  bnez t3,2f
   nop
1:
  sll t2,t2,1
2:
  j 0x8019ee84
   andi t2,t2,0xff
.set pop

PATCH_START 0x801a2ed8
  j Audio_RestoreBgmForNativeEncounter
   nop
PATCH_END

.set push
.set noreorder
Audio_RestoreBgmForNativeEncounter:
  addiu sp,sp,-0x18
  sw ra,0x14(sp)
  jal comboEnemyMusicSuppressed
   nop
  beqz v0,1f
   lw ra,0x14(sp)
  jr ra
   addiu sp,sp,0x18
1:
  j 0x801a2ee0
   nop
.set pop
