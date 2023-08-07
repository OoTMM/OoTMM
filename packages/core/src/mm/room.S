#include <combo.h>

/* Hook OnRoomChange */
PATCH_START 0x801c26ac
.int OnRoomChange
PATCH_END

SpawnSetupEntryHook:
  /* Store the current spawning actor */
  lui t7,%HI(gActorNum)
  sb s1,%LO(gActorNum)(t7)

  /* Replaced code */
  jr ra
    lh t7,0x8(s0)


PATCH_START 0x800b93c0
  jal SpawnSetupEntryHook
PATCH_END
