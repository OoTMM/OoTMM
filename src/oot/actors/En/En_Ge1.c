#include <combo.h>

int EnGe1_IsPeaceful(void)
{
    return gSave.inventory.quest.gerudoCard;
}

PATCH_FUNC(0x80a8f554, EnGe1_IsPeaceful);
PATCH_FUNC(0x80b23e68, EnGe1_IsPeaceful); /* En_Ge2 */

int EnGe1_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    switch (gi)
    {
    case GI_OOT_HEART_PIECE:
        gi = comboOverride(OV_NPC, 0, NPC_OOT_GERUDO_ARCHERY_1, gi);
        break;
    case GI_OOT_QUIVER2:
    case GI_OOT_QUIVER3:
        gi = comboOverride(OV_NPC, 0, NPC_OOT_GERUDO_ARCHERY_2, gi);
        break;
    }

    return GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80a901d0, EnGe1_GiveItem);
PATCH_CALL(0x80a90298, EnGe1_GiveItem);
