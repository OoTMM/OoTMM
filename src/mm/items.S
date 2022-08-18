#include <combo/asm.h>

/* MM already have 16 bits indices into the GI table */
/* Make link's gi held item unsigned */
PATCH_LOAD_STORE 8, 1
.int 0x80126a7c
.int 0x80129070
.int 0x801290a4
.int 0x801290d0
.int 0x8012916c
.int 0x80129178
.int 0x8012918c
.int 0x80129198
.int 0x801291cc
.int 0x801291f8
.int 0x80129248
.int 0x8082ed10
.int 0x8082ed14
.int 0x8084708c
.int 0x80848264
.int 0x80852c8c
.int 0x80853ad4
.int 0x80853b50
.int 0x80853b54
.int 0x80853bb8
.int 0x80859764
.int 0x8085a82c
.int 0x8085a83c
.int 0x8085b914
PATCH_END
