#include <combo.h>

int EnMs_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    gi = comboOverride(OV_NPC, 0, NPC_OOT_BEAN_SELLER, gi);
    gSave.inventory.beans = 1;
    return GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80aa08dc, EnMs_GiveItem);
PATCH_CALL(0x80aa0998, EnMs_GiveItem);
