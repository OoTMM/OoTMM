#include <combo.h>

int EnBox_GiveItemDefaultRange(Actor* actor, GameState_Play* play, s16 gi)
{
    gi = comboOverride(OV_CHEST, play->sceneId, actor->variable & 0x1f, gi);
    return GiveItemDefaultRange(actor, play, gi);
}

PATCH_CALL(0x80868fe0, EnBox_GiveItemDefaultRange);
