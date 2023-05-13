#include <combo.h>

void EnSyatekiMan_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    switch (gi)
    {
    case GI_OOT_BULLET_BAG2:
    case GI_OOT_BULLET_BAG3:
        gi = comboOverride(OV_NPC, 0, NPC_OOT_SHOOTING_GAME_CHILD, gi);
        break;
    case GI_OOT_QUIVER2:
    case GI_OOT_QUIVER3:
        gi = comboOverride(OV_NPC, 0, NPC_OOT_SHOOTING_GAME_ADULT, gi);
        break;
    }
    GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x809a130c, EnSyatekiMan_GiveItem);
PATCH_CALL(0x809a140c, EnSyatekiMan_GiveItem);
