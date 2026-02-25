#include <combo.h>

/* Hook ParseSceneRoomHeaders_ActorsList */
PATCH_START 0x801c26ac
.int ParseSceneRoomHeaders_ActorsList
PATCH_END

SpawnSetupEntryHook:
  /* Store the current spawning actor */
  lui t7,%HI(gCurrentSpawnActorNum)
  sb s1,%LO(gCurrentSpawnActorNum)(t7)

  /* Replaced code */
  jr ra
    lh t7,0x8(s0)

PATCH_START 0x800b93c0
  jal SpawnSetupEntryHook
PATCH_END


SpawnSetupEntryAfterHook:
  lui t0,%HI(gCurrentSpawnActorNum)
  li t1,0xff
  jr ra
   sb t1,%LO(gCurrentSpawnActorNum)(t0)

PATCH_START 0x800b9484
  j SpawnSetupEntryAfterHook
PATCH_END
