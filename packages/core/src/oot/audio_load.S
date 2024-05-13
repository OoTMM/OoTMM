#include <combo.h>

/* Use malloc to allocate the (now oversized) bank descriptors */
PATCH_START 0x800b8920
  sll a0,a1,0x02
  jal malloc
PATCH_END

/* Use our custom seq table */
PATCH_START 0x800b8884
  lui t6,%HI(gCustomAudioTables)
  addiu t6,%LO(gCustomAudioTables)
  lw t1,0x00(t6)
  lw t2,0x04(t6)
  lw t3,0x08(t6)
  lw t6,0x0c(t6)
  nop
  nop
PATCH_END

/* Don't re-process tables */
PATCH_START 0x800b88d8
  nop
PATCH_END

PATCH_START 0x800b88ec
  nop
PATCH_END

PATCH_START 0x800b8900
  nop
PATCH_END

/* Load Status Patches - AudioHeap_PopPersistentCache */
PATCH_START 0x800b3aa0
  lui t6,%HI(gAudioLoadStatusSeq)
PATCH_END

PATCH_START 0x800b3aa8
  addiu t6,%LO(gAudioLoadStatusSeq)
PATCH_END

PATCH_START 0x800b3ab8
  lui t7,%HI(gAudioLoadStatusFont)
  addiu t7,%LO(gAudioLoadStatusFont)
PATCH_END

/* Load Status Patches - AudioLoad_IsFontLoadComplete */
PATCH_START 0x800b6e8c
  lui t6,%HI(gAudioLoadStatusFont)
  addu t6,a1
  lbu t6,%LO(gAudioLoadStatusFont)(t6)
PATCH_END

PATCH_START 0x800b6eb4
  lui t7,%HI(gAudioLoadStatusFont)
  addu t7,v0
  lbu t7,%LO(gAudioLoadStatusFont)(t7)
PATCH_END

/* Load Status Patches - AudioLoad_IsSeqLoadComplete */
PATCH_START 0x800b6f04
  lui t6,%HI(gAudioLoadStatusSeq)
  addu t6,a1
  lbu t6,%LO(gAudioLoadStatusSeq)(t6)
PATCH_END

PATCH_START 0x800b6f2c
  lui t7,%HI(gAudioLoadStatusSeq)
  addu t7,v0
  lbu t7,%LO(gAudioLoadStatusSeq)(t7)
PATCH_END

/* Load Status Patches - AudioLoad_SyncLoadSeq */
PATCH_START 0x800b789c
  lui t6,%HI(gAudioLoadStatusSeq)
  addu t6,v0
  lbu t6,%LO(gAudioLoadStatusSeq)(t6)
PATCH_END

/* Load Status Patches - AudioLoad_SyncLoadFont */
AudioLoad_SyncLoadFontGetStatus:
  lui t7,%HI(gAudioLoadStatusFont)
  addu t7,v0
  jr ra
   lbu t7,%LO(gAudioLoadStatusFont)(t7)

PATCH_START 0x800b7a34
  jal AudioLoad_SyncLoadFontGetStatus
   nop
PATCH_END

/* Load Status Patches - AudioLoad_AsyncLoadInner */
PATCH_START 0x800b82e8
  lui t6,%HI(gAudioLoadStatusSeq)
  addu t6,v0
  lbu t6,%LO(gAudioLoadStatusSeq)(t6)
PATCH_END

PATCH_START 0x800b82d0
  lui t7,%HI(gAudioLoadStatusFont)
PATCH_END

PATCH_START 0x800b8314
  lbu t7,%LO(gAudioLoadStatusFont)(t7)
PATCH_END

/* Load Status Patches - AudioHeap_AllocCached */
PATCH_START 0x800b3eec
  lui t3,%HI(gAudioLoadStatusSeq)
PATCH_END

PATCH_START 0x800b3ef8
  addiu t3,%LO(gAudioLoadStatusSeq)
PATCH_END

PATCH_START 0x800b3f00
  lui t3,%HI(gAudioLoadStatusFont)
PATCH_END

PATCH_START 0x800b3f0c
  addiu t3,%LO(gAudioLoadStatusFont)
PATCH_END

/* Load Status Patches - Misc */
PATCH_START 0x800b6fd8
  j AudioLoad_SetFontLoadStatus
   nop
PATCH_END

PATCH_START 0x800b7008
  j AudioLoad_SetSeqLoadStatus
   nop
PATCH_END

PATCH_START 0x800b3540
  j AudioHeap_ResetLoadStatus
   nop
PATCH_END
