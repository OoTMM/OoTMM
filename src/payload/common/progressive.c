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

static s32 progressiveOotOcarina(void)
{
    if (gComboSave.ootOcarinas == 0)
        return GI_OOT_OCARINA_FAIRY;
    return GI_OOT_OCARINA_TIME;
}

static s32 progressiveHookshot(void)
{
    if (gComboSave.ootHookshots == 0)
        return GI_OOT_HOOKSHOT;
    return GI_OOT_LONGSHOT;
}

static s32 progressiveOotSword(void)
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
static s32 progressiveOotShield(void)
{
    if (!(gComboSave.ootShields & EQ_OOT_SHIELD_DEKU))
        return GI_OOT_DEKU_SHIELD;
    if (!(gComboSave.ootShields & EQ_OOT_SHIELD_HYLIAN))
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

static s32 progressiveOotWallet(void)
{
    if (gOotSave.upgrades.wallet)
        return GI_OOT_WALLET3;
    return GI_OOT_WALLET2;
}

static s32 progressiveOotMagic(void)
{
    if (gOotSave.magicUpgrade)
        return GI_OOT_MAGIC_UPGRADE2;
    return GI_OOT_MAGIC_UPGRADE;
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
    case GI_OOT_SLINGSHOT:
    case GI_OOT_BULLET_BAG:
    case GI_OOT_BULLET_BAG2:
    case GI_OOT_BULLET_BAG3:
        gi = progressiveSlingshot();
        break;
    case GI_OOT_OCARINA_FAIRY:
    case GI_OOT_OCARINA_TIME:
        gi = progressiveOotOcarina();
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
        gi = progressiveOotSword();
        break;
    case GI_OOT_DEKU_SHIELD:
    case GI_OOT_HYLIAN_SHIELD:
    case GI_OOT_MIRROR_SHIELD:
        gi = progressiveOotShield();
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
        gi = progressiveOotWallet();
        break;
    case GI_OOT_MAGIC_UPGRADE:
    case GI_OOT_MAGIC_UPGRADE2:
        gi = progressiveOotMagic();
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
