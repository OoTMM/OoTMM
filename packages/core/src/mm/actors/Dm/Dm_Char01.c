#include <combo.h>

void DmChar01_TempleRise(Actor* this, GameState_Play* play)
{
    play->nextEntrance = ((SCE_MM_WOODFALL - 3) << 9) | (4 << 4);
    play->transitionTrigger = TRANS_TRIGGER_NORMAL;
    play->transitionType = TRANS_TYPE_FADE_BLACK;
}

PATCH_FUNC(0x80aa884c, DmChar01_TempleRise);
