#include <combo/asm.h>

/* Hook into DrawShader, to use our extended table */
PATCH_START(0x800570e0)
  lui   t9,%hi(kShaders)
  addu  t9,t9,t6
  lw    t9,%lo(kShaders)(t9)
PATCH_END

/* Hook into Shader_Opa0_Xlu1 */
PATCH_START(0x8005831c)
  lui t4,%hi(kShaders)
PATCH_END

PATCH_START(0x80058330)
  addiu t4,%lo(kShaders)
PATCH_END
