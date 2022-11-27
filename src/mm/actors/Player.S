#include <combo.h>

/* Expand the GI buffer */
PATCH_START 0x808420c8
  li a0,0x6000
PATCH_END

/* Force GetItemCollectBehavior return value */
PATCH_START 0x8083d554
  li v0,0xff
PATCH_END

PATCH_START 0x8083d400
  li v0,0xff
PATCH_END

PATCH_START 0x8083d428
  li v0,0xff
PATCH_END

PATCH_START 0x8083d554
  li v0,0xff
PATCH_END

PATCH_START 0x8083d2a0
  li v0,0xff
PATCH_END

/* Never decrement the razor blade health */
PATCH_START 0x80840024
  move t8,v0
PATCH_END

/* Disable first song of time cutscene */
PATCH_START 0x808528c4
  li t8,0x1c00
PATCH_END

/* Make give item treat fairies as normal items */
PATCH_START 0x80852da4
  li at,0xfff
PATCH_END

PATCH_START 0x80852dd8
  li at,0xfff
PATCH_END
