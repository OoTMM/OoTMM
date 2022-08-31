#include <combo/asm.h>

#define ENTRANCE_MIDO         0x433
#define ENTRANCE_HCASTLE      0x138
#define ENTRANCE_BOSS1        0x40f
#define ENTRANCE_KAKARIKO     0x195
#define ENTRANCE_SMEADOW      0x215
#define ENTRANCE_GORON        0x14d
#define ENTRANCE_GF           0x315
#define ENTRANCE_ZWF          0x1dd
#define ENTRANCE_ZRI          0x0ea
#define ENTRANCE_HYLIA        0x560
#define ENTRANCE_ZD           0x108
#define ENTRANCE_BOSS3        0x301

/* Disable first cutscene */
PATCH_START(0x800fbf60)
.int 0x10
PATCH_END

PATCH_START(0x80090c58)
li t9, 0x0
PATCH_END

/* Spawn child link at a specific entrance */
PATCH_START(0x800903e0)
  li t4,ENTRANCE_BOSS3
PATCH_END

/* Hook save creation to create MM save */
PATCH_START(0x80090eb0)
  j comboCreateSaveMM
PATCH_END
