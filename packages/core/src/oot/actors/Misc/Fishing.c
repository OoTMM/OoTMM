#include <combo.h>
#include <combo/item.h>

int Fishing_HasGivenItem(Actor* this)
{
    if (Actor_HasParent(this))
    {
        if (gSave.age == AGE_CHILD)
            gOotExtraFlags.fishingChild = 1;
        else
            gOotExtraFlags.fishingAdult = 1;
        return 1;
    }
    return 0;
}

PATCH_CALL(0x80a427ac, Fishing_HasGivenItem);

void Fishing_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    if (gSave.age == AGE_CHILD)
    {
        if (gOotExtraFlags.fishingChild)
            gi = GI_OOT_RUPEE_BLUE;
        else
            npc = NPC_OOT_FISH_CHILD;
    }
    else
    {
        if (gOotExtraFlags.fishingAdult)
            gi = GI_OOT_RUPEE_BLUE;
        else
            npc = NPC_OOT_FISH_ADULT;
    }

    comboGiveItemNpc(this, play, gi, npc, a, b);
}

PATCH_CALL(0x80a42644, Fishing_GiveItem);
PATCH_CALL(0x80a427e0, Fishing_GiveItem);
