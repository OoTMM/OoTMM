#include <combo.h>

int comboGiveItem(Actor* actor, GameState_Play* play, s16 itemId, float a, float b)
{
    s32 override;
    u16 absItemId;

    override = -1;
    absItemId = itemId > 0 ? itemId : -itemId;

    switch (actor->id)
    {
    case AC_EN_BOX:
        override = comboGetChestOverride(play->sceneId, actor->variable & 0x1f);
        break;
    }

    if (override >= 0)
        absItemId = (u16)override;
    absItemId = comboProgressive(absItemId);

    itemId = itemId > 0 ? absItemId : -absItemId;

    return GiveItem(actor, play, itemId, a, b);
}
