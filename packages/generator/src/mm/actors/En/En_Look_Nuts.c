#include <combo.h>
#include <combo/player.h>
#include <combo/mask.h>

s32 Player_GetMask_EnLookNutsSkullMask(PlayState* play)
{
    if (gCustomSave.customMask == PLAYER_CUSTOM_MASK_SKULL) {
        return PLAYER_MASK_STONE;
    }

    return Player_GetMask(play);
}

PATCH_CALL(0x80A683C0, Player_GetMask_EnLookNutsSkullMask);