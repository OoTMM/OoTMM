#include <combo.h>

void DmChar08_WarpToTemple(Actor* this, GameState_Play* play)
{
    play->nextEntrance = ((SCE_MM_TEMPLE_GREAT_BAY - 3) << 9) | (0 << 4);
    play->transitionType = TRANS_TYPE_NORMAL;
    play->transitionGfx = TRANS_GFX_BLACK;
}

PATCH_FUNC(0x80aafa18, DmChar08_WarpToTemple);
