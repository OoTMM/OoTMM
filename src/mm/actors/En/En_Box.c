#include <combo.h>

int EnBox_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    gi = comboOverride(OV_CHEST, play->sceneId, actor->variable & 0x1f, gi);
    return GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80868fe0, EnBox_GiveItem);
