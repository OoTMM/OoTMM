#include <combo.h>

PATCH_START 0x80113ba8
  lbu a0,(gMaxBombchuMm)
PATCH_END

PATCH_START 0x80113c38
  lbu a0,(gMaxBombchuMm)
PATCH_END

PATCH_START 0x80113ca4
  lbu a0,(gMaxBombchuMm)
PATCH_END

PATCH_START 0x80115b74
  lbu a2,(gMaxBombchuMm)
PATCH_END

PATCH_START 0x80117ebc
  lbu t7,(gMaxBombchuMm)
PATCH_END

PATCH_START 0x8081b50c
  lbu t7,(gMaxBombchuMm)
PATCH_END

PATCH_START 0x80863f1c
  lui t4,%HI(gMaxBombchuMm)

PATCH_START 0x80863f28
  lbu t4,%LO(gMaxBombchuMm)(t4)
PATCH_END
