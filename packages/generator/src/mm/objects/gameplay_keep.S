#include <combo.h>

/* Hammer Animations - Patch AnimHeader Frame Count */
/* TODO: Eventual cross-anim support */

/* gPlayerAnim_Link_otituku_w -> gPlayerAnim_link_hammer_hit */
PATCH_OBJECT OBJECT_GAMEPLAY_KEEP, 0xE4E8
    .short 11
PATCH_END

/* gPlayerAnim_Link_ue_wait -> gPlayerAnim_link_hammer_hit_end */
PATCH_OBJECT OBJECT_GAMEPLAY_KEEP, 0xE4F0
    .short 23
PATCH_END

/* gPlayerAnim_Link_muku -> gPlayerAnim_link_hammer_hit_endR */
PATCH_OBJECT OBJECT_GAMEPLAY_KEEP, 0xE4E0
    .short 23
PATCH_END

/* gPlayerAnim_Link_miageru -> gPlayerAnim_link_hammer_side_hit */
PATCH_OBJECT OBJECT_GAMEPLAY_KEEP, 0xE4D8
    .short 12
PATCH_END

/* gPlayerAnim_Link_m_wait -> gPlayerAnim_link_hammer_side_hit_end */
PATCH_OBJECT OBJECT_GAMEPLAY_KEEP, 0xE4D0
    .short 23
PATCH_END

/* gPlayerAnim_Link_ha -> gPlayerAnim_link_hammer_side_hit_endR */
PATCH_OBJECT OBJECT_GAMEPLAY_KEEP, 0xE4C8
    .short 23
PATCH_END
