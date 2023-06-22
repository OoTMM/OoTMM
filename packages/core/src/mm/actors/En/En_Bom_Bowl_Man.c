#include <combo.h>
#include <combo/item.h>

int EnBomBowlMan_HasGivenItem(Actor* this)
{
    if (Actor_HasParent(this))
    {
        gMmExtraFlags2.notebook = 1;
        return 1;
    }
    return 0;
}

PATCH_CALL(0x809c59fc, EnBomBowlMan_HasGivenItem);

void EnBomBowlMan_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    if (gMmExtraFlags2.notebook)
        gi = GI_MM_RECOVERY_HEART;
    else
        npc = NPC_MM_BOMBER_NOTEBOOK;
    comboGiveItemNpc(this, play, gi, npc, a, b);
}

PATCH_CALL(0x809c59c0, EnBomBowlMan_GiveItem);
PATCH_CALL(0x809c5a8c, EnBomBowlMan_GiveItem);
