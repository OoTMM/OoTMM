#include <combo.h>

EnMag_AfterDraw_Trampoline:
  lw a0,0x40(sp)
  lw a1,0x44(sp)
  j EnMag_AfterDraw
   addiu sp,0x40

PATCH_START 0x80aefb14
  j EnMag_AfterDraw_Trampoline
   nop
PATCH_END

/* Disable stuff */
PATCH_START 0x80aeef00
  nop
PATCH_END

PATCH_START 0x80aeef4c
  nop
PATCH_END

PATCH_START 0x80aef000
  nop
PATCH_END

PATCH_START 0x80aef044
  nop
PATCH_END

PATCH_START 0x80aeecdc
  nop
PATCH_END

PATCH_START 0x80aeed78
  nop
PATCH_END

PATCH_START 0x80aef6e0
  nop
PATCH_END

PATCH_START 0x80aef790
  nop
PATCH_END

PATCH_START 0x80aef40c
  nop
PATCH_END

PATCH_START 0x80aef1cc
  li t6,0
PATCH_END

PATCH_START 0x80aeec1c
  nop
PATCH_END
