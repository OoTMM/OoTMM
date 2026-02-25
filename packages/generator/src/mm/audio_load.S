#include <combo.h>

/* Use malloc to allocate the (now oversized) bank descriptors */
PATCH_START 0x80190eb0
  sll a0,a1,0x02
  jal malloc
PATCH_END

/* Use our custom seq table */
PATCH_START 0x80190e14
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
PATCH_START 0x80190e68
  nop
PATCH_END

PATCH_START 0x80190e7c
  nop
PATCH_END

PATCH_START 0x80190e90
  nop
PATCH_END

/* Load Status Patches - AudioHeap_PopPersistentCache */
PATCH_START 0x8018b7ec
  lui t6,%HI(gAudioLoadStatusSeq)
PATCH_END

PATCH_START 0x8018b7f4
  addiu t6,%LO(gAudioLoadStatusSeq)
PATCH_END

PATCH_START 0x8018b804
  lui t7,%HI(gAudioLoadStatusFont)
  addiu t7,%LO(gAudioLoadStatusFont)
PATCH_END

/* Load Status Patches - AudioLoad_IsFontLoadComplete */
PATCH_START 0x8018f23c
  lui t6,%HI(gAudioLoadStatusFont)
  addu t6,a1
  lbu t6,%LO(gAudioLoadStatusFont)(t6)
PATCH_END

PATCH_START 0x8018f264
  lui t7,%HI(gAudioLoadStatusFont)
  addu t7,v0
  lbu t7,%LO(gAudioLoadStatusFont)(t7)
PATCH_END

/* Load Status Patches - AudioLoad_IsSeqLoadComplete */
PATCH_START 0x8018f2b4
  lui t6,%HI(gAudioLoadStatusSeq)
  addu t6,a1
  lbu t6,%LO(gAudioLoadStatusSeq)(t6)
PATCH_END

PATCH_START 0x8018f2dc
  lui t7,%HI(gAudioLoadStatusSeq)
  addu t7,v0
  lbu t7,%LO(gAudioLoadStatusSeq)(t7)
PATCH_END

/* Load Status Patches - AudioLoad_SyncLoadSeq */
PATCH_START 0x8018fce4
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

PATCH_START 0x8018fe7c
  jal AudioLoad_SyncLoadFontGetStatus
   nop
PATCH_END

/* Load Status Patches - AudioLoad_AsyncLoadInner */
PATCH_START 0x801907c4
  lui t6,%HI(gAudioLoadStatusSeq)
  addu t6,v0
  lbu t6,%LO(gAudioLoadStatusSeq)(t6)
PATCH_END

PATCH_START 0x801907ac
  lui t7,%HI(gAudioLoadStatusFont)
PATCH_END

PATCH_START 0x801907f0
  lbu t7,%LO(gAudioLoadStatusFont)(t7)
PATCH_END

/* Load Status Patches - AudioHeap_AllocCached */
PATCH_START 0x8018bc38
  lui t3,%HI(gAudioLoadStatusSeq)
PATCH_END

PATCH_START 0x8018bc44
  addiu t3,%LO(gAudioLoadStatusSeq)
PATCH_END

PATCH_START 0x8018bc4c
  lui t3,%HI(gAudioLoadStatusFont)
PATCH_END

PATCH_START 0x8018bc58
  addiu t3,%LO(gAudioLoadStatusFont)
PATCH_END

/* Load Status Patches - Misc */
PATCH_START 0x8018f388
  j AudioLoad_SetFontLoadStatus
   nop
PATCH_END

PATCH_START 0x8018f3b8
  j AudioLoad_SetSeqLoadStatus
   nop
PATCH_END

PATCH_START 0x8018b250
  j AudioHeap_ResetLoadStatus
   nop
PATCH_END
