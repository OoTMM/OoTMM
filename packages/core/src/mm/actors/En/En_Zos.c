#include <combo.h>

void EnZos_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    if (!(GET_LINK(play)->state & PLAYER_ACTOR_STATE_GET_ITEM))
        Message_Close(play);

    if (gi == GI_MM_HEART_PIECE)
    {
        gi = comboOverride(OV_NPC, 0, NPC_MM_ZORA_EVAN, gi);
    }
    GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x80bbb3fc, EnZos_GiveItem);
