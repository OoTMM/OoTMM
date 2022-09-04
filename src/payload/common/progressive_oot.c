#include <combo.h>

static s32 progressiveBombBag(void)
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

static s32 progressiveBow(void)
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
        return GI_OOT_SLINGSHOT;
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

static s32 progressiveOcarina(void)
{
    if (gOotExtraItems.ocarina == 0)
        return GI_OOT_OCARINA_FAIRY;
    return GI_OOT_OCARINA_TIME;
}

static s32 progressiveHookshot(void)
{
    if (gOotExtraItems.hookshot == 0)
        return GI_OOT_HOOKSHOT;
    return GI_OOT_LONGSHOT;
}

static s32 progressiveSword(void)
{
    if (!(gOotSave.equipment.swords & EQ_OOT_SWORD_KOKIRI))
        return GI_OOT_SWORD_KOKIRI;
    if (!(gOotSave.equipment.swords & EQ_OOT_SWORD_MASTER))
        return GI_OOT_MASTER_SWORD;
    if (!(gOotSave.equipment.swords & (EQ_OOT_SWORD_KNIFE | EQ_OOT_SWORD_KNIFE_BROKEN)))
        return GI_OOT_GIANT_KNIFE;
    return GI_OOT_SWORD_BIGGORON;
}

/* We use an extra field to know which shields we got from shops */
static s32 progressiveShield(void)
{
    if (!(gOotExtraItems.shield & EQ_OOT_SHIELD_DEKU))
        return GI_OOT_DEKU_SHIELD;
    if (!(gOotExtraItems.shield & EQ_OOT_SHIELD_HYLIAN))
        return GI_OOT_HYLIAN_SHIELD;
    return GI_OOT_MIRROR_SHIELD;
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

static s32 progressiveWallet(void)
{
    if (gOotSave.upgrades.wallet)
        return GI_OOT_WALLET3;
    return GI_OOT_WALLET2;
}

static s32 progressiveMagic(void)
{
    if (gOotSave.magicUpgrade)
        return GI_OOT_MAGIC_UPGRADE2;
    return GI_OOT_MAGIC_UPGRADE;
}

static s32 progressiveUpgradeNut(void)
{
    if (gOotSave.upgrades.dekuNut < 2)
        return GI_OOT_NUT_UPGRADE;
    return GI_OOT_NUT_UPGRADE2;
}

static s32 progressiveUpgradeStick(void)
{
    if (gOotSave.upgrades.dekuStick < 2)
        return GI_OOT_STICK_UPGRADE;
    return GI_OOT_STICK_UPGRADE2;
}

static int isItemUnavailable(s32 gi)
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

s32 comboProgressiveOot(s32 gi)
{
    switch (gi)
    {
    /* Items */
    case GI_OOT_BOMB_BAG:
    case GI_OOT_BOMB_BAG2:
    case GI_OOT_BOMB_BAG3:
        gi = progressiveBombBag();
        break;
    case GI_OOT_FAIRY_BOW:
    case GI_OOT_QUIVER2:
    case GI_OOT_QUIVER3:
        gi = progressiveBow();
        break;
    case GI_OOT_SLINGSHOT:
    case GI_OOT_BULLET_BAG:
    case GI_OOT_BULLET_BAG2:
    case GI_OOT_BULLET_BAG3:
        gi = progressiveSlingshot();
        break;
    case GI_OOT_OCARINA_FAIRY:
    case GI_OOT_OCARINA_TIME:
        gi = progressiveOcarina();
        break;
    case GI_OOT_HOOKSHOT:
    case GI_OOT_LONGSHOT:
        gi = progressiveHookshot();
        break;
    /* Equipment */
    case GI_OOT_SWORD_KOKIRI:
    case GI_OOT_MASTER_SWORD:
    case GI_OOT_SWORD_BIGGORON:
    case GI_OOT_GIANT_KNIFE:
        gi = progressiveSword();
        break;
    case GI_OOT_DEKU_SHIELD:
    case GI_OOT_HYLIAN_SHIELD:
    case GI_OOT_MIRROR_SHIELD:
        gi = progressiveShield();
        break;
    /* Upgrades */
    case GI_OOT_GORON_BRACELET:
    case GI_OOT_SILVER_GAUNTLETS:
    case GI_OOT_GOLDEN_GAUNTLETS:
        gi = progressiveStrength();
        break;
    case GI_OOT_SILVER_SCALE:
    case GI_OOT_GOLDEN_SCALE:
        gi = progressiveDive();
        break;
    case GI_OOT_WALLET2:
    case GI_OOT_WALLET3:
        gi = progressiveWallet();
        break;
    case GI_OOT_MAGIC_UPGRADE:
    case GI_OOT_MAGIC_UPGRADE2:
        gi = progressiveMagic();
        break;
    case GI_OOT_NUT_UPGRADE:
    case GI_OOT_NUT_UPGRADE2:
        gi = progressiveUpgradeNut();
        break;
    case GI_OOT_STICK_UPGRADE:
    case GI_OOT_STICK_UPGRADE2:
        gi = progressiveUpgradeStick();
        break;
    default:
        break;
    }

    if (isItemUnavailable(gi))
        gi = GI_OOT_RUPEE_BLUE;

    return gi;
}
