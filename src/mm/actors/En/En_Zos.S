#include <combo.h>

/* Skip the evan cutscene */
PATCH_START 0x80bbbc6c
  la t5, 0x80bbb354
PATCH_END
