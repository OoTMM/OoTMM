#include <combo.h>

/* Use our own note handler */
PATCH_START 0x8019af50
  j OcarinaMaskButtons
PATCH_END

Ocarina_HandleLastPlayedSong_Hook:
  HOOK_CALL_RET Ocarina_HandleLastPlayedSong
  li  at, 0x6
  lui t6, 0x1
  jr  ra
   addu t6, t6, s0

PATCH_START 0x80157de8
  jal   Ocarina_HandleLastPlayedSong_Hook
   lh   a0, 0x0000 (a3)
  bltzl v0, .+0x340
   li   t8, 0xff
  bne   v0, at, .+0x6c
PATCH_END
