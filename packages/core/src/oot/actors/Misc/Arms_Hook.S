#include <combo.h>

/* Hookshot anywhere */
PATCH_GROUP PG_OOT_HOOKSHOT_ANYWHERE

PATCH_START 0x80910378
  nop
PATCH_END

PATCH_GROUP_END

/* Don't require the adult object */
PATCH_START 0x809106e8
.short 0x0001
PATCH_END

#if 0
/* Hook the draw method */
PATCH_START 0x809106fc
.int ArmsHook_DrawWrapper
PATCH_END

/* Relocs */
PATCH_START 0x80910894
.int 0
PATCH_END
#endif

PATCH_START 0x809105c0
  sw zero,0(v1)
  sw zero,4(v1)
PATCH_END

PATCH_START 0x809106b8
  sw zero,0(v1)
  sw zero,4(v1)
PATCH_END
