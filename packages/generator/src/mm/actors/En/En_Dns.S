#include <combo.h>

PATCH_START 0x8092d4d0
  j EnDns_AfterDetection
PATCH_END

PATCH_START 0x8092c9bc
  j EnDns_GetMsgScript_Custom
  nop
PATCH_END