#include <combo.h>

#define DEFAULT_GI_OOT  GI_OOT_RUPEE_BLUE
#define DEFAULT_GI_MM   GI_MM_RUPEE_BLUE

static s32 progressiveOotBombBag(void)
{
    if (gOotSave.inventory[ITS_OOT_BOMBS] == ITEM_NONE)
        return GI_OOT_BOMB_BAG;
    switch (gOotSave.upgrades.bombBag)
    {
    case 0:
        return GI_OOT_BOMB_BAG; /* Redundent */
    case 1:
        return GI_OOT_BOMB_BAG2;
    default:
        return GI_OOT_BOMB_BAG3;
    }
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

static s32 progressiveOotOcarina(void)
{
    if (gComboSave.ootOcarinas == 0)
        return GI_OOT_FAIRY_OCARINA;
    return GI_OOT_OCARINA_OF_TIME;
}

static s32 progressiveHookshot(void)
{
    if (gComboSave.ootHookshots == 0)
        return GI_OOT_HOOKSHOT;
    return GI_OOT_LONGSHOT;
}

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

static int isItemUnavailableOot(s32 gi)
{
    switch (gi)
    {
    case GI_OOT_BOMB:
    case GI_OOT_BOMBS_5:
    case GI_OOT_BOMBS_10:
    case GI_OOT_BOMBS_20:
    case GI_OOT_BOMBS_30:
        return gOotSave.upgrades.bombBag == 0;
    case GI_OOT_ARROWS_5:
    case GI_OOT_ARROWS_10:
    case GI_OOT_ARROWS_30:
        return gOotSave.upgrades.quiver == 0;
    case GI_OOT_DEKU_SEEDS_5:
    case GI_OOT_DEKU_SEEDS_30:
        return gOotSave.upgrades.bulletBag == 0;
    default:
        return 0;
    }
}

static s32 progressiveChestItemOot(s32 gi)
{
    switch (gi)
    {
    /* Items */
    case GI_OOT_BOMB_BAG:
    case GI_OOT_BOMB_BAG2:
    case GI_OOT_BOMB_BAG3:
        gi = progressiveOotBombBag();
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
    case GI_OOT_FAIRY_OCARINA:
    case GI_OOT_OCARINA_OF_TIME:
        gi = progressiveOotOcarina();
        break;
    case GI_OOT_HOOKSHOT:
    case GI_OOT_LONGSHOT:
        gi = progressiveHookshot();
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

    if (isItemUnavailableOot(gi))
        gi = DEFAULT_GI_OOT;

    return gi;
}

static s32 progressiveChestItemMm(s32 gi)
{
    return gi;
}

s32 comboProgressiveChestItem(s32 gi)
{
#if defined(GAME_MM)
    gi ^= 0x100;
#endif

    if (gi & 0x100)
    {
        gi = 0x100 | progressiveChestItemMm(gi & 0xff);
    }
    else
    {
        gi = progressiveChestItemOot(gi);
    }

#if defined(GAME_MM)
    gi ^= 0x100;
#endif

    return gi;
}
