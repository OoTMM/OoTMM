#include <combo.h>

#define AUDIO_HEAP_START  0x80600000
#define AUDIO_HEAP_SIZE   0x80000

#define FANFARE_AUDIOSEQ_SIZE   0x1000
#define FANFARE_AUDIOBANK_SIZE  0x4800
#define FANFARE_UNK_SIZE        0
#define BGM_AUDIOSEQ_SIZE       0xe000
#define BGM_AUDIOBANK_SIZE      0x4800
#define BGM_UNK_SIZE            0

/* Patch the audio heap */
PATCH_START 0x800b87c4
  la t9,AUDIO_HEAP_START
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
