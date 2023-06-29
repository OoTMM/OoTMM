#include <combo.h>

PATCH_START 0x80167f60
  jal DrawHUDWrapper
PATCH_END

PATCH_START 0x8011ff54
  li t0,0
PATCH_END

RupeeCountLoopHook:
  la    t9,gHudRupeesBuffer
  jr ra
    addu  t9,t0

PATCH_START 0x8011ff94
  jal RupeeCountLoopHook
PATCH_END

PATCH_START 0x8011ffac
  lbu t6,0(t9)
PATCH_END
