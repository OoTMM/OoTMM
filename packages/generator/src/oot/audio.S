#include <combo.h>

#define AUDIO_HEAP_START  0x80600000
#define AUDIO_HEAP_SIZE   0x100000

#define FANFARE_AUDIOSEQ_SIZE   0x1000
#define FANFARE_AUDIOBANK_SIZE  0x4800
#define FANFARE_UNK_SIZE        0
#define BGM_AUDIOSEQ_SIZE       0xe000
#define BGM_AUDIOBANK_SIZE      0x4800
#define BGM_UNK_SIZE            0

/* Patch the audio heap */
PATCH_START 0x800b87c4
  lui t9,%HI(AUDIO_HEAP_START)
  addiu t9,%LO(AUDIO_HEAP_START)
PATCH_END

PATCH_START 0x8010a1b4
.int AUDIO_HEAP_SIZE - 0x100
PATCH_END

/* Patch the audio thread info */
AudioThreadInfo:
.int FANFARE_AUDIOSEQ_SIZE
.int FANFARE_AUDIOBANK_SIZE
.int FANFARE_UNK_SIZE
.int BGM_AUDIOSEQ_SIZE
.int BGM_AUDIOBANK_SIZE
.int BGM_UNK_SIZE

PATCH_START 0x800B5590
  la      a0,0x80128A50
  la      a1,AudioThreadInfo
  jal     memcpy
   li     a2,0x18
  la      a0,0x80128A50
  jal     0x800B3D18
   nop
  la      a0,0x80128A5C
  jal     0x800B3DDC
   nop
  nop
  nop
  nop
  nop
  nop
  nop
PATCH_END

AudioGetSizes:
  li  t9,FANFARE_AUDIOSEQ_SIZE
  li  t2,FANFARE_AUDIOBANK_SIZE
  li  t5,FANFARE_UNK_SIZE
  li  t4,BGM_AUDIOSEQ_SIZE
  li  t6,BGM_AUDIOBANK_SIZE
  jr  ra
    li t8,BGM_UNK_SIZE

PATCH_START 0x800B5528
  jal AudioGetSizes
   nop
  nop
  nop
  nop
  nop
PATCH_END

/* Fix a vanilla bug that can cause a crash with some custom songs */
PATCH_START 0x800c03bc
  move t6,t5
PATCH_END

PATCH_START 0x800c6820
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
  addiu sp,sp,-0x28
  j 0x800c6828
   sw ra,0x14(sp)
1:
  jr ra
   nop
.set pop

PATCH_START 0x800c6894
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
  j 0x800c689c
   nop
.set pop
