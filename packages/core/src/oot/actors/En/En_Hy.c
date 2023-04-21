#include <combo.h>

int EnHy_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    if (gi == GI_OOT_HEART_PIECE)
    {
        gi = comboOverride(OV_NPC, 0, NPC_OOT_DOG_LADY, gi);
    }
    return GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80ae5138, EnHy_GiveItem);
PATCH_CALL(0x80ae6e84, EnHy_GiveItem);
