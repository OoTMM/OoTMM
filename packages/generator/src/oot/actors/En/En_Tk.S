#include <combo.h>

/* 
    File:  En_Tk      -- Dampe - Alive 
    Start: 0x809251f0	-- 0xcc2960
    End:   0x80927020	-- 0xcc4790
    Size:  0x1e30
*/

/* Add a collectible flag the the dampe HP */
PATCH_START 0x80926dcc
.int 0x1f06
PATCH_END

/* Hook after dampe init */
PATCH_START 0x80926488
  j EnTk_AfterInit
PATCH_END

/* Skip the dampe intro */
PATCH_START 0x80925e2c
  nop
PATCH_END

PATCH_START 0x809251f0 + 0x1648
  sb at, 0x1f8(s0)
PATCH_END

PATCH_START 0x809251f0 + 0x16c4
  nop
PATCH_END
