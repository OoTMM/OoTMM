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

/* Track the sequence currently being loaded so font cache policy can follow it. */
Audio_SetLoadingSequenceId_SyncHook:
    addiu   sp, sp, -0x20
    sw      ra, 0x001c(sp)
    sw      a0, 0x0018(sp)

    jal     Audio_SetLoadingSequenceId
     nop

    lw      t0, 0x0018(sp)
    lw      v0, 0x283c(s3)

    lw      ra, 0x001c(sp)
    jr      ra
     addiu  sp, sp, 0x20


/* Clear the tracked sequence after the synchronous sequence load completes. */
Audio_LoadSequence_ClearHook:
    addiu   sp, sp, -0x20
    sw      ra, 0x001c(sp)

    jal     0x800b7884
     nop

    sw      v0, 0x0018(sp)

    jal     Audio_ClearLoadingSequenceId
     nop

    lw      v0, 0x0018(sp)
    lw      ra, 0x001c(sp)
    jr      ra
     addiu  sp, sp, 0x20


/* Override font cache policy with the policy of the sequence being loaded. */
Audio_GetLoadCachePolicy_SyncHook:
    addiu   sp, sp, -0x20
    sw      ra, 0x001c(sp)
    sw      v1, 0x0018(sp)

    lb      a0, 0x0019(a0)

    jal     Audio_GetLoadCachePolicy
     nop

    move    a1, v0
    lw      v1, 0x0018(sp)
    lw      t6, 0x0010(v1)

    lw      ra, 0x001c(sp)
    jr      ra
     addiu  sp, sp, 0x20


/* Track sequence ID before synchronous sequence/font loading begins. */
PATCH_START 0x800B77A8
    jal     Audio_SetLoadingSequenceId_SyncHook
     lw     a0, 0x0034(sp)
PATCH_END

/* Clear tracked sequence ID after synchronous sequence loading finishes. */
PATCH_START 0x800B77F8
    jal     Audio_LoadSequence_ClearHook
PATCH_END

/* Use the loading sequence's cache policy for synchronous font loads. */
PATCH_START 0x800B7BA0
    jal     Audio_GetLoadCachePolicy_SyncHook
     lw     a1, 0x0050(sp)
PATCH_END