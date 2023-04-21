#include <combo.h>

/* Make the beans a single purchase */
PATCH_START 0x80aa05cc
  slti at,v0,1
PATCH_END

/* Change the price to always be 60 rupees */
PATCH_START 0x80aa0bac
.short 60
.short 60
.short 60
.short 60
.short 60
.short 60
.short 60
.short 60
.short 60
.short 60
PATCH_END
