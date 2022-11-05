#include <combo.h>

int EnBox_GiveItemDefaultRange(Actor* actor, GameState_Play* play, s16 gi)
{
    if (play->sceneId == SCE_MM_TREASURE_SHOP && gi == -GI_MM_HEART_PIECE)
    {
        gi = comboOverride(OV_NPC, 0, NPC_MM_CHEST_GAME, gi);
    }
    else
    {
        gi = comboOverride(OV_CHEST, play->sceneId, actor->variable & 0x1f, gi);
    }
    return GiveItemDefaultRange(actor, play, gi);
}

PATCH_CALL(0x80868fe0, EnBox_GiveItemDefaultRange);
