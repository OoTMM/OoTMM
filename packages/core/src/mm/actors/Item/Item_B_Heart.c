#include <combo.h>

void ItemBHeart_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    gi = comboOverride(OV_COLLECTIBLE, play->sceneId, 0x1f, gi);
    GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x808bcf38, ItemBHeart_GiveItem);

void ItemBHeart_Draw(Actor* this, GameState_Play* play)
{
    s16 gi;

    gi = comboOverride(OV_COLLECTIBLE, play->sceneId, 0x1f, GI_MM_HEART_CONTAINER);
    comboDrawGI(play, this, gi, DRAW_RAW);
}

PATCH_FUNC(0x808bcfc4, ItemBHeart_Draw);
