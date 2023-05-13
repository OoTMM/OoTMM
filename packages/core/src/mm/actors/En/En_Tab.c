#include <combo.h>
#include <combo/item.h>

static u8 sItemIndex;

static s16 EnTab_GI(int index, int flags)
{
    s16 gi;
    s16 npc;
    int flag;

    switch (index)
    {
    case 0:
        gi = GI_MM_MILK;
        npc = NPC_MM_MILK_BAR_MILK;
        flag = gMmExtraFlags2.milkBarMilk;
        break;
    case 1:
        gi = GI_MM_CHATEAU;
        npc = NPC_MM_MILK_BAR_CHATEAU;
        flag = gMmExtraFlags2.milkBarChateau;
        break;
    default:
        UNREACHABLE();
        break;
    }

    gi = comboOverrideEx(OV_NPC, 0, npc, gi, flags);
    if (flag)
        gi = comboRenewable(gi, 0);
    if (!gi)
        return -1;
    return gi;
}

void EnTab_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    switch (gi)
    {
    case GI_MM_MILK:
        sItemIndex = 0;
        break;
    case GI_MM_CHATEAU:
        sItemIndex = 1;
        break;
    }

    gi = EnTab_GI(sItemIndex, OVF_PROGRESSIVE);
    GiveItem(actor, play, gi, a, b);
}

void EnTab_AfterGivingItem(Actor* actor)
{
    switch (sItemIndex)
    {
    case 0:
        gMmExtraFlags2.milkBarMilk = 1;
        break;
    case 1:
        gMmExtraFlags2.milkBarChateau = 1;
        break;
    }
}
