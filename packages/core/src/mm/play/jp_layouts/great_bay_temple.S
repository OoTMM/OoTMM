#include <combo.h>

#define SCENE     0x26bf000
#define ROOM      0x26de000

.macro PATCH_U16 addr, value
PATCH_VROM \addr
.short \value
PATCH_END
.endm

/* Patch the collision */
PATCH_U16 SCENE + 0x8b56 + 6 * 0, -20
PATCH_U16 SCENE + 0x8b56 + 6 * 1, -20
PATCH_U16 SCENE + 0x8b56 + 6 * 2,  40
PATCH_U16 SCENE + 0x8b56 + 6 * 3,  40
PATCH_U16 SCENE + 0x8b56 + 6 * 4,  40
PATCH_U16 SCENE + 0x8b56 + 6 * 5, -20
PATCH_U16 SCENE + 0x8b56 + 6 * 6,  40
PATCH_U16 SCENE + 0x8b56 + 6 * 7, -20

/* Patch the collision polygons */
PATCH_U16 SCENE + 0x60DE, -40
PATCH_U16 SCENE + 0x60EE, -40
PATCH_U16 SCENE + 0x613E, -20
PATCH_U16 SCENE + 0x614E, -20
