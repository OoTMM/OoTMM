#include <combo/asm.h>

/* Hook into DrawGiList, to use our extended table */
PATCH_START(0x800570e0)
  lui   t9,%hi(kComboGiHandlers)
  addu  t9,t9,t6
  lw    t9,%lo(kComboGiHandlers)(t9)
PATCH_END

/* Hook into zDrawGi_O1T1 */
PATCH_START(0x8005831c)
  lui t4,%hi(kComboGiHandlers)
PATCH_END

PATCH_START(0x80058330)
  addiu t4,%lo(kComboGiHandlers)
PATCH_END
