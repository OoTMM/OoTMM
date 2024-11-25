#include <combo.h>
#include <combo/mask.h>
#include <combo/player.h>

u16 MaskReaction_GetTextId(PlayState* play, int slot)
{
    Player* link;

    link = GET_PLAYER(play);

    if (link->mask == MASK_KEATON && slot == MASK_REACTION_SET_HYRULIAN_GUARD)
        return 0x712d;

    if (slot == MASK_REACTION_SET_SKULL_KID)
        return 0x10bc;

    return 0;
}

PATCH_FUNC(0x800597c0, MaskReaction_GetTextId);
