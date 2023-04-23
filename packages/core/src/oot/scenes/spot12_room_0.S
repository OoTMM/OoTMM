#include <combo.h>

/* Spawn another gerudo */
.macro SPAWN_GERUDO off
PATCH_VROM 0x021bd000 + \off
.short 0x0138
.short 0xfac8
.short 0x005d
.short 0xf448
.short 0x0000
.short 0x95b0
.short 0x0000
.short 0x0301
PATCH_END
.endm

SPAWN_GERUDO 0x3ec
SPAWN_GERUDO 0x62c
