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