#include <combo.h>

void EnFr_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    if (gi == GI_OOT_HEART_PIECE)
    {
        if (GetEventChk(EV_OOT_CHK_FROGS_GAME))
        {
            gi = comboOverride(OV_NPC, 0, NPC_OOT_FROGS_GAME, gi);
        }
        else
        {
            gi = comboOverride(OV_NPC, 0, NPC_OOT_FROGS_STORMS, gi);
        }
    }
    GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80a279b0, EnFr_GiveItem);
PATCH_CALL(0x80a27a14, EnFr_GiveItem);
