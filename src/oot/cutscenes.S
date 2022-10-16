#include <combo.h>

/* Disable the saria LW cutscene */
PATCH_START(0x80056e80)
  li v0,1
PATCH_END

/* Disable the light arrow cutscene */
PATCH_START 0x80056ef0
  li v0,1
PATCH_END

/* Disable the desert colossus cutscene */
PATCH_START 0x80056de4
  li v0,1
PATCH_END

/* Disable the bongo-bongo escape cutscene */
PATCH_START 0x80056e4c
  li v0,1
PATCH_END
