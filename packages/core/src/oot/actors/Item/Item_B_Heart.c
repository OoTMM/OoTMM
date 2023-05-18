#include <combo.h>

static void ItemBHeart_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    gi = comboOverride(OV_COLLECTIBLE, play->sceneId, 0x1f, GI_OOT_HEART_CONTAINER);
    GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x80909518, ItemBHeart_GiveItem);

static void ItemBHeart_Draw(Actor* this, GameState_Play* play)
{
    s16 gi;

    gi = comboOverride(OV_COLLECTIBLE, play->sceneId, 0x1f, GI_OOT_HEART_CONTAINER);
    comboDrawGI(play, this, gi, 0);
}

PATCH_FUNC(0x80909620, ItemBHeart_Draw);
