#include <combo.h>

/* Bytecode - Disable pictobox check */
PATCH_START 0x80a51276
.short 0
PATCH_END

/* Bytecode - Disable some event check */
PATCH_START 0x80a51394
.short 0
PATCH_END

/* GI renumbering */
/*
PATCH_START 0x80a5127e
.half GI_MM_PICTOGRAPH_BOX
PATCH_END

PATCH_START 0x80a513bb
.half GI_MM_HEART_PIECE
PATCH_END

PATCH_START 0x80a513e6
.half GI_MM_RUPEE_PURPLE
PATCH_END
*/

/* Cleared-swamp tour archery, initial/free game. */
PATCH_START 0x80a5130c
.byte MSCRIPT_CUSTOM_CMD_ID_KOUME_BOW_AGE_FREE
PATCH_END

/* Cleared-swamp tour archery, paid replay. */
PATCH_START 0x80a5134d
.byte MSCRIPT_CUSTOM_CMD_ID_KOUME_BOW_AGE_RUPEES
PATCH_END
