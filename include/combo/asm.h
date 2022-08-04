#ifndef COMBO_ASM_H
#define COMBO_ASM_H

.set noreorder
.set nobopt
.set noat

#include <combo/defs.h>
#include <sys/regdef.h>

.macro PATCH_START addr
.section .patch, "awx"
.int \addr
.int (1f - 0f)
0:
.endm

.macro PATCH_END
1:
.previous
.endm

#endif /* COMBO_ASM_H */
