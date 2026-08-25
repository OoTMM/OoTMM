#include <combo.h>

.globl _CutsceneManager_Start
_CutsceneManager_Start:
  addiu sp, -0x30
  j CutsceneManager_Start+0x8
   sw a0, 0x30(sp)

PATCH_START CutsceneManager_Start
  j CutsceneManager_StartHook
   nop
PATCH_END
