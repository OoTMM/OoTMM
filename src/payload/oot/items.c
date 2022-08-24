#include <combo.h>

#define CB_PASSIVE 0x00
#define CB_GETITEM 0xff

int GetItemCollectBehavior(s16 itemId)
{
    return CB_GETITEM;
}

int hook_SetChestItemInRange(Actor* actor, GameState_Play* play, s16 itemId, float a, float b)
{
    s32 override;
    if (actor->id == 0x0a)
    {
        override = comboGetChestOverride(play->sceneId, actor->variable & 0x1f);
        if (override >= 0)
            itemId = (s16)(-override);
    }
    return SetChestItemInRange(actor, play, itemId, a, b);
}
