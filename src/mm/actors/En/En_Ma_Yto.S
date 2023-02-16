#include <combo.h>

/* Remove the random chance of getting nothing */
PATCH_START 0x80b8e9b8
  li at,0xfff
PATCH_END

/* Skip most of the cutscene */
PATCH_START 0x80b79634
  li t3,0xce40
PATCH_END
