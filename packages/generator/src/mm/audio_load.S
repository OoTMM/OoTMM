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

/* Track the sequence currently being loaded so font cache policy can follow it. */
Audio_SetLoadingSequenceId_SyncHook:
    addiu   sp, sp, -0x20
    sw      ra, 0x001c(sp)
    sw      a0, 0x0018(sp)

    jal     Audio_SetLoadingSequenceId
     nop

    lw      t0, 0x0018(sp)
    lw      v0, 0x2860(s3)

    lw      ra, 0x001c(sp)
    jr      ra
     addiu  sp, sp, 0x20


/* Clear the tracked sequence after the synchronous sequence load completes. */
Audio_LoadSequence_ClearHook:
    addiu   sp, sp, -0x20
    sw      ra, 0x001c(sp)

    jal     0x8018fccc
     nop

    sw      v0, 0x0018(sp)

    jal     Audio_ClearLoadingSequenceId
     nop

    lw      v0, 0x0018(sp)
    lw      ra, 0x001c(sp)
    jr      ra
     addiu  sp, sp, 0x20


/* Override synchronous font cache policy using the loading sequence. */
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


/* Track the sequence responsible for an asynchronous font load. */
Audio_SetLoadingSequenceId_AsyncHook:
    addiu   sp, sp, -0x20
    sw      ra, 0x001c(sp)
    sw      v0, 0x0018(sp)

    jal     Audio_SetLoadingSequenceId
     nop

    lw      v0, 0x0018(sp)
    lw      ra, 0x001c(sp)
    addiu   sp, sp, 0x20

    lbu     t6, 0x0000(v0)
    jr      ra
     lw     t5, 0x0034(sp)


/* Override asynchronous font cache policy using the loading sequence. */
Audio_GetLoadCachePolicy_AsyncHook:
    addiu   sp, sp, -0x20
    sw      ra, 0x001c(sp)
    sw      v1, 0x0018(sp)

    lb      a0, 0x0019(a0)

    jal     Audio_GetLoadCachePolicy
     nop

    move    a1, v0
    lw      v1, 0x0018(sp)
    lw      t9, 0x0010(v1)

    lw      ra, 0x001c(sp)
    jr      ra
     addiu  sp, sp, 0x20


/* Track sequence ID before synchronous sequence/font loading begins. */
PATCH_START 0x8018FBE4
    jal     Audio_SetLoadingSequenceId_SyncHook
     lw     a0, 0x0034(sp)
PATCH_END

/* Clear tracked sequence ID after synchronous sequence loading finishes. */
PATCH_START 0x8018FC34
    jal     Audio_LoadSequence_ClearHook
PATCH_END

/* Use the loading sequence's cache policy for synchronous font loads. */
PATCH_START 0x8018FFE8
    jal     Audio_GetLoadCachePolicy_SyncHook
     lw     a1, 0x0058(sp)
PATCH_END

/* Track sequence ID before asynchronous font loading begins. */
PATCH_START 0x801A7F1C
    jal     Audio_SetLoadingSequenceId_AsyncHook
     lw     a0, 0x0044(sp)
PATCH_END

/* Use the loading sequence's cache policy for asynchronous font loads. */
PATCH_START 0x8019089C
    jal     Audio_GetLoadCachePolicy_AsyncHook
     move   a1, s0
PATCH_END
