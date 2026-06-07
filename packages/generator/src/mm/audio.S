#include <combo.h>

/* Fix a vanilla bug that can cause a crash with some custom songs */
PATCH_START 0x80198ffc
  move t6,t5
PATCH_END

/* Make Kamaro use SEQ_PLAYER_BGM_SUB instead of SEQ_PLAYER_BGM_MAIN */
PATCH_START 0x800b9e14
  li a0, 3
PATCH_END

Music_AfterChannelInit_Hook:
    addiu   sp, sp, -0x28
    or      a0, s2, zero
    jal     Music_AfterChannelInit
     lw     a1, 0x002C (sp)
    lw      ra, 0x0024 (sp)
    jr      ra
     addiu  sp, sp, 0x28

PATCH_START 0x80197D1C
    j       Music_AfterChannelInit_Hook
PATCH_END

PATCH_START 0x801941D8
    jal     Music_HandleChannelMute
     nop
    nop
    nop
    nop
    nop
    b       .+0xbc
     nop
PATCH_END
