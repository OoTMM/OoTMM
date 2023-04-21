#include <combo.h>

/* Force the default gossip message */
PATCH_START 0x80997e10
  li t6,0x20d0
PATCH_END

/* Alter the param2 of the message call */
PATCH_START 0x80997e9c
  move a1,a2
PATCH_END
