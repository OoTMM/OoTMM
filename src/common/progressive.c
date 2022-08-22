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

static s32 progressiveOotBow(void)
{
    if (gOotSave.inventory[ITS_OOT_BOW] == ITEM_NONE)
        return GI_OOT_FAIRY_BOW;
    switch (gOotSave.upgrades.quiver)
    {
    case 0:
    case 1:
        return GI_OOT_QUIVER2;
    default:
        return GI_OOT_QUIVER3;
    }
}

static s32 progressiveSlingshot(void)
{
    if (gOotSave.inventory[ITS_OOT_SLINGSHOT] == ITEM_NONE)
        return GI_OOT_FAIRY_SLINGSHOT;
    switch (gOotSave.upgrades.bulletBag)
    {
    case 0:
        return GI_OOT_BULLET_BAG; /* Not used */
    case 1:
        return GI_OOT_BULLET_BAG2;
    default:
        return GI_OOT_BULLET_BAG3;
    }
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
        if (gOotSave.inventory[ITS_OOT_OCARINA] == ITEM_NONE)
            gi = GI_OOT_FAIRY_OCARINA;
        else
            gi = GI_OOT_OCARINA_OF_TIME;
        break;
    case GI_OOT_FAIRY_BOW:
    case GI_OOT_QUIVER2:
    case GI_OOT_QUIVER3:
        gi = progressiveOotBow();
        break;
    case GI_OOT_FAIRY_SLINGSHOT:
    case GI_OOT_BULLET_BAG:
    case GI_OOT_BULLET_BAG2:
    case GI_OOT_BULLET_BAG3:
        gi = progressiveSlingshot();
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
