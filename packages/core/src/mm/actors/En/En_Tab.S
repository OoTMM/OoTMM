#include <combo.h>

/* Skip the empty bottle check */
PATCH_START 0x80be0d38
  jr  ra
   li v0,1
PATCH_END

/* Bytecode fixes */
PATCH_START 0x80be19c0
.short 0
PATCH_END

PATCH_START 0x80be19c8
.short 0
PATCH_END

PATCH_START 0x80be19de
.short 0
PATCH_END

PATCH_START 0x80be19ed
.short 0
PATCH_END

PATCH_START 0x80be1a2c
.short 0
PATCH_END

PATCH_START 0x80be1a34
.short 0
PATCH_END

PATCH_START 0x80be1a4a
.short 0
PATCH_END

PATCH_START 0x80be1a59
.short 0
PATCH_END
