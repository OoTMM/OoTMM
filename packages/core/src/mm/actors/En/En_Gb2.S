#include <combo.h>

PATCH_START 0x80b0f850
  slti at,t0,0x30
PATCH_END

PATCH_START 0x80b0f884
  slti at,t3,0x30
PATCH_END

PATCH_START 0x80b10490
  slti at,t6,0x30
PATCH_END

