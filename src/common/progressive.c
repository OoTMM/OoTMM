#include <combo.h>

static s32 progressiveStrength(void)
{
    switch (gOotSave.upgrades.strength)
    {
    case 0:
        return GI_OOT_GORON_BRACELET;
    case 1:
        return GI_OOT_SILVER_GAUNTLETS;
    default:
        return GI_OOT_GOLDEN_GAUNTLETS;
    }
}

static s32 progressiveDive(void)
{
    if (gOotSave.upgrades.dive)
        return GI_OOT_GOLDEN_SCALE;
    return GI_OOT_SILVER_SCALE;
}

s32 comboProgressiveChestItem(s32 gi)
{
#if defined(GAME_MM)
    gi ^= 0x100;
#endif

    switch (gi)
    {
    /* Items */
    case GI_OOT_FAIRY_OCARINA:
    case GI_OOT_OCARINA_OF_TIME:
        /* Ocarina */
        if (gOotSave.inventory[7] == ITEM_NONE)
            gi = GI_OOT_FAIRY_OCARINA;
        else
            gi = GI_OOT_OCARINA_OF_TIME;
        break;

    /* Equipment */
    case GI_OOT_GORON_BRACELET:
    case GI_OOT_SILVER_GAUNTLETS:
    case GI_OOT_GOLDEN_GAUNTLETS:
        gi = progressiveStrength();
        break;
    case GI_OOT_SILVER_SCALE:
    case GI_OOT_GOLDEN_SCALE:
        gi = progressiveDive();
        break;
    default:
        break;
    }

#if defined(GAME_MM)
    gi ^= 0x100;
#endif

    return gi;
}
